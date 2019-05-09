DEVICE_VARS += TPLINK_HWID TPLINK_HWREV TPLINK_FLASHLAYOUT TPLINK_HEADER_VERSION TPLINK_BOARD_NAME

define Device/oolitebox
    $(Device/tplink-32mlzma)
    DEVICE_TITLE := GainStrong OoliteBox
    DEVICE_PACKAGES := kmod-usb-ohci kmod-usb-uhci kmod-usb-storage kmod-usb2 \
        kmod-nls-cp437 kmod-nls-iso8859-1 kmod-nls-utf8 kmod-fs-ext4 \
        kmod-fs-ntfs kmod-fs-vfat kmod-block2mtd badblocks usbutils \
        block-mount luci kmod-ath10k ath10k-firmware-qca9887 \
        ath10k-firmware-qca9888 ath10k-firmware-qca988x kmod-4G-switch \
	kmod-usb-serial kmod-usb-serial-option comgt luci-proto-3g
    BOARDNAME := OOLITEBOX board
    DEVICE_PROFILE := OOLITEBOX
    TPLINK_HWID := 0x3C000107
    CONSOLE := ttyS0,115200
endef
TARGET_DEVICES += oolitebox

