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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define LOGE(...) fprintf(stdout, "E:" __VA_ARGS__)
#define LOGW(...) fprintf(stdout, "W:" __VA_ARGS__)
#define LOGI(...) fprintf(stdout, "I:" __VA_ARGS__)

#define BATTERY_STATUS_FILE "/sys/class/power_supply/battery/status"
#define AMBER_LED "/sys/class/leds/amber/brightness"
#define GREEN_LED "/sys/class/leds/green/brightness"

#define CYCLE_FREQ 2000000
#define LED_OFF "0"
#define LED_ON "1"

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

static int map_sysfs_string(const char* str,
                            struct sysfs_string_enum_map map[]) {
    int i;
    int ret = -1;

    for (i = 0; map[i].s; i++) {
        if (!strcmp(str, map[i].s))
            ret = map[i].val;
    }

    if (ret == -1)
        LOGE("[ChargeLED] Unknown charging status '%s'\n", str);

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
            LOGE("[ChargeLED] Empty battery status file %s\n",
                BATTERY_STATUS_FILE);
            return -1;
        }

        if (!strcmp(batt_stat_str, "")) {
            LOGE("[ChargeLED] Empty battery status file %s\n",
                BATTERY_STATUS_FILE);
            return -1;
        }

        size_t ln = strlen(batt_stat_str) - 1;
        if (batt_stat_str[ln] == '\n')
            batt_stat_str[ln] = '\0';

        fclose(bstat);
        ret = map_sysfs_string(batt_stat_str, battery_status_map);
    } else {
        LOGE("[ChargeLED] Could not open %s\n", BATTERY_STATUS_FILE);
        return -1;
    }

    return ret;
}

static int update_led(int charge_status) {
    FILE *aled, *gled;
    aled = fopen(AMBER_LED, "w");
    if (!aled) {
        LOGE("[ChargeLED] Could not open amber LED: %s\n", AMBER_LED);
        return -1;
    } else {
        gled = fopen(GREEN_LED, "w");
        if (!aled) {
            LOGE("[ChargeLED] Could not open green LED: %s\n", GREEN_LED);
            return -1;
        }
    }

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

int main() {
    int charge_status;
    int prev_status = BATTERY_STATUS_UNKNOWN;
    int update_status = 0;

    while (update_status == 0) {
        charge_status = get_charging_status();
        if (charge_status == -1)
            break;
        if (charge_status != prev_status) {
            prev_status = charge_status;
            update_status = update_led(charge_status);
        } else {
            update_status = 0;
        }
        usleep(CYCLE_FREQ);
    }

    return 0;
}
