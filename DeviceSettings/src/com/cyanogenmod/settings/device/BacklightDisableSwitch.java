package com.cyanogenmod.settings.device;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceManager;
import java.io.File;

public class BacklightDisableSwitch implements OnPreferenceChangeListener {

    private static final String FILE = "/sys/class/leds/button-backlight/brightness";

    public static boolean isSupported() {
        return Utils.fileExists(FILE);
    }

    public static String getValue(Context context) {
        String value = Utils.getFileValue(FILE, "255");
        SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(context);
        return sharedPrefs.getString(TouchscreenFragmentActivity.KEY_BACKLIGHTDISABLE_SWITCH, value);
    }

    /**
     * Restore BacklightDisable setting from SharedPreferences. (Write to kernel.)
     * @param context       The context to read the SharedPreferences from
     */
    public static void restore(Context context) {
        if (!isSupported()) {
            return;
        }

        File blFile = new File(FILE);
        blFile.setWritable(true);

        String value = getValue(context);
        int ref = Integer.parseInt(value);
        if(ref == 0) {
            Utils.writeValue(FILE, "0\n");
            blFile.setWritable(false);
        }
        else {
            blFile.setWritable(true);
            Utils.writeValue(FILE, "255\n");
        }
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        Boolean enabled = (Boolean) newValue;
        File blFile = new File(FILE);
        if(enabled) {
            Utils.writeValue(FILE, "0\n");
            blFile.setWritable(false);
        }
        else {
            blFile.setWritable(true);
            Utils.writeValue(FILE, "255\n");
        }
        return true;
    }
}
