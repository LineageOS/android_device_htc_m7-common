/*
 * Copyright (C) 2016 The CyanogenMod Project
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

package com.android.internal.telephony;

import static com.android.internal.telephony.RILConstants.*;

import android.content.Context;
import android.os.AsyncResult;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.os.Parcel;
import android.os.SystemProperties;
import android.text.TextUtils;
import android.telephony.CellInfo;
import android.telephony.Rlog;

import com.android.internal.telephony.uicc.IccCardApplicationStatus;
import com.android.internal.telephony.uicc.IccCardStatus;
import com.android.internal.telephony.uicc.IccUtils;
import com.android.internal.telephony.dataconnection.DcFailCause;
import com.android.internal.telephony.dataconnection.DataCallResponse;

import java.util.ArrayList;

public class HTCQualcommRIL extends RIL implements CommandsInterface {

    private static final int RIL_UNSOL_ENTER_LPM = 1523;
    private static final int RIL_UNSOL_CDMA_3G_INDICATOR = 3009;
    private static final int RIL_UNSOL_CDMA_ENHANCE_ROAMING_INDICATOR = 3012;
    private static final int RIL_UNSOL_CDMA_NETWORK_BASE_PLUSCODE_DIAL = 3020;
    private static final int RIL_UNSOL_TPMR_ID = 3024;
    private static final int RIL_UNSOL_SECTOR_ID_IND = 3057;
    private static final int RIL_UNSOL_RESPONSE_PHONE_MODE_CHANGE = 6002;
    private static final int RIL_UNSOL_RESPONSE_VOICE_RADIO_TECH_CHANGED = 21004;
    private static final int RIL_UNSOL_RESPONSE_DATA_NETWORK_STATE_CHANGED = 21007;

    public HTCQualcommRIL(Context context, int networkMode, int cdmaSubscription) {
        super(context, networkMode, cdmaSubscription, null);
    }

    public HTCQualcommRIL(Context context, int networkMode, int cdmaSubscription,
            Integer instanceId) {
        super(context, networkMode, cdmaSubscription, instanceId);
    }

    @Override
    protected Object
    responseIccCardStatus(Parcel p) {
        final int CARDSTATE_ABSENT = 0;
        final int CARDSTATE_ERROR = 2;
        int dataPosition = p.dataPosition();
        int cardState = p.readInt();

        if (cardState > CARDSTATE_ERROR) {
            p.setDataPosition(dataPosition);
            p.writeInt(CARDSTATE_ABSENT);
        }

        p.setDataPosition(dataPosition);

        return super.responseIccCardStatus(p);
    }

    private static String
    responseToStringHTC(int request) {
        switch(request) {
            case RIL_UNSOL_ENTER_LPM: return "UNSOL_ENTER_LPM";
            case RIL_UNSOL_CDMA_3G_INDICATOR: return "UNSOL_CDMA_3G_INDICATOR";
            case RIL_UNSOL_CDMA_ENHANCE_ROAMING_INDICATOR: return "UNSOL_CDMA_ENHANCE_ROAMING_INDICATOR";
            case RIL_UNSOL_CDMA_NETWORK_BASE_PLUSCODE_DIAL: return "UNSOL_CDMA_NETWORK_BASE_PLUSCODE_DIAL";
            case RIL_UNSOL_TPMR_ID: return "UNSOL_TPMR_ID";
            case RIL_UNSOL_SECTOR_ID_IND: return "UNSOL_SECTOR_ID_IND";
            case RIL_UNSOL_RESPONSE_PHONE_MODE_CHANGE: return "UNSOL_RESPONSE_PHONE_MODE_CHANGE";
            case RIL_UNSOL_RESPONSE_VOICE_RADIO_TECH_CHANGED: return "UNSOL_RESPONSE_VOICE_RADIO_TECH_CHANGED";
            case RIL_UNSOL_RESPONSE_DATA_NETWORK_STATE_CHANGED: return "UNSOL_RESPONSE_DATA_NETWORK_STATE_CHANGED";
            default: return "<unknown response>";
        }
    }

    @Override
    protected void
    processUnsolicited (Parcel p, int type) {
        Object ret;
        int dataPosition = p.dataPosition(); // save off position within the Parcel
        int response = p.readInt();

        switch(response) {
            case RIL_UNSOL_ENTER_LPM: ret = responseVoid(p); break;
            case RIL_UNSOL_CDMA_3G_INDICATOR:  ret = responseInts(p); break;
            case RIL_UNSOL_CDMA_ENHANCE_ROAMING_INDICATOR:  ret = responseInts(p); break;
            case RIL_UNSOL_CDMA_NETWORK_BASE_PLUSCODE_DIAL:  ret = responseStrings(p); break;
            case RIL_UNSOL_TPMR_ID: ret = responseInts(p); break;
            case RIL_UNSOL_SECTOR_ID_IND: ret = responseString(p); break;
            case RIL_UNSOL_RESPONSE_PHONE_MODE_CHANGE:  ret = responseInts(p); break;
            case RIL_UNSOL_RESPONSE_VOICE_RADIO_TECH_CHANGED: ret = responseVoid(p); break;
            case RIL_UNSOL_RESPONSE_DATA_NETWORK_STATE_CHANGED: ret = responseVoid(p); break;

            default:
                // Rewind the Parcel
                p.setDataPosition(dataPosition);

                // Forward responses that we are not overriding to the super class
                super.processUnsolicited(p, type);
                return;
        }

        switch(response) {
            case RIL_UNSOL_ENTER_LPM:
            case RIL_UNSOL_CDMA_3G_INDICATOR:
            case RIL_UNSOL_CDMA_ENHANCE_ROAMING_INDICATOR:
            case RIL_UNSOL_CDMA_NETWORK_BASE_PLUSCODE_DIAL:
            case RIL_UNSOL_TPMR_ID:
            case RIL_UNSOL_SECTOR_ID_IND:
            case RIL_UNSOL_RESPONSE_PHONE_MODE_CHANGE:
            case RIL_UNSOL_RESPONSE_VOICE_RADIO_TECH_CHANGED:
            case RIL_UNSOL_RESPONSE_DATA_NETWORK_STATE_CHANGED:
                if (RILJ_LOGD) {
                    riljLog("[UNSL]< " + responseToStringHTC(response) + " "
                            + retToString(response, ret));
                }
                break;
        }
    }

    @Override
    public void getRadioCapability(Message response) {
        if (response == null)
            return;

        if (RILJ_LOGD) riljLog("HTCQualcommRIL: returning static radio capability");
        Object ret = makeStaticRadioCapability();
        AsyncResult.forMessage(response, ret, null);
        response.sendToTarget();
    }

    @Override
    protected void
    send(RILRequest rr) {
        switch (rr.mRequest) {
            case RIL_REQUEST_GET_CELL_INFO_LIST:
            case RIL_REQUEST_SET_INITIAL_ATTACH_APN:
            case RIL_REQUEST_SET_DATA_PROFILE:
                if (RILJ_LOGD) {
                    riljLog("HTCQualcommRIL: received unsupported request "
                            + rr.mRequest);
                }
                rr.onError(REQUEST_NOT_SUPPORTED, null);
                rr.release();
                break;
            default:
                super.send(rr);
                break;
        }
    }
}
