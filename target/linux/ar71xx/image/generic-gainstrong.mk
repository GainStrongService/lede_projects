define Device/oolite-v9
  $(Device/tplink-16mlzma)
  DEVICE_TITLE := GainStrong Oolite V9.000
  DEVICE_PACKAGES := kmod-usb-core kmod-usb2 kmod-usb-storage
  BOARDNAME := OOLITE-V9
  DEVICE_PROFILE := OOLITEV9
  TPLINK_HWID := 0x3C00010E
  CONSOLE := ttyS0, 115200
endef
TARGET_DEVICES += oolite-v9
