#!/system/bin/sh
# Copyright (c) 2009-2012, The Linux Foundation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of The Linux Foundation nor
#       the names of its contributors may be used to endorse or promote
#       products derived from this software without specific prior written
#       permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

target=`getprop ro.board.platform`
case "$target" in
    "msm8960")
         echo 1 > /sys/module/rpm_resources/enable_low_power/L2_cache
         echo 1 > /sys/module/rpm_resources/enable_low_power/pxo
         echo 1 > /sys/module/rpm_resources/enable_low_power/vdd_dig
         echo 1 > /sys/module/rpm_resources/enable_low_power/vdd_mem
         echo 1 > /sys/module/pm_8x60/modes/cpu0/retention/idle_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu1/retention/idle_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu2/retention/idle_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu3/retention/idle_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu0/power_collapse/suspend_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu1/power_collapse/suspend_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu2/power_collapse/suspend_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu3/power_collapse/suspend_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu0/standalone_power_collapse/suspend_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu1/standalone_power_collapse/suspend_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu2/standalone_power_collapse/suspend_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu3/standalone_power_collapse/suspend_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu0/standalone_power_collapse/idle_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu1/standalone_power_collapse/idle_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu2/standalone_power_collapse/idle_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu3/standalone_power_collapse/idle_enabled
         echo 1 > /sys/module/pm_8x60/modes/cpu0/power_collapse/idle_enabled
         echo 0 > /sys/module/msm_thermal/core_control/enabled
         echo 1 > /sys/devices/system/cpu/cpu1/online
         echo 1 > /sys/devices/system/cpu/cpu2/online
         echo 1 > /sys/devices/system/cpu/cpu3/online
         #echo "ondemand" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
         #echo "ondemand" > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
         #echo "ondemand" > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
         #echo "ondemand" > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
         #echo 50000 > /sys/devices/system/cpu/cpufreq/ondemand/sampling_rate
         #echo 90 > /sys/devices/system/cpu/cpufreq/ondemand/up_threshold
         #echo 1 > /sys/devices/system/cpu/cpufreq/ondemand/io_is_busy
         #echo 4 > /sys/devices/system/cpu/cpufreq/ondemand/sampling_down_factor
         #echo 10 > /sys/devices/system/cpu/cpufreq/ondemand/down_differential
         #echo 70 > /sys/devices/system/cpu/cpufreq/ondemand/up_threshold_multi_core
         #echo 3 > /sys/devices/system/cpu/cpufreq/ondemand/down_differential_multi_core
         #echo 918000 > /sys/devices/system/cpu/cpufreq/ondemand/optimal_freq
         #echo 1026000 > /sys/devices/system/cpu/cpufreq/ondemand/sync_freq
         #echo 80 > /sys/devices/system/cpu/cpufreq/ondemand/up_threshold_any_cpu_load
         #chown -h system /sys/devices/system/cpu/cpufreq/ondemand/sampling_rate
         #chown -h system /sys/devices/system/cpu/cpufreq/ondemand/sampling_down_factor
         #chown -h system /sys/devices/system/cpu/cpufreq/ondemand/io_is_busy
         echo "interactive" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
         echo "interactive" >  /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
         echo "interactive" > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
         echo "interactive" > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
         echo 40000 > /sys/devices/system/cpu/cpufreq/interactive/min_sample_time
         echo 1134000 > /sys/devices/system/cpu/cpufreq/interactive/hispeed_freq
         echo 30000 > /sys/devices/system/cpu/cpufreq/interactive/above_hispeed_delay
         echo 30000 > /sys/devices/system/cpu/cpufreq/interactive/timer_rate
         echo 100000 > /sys/devices/system/cpu/cpufreq/interactive/sampling_down_factor
         echo 1 > /sys/devices/system/cpu/cpufreq/interactive/io_is_busy
         echo 20 > /sys/module/cpu_boost/parameters/boost_ms
         echo 1566000 > /sys/module/cpu_boost/parameters/sync_threshold
         echo 1134000 > /sys/module/cpu_boost/parameters/input_boost_freq
         echo 40 > /sys/module/cpu_boost/parameters/input_boost_ms
         echo 384000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
         echo 384000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq
         echo 384000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq
         echo 384000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/above_hispeed_delay
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/boost
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/boostpulse
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/boostpulse_duration
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/go_hispeed_load
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/hispeed_freq
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/io_is_busy
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/min_sample_time
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/target_loads
         chown -h system /sys/devices/system/cpu/cpufreq/interactive/timer_slack
         chown -h system /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
         chown -h system /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
         chown -h system /sys/devices/system/cpu/cpu1/cpufreq/scaling_max_freq
         chown -h system /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq
         chown -h system /sys/devices/system/cpu/cpu2/cpufreq/scaling_max_freq
         chown -h system /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq
         chown -h system /sys/devices/system/cpu/cpu3/cpufreq/scaling_max_freq
         chown -h system /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq
         echo 1 > /sys/module/msm_thermal/core_control/enabled
         echo 1 > /dev/cpuctl/apps/cpu.notify_on_migrate
         chown -h root.system /sys/devices/system/cpu/mfreq
         chmod -h 220 /sys/devices/system/cpu/mfreq
         chown -h root.system /sys/devices/system/cpu/cpu1/online
         chown -h root.system /sys/devices/system/cpu/cpu2/online
         chown -h root.system /sys/devices/system/cpu/cpu3/online
         chmod -h 664 /sys/devices/system/cpu/cpu1/online
         chmod -h 664 /sys/devices/system/cpu/cpu2/online
         chmod -h 664 /sys/devices/system/cpu/cpu3/online
         # set DCVS parameters for CPU
         echo 40000 > /sys/module/msm_dcvs/cores/cpu0/slack_time_max_us
         echo 40000 > /sys/module/msm_dcvs/cores/cpu0/slack_time_min_us
         echo 100000 > /sys/module/msm_dcvs/cores/cpu0/em_win_size_min_us
         echo 500000 > /sys/module/msm_dcvs/cores/cpu0/em_win_size_max_us
         echo 0 > /sys/module/msm_dcvs/cores/cpu0/slack_mode_dynamic
         echo 1000000 > /sys/module/msm_dcvs/cores/cpu0/disable_pc_threshold
         echo 25000 > /sys/module/msm_dcvs/cores/cpu1/slack_time_max_us
         echo 25000 > /sys/module/msm_dcvs/cores/cpu1/slack_time_min_us
         echo 100000 > /sys/module/msm_dcvs/cores/cpu1/em_win_size_min_us
         echo 500000 > /sys/module/msm_dcvs/cores/cpu1/em_win_size_max_us
         echo 0 > /sys/module/msm_dcvs/cores/cpu1/slack_mode_dynamic
         echo 1000000 > /sys/module/msm_dcvs/cores/cpu1/disable_pc_threshold
         echo 25000 > /sys/module/msm_dcvs/cores/cpu2/slack_time_max_us
         echo 25000 > /sys/module/msm_dcvs/cores/cpu2/slack_time_min_us
         echo 100000 > /sys/module/msm_dcvs/cores/cpu2/em_win_size_min_us
         echo 500000 > /sys/module/msm_dcvs/cores/cpu2/em_win_size_max_us
         echo 0 > /sys/module/msm_dcvs/cores/cpu2/slack_mode_dynamic
         echo 1000000 > /sys/module/msm_dcvs/cores/cpu2/disable_pc_threshold
         echo 25000 > /sys/module/msm_dcvs/cores/cpu3/slack_time_max_us
         echo 25000 > /sys/module/msm_dcvs/cores/cpu3/slack_time_min_us
         echo 100000 > /sys/module/msm_dcvs/cores/cpu3/em_win_size_min_us
         echo 500000 > /sys/module/msm_dcvs/cores/cpu3/em_win_size_max_us
         echo 0 > /sys/module/msm_dcvs/cores/cpu3/slack_mode_dynamic
         echo 1000000 > /sys/module/msm_dcvs/cores/cpu3/disable_pc_threshold
         # set DCVS parameters for GPU
         echo 20000 > /sys/module/msm_dcvs/cores/gpu0/slack_time_max_us
         echo 20000 > /sys/module/msm_dcvs/cores/gpu0/slack_time_min_us
         echo 0 > /sys/module/msm_dcvs/cores/gpu0/slack_mode_dynamic
         # set msm_mpdecision parameters
         echo 45000 > /sys/module/msm_mpdecision/slack_time_max_us
         echo 15000 > /sys/module/msm_mpdecision/slack_time_min_us
         echo 100000 > /sys/module/msm_mpdecision/em_win_size_min_us
         echo 1000000 > /sys/module/msm_mpdecision/em_win_size_max_us
         echo 3 > /sys/module/msm_mpdecision/online_util_pct_min
         echo 25 > /sys/module/msm_mpdecision/online_util_pct_max
         echo 97 > /sys/module/msm_mpdecision/em_max_util_pct
         echo 2 > /sys/module/msm_mpdecision/rq_avg_poll_ms
         echo 10 > /sys/module/msm_mpdecision/mp_em_rounding_point_min
         echo 85 > /sys/module/msm_mpdecision/mp_em_rounding_point_max
         echo 50 > /sys/module/msm_mpdecision/iowait_threshold_pct
         #set permissions for the nodes needed by display on/off hook
         chown -h system /sys/module/msm_dcvs/cores/cpu0/slack_time_max_us
         chown -h system /sys/module/msm_dcvs/cores/cpu0/slack_time_min_us
         chown -h system /sys/module/msm_mpdecision/slack_time_max_us
         chown -h system /sys/module/msm_mpdecision/slack_time_min_us
         chmod -h 664 /sys/module/msm_dcvs/cores/cpu0/slack_time_max_us
         chmod -h 664 /sys/module/msm_dcvs/cores/cpu0/slack_time_min_us
         chmod -h 664 /sys/module/msm_mpdecision/slack_time_max_us
         chmod -h 664 /sys/module/msm_mpdecision/slack_time_min_us
         echo 1 > /sys/module/rmnet_usb/parameters/rmnet_data_init
         # Allow QMUX daemon to assign port open wait time
         chown radio.radio /sys/devices/virtual/hsicctl/hsicctl0/modem_wait
         # Wake on volume
         echo 1 > /sys/keyboard/vol_wakeup
    ;;
esac
