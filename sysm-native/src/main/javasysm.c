//
// Created by wzt on 11/6/17.
//

#include <jni.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sysm.h>

#include "sysm.h"
#include "sysm_util.h"
#include "javasysm.h"
#include "javasysm_generated.h"

#ifdef SYSM_64BIT
#define SYSM_POINTER_LONG
#endif

/*Just for fast code*/
#define dSYSM_GET \
    jni_sysm_t *jsysm = sysm_get_jpointer(env, sysm_obj); \
    sysm_t *sysm

/*Just for fast code*/
#define dSYSM_VOID \
    dSYSM_GET;  \
    if (!jsysm) return; \
    sysm = jsysm->sysm; \
    jsysm->env = env

/*Just for fast code*/
#define dSYSM(val) \
    dSYSM_GET; \
    if (!jsysm) return val; \
    sysm = jsysm->sysm; \
    jsysm->env = env


typedef struct {
    jclass classref;
    jfieldID *ids;
}jsysm_field_cache_t;   //store evey bean's(du/scut/cs/sysm/bean) class and fieldID

typedef struct {
    JNIEnv *env;
    sysm_t *sysm;
    jsysm_field_cache_t *fields[JSYSM_FIELDS_MAX];  //to search bean's class and all fieldIDs in bean
    int open_status;
    jthrowable not_impl;
}jni_sysm_t;    //store sysm and cache for jni

#define SYSM_EX "SysmException"
static void sysm_throw_exception(JNIEnv *env, char *msg)
{
    jclass errorCls = SYSM_FIND_CLASS_EX(SYSM_EX);
    JENV->ThrowNew(env, errorCls, msg);
}

#define SYSM_NOTIMPLE_EX "SysmNotImplException"
static void sysm_throw_notimpl(JNIEnv *env, char *msg)
{
    jclass errorCls = SYSM_FIND_CLASS_EX(SYSM_NOTIMPLE_EX);
    JENV->ThrowNew(env, errorCls, msg);
}

static void sysm_throw_error(JNIEnv *env, jni_sysm_t *jsysm, int err)
{
    jclass errorCls;
    int err_type = err;

    switch (err_type)
    {
        case ENOENT:
            errorCls = SYSM_FIND_CLASS("SysmFileNotFoundException");
            break;

        case EACCES:
            errorCls = SYSM_FIND_CLASS("SysmPermissionDeniedException");
            break;

        case SYSM_ENOTIMPL:
            if (jsysm->not_impl == NULL)
            {
                jfieldID id;
                jthrowable not_impl;

                errorCls = SYSM_FIND_CLASS_EX(SYSM_NOTIMPLE_EX);
                //get singleton in Class SysmNotImplException
                id = JENV->GetStaticFieldID(env, errorCls,
                                            "INSTANCE",
                                            SYSM_CLASS_SIG_EX(SYSM_NOTIMPLE_EX));

                not_impl = JENV->GetStaticObjectField(env, errorCls, id);

                jsysm->not_impl = JENV->NewGlobalRef(env, not_impl);
            }

            JENV->Throw(env, jsysm->not_impl);
            return;

        default:
            errorCls = SYSM_FIND_CLASS_EX(SYSM_EX);
            break;
    }

    JENV->ThrowNew(env, errorCls, sysm_strerror(jsysm->sysm, err));
}

static void sysm_set_pointer(JNIEnv *env, jobject obj, const void *ptr)
{
    jfieldID pointer_field;
    jclass cls = JENV->GetObjectClass(env, obj);

#ifdef SYSM_POINTER_LONG
    pointer_field = JENV->GetFieldID(env, cls, "longSysmWrapper", "J");
    JENV->SetLongField(env, obj, pointer_field, (jlong)ptr);
#else
    pointer_field = JENV->GetFieldID(env, cls, "sysmWrapper", "I");
    JENV->SetIntField(env, obj, pointer_field, (int)ptr);
#endif
}

static void *sysm_get_pointer(JNIEnv *env, jobject obj)
{
    jfieldID pointer_field;
    jclass cls = JENV->GetObjectClass(env, obj);
#ifdef SYSM_POINTER_LONG
    pointer_field = JENV->GetFieldID(env, cls, "longSysmWrapper", "J");
    return (void *)JENV->GetLongField(env, obj, pointer_field);
#else
    pointer_field = JENV->GetFieldID(env, cls, "sysmWrapper", "I");
    return (void *)JENV->GetIntField(env, obj, pointer_field);
#endif
}

static jni_sysm_t *sysm_get_jpointer(JNIEnv *env, jobject obj)
{
    jni_sysm_t *jsysm =
            (jni_sysm_t *)sysm_get_pointer(env, obj);
    if (!jsysm) {
        sysm_throw_exception(env, "sysm has been closed");
        return NULL;
    }

    if (jsysm->open_status != SYSM_OK) {
        sysm_throw_error(env, jsysm, jsysm->open_status);
        return NULL;
    }

    return jsysm;
}

static jstring jnet_address_to_string(JNIEnv *env, sysm_t *sysm, sysm_net_address_t *val) {
    char addr_str[SYSM_INET6_ADDRSTRLEN];
    sysm_net_address_to_string(sysm, val, addr_str);
    return JENV->NewStringUTF(env, addr_str);
}

/**
 * ===============================================================================================
 * |                                                                                             |
 * |                          Gather method in edu.scut.cs.sysm.Sysm                             |
 * |                                                                                             |
 * ===============================================================================================
 */

JNIEXPORT jstring SYSM_JNI(getNativeLibVersion)
  (JNIEnv *env, jclass cls)
{
    sysm_version_t *version = sysm_version_get();
    return JENV->NewStringUTF(env, version->version);
}

JNIEXPORT void SYSM_JNI(nativeOpen)
  (JNIEnv *env, jobject sysm_obj)
{
    jni_sysm_t *jsysm = malloc(sizeof(*jsysm));

    memset(jsysm, '\0', sizeof(*jsysm));

    sysm_set_pointer(env, sysm_obj, jsysm);

    if ((jsysm->open_status = sysm_open(&jsysm->sysm)) != SYSM_OK)
        sysm_throw_error(env, jsysm, jsysm->open_status);
}

JNIEXPORT jint SYSM_JNI(nativeClose)
  (JNIEnv *env, jobject sysm_obj)
{
    jint status;

    dSYSM(0);

    //free space of sysm
    status = sysm_close(sysm);

    if (jsysm->not_impl != NULL)
        JENV->DeleteGlobalRef(env, jsysm->not_impl);

    for (int i = 0;i < JSYSM_FIELDS_MAX; i++)
    {
        if (jsysm->fields[i])
        {
            JENV->DeleteGlobalRef(env, jsysm->fields[i]->classref);
            free(jsysm->fields[i]->ids);
            free(jsysm->fields[i]);
        }
    }

    free(jsysm);
    sysm_set_pointer(env, sysm_obj, NULL);

    return status;
}

JNIEXPORT void SYSM_JNI(kill)
  (JNIEnv *env, jobject sysm_obj, jlong pid, jint signum)
{
    int status;

    dSYSM_VOID;

    if ((status = sysm_proc_signal((pid_t)pid, signum)) != SYSM_OK)
        sysm_throw_error(env, jsysm, status);
}

JNIEXPORT jlong SYSM_JNI(getPid)
  (JNIEnv *env, jobject sysm_obj)
{
    dSYSM(0);

    return sysm_pid_get(sysm);
}

JNIEXPORT jobjectArray SYSM_JNI(getCpuInfos)
  (JNIEnv *env, jobject sysm_obj)
{
    int status;
    sysm_cpu_info_list_t cpu_infos;
    jobjectArray cpu_info_array;
    jclass cls = SYSM_FIND_CLASS_BEAN("CpuInfo");

    dSYSM(NULL);

    if ((status = sysm_cpu_info_list_get(sysm, &cpu_infos)) != SYSM_OK) {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    JAVA_SYSM_INIT_FIELDS_CPUINFO(cls);

    cpu_info_array = JENV->NewObjectArray(env, (jsize)cpu_infos.number, cls, 0);
    SYSM_CHEX;

    for (int i = 0;i < cpu_infos.number;i++)
    {
        jobject info_obj = JENV->AllocObject(env, cls);
        SYSM_CHEX;

        JAVA_SYSM_SET_FIELDS_CPUINFO(info_obj, cpu_infos.data[i]);
        JENV->SetObjectArrayElement(env, cpu_info_array, i, info_obj);
        SYSM_CHEX;
    }

    sysm_cpu_info_list_destroy(&cpu_infos);
    return cpu_info_array;
}

JNIEXPORT jobjectArray SYSM_JNI(getCpusNative)
  (JNIEnv *env, jobject sysm_obj)
{
    int status;
    sysm_cpu_list_t cpu_list;
    jobjectArray cpu_array;
    jclass cls = SYSM_FIND_CLASS_BEAN("Cpu");
    dSYSM(NULL);

    if ((status = sysm_cpu_list_get(sysm, &cpu_list)) != SYSM_OK) {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    JAVA_SYSM_INIT_FIELDS_CPU(cls);

    cpu_array = JENV->NewObjectArray(env, (jsize) cpu_list.number, cls, 0);
    SYSM_CHEX;

    for (int i = 0; i < cpu_list.number; i++) {
        jobject info_obj = JENV->AllocObject(env, cls);
        SYSM_CHEX;
        JAVA_SYSM_SET_FIELDS_CPU(info_obj, cpu_list.data[i]);
        JENV->SetObjectArrayElement(env, cpu_array, i, info_obj);
        SYSM_CHEX;
    }

    sysm_cpu_list_destroy(&cpu_list);

    return cpu_array;
}

JNIEXPORT jdoubleArray SYSM_JNI(getLoadAverage)
  (JNIEnv *env, jobject sysm_obj)
{
    int status;
    jlongArray avgarray;
    sysm_loadavg_t loadavg;

    dSYSM(NULL);

    if ((status = sysm_loadavg_get(sysm, &loadavg)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    avgarray = JENV->NewDoubleArray(env, 3);
    SYSM_CHEX;

    JENV->SetDoubleArrayRegion(env, avgarray, 0, 3, loadavg.loadavg);

    return avgarray;
}

JNIEXPORT jlongArray SYSM_JNI(getProcesses)
  (JNIEnv *env, jobject sysm_obj)
{
    int status;
    jlongArray processes;
    sysm_proc_list_t proc_list;
    jlong *pids = NULL;

    dSYSM(NULL);

    if ((status = sysm_proc_list_get(sysm, &proc_list)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    processes = JENV->NewLongArray(env, (jsize) proc_list.number);
    SYSM_CHEX;

    pids = (jlong *)malloc(sizeof(jlong) * proc_list.number);
    for (int i = 0;i < proc_list.number;i++)
        pids[i] = proc_list.data[i];

    JENV->SetLongArrayRegion(env, processes, 0, (jsize) proc_list.number, pids);

    free(pids);
    sysm_proc_list_destroy(&proc_list);

    return processes;
}

JNIEXPORT jobjectArray SYSM_JNI(getProcessArgs)
  (JNIEnv *env, jobject sysm_obj, jlong pid)
{
    int status;
    sysm_proc_args_t proc_args;
    jobjectArray processArgs;
    jclass stringclass = JENV->FindClass(env, "java/lang/String");

    dSYSM(NULL);

    if ((status = sysm_proc_args_get(sysm, (pid_t) pid, &proc_args)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    processArgs = JENV->NewObjectArray(env, (jsize) proc_args.number, stringclass, 0);
    SYSM_CHEX;

    for (int i = 0;i < proc_args.number;i++)
    {
        jstring s = JENV->NewStringUTF(env, proc_args.data[i]);
        JENV->SetObjectArrayElement(env, processArgs, i, s);
        SYSM_CHEX;
    }

    sysm_proc_args_destroy(&proc_args);

    return processArgs;
}

JNIEXPORT jlong SYSM_JNI(getProcessPort)
  (JNIEnv *env, jobject sysm_obj, jint protocol, jlong port)
{
    int status;
    pid_t pid;

    dSYSM(0);

    status = sysm_proc_port_get(sysm, protocol, (unsigned long)port, &pid);

    if (status != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return -1;
    }

    return pid;
}

JNIEXPORT jobjectArray SYSM_JNI(getNetRoutes)
  (JNIEnv *env, jobject sysm_obj)
{
    int status;
    jarray routes;
    jclass cls = SYSM_FIND_CLASS("NetRoute");
    sysm_net_route_list_t route_list;
    dSYSM(NULL);

    if ((status = sysm_net_route_list_get(sysm, &route_list)) != SYSM_OK) {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    JAVA_SYSM_INIT_FIELDS_NETROUTE(cls);

    routes = JENV->NewObjectArray(env, route_list.number, cls, 0);
    SYSM_CHEX;

    for (int i=0; i<route_list.number; i++) {
        jobject obj = JENV->AllocObject(env, cls);
        SYSM_CHEX;
        JAVA_SYSM_SET_FIELDS_NETROUTE(cls, obj, route_list.data[i]);
        JENV->SetObjectArrayElement(env, routes, i, obj);
        SYSM_CHEX;
    }

    sysm_net_route_list_destroy(&route_list);

    return routes;
}

JNIEXPORT jobjectArray SYSM_JNI(getNetInterfaces)
  (JNIEnv *env, jobject sysm_obj)
{
    int status;
    sysm_net_interface_list_t if_list;
    jobjectArray ifs;
    jclass stringclass = JENV->FindClass(env, "java/lang/String");
    dSYSM(NULL);

    if ((status = sysm_net_interface_list_get(sysm, &if_list)) != SYSM_OK) {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    ifs = JENV->NewObjectArray(env, if_list.number, stringclass, 0);
    SYSM_CHEX;

    for (int i=0; i<if_list.number; i++) {
        jstring s = JENV->NewStringUTF(env, if_list.data[i]);
        JENV->SetObjectArrayElement(env, ifs, i, s);
        SYSM_CHEX;
    }

    sysm_net_interface_list_destroy(&if_list);

    return ifs;
}

JNIEXPORT jobjectArray SYSM_JNI(getNetConnections)
  (JNIEnv *env, jobject sysm_obj, jint flags)
{
    int status;
    unsigned int i;
    jarray connarray;
    jclass cls = SYSM_FIND_CLASS_BEAN("NetConnection");
    sysm_net_connection_list_t connlist;
    dSYSM(NULL);

    status = sysm_net_connection_list_get(sysm, &connlist, flags);

    if (status != SYSM_OK) {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    JAVA_SYSM_INIT_FIELDS_NETCONNECTION(cls);

    connarray = JENV->NewObjectArray(env, connlist.number, cls, 0);
    SYSM_CHEX;

    for (i=0; i<connlist.number; i++) {
        jobject obj = JENV->AllocObject(env, cls);
        SYSM_CHEX;
        JAVA_SYSM_SET_FIELDS_NETCONNECTION(obj, connlist.data[i]);
        JENV->SetObjectArrayElement(env, connarray, i, obj);
        SYSM_CHEX;
    }

    sysm_net_connection_list_destroy(&connlist);

    return connarray;
}

JNIEXPORT jstring SYSM_JNI(getNetListenAddress)
  (JNIEnv *env, jobject sysm_obj, jlong port)
{
    int status;
    sysm_net_address_t address;
    dSYSM(NULL);

    status = sysm_net_listen_address_get(sysm, (unsigned long) port, &address);
    if (status != SYSM_OK) {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    return jnet_address_to_string(env, sysm, &address);
}

JNIEXPORT jobjectArray SYSM_JNI(getWhos)
  (JNIEnv *env, jobject sysm_obj)
{
    int status;
    unsigned int i;
    sysm_who_list_t who_list;
    jobjectArray whos;
    jclass cls = SYSM_FIND_CLASS_BEAN("Who");
    dSYSM(NULL);

    if ((status = sysm_who_list_get(sysm, &who_list)) != SYSM_OK) {
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    JAVA_SYSM_INIT_FIELDS_WHO(cls);

    whos = JENV->NewObjectArray(env, who_list.number, cls, 0);
    SYSM_CHEX;

    for (i=0; i<who_list.number; i++) {
        jobject info_obj = JENV->AllocObject(env, cls);
        SYSM_CHEX;
        JAVA_SYSM_SET_FIELDS_WHO(cls, info_obj,
                                  who_list.data[i]);
        JENV->SetObjectArrayElement(env, whos, i, info_obj);
        SYSM_CHEX;
    }

    sysm_who_list_destroy(&who_list);

    return whos;
}

/**
 * ===============================================================================================
 * |                                                                                             |
 * |                          Gather method in edu.scut.cs.sysm.bean                             |
 * |                                                                                             |
 * ===============================================================================================
 */

JNIEXPORT void SYSM_JNIx(Memory_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj)
{
    int status;
    sysm_memory_t memory;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;


    if ((status = sysm_memory_get(sysm, &memory)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_MEMORY(cls);
    JAVA_SYSM_SET_FIELDS_MEMORY(obj, memory);
}

JNIEXPORT void SYSM_JNIx(Swap_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj)
{
    int status;
    sysm_swap_t swap;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    status = sysm_swap_get(&swap);

    if (status != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_SWAP(cls);
    JAVA_SYSM_SET_FIELDS_SWAP(obj, swap);
}

JNIEXPORT void SYSM_JNIx(Cpu_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj)
{
    int status;
    sysm_cpu_t cpu;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    status = sysm_cpu_get(sysm, &cpu);

    if (status !=SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_CPU(cls);
    JAVA_SYSM_SET_FIELDS_CPU(obj, cpu);
}

JNIEXPORT void SYSM_JNIx(CpuPercentage_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jobject jprev, jobject jcurr)
{
    sysm_cpu_t prev, curr;
    sysm_cpu_prec_t perc;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    JAVA_SYSM_GET_FIELDS_CPU(jprev, prev);
    JAVA_SYSM_GET_FIELDS_CPU(jcurr, curr);
    sysm_cpu_perc_cal(&prev, &curr, &perc);
    JAVA_SYSM_INIT_FIELDS_CPUPERCENTAGE(cls);
    JAVA_SYSM_SET_FIELDS_CPUPERCENTAGE(obj, perc);
}

JNIEXPORT void SYSM_JNIx(Uptime_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj)
{
    int status;
    sysm_uptime_t uptime;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    if ((status = sysm_uptime_get(&uptime)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_UPTIME(cls);
    JAVA_SYSM_SET_FIELDS_UPTIME(obj, uptime);
}

JNIEXPORT void SYSM_JNIx(ProcessCpu_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jlong pid)
{
    int status;
    sysm_proc_cpu_t proc_cpu;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    if ((status = sysm_proc_cpu_get(sysm, (pid_t) pid, &proc_cpu)))
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_PROCCPU(cls);
    JAVA_SYSM_SET_FIELDS_PROCCPU(obj, proc_cpu);
}

typedef struct {
    JNIEnv *env;
    jobject map;
    jmethodID id;
} jni_env_put_t;

static int jni_env_getall(void *data,
                          const char *key, int klen,
                          char *val, int vlen)
{
    jni_env_put_t *put = (jni_env_put_t *)data;
    JNIEnv *env = put->env;

    JENV->CallObjectMethod(env, put->map, put->id,
                           JENV->NewStringUTF(env, key),
                           JENV->NewStringUTF(env, val));

    return JENV->ExceptionCheck(env) ? !SYSM_OK : SYSM_OK;
}

#define MAP_PUT_SIG \
    "(Ljava/lang/Object;Ljava/lang/Object;)" \
    "Ljava/lang/Object;"

JNIEXPORT jobject SYSM_JNIx(ProcessEnv_getAll)
  (JNIEnv *env, jclass cls, jobject sysm_obj, jlong pid)
{
    int status;
    sysm_proc_env_t proc_env;
    jobject hashmap;
    jni_env_put_t put;
    jclass mapclass = JENV->FindClass(env, "java/util/HashMap");
    jmethodID  mapid = JENV->GetMethodID(env, mapclass, "<init>", "()V");
    jmethodID putid = JENV->GetMethodID(env, mapclass, "put", MAP_PUT_SIG);

    dSYSM(NULL);

    hashmap = JENV->NewObject(env, mapclass, mapid);
    SYSM_CHEX;

    put.env = env;
    put.id = putid;
    put.map = hashmap;

    proc_env.type = SYSM_PROC_ENV_ALL;
    proc_env.env_getter = jni_env_getall;
    proc_env.data = &put;

    if ((status = sysm_proc_env_get(sysm, (pid_t) pid, &proc_env)) != SYSM_OK)
    {
        JENV->DeleteLocalRef(env, hashmap);
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }


    return hashmap;
}

typedef struct {
    JNIEnv *env;
    const char *key;
    int klen;
    jstring val;
} jni_env_get_t;

static int jni_env_getvalue(void *data,
                            const char *key, int klen,
                            char *val, int vlen)
{
    jni_env_get_t *get = (jni_env_get_t *)data;
    JNIEnv *env = get->env;

    if ((get->klen == klen) &&
        (strcmp(get->key, key) == 0))
    {
        get->val = JENV->NewStringUTF(env, val);
        return !SYSM_OK;
    }

    return SYSM_OK;
}

JNIEXPORT jstring SYSM_JNIx(ProcessEnv_getValue)
  (JNIEnv *env, jclass cls, jobject sysm_obj, jlong pid, jstring key)
{
    int status;
    sysm_proc_env_t proc_env;
    jni_env_get_t get;

    dSYSM(NULL);

    get.env = env;
    get.key = JENV->GetStringUTFChars(env, key, 0);
    get.klen = JENV->GetStringUTFLength(env, key);
    get.val = NULL;

    proc_env.type = SYSM_PROC_ENV_KEY;
    proc_env.key = get.key;
    proc_env.klen = get.klen;
    proc_env.env_getter = jni_env_getvalue;
    proc_env.data = &get;

    if ((status = sysm_proc_env_get(sysm, pid, &proc_env)) != SYSM_OK)
    {
        JENV->ReleaseStringUTFChars(env, key, get.key);
        sysm_throw_error(env, jsysm, status);
        return NULL;
    }

    JENV->ReleaseStringUTFChars(env, key, get.key);

    return get.val;
}

JNIEXPORT void SYSM_JNIx(ProcessExe_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jlong pid)
{
    int status;
    sysm_proc_exe_t proc_exe;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    if ((status = sysm_proc_exe_get(sysm,
                                    (pid_t) pid,
                                    &proc_exe)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_PROCEXE(cls);

    JAVA_SYSM_SET_FIELDS_PROCEXE(obj, proc_exe);
}

JNIEXPORT void SYSM_JNIx(ProcessMem_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jlong pid)
{
    int status;
    sysm_proc_mem_t proc_mem;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    if ((status = sysm_proc_mem_get(sysm, (pid_t) pid, &proc_mem)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_PROCMEM(cls);

    JAVA_SYSM_SET_FIELDS_PROCMEM(obj, proc_mem);
}

JNIEXPORT void SYSM_JNIx(ProcessOwn_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jlong pid)
{
    int status;
    sysm_proc_own_t proc_own;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    if ((status = sysm_proc_own_get(sysm,
                                    (pid_t) pid,
                                    &proc_own)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_PROCOWN(cls);

    JAVA_SYSM_SET_FIELDS_PROCOWN(obj, proc_own);
}

JNIEXPORT void SYSM_JNIx(ProcessOwner_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jlong pid)
{
    int status;
    sysm_proc_owner_t proc_owner;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    if ((status = sysm_proc_owner_get(sysm, (pid_t) pid, &proc_owner)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_PROCOWNER(cls);

    JAVA_SYSM_SET_FIELDS_PROCOWNER(obj, proc_owner);
}

JNIEXPORT void SYSM_JNIx(ProcessStat_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj)
{
    int status;
    sysm_proc_stat_t proc_stat;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    status = sysm_proc_stat_get(sysm, &proc_stat);

    if (status != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_PROCSTAT(cls);

    JAVA_SYSM_SET_FIELDS_PROCSTAT(obj, proc_stat);
}

JNIEXPORT void SYSM_JNIx(ProcessState_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jlong pid)
{
    int status;
    sysm_proc_state_t proc_state;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    if ((status = sysm_proc_state_get(sysm, pid, &proc_state)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_PROCSTATE(cls);

    JAVA_SYSM_SET_FIELDS_PROCSTATE(obj, proc_state);

}

JNIEXPORT void JNICALL SYSM_JNIx(ProcessTime_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jlong pid)
{
    int status;
    sysm_proc_time_t proc_time;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    status = sysm_proc_time_get(sysm, pid, &proc_time);

    if (status != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_PROCTIME(cls);

    JAVA_SYSM_SET_FIELDS_PROCTIME(obj, proc_time);
}

JNIEXPORT void SYSM_JNIx(ResourceLimit_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj)
{
    int status;
    sysm_resource_limit_t resource_limit;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    if ((status = sysm_resource_limit_get(sysm, &resource_limit)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_RESOURCELIMIT(cls);

    JAVA_SYSM_SET_FIELDS_RESOURCELIMIT(obj, resource_limit);

}

JNIEXPORT void SYSM_JNIx(DiskUsage_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jstring jname)
{
    int status;
    sysm_disk_usage_t disk_usage;
    jclass cls = JENV->GetObjectClass(env, obj);
    const char *name;

    dSYSM_VOID;

    name = jname ? JENV->GetStringUTFChars(env, jname, 0) : NULL;

    if ((status = sysm_disk_usage_get(sysm, name, &disk_usage)) != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_DISKUSAGE(cls);

    JAVA_SYSM_SET_FIELDS_DISKUSAGE(obj, disk_usage);

}

JNIEXPORT void SYSM_JNIx(NetInfo_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj)
{
    int status;
    sysm_net_info_t net_info;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    status = sysm_net_info_get(sysm, &net_info);

    if (status != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_NETINFO(cls);
    JAVA_SYSM_SET_FIELDS_NETINFO(obj, net_info);
}

static int jbyteArray_to_sysm_net_address(JNIEnv *env, jbyteArray jaddress,
                                           sysm_net_address_t *address)
{
    jsize len = JENV->GetArrayLength(env, jaddress);

    JENV->GetByteArrayRegion(env, jaddress, 0, len,
                             (jbyte *)&address->addr.in6);

    switch (len) {
        case 4:
            address->family = SYSM_AF_INET;
            break;
        case 4*4:
            address->family = SYSM_AF_INET6;
            break;
        default:
            return EINVAL;
    }

    return SYSM_OK;
}

JNIEXPORT void SYSM_JNIx(NetStat_stat)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jint flags, jbyteArray jaddress, jlong jport)
{
    int status;
    sysm_net_stat_t netstat;
    jclass cls;
    jfieldID id;
    jintArray states;
    jint tcp_states[SYSM_TCP_UNKNOWN];
    sysm_net_address_t address;
    jboolean has_port = (jboolean) (jport != -1);

    dSYSM_VOID;

    if (has_port)
    {
        status = jbyteArray_to_sysm_net_address(env, jaddress, &address);
        if (status == SYSM_OK)
            status = sysm_net_stat_port_get(sysm, &netstat, flags, &address, jport);
    }
    else
    {
        status = sysm_net_stat_get(sysm, &netstat, flags);
    }

    if (status != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, flags);
        return;
    }

    cls = JENV->GetObjectClass(env, obj);

    JAVA_SYSM_INIT_FIELDS_NETSTAT(cls);
    JAVA_SYSM_SET_FIELDS_NETSTAT(obj, netstat);

    if (sizeof(tcp_states[0]) == sizeof(netstat.tcp_states[0]))
    {
        memcpy(&tcp_states[0], &netstat.tcp_states[0], sizeof(netstat.tcp_states));
    }
    else
    {
        for (int i = 0;i < SYSM_TCP_UNKNOWN;i++)
        {
            tcp_states[i] = netstat.tcp_states[i];
        }
    }

    states = JENV->NewIntArray(env, SYSM_TCP_UNKNOWN);
    if (JENV->ExceptionCheck)
        return;

    JENV->SetIntArrayRegion(env, states, 0, SYSM_TCP_UNKNOWN, tcp_states);

    id = JENV->GetFieldID(env, cls, "tcpStates", "[I");
    JENV->SetObjectField(env, obj, id, states);
}

JNIEXPORT void SYSM_JNIx(NetInterfaceConfig_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jstring jname)
{
    int status;
    sysm_net_interface_config_t ifconfig;
    const char *name;
    jclass cls = JENV->GetObjectClass(env, obj);

    dSYSM_VOID;

    name = jname ? JENV->GetStringUTFChars(env, jname, 0) : NULL;

    status = sysm_net_interface_config_get(sysm, name, &ifconfig);
    if (status != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_NETINTERFACECONFIG(cls);

    JAVA_SYSM_SET_FIELDS_NETINTERFACECONFIG(obj, ifconfig);
}

JNIEXPORT void SYSM_JNIx(NetInterfaceStat_gather)
  (JNIEnv *env, jobject obj, jobject sysm_obj, jstring jname)
{
    int status;
    sysm_net_interface_stat_t ifstat;
    jclass cls = JENV->GetObjectClass(env, obj);
    const char *name;

    dSYSM_VOID;

    name = jname ? JENV->GetStringUTFChars(env, jname, 0) : NULL;

    status = sysm_net_interface_stat_get(sysm, name, &ifstat);

    if (status != SYSM_OK)
    {
        sysm_throw_error(env, jsysm, status);
        return;
    }

    JAVA_SYSM_INIT_FIELDS_NETINTERFACESTAT(cls);
    JAVA_SYSM_SET_FIELDS_NETINTERFACESTAT(obj, ifstat);
}