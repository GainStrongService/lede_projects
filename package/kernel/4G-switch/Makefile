#
# Copyright (C) 2016 GainStrong Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=4g-switch
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/4G-switch
  SUBMENU:=Other modules
  TITLE:=4G-switch/4g-switch driver support
  DEPENDS:=+kmod-usb-net
  FILES:=$(PKG_BUILD_DIR)/GobiNet.ko
  AUTOLOAD:=$(call AutoLoad,69,4g-switch)
  KCONFIG:=
endef

define KernelPackage/4G-switch/description
 Kernel module for 4G-switch/4g-switch.
endef

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -R ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" $(MAKE_OPTS) modules
endef

$(eval $(call KernelPackage,4G-switch))
