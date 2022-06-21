/*
 * app_params.h
 *
 *  Created on: 15 juin 2022
 *      Author: pa267340
 */

#ifndef APP_PARAMS_H
#define APP_PARAMS_H

/* The parameters the user should modify to test the app */
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
#define NB_TC_PKTS_TO_TRANSMIT 1
#define NB_TM_PKTS_TO_TRANSMIT 1
#define TOTAL_PKTS_NB NB_TC_PKTS_TO_TRANSMIT + NB_TM_PKTS_TO_TRANSMIT

// whether it's a TC or a TM
#define TC_PKT 0
#define TM_PKT 1
//#define PKT_TYPE TM_PKT
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////


/**************  Some default constants from the RCC example ***************/
#include <rtems.h>
#include <bsp.h> /* for device driver prototypes */

/* Number of GRSPW cores supported */
#define DEVS_MAX 4 //32

/* configuration information */
#define CONFIGURE_INIT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

/* Set some values if someone should modify the example. The shared IRQ layer
 * need one semaphore.
 */
#define CONFIGURE_MAXIMUM_TASKS             8
#define CONFIGURE_MAXIMUM_SEMAPHORES        (5 + (DEVS_MAX * 5))
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES    20
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 32
#define CONFIGURE_MAXIMUM_DRIVERS 32
#define CONFIGURE_MAXIMUM_PERIODS             1
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT_TASK_ATTRIBUTES    RTEMS_DEFAULT_ATTRIBUTES | RTEMS_FLOATING_POINT
#define CONFIGURE_EXTRA_TASK_STACKS         (40 * RTEMS_MINIMUM_STACK_SIZE)
/* One tick every 2 ms => 500 ticks per second */
#define CONFIGURE_MILLISECONDS_PER_TICK	2
#define CONFIGURE_MICROSECONDS_PER_TICK RTEMS_MILLISECONDS_TO_MICROSECONDS(CONFIGURE_MILLISECONDS_PER_TICK)

/* Configure Driver manager */
#if defined(RTEMS_DRVMGR_STARTUP) && defined(LEON3) /* if --drvmgr was given to configure */
 /* Add Timer and UART Driver for this example */
 #ifdef CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
  #define CONFIGURE_DRIVER_AMBAPP_GAISLER_GPTIMER
 #endif
 #ifdef CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
  #define CONFIGURE_DRIVER_AMBAPP_GAISLER_APBUART
 #endif
#endif

#define CONFIGURE_DRIVER_AMBAPP_GAISLER_SPW_ROUTER /* SpaceWire Router  */
#define CONFIGURE_DRIVER_AMBAPP_GAISLER_GRSPW2     /* SpaceWire Packet driver */


/*******************************************/

#endif /* APP_PARAMS_H */
