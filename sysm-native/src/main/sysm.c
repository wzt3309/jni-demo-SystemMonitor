//
// Created by wzt on 11/7/17.
//

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/param.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <time.h>
#include <mntent.h>
#include <utmp.h>
#include <net/if_arp.h>
#include <netinet/in.h>


#include "sysm.h"
#include "sysm_util.h"

#define PROC_FS_ROOT "/proc/"

#define SYS_BLOCK "/sys/block"
#define PROC_DISKSTATS    PROC_FS_ROOT "diskstats"
#define PROC_PARTITIONS   PROC_FS_ROOT "partitions"


#define PROC_STAT         PROC_FS_ROOT "stat"
#define PROC_MEMINFO      PROC_FS_ROOT "meminfo"
#define PROC_VMSTAT       PROC_FS_ROOT "vmstat"
#define PROC_MTRR         PROC_FS_ROOT "mtrr"
#define PROC_LOADAVG      PROC_FS_ROOT "loadavg"
#define PROC_UPTIME       PROC_FS_ROOT "uptime"

#define PROC_PSTAT   "/stat"
#define PROC_PSTATUS "/status"

#define PID_CACHE_CLEANUP_PERIOD 1000*60*10 /* 10 minutes */
#define PID_CACHE_ENTRY_EXPIRE_PERIOD 1000*60*20 /* 20 minutes */

#define pageshift(x) ((x) << sysm->pagesize)

#define HEX_ENT_LEN 8
static unsigned int hex2int(const char *x, int len)
{
    int i;
    unsigned int j;

    for (i=0, j=0; i<len; i++) {
        register int ch = x[i];
        j <<= 4;
        if (isdigit(ch)) {
            j |= ch - '0';
        }
        else if (isupper(ch)) {
            j |= ch - ('A' - 10);
        }
        else {
            j |= ch - ('a' - 10);
        }
    }

    return j;
}

static int sysm_boot_time_get(sysm_t *sysm)
{
    FILE *fp;
    char buffer[BUFSIZ], *ptr;
    int found = 0;

    if (!(fp = fopen(PROC_STAT, "r")))
        return errno;

    while ((ptr = fgets(buffer, sizeof(buffer), fp)))
    {
        if (strncmp(ptr, "btime", 5) == 0)
        {
            if ((ptr = sysm_skip_token(ptr)))
            {
                sysm->boot_time = strtoul(ptr, &ptr, 10);
                found = 1;
            }
        }
        break;
    }

    fclose(fp);

    if (!found)
        sysm->boot_time = 0;

    return SYSM_OK;
}



int sysm_open(sysm_t **sysm)
{
    int status = sysm_os_open(sysm);

    if (status == SYSM_OK)
    {
        (*sysm)->cpu_list_cores = 0;
        (*sysm)->pid = 0;
        (*sysm)->ifconf_buf = NULL;
        (*sysm)->ifconf_len = 0;
        (*sysm)->self_path = NULL;
        (*sysm)->pids = NULL;
        (*sysm)->proc_cpu = NULL;
        (*sysm)->fsdev = NULL;
        (*sysm)->net_listen = NULL;
    }

    return status;
}

int sysm_close(sysm_t *sysm)
{
    if (sysm->ifconf_buf)
        free(sysm->ifconf_buf);
    if (sysm->self_path)
        free(sysm->self_path);
    if (sysm->pids)
    {
        sysm_proc_list_destroy(sysm->pids);
        free(sysm->pids);
    }
    if (sysm->proc_cpu)
    {
        sysm_cache_destroy(sysm->proc_cpu);
    }
    if (sysm->fsdev) {
        sysm_cache_destroy(sysm->fsdev);
    }
    if (sysm->net_listen)
    {
        sysm_cache_destroy(sysm->net_listen);
    }

    return sysm_os_close(sysm);
}

int sysm_os_open(sysm_t **sysm)
{
    int i, status;
    int kernel_rev, has_nptl;
    struct stat sb;
    struct utsname name;

    *sysm = malloc(sizeof(**sysm));

    (*sysm)->pagesize = 0;
    i = getpagesize();
    while ((i >>= 1) > 0)
    {
        (*sysm)->pagesize++;
    }

    status = sysm_boot_time_get(*sysm);
    if (status != SYSM_OK)
        return status;

    (*sysm)->ticks = sysconf(_SC_CLK_TCK);

    (*sysm)->mtrr = -1;

    (*sysm)->proc_signal_offset = -1;

    (*sysm)->last_proc_stat.pid = -1;

    (*sysm)->lcpu = -1;

    if (stat(PROC_DISKSTATS, &sb) == 0)
        (*sysm)->iostat = IOSTAT_DISKSTATS;
    else if (stat(SYS_BLOCK, &sb) == 0)
        (*sysm)->iostat = IOSTAT_SYS;
    else if (stat(PROC_PARTITIONS, &sb) == 0)
        (*sysm)->iostat = IOSTAT_PARTITIONS;
    else
        (*sysm)->iostat = IOSTAT_NONE;

    (*sysm)->proc_net = NULL;

    uname(&name);

    //since kernel 2.6 linux has Native POSIX Thread Library（NPTL）
    kernel_rev = atoi(&name.release[2]);
    if (kernel_rev >= 6)
        has_nptl = 1;
    else
        has_nptl = 0;

    (*sysm)->has_nptl = has_nptl;

    return SYSM_OK;
}

int sysm_os_close(sysm_t *sysm)
{
    if (sysm->proc_net)
        free(sysm->proc_net);
    free(sysm);

    return SYSM_OK;
}

int sysm_proc_list_get(sysm_t *sysm, sysm_proc_list_t *proc_list)
{
    if (proc_list == NULL)
    {
        //re-use cache
        if (sysm->pids == NULL)
        {
            sysm->pids = malloc(sizeof(*(sysm->pids)));
            sysm_proc_list_create(sysm->pids);
        }
        else
        {
            sysm->pids->number = 0;
        }
        proc_list = sysm->pids;
    }
    else
    {
        sysm_proc_list_create(proc_list);
    }

    return sysm_os_proc_list_get(sysm, proc_list);
}

int sysm_proc_list_create(sysm_proc_list_t *proc_list)
{
    proc_list->number = 0;
    proc_list->size = SYSM_PROC_LIST_MAX;
    proc_list->data = malloc(sizeof(*(proc_list->data)) * proc_list->size);

    return SYSM_OK;
}

int sysm_proc_list_grow(sysm_proc_list_t *proc_list)
{
    proc_list->data = realloc(proc_list->data,
                              sizeof(*(proc_list->data)) *
                                      (proc_list->size + SYSM_PROC_LIST_MAX));
    proc_list->size += SYSM_PROC_LIST_MAX;

    return SYSM_OK;
}

int sysm_proc_list_destroy(sysm_proc_list_t *proc_list)
{
    if (proc_list->size)
    {
        free(proc_list->data);
        proc_list->number = proc_list->size = 0;
    }

    return SYSM_OK;
}

int sysm_proc_args_get(sysm_t *sysm, pid_t pid, sysm_proc_args_t *proc_args)
{
    int status;
    sysm_proc_args_create(proc_args);
    status = sysm_os_proc_args_get(sysm, pid, proc_args);
    if (status != SYSM_OK)
        sysm_proc_args_destroy(proc_args);
    return status;
}

int sysm_proc_args_create(sysm_proc_args_t *proc_args)
{
    proc_args->number = 0;
    proc_args->size = SYSM_PROC_ARGS_MAX;
    proc_args->data = malloc(sizeof(*(proc_args->data)) *
                                     proc_args->size);

    return SYSM_OK;
}

int sysm_proc_args_grow(sysm_proc_args_t *proc_args)
{
    proc_args->data = realloc(proc_args->data,
                              sizeof(*(proc_args->data)) *
                                      (proc_args->size + SYSM_PROC_ARGS_MAX));
    proc_args->size += SYSM_PROC_ARGS_MAX;
    return SYSM_OK;
}

int sysm_proc_args_destroy(sysm_proc_args_t *proc_args)
{
    if (proc_args->size)
    {
        for (int i = 0;i < proc_args->number;i++)
        {
            free(proc_args->data[i]);
        }
        free(proc_args->data);
        proc_args->number = proc_args->size = 0;
    }
    return SYSM_OK;
}

int sysm_os_proc_args_get(sysm_t *sysm, pid_t pid, sysm_proc_args_t *proc_args)
{
    char buffer[BUFSIZ], *buf = NULL, *ptr;
    int fd, len, total = 0;

    SYSM_PROC_FILENAME(buffer, pid, "/cmdline");

    if ((fd = open(buffer, O_RDONLY)) < 0)
    {
        if (errno == ENOENT)
            return ESRCH;
        return errno;
    }

    buffer[0] = '\0';

    while ((len = (int) read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        if (len == 0) break;
        buf = realloc(buf, (size_t) (total + len + 1));
        memcpy(buf+total, buffer, (size_t) len);
        total += len;
    }

    close(fd);

    if (total == 0)
    {
        proc_args->number = 0;
        return SYSM_OK;
    }

    buf[total] = '\0';
    ptr = buf;

    while (total > 0)
    {
        int alen = (int) (strlen(ptr) + 1);
        char *arg = malloc((size_t) alen);

        SYSM_PROC_ARGS_GROW(proc_args);
        memcpy(arg, ptr, (size_t) alen);

        proc_args->data[proc_args->number++] = arg;

        total -= alen;
        if (total > 0) ptr += alen;
    }

    free(buf);

    return SYSM_OK;


}

static void convert_hex_address(sysm_net_address_t *address, char *ptr, int len)
{
    if (len > HEX_ENT_LEN)
    {
        for (int i = 0;i <= 3;i++, ptr += HEX_ENT_LEN)
            address->addr.in6[i] = hex2int(ptr, HEX_ENT_LEN);

        address->family = SYSM_AF_INET6;
    }
    else
    {
        address->addr.in = (len == HEX_ENT_LEN) ? hex2int(ptr, HEX_ENT_LEN) : 0;
        address->family = SYSM_AF_INET;
    }
}

typedef struct {
    sysm_net_connection_list_t *conn_list;
    sysm_net_connection_t *conn;
    unsigned long port;
} net_conn_getter_t;

static int proc_net_walker(sysm_net_connection_walker_t *walker,
                           sysm_net_connection_t *conn)
{
    net_conn_getter_t *getter = (net_conn_getter_t *)walker->data;

    if (getter->conn_list)
    {
        SYSM_NET_CONNLIST_GROW(getter->conn_list);
        memcpy(&getter->conn_list->data[getter->conn_list->number++],
               conn, sizeof(*conn));
    }
    else
    {
        if ((getter->port == conn->lo_port)
            && (conn->re_port == 0))
        {
            memcpy(getter->conn, conn, sizeof(*conn));
            return !SYSM_OK;
        }
    }

    return SYSM_OK;
}

#define SKIP_WHILE(p, c) while (*p == c) p++
#define SKIP_PAST(p, c) \
   while(*p && (*p != c)) p++; \
   SKIP_WHILE(p, c)

typedef struct {
    FILE *fp;
    int (*close)(FILE *);
} xproc_t;

static FILE *xproc_open(const char *command, xproc_t *xproc)
{
    struct stat sb;
    if (stat(command, &sb) == 0)
    {
        if (sb.st_mode &S_IXUSR)
        {
            xproc->fp = popen(command, "r");
            xproc->close = pclose;
        }
        else
        {
            xproc->fp = fopen(command, "r");
            xproc->close = fclose;
        }
        return xproc->fp;
    }
    return NULL;
}

static int proc_net_read(sysm_net_connection_walker_t *walker, const char *fname, int type)
{
    FILE *fp = NULL;
    char buffer[8192];
    sysm_t *sysm = walker->sysm;
    char *ptr = sysm->proc_net;
    int flags = walker->flags;
    xproc_t xproc = {NULL, fclose};

    if (ptr)
    {
        snprintf(buffer, sizeof(buffer), "%s/%s", ptr, fname + sizeof(PROC_FS_ROOT) - 1);
        fp = xproc_open(buffer, &xproc);
    }

    if (!(fp || (fp = fopen(fname, "r"))))
        return errno;

    fgets(buffer, sizeof(buffer), fp);/*skip first line*/

    while ((ptr = fgets(buffer, sizeof(buffer), fp)))
    {
        sysm_net_connection_t conn;
        char *laddr, *raddr;
        int laddr_len = 0, raddr_len = 0;
        int more;

        SKIP_WHILE(ptr, ' ');
        /*skip "%d: "*/
        SKIP_PAST(ptr, ' ');

        laddr = ptr;
        while (*ptr && (*ptr != ':'))
        {
            laddr_len++;
            ptr++;
        }
        SKIP_WHILE(ptr, ':');

        conn.lo_port = (strtoul(ptr, &ptr, 16) & 0xffff);

        SKIP_WHILE(ptr, ' ');

        raddr = ptr;
        while (*ptr && (*ptr != ':'))
        {
            raddr_len++;
            ptr++;
        }
        SKIP_WHILE(ptr, ':');

        conn.re_port = (strtoul(ptr, &ptr, 16) & 0xffff);

        SKIP_WHILE(ptr, ' ');

        if (!((conn.re_port && (flags & SYSM_NETCONN_CLIENT)) ||
              (!conn.re_port && (flags & SYSM_NETCONN_SERVER))))
        {
            continue;
        }

        conn.type = type;

        convert_hex_address(&conn.lo_address, laddr, laddr_len);
        convert_hex_address(&conn.re_address, raddr, raddr_len);

        conn.state = hex2int(ptr, 2);
        ptr += 2;
        SKIP_WHILE(ptr, ' ');

        conn.se_q = hex2int(ptr, HEX_ENT_LEN);
        ptr += HEX_ENT_LEN + 1; /*tx + ':'*/

        conn.re_q = hex2int(ptr, HEX_ENT_LEN);
        ptr += HEX_ENT_LEN;
        SKIP_WHILE(ptr, ' ');

        SKIP_PAST(ptr, ' ');
        SKIP_PAST(ptr, ' ');

        conn.uid = (pid_t) strtoul(ptr, &ptr, 10);

        SKIP_WHILE(ptr, ' ');
        SKIP_PAST(ptr, ' ');

        conn.inode = strtoul(ptr, &ptr, 10);

        more = walker->add_connection(walker, &conn);
        if (more != SYSM_OK)
        {
            xproc.close(fp);
            return SYSM_OK;
        }

    }

    xproc.close(fp);

    return SYSM_OK;
}

static int sysm_net_connection_get(sysm_t *sysm, sysm_net_connection_t *netconn, unsigned long port, int flags)
{
    int status;
    sysm_net_connection_walker_t walker;
    net_conn_getter_t getter;

    getter.conn = netconn;
    getter.conn_list = NULL;
    getter.port = port;

    walker.sysm = sysm;
    walker.flags = flags;
    walker.data = &getter;
    walker.add_connection = proc_net_walker;

    status = sysm_net_connection_walk(&walker);

    return status;
}

int sysm_proc_port_get(sysm_t *sysm, int protocol, unsigned long port, pid_t *pid)
{
    int status;
    sysm_net_connection_t netconn;
    DIR *dirp;
    struct dirent *ent, dbuf;

    SYSM_ZERO(&netconn);
    *pid = 0;

    status = sysm_net_connection_get(sysm, &netconn, port, SYSM_NETCONN_SERVER | protocol);

    if (status != SYSM_OK) return status;

    if (netconn.lo_port != port) return SYSM_OK;

    if (!(dirp = opendir(PROC_FS_ROOT))) return errno;

    while (readdir_r(dirp, &dbuf, &ent) == 0)
    {
        DIR *fd_dirp;
        struct dirent *fd_ent, fd_dbuf;
        struct stat sb;
        char fd_name[BUFSIZ], pid_name[BUFSIZ];
        int len, slen;

        if (ent == NULL) {
            break;
        }

        if (!isdigit(*ent->d_name)) continue;

        len = (int) strlen(PROC_FS_ROOT);
        memcpy(&pid_name[0], PROC_FS_ROOT, strlen(PROC_FS_ROOT));
        pid_name[len++] = '/';

        slen = (int) strlen(ent->d_name);
        memcpy(&pid_name[len], ent->d_name, strlen(ent->d_name));
        len += slen;
        pid_name[len] = '\0';

        if (stat(pid_name, &sb) < 0) {
            continue;
        }
        if (sb.st_uid != netconn.uid) {
            continue;
        }

        memcpy(&fd_name[0], pid_name, (size_t) len);
        memcpy(&fd_name[len], "/fd", 3);
        fd_name[len+=3] = '\0';

        if (!(fd_dirp = opendir(fd_name))) {
            continue;
        }

        while (readdir_r(fd_dirp, &fd_dbuf, &fd_ent) == 0) {
            char fd_ent_name[BUFSIZ];

            if (fd_ent == NULL) {
                break;
            }

            if (!isdigit(*ent->d_name)) continue;

            slen = (int) strlen(fd_ent->d_name);
            memcpy(&fd_ent_name[0], fd_name, len);
            fd_ent_name[len] = '/';
            memcpy(&fd_ent_name[len + 1], fd_ent->d_name, slen);
            fd_ent_name[len + 1 + slen] = '\0';

            if (stat(fd_ent_name, &sb) < 0) {
                continue;
            }

            if (sb.st_ino == netconn.inode) {
                closedir(fd_dirp);
                closedir(dirp);
                *pid = (pid_t) strtoul(ent->d_name, NULL, 10);
                return SYSM_OK;
            }
        }
        closedir(fd_dirp);
    }
    closedir(dirp);

    return SYSM_OK;
}

#define PROC_SIGNAL_IX 38

static int get_proc_signal_offset()
{
    char buffer[BUFSIZ], *ptr = buffer;
    int fields = 0;
    int status = sysm_file2str(PROC_FS_ROOT "self/stat", buffer, sizeof(buffer));

    if (status != SYSM_OK) return 1;

    while (*ptr)
    {
        if (*ptr++ == ' ')
            fields++;
    }

    return (fields - PROC_SIGNAL_IX) + 1;
}

static int proc_isthread(sysm_t *sysm, char *pidstr, int len)
{
    char buffer[BUFSIZ], *ptr = buffer;
    int fd, n, offset = sysm->proc_signal_offset;

    memcpy(ptr, PROC_FS_ROOT, strlen(PROC_FS_ROOT));
    ptr += strlen(PROC_FS_ROOT);

    memcpy(ptr, pidstr, (size_t) len);
    ptr += len;

    memcpy(ptr, PROC_PSTAT, strlen(PROC_PSTAT));
    ptr += strlen(PROC_PSTAT);

    *ptr = '\0';

    if ((fd = open(buffer, O_RDONLY)) < 0) return 0;

    n = (int) read(fd, buffer, sizeof(buffer));
    close(fd);

    if (n < 0) return 0;

    buffer[n--] = '\0';

    /* skip trailing crap */
    while ((n > 0) && !isdigit(buffer[n--])) ;

    while (offset-- > 0) {
        /* skip last field */
        while ((n > 0) && isdigit(buffer[n--])) ;

        /* skip whitespace */
        while ((n > 0) && !isdigit(buffer[n--])) ;
    }

    if (n < 3) {
        return 0; /* hulk smashed /proc? */
    }

    ptr = &buffer[n];
    /*
     * '17' == SIGCHLD == real process.
     * '33' and '0' are threads
     */
    if ((*ptr++ == '1') &&
        (*ptr++ == '7') &&
        (*ptr++ == ' '))
    {
        return 0;
    }

    return 1;
}

int sysm_os_proc_list_get(sysm_t *sysm, sysm_proc_list_t *proc_list)
{
    DIR *dirp = opendir(PROC_FS_ROOT);
    struct dirent *ent, dbuf;
    register const int threadbadhack = !sysm->has_nptl;

    if (!dirp) return errno;

    if (threadbadhack && (sysm->proc_signal_offset == -1))
        sysm->proc_signal_offset = get_proc_signal_offset();

    while (readdir_r(dirp, &dbuf, &ent) == 0)
    {
        if (!ent) break;

        if (!isdigit(*ent->d_name)) continue;

        if (threadbadhack
            && proc_isthread(sysm, ent->d_name, (int) strlen(ent->d_name)))
            continue;

        SYSM_PROC_LIST_GROW(proc_list);

        proc_list->data[proc_list->number++] = (pid_t) strtoul(ent->d_name, NULL, 10);
    }

    closedir(dirp);
    return SYSM_OK;
}

int sysm_proc_signal(pid_t pid, int signum)
{
    if (kill(pid, signum) == -1)
        return errno;
    return SYSM_OK;
}

pid_t sysm_pid_get(sysm_t *sysm)
{
    sysm->pid = getpid();
    return sysm->pid;
}

static char *cpu_info_strval(char *ptr)
{
    if ((ptr = strchr(ptr, ':')))
    {
        ptr++;
        while(isspace(*ptr)) ptr++;
        return ptr;
    }
    return NULL;
}

static void cpu_info_strcpy(char *ptr, char *buf, size_t len)
{
    size_t slen;
    ptr = cpu_info_strval(ptr);
    if (!ptr) return;
    slen = strlen(ptr);
    strncpy(buf, ptr, len);
    buf[len] = '\0';
    if (slen < len)
        buf[slen - 1] = '\0'; /*make \n to \0*/
}

static int get_cpu_info(sysm_cpu_info_t *info, FILE *fp)
{
    char buffer[BUFSIZ], *ptr;
    int found = 0;

    info->mhz = 0;
    info->cache_size = 0;

    while ((ptr = fgets(buffer, sizeof(buffer), fp)))
    {
        switch (*ptr)
        {
            case 'p':
                /* processor	: 0 */
                if (strncmp(ptr, "processor", 9) == 0) found = 1;
                break;
            case 'v':
                /* "vendor_id" or "vendor" */
                if (strncmp(ptr, "vendor", 6) == 0)
                {
                    cpu_info_strcpy(ptr, info->vendor, sizeof(info->vendor));
                    if (strcmp(info->vendor, "GenuineIntel") == 0)
                    {
                        strncpy(info->vendor, "Intel", sizeof(info->vendor));
                        info->vendor[sizeof(info->vendor)] = '\0';
                    }
                    else if (strcmp(info->vendor, "AuthenticAMD") == 0)
                    {
                        strncpy(info->vendor, "AMD", sizeof(info->vendor));
                        info->vendor[sizeof(info->vendor)] = '\0';
                    }

                }
                break;
            case 'f':
                if (strncmp(ptr, "family", 6) == 0)
                {
                    /* IA64 version of "model name" is in this place*/
                    cpu_info_strcpy(ptr, info->model, sizeof(info->model));
                    sysm_cpu_model_adjust(info);
                }
                break;
            case 'm':
                if (strncmp(ptr, "model name", 10) == 0)
                {
                    cpu_info_strcpy(ptr, info->model, sizeof(info->model));
                    sysm_cpu_model_adjust(info);
                }
                break;
            case 'c':
                if (strncmp(ptr, "cpu MHz", 7) == 0)
                {
                    ptr = cpu_info_strval(ptr);
                    info->mhz = (int)strtol(ptr, &ptr, 10);
                }
                else if (strncmp(ptr, "cache size", 10) == 0)
                {
                    ptr = cpu_info_strval(ptr);
                    info->cache_size = (sysm_uint64_t)strtol(ptr, &ptr, 10);
                }
                break;
            case '\n':
                /* lone \n means end of info for this processor */
                return found;
            default:
                continue;
        }
    }

    return found;
}

static void get_cpuinfo_max_freq(sysm_cpu_info_t *cpu_info, /*num is cpu number*/ int num)
{
    int status;
    char max_freq[PATH_MAX];
    snprintf(max_freq, sizeof(max_freq),
             "/sys/devices/system/cpu/cpu%d"
                     "/cpufreq/cpuinfo_max_freq", num);

    status = sysm_file2str(max_freq, max_freq, sizeof(max_freq) - 1);

    if (status == SYSM_OK)
        cpu_info->mhz_max = atoi(max_freq) / 1000;
    else
        cpu_info->mhz_max = -1;
}

static void get_cpuinfo_mim_freq(sysm_cpu_info_t *cpu_info, int num)
{
    int status;
    char min_freq[PATH_MAX];
    snprintf(min_freq, sizeof(min_freq),
             "/sys/devices/system/cpu/cpu%d"
                     "/cpufreq/cpuinfo_min_freq", num);
    status = sysm_file2str(min_freq, min_freq, sizeof(min_freq) - 1);

    if (status == SYSM_OK)
        cpu_info->mhz_min = atoi(min_freq) / 1000;
    else
        cpu_info->mhz_min = -1;
}

int sysm_cpu_info_list_get(sysm_t *sysm, sysm_cpu_info_list_t *cpu_infos)
{
    FILE *fp;
    int core_rollup = sysm_cpu_core_rollup(sysm), i = 0;

    if (!(fp = fopen(PROC_FS_ROOT "cpuinfo", "r")))
    {
        return errno;
    }

    sysm->ncpu = (unsigned int)sysconf(_SC_NPROCESSORS_CONF);
    sysm_cpu_info_list_create(cpu_infos);

    while(get_cpu_info(&cpu_infos->data[cpu_infos->number], fp))
    {
        sysm_cpu_info_t *info;

        if (core_rollup && (i++ % sysm->lcpu))
        {
            continue;   /* fold logical processors */
        }

        info = &cpu_infos->data[cpu_infos->number];
        get_cpuinfo_max_freq(info, (int)cpu_infos->number);
        get_cpuinfo_mim_freq(info, (int)cpu_infos->number);

        info->total_cores = sysm->ncpu;
        info->cores_per_socket = sysm->lcpu;
        info->total_sockets = sysm_cpu_socket_count(sysm);

        ++cpu_infos->number;
        SYSM_CPU_INFO_LIST_GROW(cpu_infos);

    }
    fclose(fp);

    return SYSM_OK;
}

int sysm_cpu_info_list_create(sysm_cpu_info_list_t *cpu_infos)
{
    cpu_infos ->number = 0;
    cpu_infos->size = SYSM_CPU_MAX;
    cpu_infos->data = malloc(sizeof(*(cpu_infos->data)) * cpu_infos->size);
    return SYSM_OK;
}

/*used in macro SYSM_CPU_INFO_LIST_GROW in sysm.h*/
int sysm_cpu_info_list_grow(sysm_cpu_info_list_t *cpu_infos)
{
    cpu_infos->data = realloc(cpu_infos->data,
                              sizeof(*(cpu_infos->data)) *
                                      (cpu_infos->size + SYSM_CPU_MAX));
    cpu_infos->size += SYSM_CPU_MAX;
    return SYSM_OK;
}

int sysm_cpu_info_list_destroy(sysm_cpu_info_list_t *cpu_infos)
{
    if (cpu_infos->size)
    {
        free(cpu_infos->data);
        cpu_infos->number = cpu_infos->size = 0;
    }

    return SYSM_OK;
}

static void get_cpu_metrics(sysm_t *sysm, sysm_cpu_t *cpu, char *line)
{
    char *ptr = sysm_skip_token(line);

    cpu->user += SYSM_TICK2MSEC(strtoull(ptr, &ptr, 10));
    cpu->nice += SYSM_TICK2MSEC(strtoull(ptr, &ptr, 10));
    cpu->sys += SYSM_TICK2MSEC(strtoull(ptr, &ptr, 10));
    cpu->idle += SYSM_TICK2MSEC(strtoull(ptr, &ptr, 10));

    if (*ptr == ' ')
    {
        cpu->wait += SYSM_TICK2MSEC(strtoull(ptr, &ptr, 10));
        cpu->hi += SYSM_TICK2MSEC(strtoull(ptr, &ptr, 10));
        cpu->si += SYSM_TICK2MSEC(strtoull(ptr, &ptr, 10));
    }
    if (*ptr == ' ')
        cpu->steal += SYSM_TICK2MSEC(strtoull(ptr, &ptr, 10));

    cpu->total =
        cpu->user + cpu->nice + cpu->sys + cpu->idle +
        cpu->wait + cpu->hi + cpu->si + cpu->steal;
}

int sysm_cpu_list_get(sysm_t *sysm, sysm_cpu_list_t *cpu_list)
{
    FILE *fp;
    char buffer[BUFSIZ], cpu_total[BUFSIZ], *ptr;
    int core_rollup = sysm_cpu_core_rollup(sysm), i = 0;
    sysm_cpu_t *cpu;

    if (!(fp = fopen(PROC_STAT, "r")))
        return errno;

    (void)fgets(cpu_total, sizeof(cpu_total), fp);

    sysm_cpu_list_create(cpu_list);

    /*start from cpu%d*/
    while((ptr = fgets(buffer, sizeof(buffer), fp)))
    {
        if (strncmp(ptr, "cpu", 3) != 0)
            break;
        if (core_rollup && (i % sysm->lcpu))
            cpu = &cpu_list->data[cpu_list->number - 1];
        else
        {
            SYSM_CPU_LIST_GROW(cpu_list);
            cpu = &cpu_list->data[cpu_list->number++];
            SYSM_ZERO(cpu);
        }

        get_cpu_metrics(sysm, cpu, ptr);

        i++;
    }

    fclose(fp);

    if (cpu_list->number == 0)
    {
        cpu = &cpu_list->data[cpu_list->number++];
        SYSM_ZERO(cpu);
        get_cpu_metrics(sysm, cpu, cpu_total);
    }

    return SYSM_OK;
}

int sysm_cpu_list_create(sysm_cpu_list_t *cpu_list)
{
    cpu_list->number = 0;
    cpu_list->size = SYSM_CPU_MAX;
    cpu_list->data = malloc(sizeof(*(cpu_list->data)) *
                                    cpu_list->size);

    return SYSM_OK;
}

/*used in macro SYSM_CPU_LIST_GROW in sysm.h*/
int sysm_cpu_list_grow(sysm_cpu_list_t *cpu_list)
{
    cpu_list->data = realloc(cpu_list->data,
                             sizeof(*(cpu_list->data)) *
                                     (cpu_list->size + SYSM_CPU_MAX));
    cpu_list->size += SYSM_CPU_MAX;

    return SYSM_OK;
}

int sysm_cpu_list_destroy(sysm_cpu_list_t *cpu_list)
{
    if (cpu_list->size)
    {
        free(cpu_list->data);
        cpu_list->number = cpu_list->size = 0;
    }

    return SYSM_OK;
}

int sysm_file_system_list_get(sysm_t *sysm, sysm_file_system_list_t *fslist)
{
    struct mntent ent;
    char buf[1025];
    FILE *fp;
    sysm_file_system_t *fsp;

    if (!(fp = setmntent(MOUNTED, "r"))) {
        return errno;
    }

    sysm_file_system_list_create(fslist);

    while (getmntent_r(fp, &ent, buf, sizeof(buf))) {
        SYSM_FILE_SYSTEM_LIST_GROW(fslist);

        fsp = &fslist->data[fslist->number++];

        fsp->type = SYSM_FSTYPE_UNKNOWN; /* unknown, will be set later */
        strcpy(fsp->dir_name, ent.mnt_dir);
        strcpy(fsp->dev_name, ent.mnt_fsname);
        strcpy(fsp->sys_type_name, ent.mnt_type);
        strcpy(fsp->options, ent.mnt_opts);
        sysm_fs_type_get(fsp);
    }

    endmntent(fp);

    return SYSM_OK;
}

int sysm_file_system_list_create(sysm_file_system_list_t *fslist)
{
    fslist->number = 0;
    fslist->size = SYSM_FS_MAX;
    fslist->data = malloc(sizeof(*(fslist->data)) *
                          fslist->size);
    return SYSM_OK;
}

int sysm_file_system_list_grow(sysm_file_system_list_t *fslist)
{
    fslist->data = realloc(fslist->data,
                           sizeof(*(fslist->data)) *
                           (fslist->size + SYSM_FS_MAX));
    fslist->size += SYSM_FS_MAX;

    return SYSM_OK;
}

int sysm_file_system_list_destroy(sysm_file_system_list_t *fslist)
{
    if (fslist->size) {
        free(fslist->data);
        fslist->number = fslist->size = 0;
    }

    return SYSM_OK;
}

#ifdef SYSM_64BIT
#define ROUTE_FMT "%16s %128s %128s %X %ld %ld %ld %128s %ld %ld %ld\n"
#else
#define ROUTE_FMT "%16s %128s %128s %X %lld %lld %lld %128s %lld %lld %lld\n"
#endif
#define RTF_UP 0x0001

int sysm_net_route_list_get(sysm_t *sysm, sysm_net_route_list_t *route_list)
{
    FILE *fp;
    char buffer[1024];
    char net_addr[128], gate_addr[128], mask_addr[128];
    int flags;
    sysm_net_route_t *route;

    route_list->size = route_list->number = 0;

    if (!(fp = fopen(PROC_FS_ROOT "net/route", "r")))
        return errno;

    sysm_net_route_list_create(route_list);

    (void)fgets(buffer, sizeof(buffer), fp);
    while (fgets(buffer, sizeof(buffer), fp))
    {
        int num;

        SYSM_NET_ROUTE_LIST_GROW(route_list);
        route = &route_list->data[route_list->number++];
        /* XXX rid sscanf */
        num = sscanf(buffer, ROUTE_FMT,
                     route->ifname, net_addr, gate_addr,
                     &flags, &route->refcnt, &route->use,
                     &route->metric, mask_addr,
                     &route->mtu, &route->window, &route->irtt);

        if ((num < 10) || !(flags & RTF_UP)) {
            --route_list->number;
            continue;
        }

        route->flags = flags;

        sysm_net_address_set(route->destination, hex2int(net_addr, HEX_ENT_LEN));
        sysm_net_address_set(route->gateway, hex2int(gate_addr, HEX_ENT_LEN));
        sysm_net_address_set(route->mask, hex2int(mask_addr, HEX_ENT_LEN));
    }

    fclose(fp);

    return SYSM_OK;

}

int sysm_net_route_list_create(sysm_net_route_list_t *route_list)
{
    route_list->number = 0;
    route_list->size =  SYSM_NET_ROUTE_LIST_MAX;
    route_list->data = malloc(sizeof(*(route_list->data)) *
                             route_list->size);
    return SYSM_OK;
}

int sysm_net_route_list_grow(sysm_net_route_list_t *route_list)
{
    route_list->data =
            realloc(route_list->data,
                    sizeof(*(route_list->data)) *
                    (route_list->size + SYSM_NET_ROUTE_LIST_MAX));
    route_list->size += SYSM_NET_ROUTE_LIST_MAX;

    return SYSM_OK;
}

int sysm_net_route_list_destroy(sysm_net_route_list_t *route_list)
{
    if (route_list->size) {
        free(route_list->data);
        route_list->number = route_list->size = 0;
    }

    return SYSM_OK;
}

static int has_interface(sysm_net_interface_list_t *iflist,
                                      char *name)
{
    register int i;
    register int num = iflist->number;
    register char **data = iflist->data;
    for (i=0; i<num; i++) {
        if (strcmp(name, data[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static int proc_net_interface_list_get(sysm_t *sysm,
                                       sysm_net_interface_list_t *iflist) {
    char buffer[BUFSIZ];
    FILE *fp = fopen("/proc/net/dev", "r");

    if (!fp) {
        return errno;
    }

    /* skip header */
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    while (fgets(buffer, sizeof(buffer), fp)) {
        char *ptr, *dev;

        dev = buffer;
        while (isspace(*dev)) {
            dev++;
        }

        if (!(ptr = strchr(dev, ':'))) {
            continue;
        }

        *ptr++ = 0;

        if (has_interface(iflist, dev)) {
            continue;
        }

        SYSM_NET_IFLIST_GROW(iflist);

        iflist->data[iflist->number++] =
                strdup(dev);
    }

    fclose(fp);

    return SYSM_OK;

}

int sysm_net_interface_list_get(sysm_t *sysm, sysm_net_interface_list_t *iflist)
{
    int n, lastlen=0;
    struct ifreq *ifr;
    struct ifconf ifc;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        return errno;
    }

    for (;;) {
        if (!sysm->ifconf_buf || lastlen) {
            sysm->ifconf_len += sizeof(struct ifreq) * SYSM_NET_IFLIST_MAX;
            sysm->ifconf_buf = realloc(sysm->ifconf_buf, sysm->ifconf_len);
        }

        ifc.ifc_len = sysm->ifconf_len;
        ifc.ifc_buf = sysm->ifconf_buf;

        if (ioctl(sock, SIOCGIFCONF, &ifc) < 0) {
            /* EINVAL should mean num_interfaces > ifc.ifc_len */
            if ((errno != EINVAL) ||
                (lastlen == ifc.ifc_len))
            {
                free(ifc.ifc_buf);
                return errno;
            }
        }

        if (ifc.ifc_len < sysm->ifconf_len) {
            break; /* got em all */
        }

        if (ifc.ifc_len != lastlen) {
            /* might be more */
            lastlen = ifc.ifc_len;
            continue;
        }

        break;
    }

    close(sock);

    iflist->number = 0;
    iflist->size = ifc.ifc_len;
    iflist->data = malloc(sizeof(*(iflist->data)) *
                          iflist->size);

    ifr = ifc.ifc_req;
    for (n = 0; n < ifc.ifc_len; n += sizeof(struct ifreq), ifr++) {
        iflist->data[iflist->number++] =
                strdup(ifr->ifr_name);
    }

    proc_net_interface_list_get(sysm, iflist);

    return SYSM_OK;
}

int sysm_net_interface_list_create(sysm_net_interface_list_t *iflist)
{
    iflist->number = 0;
    iflist->size = SYSM_NET_IFLIST_MAX;
    iflist->data = malloc(sizeof(*(iflist->data)) *
                          iflist->size);
    return SYSM_OK;
}
int sysm_net_interface_list_grow(sysm_net_interface_list_t *iflist)
{
    iflist->data = realloc(iflist->data,
                           sizeof(*(iflist->data)) *
                           (iflist->size + SYSM_NET_IFLIST_MAX));
    iflist->size += SYSM_NET_IFLIST_MAX;

    return SYSM_OK;
}
int sysm_net_interface_list_destroy(sysm_net_interface_list_t *iflist)
{
    if (iflist->size) {
        for (int i=0; i<iflist->number; i++) {
            free(iflist->data[i]);
        }
        free(iflist->data);
        iflist->number = iflist->size = 0;
    }

    return SYSM_OK;
}

int sysm_net_connection_list_get(sysm_t *sysm, sysm_net_connection_list_t *conn_list, int flags)
{
    int status;
    sysm_net_connection_walker_t walker;
    net_conn_getter_t getter;

    sysm_net_connection_list_create(conn_list);

    getter.conn = NULL;
    getter.conn_list = conn_list;

    walker.sysm = sysm;
    walker.flags = flags;
    walker.data = &getter;
    walker.add_connection = proc_net_walker;

    status = sysm_net_connection_walk(&walker);

    if (status != SYSM_OK)
        sysm_net_connection_list_destroy(conn_list);

    return status;
}

int sysm_net_connection_list_create(sysm_net_connection_list_t *conn_list)
{
    conn_list->number = 0;
    conn_list->size = SYSM_NET_CONNLIST_MAX;
    conn_list->data = malloc(sizeof(*(conn_list->data)) *
                                     conn_list->size);
    return SYSM_OK;
}

int sysm_net_connection_list_grow(sysm_net_connection_list_t *conn_list)
{
    conn_list->data =
        realloc(conn_list->data,
                sizeof(*(conn_list->data)) *
                (conn_list->size + SYSM_NET_CONNLIST_MAX));
    conn_list->size += SYSM_NET_CONNLIST_MAX;

    return SYSM_OK;
}

int sysm_net_connection_list_destroy(sysm_net_connection_list_t *conn_list) {
    if (conn_list->size) {
        free(conn_list->data);
        conn_list->number = conn_list->size = 0;
    }

    return SYSM_OK;
}

#define WHOCPY(dest, src) \
    strcpy(dest, src); \
    if (sizeof(src) < sizeof(dest)) \
        dest[sizeof(src)] = '\0'

static int sysm_who_utmp(sysm_t *sysm, sysm_who_list_t *who_list)
{

    FILE *fp;
    struct utmp ut;

    if (!(fp = fopen(_PATH_UTMP, "r"))) {
        return errno;
    }

    while (fread(&ut, sizeof(ut), 1, fp) == 1) {
        sysm_who_t *who;

        if (*ut.ut_name == '\0') {
            continue;
        }

#ifdef USER_PROCESS
        if (ut.ut_type != USER_PROCESS) {
            continue;
        }
#endif

        SYSM_WHO_LIST_GROW(who_list);
        who = &who_list->data[who_list->number++];

        WHOCPY(who->user, ut.ut_name);
        WHOCPY(who->device, ut.ut_line);
        WHOCPY(who->host, ut.ut_host);

        who->time = ut.ut_time;
    }

    fclose(fp);

    return SYSM_OK;

}

int sysm_who_list_get(sysm_t *sysm, sysm_who_list_t *who_list)
{
    int status;

    sysm_who_list_create(who_list);

    status = sysm_who_utmp(sysm, who_list);
    if (status != SYSM_OK) {
        sysm_who_list_destroy(who_list);
        return status;
    }

    return SYSM_OK;
}

int sysm_who_list_create( sysm_who_list_t *who_list)
{
    who_list->number = 0;
    who_list->size = SYSM_WHO_LIST_MAX;
    who_list->data = malloc(sizeof(*(who_list->data)) *
                           who_list->size);
    return SYSM_OK;
}

int sysm_who_list_grow(sysm_who_list_t *who_list)
{
    who_list->data = realloc(who_list->data,
                            sizeof(*(who_list->data)) *
                            (who_list->size + SYSM_WHO_LIST_MAX));
    who_list->size += SYSM_WHO_LIST_MAX;

    return SYSM_OK;
}

int sysm_who_list_destroy(sysm_who_list_t *who_list)
{
    if (who_list->size) {
        free(who_list->data);
        who_list->number = who_list->size = 0;
    }

    return SYSM_OK;
}

int sysm_net_connection_walk(sysm_net_connection_walker_t *walker)
{
    int flags = walker->flags;
    int status;

    if (flags & SYSM_NETCONN_TCP)
    {
        status = proc_net_read(walker,
                               PROC_FS_ROOT "net/tcp",
                               SYSM_NETCONN_TCP);

        if (status != SYSM_OK)
            return status;

        status = proc_net_read(walker,
                               PROC_FS_ROOT "net/tcp6",
                               SYSM_NETCONN_TCP);

        if (!((status == SYSM_OK) || (status == ENOENT)))
            return status;
    }

    if (flags & SYSM_NETCONN_UDP)
    {
        status = proc_net_read(walker,
                               PROC_FS_ROOT "net/udp",
                               SYSM_NETCONN_UDP);

        if (status != SYSM_OK)
            return status;

        status = proc_net_read(walker,
                               PROC_FS_ROOT "net/udp6",
                               SYSM_NETCONN_UDP);

        if (!((status == SYSM_OK) || (status == ENOENT)))
            return status;
    }

    if (flags & SYSM_NETCONN_RAW)
    {
        status = proc_net_read(walker,
                               PROC_FS_ROOT "net/raw",
                               SYSM_NETCONN_RAW);

        if (status != SYSM_OK)
            return status;

        status = proc_net_read(walker,
                               PROC_FS_ROOT "net/raw6",
                               SYSM_NETCONN_RAW);

        if (!((status == SYSM_OK) || (status == ENOENT)))
            return status;
    }

    return SYSM_OK;
}

#define MEMINFO_PARAM(a) a ":", (sizeof(a ":") - 1)

static sysm_uint64_t sysm_meminfo(char *buffer, char *attr, int len)
{
    sysm_uint64_t val = 0;
    char *ptr, *tok;

    if ((ptr = strstr(buffer, attr)))
    {
        ptr += len;
        val = strtoull(ptr, &tok, 0);
        while (*tok == ' ') ++tok;
        if (*tok == 'k' || *tok == 'K') val *= 1024;
        else if (*tok == 'M' || *tok == 'm') val *= (1024 * 1024);
    }

    return val;
}

/*Get Memory Type Range Registers*/
static int get_mtrr(sysm_t *sysm, sysm_memory_t *memory)
{
    char buffer[BUFSIZ], *ptr;
    FILE *fp;
    int mtrr = 0;


    if (sysm->mtrr > 0)
    {
        //use cache
        memory->mtrr = (sysm_uint64_t)sysm->mtrr;
        return SYSM_OK;
    }

    //init value is -1 see sysm_os_open
    if (sysm->mtrr == 0) return ENOENT;

    if (!(fp = fopen(PROC_MTRR, "r")))
        return errno;

    while ((ptr = fgets(buffer, sizeof(buffer), fp)))
    {
        if (!(ptr = strstr(ptr, "size=")))
            continue;

        if (!strstr(ptr, "write-back"))
            continue;
        ptr += 5;
        while (sysm_isspace(*ptr)) ptr++;

        mtrr += atoi(ptr);
    }

    fclose(fp);

    sysm->mtrr = mtrr;
    memory->mtrr = (sysm_uint64_t)sysm->mtrr;

    return SYSM_OK;

}

int sysm_memory_get(sysm_t *sysm, sysm_memory_t *memory)
{
    sysm_uint64_t buffers, cached, kern, diff;
    sysm_int64_t total;
    char buffer[BUFSIZ];
    int status;

    if ((status = sysm_file2str(PROC_MEMINFO,
                                buffer,
                                sizeof(buffer))) != SYSM_OK)
        return status;

    memory->total = sysm_meminfo(buffer, MEMINFO_PARAM("MemTotal"));
    memory->free = sysm_meminfo(buffer, MEMINFO_PARAM("MemFree"));
    memory->used = memory->total - memory->free;

    buffers = sysm_meminfo(buffer, MEMINFO_PARAM("Buffers"));
    cached = sysm_meminfo(buffer, MEMINFO_PARAM("Cached"));

    kern = buffers + cached;
    memory->actual_free = memory->free + kern;
    memory->actual_used = memory->used - kern;

    total = memory->total / 1024;

    diff = total - (memory->actual_free / 1024);
    memory->used_percent = (double)(diff * 100) / total;

    diff = total - (memory->actual_used / 1024);
    memory->free_percent = (double)(diff * 100) / total;


    if ((status = get_mtrr(sysm, memory)) != SYSM_OK)
        return status;

    return SYSM_OK;
}

int sysm_swap_get(sysm_swap_t *swap)
{
    char buffer[BUFSIZ], *ptr;

    int status = sysm_file2str(PROC_MEMINFO, buffer, sizeof(buffer));
    if (status != SYSM_OK) return status;

    swap->total = sysm_meminfo(buffer, MEMINFO_PARAM("SwapTotal"));
    swap->free = sysm_meminfo(buffer, MEMINFO_PARAM("SwapFree"));
    swap->used = swap->total - swap->free;

    swap->page_in = swap->page_out = (sysm_uint64_t)(-1);

    status = sysm_file2str(PROC_VMSTAT, buffer, sizeof(buffer));
    if (status != SYSM_OK) return status;

    if ((ptr = strstr(buffer, "\npswpin")))
    {
        ptr = sysm_skip_token(ptr);
        swap->page_in = strtoull(ptr, &ptr, 10);
        ptr = sysm_skip_token(ptr);
        swap->page_out = strtoull(ptr, &ptr, 10);
    }

    return SYSM_OK;
}

int sysm_cpu_get(sysm_t *sysm, sysm_cpu_t *cpu)
{
    char buffer[BUFSIZ];
    int status = sysm_file2str(PROC_STAT, buffer, sizeof(buffer));

    if (status != SYSM_OK) return status;

    SYSM_ZERO(cpu);

    get_cpu_metrics(sysm, cpu, buffer);
    return SYSM_OK;
}

int sysm_loadavg_get(sysm_t *sysm, sysm_loadavg_t *loadavg)
{
    char buffer[BUFSIZ], *ptr = buffer;
    int status = sysm_file2str(PROC_LOADAVG, buffer, sizeof(buffer));

    if (status != SYSM_OK) return status;

    loadavg->loadavg[0] = strtod(buffer, &ptr);
    loadavg->loadavg[1] = strtod(ptr, &ptr);
    loadavg->loadavg[2] = strtod(ptr, &ptr);

    return SYSM_OK;
}

int sysm_uptime_get(sysm_uptime_t *uptime)
{
    char buffer[BUFSIZ], *ptr = buffer;
    int status = sysm_file2str(PROC_UPTIME, buffer, sizeof(buffer));

    if (status != SYSM_OK)
        return status;

    uptime->uptime = strtod(buffer, &ptr);

    return SYSM_OK;
}

int sysm_proc_cpu_get(sysm_t *sysm, pid_t pid, sysm_proc_cpu_t *proc_cpu)
{
    sysm_cache_entry_t *entry;
    sysm_proc_cpu_t *prev;
    sysm_uint64_t otime, time_now = sysm_time_now_millis();
    sysm_uint64_t time_diff, total_diff;
    int status;

    if (!sysm->proc_cpu)
    {
        sysm->proc_cpu = sysm_expired_cache_new(128, PID_CACHE_CLEANUP_PERIOD, PID_CACHE_ENTRY_EXPIRE_PERIOD);
    }

    entry = sysm_cache_get(sysm->proc_cpu, (sysm_uint64_t) pid);
    if (entry->value)
        prev = (sysm_proc_cpu_t *)entry->value;
    else
    {
        prev = entry->value = malloc(sizeof(*prev));
        SYSM_ZERO(prev);
    }

    time_diff = time_now - prev->last_time;
    proc_cpu->last_time = prev->last_time = time_now;

    if (time_diff == 0)
    {
        memcpy(proc_cpu, prev, sizeof(*proc_cpu));
        return SYSM_OK;
    }

    otime = prev->total;

    status = sysm_proc_time_get(sysm, pid, (sysm_proc_time_t *)proc_cpu);

    if (status != SYSM_OK)
        return status;

    memcpy(prev, proc_cpu, sizeof(*prev));

    if (proc_cpu->total < otime)
        otime = 0;

    if (otime == 0)
    {
        proc_cpu->percent = 0.0;
        /* first time called */
        return SYSM_OK;
    }

    total_diff = proc_cpu->total - otime;
    proc_cpu->percent = total_diff / (double)time_diff;

    return SYSM_OK;
}

static int proc_stat_read(sysm_t *sysm, pid_t pid)
{
    char buffer[BUFSIZ], *ptr = buffer, *tmp;
    unsigned int len;
    linux_proc_stat_t *pstat = &sysm->last_proc_stat;
    int status;

    time_t timenow = time(NULL);

    if (pstat->pid == pid)
    {
        if ((timenow - pstat->mtime) < SYSM_LAST_PROC_EXPIRE)
        {
            return SYSM_OK;
        }
    }

    pstat->pid = pid;
    pstat->mtime = timenow;

    status = SYSM_PROC_FILE2STR(buffer, pid, PROC_PSTAT);

    if (status != SYSM_OK)
        return status;

    if (!(ptr = strchr(ptr, '('))) {
        return EINVAL;
    }
    if (!(tmp = strrchr(++ptr, ')'))) {
        return EINVAL;
    }
    len = (unsigned int) (tmp - ptr);

    if (len >= sizeof(pstat->name)) {
        len = sizeof(pstat->name)-1;
    }

    /* (1,2) */
    memcpy(pstat->name, ptr, len);
    pstat->name[len] = '\0';
    ptr = tmp+1;

    SYSM_SKIP_SPACE(ptr);
    pstat->state = *ptr++;  /* (3) */
    SYSM_SKIP_SPACE(ptr);

    pstat->ppid = (pid_t) strtoul(ptr, &ptr, 10); /* (4) */
    ptr = sysm_skip_token(ptr); /* (5) pgrp */
    ptr = sysm_skip_token(ptr);/* (6) session */
    pstat->tty = (int) strtoul(ptr, &ptr, 10); /* (7) */
    ptr = sysm_skip_token(ptr); /* (8) tty pgrp */

    ptr = sysm_skip_token(ptr); /* (9) flags */
    pstat->minor_faults = strtoul(ptr, &ptr, 10); /* (10) */
    ptr = sysm_skip_token(ptr); /* (11) cmin flt */
    pstat->major_faults = strtoul(ptr, &ptr, 10); /* (12) */
    ptr = sysm_skip_token(ptr); /* (13) cmaj flt */

    pstat->utime = (sysm_uint64_t) SYSM_TICK2MSEC(strtoul(ptr, &ptr, 10)); /* (14) */
    pstat->stime = (sysm_uint64_t) SYSM_TICK2MSEC(strtoul(ptr, &ptr, 10)); /* (15) */

    ptr = sysm_skip_token(ptr); /* (20) timeout */
    ptr = sysm_skip_token(ptr); /* (21) it_real_value */

    pstat->start_time  = strtoul(ptr, &ptr, 10); /* (22) */
    pstat->start_time /= sysm->ticks;
    pstat->start_time += sysm->boot_time; /* seconds */
    pstat->start_time *= 1000; /* milliseconds */

    pstat->vsize = strtoul(ptr, &ptr, 10); /* (23) */
    pstat->rss   = pageshift(strtoul(ptr, &ptr, 10)); /* (24) */

    ptr = sysm_skip_token(ptr); /* (25) rlim */
    ptr = sysm_skip_token(ptr); /* (26) startcode */
    ptr = sysm_skip_token(ptr); /* (27) endcode */
    ptr = sysm_skip_token(ptr); /* (28) startstack */
    ptr = sysm_skip_token(ptr); /* (29) kstkesp */
    ptr = sysm_skip_token(ptr); /* (30) kstkeip */
    ptr = sysm_skip_token(ptr); /* (31) signal */
    ptr = sysm_skip_token(ptr); /* (32) blocked */
    ptr = sysm_skip_token(ptr); /* (33) sigignore */
    ptr = sysm_skip_token(ptr); /* (34) sigcache */
    ptr = sysm_skip_token(ptr); /* (35) wchan */
    ptr = sysm_skip_token(ptr); /* (36) nswap */
    ptr = sysm_skip_token(ptr); /* (37) cnswap */
    ptr = sysm_skip_token(ptr); /* (38) exit_signal */

    pstat->processor = (int) strtoul(ptr, &ptr, 10); /* (39) */

    return SYSM_OK;
}

int sysm_proc_time_get(sysm_t *sysm, pid_t pid, sysm_proc_time_t *proc_time)
{
    int status = proc_stat_read(sysm, pid);
    linux_proc_stat_t *pstat = &sysm->last_proc_stat;

    if (status != SYSM_OK)
        return status;

    proc_time->user = pstat->utime;
    proc_time->sys  = pstat->stime;
    proc_time->total = proc_time->user + proc_time->sys;
    proc_time->start_time = pstat->start_time;

    return SYSM_OK;
}

#ifndef ARG_MAX
#define ARG_MAX 131072
#endif

#define SYSM_PROC_ENV_KEY_LOOKUP() \
    if ((proc_env->type == SYSM_PROC_ENV_KEY) && \
        (pid == sysm->pid)) \
    { \
        char *value = getenv(proc_env->key); \
        if (value != NULL) { \
            proc_env->env_getter(proc_env->data, \
                                proc_env->key, \
                                proc_env->klen, \
                                value, strlen(value)); \
        } \
        return SYSM_OK; \
    }

int sysm_proc_env_get(sysm_t *sysm, pid_t pid, sysm_proc_env_t *proc_env)
{
    int fd;
    char buffer[ARG_MAX];
    char name[BUFSIZ];
    size_t len;
    char *ptr, *end;

    SYSM_PROC_ENV_KEY_LOOKUP();

    (void)SYSM_PROC_FILENAME(name, pid, "/environ");

    if ((fd = open(name, O_RDONLY)) < 0)
    {
        if (errno == ENOENT)
            return ESRCH;
        return errno;
    }

    len = (size_t) read(fd, buffer, sizeof(buffer));

    close(fd);

    buffer[len] = '\0';
    ptr = buffer;

    end = buffer + len;
    while (ptr < end)
    {
        char *val = strchr(ptr, '=');
        int klen, vlen, status;
        char key[128]; /* XXX is there a max key size? */

        if (val == NULL) {
            /* not key=val format */
            break;
        }

        klen = (int) (val - ptr);
        strncpy(key, ptr, sizeof(key));
        key[klen] = '\0';
        ++val;

        vlen = (int) strlen(val);
        status = proc_env->env_getter(proc_env->data,
                                     key, klen, val, vlen);

        if (status != SYSM_OK) {
            /* not an error; just stop iterating */
            break;
        }

        ptr += (klen + 1 + vlen + 1);
    }

    return SYSM_OK;
}

int sysm_proc_exe_get(sysm_t *sysm, pid_t pid, sysm_proc_exe_t *proc_exe)
{
    int len;
    char name[BUFSIZ];

    (void)SYSM_PROC_FILENAME(name, pid, "/cwd");

    if ((len = (int) readlink(name, proc_exe->cwd,
                            sizeof(proc_exe->cwd)-1)) < 0)
    {
        return errno;
    }

    proc_exe->cwd[len] = '\0';

    (void)SYSM_PROC_FILENAME(name, pid, "/exe");

    if ((len = (int) readlink(name, proc_exe->name,
                            sizeof(proc_exe->name)-1)) < 0)
    {
        return errno;
    }

    proc_exe->name[len] = '\0';

    (void)SYSM_PROC_FILENAME(name, pid, "/root");

    if ((len = (int) readlink(name, proc_exe->root,
                            sizeof(proc_exe->root)-1)) < 0)
    {
        return errno;
    }

    proc_exe->root[len] = '\0';

    return SYSM_OK;
}

int sysm_proc_mem_get(sysm_t *sysm, pid_t pid, sysm_proc_mem_t *proc_mem)
{
    char buffer[BUFSIZ], *ptr=buffer;
    int status = proc_stat_read(sysm, pid);
    linux_proc_stat_t *pstat = &sysm->last_proc_stat;

    proc_mem->minor_faults = pstat->minor_faults;
    proc_mem->major_faults = pstat->major_faults;
    proc_mem->page_faults =
            proc_mem->minor_faults + proc_mem->major_faults;

    status = SYSM_PROC_FILE2STR(buffer, pid, "/statm");

    if (status != SYSM_OK) {
        return status;
    }

    proc_mem->size     = pageshift(strtoull(ptr, &ptr, 10));
    proc_mem->resident = pageshift(strtoull(ptr, &ptr, 10));
    proc_mem->share    = pageshift(strtoull(ptr, &ptr, 10));

    return SYSM_OK;
}


int sysm_proc_own_get(sysm_t *sysm, pid_t pid, sysm_proc_own_t *proc_own)
{
    char buffer[BUFSIZ], *ptr;
    int status = SYSM_PROC_FILE2STR(buffer, pid, PROC_PSTATUS);

    if (status != SYSM_OK)
        return status;

    if ((ptr = strstr(buffer, "\nUid:")))
    {
        ptr = sysm_skip_token(ptr);

        proc_own->uid = (pid_t) strtoul(ptr, &ptr, 10);
        proc_own->euid = (pid_t) strtoul(ptr, &ptr, 10);
    }
    else
        return ENOENT;

    if ((ptr = strstr(ptr, "\nGid:")))
    {
        ptr = sysm_skip_token(ptr);

        proc_own->gid = (pid_t) strtoul(ptr, &ptr, 10);
        proc_own->egid = (pid_t) strtoul(ptr, &ptr, 10);
    }
    else
        return ENOENT;

    return SYSM_OK;
}

int sysm_proc_owner_get(sysm_t *sysm, pid_t pid, sysm_proc_owner_t *proc_owner)
{
    sysm_proc_own_t proc_own;

    int status = sysm_proc_own_get(sysm, pid, &proc_own);

    if (status != SYSM_OK)
        return status;

    status = sysm_user_name_get(proc_own.uid,
                                proc_owner->user,
                                sizeof(proc_owner->user));
    if (status != SYSM_OK)
        return status;

    status = sysm_group_name_get(proc_own.gid,
                                 proc_owner->group,
                                 sizeof(proc_owner->group));

    return status;
}

int sysm_proc_stat_get(sysm_t *sysm, sysm_proc_stat_t *proc_stat)
{
    int status;
    sysm_proc_list_t *pids;

    SYSM_ZERO(proc_stat);
    proc_stat->threads = (sysm_uint64_t) -1;

    if ((status = sysm_proc_list_get(sysm, NULL)) != SYSM_OK)
        return status;

    pids = sysm->pids;
    proc_stat->total = pids->number;

    for (int i = 0;i < pids->number;i++)
    {
        sysm_proc_state_t state;

        status = sysm_proc_state_get(sysm, pids->data[i], &state);
        if (status != SYSM_OK)
            continue;

        if (state.threads != -1)
            proc_stat->threads += state.threads;

        switch (state.state)
        {
            case SYSM_PROC_STATE_IDLE:
                proc_stat->idle++;
                break;
            case SYSM_PROC_STATE_RUN:
                proc_stat->running++;
                break;
            case SYSM_PROC_STATE_SLEEP:
                proc_stat->sleeping++;
                break;
            case SYSM_PROC_STATE_STOP:
                proc_stat->stopped++;
                break;
            case SYSM_PROC_STATE_ZOMBIE:
                proc_stat->zombie++;
                break;
            default:
                break;
        }
    }

    return SYSM_OK;
}

static int proc_status_get(pid_t pid, sysm_proc_state_t *proc_state)
{
    char buffer[BUFSIZ], *ptr;
    int status = SYSM_PROC_FILE2STR(buffer, pid, PROC_PSTATUS);

    if (status != SYSM_OK)
        return status;

    ptr = strstr(buffer, "\nThread:");
    if (ptr)
    {
        ptr = sysm_skip_token(ptr);
        proc_state->threads = strtoul(ptr, &ptr, 10);
    }
    else
    {
        proc_state->threads = -1;
    }

    return SYSM_OK;
}

#define OffsetOf(structure, field) \
   (size_t)(&((structure *)NULL)->field)

#define RlimitOffsets(field) \
    OffsetOf(sysm_resource_limit_t, field##_cur), \
    OffsetOf(sysm_resource_limit_t, field##_max)

#define RlimitSet(structure, ptr, val) \
    *(sysm_uint64_t *)((char *)structure + (int)(long)ptr) = val

typedef struct {
    int resource;
    int factor;
    size_t cur;
    size_t max;
} rlimit_field_t;

#ifndef RLIMIT_RSS
#define RLIMIT_RSS (RLIM_NLIMITS+1)
#endif

#ifndef RLIMIT_NPROC
#define RLIMIT_NPROC (RLIM_NLIMITS+2)
#endif

#define RLIMIT_PSIZE (RLIM_NLIMITS+3)

#ifndef RLIMIT_AS
#  if defined(RLIMIT_VMEM)
#    define RLIMIT_AS RLIMIT_VMEM
#  elif defined(RLIMIT_RSS)
#    define RLIMIT_AS RLIMIT_RSS
#  endif
#endif

static rlimit_field_t sysm_rlimits[] = {
        { RLIMIT_CPU, 1, RlimitOffsets(cpu) },
        { RLIMIT_FSIZE,  1024, RlimitOffsets(file_size) },
        { RLIMIT_DATA,   1024, RlimitOffsets(data) },
        { RLIMIT_STACK,  1024, RlimitOffsets(stack) },
        { RLIMIT_PSIZE,   512, RlimitOffsets(pipe_size) },
        { RLIMIT_CORE,   1024, RlimitOffsets(core) },
        { RLIMIT_RSS,    1024, RlimitOffsets(memory) },
        { RLIMIT_NPROC,  1,    RlimitOffsets(processes) },
        { RLIMIT_NOFILE, 1,    RlimitOffsets(open_files) },
        { RLIMIT_AS,     1024, RlimitOffsets(virtual_memory) },
        { -1 }
};

#define RlimitScale(val) \
    if (val != RLIM_INFINITY) val /= r->factor

#define RlimitHS(val) \
    rl.rlim_cur = rl.rlim_max = (val)

int sysm_proc_state_get(sysm_t *sysm, pid_t pid, sysm_proc_state_t *proc_state)
{
    int status = proc_stat_read(sysm, pid);
    linux_proc_stat_t *pstat = &sysm->last_proc_stat;

    if (status != SYSM_OK)
        return status;

    memcpy(proc_state->name, pstat->name, sizeof(proc_state->name));
    proc_state->state = pstat->state;

    proc_state->ppid      = pstat->ppid;
    proc_state->tty       = pstat->tty;
    proc_state->priority  = pstat->pri;
    proc_state->nice      = pstat->ni;
    proc_state->processor = pstat->processor;

    if (sysm_cpu_core_rollup(sysm))
        proc_state->processor /= sysm->lcpu;

    return proc_status_get(pid, proc_state);
}

int sysm_resource_limit_get(sysm_t *sysm, sysm_resource_limit_t *rlimit)
{
    for (int i = 0;sysm_rlimits[i].resource != -1;i++)
    {
        struct rlimit rl;
        rlimit_field_t *r = &sysm_rlimits[i];

        if (r->resource > RLIM_NLIMITS)
        {
            switch (r->resource)
            {
                case RLIMIT_NPROC:
                    RlimitHS(sysconf(_SC_CHILD_MAX));
                    break;
                case RLIMIT_PSIZE:
                    RlimitHS(PIPE_BUF / 512);
                    break;
                default:
                    RlimitHS(RLIM_INFINITY);
                    break;
            }
        }
        else if (getrlimit(r->resource, &rl) != 0)
        {
            RlimitHS(RLIM_INFINITY);
        }
        else
        {
            RlimitScale(rl.rlim_cur);
            RlimitScale(rl.rlim_max);
        }

        RlimitSet(rlimit, r->cur, rl.rlim_cur);
        RlimitSet(rlimit, r->max, rl.rlim_max);
    }

    return SYSM_OK;
}

static int get_iostat_sys(sysm_t *sysm,
                          const char *dirname,
                          sysm_disk_usage_t *disk,
                          sysm_iodev_t **iodev)
{
    char stat[1025], dev[1025];
    char *name, *ptr, *fsdev;
    int partition, status;

    if (!(*iodev = sysm_iodev_get(sysm, dirname)))
        return ENXIO;

    name = fsdev = (*iodev)->name;

    if (SYSM_NAME_IS_DEV(name))
        name += strlen(SYSM_DEV_PREFIX);

    while (!isdigit(*fsdev))
        fsdev++;

    partition = (int) strtoul(fsdev, NULL, 0);
    *fsdev = '\0';

    snprintf(stat, sizeof(stat), SYS_BLOCK "/%s/%s%d/stat", name, name, partition);

    status = sysm_file2str(stat, dev, sizeof(dev));
    if (status != SYSM_OK) {
        return status;
    }

    ptr = dev;
    ptr = sysm_skip_token(ptr);
    disk->reads = strtoull(ptr, &ptr, 10);
    ptr = sysm_skip_token(ptr);
    disk->writes = strtoull(ptr, &ptr, 10);

    disk->read_bytes  = -1;
    disk->write_bytes = -1;
    disk->util       = -1;

    return SYSM_OK;
}

#define ST_MAJOR(sb) major((sb).st_rdev)
#define ST_MINOR(sb) minor((sb).st_rdev)

static int get_iostat_proc_dstat(sysm_t *sysm,
                                 const char *dirname,
                                 sysm_disk_usage_t *disk,
                                 sysm_iodev_t **iodev,
                                 sysm_disk_usage_t *device_usage)
{
    FILE *fp;
    char buffer[1025];
    char *ptr;
    struct stat sb;
    int status=ENOENT;

    SYSM_DISK_STATS_INIT(device_usage);

    if (!(*iodev = sysm_iodev_get(sysm, dirname))) {
        return ENXIO;
    }

    if (stat((*iodev)->name, &sb) < 0) {
        return errno;
    }

    if (!(fp = fopen(PROC_DISKSTATS, "r"))) {
        return errno;
    }

    while ((ptr = fgets(buffer, sizeof(buffer), fp))) {
        unsigned long major, minor;

        major = strtoul(ptr, &ptr, 10);
        minor = strtoul(ptr, &ptr, 10);

        if ((major == ST_MAJOR(sb)) &&
            ((minor == ST_MINOR(sb)) || (minor == 0)))
        {
            int num;
            unsigned long
                    rio, rmerge, rsect, ruse,
                    wio, wmerge, wsect, wuse,
                    running, use, aveq;

            ptr = sysm_skip_token(ptr); /* name */

            num = sscanf(ptr,
                         "%lu %lu %lu %lu "
                                 "%lu %lu %lu %lu "
                                 "%lu %lu %lu",
                         &rio,     /* 1  # reads issued  This is the total number of reads completed successfully */
                         &rmerge,  /* 2  # reads merged number of reads merged */
                         &rsect,   /* 3  # sectors read  This is the total number of sectors read successfully */
                         &ruse,    /* 4  # millis spent reading */
                         &wio,     /* 5  # writes completed */
                         &wmerge,  /* 6  # writes merged */
                         &wsect,   /* 7  # sectors written */
                         &wuse,    /* 8  # millis spent writing */
                         &running, /* 9  # I/Os currently in progress */
                         &use,     /* 10 # millis spent doing I/Os */
                         &aveq);   /* 11 # of millis spent doing I/Os (weighted) */

            if (num == 11) {
                disk->rtime = ruse;
                disk->wtime = wuse;
                disk->time = use;
                disk->qtime = aveq;
            }
            else if (num == 4) {
                wio = rsect;
                rsect = rmerge;
                wsect = ruse;
                disk->time = disk->qtime = -1;
            }
            else {
                status = ENOENT;
            }

            disk->reads = rio;
            disk->writes = wio;
            disk->read_bytes  = rsect;
            disk->write_bytes = wsect;

            /* convert sectors to bytes (512 is fixed size in 2.6 kernels) */
            disk->read_bytes  *= 512;
            disk->write_bytes *= 512;

            if (minor == ST_MINOR(sb)) {
                status = SYSM_OK;
                break;
            }
            else if (minor == 0) {
                memcpy(device_usage, disk, sizeof(*device_usage));
            }
        }
    }

    fclose(fp);

    return status;
}

static int get_iostat_procp(sysm_t *sysm,
                            const char *dirname,
                            sysm_disk_usage_t *disk,
                            sysm_iodev_t **iodev)
{
    FILE *fp;
    char buffer[1025];
    char *ptr;
    struct stat sb;

    if (!(*iodev = sysm_iodev_get(sysm, dirname))) {
        return ENXIO;
    }

    if (stat((*iodev)->name, &sb) < 0) {
        return errno;
    }

    if (!(fp = fopen(PROC_PARTITIONS, "r"))) {
        return errno;
    }

    (void)fgets(buffer, sizeof(buffer), fp); /* skip header */
    while ((ptr = fgets(buffer, sizeof(buffer), fp))) {
        unsigned long major, minor;

        major = strtoul(ptr, &ptr, 10);
        minor = strtoul(ptr, &ptr, 10);

        if ((major == ST_MAJOR(sb)) && (minor == ST_MINOR(sb))) {
            ptr = sysm_skip_token(ptr); /* blocks */
            ptr = sysm_skip_token(ptr); /* name */
            disk->reads = strtoul(ptr, &ptr, 10); /* rio */
            ptr = sysm_skip_token(ptr);  /* rmerge */
            disk->read_bytes  = strtoul(ptr, &ptr, 10); /* rsect */
            disk->rtime = strtoul(ptr, &ptr, 10); /* ruse */
            disk->writes = strtoul(ptr, &ptr, 10); /* wio */
            ptr = sysm_skip_token(ptr);  /* wmerge */
            disk->write_bytes = strtoul(ptr, &ptr, 10); /* wsect */
            disk->wtime = strtoul(ptr, &ptr, 10); /* wuse */
            ptr = sysm_skip_token(ptr); /* running */
            disk->time = strtoul(ptr, &ptr, 10); /* use */
            disk->qtime  = strtoul(ptr, &ptr, 10); /* aveq */

            /* convert sectors to bytes (512 is fixed size in 2.6 kernels) */
            disk->read_bytes  *= 512;
            disk->write_bytes *= 512;

            fclose(fp);
            return SYSM_OK;
        }
    }

    fclose(fp);

    return ENOENT;
}

int sysm_disk_usage_get(sysm_t *sysm, const char *name, sysm_disk_usage_t *disk)
{
    int status;
    sysm_iodev_t *iodev;
    sysm_disk_usage_t device_usage;
    SYSM_DISK_STATS_INIT(disk);

    switch (sysm->iostat)
    {
        case IOSTAT_SYS:
            status = get_iostat_sys(sysm, name, disk, &iodev);
            break;
        case IOSTAT_DISKSTATS:
            status = get_iostat_proc_dstat(sysm, name, disk, &iodev, &device_usage);
            break;
        case IOSTAT_PARTITIONS:
            status = get_iostat_procp(sysm, name, disk, &iodev);
            break;
        case IOSTAT_NONE:
        default:
            status = ENOENT;
            break;
    }

    if ((status == SYSM_OK) && iodev) {
        sysm_uptime_t uptime;
        sysm_uint64_t interval, ios;
        double tput, util;
        sysm_disk_usage_t *partition_usage=NULL;

        sysm_uptime_get(&uptime);

        if (iodev->is_partition &&
            (sysm->iostat == IOSTAT_DISKSTATS))
        {
            /* 2.6 kernels do not have per-partition times */
            partition_usage = disk;
            disk = &device_usage;
        }

        disk->snaptime = uptime.uptime;

        if (iodev->disk.snaptime) {
            interval = disk->snaptime - iodev->disk.snaptime;
        }
        else {
            interval = disk->snaptime;
        }

        ios =
                (disk->reads - iodev->disk.reads) +
                (disk->writes - iodev->disk.writes);

        if (disk->time == -1) {
            disk->service_time = -1;
        }
        else {
            tput = ((double)ios) * HZ / interval;
            util = ((double)(disk->time - iodev->disk.time)) / interval * HZ;
            disk->service_time = tput ? util / tput : 0.0;
        }
        if (disk->qtime == -1) {
            disk->util = -1;
        }
        else {
            util = ((double)(disk->time - iodev->disk.time)) / interval;
//            printf("%ld, %ld, %ld, %ld\n", disk->time, iodev->disk.time, interval, disk->time - iodev->disk.time);
            disk->util = util / 1000.0;
        }

        memcpy(&iodev->disk, disk, sizeof(iodev->disk));
        if (partition_usage) {
            partition_usage->service_time = disk->service_time;
            partition_usage->util = disk->util;
        }
    }

    return status;
}

static const char *fstype_names[] = {
        "unknown", "none", "local", "remote", "ram", "cdrom", "swap"
};

static int sysm_common_fs_type_get(sysm_file_system_t *fsp)
{
    char *type = fsp->sys_type_name;

    switch (*type) {
        case 'n':
            if (strncmp(type, "nfs", 3) == 0) {
                fsp->type = SYSM_FSTYPE_NETWORK;
            }
            break;
        case 's':
            if (strcmp(type, "smbfs") == 0) { /* samba */
                fsp->type = SYSM_FSTYPE_NETWORK;
            }
            else if (strcmp(type, "swap") == 0) {
                fsp->type = SYSM_FSTYPE_SWAP;
            }
            break;
        case 'a':
            if (strcmp(type, "afs") == 0) {
                fsp->type = SYSM_FSTYPE_NETWORK;
            }
            break;
        case 'i':
            if (strcmp(type, "iso9660") == 0) {
                fsp->type = SYSM_FSTYPE_CDROM;
            }
            break;
        case 'c':
            if (strcmp(type, "cvfs") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            else if (strcmp(type, "cifs") == 0) {
                fsp->type = SYSM_FSTYPE_NETWORK;
            }
            break;
        case 'm':
            if (strcmp(type, "msdos") == 0 || strcmp(type, "minix") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'h':
            if (strcmp(type, "hpfs") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'v':
            if (strcmp(type, "vxfs") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            else if (strcmp(type, "vfat") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'z':
            if (strcmp(type, "zfs") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
    }

    return fsp->type;
}

int sysm_os_fs_type_get(sysm_file_system_t *fsp)
{
    char *type = fsp->sys_type_name;

    switch (*type) {
        case 'e':
            if (strncmp(type, "ext", 3) == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'g':
            if (strcmp(type, "gfs") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'h':
            if (strcmp(type, "hpfs") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'j':
            if (strncmp(type, "jfs", 3) == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'o':
            if (strncmp(type, "ocfs", 4) == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'p':
            if (strncmp(type, "psfs", 4) == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'r':
            if (strcmp(type, "reiserfs") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'v':
            if (strcmp(type, "vzfs") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
        case 'x':
            if (strcmp(type, "xfs") == 0 || strcmp(type, "xiafs") == 0) {
                fsp->type = SYSM_FSTYPE_LOCAL_DISK;
            }
            break;
    }

    return fsp->type;
}

void sysm_fs_type_get(sysm_file_system_t *fsp)
{
    if (!(fsp->type ||                    /* already set */
          sysm_os_fs_type_get(fsp) ||    /* try os specifics first */
          sysm_common_fs_type_get(fsp))) /* try common ones last */
    {
        fsp->type = SYSM_FSTYPE_NONE;
    }

    if (fsp->type >= SYSM_FSTYPE_MAX) {
        fsp->type = SYSM_FSTYPE_NONE;
    }

    strcpy(fsp->type_name, fstype_names[fsp->type]);
}

static int sysm_get_default_gateway(sysm_t *sysm,
                                     sysm_net_info_t *netinfo)
{
    int status, i;
    sysm_net_route_list_t route_list;

    status = sysm_net_route_list_get(sysm, &route_list);
    if (status != SYSM_OK) {
        return status;
    }

    for (i=0; i<route_list.number; i++) {
        if ((route_list.data[i].flags & SYSM_RTF_GATEWAY) &&
            (route_list.data[i].destination.addr.in == 0))
        {
            sysm_net_address_to_string(sysm,
                                        &route_list.data[i].gateway,
                                        netinfo->default_gateway);

            strncpy(netinfo->default_gateway_interface,
                          route_list.data[i].ifname,
                          sizeof(netinfo->default_gateway_interface));
            break;
        }
    }

    sysm_net_route_list_destroy(&route_list);

    return SYSM_OK;
}

int sysm_net_info_get(sysm_t *sysm, sysm_net_info_t *net_info)
{
    int size;
    char buffer[BUFSIZ], *ptr;
    FILE *fp;

    SYSM_ZERO(net_info);

    if ((fp = fopen("/etc/resolv.conf", "r")))
    {
        while ((ptr = fgets(buffer, sizeof(buffer), fp)))
        {
            int len;

            SYSM_SKIP_SPACE(ptr);
            if ((*ptr == '#') || !(ptr = strstr(ptr, "nameserver")))
                continue;
            ptr += 10;
            SYSM_SKIP_SPACE(ptr);

            len = (int) strlen(ptr);
            ptr[len - 1] = '\0';

            if (!net_info->primary_dns[0])
                strcpy(net_info->primary_dns, ptr);
            else if (!net_info->secondary_dns[0])
                strcpy(net_info->secondary_dns, ptr);
            else
                break;
        }
        fclose(fp);
    }

    size = sizeof(net_info->host_name) - 1;
    if (gethostname(net_info->host_name, size) == 0)
        net_info->host_name[size] = '\0';
    else
        net_info->host_name[0] = '\0';

    size = sizeof(net_info->domain_name) - 1;
    if (getdomainname(net_info->domain_name, size) == 0)
        net_info->domain_name[size] = '\0';
    else
        net_info->domain_name[0] = '\0';

    sysm_get_default_gateway(sysm, net_info);

    return SYSM_OK;
}

typedef struct {
    sysm_net_stat_t *netstat;
    sysm_net_address_t *address;
    unsigned long port;
} net_stat_port_getter_t;

static int net_stat_port_walker(sysm_net_connection_walker_t *walker,
                                sysm_net_connection_t *conn)
{
    net_stat_port_getter_t *getter =
            (net_stat_port_getter_t *)walker->data;
    sysm_net_stat_t *netstat = getter->netstat;

    if (conn->type == SYSM_NETCONN_TCP) {
        if (conn->lo_port == getter->port) {
            netstat->all_inbound_total++;

            if (sysm_net_address_equals(getter->address,
                                         &conn->lo_address) == SYSM_OK)
            {
                netstat->tcp_inbound_total++;
            }
        }
        else if (conn->re_port == getter->port) {
            netstat->all_outbound_total++;

            if (sysm_net_address_equals(getter->address,
                                         &conn->re_address) == SYSM_OK)
            {
                netstat->tcp_outbound_total++;
            }
        }
        else {
            return SYSM_OK;
        }

        netstat->tcp_states[conn->state]++;
    }
    else if (conn->type == SYSM_NETCONN_UDP) {
        /*XXX*/
    }

    return SYSM_OK;
}

int sysm_net_stat_port_get(sysm_t *sysm, sysm_net_stat_t *netstat, int flags, sysm_net_address_t *address, unsigned long port)
{
    sysm_net_connection_walker_t walker;
    net_stat_port_getter_t getter;

    SYSM_ZERO(netstat);

    getter.netstat = netstat;
    getter.address = address;
    getter.port = port;

    walker.sysm = sysm;
    walker.data = &getter;
    walker.add_connection = net_stat_port_walker;

    walker.flags = flags;

    return sysm_net_connection_walk(&walker);
}

static void sysm_net_listen_address_add(sysm_t *sysm,
                                         sysm_net_connection_t *conn)
{
    sysm_cache_entry_t *entry =
            sysm_cache_get(sysm->net_listen, conn->lo_port);

    if (entry->value) {
        if (conn->lo_address.family == SYSM_AF_INET6) {
            return; /* prefer ipv4 */
        }
    }
    else {
        entry->value = malloc(sizeof(conn->lo_address));
    }

    memcpy(entry->value, &conn->lo_address,
           sizeof(conn->lo_address));
}

typedef struct {
    sysm_net_stat_t *netstat;
    sysm_net_connection_list_t *connlist;
} net_stat_getter_t;

static int net_stat_walker(sysm_net_connection_walker_t *walker,
                           sysm_net_connection_t *conn)
{
    int state = conn->state;
    sysm_cache_t *listen_ports = walker->sysm->net_listen;
    net_stat_getter_t *getter =
            (net_stat_getter_t *)walker->data;

    if (conn->type == SYSM_NETCONN_TCP) {
        getter->netstat->tcp_states[state]++;

        /* XXX listen_ports may get stale */
        if (state == SYSM_TCP_LISTEN) {
            sysm_net_listen_address_add(walker->sysm, conn);
        }
        else {
            if (sysm_cache_find(listen_ports,
                                 conn->lo_port))
            {
                getter->netstat->tcp_inbound_total++;
            }
            else {
                getter->netstat->tcp_outbound_total++;
            }
        }
    }
    else if (conn->type == SYSM_NETCONN_UDP) {
        /*XXX*/
    }

    getter->netstat->all_inbound_total =
            getter->netstat->tcp_inbound_total;

    getter->netstat->all_outbound_total =
            getter->netstat->tcp_outbound_total;

    return SYSM_OK;
}

int sysm_net_stat_get(sysm_t *sysm, sysm_net_stat_t *net_stat, int flags)
{
    sysm_net_connection_walker_t walker;
    net_stat_getter_t getter;

    if (!sysm->net_listen)
    {
        sysm->net_listen = sysm_cache_new(32);
    }

    SYSM_ZERO(net_stat);

    getter.netstat = net_stat;

    walker.sysm = sysm;
    walker.data = &getter;
    walker.add_connection = net_stat_walker;

    walker.flags = flags;

    return sysm_net_connection_walk(&walker);

}

#define ifr_s_addr(ifr) \
    ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr

static void get_interface_type(sysm_net_interface_config_t *ifconfig,
                               int family) {
    char *type;
    switch (family) {
        case ARPHRD_SLIP:
            type = SYSM_NIC_SLIP;
            break;
        case ARPHRD_CSLIP:
            type = SYSM_NIC_CSLIP;
            break;
        case ARPHRD_SLIP6:
            type = SYSM_NIC_SLIP6;
            break;
        case ARPHRD_CSLIP6:
            type = SYSM_NIC_CSLIP6;
            break;
        case ARPHRD_ADAPT:
            type = SYSM_NIC_ADAPTIVE;
            break;
        case ARPHRD_ETHER:
            type = SYSM_NIC_ETHERNET;
            break;
        case ARPHRD_ASH:
            type = SYSM_NIC_ASH;
            break;
        case ARPHRD_FDDI:
            type = SYSM_NIC_FDDI;
            break;
        case ARPHRD_HIPPI:
            type = SYSM_NIC_HIPPI;
            break;
        case ARPHRD_AX25:
            type = SYSM_NIC_AX25;
            break;
        case ARPHRD_ROSE:
            type = SYSM_NIC_ROSE;
            break;
        case ARPHRD_NETROM:
            type = SYSM_NIC_NETROM;
            break;
        case ARPHRD_X25:
            type = SYSM_NIC_X25;
            break;
        case ARPHRD_TUNNEL:
            type = SYSM_NIC_TUNNEL;
            break;
        case ARPHRD_PPP:
            type = SYSM_NIC_PPP;
            break;
        case ARPHRD_CISCO:
            type = SYSM_NIC_HDLC;
            break;
        case ARPHRD_LAPB:
            type = SYSM_NIC_LAPB;
            break;
        case ARPHRD_ARCNET:
            type = SYSM_NIC_ARCNET;
            break;
        case ARPHRD_DLCI:
            type = SYSM_NIC_DLCI;
            break;
        case ARPHRD_FRAD:
            type = SYSM_NIC_FRAD;
            break;
        case ARPHRD_SIT:
            type = SYSM_NIC_SIT;
            break;
        case ARPHRD_IRDA:
            type = SYSM_NIC_IRDA;
            break;
        case ARPHRD_ECONET:
            type = SYSM_NIC_EC;
            break;
        default:
            type = SYSM_NIC_UNSPEC;
            break;
    }

    strcpy(ifconfig->type, type);

}

int sysm_net_interface_config_primary_get(sysm_t *sysm, sysm_net_interface_config_t *ifconfig)
{
    int i, status, found=0;
    sysm_net_interface_list_t iflist;
    sysm_net_interface_config_t possible_config;

    possible_config.flags = 0;

    if ((status = sysm_net_interface_list_get(sysm, &iflist)) != SYSM_OK) {
        return status;
    }

    for (i=0; i<iflist.number; i++) {
        status = sysm_net_interface_config_get(sysm,
                                                iflist.data[i], ifconfig);

        if ((status != SYSM_OK) ||
            (ifconfig->flags & SYSM_IFF_LOOPBACK) ||
            !ifconfig->hwaddr.addr.in)   /* no mac address */
        {
            continue;
        }

        if (!possible_config.flags) {
            /* save for later for use if we're not connected to the net
             * or all interfaces are aliases (e.g. solaris zone)
             */
            memcpy(&possible_config, ifconfig, sizeof(*ifconfig));
        }
        if (!ifconfig->address.addr.in) {
            continue; /* no ip address */
        }
        if (strchr(iflist.data[i], ':')) {
            continue; /* alias */
        }

        found = 1;
        break;
    }

    sysm_net_interface_list_destroy(&iflist);

    if (found) {
        return SYSM_OK;
    }
    else if (possible_config.flags) {
        memcpy(ifconfig, &possible_config, sizeof(*ifconfig));
        return SYSM_OK;
    }
    else {
        return ENXIO;
    }
}

int sysm_net_interface_config_get(sysm_t *sysm, const char *name, sysm_net_interface_config_t *ifconfig)
{
    int sock;
    struct ifreq ifr;

    if (!name)
        return sysm_net_interface_config_primary_get(sysm, ifconfig);

    SYSM_ZERO(ifconfig);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return errno;

    strcpy(ifconfig->name, name);
    strcpy(ifr.ifr_name, name);

    if (!ioctl(sock, SIOCGIFADDR, &ifr)) {
        sysm_net_address_set(ifconfig->address,
        ifr_s_addr(ifr));
    }

    if (!ioctl(sock, SIOCGIFNETMASK, &ifr)) {
        sysm_net_address_set(ifconfig->netmask, ifr_s_addr(ifr));
    }

    if (!ioctl(sock, SIOCGIFFLAGS, &ifr)) {
        sysm_uint64_t flags = (sysm_uint64_t) ifr.ifr_flags;
#ifndef IFF_DYNAMIC
#define IFF_DYNAMIC 0x8000
#endif
        int is_mcast = flags & IFF_MULTICAST;
        int is_slave = flags & IFF_SLAVE;
        int is_master = flags & IFF_MASTER;
        int is_dynamic = flags & IFF_DYNAMIC;
        flags &= ~(IFF_MULTICAST|IFF_SLAVE|IFF_MASTER);
        if (is_mcast) {
            flags |= SYSM_IFF_MULTICAST;
        }
        if (is_slave) {
            flags |= SYSM_IFF_SLAVE;
        }
        if (is_master) {
            flags |= SYSM_IFF_MASTER;
        }
        if (is_dynamic) {
            flags |= SYSM_IFF_DYNAMIC;
        }

        ifconfig->flags = flags;
    }
    else
    {
        close(sock);
        return errno;
    }

    if (ifconfig->flags & IFF_LOOPBACK) {
        sysm_net_address_set(ifconfig->destination,
                              ifconfig->address.addr.in);
        sysm_net_address_set(ifconfig->broadcast, 0);
        sysm_hwaddr_set_null(ifconfig);
        strcpy(ifconfig->type, SYSM_NIC_LOOPBACK);
    }
    else {
        if (!ioctl(sock, SIOCGIFDSTADDR, &ifr)) {
            sysm_net_address_set(ifconfig->destination,
                                  ifr_s_addr(ifr));
        }

        if (!ioctl(sock, SIOCGIFBRDADDR, &ifr)) {
            sysm_net_address_set(ifconfig->broadcast,
                                  ifr_s_addr(ifr));
        }

        if (!ioctl(sock, SIOCGIFHWADDR, &ifr)) {
            get_interface_type(ifconfig,
                               ifr.ifr_hwaddr.sa_family);
            sysm_net_address_mac_set(ifconfig->hwaddr, ifr.ifr_hwaddr.sa_data, IFHWADDRLEN);
        }
    }

    if (!ioctl(sock, SIOCGIFMTU, &ifr)) {
        ifconfig->mtu = ifr.ifr_mtu;
    }
    else
    {
        ifconfig->mtu = 0;
    }

    if (!ioctl(sock, SIOCGIFMETRIC, &ifr)) {
        ifconfig->metric = ifr.ifr_metric ? ifr.ifr_metric : 1;
    }

#if defined(SIOCGIFTXQLEN)
    if (!ioctl(sock, SIOCGIFTXQLEN, &ifr)) {
        ifconfig->tx_queue_len = ifr.ifr_qlen;
    }
    else {
        ifconfig->tx_queue_len = -1; /* net-tools behaviour */
    }
#else
    ifconfig->tx_queue_len = -1;
#endif

    close(sock);
    strcpy(ifconfig->description, ifconfig->name);

    sysm_net_interface_ipv6_config_init(ifconfig);
    sysm_net_interface_ipv6_config_get(sysm, name, ifconfig);

    return SYSM_OK;
}

int sysm_net_interface_ipv6_config_get(sysm_t *sysm, const char *name, sysm_net_interface_config_t *ifconfig)
{
    FILE *fp;
    char addr[32+1], ifname[8+1];
    int status = ENOENT;
    int idx, prefix, scope, flags;

    if (!(fp = fopen(PROC_FS_ROOT "net/if_inet6", "r"))) {
        return errno;
    }

    while (fscanf(fp, "%32s %02x %02x %02x %02x %8s\n",
                  addr, &idx, &prefix, &scope, &flags, ifname) != EOF)
    {
        if (strcmp(name, ifname) == 0) {
            status = SYSM_OK;
            break;
        }
    }

    fclose(fp);

    if (status == SYSM_OK) {
        int i=0;
        unsigned char *addr6 = (unsigned char *)&(ifconfig->address6.addr.in6);
        char *ptr = addr;

        for (i=0; i<16; i++, ptr+=2) {
            addr6[i] = (unsigned char)hex2int(ptr, 2);
        }

        ifconfig->prefix6_length = prefix;
        ifconfig->scope6 = scope;
    }

    return status;
}

int sysm_net_interface_stat_get(sysm_t *sysm, const char *name, sysm_net_interface_stat_t *ifstat)
{
    int found = 0;
    char buffer[BUFSIZ];
    FILE *fp = fopen(PROC_FS_ROOT "net/dev", "r");

    if (!fp) {
        return errno;
    }

    /* skip header */
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    while (fgets(buffer, sizeof(buffer), fp)) {
        char *ptr, *dev;

        dev = buffer;
        while (isspace(*dev)) {
            dev++;
        }

        if (!(ptr = strchr(dev, ':'))) {
            continue;
        }

        *ptr++ = 0;

        if (strcmp(dev, name) != 0) {
            continue;
        }

        found = 1;
        ifstat->rx_bytes    = strtoull(ptr, &ptr, 10);
        ifstat->rx_packets  = strtoull(ptr, &ptr, 10);;
        ifstat->rx_errors   = strtoull(ptr, &ptr, 10);
        ifstat->rx_dropped  = strtoull(ptr, &ptr, 10);
        ifstat->rx_overruns = strtoull(ptr, &ptr, 10);
        ifstat->rx_frame    = strtoull(ptr, &ptr, 10);

        /* skip: compressed multicast */
        ptr = sysm_skip_multiple_token(ptr, 2);

        ifstat->tx_bytes      = strtoull(ptr, &ptr, 10);
        ifstat->tx_packets    = strtoull(ptr, &ptr, 10);
        ifstat->tx_errors     = strtoull(ptr, &ptr, 10);
        ifstat->tx_dropped    = strtoull(ptr, &ptr, 10);
        ifstat->tx_overruns   = strtoull(ptr, &ptr, 10);
        ifstat->tx_collisions = strtoull(ptr, &ptr, 10);
        ifstat->tx_carrier    = strtoull(ptr, &ptr, 10);

        ifstat->speed         = -1;

        break;
    }

    fclose(fp);

    return found ? SYSM_OK : ENXIO;
}

int sysm_net_listen_address_get(sysm_t *sysm, unsigned long port, sysm_net_address_t *address)
{
    if (!sysm->net_listen ||
        !sysm_cache_find(sysm->net_listen, port))
    {
        sysm_net_stat_t netstat;
        int status =
                sysm_net_stat_get(sysm, &netstat,
                                   SYSM_NETCONN_SERVER|SYSM_NETCONN_TCP);

        if (status != SYSM_OK) {
            return status;
        }
    }

    if (sysm_cache_find(sysm->net_listen, port)) {
        void *value = sysm_cache_get(sysm->net_listen, port)->value;
        memcpy(address, value, sizeof(*address));
        return SYSM_OK;
    }
    else {
        return ENOENT;
    }
}