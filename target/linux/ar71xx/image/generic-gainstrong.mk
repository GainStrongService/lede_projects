DEVICE_VARS += TPLINK_HWID TPLINK_HWREV TPLINK_FLASHLAYOUT TPLINK_HEADER_VERSION TPLINK_BOARD_NAME

define Device/CeilingAP-V1
    $(Device/tplink-16mlzma)
    DEVICE_TITLE := GainStrong CeilingAP V1.0
    DEVICE_PACKAGES := kmod-usb-ohci kmod-usb-uhci kmod-usb-storage kmod-usb2 \
	kmod-nls-cp437 kmod-nls-iso8859-1 kmod-nls-utf8 kmod-fs-ext4 \
	kmod-fs-ntfs kmod-fs-vfat kmod-block2mtd badblocks usbutils \
	block-mount luci kmod-ath10k ath10k-firmware-qca9887 \
	ath10k-firmware-qca9888 ath10k-firmware-qca988x
    BOARDNAME := CEILINGAP_V1
    DEVICE_PROFILE := CEILINGAP_V1
    TPLINK_HWID := 0x3C00010C
    CONSOLE := ttyS0,115200
endef
TARGET_DEVICES += CeilingAP-V1
