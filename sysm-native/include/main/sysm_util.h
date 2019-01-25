//
// Created by wzt on 11/7/17.
//

#ifndef HBDC_SYSM_NATIVE_SYSM_UTIL_H
#define HBDC_SYSM_NATIVE_SYSM_UTIL_H

#ifndef PROC_FS_ROOT
#define PROC_FS_ROOT "/proc/"
#endif

#define UITOA_BUFFER_SIZE \
    (sizeof(int) * 3 + 1)

#define sysm_isspace(c) \
    (isspace(((unsigned char)(c))))

#define sysm_cpu_socket_count(sysm) \
    ((sysm)->ncpu < (sysm)->lcpu) ? (sysm)->ncpu : \
    ((sysm)->ncpu / (sysm)->lcpu)

#define SYSM_PROC_FILENAME(buffer, pid, fname) \
    sysm_proc_filename(buffer, sizeof(buffer), \
                       pid, fname, strlen(fname))

/*Get str from /proc/$pid$/fname*/
#define SYSM_PROC_FILE2STR(buffer, pid, fname) \
    sysm_proc_file2str(buffer, sizeof(buffer), \
                       pid, fname, strlen(fname))

#define SYSM_SKIP_SPACE(ptr) \
    while (isspace(*ptr)) ++ptr;

char *sysm_skip_token(char *p);
char *sysm_skip_multiple_token(char *p, int count);
int sysm_cpu_core_count(sysm_t *sigar);
int sysm_cpu_core_rollup(sysm_t *sysm);
void sysm_cpu_model_adjust(sysm_cpu_info_t *info);
int sysm_cpu_perc_cal(sysm_cpu_t *prev, sysm_cpu_t *curr, sysm_cpu_prec_t *perc);
int sysm_file2str(const char *fname, char *buffer, size_t buflen);
char *sysm_uitoa(char *buf, unsigned int n, int *len);
int sysm_proc_file2str(char *buffer, int buflen, pid_t pid, const char *fname, int fname_len);
char *sysm_proc_filename(char *buffer, int buflen, pid_t pid, const char *fname, int fname_len);
sysm_uint64_t sysm_time_now_millis(void);

sysm_cache_t *sysm_expired_cache_new(unsigned int size, sysm_uint64_t clean_up_period_millis, sysm_uint64_t extry_expire_period);
sysm_cache_t *sysm_cache_new(unsigned int size);
sysm_cache_entry_t *sysm_cache_get(sysm_cache_t *table, sysm_uint64_t key);
sysm_cache_entry_t *sysm_cache_find(sysm_cache_t *table, sysm_uint64_t key);
void sysm_cache_destroy(sysm_cache_t *table);

sysm_iodev_t *sysm_iodev_get(sysm_t *sysm, const char *dirname);

int sysm_net_address_equals(sysm_net_address_t *addr1, sysm_net_address_t *addr2);
int sysm_net_address_to_string(sysm_t *sysm, sysm_net_address_t *address, char *addr_str);

#endif //HBDC_SYSM_NATIVE_SYSM_UTIL_H