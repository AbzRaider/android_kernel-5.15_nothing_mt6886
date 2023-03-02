#
# Copyright (C) 2015 MediaTek Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#

ifneq ($(CONFIG_MTK_GPU_VERSION),"")
MTK_GPU_VERSION = $(subst ",,$(CONFIG_MTK_GPU_VERSION))
endif

export CONFIG_MALI_MEMORY_GROUP_MANAGER = y
export CONFIG_MALI_PROTECTED_MEMORY_ALLOCATOR = y

ifneq ($(word 3,$(MTK_GPU_VERSION)),)
    obj-y += mali-$(word 3,$(MTK_GPU_VERSION))/drivers/gpu/arm/midgard/
    obj-y += mali-$(word 3,$(MTK_GPU_VERSION))/drivers/base/arm/
    obj-y += mali-$(word 3,$(MTK_GPU_VERSION))/drivers/base/arm/memory_group_manager/
    obj-y += mali-$(word 3,$(MTK_GPU_VERSION))/drivers/base/arm/protected_memory_allocator/
else
    obj-y += mali-EAC/drivers/
endif
