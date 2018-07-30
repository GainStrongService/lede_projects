define Device/cpe9344
  $(Device/tplink-16mlzma)
  DEVICE_TITLE := GainStrong CPE9344
  DEVICE_PACKAGES := kmod-usb-core kmod-usb2 kmod-usb-storage
  BOARDNAME := CPE9344
  DEVICE_PROFILE := CPE9344
  TPLINK_HWID := 0x3C000109
  CONSOLE := ttyUSBS0, 115200
endef
TARGET_DEVICES += cpe9344
