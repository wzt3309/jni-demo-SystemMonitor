//
// Created by wzt on 11/6/17.
//
#ifndef HBDC_SYSM_NATIVE_JAVASYSM_H
#define HBDC_SYSM_NATIVE_JAVASYSM_H

#include <jni.h>

#define JENV (*env)

#define SYSM_PACKAGE "edu/scut/cs/sysm/"

#define SYSM_PACKAGE_BEAN "edu/scut/cs/sysm/bean/"

#define SYSM_PACKAGE_EX "edu/scut/cs/sysm/exception/"

#define SYSM_JNI(m) JNICALL Java_edu_scut_cs_sysm_Sysm_##m

//edu.scut.cs.sysm.bean.* native method
#define SYSM_JNIx(m) JNICALL Java_edu_scut_cs_sysm_bean_##m

#define SYSM_FIND_CLASS(name) \
    JENV->FindClass(env, SYSM_PACKAGE name)

#define SYSM_FIND_CLASS_BEAN(name) \
    JENV->FindClass(env, SYSM_PACKAGE_BEAN name)

#define SYSM_FIND_CLASS_EX(name) \
    JENV->FindClass(env, SYSM_PACKAGE_EX name)

#define SYSM_CLASS_SIG(name) \
    "L" SYSM_PACKAGE name ";"

#define SYSM_CLASS_SIG_BEAN(name) \
    "L" SYSM_PACKAGE_BEAN name ";"

#define SYSM_CLASS_SIG_EX(name) \
    "L" SYSM_PACKAGE_EX name ";"

#define SYSM_CHEX if (JENV->ExceptionCheck(env)) return NULL

#define JSYSM_FIELDS_MAX 40

#define SetStringField(env, obj, fieldID, val) \
    SetObjectField(env, obj, fieldID, JENV->NewStringUTF(env, val))

#define SetNetAddressField(env, obj, fieldID, val) \
    SetObjectField(env, obj, fieldID, jnet_address_to_string(env, sysm, &val))

/*
 * The bean's(edu/scut/cs/sysm/bean) fields
 */
#define JSYSM_FIELDS_MEMORY 0
#   define JSYSM_FIELDS_MEMORY_TOTAL 0
#   define JSYSM_FIELDS_MEMORY_MTRR 1
#   define JSYSM_FIELDS_USED 2
#   define JSYSM_FIELDS_FREE 3
#   define JSYSM_FIELDS_ACTUAL_USED 4
#   define JSYSM_FIELDS_ACTUAL_FREE 5
#   define JSYSM_FIELDS_USED_PERCENT 6
#   define JSYSM_FIELDS_FREE_PERCENT 7
#   define JSYSM_FIELDS_MEMORY_MAX 8

#define JSYSM_FIELDS_SWAP 1
#   define JSYSM_FIELDS_SWAP_TOTAL 0
#   define JSYSM_FIELDS_SWAP_USED 1
#   define JSYSM_FIELDS_SWAP_FREE 2
#   define JSYSM_FIELDS_SWAP_PAGE_IN 3
#   define JSYSM_FIELDS_SWAP_PAGE_OUT 4
#   define JSYSM_FIELDS_SWAP_MAX 5

#define JSYSM_FIELDS_CPU 2
#   define JSYSM_FIELDS_CPU_USER 0
#   define JSYSM_FIELDS_CPU_SYS 1
#   define JSYSM_FIELDS_CPU_NICE 2
#   define JSYSM_FIELDS_CPU_IDLE 3
#   define JSYSM_FIELDS_CPU_WAIT 4
#   define JSYSM_FIELDS_CPU_HI 5
#   define JSYSM_FIELDS_CPU_SI 6
#   define JSYSM_FIELDS_CPU_STEAL 7
#   define JSYSM_FIELDS_CPU_TOTAL 8
#   define JSYSM_FIELDS_CPU_MAX 9

#define JSYSM_FIELDS_CPUINFO 3
#    define JSYSM_FIELDS_CPUINFO_VENDOR 0
#    define JSYSM_FIELDS_CPUINFO_MODEL 1
#    define JSYSM_FIELDS_CPUINFO_MHZ 2
#    define JSYSM_FIELDS_CPUINFO_MHZ_MAX 3
#    define JSYSM_FIELDS_CPUINFO_MHZ_MIN 4
#    define JSYSM_FIELDS_CPUINFO_CACHE_SIZE 5
#    define JSYSM_FIELDS_CPUINFO_TOTAL_CORES 6
#    define JSYSM_FIELDS_CPUINFO_TOTAL_SOCKETS 7
#    define JSYSM_FIELDS_CPUINFO_CORES_PER_SOCKET 8
#    define JSYSM_FIELDS_CPUINFO_MAX 9

#define JSYSM_FIELDS_CPUPERCENTAGE 4
#    define JSYSM_FIELDS_CPUPERCENTAGE_USER 0
#    define JSYSM_FIELDS_CPUPERCENTAGE_SYS 1
#    define JSYSM_FIELDS_CPUPERCENTAGE_NICE 2
#    define JSYSM_FIELDS_CPUPERCENTAGE_IDLE 3
#    define JSYSM_FIELDS_CPUPERCENTAGE_WAIT 4
#    define JSYSM_FIELDS_CPUPERCENTAGE_HI 5
#    define JSYSM_FIELDS_CPUPERCENTAGE_SI 6
#    define JSYSM_FIELDS_CPUPERCENTAGE_STEAL 7
#    define JSYSM_FIELDS_CPUPERCENTAGE_COMBINED 8
#    define JSYSM_FIELDS_CPUPERCENTAGE_MAX 9

#define JSYSM_FIELDS_UPTIME 5
#   define JSYSM_FIELDS_UPTIME_UPTIME 0
#   define JSYSM_FIELDS_UPTIME_MAX 1

#define JSYSM_FIELDS_PROCCPU 6
#   define JSYSM_FIELDS_PROCCPU_PERCENT 0
#   define JSYSM_FIELDS_PROCCPU_LAST_TIME 1
#   define JSYSM_FIELDS_PROCCPU_START_TIME 2
#   define JSYSM_FIELDS_PROCCPU_USER 3
#   define JSYSM_FIELDS_PROCCPU_SYS 4
#   define JSYSM_FIELDS_PROCCPU_TOTAL 5
#   define JSYSM_FIELDS_PROCCPU_MAX 6

#define JSYSM_FIELDS_PROCEXE 7
#   define JSYSM_FIELDS_PROCEXE_NAME 0
#   define JSYSM_FIELDS_PROCEXE_CWD 1
#   define JSYSM_FIELDS_PROCEXE_MAX 2

#define JSYSM_FIELDS_PROCMEM 8
#   define JSYSM_FIELDS_PROCMEM_SIZE 0
#   define JSYSM_FIELDS_PROCMEM_RESIDENT 1
#   define JSYSM_FIELDS_PROCMEM_SHARE 2
#   define JSYSM_FIELDS_PROCMEM_MINOR_FAULTS 3
#   define JSYSM_FIELDS_PROCMEM_MAJOR_FAULTS 4
#   define JSYSM_FIELDS_PROCMEM_PAGE_FAULTS 5
#   define JSYSM_FIELDS_PROCMEM_MAX 6

#define JSYSM_FIELDS_PROCOWN 9
#   define JSYSM_FIELDS_PROCOWN_UID 0
#   define JSYSM_FIELDS_PROCOWN_GID 1
#   define JSYSM_FIELDS_PROCOWN_EUID 2
#   define JSYSM_FIELDS_PROCOWN_EGID 3
#   define JSYSM_FIELDS_PROCOWN_MAX 4

#define JSYSM_FIELDS_PROCOWNER 10
#   define JSYSM_FIELDS_PROCOWNER_USER 0
#   define JSYSM_FIELDS_PROCOWNER_GROUP 1
#   define JSYSM_FIELDS_PROCOWNER_MAX 2

#define JSYSM_FIELDS_PROCSTAT 11
#   define JSYSM_FIELDS_PROCSTAT_TOTAL 0
#   define JSYSM_FIELDS_PROCSTAT_IDLE 1
#   define JSYSM_FIELDS_PROCSTAT_RUNNING 2
#   define JSYSM_FIELDS_PROCSTAT_SLEEPING 3
#   define JSYSM_FIELDS_PROCSTAT_STOPPED 4
#   define JSYSM_FIELDS_PROCSTAT_ZOMBIE 5
#   define JSYSM_FIELDS_PROCSTAT_THREADS 6
#   define JSYSM_FIELDS_PROCSTAT_MAX 7

#define JSYSM_FIELDS_PROCSTATE 12
#   define JSYSM_FIELDS_PROCSTATE_STATE 0
#   define JSYSM_FIELDS_PROCSTATE_NAME 1
#   define JSYSM_FIELDS_PROCSTATE_PPID 2
#   define JSYSM_FIELDS_PROCSTATE_TTY 3
#   define JSYSM_FIELDS_PROCSTATE_NICE 4
#   define JSYSM_FIELDS_PROCSTATE_PRIORITY 5
#   define JSYSM_FIELDS_PROCSTATE_THREADS 6
#   define JSYSM_FIELDS_PROCSTATE_PROCESSOR 7
#   define JSYSM_FIELDS_PROCSTATE_MAX 8

#define JSYSM_FIELDS_PROCTIME 13
#   define JSYSM_FIELDS_PROCTIME_START_TIME 0
#   define JSYSM_FIELDS_PROCTIME_USER 1
#   define JSYSM_FIELDS_PROCTIME_SYS 2
#   define JSYSM_FIELDS_PROCTIME_TOTAL 3
#   define JSYSM_FIELDS_PROCTIME_MAX 4

#define JSYSM_FIELDS_RESOURCELIMIT 14
#   define JSYSM_FIELDS_RESOURCELIMIT_CPU_CUR 0
#   define JSYSM_FIELDS_RESOURCELIMIT_CPU_MAX 1
#   define JSYSM_FIELDS_RESOURCELIMIT_FILE_SIZE_CUR 2
#   define JSYSM_FIELDS_RESOURCELIMIT_FILE_SIZE_MAX 3
#   define JSYSM_FIELDS_RESOURCELIMIT_PIPE_SIZE_MAX 4
#   define JSYSM_FIELDS_RESOURCELIMIT_PIPE_SIZE_CUR 5
#   define JSYSM_FIELDS_RESOURCELIMIT_DATA_CUR 6
#   define JSYSM_FIELDS_RESOURCELIMIT_DATA_MAX 7
#   define JSYSM_FIELDS_RESOURCELIMIT_STACK_CUR 8
#   define JSYSM_FIELDS_RESOURCELIMIT_STACK_MAX 9
#   define JSYSM_FIELDS_RESOURCELIMIT_CORE_CUR 10
#   define JSYSM_FIELDS_RESOURCELIMIT_CORE_MAX 11
#   define JSYSM_FIELDS_RESOURCELIMIT_MEMORY_CUR 12
#   define JSYSM_FIELDS_RESOURCELIMIT_MEMORY_MAX 13
#   define JSYSM_FIELDS_RESOURCELIMIT_PROCESSES_CUR 14
#   define JSYSM_FIELDS_RESOURCELIMIT_PROCESSES_MAX 15
#   define JSYSM_FIELDS_RESOURCELIMIT_OPEN_FILES_CUR 16
#   define JSYSM_FIELDS_RESOURCELIMIT_OPEN_FILES_MAX 17
#   define JSYSM_FIELDS_RESOURCELIMIT_VIRTUAL_MEMORY_CUR 18
#   define JSYSM_FIELDS_RESOURCELIMIT_VIRTUAL_MEMORY_MAX 19
#   define JSYSM_FIELDS_RESOURCELIMIT_MAX 20

#define JSYSM_FIELDS_DISKUSAGE 15
#   define JSYSM_FIELDS_DISKUSAGE_READS 0
#   define JSYSM_FIELDS_DISKUSAGE_WRITES 1
#   define JSYSM_FIELDS_DISKUSAGE_READ_BYTES 2
#   define JSYSM_FIELDS_DISKUSAGE_WRITE_BYTES 3
#   define JSYSM_FIELDS_DISKUSAGE_UTIL 4
#   define JSYSM_FIELDS_DISKUSAGE_SERVICE_TIME 5
#   define JSYSM_FIELDS_DISKUSAGE_MAX 6

#define JSYSM_FIELDS_NETSTAT 16
#   define JSYSM_FIELDS_NETSTAT_TCP_INBOUND_TOTAL 0
#   define JSYSM_FIELDS_NETSTAT_TCP_OUTBOUND_TOTAL 1
#   define JSYSM_FIELDS_NETSTAT_ALL_INBOUND_TOTAL 2
#   define JSYSM_FIELDS_NETSTAT_ALL_OUTBOUND_TOTAL 3
#   define JSYSM_FIELDS_NETSTAT_MAX 4

#define JSYSM_FIELDS_NETINFO 17
#   define JSYSM_FIELDS_NETINFO_DEFAULT_GATEWAY 0
#   define JSYSM_FIELDS_NETINFO_DEFAULT_GATEWAY_INTERFACE 1
#   define JSYSM_FIELDS_NETINFO_HOST_NAME 2
#   define JSYSM_FIELDS_NETINFO_DOMAIN_NAME 3
#   define JSYSM_FIELDS_NETINFO_PRIMARY_DNS 4
#   define JSYSM_FIELDS_NETINFO_SECONDARY_DNS 5
#   define JSYSM_FIELDS_NETINFO_MAX 6

#define JSYSM_FIELDS_NETINTERFACECONFIG 18
#   define JSYSM_FIELDS_NETINTERFACECONFIG_NAME 0
#   define JSYSM_FIELDS_NETINTERFACECONFIG_HWADDR 1
#   define JSYSM_FIELDS_NETINTERFACECONFIG_TYPE 2
#   define JSYSM_FIELDS_NETINTERFACECONFIG_DESCRIPTION 3
#   define JSYSM_FIELDS_NETINTERFACECONFIG_ADDRESS 4
#   define JSYSM_FIELDS_NETINTERFACECONFIG_ADDRESS6 5
#   define JSYSM_FIELDS_NETINTERFACECONFIG_PREFIX6_LENGTH 6
#   define JSYSM_FIELDS_NETINTERFACECONFIG_SCOPE6 7
#   define JSYSM_FIELDS_NETINTERFACECONFIG_DESTINATION 8
#   define JSYSM_FIELDS_NETINTERFACECONFIG_BROADCAST 9
#   define JSYSM_FIELDS_NETINTERFACECONFIG_NETMASK 10
#   define JSYSM_FIELDS_NETINTERFACECONFIG_FLAGS 11
#   define JSYSM_FIELDS_NETINTERFACECONFIG_MTU 12
#   define JSYSM_FIELDS_NETINTERFACECONFIG_METRIC 13
#   define JSYSM_FIELDS_NETINTERFACECONFIG_TX_QUEUE_LEN 14
#   define JSYSM_FIELDS_NETINTERFACECONFIG_MAX 15

#define JSYSM_FIELDS_NETINTERFACESTAT 19
#   define JSYSM_FIELDS_NETINTERFACESTAT_RX_BYTES 0
#   define JSYSM_FIELDS_NETINTERFACESTAT_RX_PACKETS 1
#   define JSYSM_FIELDS_NETINTERFACESTAT_RX_ERRORS 2
#   define JSYSM_FIELDS_NETINTERFACESTAT_RX_DROPPED 3
#   define JSYSM_FIELDS_NETINTERFACESTAT_RX_OVERRUNS 4
#   define JSYSM_FIELDS_NETINTERFACESTAT_RX_FRAME 5
#   define JSYSM_FIELDS_NETINTERFACESTAT_TX_BYTES 6
#   define JSYSM_FIELDS_NETINTERFACESTAT_TX_PACKETS 7
#   define JSYSM_FIELDS_NETINTERFACESTAT_TX_ERRORS 8
#   define JSYSM_FIELDS_NETINTERFACESTAT_TX_DROPPED 9
#   define JSYSM_FIELDS_NETINTERFACESTAT_TX_OVERRUNS 10
#   define JSYSM_FIELDS_NETINTERFACESTAT_TX_COLLISIONS 11
#   define JSYSM_FIELDS_NETINTERFACESTAT_TX_CARRIER 12
#   define JSYSM_FIELDS_NETINTERFACESTAT_SPEED 13
#   define JSYSM_FIELDS_NETINTERFACESTAT_MAX 14

#define JSYSM_FIELDS_NETROUTE 20
#   define JSYSM_FIELDS_NETROUTE_DESTINATION 0
#   define JSYSM_FIELDS_NETROUTE_GATEWAY 1
#   define JSYSM_FIELDS_NETROUTE_FLAGS 2
#   define JSYSM_FIELDS_NETROUTE_REFCNT 3
#   define JSYSM_FIELDS_NETROUTE_USE 4
#   define JSYSM_FIELDS_NETROUTE_METRIC 5
#   define JSYSM_FIELDS_NETROUTE_MASK 6
#   define JSYSM_FIELDS_NETROUTE_MTU 7
#   define JSYSM_FIELDS_NETROUTE_WINDOW 8
#   define JSYSM_FIELDS_NETROUTE_IRTT 9
#   define JSYSM_FIELDS_NETROUTE_IFNAME 10
#   define JSYSM_FIELDS_NETROUTE_MAX 11

#define JSYSM_FIELDS_NETCONNECTION 21
#   define JSYSM_FIELDS_NETCONNECTION_LOCAL_PORT 0
#   define JSYSM_FIELDS_NETCONNECTION_LOCAL_ADDRESS 1
#   define JSYSM_FIELDS_NETCONNECTION_REMOTE_PORT 2
#   define JSYSM_FIELDS_NETCONNECTION_REMOTE_ADDRESS 3
#   define JSYSM_FIELDS_NETCONNECTION_TYPE 4
#   define JSYSM_FIELDS_NETCONNECTION_STATE 5
#   define JSYSM_FIELDS_NETCONNECTION_SEND_QUEUE 6
#   define JSYSM_FIELDS_NETCONNECTION_RECEIVE_QUEUE 7
#   define JSYSM_FIELDS_NETCONNECTION_MAX 8

#define JSYSM_FIELDS_WHO 22
#   define JSYSM_FIELDS_WHO_USER 0
#   define JSYSM_FIELDS_WHO_DEVICE 1
#   define JSYSM_FIELDS_WHO_HOST 2
#   define JSYSM_FIELDS_WHO_TIME 3
#   define JSYSM_FIELDS_WHO_MAX 4

#define JAVA_SYSM_INIT_FIELDS_MEMORY(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_MEMORY]) { \
        jsysm->fields[JSYSM_FIELDS_MEMORY] = \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_MEMORY])); \
        jsysm->fields[JSYSM_FIELDS_MEMORY]->classref = \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_MEMORY]->ids = \
            malloc(JSYSM_FIELDS_MEMORY_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_MEMORY]->ids));\
        jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_MEMORY_TOTAL] = \
            JENV->GetFieldID(env, cls, "total", "J"); \
        jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_MEMORY_MTRR] = \
            JENV->GetFieldID(env, cls, "mtrr", "J"); \
        jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_USED] = \
            JENV->GetFieldID(env, cls, "used", "J"); \
        jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_FREE] = \
            JENV->GetFieldID(env, cls, "free", "J"); \
        jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_ACTUAL_USED] = \
            JENV->GetFieldID(env, cls, "actualUsed", "J"); \
        jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_ACTUAL_FREE] = \
            JENV->GetFieldID(env, cls, "actualFree", "J"); \
        jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_USED_PERCENT] = \
            JENV->GetFieldID(env, cls, "usedPercent", "D"); \
        jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_FREE_PERCENT] = \
            JENV->GetFieldID(env, cls, "freePercent", "D"); \
    }

#define JAVA_SYSM_SET_FIELDS_MEMORY(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_MEMORY_TOTAL], s.total); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_MEMORY_MTRR], s.mtrr); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_USED], s.used); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_FREE], s.free); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_ACTUAL_USED], s.actual_used); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_ACTUAL_FREE], s.actual_free); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_USED_PERCENT], s.used_percent); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_MEMORY]->ids[JSYSM_FIELDS_FREE_PERCENT], s.free_percent);

#define JAVA_SYSM_INIT_FIELDS_SWAP(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_SWAP]) { \
        jsysm->fields[JSYSM_FIELDS_SWAP] = \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_SWAP])); \
        jsysm->fields[JSYSM_FIELDS_SWAP]->classref = \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_SWAP]->ids = \
            malloc(JSYSM_FIELDS_SWAP_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_SWAP]->ids)); \
        jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_TOTAL] = \
            JENV->GetFieldID(env, cls, "total", "J"); \
        jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_USED] = \
            JENV->GetFieldID(env, cls, "used", "J"); \
        jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_FREE] = \
            JENV->GetFieldID(env, cls, "free", "J"); \
        jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_PAGE_IN] = \
            JENV->GetFieldID(env, cls, "pageIn", "J"); \
        jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_PAGE_OUT] = \
            JENV->GetFieldID(env, cls, "pageOut", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_SWAP(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_TOTAL], s.total); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_USED], s.used); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_FREE], s.free); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_PAGE_IN], s.page_in); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_SWAP]->ids[JSYSM_FIELDS_SWAP_PAGE_OUT], s.page_out);

#define JAVA_SYSM_INIT_FIELDS_CPU(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_CPU]) { \
        jsysm->fields[JSYSM_FIELDS_CPU] = \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_CPU])); \
        jsysm->fields[JSYSM_FIELDS_CPU]->classref = \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids = \
            malloc(JSYSM_FIELDS_CPU_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_CPU]->ids)); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_USER] = \
             JENV->GetFieldID(env, cls, "user", "J"); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_SYS] = \
             JENV->GetFieldID(env, cls, "sys", "J"); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_NICE] = \
             JENV->GetFieldID(env, cls, "nice", "J"); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_IDLE] = \
             JENV->GetFieldID(env, cls, "idle", "J"); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_WAIT] = \
             JENV->GetFieldID(env, cls, "wait", "J"); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_HI] = \
             JENV->GetFieldID(env, cls, "hi", "J"); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_SI] = \
             JENV->GetFieldID(env, cls, "si", "J"); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_STEAL] = \
             JENV->GetFieldID(env, cls, "steal", "J"); \
        jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_TOTAL] = \
             JENV->GetFieldID(env, cls, "total", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_CPU(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_USER], s.user); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_SYS], s.sys); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_NICE], s.nice); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_IDLE], s.idle); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_WAIT], s.wait); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_HI], s.hi); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_SI], s.si); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_STEAL], s.steal); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_TOTAL], s.total); \

#define JAVA_SYSM_GET_FIELDS_CPU(obj, s) \
    s.user = JENV->GetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_USER]); \
    s.sys = JENV->GetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_SYS]); \
    s.nice = JENV->GetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_NICE]); \
    s.idle = JENV->GetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_IDLE]); \
    s.wait = JENV->GetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_WAIT]); \
    s.hi = JENV->GetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_HI]); \
    s.si = JENV->GetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_SI]); \
    s.steal = JENV->GetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_STEAL]); \
    s.total = JENV->GetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPU]->ids[JSYSM_FIELDS_CPU_TOTAL]); \


#define JAVA_SYSM_INIT_FIELDS_CPUINFO(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_CPUINFO]) { \
        jsysm->fields[JSYSM_FIELDS_CPUINFO] = \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_CPUINFO])); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->classref = \
            JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids = \
            malloc(JSYSM_FIELDS_CPUINFO_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids)); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_VENDOR] = \
            JENV->GetFieldID(env, cls, "vendor", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_MODEL] = \
            JENV->GetFieldID(env, cls, "model", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_MHZ] = \
            JENV->GetFieldID(env, cls, "mhz", "I"); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_MHZ_MAX] = \
            JENV->GetFieldID(env, cls, "mhzMax", "I"); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_MHZ_MIN] = \
            JENV->GetFieldID(env, cls, "mhzMin", "I"); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_CACHE_SIZE] = \
            JENV->GetFieldID(env, cls, "cacheSize", "J"); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_TOTAL_CORES] = \
            JENV->GetFieldID(env, cls, "totalCores", "I"); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_TOTAL_SOCKETS] = \
            JENV->GetFieldID(env, cls, "totalSockets", "I"); \
        jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_CORES_PER_SOCKET] = \
            JENV->GetFieldID(env, cls, "coresPerSocket", "I"); \
    }

#define JAVA_SYSM_SET_FIELDS_CPUINFO(obj, s) \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_VENDOR], s.vendor); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_MODEL], s.model); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_MHZ], s.mhz); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_MHZ_MAX], s.mhz_max); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_MHZ_MIN], s.mhz_min); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_CACHE_SIZE], s.cache_size); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_TOTAL_CORES], s.total_cores); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_TOTAL_SOCKETS], s.total_sockets); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUINFO]->ids[JSYSM_FIELDS_CPUINFO_CORES_PER_SOCKET], s.cores_per_socket);

#define JAVA_SYSM_INIT_FIELDS_CPUPERCENTAGE(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]) { \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE])); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids =  \
            malloc(JSYSM_FIELDS_CPUPERCENTAGE_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids)); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_USER] =  \
            JENV->GetFieldID(env, cls, "user", "D"); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_SYS] =  \
            JENV->GetFieldID(env, cls, "sys", "D"); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_NICE] =  \
            JENV->GetFieldID(env, cls, "nice", "D"); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_IDLE] =  \
            JENV->GetFieldID(env, cls, "idle", "D"); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_WAIT] =  \
            JENV->GetFieldID(env, cls, "wait", "D"); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_HI] =  \
            JENV->GetFieldID(env, cls, "hi", "D"); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_SI] =  \
            JENV->GetFieldID(env, cls, "si", "D"); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_STEAL] =  \
            JENV->GetFieldID(env, cls, "steal", "D"); \
        jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_COMBINED] =  \
            JENV->GetFieldID(env, cls, "combined", "D"); \
    }

#define JAVA_SYSM_SET_FIELDS_CPUPERCENTAGE(obj, s) \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_USER], s.user); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_SYS], s.sys); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_NICE], s.nice); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_IDLE], s.idle); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_WAIT], s.wait); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_HI], s.hi); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_SI], s.si); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_STEAL], s.steal); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_CPUPERCENTAGE]->ids[JSYSM_FIELDS_CPUPERCENTAGE_COMBINED], s.combined);

#define JAVA_SYSM_INIT_FIELDS_UPTIME(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_UPTIME]) { \
        jsysm->fields[JSYSM_FIELDS_UPTIME] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_UPTIME])); \
        jsysm->fields[JSYSM_FIELDS_UPTIME]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_UPTIME]->ids =  \
            malloc(JSYSM_FIELDS_UPTIME_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_UPTIME]->ids)); \
        jsysm->fields[JSYSM_FIELDS_UPTIME]->ids[JSYSM_FIELDS_UPTIME_UPTIME] =  \
            JENV->GetFieldID(env, cls, "uptime", "D"); \
    }

#define JAVA_SYSM_SET_FIELDS_UPTIME(obj, s) \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_UPTIME]->ids[JSYSM_FIELDS_UPTIME_UPTIME], s.uptime);

#define JAVA_SYSM_INIT_FIELDS_PROCCPU(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_PROCCPU]) { \
        jsysm->fields[JSYSM_FIELDS_PROCCPU] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_PROCCPU])); \
        jsysm->fields[JSYSM_FIELDS_PROCCPU]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids =  \
            malloc(JSYSM_FIELDS_PROCCPU_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids)); \
        jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_PERCENT] =  \
            JENV->GetFieldID(env, cls, "percent", "D"); \
        jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_LAST_TIME] =  \
            JENV->GetFieldID(env, cls, "lastTime", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_START_TIME] =  \
            JENV->GetFieldID(env, cls, "startTime", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_USER] =  \
            JENV->GetFieldID(env, cls, "user", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_SYS] =  \
            JENV->GetFieldID(env, cls, "sys", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_TOTAL] =  \
            JENV->GetFieldID(env, cls, "total", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_PROCCPU(obj, s) \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_PERCENT], s.percent); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_LAST_TIME], s.last_time); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_START_TIME], s.start_time); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_USER], s.user); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_SYS], s.sys); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCCPU]->ids[JSYSM_FIELDS_PROCCPU_TOTAL], s.total);

#define JAVA_SYSM_INIT_FIELDS_PROCEXE(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_PROCEXE]) { \
        jsysm->fields[JSYSM_FIELDS_PROCEXE] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_PROCEXE])); \
        jsysm->fields[JSYSM_FIELDS_PROCEXE]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_PROCEXE]->ids =  \
            malloc(JSYSM_FIELDS_PROCEXE_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_PROCEXE]->ids)); \
        jsysm->fields[JSYSM_FIELDS_PROCEXE]->ids[JSYSM_FIELDS_PROCEXE_NAME] =  \
            JENV->GetFieldID(env, cls, "name", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_PROCEXE]->ids[JSYSM_FIELDS_PROCEXE_CWD] =  \
            JENV->GetFieldID(env, cls, "cwd", "Ljava/lang/String;"); \
    }

#define JAVA_SYSM_SET_FIELDS_PROCEXE(obj, s) \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCEXE]->ids[JSYSM_FIELDS_PROCEXE_NAME], s.name); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCEXE]->ids[JSYSM_FIELDS_PROCEXE_CWD], s.cwd);

#define JAVA_SYSM_INIT_FIELDS_PROCMEM(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_PROCMEM]) { \
        jsysm->fields[JSYSM_FIELDS_PROCMEM] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_PROCMEM])); \
        jsysm->fields[JSYSM_FIELDS_PROCMEM]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids =  \
            malloc(JSYSM_FIELDS_PROCMEM_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids)); \
        jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_SIZE] =  \
            JENV->GetFieldID(env, cls, "size", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_RESIDENT] =  \
            JENV->GetFieldID(env, cls, "resident", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_SHARE] =  \
            JENV->GetFieldID(env, cls, "share", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_MINOR_FAULTS] =  \
            JENV->GetFieldID(env, cls, "minorFaults", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_MAJOR_FAULTS] =  \
            JENV->GetFieldID(env, cls, "majorFaults", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_PAGE_FAULTS] =  \
            JENV->GetFieldID(env, cls, "pageFaults", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_PROCMEM(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_SIZE], s.size); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_RESIDENT], s.resident); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_SHARE], s.share); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_MINOR_FAULTS], s.minor_faults); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_MAJOR_FAULTS], s.major_faults); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCMEM]->ids[JSYSM_FIELDS_PROCMEM_PAGE_FAULTS], s.page_faults);

#define JAVA_SYSM_INIT_FIELDS_PROCOWN(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_PROCOWN]) { \
        jsysm->fields[JSYSM_FIELDS_PROCOWN] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_PROCOWN])); \
        jsysm->fields[JSYSM_FIELDS_PROCOWN]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids =  \
            malloc(JSYSM_FIELDS_PROCOWN_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids)); \
        jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids[JSYSM_FIELDS_PROCOWN_UID] =  \
            JENV->GetFieldID(env, cls, "uid", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids[JSYSM_FIELDS_PROCOWN_GID] =  \
            JENV->GetFieldID(env, cls, "gid", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids[JSYSM_FIELDS_PROCOWN_EUID] =  \
            JENV->GetFieldID(env, cls, "euid", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids[JSYSM_FIELDS_PROCOWN_EGID] =  \
            JENV->GetFieldID(env, cls, "egid", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_PROCOWN(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids[JSYSM_FIELDS_PROCOWN_UID], s.uid); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids[JSYSM_FIELDS_PROCOWN_GID], s.gid); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids[JSYSM_FIELDS_PROCOWN_EUID], s.euid); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCOWN]->ids[JSYSM_FIELDS_PROCOWN_EGID], s.egid);

#define JAVA_SYSM_INIT_FIELDS_PROCOWNER(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_PROCOWNER]) { \
        jsysm->fields[JSYSM_FIELDS_PROCOWNER] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_PROCOWNER])); \
        jsysm->fields[JSYSM_FIELDS_PROCOWNER]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_PROCOWNER]->ids =  \
            malloc(JSYSM_FIELDS_PROCOWNER_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_PROCOWNER]->ids)); \
        jsysm->fields[JSYSM_FIELDS_PROCOWNER]->ids[JSYSM_FIELDS_PROCOWNER_USER] =  \
            JENV->GetFieldID(env, cls, "user", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_PROCOWNER]->ids[JSYSM_FIELDS_PROCOWNER_GROUP] =  \
            JENV->GetFieldID(env, cls, "group", "Ljava/lang/String;"); \
    }

#define JAVA_SYSM_SET_FIELDS_PROCOWNER(obj, s) \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCOWNER]->ids[JSYSM_FIELDS_PROCOWNER_USER], s.user); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCOWNER]->ids[JSYSM_FIELDS_PROCOWNER_GROUP], s.group);

#define JAVA_SYSM_INIT_FIELDS_PROCSTAT(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_PROCSTAT]) { \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_PROCSTAT])); \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids =  \
            malloc(JSYSM_FIELDS_PROCSTAT_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids)); \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_TOTAL] =  \
            JENV->GetFieldID(env, cls, "total", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_IDLE] =  \
            JENV->GetFieldID(env, cls, "idle", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_RUNNING] =  \
            JENV->GetFieldID(env, cls, "running", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_SLEEPING] =  \
            JENV->GetFieldID(env, cls, "sleeping", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_STOPPED] =  \
            JENV->GetFieldID(env, cls, "stopped", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_ZOMBIE] =  \
            JENV->GetFieldID(env, cls, "zombie", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_THREADS] =  \
            JENV->GetFieldID(env, cls, "threads", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_PROCSTAT(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_TOTAL], s.total); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_IDLE], s.idle); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_RUNNING], s.running); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_SLEEPING], s.sleeping); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_STOPPED], s.stopped); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_ZOMBIE], s.zombie); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTAT]->ids[JSYSM_FIELDS_PROCSTAT_THREADS], s.threads);

#define JAVA_SYSM_INIT_FIELDS_PROCSTATE(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_PROCSTATE]) { \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_PROCSTATE])); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids =  \
            malloc(JSYSM_FIELDS_PROCSTATE_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids)); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_STATE] =  \
            JENV->GetFieldID(env, cls, "state", "C"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_NAME] =  \
            JENV->GetFieldID(env, cls, "name", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_PPID] =  \
            JENV->GetFieldID(env, cls, "ppid", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_TTY] =  \
            JENV->GetFieldID(env, cls, "tty", "I"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_NICE] =  \
            JENV->GetFieldID(env, cls, "nice", "I"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_PRIORITY] =  \
            JENV->GetFieldID(env, cls, "priority", "I"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_THREADS] =  \
            JENV->GetFieldID(env, cls, "threads", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_PROCESSOR] =  \
            JENV->GetFieldID(env, cls, "processor", "I"); \
    }

#define JAVA_SYSM_SET_FIELDS_PROCSTATE(obj, s) \
    JENV->SetCharField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_STATE], s.state); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_NAME], s.name); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_PPID], s.ppid); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_TTY], s.tty); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_NICE], s.nice); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_PRIORITY], s.priority); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_THREADS], s.threads); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCSTATE]->ids[JSYSM_FIELDS_PROCSTATE_PROCESSOR], s.processor);

#define JAVA_SYSM_INIT_FIELDS_PROCTIME(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_PROCTIME]) { \
        jsysm->fields[JSYSM_FIELDS_PROCTIME] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_PROCTIME])); \
        jsysm->fields[JSYSM_FIELDS_PROCTIME]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids =  \
            malloc(JSYSM_FIELDS_PROCTIME_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids)); \
        jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids[JSYSM_FIELDS_PROCTIME_START_TIME] =  \
            JENV->GetFieldID(env, cls, "startTime", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids[JSYSM_FIELDS_PROCTIME_USER] =  \
            JENV->GetFieldID(env, cls, "user", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids[JSYSM_FIELDS_PROCTIME_SYS] =  \
            JENV->GetFieldID(env, cls, "sys", "J"); \
        jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids[JSYSM_FIELDS_PROCTIME_TOTAL] =  \
            JENV->GetFieldID(env, cls, "total", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_PROCTIME(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids[JSYSM_FIELDS_PROCTIME_START_TIME], s.start_time); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids[JSYSM_FIELDS_PROCTIME_USER], s.user); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids[JSYSM_FIELDS_PROCTIME_SYS], s.sys); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_PROCTIME]->ids[JSYSM_FIELDS_PROCTIME_TOTAL], s.total);

#define JAVA_SYSM_INIT_FIELDS_RESOURCELIMIT(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]) { \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT])); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids =  \
            malloc(JSYSM_FIELDS_RESOURCELIMIT_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids)); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_CPU_CUR] =  \
            JENV->GetFieldID(env, cls, "cpuCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_CPU_MAX] =  \
            JENV->GetFieldID(env, cls, "cpuMax", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_FILE_SIZE_CUR] =  \
            JENV->GetFieldID(env, cls, "fileSizeCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_FILE_SIZE_MAX] =  \
            JENV->GetFieldID(env, cls, "fileSizeMax", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_PIPE_SIZE_MAX] =  \
            JENV->GetFieldID(env, cls, "pipeSizeMax", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_PIPE_SIZE_CUR] =  \
            JENV->GetFieldID(env, cls, "pipeSizeCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_DATA_CUR] =  \
            JENV->GetFieldID(env, cls, "dataCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_DATA_MAX] =  \
            JENV->GetFieldID(env, cls, "dataMax", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_STACK_CUR] =  \
            JENV->GetFieldID(env, cls, "stackCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_STACK_MAX] =  \
            JENV->GetFieldID(env, cls, "stackMax", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_CORE_CUR] =  \
            JENV->GetFieldID(env, cls, "coreCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_CORE_MAX] =  \
            JENV->GetFieldID(env, cls, "coreMax", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_MEMORY_CUR] =  \
            JENV->GetFieldID(env, cls, "memoryCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_MEMORY_MAX] =  \
            JENV->GetFieldID(env, cls, "memoryMax", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_PROCESSES_CUR] =  \
            JENV->GetFieldID(env, cls, "processesCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_PROCESSES_MAX] =  \
            JENV->GetFieldID(env, cls, "processesMax", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_OPEN_FILES_CUR] =  \
            JENV->GetFieldID(env, cls, "openFilesCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_OPEN_FILES_MAX] =  \
            JENV->GetFieldID(env, cls, "openFilesMax", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_VIRTUAL_MEMORY_CUR] =  \
            JENV->GetFieldID(env, cls, "virtualMemoryCur", "J"); \
        jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_VIRTUAL_MEMORY_MAX] =  \
            JENV->GetFieldID(env, cls, "virtualMemoryMax", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_RESOURCELIMIT(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_CPU_CUR], s.cpu_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_CPU_MAX], s.cpu_max); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_FILE_SIZE_CUR], s.file_size_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_FILE_SIZE_MAX], s.file_size_max); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_PIPE_SIZE_MAX], s.pipe_size_max); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_PIPE_SIZE_CUR], s.pipe_size_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_DATA_CUR], s.data_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_DATA_MAX], s.data_max); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_STACK_CUR], s.stack_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_STACK_MAX], s.stack_max); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_CORE_CUR], s.core_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_CORE_MAX], s.core_max); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_MEMORY_CUR], s.memory_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_MEMORY_MAX], s.memory_max); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_PROCESSES_CUR], s.processes_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_PROCESSES_MAX], s.processes_max); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_OPEN_FILES_CUR], s.open_files_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_OPEN_FILES_MAX], s.open_files_max); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_VIRTUAL_MEMORY_CUR], s.virtual_memory_cur); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_RESOURCELIMIT]->ids[JSYSM_FIELDS_RESOURCELIMIT_VIRTUAL_MEMORY_MAX], s.virtual_memory_max);

#define JAVA_SYSM_INIT_FIELDS_DISKUSAGE(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_DISKUSAGE]) { \
        jsysm->fields[JSYSM_FIELDS_DISKUSAGE] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_DISKUSAGE])); \
        jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids =  \
            malloc(JSYSM_FIELDS_DISKUSAGE_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids)); \
        jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_READS] =  \
            JENV->GetFieldID(env, cls, "reads", "J"); \
        jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_WRITES] =  \
            JENV->GetFieldID(env, cls, "writes", "J"); \
        jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_READ_BYTES] =  \
            JENV->GetFieldID(env, cls, "readBytes", "J"); \
        jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_WRITE_BYTES] =  \
            JENV->GetFieldID(env, cls, "writeBytes", "J"); \
        jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_UTIL] =  \
            JENV->GetFieldID(env, cls, "util", "D"); \
        jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_SERVICE_TIME] =  \
            JENV->GetFieldID(env, cls, "serviceTime", "D"); \
    }

#define JAVA_SYSM_SET_FIELDS_DISKUSAGE(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_READS], s.reads); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_WRITES], s.writes); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_READ_BYTES], s.read_bytes); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_WRITE_BYTES], s.write_bytes); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_UTIL], s.util); \
    JENV->SetDoubleField(env, obj, jsysm->fields[JSYSM_FIELDS_DISKUSAGE]->ids[JSYSM_FIELDS_DISKUSAGE_SERVICE_TIME], s.service_time);

#define JAVA_SYSM_INIT_FIELDS_NETSTAT(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_NETSTAT]) { \
        jsysm->fields[JSYSM_FIELDS_NETSTAT] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_NETSTAT])); \
        jsysm->fields[JSYSM_FIELDS_NETSTAT]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids =  \
            malloc(JSYSM_FIELDS_NETSTAT_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids)); \
        jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids[JSYSM_FIELDS_NETSTAT_TCP_INBOUND_TOTAL] =  \
            JENV->GetFieldID(env, cls, "tcpInboundTotal", "I"); \
        jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids[JSYSM_FIELDS_NETSTAT_TCP_OUTBOUND_TOTAL] =  \
            JENV->GetFieldID(env, cls, "tcpOutboundTotal", "I"); \
        jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids[JSYSM_FIELDS_NETSTAT_ALL_INBOUND_TOTAL] =  \
            JENV->GetFieldID(env, cls, "allInboundTotal", "I"); \
        jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids[JSYSM_FIELDS_NETSTAT_ALL_OUTBOUND_TOTAL] =  \
            JENV->GetFieldID(env, cls, "allOutboundTotal", "I"); \
    }

#define JAVA_SYSM_SET_FIELDS_NETSTAT(obj, s) \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids[JSYSM_FIELDS_NETSTAT_TCP_INBOUND_TOTAL], s.tcp_inbound_total); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids[JSYSM_FIELDS_NETSTAT_TCP_OUTBOUND_TOTAL], s.tcp_outbound_total); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids[JSYSM_FIELDS_NETSTAT_ALL_INBOUND_TOTAL], s.all_inbound_total); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_NETSTAT]->ids[JSYSM_FIELDS_NETSTAT_ALL_OUTBOUND_TOTAL], s.all_outbound_total);

#define JAVA_SYSM_INIT_FIELDS_NETINFO(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_NETINFO]) { \
        jsysm->fields[JSYSM_FIELDS_NETINFO] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_NETINFO])); \
        jsysm->fields[JSYSM_FIELDS_NETINFO]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_NETINFO]->ids =  \
            malloc(JSYSM_FIELDS_NETINFO_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_NETINFO]->ids)); \
        jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_DEFAULT_GATEWAY] =  \
            JENV->GetFieldID(env, cls, "defaultGateway", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_DEFAULT_GATEWAY_INTERFACE] =  \
            JENV->GetFieldID(env, cls, "defaultGatewayInterface", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_HOST_NAME] =  \
            JENV->GetFieldID(env, cls, "hostName", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_DOMAIN_NAME] =  \
            JENV->GetFieldID(env, cls, "domainName", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_PRIMARY_DNS] =  \
            JENV->GetFieldID(env, cls, "primaryDns", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_SECONDARY_DNS] =  \
            JENV->GetFieldID(env, cls, "secondaryDns", "Ljava/lang/String;"); \
    }

#define JAVA_SYSM_SET_FIELDS_NETINFO(obj, s) \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_DEFAULT_GATEWAY], s.default_gateway); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_DEFAULT_GATEWAY_INTERFACE], s.default_gateway_interface); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_HOST_NAME], s.host_name); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_DOMAIN_NAME], s.domain_name); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_PRIMARY_DNS], s.primary_dns); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINFO]->ids[JSYSM_FIELDS_NETINFO_SECONDARY_DNS], s.secondary_dns);

#define JAVA_SYSM_INIT_FIELDS_NETINTERFACECONFIG(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]) { \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG])); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids =  \
            malloc(JSYSM_FIELDS_NETINTERFACECONFIG_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids)); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_NAME] =  \
            JENV->GetFieldID(env, cls, "name", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_HWADDR] =  \
            JENV->GetFieldID(env, cls, "hwaddr", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_TYPE] =  \
            JENV->GetFieldID(env, cls, "type", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_DESCRIPTION] =  \
            JENV->GetFieldID(env, cls, "description", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_ADDRESS] =  \
            JENV->GetFieldID(env, cls, "address", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_ADDRESS6] =  \
            JENV->GetFieldID(env, cls, "address6", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_PREFIX6_LENGTH] =  \
            JENV->GetFieldID(env, cls, "prefix6Length", "I"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_SCOPE6] =  \
            JENV->GetFieldID(env, cls, "scope6", "I"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_DESTINATION] =  \
            JENV->GetFieldID(env, cls, "destination", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_BROADCAST] =  \
            JENV->GetFieldID(env, cls, "broadcast", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_NETMASK] =  \
            JENV->GetFieldID(env, cls, "netmask", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_FLAGS] =  \
            JENV->GetFieldID(env, cls, "flags", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_MTU] =  \
            JENV->GetFieldID(env, cls, "mtu", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_METRIC] =  \
            JENV->GetFieldID(env, cls, "metric", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_TX_QUEUE_LEN] =  \
            JENV->GetFieldID(env, cls, "txQueueLen", "I"); \
    }

#define JAVA_SYSM_SET_FIELDS_NETINTERFACECONFIG(obj, s) \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_NAME], s.name); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_HWADDR], s.hwaddr); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_TYPE], s.type); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_DESCRIPTION], s.description); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_ADDRESS], s.address); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_ADDRESS6], s.address6); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_PREFIX6_LENGTH], s.prefix6_length); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_SCOPE6], s.scope6); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_DESTINATION], s.destination); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_BROADCAST], s.broadcast); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_NETMASK], s.netmask); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_FLAGS], s.flags); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_MTU], s.mtu); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_METRIC], s.metric); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACECONFIG]->ids[JSYSM_FIELDS_NETINTERFACECONFIG_TX_QUEUE_LEN], s.tx_queue_len);

#define JAVA_SYSM_INIT_FIELDS_NETINTERFACESTAT(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]) { \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT])); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids =  \
            malloc(JSYSM_FIELDS_NETINTERFACESTAT_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids)); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_BYTES] =  \
            JENV->GetFieldID(env, cls, "rxBytes", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_PACKETS] =  \
            JENV->GetFieldID(env, cls, "rxPackets", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_ERRORS] =  \
            JENV->GetFieldID(env, cls, "rxErrors", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_DROPPED] =  \
            JENV->GetFieldID(env, cls, "rxDropped", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_OVERRUNS] =  \
            JENV->GetFieldID(env, cls, "rxOverruns", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_FRAME] =  \
            JENV->GetFieldID(env, cls, "rxFrame", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_BYTES] =  \
            JENV->GetFieldID(env, cls, "txBytes", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_PACKETS] =  \
            JENV->GetFieldID(env, cls, "txPackets", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_ERRORS] =  \
            JENV->GetFieldID(env, cls, "txErrors", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_DROPPED] =  \
            JENV->GetFieldID(env, cls, "txDropped", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_OVERRUNS] =  \
            JENV->GetFieldID(env, cls, "txOverruns", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_COLLISIONS] =  \
            JENV->GetFieldID(env, cls, "txCollisions", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_CARRIER] =  \
            JENV->GetFieldID(env, cls, "txCarrier", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_SPEED] =  \
            JENV->GetFieldID(env, cls, "speed", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_NETINTERFACESTAT(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_BYTES], s.rx_bytes); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_PACKETS], s.rx_packets); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_ERRORS], s.rx_errors); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_DROPPED], s.rx_dropped); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_OVERRUNS], s.rx_overruns); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_RX_FRAME], s.rx_frame); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_BYTES], s.tx_bytes); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_PACKETS], s.tx_packets); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_ERRORS], s.tx_errors); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_DROPPED], s.tx_dropped); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_OVERRUNS], s.tx_overruns); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_COLLISIONS], s.tx_collisions); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_TX_CARRIER], s.tx_carrier); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETINTERFACESTAT]->ids[JSYSM_FIELDS_NETINTERFACESTAT_SPEED], s.speed);

#define JAVA_SYSM_INIT_FIELDS_NETROUTE(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_NETROUTE]) { \
        jsysm->fields[JSYSM_FIELDS_NETROUTE] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_NETROUTE])); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids =  \
            malloc(JSYSM_FIELDS_NETROUTE_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids)); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_DESTINATION] =  \
            JENV->GetFieldID(env, cls, "destination", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_GATEWAY] =  \
            JENV->GetFieldID(env, cls, "gateway", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_FLAGS] =  \
            JENV->GetFieldID(env, cls, "flags", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_REFCNT] =  \
            JENV->GetFieldID(env, cls, "refcnt", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_USE] =  \
            JENV->GetFieldID(env, cls, "use", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_METRIC] =  \
            JENV->GetFieldID(env, cls, "metric", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_MASK] =  \
            JENV->GetFieldID(env, cls, "mask", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_MTU] =  \
            JENV->GetFieldID(env, cls, "mtu", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_WINDOW] =  \
            JENV->GetFieldID(env, cls, "window", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_IRTT] =  \
            JENV->GetFieldID(env, cls, "irtt", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_IFNAME] =  \
            JENV->GetFieldID(env, cls, "ifname", "Ljava/lang/String;"); \
    }

#define JAVA_SYSM_SET_FIELDS_NETROUTE(cls, obj, s) \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_DESTINATION], s.destination); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_GATEWAY], s.gateway); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_FLAGS], s.flags); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_REFCNT], s.refcnt); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_USE], s.use); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_METRIC], s.metric); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_MASK], s.mask); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_MTU], s.mtu); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_WINDOW], s.window); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_IRTT], s.irtt); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_NETROUTE]->ids[JSYSM_FIELDS_NETROUTE_IFNAME], s.ifname);

#define JAVA_SYSM_INIT_FIELDS_NETCONNECTION(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_NETCONNECTION]) { \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_NETCONNECTION])); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids =  \
            malloc(JSYSM_FIELDS_NETCONNECTION_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids)); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_LOCAL_PORT] =  \
            JENV->GetFieldID(env, cls, "localPort", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_LOCAL_ADDRESS] =  \
            JENV->GetFieldID(env, cls, "localAddress", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_REMOTE_PORT] =  \
            JENV->GetFieldID(env, cls, "remotePort", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_REMOTE_ADDRESS] =  \
            JENV->GetFieldID(env, cls, "remoteAddress", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_TYPE] =  \
            JENV->GetFieldID(env, cls, "type", "I"); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_STATE] =  \
            JENV->GetFieldID(env, cls, "state", "I"); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_SEND_QUEUE] =  \
            JENV->GetFieldID(env, cls, "sendQueue", "J"); \
        jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_RECEIVE_QUEUE] =  \
            JENV->GetFieldID(env, cls, "receiveQueue", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_NETCONNECTION(obj, s) \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_LOCAL_PORT], s.lo_port); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_LOCAL_ADDRESS], s.lo_address); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_REMOTE_PORT], s.re_port); \
    JENV->SetNetAddressField(env, obj, jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_REMOTE_ADDRESS], s.re_address); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_TYPE], s.type); \
    JENV->SetIntField(env, obj, jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_STATE], s.state); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_SEND_QUEUE], s.se_q); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_NETCONNECTION]->ids[JSYSM_FIELDS_NETCONNECTION_RECEIVE_QUEUE], s.re_q);

#define JAVA_SYSM_INIT_FIELDS_WHO(cls) \
    if (!jsysm->fields[JSYSM_FIELDS_WHO]) { \
        jsysm->fields[JSYSM_FIELDS_WHO] =  \
            malloc(sizeof(*jsysm->fields[JSYSM_FIELDS_WHO])); \
        jsysm->fields[JSYSM_FIELDS_WHO]->classref =  \
            (jclass)JENV->NewGlobalRef(env, cls); \
        jsysm->fields[JSYSM_FIELDS_WHO]->ids =  \
            malloc(JSYSM_FIELDS_WHO_MAX * \
                   sizeof(*jsysm->fields[JSYSM_FIELDS_WHO]->ids)); \
        jsysm->fields[JSYSM_FIELDS_WHO]->ids[JSYSM_FIELDS_WHO_USER] =  \
            JENV->GetFieldID(env, cls, "user", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_WHO]->ids[JSYSM_FIELDS_WHO_DEVICE] =  \
            JENV->GetFieldID(env, cls, "device", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_WHO]->ids[JSYSM_FIELDS_WHO_HOST] =  \
            JENV->GetFieldID(env, cls, "host", "Ljava/lang/String;"); \
        jsysm->fields[JSYSM_FIELDS_WHO]->ids[JSYSM_FIELDS_WHO_TIME] =  \
            JENV->GetFieldID(env, cls, "time", "J"); \
    }

#define JAVA_SYSM_SET_FIELDS_WHO(cls, obj, s) \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_WHO]->ids[JSYSM_FIELDS_WHO_USER], s.user); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_WHO]->ids[JSYSM_FIELDS_WHO_DEVICE], s.device); \
    JENV->SetStringField(env, obj, jsysm->fields[JSYSM_FIELDS_WHO]->ids[JSYSM_FIELDS_WHO_HOST], s.host); \
    JENV->SetLongField(env, obj, jsysm->fields[JSYSM_FIELDS_WHO]->ids[JSYSM_FIELDS_WHO_TIME], s.time);



#endif  //HBDC_SYSM_NATIVE_JAVASYSM_H