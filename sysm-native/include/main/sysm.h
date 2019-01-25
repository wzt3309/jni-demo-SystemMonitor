//
// Created by wzt on 11/6/17.
//

#ifndef HBDC_SYSM_NATIVE_SYSM_H
#define HBDC_SYSM_NATIVE_SYSM_H

#include <limits.h>
#include <sys/types.h>

#if defined(_LP64)      || \
    defined(__LP64__)
#define SYSM_64BIT
#endif

#if defined(PATH_MAX)
#   define SYSM_PATH_MAX PATH_MAX
#elif defined(MAXPATHLEN)
#   define SYSM_PATH_MAX MAXPATHLEN
#else
#   define SYSM_PATH_MAX 4096
#endif

//set int and long length
#if ULONG_MAX > 4294967295UL
typedef unsigned long sysm_uint64_t;
typedef long sysm_int64_t;
#else
typedef unsigned long long sysm_uint64_t;
typedef long long sysm_int64_t;
#endif

#define SYSM_OK 0
#define SYSM_START_ERROR 20000 //not conflict with errno range
#define SYSM_ENOTIMPL   (SYSM_START_ERROR + 1)

#define SYSM_PROC_NAME_LEN 128
#define SYSM_PROC_LIST_MAX 256
#define SYSM_PROC_ARGS_MAX 12
#define SYSM_CPU_MAX 4
#define SYSM_FS_MAX 10
#define SYSM_NET_ROUTE_LIST_MAX 6
#define SYSM_NET_IFLIST_MAX 20
#define SYSM_NET_CONNLIST_MAX 20
#define SYSM_WHO_LIST_MAX 12

#define SYSM_MSEC 1000L
#define SYSM_USEC 1000000L
#define SYSM_NSEC 1000000000L

#define SYSM_NETCONN_CLIENT 0x01
#define SYSM_NETCONN_SERVER 0x02
#define SYSM_NETCONN_TCP  0x10
#define SYSM_NETCONN_UDP  0x20
#define SYSM_NETCONN_RAW  0x40

#define SYSM_LAST_PROC_EXPIRE 2

#define SYSM_PROC_STATE_SLEEP  'S'
#define SYSM_PROC_STATE_RUN    'R'
#define SYSM_PROC_STATE_STOP   'T'
#define SYSM_PROC_STATE_ZOMBIE 'Z'
#define SYSM_PROC_STATE_IDLE   'D'

#define sysm_net_address_set(a, val) \
    (a).addr.in = val; \
    (a).family = SYSM_AF_INET

#define sysm_hwaddr_set_null(ifconfig) \
    SYSM_ZERO(&ifconfig->hwaddr.addr.mac); \
    ifconfig->hwaddr.family = SYSM_AF_LINK

#define sysm_net_address_mac_set(a, val, len) \
    memcpy(&((a).addr.mac), val, len); \
    (a).family = SYSM_AF_LINK

#define sysm_net_interface_ipv6_config_init(ifconfig) \
    ifconfig->address6.family = SYSM_AF_INET6; \
    ifconfig->prefix6_length = 0; \
    ifconfig->scope6 = 0

#define SYSM_RTF_UP      0x1
#define SYSM_RTF_GATEWAY 0x2
#define SYSM_RTF_HOST    0x4

#define SYSM_IFF_UP          0x1
#define SYSM_IFF_BROADCAST   0x2
#define SYSM_IFF_DEBUG       0x4
#define SYSM_IFF_LOOPBACK    0x8
#define SYSM_IFF_POINTOPOINT 0x10
#define SYSM_IFF_NOTRAILERS  0x20
#define SYSM_IFF_RUNNING     0x40
#define SYSM_IFF_NOARP       0x80
#define SYSM_IFF_PROMISC     0x100
#define SYSM_IFF_ALLMULTI    0x200
#define SYSM_IFF_MULTICAST   0x800
#define SYSM_IFF_SLAVE       0x1000
#define SYSM_IFF_MASTER      0x2000
#define SYSM_IFF_DYNAMIC     0x4000

#define SYSM_NIC_LOOPBACK "Local Loopback"
#define SYSM_NIC_UNSPEC   "UNSPEC"
#define SYSM_NIC_SLIP     "Serial Line IP"
#define SYSM_NIC_CSLIP    "VJ Serial Line IP"
#define SYSM_NIC_SLIP6    "6-bit Serial Line IP"
#define SYSM_NIC_CSLIP6   "VJ 6-bit Serial Line IP"
#define SYSM_NIC_ADAPTIVE "Adaptive Serial Line IP"
#define SYSM_NIC_ETHERNET "Ethernet"
#define SYSM_NIC_ASH      "Ash"
#define SYSM_NIC_FDDI     "Fiber Distributed Data Interface"
#define SYSM_NIC_HIPPI    "HIPPI"
#define SYSM_NIC_AX25     "AMPR AX.25"
#define SYSM_NIC_ROSE     "AMPR ROSE"
#define SYSM_NIC_NETROM   "AMPR NET/ROM"
#define SYSM_NIC_X25      "generic X.25"
#define SYSM_NIC_TUNNEL   "IPIP Tunnel"
#define SYSM_NIC_PPP      "Point-to-Point Protocol"
#define SYSM_NIC_HDLC     "(Cisco)-HDLC"
#define SYSM_NIC_LAPB     "LAPB"
#define SYSM_NIC_ARCNET   "ARCnet"
#define SYSM_NIC_DLCI     "Frame Relay DLCI"
#define SYSM_NIC_FRAD     "Frame Relay Access Device"
#define SYSM_NIC_SIT      "IPv6-in-IPv4"
#define SYSM_NIC_IRDA     "IrLAP"
#define SYSM_NIC_EC       "Econet"

#define SYSM_ZERO(s) \
    memset(s, '\0', sizeof(*(s)))

/*cpu time clock to ms*/
#define SYSM_TICK2MSEC(s) \
    ((sysm_uint64_t)(s) * ((sysm_uint64_t)SYSM_MSEC / (double)sysm->ticks))

#define SYSM_TICK2USEC(s) \
    ((sysm_uint64_t)(s) * ((sysm_uint64_t)SYSM_USEC / (double)sysm->ticks))

#define SYSM_TICK2NSEC(s) \
    ((sysm_uint64_t)(s) * ((sysm_uint64_t)SYSM_NSEC / (double)sysm->ticks))

#define SYSM_CPU_INFO_LIST_GROW(cpu_infos) \
    if ((cpu_infos)->number >= (cpu_infos)->size) { \
        sysm_cpu_info_list_grow(cpu_infos); \
    }

#define SYSM_CPU_LIST_GROW(cpu_list) \
    if ((cpu_list)->number >= (cpu_list)->size) { \
        sysm_cpu_list_grow(cpu_list); \
    }

#define SYSM_PROC_LIST_GROW(proc_list) \
    if ((proc_list)->number >= (proc_list)->size) { \
        sysm_proc_list_grow(proc_list); \
    }

#define SYSM_PROC_ARGS_GROW(proc_args) \
    if ((proc_args)->number >= (proc_args)->size) { \
        sysm_proc_args_grow(proc_args); \
    }

#define SYSM_NET_CONNLIST_GROW(conn_list) \
    if ((conn_list)->number >= (conn_list)->size) { \
        sysm_net_connection_list_grow(conn_list); \
    }

#define SYSM_DISK_STATS_INIT(disk) \
    (disk)->reads = (disk)->writes = \
    (disk)->read_bytes = (disk)->write_bytes = \
    (disk)->rtime = (disk)->wtime = (disk)->qtime = (disk)->time = \
    (disk)->util = (disk)->service_time = -1; \
    (disk)->snaptime = 0

#define SYSM_FILE_SYSTEM_LIST_GROW(fslist) \
    if ((fslist)->number >= (fslist)->size) { \
        sysm_file_system_list_grow(fslist); \
    }

#define SYSM_NET_ROUTE_LIST_GROW(routelist) \
    if ((routelist)->number >= (routelist)->size) { \
        sysm_net_route_list_grow(routelist); \
    }

#define SYSM_NET_IFLIST_GROW(iflist) \
    if ((iflist)->number >= (iflist)->size) { \
        sysm_net_interface_list_grow(iflist); \
    }

#define SYSM_WHO_LIST_GROW(who_list) \
    if ((who_list)->number >= (who_list)->size) { \
        sysm_who_list_grow(who_list); \
    }

#define SYSM_FSDEV_ID(sb) \
    (S_ISBLK((sb).st_mode) ? (sb).st_rdev : ((sb).st_ino + (sb).st_dev))

typedef struct {
    const char *version;
    int major, minor;
} sysm_version_t;

/*cpu information*/
typedef struct {
    char vendor[128];
    char model[128];
    int mhz;
    int mhz_max;
    int mhz_min;
    sysm_uint64_t cache_size;
    int total_sockets;          //physical cores
    int total_cores;            //logical cores
    int cores_per_socket;
} sysm_cpu_info_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sysm_cpu_info_t *data;
} sysm_cpu_info_list_t;

/*cpu clock time use just like command "top" in linux*/
typedef struct {
    sysm_uint64_t
        user,
        sys,
        nice,
        idle,
        wait,
        hi,
        si,
        steal,
        total;
} sysm_cpu_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sysm_cpu_t *data;
} sysm_cpu_list_t;

/*memory*/
typedef struct {
    sysm_uint64_t
        total,
        mtrr,
        used,
        free,
        actual_used,
        actual_free;
    double used_percent;
    double free_percent;
} sysm_memory_t;

/*swap*/
typedef struct {
    sysm_uint64_t
        total,
        used,
        free,
        page_in,
        page_out;
} sysm_swap_t;

typedef struct {
    double
        user,
        sys,
        nice,
        idle,
        wait,
        hi,
        si,
        steal,
        combined;
} sysm_cpu_prec_t;

/*LoadAverage per 1 5 15 minutes*/
typedef struct {
    double loadavg[3];
} sysm_loadavg_t;

/*process list*/
typedef struct {
    unsigned long number;
    unsigned long size;
    pid_t *data;
} sysm_proc_list_t;

/*a process's(find by pid) args*/
typedef struct {
    unsigned long number;
    unsigned long size;
    char **data;
} sysm_proc_args_t;

typedef struct {
    double uptime;
} sysm_uptime_t;

typedef struct {
    sysm_uint64_t
        start_time,
        user,
        sys,
        total,
        last_time;
    double percent;
} sysm_proc_cpu_t;

typedef struct {
    sysm_uint64_t
            start_time,
            user,
            sys,
            total;
} sysm_proc_time_t;

typedef enum {
    SYSM_PROC_ENV_ALL,
    SYSM_PROC_ENV_KEY
} SYSM_PROC_ENV;

typedef struct {
    void *data;
    SYSM_PROC_ENV type;

    const char *key;
    int klen;

    int (*env_getter)(void *, const char *, int, char *, int);
} sysm_proc_env_t;

typedef struct {
    char name[SYSM_PATH_MAX+1];
    char cwd[SYSM_PATH_MAX+1];
    char root[SYSM_PATH_MAX+1];
} sysm_proc_exe_t;

typedef struct {
    sysm_uint64_t
            size,
            resident,
            share,
            minor_faults,
            major_faults,
            page_faults;
} sysm_proc_mem_t;

typedef struct {
    pid_t uid;
    pid_t gid;
    pid_t euid;
    pid_t egid;
} sysm_proc_own_t;

#define SYSM_OWNER_MAX 512

typedef struct {
    char user[SYSM_OWNER_MAX];
    char group[SYSM_OWNER_MAX];
} sysm_proc_owner_t;

typedef struct {
    sysm_uint64_t total;
    sysm_uint64_t sleeping;
    sysm_uint64_t running;
    sysm_uint64_t zombie;
    sysm_uint64_t stopped;
    sysm_uint64_t idle;
    sysm_uint64_t threads;
} sysm_proc_stat_t;

typedef struct {
    char name[SYSM_PROC_NAME_LEN];
    char state;
    pid_t ppid;
    int tty;
    int priority;
    int nice;
    int processor;
    sysm_uint64_t threads;
} sysm_proc_state_t;

typedef struct {
    /* RLIMIT_CPU */
    sysm_uint64_t cpu_cur, cpu_max;
    /* RLIMIT_FSIZE */
    sysm_uint64_t file_size_cur, file_size_max;
    /* PIPE_BUF */
    sysm_uint64_t pipe_size_cur, pipe_size_max;
    /* RLIMIT_DATA */
    sysm_uint64_t data_cur, data_max;
    /* RLIMIT_STACK */
    sysm_uint64_t stack_cur, stack_max;
    /* RLIMIT_CORE */
    sysm_uint64_t core_cur, core_max;
    /* RLIMIT_RSS */
    sysm_uint64_t memory_cur, memory_max;
    /* RLIMIT_NPROC */
    sysm_uint64_t processes_cur, processes_max;
    /* RLIMIT_NOFILE */
    sysm_uint64_t open_files_cur, open_files_max;
    /* RLIMIT_AS */
    sysm_uint64_t virtual_memory_cur, virtual_memory_max;
} sysm_resource_limit_t;

typedef struct {
    sysm_uint64_t reads;
    sysm_uint64_t writes;
    sysm_uint64_t write_bytes;
    sysm_uint64_t read_bytes;
    sysm_uint64_t rtime;
    sysm_uint64_t wtime;
    sysm_uint64_t qtime;
    sysm_uint64_t time;
    sysm_uint64_t snaptime;
    double service_time;
    double util;
} sysm_disk_usage_t;

#define SYSM_DEV_PREFIX "/dev/"

#define SYSM_NAME_IS_DEV(dev) \
    (strncmp(dev, SYSM_DEV_PREFIX, strlen(SYSM_DEV_PREFIX)) == 0)

typedef struct {
    char name[256];
    int is_partition;
    sysm_disk_usage_t disk;
} sysm_iodev_t;

typedef enum {
    SYSM_FSTYPE_UNKNOWN,
    SYSM_FSTYPE_NONE,
    SYSM_FSTYPE_LOCAL_DISK,
    SYSM_FSTYPE_NETWORK,
    SYSM_FSTYPE_RAM_DISK,
    SYSM_FSTYPE_CDROM,
    SYSM_FSTYPE_SWAP,
    SYSM_FSTYPE_MAX
} sysm_file_system_type_e;

#define SYSM_FS_NAME_LEN SYSM_PATH_MAX
#define SYSM_FS_INFO_LEN 256

typedef struct {
    char dir_name[SYSM_FS_NAME_LEN];
    char dev_name[SYSM_FS_NAME_LEN];
    char type_name[SYSM_FS_INFO_LEN];     /* e.g. "local" */
    char sys_type_name[SYSM_FS_INFO_LEN]; /* e.g. "ext3" */
    char options[SYSM_FS_INFO_LEN];
    sysm_file_system_type_e type;
    unsigned long flags;
} sysm_file_system_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sysm_file_system_t *data;
} sysm_file_system_list_t;

enum {
    SYSM_TCP_ESTABLISHED = 1,
    SYSM_TCP_SYN_SENT,
    SYSM_TCP_SYN_RECV,
    SYSM_TCP_FIN_WAIT1,
    SYSM_TCP_FIN_WAIT2,
    SYSM_TCP_TIME_WAIT,
    SYSM_TCP_CLOSE,
    SYSM_TCP_CLOSE_WAIT,
    SYSM_TCP_LAST_ACK,
    SYSM_TCP_LISTEN,
    SYSM_TCP_CLOSING,
    SYSM_TCP_IDLE,
    SYSM_TCP_BOUND,
    SYSM_TCP_UNKNOWN
};

typedef enum {
    SYSM_AF_UNSPEC,
    SYSM_AF_INET,
    SYSM_AF_INET6,
    SYSM_AF_LINK
} SYSM_AF;

typedef struct {
    SYSM_AF family;
    union {
        unsigned int in;
        unsigned int in6[4];
        unsigned char mac[8];
    } addr;
} sysm_net_address_t;

#define SYSM_INET6_ADDRSTRLEN 46
#define MAX_INTERFACE_NAME_LEN 256
#define SYSM_MAXDOMAINNAMELEN 256
#define SYSM_MAXHOSTNAMELEN 256

typedef struct {
    char default_gateway[SYSM_INET6_ADDRSTRLEN];
    char default_gateway_interface[MAX_INTERFACE_NAME_LEN];
    char host_name[SYSM_MAXHOSTNAMELEN];
    char domain_name[SYSM_MAXDOMAINNAMELEN];
    char primary_dns[SYSM_INET6_ADDRSTRLEN];
    char secondary_dns[SYSM_INET6_ADDRSTRLEN];
} sysm_net_info_t;

typedef struct {
    sysm_net_address_t destination;
    sysm_net_address_t gateway;
    sysm_net_address_t mask;
    sysm_uint64_t
            flags,
            refcnt,
            use,
            metric,
            mtu,
            window,
            irtt;
    char ifname[MAX_INTERFACE_NAME_LEN];
} sysm_net_route_t;

typedef struct {
    int tcp_states[SYSM_TCP_UNKNOWN];
    unsigned int tcp_inbound_total;
    unsigned int tcp_outbound_total;
    unsigned int all_inbound_total;
    unsigned int all_outbound_total;
} sysm_net_stat_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sysm_net_route_t *data;
} sysm_net_route_list_t;

typedef struct {
    char name[MAX_INTERFACE_NAME_LEN];
    char type[64];
    char description[256];
    sysm_net_address_t hwaddr;
    sysm_net_address_t address;
    sysm_net_address_t destination;
    sysm_net_address_t broadcast;
    sysm_net_address_t netmask;
    sysm_net_address_t address6;
    int prefix6_length;
    int scope6;
    sysm_uint64_t
            flags,
            mtu,
            metric;
    int tx_queue_len;
} sysm_net_interface_config_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    char **data;
} sysm_net_interface_list_t;

typedef struct {
    sysm_uint64_t
    /* received */
            rx_packets,
            rx_bytes,
            rx_errors,
            rx_dropped,
            rx_overruns,
            rx_frame,
    /* transmitted */
            tx_packets,
            tx_bytes,
            tx_errors,
            tx_dropped,
            tx_overruns,
            tx_collisions,
            tx_carrier,
            speed;
} sysm_net_interface_stat_t;

typedef struct {
    unsigned long lo_port;
    sysm_net_address_t lo_address;
    unsigned long re_port;
    sysm_net_address_t re_address;
    pid_t uid;
    unsigned long inode;
    int type;
    int state;
    unsigned long se_q;
    unsigned long re_q;
} sysm_net_connection_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sysm_net_connection_t *data;
} sysm_net_connection_list_t;

typedef struct {
    char user[32];
    char device[32];
    char host[256];
    sysm_uint64_t time;
} sysm_who_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sysm_who_t *data;
} sysm_who_list_t;

typedef struct sysm_net_connection_walker_t sysm_net_connection_walker_t;

/*process stat*/
typedef struct {
    pid_t pid;
    pid_t ppid;
    time_t mtime;
    sysm_uint64_t vsize;
    sysm_uint64_t rss;
    sysm_uint64_t minor_faults;
    sysm_uint64_t major_faults;
    int tty;
    int pri;
    int ni;
    sysm_uint64_t start_time;
    sysm_uint64_t utime;    //user time
    sysm_uint64_t stime;    //sys time
    char name[SYSM_PROC_NAME_LEN];
    char state;
    int processor;
}linux_proc_stat_t;

/*where to get iostat information*/
typedef enum {
    IOSTAT_NONE,
    IOSTAT_PARTITIONS,  // /proc/partitions
    IOSTAT_DISKSTATS,   // /proc/diskstats
    IOSTAT_SYS          // /sys/block/
}linux_iostat_e;

typedef struct sysm_t sysm_t;

typedef struct sysm_cache_entry_t sysm_cache_entry_t;
struct sysm_cache_entry_t {
    sysm_cache_entry_t *next;
    sysm_uint64_t id;
    void *value;
    sysm_uint64_t last_access_time;
};

typedef struct {
    sysm_cache_entry_t **entries;
    unsigned int count, size;
    void (*free_value)(void *ptr);
    sysm_uint64_t entry_expire_period;
    sysm_uint64_t cleanup_period_millis;
    sysm_uint64_t last_cleanup_time;
} sysm_cache_t; /*like a map store sysm_cache_entry_t(has id)*/

#define SYSM_CACHE \
    sysm_cache_t *proc_cpu; \
    sysm_cache_t *fsdev; \
    sysm_cache_t *net_listen;

/*store all system information and cache*/
struct sysm_t {
    int cpu_list_cores;                     //flag whether list all processor
    int lcpu;                               //core per socket
    unsigned int ncpu;                      //logical cores
    unsigned long version;
    unsigned long boot_time;
    long ticks;                             //number of clock ticks per second
    pid_t pid;
    char errbuf[255];
    char *ifconf_buf;
    int ifconf_len;
    char *self_path;
    sysm_proc_list_t *pids;
    int pagesize;
    int mtrr;                               //memroy type range registers MTRR
    int proc_signal_offset;
    linux_proc_stat_t last_proc_stat;
    linux_iostat_e iostat;
    char *proc_net;
    int has_nptl;   //if has Native POSIX Thread Library（NPTL）

    SYSM_CACHE
};

struct sysm_net_connection_walker_t {
    sysm_t *sysm;
    int flags;
    void *data;
    int (*add_connection)(sysm_net_connection_walker_t *walker,
                          sysm_net_connection_t *conn);
};

char *sysm_strerror(sysm_t *sysm, int err);
char *sysm_strerror_get(int err, char *errbuf, size_t buflen);
int sysm_user_name_get(int uid, char *buf, int buflen);
int sysm_group_name_get(int gid, char *buf, int buflen);

sysm_version_t *sysm_version_get(void);

int sysm_open(sysm_t **sysm);
int sysm_close(sysm_t *sysm);
int sysm_os_open(sysm_t **sysm);
int sysm_os_close(sysm_t *sysm);

int sysm_proc_signal(pid_t pid, int signum);

pid_t sysm_pid_get(sysm_t *sysm);

int sysm_proc_list_get(sysm_t *sysm, sysm_proc_list_t *proc_list);
int sysm_proc_list_create(sysm_proc_list_t *proc_list);
int sysm_proc_list_grow(sysm_proc_list_t *proc_list);
int sysm_proc_list_destroy(sysm_proc_list_t *proc_list);
int sysm_os_proc_list_get(sysm_t *sysm, sysm_proc_list_t *proc_list);

int sysm_proc_args_get(sysm_t *sysm, pid_t pid, sysm_proc_args_t *proc_args);
int sysm_proc_args_create(sysm_proc_args_t *proc_args);
int sysm_proc_args_grow(sysm_proc_args_t *proc_args);
int sysm_proc_args_destroy(sysm_proc_args_t *proc_args);
int sysm_os_proc_args_get(sysm_t *sysm, pid_t pid, sysm_proc_args_t *proc_args);

int sysm_proc_port_get(sysm_t *sysm, int protocol, unsigned long port, pid_t *pid);

int sysm_cpu_info_list_get(sysm_t *sysm, sysm_cpu_info_list_t *cpu_infos);
int sysm_cpu_info_list_create(sysm_cpu_info_list_t *cpu_infos);
int sysm_cpu_info_list_grow(sysm_cpu_info_list_t *cpulist);
int sysm_cpu_info_list_destroy(sysm_cpu_info_list_t *cpu_infos);

int sysm_cpu_list_get(sysm_t *sysm, sysm_cpu_list_t *cpu_list);
int sysm_cpu_list_create(sysm_cpu_list_t *cpu_list);
int sysm_cpu_list_grow(sysm_cpu_list_t *cpu_list);
int sysm_cpu_list_destroy(sysm_cpu_list_t *cpu_list);

int sysm_file_system_list_get(sysm_t *sysm, sysm_file_system_list_t *fslist);
int sysm_file_system_list_create(sysm_file_system_list_t *fslist);
int sysm_file_system_list_grow(sysm_file_system_list_t *fslist);
int sysm_file_system_list_destroy(sysm_file_system_list_t *fslist);

int sysm_net_route_list_get(sysm_t *sysm, sysm_net_route_list_t *route_list);
int sysm_net_route_list_create(sysm_net_route_list_t *route_list);
int sysm_net_route_list_grow(sysm_net_route_list_t *route_list);
int sysm_net_route_list_destroy(sysm_net_route_list_t *route_list);

int sysm_net_interface_list_get(sysm_t *sysm, sysm_net_interface_list_t *iflist);
int sysm_net_interface_list_create(sysm_net_interface_list_t *iflist);
int sysm_net_interface_list_grow(sysm_net_interface_list_t *iflist);
int sysm_net_interface_list_destroy(sysm_net_interface_list_t *iflist);

int sysm_net_connection_list_get(sysm_t *sysm, sysm_net_connection_list_t *conn_list, int flags);
int sysm_net_connection_list_create(sysm_net_connection_list_t *conn_list);
int sysm_net_connection_list_grow(sysm_net_connection_list_t *conn_list);
int sysm_net_connection_list_destroy(sysm_net_connection_list_t *conn_list);

int sysm_who_list_get(sysm_t *sysm, sysm_who_list_t *who_list);
int sysm_who_list_create( sysm_who_list_t *who_list);
int sysm_who_list_grow(sysm_who_list_t *who_list);
int sysm_who_list_destroy(sysm_who_list_t *who_list);

int sysm_net_connection_walk(sysm_net_connection_walker_t *walker);

int sysm_memory_get(sysm_t *sysm, sysm_memory_t *memory);
int sysm_swap_get(sysm_swap_t *swap);
int sysm_cpu_get(sysm_t *sysm, sysm_cpu_t *cpu);
int sysm_loadavg_get(sysm_t *sysm, sysm_loadavg_t *loadavg);
int sysm_uptime_get(sysm_uptime_t *uptime);
int sysm_proc_cpu_get(sysm_t *sysm, pid_t pid, sysm_proc_cpu_t *proc_cpu);
int sysm_proc_time_get(sysm_t *sysm, pid_t pid, sysm_proc_time_t *proc_time);
int sysm_proc_env_get(sysm_t *sysm, pid_t pid, sysm_proc_env_t *proc_env);
int sysm_proc_exe_get(sysm_t *sysm, pid_t pid, sysm_proc_exe_t *proc_exe);
int sysm_proc_mem_get(sysm_t *sysm, pid_t pid, sysm_proc_mem_t *proc_mem);
int sysm_proc_own_get(sysm_t *sysm, pid_t pid, sysm_proc_own_t *proc_own);
int sysm_proc_owner_get(sysm_t *sysm, pid_t pid, sysm_proc_owner_t *proc_owner);
int sysm_proc_stat_get(sysm_t *sysm, sysm_proc_stat_t *proc_stat);
int sysm_proc_state_get(sysm_t *sysm, pid_t pid, sysm_proc_state_t *proc_state);
int sysm_resource_limit_get(sysm_t *sysm, sysm_resource_limit_t *rlimit);
int sysm_disk_usage_get(sysm_t *sysm, const char *name, sysm_disk_usage_t *disk);
int sysm_os_fs_type_get(sysm_file_system_t *fsp);
void sysm_fs_type_get(sysm_file_system_t *fsp);
int sysm_net_info_get(sysm_t *sysm, sysm_net_info_t *net_info);
int sysm_net_stat_port_get(sysm_t *sysm, sysm_net_stat_t *netstat, int flags, sysm_net_address_t *address, unsigned long port);
int sysm_net_stat_get(sysm_t *sysm, sysm_net_stat_t *net_stat, int flags);
int sysm_net_interface_config_primary_get(sysm_t *sysm, sysm_net_interface_config_t *ifconfig);
int sysm_net_interface_config_get(sysm_t *sysm, const char *name, sysm_net_interface_config_t *ifconfig);
int sysm_net_interface_ipv6_config_get(sysm_t *sysm, const char *name, sysm_net_interface_config_t *ifconfig);
int sysm_net_interface_stat_get(sysm_t *sysm, const char *name, sysm_net_interface_stat_t *ifstat);
int sysm_net_listen_address_get(sysm_t *sysm, unsigned long port, sysm_net_address_t *address);


#endif //HBDC_SYSM_NATIVE_SYSM_H
