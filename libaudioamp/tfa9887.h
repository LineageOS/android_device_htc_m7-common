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

#define TFA9887_DEVICE "/dev/tfa9887"
#define TFA9887L_DEVICE "/dev/tfa9887l"

#define TPA9887_IOCTL_MAGIC 'a'
#define TPA9887_WRITE_CONFIG	_IOW(TPA9887_IOCTL_MAGIC, 0x01, unsigned int)
#define TPA9887_READ_CONFIG	_IOW(TPA9887_IOCTL_MAGIC, 0x02, unsigned int)
#define TPA9887_ENABLE_DSP	_IOW(TPA9887_IOCTL_MAGIC, 0x03, unsigned int)
#define TPA9887_WRITE_L_CONFIG	_IOW(TPA9887_IOCTL_MAGIC, 0x04, unsigned int)
#define TPA9887_READ_L_CONFIG	_IOW(TPA9887_IOCTL_MAGIC, 0x05, unsigned int)
#define TPA9887_KERNEL_LOCK    _IOW(TPA9887_IOCTL_MAGIC, 0x06, unsigned int)

int tfa9887_set_mode(audio_mode_t mode);
