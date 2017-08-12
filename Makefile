#
# Copyright (C) 2006-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ralink-spec-api
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/$(PKG_NAME)
  CATEGORY:=Ralink
  TITLE:=Ralink Specific API driver
  #DEPENDS:= @TARGET_ramips
  FILES:=$(PKG_BUILD_DIR)/ralink-spec-api.ko
  AUTOLOAD:=$(call AutoLoad,30,ralink-spec-api)
endef

define KernelPackage/$(PKG_NAME)/description
  This package aims to provide ralink specific API for OpenWRT/LEDE.
  With this package, the MTK Propriertary WLAN driver can be used with the
  orignal OpenWRT/LEDE
endef

define KernelPackage/$(PKG_NAME)/config
	source "$(SOURCE)/Config.in"
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		modules
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
