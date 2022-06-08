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

#include "../ccsds/CCSDS_Pkt.h"
#include "../ccsds/Apid.h"

#define DEVS_MAX 4 // 32
#define PKT_SIZE 32
#define CCSDS_PKT_SIZE 14 //16
#define DATA_MAX 136 //120 + 16

/* Protocol ID */
#define SPW_PROT_ID 155
#define GRSPW_PORT 43


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

struct spwpkt {
	struct grspw_pkt p;
	unsigned long long data[PKT_SIZE/8+1]; /* 32 bytes of data - 4byte data-header (8 extra bytes to avoid truncated bad packets)*/
	unsigned long long hdr[2]; /* up to 16byte header (path address) */
};


/******  Functions  *******************/


/**
 * This function is used to fill an int array from a string array.
 * Each hexadecimal value contained in the string array is converted into decimal 
 * value and then put into the int array.
 * This function is called in init_pkts()
 */
void init_simple_pkt_data(int *decs, char *word);

/**
 * This function is called to initialize the packet data
 * The two last arguments are used to initialize the spw header
 */
void init_pkts(struct grspw_device *devs,
			   int tx_devno,
			   int rx_devno,
			   int dest_port_addr,
			   size_t nb_pkts,
			   struct spwpkt pkts[nb_pkts],
			   void **toDel);
/**
 * This function prints each field of the CCSDS pkt given in argument
 */
void print_CCSDS_pkt(void *data);

int dma_TX(struct grspw_device *dev);
int dma_RX(struct grspw_device *dev);


#endif /* PKT_UTILS_H_ */
