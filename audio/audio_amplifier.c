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
#include <hardware/hardware.h>
#include <system/audio.h>

#include "tfa9887.h"
#include "rt5501.h"

#define UNUSED __attribute__((unused))

static int mDevices = AUDIO_DEVICE_NONE;
static audio_mode_t mMode = AUDIO_MODE_NORMAL;

static int amp_module_open(const hw_module_t *module, const char *name,
        hw_device_t **device)
{
    UNUSED(module);
    UNUSED(name);
    UNUSED(device);

    return 0;
}

static int amp_init(struct amplifier_module *module)
{
    UNUSED(module);

    return 0;
}

static int amp_set_devices(struct amplifier_module *module, int devices)
{
    UNUSED(module);

    if (devices != 0) {
        if (mDevices != devices) {
            mDevices = devices;
            /* Set amplifier mode when device changes */
            amplifier_set_mode(mMode);
        }
    }

    return 0;
}

static int amp_set_mode(struct amplifier_module *module, audio_mode_t mode)
{
    int ret = 0;

    UNUSED(module);

    mMode = mode;

    if (mDevices & AUDIO_DEVICE_OUT_WIRED_HEADSET || mDevices & AUDIO_DEVICE_OUT_WIRED_HEADPHONE) {
        /* Write config for headset amplifier */
        ret = rt5501_set_mode(mode);
    } else {
        /* Write config for speaker amplifier */
        ret = tfa9887_set_mode(mode);
    }

    return ret;
}

static int amp_stream_start(struct amplifier_module *module,
        struct audio_stream_out *stream, bool offload)
{
    UNUSED(module);
    UNUSED(stream);
    UNUSED(offload);

    return 0;
}

static int amp_stream_standby(struct amplifier_module *module,
        struct audio_stream_out *stream)
{
    UNUSED(module);
    UNUSED(stream);

    return 0;
}

static int amp_shutdown(struct amplifier_module *module)
{
    UNUSED(module);

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
    .open = amp_module_open,
};

struct amplifier_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AMPLIFIER_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AMPLIFIER_HARDWARE_MODULE_ID,
        .name = "M7 audio amplifier HAL",
        .author = "The CyanogenMod Open Source Project",
        .methods = &hal_module_methods,
        .dso = NULL,
        .reserved = {0},
    },
    .init = amp_init,
    .set_devices = amp_set_devices,
    .set_mode = amp_set_mode,
    .stream_start = amp_stream_start,
    .stream_standby = amp_stream_standby,
    .shutdown = amp_shutdown,
};
