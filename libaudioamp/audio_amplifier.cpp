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

#include "audio_amplifier.h"

int amplifier_set_mode(audio_mode_t mode) {
    struct rt5501_comm_data amp_data;
    struct rt5501_config amp_config;
    int fd;
    int ret = -1;

    switch(mode)
    {
        case AUDIO_MODE_NORMAL:
            ALOGI("Playback mode");
            amp_config.reg_len = sizeof(playback_data) / sizeof(rt5501_reg_data);
            memcpy (&amp_config.reg, playback_data, sizeof(playback_data) / sizeof(rt5501_reg_data));
            amp_data.out_mode = RT5501_MODE_PLAYBACK;
            amp_data.config = amp_config;
            break;
        case AUDIO_MODE_RINGTONE:
            ALOGI("Ring mode");
            amp_config.reg_len = sizeof(ring_data) / sizeof(rt5501_reg_data);
            memcpy (&amp_config.reg, ring_data, sizeof(ring_data) / sizeof(rt5501_reg_data));
            amp_data.out_mode = RT5501_MODE_RING;
            amp_data.config = amp_config;
            break;
        case AUDIO_MODE_IN_CALL:
            ALOGI("Voice mode");
            amp_config.reg_len = sizeof(voice_data) / sizeof(rt5501_reg_data);
            memcpy (&amp_config.reg, voice_data, sizeof(voice_data) / sizeof(rt5501_reg_data));
            amp_data.out_mode = RT5501_MODE_VOICE;
            amp_data.config = amp_config;
            break;
        case AUDIO_MODE_IN_COMMUNICATION:
            ALOGI("Voice mode");
            amp_config.reg_len = sizeof(voice_data) / sizeof(rt5501_reg_data);
            memcpy (&amp_config.reg, voice_data, sizeof(voice_data) / sizeof(rt5501_reg_data));
            amp_data.out_mode = RT5501_MODE_VOICE;
            amp_data.config = amp_config;
            break;
        default:
            ALOGI("Default mode");
            amp_config.reg_len = sizeof(playback_data) / sizeof(rt5501_reg_data);
            memcpy (&amp_config.reg, playback_data, sizeof(playback_data) / sizeof(rt5501_reg_data));
            amp_data.out_mode = RT5501_MODE_PLAYBACK;
            amp_data.config = amp_config;
            break;
    }

	if ((fd = open(AMPLIFIER_DEVICE, O_RDWR)) < 0) {
		ALOGE("error opening amplifier device %s", AMPLIFIER_DEVICE);
		return -1;
	}

	if((ret = ioctl(fd, RT5501_SET_CONFIG, &amp_data)) != 0) {
		ALOGE("ioctl %d failed. ret = %d", RT5501_SET_CONFIG, ret);
    }

    close(fd);

    return ret;
}
