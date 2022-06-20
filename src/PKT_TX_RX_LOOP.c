/*
 * A simple program to send and receive packets on spacewire ports.
 *
 * The application consists of four threads:
 *
 *  TA01. Packet scheduling task
 *
 *  TA02. Link monitor task. Prints out whenever a SpaceWire link switch
 *        from run-state to any other state or vice versa.
 *
 *  TA03. SpaceWire DMA task for TC packets.Handles reception and transmission
 *  	  of TC packets on all SpaceWire devices.
 *
 *  TA04. SpaceWire DMA task for TM packets.
 *
 *  Tick-out IRQs are catched by the time-code ISR, and printed on STDOUT.
 *
 *
 */

#include "app_params.h"

#include <rtems/confdefs.h>
#include <drvmgr/drvmgr_confdefs.h>
#include <rtems/bspIo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <grlib/grspw_router.h>
#include "/opt/rcc-1.3.1-gcc/src/samples/config.c"

#include "rtems_utils/pkt.h"
#include "rtems_utils/dev.h"
#include "ccsds/CCSDS_Pkt.h"

//#undef ENABLE_NETWORK
//#undef ENABLE_NETWORK_SMC_LEON3

/* Forward declarations */
/* All the tasks */
rtems_task Init( rtems_task_argument argument);
rtems_task test_app(rtems_task_argument ignored);
rtems_task link_ctrl_task(rtems_task_argument unused);
rtems_task dma_task_tc(rtems_task_argument unused);
rtems_task dma_task_tm(rtems_task_argument unused);
/* Functions */
int dma_process(struct grspw_device *dev);
extern int router_setup_custom(void);
extern int router_print_port_status(void);

// task ids
rtems_id tid, tid_link, tid_dma_tc, tid_dma_tm;
// semaphore id
rtems_id dma_tc_sem;
rtems_id dma_tm_sem;
rtems_name timer;
rtems_id *timer_id;

/* Global variables */
int nospw = 0;
int tasks_stop = 0;
int rx_packets = 0;
struct spw_tc_pkt *tc_pkts;
struct spw_tm_pkt *tm_pkts;
// Router:
extern struct router_hw_info router_hw;
extern void *router;
int router_present = 0;

/***********************************************/

/*
 * A function to initialize the router and the GRSPW cores
 */
void init_router()
{

	int i;

/* Initialize two GRSPW AMBA ports */
	DBG(("Setting up SpaceWire router\n"));
	DBG(("Activating all Links/Ports...\n"));
	if (router_setup_custom()) {
		DBG(("Failed router initialization, assuming that it does not exists\n"));
	} else {
		/* on-chip router found */
		if (router_hw.nports_amba < 2) {
			DBG(("Error. Router with less than 2 AMBA ports not supported\n"));
			exit(0);
		}
		router_present = 1;
	}
	DBG(("OK\n\n"));

	nospw = grspw_dev_count();
	if (nospw < 1) {
		DBG(("Found no SpaceWire cores, aborting\n"));
		exit(0);
	}
	if (nospw > DEVS_MAX) {
		DBG(("Limiting to %d SpaceWire devices\n", DEVS_MAX));
		nospw = DEVS_MAX;
	}

	DBG(("Initializing all SpaceWire devices...\n"));
	memset(devs, 0, sizeof(devs));
	for (i=0; i<nospw; i++) {
		if (dev_init(i)) {
			DBG(("Failed to initialize GRSPW%d\n", i));
			exit(0);
		}
		fflush(NULL);
	}
	DBG(("OK\n\n"));

	DBG(("Starting SpW DMA channels...\n"));
	for (i = 0; i < nospw; i++) {
		//DBG(("Starting GRSPW%d: ", i);
		fflush(NULL);
		if (grspw_start(DEV(&devs[i]))) {
			DBG(("Failed to initialize GRSPW%d\n", i));
			exit(0);
		}
		//DBG(("DMA Started Successfully\n"));
	}
	DBG(("OK\n\n"));

	fflush(NULL);
}

/**-------------------------- INIT TASK --------------------------**/

rtems_task Init(
  rtems_task_argument ignored
)
{
	/* Initialize Driver manager and Networking, in config.c */
	system_init();

	/* Print device topology */
	/*drvmgr_print_topo();*/
	rtems_task_wake_after(4);

	tasks_stop = nospw = 0;

	/* Run SpaceWire Test application */
	rtems_task_create(
			rtems_build_name( 'T', 'A', '0', '1' ),
			10, RTEMS_MINIMUM_STACK_SIZE * 10, RTEMS_DEFAULT_MODES,
			RTEMS_FLOATING_POINT, &tid);
	rtems_task_create(
			rtems_build_name( 'T', 'A', '0', '2' ),
			10, RTEMS_MINIMUM_STACK_SIZE * 10, RTEMS_DEFAULT_MODES,
			RTEMS_FLOATING_POINT, &tid_link);
	rtems_task_create(
			rtems_build_name( 'T', 'A', '0', '3' ),
			10, RTEMS_MINIMUM_STACK_SIZE * 10, RTEMS_DEFAULT_MODES,
			RTEMS_FLOATING_POINT, &tid_dma_tc);
	rtems_task_create(
			rtems_build_name( 'T', 'A', '0', '4' ),
			10, RTEMS_MINIMUM_STACK_SIZE * 10, RTEMS_DEFAULT_MODES,
			RTEMS_FLOATING_POINT, &tid_dma_tm);
	/* Device Semaphore created with count = 1 */
	if (rtems_semaphore_create(rtems_build_name('S', 'E', 'M', '0'), 1,
	    RTEMS_FIFO | RTEMS_SIMPLE_BINARY_SEMAPHORE | \
	    RTEMS_NO_INHERIT_PRIORITY | RTEMS_LOCAL | \
	    RTEMS_NO_PRIORITY_CEILING, 0, &dma_tc_sem) != RTEMS_SUCCESSFUL) {
		DBG(("Failed creating Semaphore\n"));
		exit(0);
	}
	if (rtems_semaphore_create(rtems_build_name('S', 'E', 'M', '1'), 1,
	    RTEMS_FIFO | RTEMS_SIMPLE_BINARY_SEMAPHORE | \
	    RTEMS_NO_INHERIT_PRIORITY | RTEMS_LOCAL | \
	    RTEMS_NO_PRIORITY_CEILING, 0, &dma_tm_sem) != RTEMS_SUCCESSFUL) {
		DBG(("Failed creating Semaphore\n"));
		exit(0);
	}
	rtems_task_start(tid, test_app, 0);
	rtems_task_suspend( RTEMS_SELF );
}


/*************************  TEST APP TASK  *******************************/

rtems_task test_app(rtems_task_argument ignored)
{

	int i;
	struct grspw_pkt *pkt_tc, *pkt_tm;
	struct route_entry route_TC, route_TM;
	/// this variable will decrease !
	int nb_tc_pkts = NB_TC_PKTS_TO_TRANSMIT;
	int nb_tm_pkts = NB_TM_PKTS_TO_TRANSMIT;

	DBG(("\nStarted test app task\n"));

	/* Initialize router, AMBA ports */
	init_router();

	/* Initializing CCSDS pkt according to its type */

	/* Initializing TC packets */
	tc_pkts = malloc(sizeof(struct spw_tc_pkt) * nb_tc_pkts);
	init_ccsds_tc_pkts(devs, tc_pkts);
	/* Initializing TM packets */
	tm_pkts = malloc(sizeof(struct spw_tm_pkt) * nb_tm_pkts);
	init_ccsds_tm_pkts(devs, tm_pkts);


	rtems_task_start(tid_link, link_ctrl_task, 0);
	rtems_task_start(tid_dma_tc, dma_task_tc, 0);
	rtems_task_start(tid_dma_tm, dma_task_tm, 0);
	rtems_task_wake_after(12);

	////////////
	DBG(("\n***********  PKT TX/RX TEST  **************\n\n"));

	/* Setting the two routes */

	memset(&route_TC, 0, sizeof(route_TC));
	memset(&route_TM, 0, sizeof(route_TM));

	route_TC.dstadr[0]=SPW_SRC_PORT_TC;
	route_TC.dstadr[1]=SPW_DEST_PORT_TC;
	route_TC.dstadr[2]=AMBA_LOG_DEST_PORT_TC;

	route_TM.dstadr[0]=SPW_SRC_PORT_TM;
	route_TM.dstadr[1]=SPW_DEST_PORT_TM;
	route_TM.dstadr[2]=AMBA_LOG_DEST_PORT_TM;


	while(!(nb_tc_pkts==0 && nb_tm_pkts==0))
	{

		rtems_task_wake_after(12);

		/* TM */
		if(nb_tm_pkts>0)
		{
			rtems_semaphore_obtain(dma_tm_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
			pkt_tm = devs[TX_DEVNO_TM].tx_buf_list.head;
			send_pkt(DEVS_MAX, devs, pkt_tm, route_TM, TX_DEVNO_TM);
			//print_string_breakpoint("called send_pkt TM");
			rtems_semaphore_release(dma_tm_sem);
			nb_tm_pkts--;
		}

		/* TC */
		if(nb_tc_pkts>0)
		{
			rtems_semaphore_obtain(dma_tc_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
			pkt_tc = devs[TX_DEVNO_TC].tx_buf_list.head;
			send_pkt(DEVS_MAX, devs, pkt_tc, route_TC, TX_DEVNO_TC);
			//print_string_breakpoint("called send_pkt TC");
			rtems_semaphore_release(dma_tc_sem);
			nb_tc_pkts--;
		}

	}

	rtems_task_wake_after(100);
	tasks_stop = 1;
	for ( i=0; i<nospw; i++)
		dev_cleanup(i);
	rtems_task_wake_after(8);

	DBG(("\n\n[DEBUG]--------- MEMORY CLEANING ---------.\n\n"));

	for (int i = 0; i < NB_TC_PKTS_TO_TRANSMIT; i++)
		delete_CCSDS_Pkt_TC(tc_pkts[i].p.data, i);
	free(tc_pkts);

	for (int i = 0; i < NB_TM_PKTS_TO_TRANSMIT; i++)
		delete_CCSDS_Pkt_TM(tm_pkts[i].p.data, i);
	free(tm_pkts);

	DBG(("=> Array of packets has been freed.\n"));

	if(rx_packets < TOTAL_PKTS_NB)
	{
		print_fail_breakpoint(rx_packets, TOTAL_PKTS_NB);
	}
	else
	{
		print_ok_breakpoint(TOTAL_PKTS_NB);
		DBG(("END OF THE TEST: %d packet(s) was (were) successfully sended and received.\n", NB_PKTS_TO_TRANSMIT));
		//printf("\nEND OF THE TEST: %d packet was (were) successfully sended and received.\n", NB_PKTS_TO_TRANSMIT);
	}
	print_elapsed_time();
	exit(0);

}


/**************************************************************************/

/*************************  LINK CTRL TASK  **************************/

rtems_task link_ctrl_task(rtems_task_argument unused)
{
	int i, run;
	struct grspw_device *dev;
	spw_link_state_t state;
	uint32_t sts;
	spw_link_state_t rtrp[32]; /* router ports link status */

	memset(rtrp, 0, sizeof(rtrp));

	DBG(("\nStarted link control task\n"));

	while (tasks_stop == 0) {
		for (i = 0; i < nospw; i++) {
			dev = &devs[i];
			if (dev->dh == NULL)
				continue;

			/* Check if link status has changed */
			state = grspw_link_state(dev->dh);
			run = 0;
			if (state == SPW_LS_RUN)
				run = 1;
			if ((run && dev->run == 0) || (run == 0 && dev->run)) {
				if (run)
					DBG(("GRSPW%d: link state entering run-state\n", i));
				else
					DBG(("GRSPW%d: link state leaving run-state\n", i));
				dev->run = run;
			}
		}

		/* Check link-status of router ports */
		if (router_present) {
			for (i = 0; i < router_hw.nports_spw; i++) {
				router_port_status(router, i+1, &sts, 0x0);
				state = (sts >> 12) & 0x7;

				run = 0;
				if (state == SPW_LS_RUN)
					run = 1;
				if ((run && rtrp[i] == 0) || (run == 0 && rtrp[i])) {
					if (run)
						DBG(("ROUTER SpW PORT%d: link state entering run-state\n", i+1));
					else
						DBG(("ROUTER SpW PORT%d: link state leaving run-state\n", i+1));
					rtrp[i] = run;
				}
			}
		}

		rtems_task_wake_after(4);
	}

	DBG((" Link control task shutdown\n"));

	rtems_task_delete(RTEMS_SELF);
}

/**************************************************************************/

/****************************  DMA TASK  ***********************************/

rtems_task dma_task_tc(rtems_task_argument unused)
{
	int i;
	struct grspw_device *dev;

	DBG(("\nStarted DMA control task\n"));

	while (tasks_stop == 0) {
		rtems_semaphore_obtain(dma_tc_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

		dev = &devs[TX_DEVNO_TC];
		if (dev->dh != NULL)
			dma_process(dev);
		dev = &devs[RX_DEVNO_TC];
		if (dev->dh != NULL)
			dma_process(dev);

		rtems_semaphore_release(dma_tc_sem);
		rtems_task_wake_after(20);
	}

	DBG((" DMA task shutdown\n"));

	rtems_task_delete(RTEMS_SELF);

}

rtems_task dma_task_tm(rtems_task_argument unused)
{
	int i;
	struct grspw_device *dev;

	DBG(("\nStarted DMA control task\n"));

	while (tasks_stop == 0) {
		rtems_semaphore_obtain(dma_tm_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

		dev = &devs[TX_DEVNO_TM];
		if (dev->dh != NULL)
			dma_process(dev);
		dev = &devs[RX_DEVNO_TM];
		if (dev->dh != NULL)
			dma_process(dev);

		rtems_semaphore_release(dma_tm_sem);
		rtems_task_wake_after(20);
	}

	DBG((" DMA task shutdown\n"));

	rtems_task_delete(RTEMS_SELF);

}

int dma_process(struct grspw_device *dev)
{

	int rx_ready, rx_sched, rx_recv, tx_send, tx_sched, tx_sent, tx_hwcnt, rx_hwcnt;

	grspw_dma_rx_count(dev->dma[0], &rx_ready, &rx_sched, &rx_recv, &rx_hwcnt);
	grspw_dma_tx_count(dev->dma[0], &tx_send, &tx_sched, &tx_sent, &tx_hwcnt);
	if (rx_hwcnt >= 127) {
		DBG((" DMA DRVQ RX_READY: %d\n", rx_ready));
		DBG((" DMA DRVQ RX_SCHED: %d\n", rx_sched));
		DBG((" DMA DRVQ RX_RECV: %d\n", rx_recv));
		DBG((" DMA DRVQ RX_HWCNT: %d\n", rx_hwcnt));
		DBG((" DMA DRVQ TX_SEND: %d\n", tx_send));
		DBG((" DMA DRVQ TX_SCHED: %d\n", tx_sched));
		DBG((" DMA DRVQ TX_SENT: %d\n", tx_sent));
		DBG((" DMA DRVQ TX_HWCNT: %d\n", tx_hwcnt));

	}

	rx_packets = dma_RX(dev, rx_packets);

	dma_TX(dev);


	return 0;

}
