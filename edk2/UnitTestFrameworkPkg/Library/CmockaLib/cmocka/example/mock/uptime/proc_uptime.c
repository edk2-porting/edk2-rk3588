/*
 * Copyright 2018 Andreas Scheider <asn@cryptomilk.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <locale.h>
#include <unistd.h>

#include "proc_uptime.h"

#define UPTIME_FILE  "/proc/uptime"

int uptime(const char *uptime_path, double *uptime_secs, double *idle_secs)
{
    double up = 0;
    double idle = 0;
    char *savelocale = NULL;
    char buf[1024] = {0};
    ssize_t nread;
    int fd = -1;
    int rc;

    if (uptime_path == NULL) {
        uptime_path = UPTIME_FILE;
    }

    fd = open(uptime_path, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    nread = read(fd, buf, sizeof(buf));
    close(fd);
    if (nread < 0) {
        return 0;
    }

    savelocale = strdup(setlocale(LC_NUMERIC, NULL));
    if (savelocale == NULL) {
        return 0;
    }

    setlocale(LC_NUMERIC, "C");
    rc = sscanf(buf, "%lf %lf", &up, &idle);
    setlocale(LC_NUMERIC, savelocale);
    free(savelocale);
    if (rc < 2) {
        errno = EFAULT;
        return 0;
    }

    if (uptime_secs != NULL) {
        *uptime_secs = up;
    }
    if (idle_secs != NULL) {
        *idle_secs = idle;
    }

    return (int)up;
}
