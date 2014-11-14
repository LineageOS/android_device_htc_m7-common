# Camera
PRODUCT_PROPERTY_OVERRIDES += \
    ro.cam.hw.version=m7

# Display
PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=196608 \
    ro.sf.lcd_density=480

# Input
PRODUCT_PROPERTY_OVERRIDES += \
    ro.input.noresample=1

# NFC
PRODUCT_PROPERTY_OVERRIDES += \
    debug.nfc.fw_download=true \
    debug.nfc.fw_boot_download=false \
    debug.nfc.se=true \
    ro.nfc.port=I2C

# Performance
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.extension_library=/system/vendor/lib/libqc-opt.so

# Radio
PRODUCT_PROPERTY_OVERRIDES += \
    ro.baseband.arch=mdm

# Recovery
PRODUCT_PROPERTY_OVERRIDES += \
    ro.cwm.forbid_format="/firmware/mdm,/firmware/q6" \
    ro.cwm.forbid_mount="/firmware/mdm,/firmware/q6"

# Time services
PRODUCT_PROPERTY_OVERRIDES += \
    persist.timed.enable=true
