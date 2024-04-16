extra_symbols := $(abspath $(O)/../drivers/misc/mediatek/connectivity/conninfra/Module.symvers)
extra_symbols += $(abspath $(O)/../drivers/misc/mediatek/connectivity/common/Module.symvers)

$(info [wlan] WLAN_BUILD_COMMON=$(WLAN_BUILD_COMMON))

ifndef TOP
    TOP := $(srctree)/..
endif

MODULE_NAME := wmt_chrdev_wifi

# Force build fail on modpost warning
KBUILD_MODPOST_FAIL_ON_WARNINGS := y

ccflags-y += \
    -I$(srctree)/drivers/misc/mediatek/include/mt-plat \
    -I$(srctree)/drivers/misc/mediatek/connectivity/common/common_main/include \
    -I$(srctree)/drivers/misc/mediatek/connectivity/common/common_main/linux/include

ifeq ($(CONNAC_VER), 2_0)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/conninfra/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/conninfra/debug_utility
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/conninfra/debug_utility/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/conninfra/debug_utility/connsyslog
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/conninfra/debug_utility/coredump
endif

ifneq ($(CONFIG_MTK_CONNSYS_DEDICATED_LOG_PATH),)
ccflags-y += -DCONFIG_MTK_CONNSYS_DEDICATED_LOG_PATH
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/common/debug_utility
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/wlan/adaptor/include
endif

ifeq ($(CONFIG_MTK_CONN_LTE_IDC_SUPPORT),y)
    ccflags-y += -DWMT_IDC_SUPPORT=1
else

ifneq ($(wildcard $(abspath $(O)/drivers/misc/mediatek/connectivity/common/Module.symvers)) ,)
	$(MAKE) -C $(KERNEL_SRC) M=$(M)/build/connac1x modules $(KBUILD_OPTIONS) KBUILD_EXTRA_SYMBOLS="$(extra_symbols)" CONNAC_VER=1_0 MODULE_NAME=wmt_chrdev_wifi
endif

ifneq ($(wildcard $(abspath $(O)/drivers/misc/mediatek/connectivity/conninfra/Module.symvers)) ,)
	$(MAKE) -C $(KERNEL_SRC) M=$(M)/build/connac2x modules $(KBUILD_OPTIONS) KBUILD_EXTRA_SYMBOLS="$(extra_symbols)" CONNAC_VER=2_0 MODULE_NAME=wmt_chrdev_wifi_connac2
	$(MAKE) -C $(KERNEL_SRC) M=$(M)/build/connac3x modules $(KBUILD_OPTIONS) KBUILD_EXTRA_SYMBOLS="$(extra_symbols)" CONNAC_VER=3_0 MODULE_NAME=wmt_chrdev_wifi_connac3
endif

endif

modules_install:
ifdef WLAN_BUILD_COMMON
	$(MAKE) M=$(M) -C $(KERNEL_SRC) modules_install
else

ifneq ($(wildcard $(abspath $(O)/drivers/misc/mediatek/connectivity/common/Module.symvers)) ,)
	$(MAKE) M=$(M)/build/connac1x -C $(KERNEL_SRC) modules_install
endif

ifneq ($(wildcard $(abspath $(O)/drivers/misc/mediatek/connectivity/conninfra/Module.symvers)) ,)
	$(MAKE) M=$(M)/build/connac2x -C $(KERNEL_SRC) modules_install
	$(MAKE) M=$(M)/build/connac3x -C $(KERNEL_SRC) modules_install
endif

endif

clean:
ifdef WLAN_BUILD_COMMON
	$(MAKE) -C $(KERNEL_SRC) M=$(M) clean
else

ifneq ($(wildcard $(abspath $(O)/drivers/misc/mediatek/connectivity/common/Module.symvers)) ,)
	$(MAKE) -C $(KERNEL_SRC) M=$(M)/build/connac1x clean
endif

ifneq ($(wildcard $(abspath $(O)/drivers/misc/mediatek/connectivity/conninfra/Module.symvers)) ,)
	$(MAKE) -C $(KERNEL_SRC) M=$(M)/build/connac2x clean
	$(MAKE) -C $(KERNEL_SRC) M=$(M)/build/connac3x clean
endif

endif
