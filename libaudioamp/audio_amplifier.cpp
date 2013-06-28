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
#define LOG_TAG "libaudioamp"
#include <cutils/log.h>

#include "tfa9887.h"
#include "rt5501.h"
#include "audio_amplifier.h"

int mDevices = AUDIO_DEVICE_NONE;
audio_mode_t mMode = AUDIO_MODE_NORMAL;

void amplifier_set_devices(int devices) {
    if (devices != NULL) {
        if (mDevices != devices) {
            mDevices = devices;
            /* Set amplifier mode when device changes */
            amplifier_set_mode(mMode);
        }
    }
}

int amplifier_set_mode(audio_mode_t mode) {
    int ret = 0;

    mMode = mode;

    if (mDevices & AUDIO_DEVICE_OUT_WIRED_HEADSET || mDevices & AUDIO_DEVICE_OUT_WIRED_HEADPHONE) {
        /* Write config for headset amplifier */
        ret = rt5501_set_mode(mode);
    } else {
        /* Write config for speaker amplifier */
        //ret = tfa9887_set_mode(mode);
    }

    return ret;
}

int tfa9887_set_mode(audio_mode_t mode) {
    unsigned int reg_value[2];
    int tfa9887_fd;
    int tfa9887l_fd;
    int ret = -1;

    /* Open the amplifier devices */
    if ((tfa9887_fd = open(TFA9887_DEVICE, O_RDWR)) < 0) {
        ALOGE("[TFA9887] error opening amplifier device %s", TFA9887_DEVICE);
        return -1;
    }
    if ((tfa9887l_fd = open(TFA9887L_DEVICE, O_RDWR)) < 0) {
        ALOGE("[TFA9887L] error opening amplifier device %s", TFA9887L_DEVICE);
        return -1;
    }

    switch(mode)
    {
        case AUDIO_MODE_NORMAL:
            ALOGI("[TFA9887] Mode: Playback");
            reg_value[0] = 1;
            reg_value[1] = 1; // enable dsp
            break;
        case AUDIO_MODE_RINGTONE:
            ALOGI("[TFA9887] Mode: Ring");
            reg_value[0] = 1;
            reg_value[1] = 1; // enable dsp
            break;
        case AUDIO_MODE_IN_CALL:
        case AUDIO_MODE_IN_COMMUNICATION:
            ALOGI("[TFA9887] Mode: Voice");
            reg_value[0] = 1;
            reg_value[1] = 0; // disable dsp
            break;
        default:
            ALOGI("[TFA9887] Mode: Default");
            reg_value[0] = 1;
            reg_value[1] = 1; // enable dsp
            break;
    }

    /* Set DSP state */
    if((ret = ioctl(tfa9887_fd, TPA9887_ENABLE_DSP, &reg_value)) != 0) {
        ALOGE("[TFA9887] ioctl %d failed. ret = %d", TPA9887_ENABLE_DSP, ret);
    }
    if((ret = ioctl(tfa9887l_fd, TPA9887_ENABLE_DSP, &reg_value)) != 0) {
        ALOGE("[TFA9887L] ioctl %d failed. ret = %d", TPA9887_ENABLE_DSP, ret);
    }

    close(tfa9887_fd);
    close(tfa9887l_fd);

    return ret;
}

int rt5501_set_mode(audio_mode_t mode) {
    struct rt5501_comm_data amp_data;
    struct rt5501_config amp_config;
    int headsetohm = HEADSET_OM_UNDER_DETECT;
    int rt5501_fd;
    int ret = -1;

    /* Open the amplifier device */
    if ((rt5501_fd = open(RT5501_DEVICE, O_RDWR)) < 0) {
        ALOGE("[RT5501] error opening amplifier device %s", RT5501_DEVICE);
        return -1;
    }

    /* Get impedance of headset */
    if (ioctl(rt5501_fd, RT5501_QUERY_OM, &headsetohm) < 0)
        ALOGE("[RT5501] error querying headset impedance");

    switch(mode)
    {
        case AUDIO_MODE_NORMAL:
            /* For headsets with a impedance between 128ohm and 1000ohm */
            if (headsetohm >= HEADSET_128OM && headsetohm <= HEADSET_1KOM) {
                ALOGI("[RT5501] Mode: Playback 128");
                amp_config.reg_len = sizeof(rt5501_playback_128_data) / sizeof(rt5501_reg_data);
                memcpy(&amp_config.reg, rt5501_playback_128_data, sizeof(rt5501_playback_128_data));
            } else {
                ALOGI("[RT5501] Mode: Playback");
                amp_config.reg_len = sizeof(rt5501_playback_data) / sizeof(rt5501_reg_data);
                memcpy(&amp_config.reg, rt5501_playback_data, sizeof(rt5501_playback_data));
            }
            amp_data.out_mode = RT5501_MODE_PLAYBACK;
            amp_data.config = amp_config;
            break;
        case AUDIO_MODE_RINGTONE:
            ALOGI("[RT5501] Mode: Ring");
            amp_config.reg_len = sizeof(rt5501_ring_data) / sizeof(rt5501_reg_data);
            memcpy(&amp_config.reg, rt5501_ring_data, sizeof(rt5501_ring_data));
            amp_data.out_mode = RT5501_MODE_RING;
            amp_data.config = amp_config;
            break;
        case AUDIO_MODE_IN_CALL:
        case AUDIO_MODE_IN_COMMUNICATION:
            ALOGI("[RT5501] Mode: Voice");
            amp_config.reg_len = sizeof(rt5501_voice_data) / sizeof(rt5501_reg_data);
            memcpy(&amp_config.reg, rt5501_voice_data, sizeof(rt5501_voice_data));
            amp_data.out_mode = RT5501_MODE_VOICE;
            amp_data.config = amp_config;
            break;
        default:
            ALOGI("[RT5501] Mode: Default");
            amp_config.reg_len = sizeof(rt5501_playback_data) / sizeof(rt5501_reg_data);
            memcpy(&amp_config.reg, rt5501_playback_data, sizeof(rt5501_playback_data));
            amp_data.out_mode = RT5501_MODE_PLAYBACK;
            amp_data.config = amp_config;
            break;
    }

    /* Set the selected config */
    if((ret = ioctl(rt5501_fd, RT5501_SET_CONFIG, &amp_data)) != 0) {
        ALOGE("[RT5501] ioctl %d failed. ret = %d", RT5501_SET_CONFIG, ret);
    }

    close(rt5501_fd);

    return ret;
}
