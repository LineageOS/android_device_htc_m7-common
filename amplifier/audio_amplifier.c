/*
 * Copyright (C) 2015, The CyanogenMod Project
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

#define LOG_TAG "audio_amplifier"
//#define LOG_NDEBUG 0

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <cutils/log.h>

#include <hardware/audio_amplifier.h>

#include "tfa9887.h"
#include "rt5501.h"

#define UNUSED __attribute__((unused))

#define DEVICE_OUT_WIRED_HEADSET 0x4
#define DEVICE_OUT_WIRED_HEADPHONE 0x8

typedef struct m7_device {
    amplifier_device_t amp_dev;
    uint32_t current_input_devices;
    uint32_t current_output_devices;
    audio_mode_t current_mode;
} m7_device_t;

static m7_device_t *m7_dev = NULL;

static int amp_set_mode(amplifier_device_t *device, audio_mode_t mode)
{
    int ret = 0;
    m7_device_t *tfa9887 = (m7_device_t *) device;

    tfa9887->current_mode = mode;

    if ((tfa9887->current_output_devices & DEVICE_OUT_WIRED_HEADSET) ||
            (tfa9887->current_output_devices & DEVICE_OUT_WIRED_HEADPHONE)) {
        /* Write config for headset amplifier */
        ret = rt5501_set_mode(mode);
    } else {
        /* Write config for speaker amplifier */
        ret = tfa9887_set_mode(mode);
    }

    return ret;
}

static int amp_set_output_devices(amplifier_device_t *device, uint32_t devices)
{
    m7_device_t *tfa9887 = (m7_device_t *) device;

    if (devices != 0) {
        if (tfa9887->current_output_devices != devices) {
            tfa9887->current_output_devices = devices;
            /* Set amplifier mode when device changes */
            amp_set_mode(device, tfa9887->current_mode);
        }
    }

    return 0;
}

static int amp_dev_close(hw_device_t *device)
{
    m7_device_t *tfa9887 = (m7_device_t *) device;

    free(tfa9887);

    return 0;
}

static int amp_module_open(const hw_module_t *module, UNUSED const char *name,
        hw_device_t **device)
{
    if (m7_dev) {
        ALOGE("%s:%d: Unable to open second instance of TFA9887 amplifier\n",
                __func__, __LINE__);
        return -EBUSY;
    }

    m7_dev = calloc(1, sizeof(m7_device_t));
    if (!m7_dev) {
        ALOGE("%s:%d: Unable to allocate memory for amplifier device\n",
                __func__, __LINE__);
        return -ENOMEM;
    }

    m7_dev->amp_dev.common.tag = HARDWARE_DEVICE_TAG;
    m7_dev->amp_dev.common.module = (hw_module_t *) module;
    m7_dev->amp_dev.common.version = HARDWARE_DEVICE_API_VERSION(1, 0);
    m7_dev->amp_dev.common.close = amp_dev_close;

    m7_dev->amp_dev.set_input_devices = NULL;
    m7_dev->amp_dev.set_output_devices = amp_set_output_devices;
    m7_dev->amp_dev.set_mode = amp_set_mode;
    m7_dev->amp_dev.output_stream_start = NULL;
    m7_dev->amp_dev.input_stream_start = NULL;
    m7_dev->amp_dev.output_stream_standby = NULL;
    m7_dev->amp_dev.input_stream_standby = NULL;

    m7_dev->current_input_devices = 0;
    m7_dev->current_output_devices = 0;
    m7_dev->current_mode = AUDIO_MODE_NORMAL;

    *device = (hw_device_t *) m7_dev;

    tfa9887_open();

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
    .open = amp_module_open,
};

amplifier_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AMPLIFIER_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AMPLIFIER_HARDWARE_MODULE_ID,
        .name = "M7 audio amplifier HAL",
        .author = "The CyanogenMod Open Source Project",
        .methods = &hal_module_methods,
    },
};
