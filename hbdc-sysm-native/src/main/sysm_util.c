//
// Created by wzt on 11/7/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "sysm.h"
#include "sysm_util.h"

//Skip space + non space combination
char *sysm_skip_token(char *p)
{
    while (sysm_isspace(*p)) p++;
    while (*p && !sysm_isspace(*p)) p++;
    return p;
}

char *sysm_skip_multiple_token(char *p, int count)
{
    int i;

    for (i = 0; i < count; i++) {
        p = sysm_skip_token(p);
    }

    return p;
}

typedef struct {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
} sysm_cpuid_t;

#if defined(__GNUC__) && !defined(__sun)
#if defined(__i386__)
#define SYSM_HAS_CPUID
static void sysm_cpuid(unsigned int request, sysm_cpuid_t *id)
{
    asm volatile ("mov %%ebx, %%esi\n\t"
                  "cpuid\n\t"
                  "xchgl %%ebx, %%esi"
                  : "=a" (id->eax),
                    "=S" (id->ebx),
                    "=c" (id->ecx),
                    "=d" (id->edx)
                  : "0" (request)
                  : "memory");
}
#elif defined(__amd64__)
#define SYSM_HAS_CPUID
static void sysm_cpuid(unsigned int request, sysm_cpuid_t *id)
{
    asm volatile ("cpuid\n\t"
                  : "=a" (id->eax),
                    "=b" (id->ebx),
                    "=c" (id->ecx),
                    "=d" (id->edx)
                  : "0" (request)
                  : "memory");
}
#endif
#endif
#define INTEL_ID 0x756e6547
#define AMD_ID   0x68747541

int sysm_cpu_core_count(sysm_t *sysm)
{
#if defined(SYSM_HAS_CPUID)
    sysm_cpuid_t id;

    if (sysm->lcpu == -1)
    {
        sysm->lcpu = 1;

        sysm_cpuid(0, &id);

        if (id.ebx == INTEL_ID || id.ebx == AMD_ID)
        {
            sysm_cpuid(1, &id);

            if (id.edx & (1 << 28))
                sysm->lcpu = (id.ebx & 0x00FF0000) >> 16;
        }
    }
#endif
}

int sysm_cpu_core_rollup(sysm_t *sysm)
{
#ifdef SYSM_HAS_CPUID
    int rollup = sysm->lcpu == -1;
    (void) sysm_cpu_core_count(sysm);

    if (!sysm->cpu_list_cores)
        if (rollup && sysm->lcpu > 1)
            return 1;
            
#endif
    return 0;
}

typedef struct {
    char *name;  /* search */
    size_t len;
    char *rname; /* replace */
    size_t rlen;
} cpu_model_str_t;

#define CPU_MODEL_ENT_R(s, r) \
    { s, sizeof(s)-1, r, sizeof(r) }

#define CPU_MODEL_ENT(s) \
    CPU_MODEL_ENT_R(s, s)

static const cpu_model_str_t cpu_models[] = {
        /* intel */
        CPU_MODEL_ENT("Xeon"),
        CPU_MODEL_ENT_R("XEON", "Xeon"),
        CPU_MODEL_ENT("Pentium III"),
        CPU_MODEL_ENT("Pentium II"),
        CPU_MODEL_ENT_R("Pentium(R) III", "Pentium III"),
        CPU_MODEL_ENT_R("Pentium(R) 4", "Pentium 4"),
        CPU_MODEL_ENT_R("Pentium(R) M", "Pentium M"),
        CPU_MODEL_ENT("Pentium Pro"),
        CPU_MODEL_ENT("Celeron"),

        /* amd */
        CPU_MODEL_ENT("Opteron"),
        CPU_MODEL_ENT("Athlon"),
        CPU_MODEL_ENT("Duron"),
        CPU_MODEL_ENT_R("K6(tm)-III", "K6 III"),
        CPU_MODEL_ENT_R("K6(tm) 3D+", "K6 3D+"),
        { NULL }
};

void sysm_cpu_model_adjust(sysm_cpu_info_t *info)
{
    size_t len;
    char model[128], *ptr = model, *end;
    memcpy(model, info->model, sizeof(model));

    //trim start and end spaces
    len = strlen(model);
    end = &model[len - 1];
    while (*ptr == ' ') ++ptr;
    while (*end == ' ') *end-- = '\0';

    //e.g. Intel(R) Core(TM) i7-8550U CPU @ 1.80GHz
    len = strlen(info->vendor);
    if (strncmp(ptr, info->vendor, len) == 0)
    {
        ptr += len;
        if (*ptr == '(' && *(ptr + 1) == 'R' && *(ptr + 2) == ')')
        {
            ptr += 3;   //remove R
        }
        while (*ptr == ' ') ++ptr;
    }

    if (*ptr == '-') ++ptr; //e.g. was AMD-K6

    for (int i = 0;cpu_models[i].name;i++)
    {
        const cpu_model_str_t *cpu_model = &cpu_models[i];
        if (strncmp(ptr, cpu_model->name, cpu_model->len) == 0)
        {
            memcpy(info->model, cpu_model->rname, cpu_model->rlen);
            return;
        }
    }

    strcpy(info->model, ptr);
}

int sysm_cpu_perc_cal(sysm_cpu_t *prev, sysm_cpu_t *curr, sysm_cpu_prec_t *perc)
{
    double diff_user, diff_sys, diff_nice, diff_idle;
    double diff_wait, diff_hi, diff_si, diff_steal;
    double diff_total;

    diff_user = curr->user - prev->user;
    diff_sys = curr->sys - prev->sys;
    diff_nice = curr->nice - prev->nice;
    diff_idle = curr->idle - prev->idle;
    diff_wait = curr->wait - prev->wait;
    diff_hi = curr->hi - prev->hi;
    diff_si = curr->si - prev->si;
    diff_steal = curr->steal - prev->steal;

    diff_user = diff_user < 0 ? 0 : diff_user;
    diff_sys = diff_sys < 0 ? 0 : diff_sys;
    diff_nice = diff_nice < 0 ? 0 : diff_nice;
    diff_idle = diff_idle < 0 ? 0 : diff_idle;
    diff_wait = diff_wait < 0 ? 0 : diff_wait;
    diff_hi = diff_hi < 0 ? 0 : diff_hi;
    diff_si = diff_si < 0 ? 0 : diff_si;
    diff_steal = diff_steal < 0 ? 0 : diff_steal;

    diff_total = diff_user + diff_sys + diff_nice + diff_idle +
                 diff_wait + diff_hi  + diff_si   + diff_steal;

    perc->user = diff_user / diff_total;
    perc->sys = diff_sys / diff_total;
    perc->nice = diff_nice / diff_total;
    perc->idle = diff_idle / diff_total;
    perc->wait = diff_wait / diff_total;
    perc->hi = diff_hi / diff_total;
    perc->si = diff_si / diff_total;
    perc->steal = diff_steal / diff_total;

    perc->combined = perc->user + perc->sys + perc->nice + perc->wait;

    return SYSM_OK;
}

int sysm_file2str(const char *fname, char *buffer, size_t buflen)
{
    ssize_t len;
    int status;
    int fd = open(fname, O_RDONLY);

    if (fd < 0)
        return ENOENT;

    if ((len = read(fd, buffer, buflen)) < 0)
    {
        status = errno;
    }
    else
    {
        status = SYSM_OK;
        buffer[len] = '\0';
    }

    close(fd);
    return status;
}

char *sysm_uitoa(char *buffer, unsigned int n, int *len)
{
    char *start = buffer + UITOA_BUFFER_SIZE - 1;
    *start = 0;

    do {
        *--start = (char) ('0' + (n % 10));
        ++*len;
        n /= 10;
    }while (n);

    return start;
}

int sysm_proc_file2str(char *buffer, int buflen, pid_t pid, const char *fname, int fname_len)
{
    int retval;

    buffer = sysm_proc_filename(buffer, buflen, pid, fname, fname_len);

    retval = sysm_file2str(buffer, buffer, (size_t) buflen);

    if (retval != SYSM_OK)
    {
        switch (retval)
        {
            case ENOENT:
                retval = ESRCH;
            default:
                break;
        }
    }

    return retval;
}

char *sysm_proc_filename(char *buffer, int buflen, pid_t pid, const char *fname, int fname_len)
{
    int len = 0;
    char *ptr = buffer;
    char pid_buf[UITOA_BUFFER_SIZE];
    char *pid_str = sysm_uitoa(pid_buf, (unsigned int) pid, &len);
    assert((unsigned int) buflen >=
                   strlen(PROC_FS_ROOT) + UITOA_BUFFER_SIZE + fname_len + 1);

    memcpy(ptr, PROC_FS_ROOT, strlen(PROC_FS_ROOT));
    ptr += strlen(PROC_FS_ROOT);

    memcpy(ptr, pid_str, (size_t) len);
    ptr += len;

    memcpy(ptr, fname, (size_t) fname_len);
    ptr += fname_len;
    *ptr = '\0';
    return buffer;
}

sysm_uint64_t sysm_time_now_millis(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (sysm_uint64_t) (((tv.tv_sec * SYSM_USEC) + tv.tv_usec) / SYSM_MSEC);
}

#define ENTRIES_SIZE(n) \
    (sizeof(sysm_cache_entry_t *) * (n))

static void free_value(void *ptr)
{
    free(ptr);
}

sysm_cache_t *sysm_expired_cache_new(unsigned int size, sysm_uint64_t clean_up_period_millis, sysm_uint64_t entry_expire_period)
{
    sysm_cache_t *table = malloc(sizeof(*table));
    table->count = 0;
    table->size = size;
    table->entries = malloc(ENTRIES_SIZE(size));
    memset(table->entries, '\0', ENTRIES_SIZE(size));
    table->free_value = free_value;
    table->cleanup_period_millis = clean_up_period_millis;
    table->last_cleanup_time = sysm_time_now_millis();
    table->entry_expire_period = entry_expire_period;
    return table;
}

sysm_cache_t *sysm_cache_new(unsigned int size)
{
    return sysm_expired_cache_new(size, -1, -1);
}

#define SYSM_CACHE_IX(t, k) \
    t->entries + (k % t->size)

static void sysm_cache_rehash(sysm_cache_t *table)
{
    unsigned int new_size = table->count * 2 + 1;
    sysm_cache_entry_t **entries = table->entries;
    sysm_cache_entry_t **new_entries = malloc(ENTRIES_SIZE(new_size));

    memset(new_entries, '\0', ENTRIES_SIZE(new_size));

    for (int i = 0;i < table->size;i++)
    {
        sysm_cache_entry_t *entry = *entries++;

        while (entry)
        {
            sysm_cache_entry_t *next = entry->next;
            sysm_uint64_t hash = entry->id % new_size;

            entry->next = new_entries[hash];
            entry = next;
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->size = new_size;
}

static void sysm_perform_cleanup_if_necessary(sysm_cache_t *table)
{
    sysm_uint64_t current_time;
    sysm_cache_entry_t **entries;
    if (table->cleanup_period_millis == -1)
        return;
    current_time = sysm_time_now_millis();
    if ((current_time - table->last_cleanup_time) < table->cleanup_period_millis)
        return;

    entries = table->entries;

    table->last_cleanup_time = current_time;

    for (int i = 0;i < table->size;i++)
    {
        sysm_cache_entry_t *entry, *ptr, *entry_prev = NULL, **entry_in_table;
        entry_in_table = entries;
        entry = *entries++;

        while (entry)
        {
            sysm_uint64_t period_with_no_acess = current_time - entry->last_access_time;
            ptr = entry->next;
            if (table->entry_expire_period < period_with_no_acess)
            {
                if (entry->value)
                    table->free_value(entry->value);
                free(entry);
                table->count--;
                if (entry_prev != NULL)
                    entry_prev->next = ptr;
                else
                    *entry_in_table = ptr;
            }
            else
            {
                entry_prev = entry;
            }
            entry = ptr;
        }
    }
    if (table->count < (table->size / 4))
        sysm_cache_rehash(table);
}

sysm_cache_entry_t *sysm_cache_get(sysm_cache_t *table, sysm_uint64_t key)
{
    sysm_cache_entry_t *entry, **ptr;
    sysm_perform_cleanup_if_necessary(table);

    for (ptr = SYSM_CACHE_IX(table, key), entry = *ptr;
         entry;
         ptr = &entry->next, entry = *ptr)
    {
        if (entry->id == key)
        {
            entry->last_access_time = sysm_time_now_millis();
            return entry;
        }
    }

    if (++table->count > table->size)
    {
        sysm_cache_rehash(table);

        for (ptr = SYSM_CACHE_IX(table, key), entry = *ptr;
             entry;
             ptr = &entry->next, entry = *ptr);
    }

    *ptr = entry = malloc(sizeof(*entry));
    entry->id = key;
    entry->value = NULL;
    entry->next = NULL;
    entry->last_access_time = sysm_time_now_millis();

    return entry;
}

sysm_cache_entry_t *sysm_cache_find(sysm_cache_t *table, sysm_uint64_t key)
{
    sysm_cache_entry_t *entry, **ptr;
    sysm_perform_cleanup_if_necessary(table);

    for (ptr = SYSM_CACHE_IX(table, key), entry = *ptr;
         entry;
         ptr = &entry->next, entry = *ptr)
    {
        if (entry->id == key) {
            entry->last_access_time = sysm_time_now_millis();
            return entry;
        }
    }

    return NULL;
}

void sysm_cache_destroy(sysm_cache_t *table)
{
    sysm_cache_entry_t **entries = table->entries;

    for (int i = 0;i < table->size;i++)
    {
        sysm_cache_entry_t *entry, *ptr;
        entry = *entries++;

        while (entry)
        {
            if (entry->value)
                table->free_value(entry->value);
            ptr = entry->next;
            free(entry);
            entry = ptr;
        }
    }

    free(table->entries);
    free(table);
}

sysm_iodev_t *sysm_iodev_get(sysm_t *sysm, const char *dirname)
{
    sysm_cache_entry_t *entry;
    struct stat sb;
    sysm_uint64_t id;
    sysm_file_system_list_t fslist;
    int i, status, is_dev=0;
    char dev_name[SYSM_FS_NAME_LEN];

    if (!sysm->fsdev) {
        sysm->fsdev = sysm_cache_new(15);
    }

    if (*dirname != '/') {
        snprintf(dev_name, sizeof(dev_name),
                 SYSM_DEV_PREFIX "%s", dirname);
        dirname = dev_name;
        is_dev = 1;
    }
    else if (SYSM_NAME_IS_DEV(dirname)) {
        is_dev = 1;
    }

    if (stat(dirname, &sb) < 0) {
        return NULL;
    }

    id = SYSM_FSDEV_ID(sb);

    entry = sysm_cache_get(sysm->fsdev, id);

    if (entry->value != NULL) {
        return (sysm_iodev_t *)entry->value;
    }

    if (is_dev) {
        sysm_iodev_t *iodev;
        entry->value = iodev = malloc(sizeof(*iodev));
        SYSM_ZERO(iodev);
        strcpy(iodev->name, dirname);

        return iodev;
    }

    status = sysm_file_system_list_get(sysm, &fslist);

    if (status != SYSM_OK) {
        return NULL;
    }

    for (i=0; i<fslist.number; i++) {
        sysm_file_system_t *fsp = &fslist.data[i];

        if (fsp->type == SYSM_FSTYPE_LOCAL_DISK) {
            int retval = stat(fsp->dir_name, &sb);
            sysm_cache_entry_t *ent;

            if (retval < 0) {
                continue; /* cant cache w/o inode */
            }

            ent = sysm_cache_get(sysm->fsdev, SYSM_FSDEV_ID(sb));
            if (ent->value) {
                continue; /* already cached */
            }

            if (SYSM_NAME_IS_DEV(fsp->dev_name)) {
                sysm_iodev_t *iodev;
                ent->value = iodev = malloc(sizeof(*iodev));
                SYSM_ZERO(iodev);
                iodev->is_partition = 1;
                strcpy(iodev->name, fsp->dev_name);
            }
        }
    }

    sysm_file_system_list_destroy(&fslist);

    if (entry->value &&
        (((sysm_iodev_t *)entry->value)->name[0] != '\0'))
    {
        return (sysm_iodev_t *)entry->value;
    }
    else {
        return NULL;
    }
}

int sysm_net_address_equals(sysm_net_address_t *addr1, sysm_net_address_t *addr2)
{
    if (addr1->family != addr2->family)
    {
        return EINVAL;
    }

    switch (addr1->family)
    {
        case SYSM_AF_INET:
            return memcmp(&addr1->addr.in, &addr2->addr.in, sizeof(addr1->addr.in));
        case SYSM_AF_INET6:
            return memcmp(&addr1->addr.in6, &addr2->addr.in6, sizeof(addr1->addr.in6));
        case SYSM_AF_LINK:
            return memcmp(&addr1->addr.mac, &addr2->addr.mac, sizeof(addr1->addr.mac));
        default:
            return EINVAL;
    }
}

static int sysm_inet_ntoa(unsigned int address, char *addr_str)
{
    char *next=addr_str;
    int n=0;
    const unsigned char *src =
            (const unsigned char *)&address;

    do {
        unsigned char u = *src++;
        if (u > 99) {
            *next++ = '0' + u/100;
            u %= 100;
            *next++ = '0' + u/10;
            u %= 10;
        }
        else if (u > 9) {
            *next++ = '0' + u/10;
            u %= 10;
        }
        *next++ = '0' + u;
        *next++ = '.';
        n++;
    } while (n < 4);

    *--next = 0;

    return SYSM_OK;
}

static int sysm_ether_ntoa(char *buff, unsigned char *ptr)
{
    sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X",
            (ptr[0] & 0xff), (ptr[1] & 0xff), (ptr[2] & 0xff),
            (ptr[3] & 0xff), (ptr[4] & 0xff), (ptr[5] & 0xff));
    return SYSM_OK;
}

int sysm_net_address_to_string(sysm_t *sysm, sysm_net_address_t *address, char *addr_str)
{
    *addr_str = '\0';
    switch (address->family)
    {
        case SYSM_AF_INET6:
            if (inet_ntop(AF_INET6, (const void *)&address->addr.in6,
                           addr_str, SYSM_INET6_ADDRSTRLEN))
                return SYSM_OK;
            else
                return errno;
        case SYSM_AF_INET:
            return sysm_inet_ntoa(address->addr.in, addr_str);
        case SYSM_AF_UNSPEC:
            return sysm_inet_ntoa(0, addr_str); /*XXX*/
        case SYSM_AF_LINK:
            return sysm_ether_ntoa(addr_str, &address->addr.mac[0]);
        default:
            return EINVAL;
    }
}