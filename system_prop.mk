# Art
PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.dex2oat-swap=false

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
    audio.offload.disable=1 \
    persist.audio.fluence.speaker=true \
    persist.audio.fluence.voicecall=true \
    persist.audio.fluence.voicerec=false \
    ro.qc.sdk.audio.fluencetype=fluence \
    use.dedicated.device.for.voip=true \
    use.voice.path.for.pcm.voip=true

# Bluetooth
PRODUCT_PROPERTY_OVERRIDES += \
    ro.bt.bdaddr_path=/sys/module/htc_bdaddress/parameters/bdaddress

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
    ro.baseband.arch=mdm \
    ro.telephony.ril_class=HTCQualcommRIL

# Scheduler
PRODUCT_PROPERTY_OVERRIDES += \
    sys.io.scheduler=bfq

# Time services
PRODUCT_PROPERTY_OVERRIDES += \
    persist.timed.enable=true
