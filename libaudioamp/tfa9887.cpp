/*
 * Copyright (C) 2013, The CyanogenMod Project
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
#include <sys/ioctl.h>
#include <fcntl.h>

#include <system/audio.h>

//#define LOG_NDEBUG 0
#define LOG_TAG "tfa9887"
#include <cutils/log.h>

#include "tfa9887.h"

int tfa9887_set_mode(audio_mode_t mode) {
    unsigned int reg_value[2];
    int tfa9887_fd;
    int tfa9887l_fd;
    int ret = -1;

    /* Open the amplifier devices */
    if ((tfa9887_fd = open(TFA9887_DEVICE, O_RDWR)) < 0) {
        ALOGE("error opening amplifier device %s", TFA9887_DEVICE);
        return -1;
    }
    if ((tfa9887l_fd = open(TFA9887L_DEVICE, O_RDWR)) < 0) {
        ALOGE("error opening amplifier device %s", TFA9887L_DEVICE);
        return -1;
    }

    switch(mode) {
        case AUDIO_MODE_NORMAL:
            ALOGI("Mode: Playback");
            reg_value[0] = 1;
            reg_value[1] = 1; // enable dsp
            break;
        case AUDIO_MODE_RINGTONE:
            ALOGI("Mode: Ring");
            reg_value[0] = 1;
            reg_value[1] = 1; // enable dsp
            break;
        case AUDIO_MODE_IN_CALL:
        case AUDIO_MODE_IN_COMMUNICATION:
            ALOGI("Mode: Voice");
            reg_value[0] = 1;
            reg_value[1] = 0; // disable dsp
            break;
        default:
            ALOGI("Mode: Default");
            reg_value[0] = 1;
            reg_value[1] = 1; // enable dsp
            break;
    }

    /* Set DSP state */
    if ((ret = ioctl(tfa9887_fd, TPA9887_ENABLE_DSP, &reg_value)) != 0) {
        ALOGE("ioctl %d failed. ret = %d", TPA9887_ENABLE_DSP, ret);
    }
    if ((ret = ioctl(tfa9887l_fd, TPA9887_ENABLE_DSP, &reg_value)) != 0) {
        ALOGE("ioctl %d failed. ret = %d", TPA9887_ENABLE_DSP, ret);
    }

    close(tfa9887_fd);
    close(tfa9887l_fd);

    return ret;
}
