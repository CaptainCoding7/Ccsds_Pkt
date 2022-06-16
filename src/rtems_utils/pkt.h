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

#include "../app_params.h"
#include "../ccsds/CCSDS_Pkt.h"
#include "../ccsds/Apid.h"
#include "../debug_print.h"


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
/// Custom structures

struct spw_tc_pkt {
	struct grspw_pkt p;
	//unsigned long long ccsds_pkt[PKT_SIZE/8+1]; /* 32 bytes of data - 4byte data-header (8 extra bytes to avoid truncated bad packets)*/
	//unsigned long long ccsds_pkt[CCSDS_PKT_LONG_LONG_SIZE];
	//unsigned long long path_hdr[2]; /* up to 16byte header (path address) */
	uint8_t ccsds_tc_pkt[CCSDS_PKT_TC_SIZE];
	uint8_t path_hdr[2]; /* up to 16byte header (path address) */
};

struct spw_tm_pkt {
	struct grspw_pkt p;
	uint8_t ccsds_tm_pkt[CCSDS_PKT_TM_SIZE];
	uint8_t path_hdr[2]; /* up to 16byte header (path address) */
};

/******  Functions  *******************/
/***************************************/

/******************  BREAKPOINT FUNCTIONS  ******************************/

void end_breakpoint(int nb);

void newPkt_breakpoint(int pkt_cnt, int tx_devno);

void print_CCSDS_pkt_breakpoint(
	unsigned char spw_addr, unsigned char spw_protid, unsigned char spw_spare,
	unsigned char spw_user_app,
	enum Id prim_id, uint16_t prim_seqCount, uint16_t prim_len,
	int sec_pus_version, uint8_t sec_ackFlag, uint8_t sec_serviceType,
	uint8_t sec_serviceSubType,	uint16_t sec_sourceId, uint8_t sec_spare,
	uint8_t *app_data, uint16_t crc,
	char *transactionType);

void print_CCSDS_pkt_TM_breakpoint(
	unsigned char spw_addr, unsigned char spw_protid, unsigned char spw_spare,
	unsigned char spw_user_app,
	enum Id prim_id, uint16_t prim_seqCount, uint16_t prim_len,
	int sec_pus_version, int sec_scTimeRefStatus, uint8_t sec_serviceType,
	uint8_t sec_serviceSubType,	uint16_t sec_msgTypeCounter, uint16_t sec_destId, 
	uint8_t sec_spare, uint8_t *app_data, uint16_t crc,
	char *transactionType);

void pkt_tx_breakpoint(int index, int count, int len);

void pkt_rx_breakpoint(int index, int count, int len);

/***************************************/

/**
 * This function is used to fill an int array from a string array.
 * Each hexadecimal value contained in the string array is converted into decimal 
 * value and then put into the int array.
 * This function is called in init_pkts()
 */
void init_simple_pkt_data(int *decs, char *word);

/**
 * This function is called to initialize the TC packets data
 * We create as many packets as determined by nb_pkts.
 * Each packet will be added to the tx list of the th device and the
 * rx list of the rx device
 */
void init_ccsds_tc_pkts(struct grspw_device *devs,
			   			struct spw_tc_pkt pkts[NB_PKTS_TO_TRANSMIT]
			   			);

/**
 * This function is called to initialize the TM packets data
 */
void init_ccsds_tm_pkts(struct grspw_device *devs,
			   			struct spw_tm_pkt pkts[NB_PKTS_TO_TRANSMIT]
			  			);
/**
 * This function get each field of the CCSDS pkt given in argument
 */
void get_CCSDS_pkt_fields(void *data, char *transactionType);

int dma_TX(struct grspw_device *dev);
int dma_RX(struct grspw_device *dev);


#endif /* PKT_UTILS_H_ */
