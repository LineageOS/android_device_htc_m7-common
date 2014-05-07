/*
   Copyright (c) 2013, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

void cdma_properties(char cdma_subscription[],
                     char default_network[],
                     char operator_numeric[],
                     char operator_alpha[]);

void vendor_load_properties()
{
    char platform[PROP_VALUE_MAX];
    char bootmid[PROP_VALUE_MAX];
    char device[PROP_VALUE_MAX];
    char devicename[PROP_VALUE_MAX];
    int rc;

    rc = property_get("ro.board.platform", platform);
    if (!rc || strncmp(platform, ANDROID_TARGET, PROP_VALUE_MAX))
        return;

    property_get("ro.boot.mid", bootmid);

    if (strstr(bootmid, "PN0731000")) {
        /* m7vzw (m7wlv) */
        cdma_properties("0", "10", "311480", "Verizon");
        property_set("ro.build.fingerprint", "VERIZON/HTCOneVZW/m7wlv:4.4.2/KOT49H/304035.1:user/release-keys");
        property_set("ro.build.description", "3.11.605.1 CL304035 release-keys");
        property_set("ro.product.device", "m7vzw");
        property_set("ro.build.product", "m7vzw");
        property_set("ro.cdma.subscribe_on_ruim_ready", "true");
        property_set("persist.radio.snapshot_timer", "22");
        property_set("ro.ril.wp.feature", "1");
        property_set("ro.use_data_netmgrd", "true");
    } else if (strstr(bootmid, "PN0720000")) {
        /* m7spr (m7whl) */
        cdma_properties("1", "10", "310120", "Sprint");
        property_set("ro.build.fingerprint", "htc/sprint_wwe/m7wls:4.4.2/KOT49H/303687.4:user/release-keys");
        property_set("ro.build.description", "4.06.651.4 CL303687 release-keys");
        property_set("ro.product.device", "m7spr");
        property_set("ro.build.product", "m7spr");
        property_set("telephony.sms.pseudo_multipart", "1");
        property_set("persist.radio.use_nv_for_ehrpd", "true");
        property_set("persist.radio.mode_pref_nv10", "1");
        property_set("persist.radio.always_send_plmn", "1");
        property_set("persist.radio.snapshot_timer", "1");
        property_set("persist.radio.apm_sim_not_pwdn", "1");
    } else {
        /* m7 */
        property_set("ro.build.fingerprint", "htc/m7_google/m7:4.4.2/KOT49H.H1/288144:user/release-keys");
        property_set("ro.build.description", "3.62.1700.1 CL288144 release-keys");
        property_set("ro.product.device", "m7");
        property_set("ro.telephony.default_network", "9");
        property_set("telephony.lteOnGsmDevice", "1");
    }

    property_set("ro.product.model", "One");
    property_get("ro.product.device", device);
    ERROR("Found bootmid %s setting build properties for %s device\n", bootmid, device);
}

void cdma_properties(char default_cdma_sub[], char default_network[],
                     char operator_numeric[], char operator_alpha[])
{
    property_set("ro.telephony.default_cdma_sub", default_cdma_sub); //0: RUIM/SIM  1: NV
    property_set("ro.telephony.default_network", default_network);
    property_set("ro.cdma.home.operator.numeric", operator_numeric);
    property_set("gsm.sim.operator.numeric", operator_numeric);
    property_set("gsm.operator.numeric", operator_numeric);
    property_set("ro.cdma.home.operator.alpha", operator_alpha);
    property_set("gsm.sim.operator.alpha", operator_alpha);
    property_set("gsm.operator.alpha", operator_numeric);
    property_set("telephony.lteOnCdmaDevice", "1");
    property_set("persist.radio.snapshot_enabled", "1");
    property_set("gsm.sim.operator.iso-country", "us");
    property_set("gsm.operator.iso-country", "us");
}
