#!/system/bin/sh
# Copyright (c) 2009-2012, Code Aurora Forum. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Code Aurora nor
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
	echo 1 > /dev/cpuctl/apps/cpu.notify_on_migrate 1
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
	#echo 960000 > /sys/devices/system/cpu/cpufreq/ondemand/sync_freq
	#echo 80 > /sys/devices/system/cpu/cpufreq/ondemand/up_threshold_any_cpu_load
	#chown system /sys/devices/system/cpu/cpufreq/ondemand/sampling_rate
	#chown system /sys/devices/system/cpu/cpufreq/ondemand/sampling_down_factor
	#chown system /sys/devices/system/cpu/cpufreq/ondemand/io_is_busy
	echo "interactive" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
	echo "interactive" > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
	echo "interactive" > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
	echo "interactive" > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
	echo 1 > /sys/devices/system/cpu/cpufreq/interactive/io_is_busy
	echo 384000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
	echo 384000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq
	echo 384000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq
	echo 384000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq
	chown system /sys/devices/system/cpu/cpufreq/interactive/above_hispeed_delay
	chown system /sys/devices/system/cpu/cpufreq/interactive/boost
	chown system /sys/devices/system/cpu/cpufreq/interactive/boostpulse
	chown system /sys/devices/system/cpu/cpufreq/interactive/boostpulse_duration
	chown system /sys/devices/system/cpu/cpufreq/interactive/go_hispeed_load
	chown system /sys/devices/system/cpu/cpufreq/interactive/hispeed_freq
	chown system /sys/devices/system/cpu/cpufreq/interactive/io_is_busy
	chown system /sys/devices/system/cpu/cpufreq/interactive/min_sample_time
	chown system /sys/devices/system/cpu/cpufreq/interactive/target_loads
	chown system /sys/devices/system/cpu/cpufreq/interactive/timer_rate
	chown system /sys/devices/system/cpu/cpufreq/interactive/timer_slack
	chown system /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
	chown system /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
	chown system /sys/devices/system/cpu/cpu1/cpufreq/scaling_max_freq
	chown system /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq
	chown system /sys/devices/system/cpu/cpu2/cpufreq/scaling_max_freq
	chown system /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq
	chown system /sys/devices/system/cpu/cpu3/cpufreq/scaling_max_freq
	chown system /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq
	echo 1 > /sys/module/msm_thermal/core_control/enabled
	chown root.system /sys/devices/system/cpu/mfreq
	chown root.system /sys/devices/system/cpu/cpu1/online
	chown root.system /sys/devices/system/cpu/cpu2/online
	chown root.system /sys/devices/system/cpu/cpu3/online
	chmod 664 /sys/devices/system/cpu/cpu1/online
	chmod 664 /sys/devices/system/cpu/cpu2/online
	chmod 664 /sys/devices/system/cpu/cpu3/online
	# set DCVS parameters for CPU

	chmod 664 /sys/power/pnpmgr/apps/media_mode
	chown media.system /sys/power/pnpmgr/apps/media_mode
	chown system /sys/power/pnpmgr/apps/activity_trigger
	chown system /sys/power/perflock
	chown system /sys/power/launch_event
	chown system /sys/power/powersave
	chown system /sys/power/cpufreq_ceiling
	chown system /sys/power/cpunum_floor
	chown system /sys/power/cpunum_ceiling
	echo 1 > /sys/module/rmnet_usb/parameters/rmnet_data_init
	# Allow QMUX daemon to assign port open wait time
	chown radio.radio /sys/devices/virtual/hsicctl/hsicctl0/modem_wait
    ;;
esac

# Post-setup services
case "$target" in
    "msm8960")
	start adaptive
    ;;
esac
