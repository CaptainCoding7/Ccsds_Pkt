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
#define TX_DEVNO 0 // 0 FOR THE FIRST PKT (CHANGES FOR EACH PKT IN THE LOOP)
#define RX_DEVNO 2
#define SPW_SRC_PORT 3
#define SPW_DEST_PORT 6                                                     ///
/// 0x2b and 0x9b are logical addresses mapped to AMBA port 2
/// 0x2b is the same as 0x9b but without header deletion
#define AMBA_LOG_DEST_PORT RX_DEVNO + 41 //0x2b;
/// The initial number of packets to transmit
#define NB_PKTS_TO_TRANSMIT 1
// whether it's a TC or a TM
#define TC_PKT 0
#define TM_PKT 1
#define PKT_TYPE TM_PKT
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////


#endif /* APP_PARAMS_H */
