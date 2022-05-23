/*
 * dev_pkt_util.h
 *
 *  Created on: 13 mai 2022
 *      Author: pa267340
 */

#ifndef PKT_UTILS_H_
#define PKT_UTILS_H_


#include <rtems.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <grlib/grspw_pkt.h>
#include "grspw_pkt_lib.h"

#define DEVS_MAX 4 // 32
#define PKT_SIZE 32
#define DATA_MAX 136 //120 + 16

/* Protocol ID */
#define SPW_PROT_ID 155


struct grspw_device {
	/* GRSPW Device layout - must be the same as 'struct grspw_dev' */
	void *dh;
	void *dma[4];
	int index;
	struct grspw_hw_sup hwsup;

	/* Test structures */
	struct grspw_config cfg;
	int run;

	/* RX and TX lists with packet buffers */
	struct grspw_list rx_list, tx_list, tx_buf_list;
	int rx_list_cnt, tx_list_cnt, tx_buf_list_cnt;
};
#define DEV(device) ((struct grspw_dev *)(device))

/* SpaceWire Routing table entry */
struct route_entry {
	unsigned char dstadr[16];	/* 0 terminates array */
};

/******************************************************************************/
/// Structures

/* SpaceWire packet payload (data) content layout */
struct pkt_hdr {
	unsigned char addr;
	unsigned char protid;
	unsigned char port_src; /* port index of source */
	unsigned char resv2; /* Zero for now */
	unsigned int data[(PKT_SIZE-4)/4];
};

/* Custom pkt_hdr structure */
struct my_pkt_hdr{
	unsigned char addr;
	unsigned char protid; /* necessary to receive data */
	unsigned int data[(PKT_SIZE-4)/4];
};

struct spwpkt {
	struct grspw_pkt p;
	unsigned long long data[PKT_SIZE/8+1]; /* 32 bytes of data - 4byte data-header (8 extra bytes to avoid truncated bad packets)*/
	unsigned long long hdr[2]; /* up to 16byte header (path address) */
};


/******  Functions  *******************/

void pkt_init_hdr(struct grspw_pkt *pkt, struct route_entry *route, int idx);
void init_pkts(struct grspw_device *devs, struct spwpkt pkts[DEVS_MAX][DATA_MAX]);
int dma_RX(struct grspw_device *dev);
int dma_TX(struct grspw_device *dev);


#endif /* PKT_UTILS_H_ */
