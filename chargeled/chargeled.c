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
#include <unistd.h>
#include <sys/time.h>
#include "chargeled.h"

static char get_charging_status() {
    char charge_status[30]; // Full, Charging, Not charging

    FILE *bstat;
    bstat = fopen(BATTERY_STATUS_FILE, "r");
    if (bstat) {
        fgets(charge_status, 30, bstat);
        fclose(bstat);
    } else {
        LOGE("[ChargeLED] Could not open %s\n", BATTERY_STATUS_FILE);
        return 'X';
    }

    return charge_status[0];
}

static int update_led(char charge_status) {
    const char on[] = "1";
    const char off[] = "0";

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
        case 'C':
            fputs(on, aled);
            fputs(off, gled);
            break;
        case 'F':
            fputs(off, aled);
            fputs(on, gled);
            break;
        default:
            fputs(off, aled);
            fputs(off, gled);
            break;
    }

    fclose(aled);
    fclose(gled);

    return 0;
}

int main() {
    char charge_status;
    char prev_status = 'Z';
    int update_status = 0;

    while (update_status == 0) {
        charge_status = get_charging_status();
        switch (charge_status) {
            case 'C':
            case 'F':
            case 'N':
                if (charge_status != prev_status) {
                    prev_status = charge_status;
                    update_status = update_led(charge_status);
                } else {
                    update_status = 0;
                }
                usleep(CYCLE_FREQ);
                break;
            default:
                LOGE("[ChargeLED] Invalid charge state, exiting\n");
                update_status = -1;
                break;
        }
    }

    return 0;
}
