// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2021 MediaTek Inc.
 */
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/io.h>

#include <lpm_trace_event/lpm_trace_event.h>
#include <mtk_spm_sysfs.h>
#include <spm_reg.h>


#define plat_mmio_read(offset)	__raw_readl(spm_base + offset)

void __iomem *spm_base;
struct timer_list spm_resource_req_timer;
u32 spm_resource_req_timer_is_enabled;
u32 spm_resource_req_timer_ms;

static void spm_resource_req_timer_fn(struct timer_list *data)
{
	u32 req_sta_0, req_sta_1, req_sta_2;
	u32 req_sta_3, req_sta_4, req_sta_5;
	u32 req_sta_mix;
	u32 src_req;
	u32 md = 0, conn = 0, scp = 0, adsp = 0;
	u32 ufs = 0, msdc = 0, disp = 0, apu = 0;
	u32 spm = 0;

	if (!spm_base)
		return;

	req_sta_0 = plat_mmio_read(SPM_REQ_STA_0);
	req_sta_1 = plat_mmio_read(SPM_REQ_STA_1);
	req_sta_2 = plat_mmio_read(SPM_REQ_STA_2);
	req_sta_3 = plat_mmio_read(SPM_REQ_STA_3);
	req_sta_4 = plat_mmio_read(SPM_REQ_STA_4);
	req_sta_5 = plat_mmio_read(SPM_REQ_STA_5);
	req_sta_mix = req_sta_0 | req_sta_1 | req_sta_2 | req_sta_3 | req_sta_4 | req_sta_5;

	if (req_sta_mix & (0x3 << 17))
		md = 1;
	if (req_sta_mix & (0x3 << 5))
		conn = 1;
	if (req_sta_mix & (0x1 << 8))
		disp = 1;

	if (req_sta_mix & (0x1 << 22))
		scp = 1;
	if (req_sta_mix & (0x1 << 2))
		adsp = 1;
	if (req_sta_mix & (0x1 << 27))
		ufs = 1;
	if (req_sta_mix & (0x1 << 21))
		msdc = 1;

	if (req_sta_mix & (0x1 << 1))
		apu = 1;

	src_req = plat_mmio_read(SPM_SRC_REQ);
	if (src_req & 0x18F6)
		spm = 1;

	trace_SPM__resource_req_0(
		md, conn,
		scp, adsp,
		ufs, msdc,
		disp, apu,
		spm);

	spm_resource_req_timer.expires = jiffies +
		msecs_to_jiffies(spm_resource_req_timer_ms);
	add_timer(&spm_resource_req_timer);
}

static void spm_resource_req_timer_en(u32 enable, u32 timer_ms)
{
	if (enable) {
		if (spm_resource_req_timer_is_enabled)
			return;

		timer_setup(&spm_resource_req_timer,
			spm_resource_req_timer_fn, 0);

		spm_resource_req_timer_ms = timer_ms;
		spm_resource_req_timer.expires = jiffies +
			msecs_to_jiffies(spm_resource_req_timer_ms);
		add_timer(&spm_resource_req_timer);

		spm_resource_req_timer_is_enabled = true;
	} else if (spm_resource_req_timer_is_enabled) {
		del_timer(&spm_resource_req_timer);
		spm_resource_req_timer_is_enabled = false;
	}
}

ssize_t get_spm_resource_req_timer_enable(char *ToUserBuf
		, size_t sz, void *priv)
{
	int bLen = snprintf(ToUserBuf, sz
				, "spm resource request timer is enabled: %d\n",
				spm_resource_req_timer_is_enabled);
	return (bLen > sz) ? sz : bLen;
}

ssize_t set_spm_resource_req_timer_enable(char *ToUserBuf
		, size_t sz, void *priv)
{
	u32 is_enable;
	u32 timer_ms;

	if (!ToUserBuf)
		return -EINVAL;

	if (sscanf(ToUserBuf, "%d %d", &is_enable, &timer_ms) == 2) {
		spm_resource_req_timer_en(is_enable, timer_ms);
		return sz;
	}

	if (kstrtouint(ToUserBuf, 10, &is_enable) == 0) {
		if (is_enable == 0) {
			spm_resource_req_timer_en(is_enable, 0);
			return sz;
		}
	}

	return -EINVAL;
}

static const struct mtk_lp_sysfs_op spm_resource_req_timer_enable_fops = {
	.fs_read = get_spm_resource_req_timer_enable,
	.fs_write = set_spm_resource_req_timer_enable,
};

int __init lpm_trace_init(void)
{
	struct device_node *node = NULL;

	node = of_find_compatible_node(NULL, NULL, "mediatek,sleep");

	if (node) {
		spm_base = of_iomap(node, 0);
		of_node_put(node);
	}

	mtk_spm_sysfs_root_entry_create();
	mtk_spm_sysfs_entry_node_add("spm_dump_res_req_enable", 0444
			, &spm_resource_req_timer_enable_fops, NULL);

	return 0;
}

void __exit lpm_trace_deinit(void)
{
}

