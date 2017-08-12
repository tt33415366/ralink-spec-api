obj-m := ralink-spec-api.o
ifeq ($(CONFIG_RA_HW_NAT_API),y)
EXTRA_CFLAGS += -DRA_HW_NAT_API
endif
ifeq ($(CONFIG_RA_HW_NAT_WIFI_NEW_ARCH_API),y)
EXTRA_CFLAGS += -DRA_HW_NAT_WIFI_NEW_ARCH_API
endif
ifeq ($(CONFIG_RA_MTD_RW_BY_NUM_API),y)
EXTRA_CFLAGS += -DRA_MTD_RW_BY_NUM_API
endif

ralink-spec-api-y := ra-spec-api.o