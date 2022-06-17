/*
 * app_params.h
 *
 *  Created on: 15 juin 2022
 *      Author: pa267340
 */

#ifndef APP_PARAMS_H
#define APP_PARAMS_H

////////////////////////////////////////////////////////////////////////////////
///	APP PARAMETERS
/// /// devno is the number of the GRSPW device used
#define TX_DEVNO_TC 0
#define RX_DEVNO_TC 1
#define TX_DEVNO_TM 2
#define RX_DEVNO_TM 3

#define SPW_SRC_PORT_TC 3
#define SPW_DEST_PORT_TC 6
#define SPW_SRC_PORT_TM 4
#define SPW_DEST_PORT_TM 7

/// 0x2b and 0x9b are logical addresses mapped to AMBA port 2
/// 0x2b is the same as 0x9b but without header deletion
#define AMBA_LOG_DEST_PORT_TC RX_DEVNO_TC + 41 //0x2b;
#define AMBA_LOG_DEST_PORT_TM RX_DEVNO_TM + 41 //0x2b;

/// The initial number of packets to transmit
#define NB_TC_PKTS_TO_TRANSMIT 2
#define NB_TM_PKTS_TO_TRANSMIT 1
#define TOTAL_PKTS_NB NB_TC_PKTS_TO_TRANSMIT + NB_TM_PKTS_TO_TRANSMIT

// whether it's a TC or a TM
#define TC_PKT 0
#define TM_PKT 1
//#define PKT_TYPE TM_PKT
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////


#endif /* APP_PARAMS_H */
