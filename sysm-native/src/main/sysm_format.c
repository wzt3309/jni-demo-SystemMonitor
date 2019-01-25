//
// Created by wzt on 11/7/17.
//

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

#include "sysm.h"

static char *sysm_error_str(int err)
{
    switch (err)
    {
        case SYSM_ENOTIMPL:
            return "This method has not been implemented yet";
        default:
            return "Error string not specified yet";
    }
}

char *sysm_strerror(sysm_t *sysm, int err)
{
    char *buf;

    if (err < 0)
        return sysm->errbuf;

    if (err > SYSM_START_ERROR)
        return sysm_error_str(err);

    return sysm_strerror_get(err, sysm->errbuf, sizeof(sysm->errbuf));
}

char *sysm_strerror_get(int err, char *errbuf, size_t buflen)
{
    char *buf = NULL;

    if (strerror_r(err, errbuf, buflen) < 0)
        buf = "Unknown Error";

    if (buf != NULL) {
        strncpy(errbuf, buf, buflen);
        errbuf[buflen - 1] = '\0';
    }

    return errbuf;
}

#define R_SIZE_MAX 2048
int sysm_user_name_get(int uid, char *buf, int buflen)
{
    struct passwd *pw = NULL, pwbuf;
    char buffer[R_SIZE_MAX];

    if (getpwuid_r(uid, &pwbuf, buffer, sizeof(buffer), &pw) != 0)
        return errno;
    if (!pw)
        return ENOENT;

    strncpy(buf, pw->pw_name, buflen);
    buf[buflen - 1] = '\0';
    return SYSM_OK;

}

int sysm_group_name_get(int gid, char *buf, int buflen)
{
    struct group *gr, grbuf;
    char buffer[R_SIZE_MAX];

    if (getgrgid_r(gid, &grbuf, buffer, sizeof(buffer), &gr) != 0)
        return errno;

    if (gr && gr->gr_name)
        strncpy(buf, gr->gr_name, buflen);
    else
        sprintf(buf, "%d", gid);
    buf[buflen - 1] = '\0';

    return SYSM_OK;
}