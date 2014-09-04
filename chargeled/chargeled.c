/*
 * Copyright (C) 2014 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "[CHG_LED]"
#define KLOG_LEVEL 6

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <cutils/klog.h>
#include <cutils/uevent.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>

#define POWER_SUPPLY_SUBSYSTEM "power_supply"

#define BATTERY_STATUS_FILE "/sys/class/power_supply/battery/status"
#define AMBER_LED "/sys/class/leds/amber/brightness"
#define GREEN_LED "/sys/class/leds/green/brightness"

#define CYCLE_FREQ 2000000
#define LED_OFF "0"
#define LED_ON "1"

#define MAX_EPOLL_EVENTS 1
static int awake_poll_interval = -1; // -1 for no epoll timeout
static int uevent_fd;

enum { // Matches frameworks/native/include/batteryservice/BatteryService.h
    BATTERY_STATUS_UNKNOWN = 1,
    BATTERY_STATUS_CHARGING = 2,
    BATTERY_STATUS_DISCHARGING = 3,
    BATTERY_STATUS_NOT_CHARGING = 4,
    BATTERY_STATUS_FULL = 5,
};

struct sysfs_string_enum_map {
    char* s;
    int val;
};

struct sysfs_string_enum_map battery_status_map[] = {
    { "Unknown", BATTERY_STATUS_UNKNOWN },
    { "Charging", BATTERY_STATUS_CHARGING },
    { "Discharging", BATTERY_STATUS_DISCHARGING },
    { "Not charging", BATTERY_STATUS_NOT_CHARGING },
    { "Full", BATTERY_STATUS_FULL },
    { NULL, 0 },
};

static void uevent_init() {
    uevent_fd = uevent_open_socket(64*1024, true);

    if (uevent_fd >= 0)
        fcntl(uevent_fd, F_SETFL, O_NONBLOCK);
    else
        KLOG_ERROR(LOG_TAG, "uevent_init: uevent_open_socket failed\n");
}

static int map_sysfs_string(const char* str,
                            struct sysfs_string_enum_map map[]) {
    int i;
    int ret = -1;

    for (i = 0; map[i].s; i++) {
        if (!strcmp(str, map[i].s))
            ret = map[i].val;
    }

    if (ret == -1)
        KLOG_ERROR(LOG_TAG, "map_sysfs_string: unknown charging status '%s'\n",
            str);

    return ret;
}

static int get_charging_status() {
    const int SIZE = 128;
    char batt_stat_str[SIZE];
    int ret = -1;

    FILE *bstat;
    bstat = fopen(BATTERY_STATUS_FILE, "r");
    if (bstat) {
        if (fgets(batt_stat_str, SIZE, bstat) == NULL) {
            KLOG_ERROR(LOG_TAG,
                "get_charging_status: empty battery status file %s\n",
                BATTERY_STATUS_FILE);
            return -1;
        }

        if (!strcmp(batt_stat_str, "")) {
            KLOG_ERROR(LOG_TAG,
                "get_charging_status: empty battery status file %s\n",
                BATTERY_STATUS_FILE);
            return -1;
        }

        size_t ln = strlen(batt_stat_str) - 1;
        if (batt_stat_str[ln] == '\n')
            batt_stat_str[ln] = '\0';

        fclose(bstat);
        ret = map_sysfs_string(batt_stat_str, battery_status_map);
    } else {
        KLOG_ERROR(LOG_TAG, "get_charging_status: could not open %s\n",
            BATTERY_STATUS_FILE);
        return -1;
    }

    return ret;
}

static int update_led(int charge_status) {
    FILE *aled, *gled;
    aled = fopen(AMBER_LED, "w");
    if (!aled) {
        KLOG_ERROR(LOG_TAG, "update_led: could not open amber LED: %s\n",
            AMBER_LED);
        return -1;
    } else {
        gled = fopen(GREEN_LED, "w");
        if (!aled) {
            KLOG_ERROR(LOG_TAG, "update_led: could not open green LED: %s\n",
                GREEN_LED);
            return -1;
        }
    }

    KLOG_INFO(LOG_TAG, "update_led: setting charging status '%d'\n",
            charge_status);

    switch (charge_status) {
        case BATTERY_STATUS_CHARGING:
            fputs(LED_ON, aled);
            fputs(LED_OFF, gled);
            break;
        case BATTERY_STATUS_FULL:
            fputs(LED_OFF, aled);
            fputs(LED_ON, gled);
            break;
        default:
            fputs(LED_OFF, aled);
            fputs(LED_OFF, gled);
            break;
    }

    fclose(aled);
    fclose(gled);

    return 0;
}

static int last_charge_status = BATTERY_STATUS_UNKNOWN;
static void chargeled_update() {
    int charge_status;

    charge_status = get_charging_status();
    if (charge_status == -1 || charge_status == last_charge_status)
        return;

    last_charge_status = charge_status;
    update_led(charge_status);
}

#define UEVENT_MSG_LEN 1024
static void uevent_event() {
    char msg[UEVENT_MSG_LEN+2];
    char *cp;
    int n;

    n = uevent_kernel_multicast_recv(uevent_fd, msg, UEVENT_MSG_LEN);
    if (n <= 0)
        return;
    if (n >= UEVENT_MSG_LEN)   /* overflow -- discard */
        return;

    msg[n] = '\0';
    msg[n+1] = '\0';
    cp = msg;

    while (*cp) {
        if (!strcmp(cp, "SUBSYSTEM=" POWER_SUPPLY_SUBSYSTEM)) {
            chargeled_update();
            break;
        }

        /* advance to after the next \0 */
        while (*cp++)
            ;
    }
}

static void chargeled_mainloop() {
    struct epoll_event ev;
    int epollfd;
    int maxevents = 0;

    epollfd = epoll_create(MAX_EPOLL_EVENTS);
    if (epollfd == -1) {
        KLOG_ERROR(LOG_TAG,
            "chargeled_mainloop: epoll_create failed; errno=%d\n", errno);
        return;
    }

    if (uevent_fd >= 0) {
        ev.events = EPOLLIN | EPOLLWAKEUP;
        ev.data.ptr = (void *)uevent_event;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, uevent_fd, &ev) == -1)
            KLOG_ERROR(LOG_TAG,
                "chargeled_mainloop: epoll_ctl for uevent_fd failed; errno=%d\n",
                errno);
        else
            maxevents++;
    }

    // If we've gotten this far, fire-off an LED update before the loop begins
    if (maxevents > 0)
        chargeled_update();

    while (1) {
        struct epoll_event events[maxevents];
        int nevents;
        int n;

        nevents = epoll_wait(epollfd, events, maxevents, awake_poll_interval);

        if (nevents == -1) {
            if (errno == EINTR)
                continue;
            KLOG_ERROR(LOG_TAG, "chargeled_mainloop: epoll_wait failed\n");
            break;
        }

        for (n = 0; n < nevents; ++n) {
            if (events[n].data.ptr)
                (*(void (*)())events[n].data.ptr)();
        }
    }

    return;
}

int main() {
    klog_set_level(KLOG_LEVEL);
    uevent_init();
    chargeled_mainloop();

    return 0;
}
