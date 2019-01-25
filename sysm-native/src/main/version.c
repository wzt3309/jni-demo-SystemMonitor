//
// Created by wzt on 11/7/17.
//

#include "sysm.h"

static sysm_version_t sysm_version = {
        "1.0.0",
        1,
        0
};

sysm_version_t *sysm_version_get(void)
{
    return &sysm_version;
}