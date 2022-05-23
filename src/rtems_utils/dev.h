/*
 * dev.h
 *
 *  Created on: 18 mai 2022
 *      Author: pa267340
 */

#ifndef DEV_H_
#define DEV_H_

static struct grspw_device devs[DEVS_MAX];

/* Interrupt handler for TimeCode reception on RXDEV */
void app_tc_isr(void *data, int tc)
{
	struct grspw_device *dev = data;

	printk("GRSPW%d: TC-ISR received 0x%02x\n", dev->index, tc);
}


struct grspw_config dev_def_cfg =
 {
 		.adrcfg =
 		{
 			.promiscuous = 1, /* Detect all packets */
 			.def_addr = 32, /* updated bu dev_init() */
 			.def_mask = 0,
 			.dma_nacfg =
 			{
 				/* Since only one DMA Channel is used, only
 				 * the default Address|Mask is used.
 				 */
 				{
 					.node_en = 0,
 					.node_addr = 0,
 					.node_mask = 0,
 				},
 				{
 					.node_en = 0,
 					.node_addr = 0,
 					.node_mask = 0,
 				},
 				{
 					.node_en = 0,
 					.node_addr = 0,
 					.node_mask = 0,
 				},
 				{
 					.node_en = 0,
 					.node_addr = 0,
 					.node_mask = 0,
 				},
 			},
 		},
 		.rmap_cfg = 0,		/* Disable RMAP */
 		.rmap_dstkey = 0,	/* No RMAP DESTKEY needed when disabled */
 		.tc_cfg = TCOPTS_EN_TX|TCOPTS_EN_RX,/* Enable TimeCode */
 		.tc_isr_callback = app_tc_isr,/* TimeCode ISR */
 		.tc_isr_arg = NULL,	/* No TimeCode ISR Argument */
 		.enable_chan_mask = 1,	/* Enable only the first DMA Channel */
 		.chan =
 		{
 			{
 				.flags = DMAFLAG_NO_SPILL,
 				.rxmaxlen = PKT_SIZE+4,
 				.rx_irq_en_cnt = 0, /* Disable RX IRQ generation */
 				.tx_irq_en_cnt = 0, /* Disable TX IRQ generation */
 			},
 			/* The other 3 DMA Channels are unused */

 		},
 };



int dev_init(int idx)
{
	struct grspw_device *dev = &devs[idx];
	int i, ctrl, clkdiv, tc;

	printf(" Initializing SpaceWire device %d\n", idx);

	memset(dev, 0, sizeof(struct grspw_device));

	dev->index = idx;
	dev->dh = grspw_open(idx);
	if (dev->dh == NULL) {
		printf("Failed to open GRSPW device %d\n", idx);
		return -1;
	}
	grspw_hw_support(dev->dh, &dev->hwsup);
#ifdef PRINT_GRSPW_RESET_CFG
	grspw_config_read(DEV(dev), &dev->cfg);
	printf("\n\n---- DEFAULT CONFIGURATION FROM DRIVER/HARDWARE ----\n");
	grspw_cfg_print(&dev->hwsup, &dev->cfg);
#endif
	dev->cfg = dev_def_cfg;
	dev->cfg.adrcfg.def_addr = 32 + idx;
	dev->cfg.tc_isr_arg = dev;
	tc = TCOPTS_EN_TX | TCOPTS_EN_RX | TCOPTS_EN_RXIRQ;
	grspw_tc_ctrl(dev->dh, &tc);

	if (grspw_cfg_set(DEV(dev), &dev->cfg)) {
		grspw_close(dev->dh);
		return -1;
	}
#ifdef PRINT_GRSPW_RESET_CFG
	printf("\n\n---- APPLICATION CONFIGURATION ----\n");
	grspw_cfg_print(&dev->hwsup, &dev->cfg);
	printf("\n\n");
#endif

	/* This will result in an error if only one port available */
	if (dev->hwsup.nports < 2) {
		int port = 1;
		if ( grspw_port_ctrl(dev->dh, &port) == 0 ) {
			printf("Succeeded to select port1, however only one PORT on dev %d!\n", dev->index);
			return -1;
		}
	}

	/* Try to bring link up at fastest clockdiv but do not touch
	 * start-up clockdivisor */
	clkdiv = -1;
	grspw_link_ctrl(dev->dh, NULL, NULL, &clkdiv);
	ctrl = LINKOPTS_ENABLE | LINKOPTS_AUTOSTART | LINKOPTS_START;
	clkdiv &= 0xff00;
	grspw_link_ctrl(dev->dh, &ctrl, NULL, &clkdiv);

	if ( (dev->hwsup.hw_version >> 16) != GAISLER_SPW2_DMA ){
		printf(" NOTE: running on SPW-ROUTER DMA SpaceWire link (no link-state available)\n");
	}
	else
		printf(" After Link Start: %d\n", (int)grspw_link_state(dev->dh));
	dev->run = 0;

	grspw_stats_clr(dev->dh);

	for (i=0; i<dev->hwsup.ndma_chans; i++) {
		if (dev->dma[i])
			grspw_dma_stats_clr(dev->dma[i]);
	}

	grspw_list_clr(&dev->rx_list);
	grspw_list_clr(&dev->tx_list);
	grspw_list_clr(&dev->tx_buf_list);
	dev->rx_list_cnt = dev->tx_list_cnt = dev->tx_buf_list_cnt = 0;

	return 0;
}



int dev_dma_close_all(int idx)
{
	struct grspw_device *dev = &devs[idx];
	int i, rc;
	for (i=0; i<dev->hwsup.ndma_chans; i++) {
		if (dev->dma[i]) {
			rc = grspw_dma_close(dev->dma[i]);
			if (rc)
				return rc;
			dev->dma[i] = NULL;
		}
	}
	return 0;
}

void dev_cleanup(int idx)
{
	struct grspw_device *dev = &devs[idx];

	if (dev->dh == NULL)
		return;

	/* Stop all DMA activity first */
	grspw_stop(DEV(dev));

	/* wait for other tasks to be thrown out from driver */
	rtems_task_wake_after(4);

	/* close all DMA channels */
	if (dev_dma_close_all(idx)) {
		printf("FAILED to close GRSPW%d DMA\n", idx);
	}

	if (grspw_close(dev->dh)) {
		printf("FAILED to close GRSPW%d\n", idx);
	}
	dev->dh = NULL;
}

#endif /* DEV_H_ */
