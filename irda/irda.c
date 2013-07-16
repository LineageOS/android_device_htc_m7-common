/*
 * Copyright (C) 2013 Cyanogenmod Project
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

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_TAG "irda"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/irda.h>

#define CIR_DEVICE "/dev/ttyHSL2"
#define CIR_RESET "/sys/class/htc_cir/cir/reset_cir"

#define MAX_CMD_LEN 1024

#define UPPER_BITS(x) (((x) & 0xFF00) >> 8)
#define LOWER_BITS(x) ((x) & 0x00FF)

static int irda_enable(int on)
{
    int fd, ret = 0;
    size_t bytes_written;
    char enable;

    /* Don't ask me why 0 == off, 1 == reset, 2 == on */
    enable = on ? '2' : '0';

    fd = open(CIR_RESET, O_WRONLY);
    if (fd < 0) {
        ALOGE("%s: Error opening %s: %d\n", __func__, CIR_RESET, errno);
        ret = -EINVAL;
        goto enable_err;
    }

    bytes_written = write(fd, &enable, 1);
    if (bytes_written != 1) {
        ALOGE("%s: Wrote %u bytes of %u!\n", __func__, bytes_written, 1);
        ret = -ECOMM;
        goto enable_err1;
    }
    usleep(750000);

enable_err1:
    close(fd);
enable_err:
    return ret;
}

static int irda_send(char *buf, size_t len)
{
    int fd, ret = 0;
    size_t bytes_written;
    struct termios t_opts;
    char wakeup = -69;

    /* TODO: are these necessary? */
    ALOGI("%s: enabling\n", __func__);
    irda_enable(1);

    fd = open(CIR_DEVICE, O_WRONLY | O_NOCTTY);
    if (fd < 0) {
        ALOGE("%s: Error opening %s: %d\n", __func__, CIR_DEVICE, errno);
        ret = -EINVAL;
        goto send_err;
    }

    ALOGI("%s: flushing\n", __func__);
    tcflush(fd, TCIOFLUSH);

    if (tcgetattr(fd, &t_opts) < 0) {
        ALOGE("%s: Error getting device attributes: %d\n", __func__, errno);
        ret = -EINVAL;
        goto send_err1;
    }

    t_opts.c_iflag = IGNPAR;
    t_opts.c_oflag = 0;
    t_opts.c_cflag = t_opts.c_cflag & (0xFFFFE600 | 0x18F2);
    t_opts.c_lflag = 0;
    t_opts.c_line = 0;
    //t_opts.c_cc = 0;

    ALOGI("%s: setting attributes\n", __func__);
    if (tcsetattr(fd, TCSANOW, &t_opts) < 0) {
        ALOGE("%s: Error setting device attributes: %d\n", __func__, errno);
        ret = -EINVAL;
        goto send_err1;
    }

    ALOGI("%s: writing buffer\n", __func__);
    bytes_written = write(fd, buf, len);
    if (bytes_written != len) {
        ALOGE("%s: Wrote %u bytes of %u!\n", __func__, bytes_written, len);
        ret = -ECOMM;
        goto send_err1;
    }

send_err1:
    close(fd);
send_err:
    irda_enable(0);
    return ret;
}

static size_t split_cmd(char *cmd, size_t cmd_len,
        unsigned *split_cmd, size_t split_len)
{
    char *code;
    size_t code_cnt = 0;

    code = strtok(cmd, ",");
    while (code != NULL) {
        if (code_cnt < split_len) {
            split_cmd[code_cnt++] = atoi(code);
        } else {
            ALOGW("%s: Truncating command buffer! size: %u\n",
                    __func__, split_len);
        }
        code = strtok(NULL, ",");
    }

    return code_cnt;
}

static size_t irda_generate_htc_command(char *buf, size_t len,
        char *htc_cmd, size_t cmd_len)
{
    unsigned split_buf[MAX_CMD_LEN - 7];
    size_t split_len, htc_cmd_len;
    unsigned i;
    char checksum = 0;

    /* cmd positions 0-7 are for header */
    htc_cmd_len = 7;
    split_len = split_cmd(buf, len, split_buf, MAX_CMD_LEN - 7);

    /* position 0 is frequency, process other values > 127 */
    for (i = 1; i < split_len; i++) {
        if (htc_cmd_len >= cmd_len) {
            ALOGW("%s: Truncating command buffer! size: %u\n",
                    __func__, htc_cmd_len);
            break;
        }
        if (split_buf[i] > 127) {
            /* HTC format for values > 127, upper bits set 0x80 */
            htc_cmd[htc_cmd_len++] = (0x80 | UPPER_BITS(split_buf[i]));
            htc_cmd[htc_cmd_len++] = LOWER_BITS(split_buf[i]);
        } else {
            htc_cmd[htc_cmd_len++] = split_buf[i];
        }
    }

    htc_cmd[0] = 0xaa;
    //htc_cmd[1] = 0xbb; /* only used in some kinds of packets?? */
    htc_cmd[1] = UPPER_BITS(htc_cmd_len);
    htc_cmd[2] = LOWER_BITS(htc_cmd_len);
    htc_cmd[3] = 0x04; /* cmd[3] request type? */
    htc_cmd[4] = 0x01; /* cmd[4] repeat count? */
    htc_cmd[5] = UPPER_BITS(split_buf[0]); /* cmd[5] frequency upper */
    htc_cmd[6] = LOWER_BITS(split_buf[0]); /* cmd[6] frequency lower */

    for (i = 0; i < htc_cmd_len; i++) {
        checksum ^= htc_cmd[i];
    }
    htc_cmd[htc_cmd_len++] = checksum;

    return htc_cmd_len;
}

static void irda_send_ircode(char *buffer, int length)
{
    char buf[length + 1];
    char cmd[MAX_CMD_LEN];
    size_t cmd_len;
    unsigned i;

    memcpy(buf, buffer, length);
    buf[length] = '\0'; /* null terminate */
    ALOGI("%s: Received IR buffer %s\n", __func__, buf);

    cmd_len = irda_generate_htc_command(buf, length + 1, cmd, MAX_CMD_LEN);
    ALOGI("%s: Generated HTC IR command\n", __func__);
    for (i = 0; i < cmd_len; i++) {
        ALOGI("cmd[%u] = 0x%x", i, cmd[i]);
    }

    irda_send(cmd, cmd_len);
}

static int open_irda(const struct hw_module_t *module, const char *name,
    struct hw_device_t **device)
{
    struct irda_device_t *dev = malloc(sizeof(struct irda_device_t));
    memset(dev, 0, sizeof(dev));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t *) module;
    dev->send_ircode = irda_send_ircode;

    *device = (struct hw_device_t *) dev;

    return 0;
}

static struct hw_module_methods_t irda_module_methods = {
    .open = open_irda,
};

struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .module_api_version = 1,
    .hal_api_version = 0,
    .id = IRDA_HARDWARE_MODULE_ID,
    .name = "Irda HAL Module",
    .author = "The CyanogenMod Project",
    .methods = &irda_module_methods,
};
