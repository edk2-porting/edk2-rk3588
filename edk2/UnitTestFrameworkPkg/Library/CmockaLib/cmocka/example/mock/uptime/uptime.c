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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "proc_uptime.h"

static char *calc_uptime(void)
{
    uint32_t up_minutes, up_hours, up_days, up_weeks, up_years;
    ssize_t pos = 0;
    size_t comma = 0;
    double uptime_secs, idle_secs;
    char buf[1024] = {0};
    int up;

    up = uptime("/proc/uptime", &uptime_secs, &idle_secs);
    if (up == 0) {
        return NULL;
    }

    up_years = ((uint32_t)uptime_secs / (60 * 60 * 24 * 365)) % 10;
    up_weeks = ((uint32_t)uptime_secs / (60 * 60 * 24 * 7)) % 52;
    up_days = ((uint32_t)uptime_secs / (60 * 60 * 24)) % 7;

    pos += snprintf(buf + pos, sizeof(buf) - pos, "up ");

    up_minutes = (uint32_t)uptime_secs / 60;
    up_hours = up_minutes / 60;
    up_hours = up_hours % 24;
    up_minutes = up_minutes % 60;

    if (up_years > 0) {
        pos += snprintf(buf + pos, sizeof(buf) - pos,
                        "%u %s",
                        up_years,
                        up_years > 1 ? "years" : "year");
        comma++;
    }

    if (up_weeks > 0) {
        pos += snprintf(buf + pos, sizeof(buf) - pos,
                        "%s%u %s",
                        comma > 0 ? ", " : "",
                        up_weeks,
                        up_weeks > 1 ? "weeks" : "week");
        comma++;
    }

    if (up_days > 0) {
        pos += snprintf(buf + pos, sizeof(buf) - pos,
                        "%s%u %s",
                        comma > 0 ? ", " : "",
                        up_days,
                        up_days > 1 ? "days" : "day");
        comma++;
    }

    if (up_hours > 0) {
        pos += snprintf(buf + pos, sizeof(buf) - pos,
                        "%s%u %s",
                        comma > 0 ? ", " : "",
                        up_hours,
                        up_hours > 1 ? "hours" : "hour");
        comma++;
    }

    if (up_minutes > 0 || (up_minutes == 0 && uptime_secs < 60)) {
        pos += snprintf(buf + pos, sizeof(buf) - pos,
                        "%s%u %s",
                        comma > 0 ? ", " : "",
                        up_minutes,
                        up_minutes != 1 ? "minutes" : "minute");
        comma++;
    }

    return strdup(buf);
}

#ifndef UNIT_TESTING
int main(void)
{
    char *uptime_str = NULL;

    uptime_str = calc_uptime();
    if (uptime_str == NULL) {
        fprintf(stderr, "Failed to read uptime\n");
        return 1;
    }

    printf("%s\n", uptime_str);

    free(uptime_str);

    return 0;
}
#endif /* UNIT_TESTING */
