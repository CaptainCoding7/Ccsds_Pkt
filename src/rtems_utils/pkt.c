#include "pkt.h"

/******************  BREAKPOINT FUNCTIONS  ******************************/

void end_breakpoint(int nb)
{
}

void newPkt_breakpoint(int pkt_cnt, int tx_devno)
{
}

void print_CCSDS_pkt_TC_breakpoint(
	unsigned char spw_addr, unsigned char spw_protid, unsigned char spw_spare,
	unsigned char spw_user_app,
	enum Id prim_id, uint16_t prim_seqCount, uint16_t prim_len,
	int sec_pus_version, uint8_t sec_ackFlag, uint8_t sec_serviceType,
	uint8_t sec_serviceSubType,	uint16_t sec_sourceId, uint8_t sec_spare,
	uint8_t *app_data, uint16_t crc,
	char *transactionType)
{
}

void print_CCSDS_pkt_TM_breakpoint(
	unsigned char spw_addr, unsigned char spw_protid, unsigned char spw_spare,
	unsigned char spw_user_app,
	enum Id prim_id, uint16_t prim_seqCount, uint16_t prim_len,
	int sec_pus_version, int sec_scTimeRefStatus, uint8_t sec_serviceType,
	uint8_t sec_serviceSubType,	uint16_t sec_msgTypeCounter, uint16_t sec_destId, 
	uint8_t sec_spare, uint8_t *app_data, uint16_t crc,
	char *transactionType)
{
}

void pkt_tx_breakpoint(int index, int count, int len)
{
}

void pkt_rx_breakpoint(int index, int count, int len)
{
}

/***************************************************/

void get_CCSDS_pkt_fields(void *ccsds_pkt, char *transactionType)
{
	//getting spw_hdr fields
	SPW_HDR spw_hdr = call_CCSDS_Pkt_get_spw_hdr(ccsds_pkt);
	unsigned char spw_addr = call_Spw_hdr_get_addr(spw_hdr);
	unsigned char spw_protid = call_Spw_hdr_get_protid(spw_hdr);
	unsigned char spw_spare = call_Spw_hdr_get_spare(spw_hdr);
	unsigned char spw_user_app = call_Spw_hdr_get_user_app(spw_hdr);

	// getting prim hdr fields
	PRIM_HDR prim_hdr = call_CCSDS_Pkt_get_prim_hdr(ccsds_pkt);
	enum Id prim_id= call_Prim_hdr_get_id(prim_hdr);
	uint16_t prim_seqCount = call_Prim_hdr_get_counter(prim_hdr);
	uint16_t prim_len = call_Prim_hdr_get_len(prim_hdr);

	if(PKT_TYPE==TC_PKT)
	{
		// getting pkt data TC fields
		PKT_DATA_TC pkt_data = call_CCSDS_Pkt_TC_get_pkt_data(ccsds_pkt);
		uint8_t *app_data = call_Pkt_data_TC_get_app_data(pkt_data);
		uint16_t crc = call_Pkt_data_TC_get_crc(pkt_data);
		SEC_HDR_TC sec_hdr_TC = call_Pkt_data_TC_get_sec_hdr(pkt_data);

		//getting sec hdr TC fields
		int sec_pus_version = call_Sec_hdr_TC_get_pus_version(sec_hdr_TC);
		uint8_t sec_ackFlag = call_Sec_hdr_TC_get_ackflag(sec_hdr_TC);
		uint8_t sec_serviceType = call_Sec_hdr_TC_get_serviceType(sec_hdr_TC);
		uint8_t sec_serviceSubType = call_Sec_hdr_TC_get_serviceSubType(sec_hdr_TC);
		uint16_t sec_sourceId = call_Sec_hdr_TC_get_sourceId(sec_hdr_TC);
		uint8_t sec_spare = call_Sec_hdr_TC_get_spare(sec_hdr_TC);

		print_CCSDS_pkt_TC_breakpoint(
				spw_addr, spw_protid, spw_spare, spw_user_app,
				prim_id, prim_seqCount, prim_len,
				sec_pus_version, sec_ackFlag, sec_serviceType, sec_serviceSubType,
				sec_sourceId, sec_spare, app_data, crc,
				transactionType);
	}

	else if(PKT_TYPE==TM_PKT)
	{
		// getting pkt data TC fields
		PKT_DATA_TM pkt_data = call_CCSDS_Pkt_TM_get_pkt_data(ccsds_pkt);
		uint8_t *app_data = call_Pkt_data_TM_get_app_data(pkt_data);
		uint16_t crc = call_Pkt_data_TM_get_crc(pkt_data);
		SEC_HDR_TM sec_hdr_TM = call_Pkt_data_TM_get_sec_hdr(pkt_data);

		//getting sec hdr TM fields
		int sec_pus_version = call_Sec_hdr_TM_get_pus_version(sec_hdr_TM);
		int sec_scTimeRefStatus = call_Sec_hdr_TM_get_scTimeRefStatus(sec_hdr_TM);
		uint8_t sec_serviceType = call_Sec_hdr_TM_get_serviceType(sec_hdr_TM);
		uint8_t sec_serviceSubType = call_Sec_hdr_TM_get_serviceSubType(sec_hdr_TM);
		uint16_t sec_msgTypeCounter = call_Sec_hdr_TM_get_msgTypeCounter(sec_hdr_TM);
		uint16_t sec_destId = call_Sec_hdr_TM_get_destId(sec_hdr_TM);
		uint8_t sec_spare = call_Sec_hdr_TM_get_spare(sec_hdr_TM);

		print_CCSDS_pkt_TM_breakpoint(
				spw_addr, spw_protid, spw_spare, spw_user_app,
				prim_id, prim_seqCount, prim_len,
				sec_pus_version, sec_scTimeRefStatus, sec_serviceType, sec_serviceSubType,
				sec_msgTypeCounter, sec_destId, sec_spare, app_data, crc,
				transactionType);
	}

}


void init_ccsds_tc_pkts(struct grspw_device *devs,
			   struct spw_tc_pkt pkts[NB_PKTS_TO_TRANSMIT])
{
	struct spw_tc_pkt *pkt;
	int i;

	DBG(("Initializing CCSDS TC Packets...\n"));

	/* Reinitializing tx and rx lists */
	grspw_list_clr(&devs[RX_DEVNO].rx_list);
	grspw_list_clr(&devs[TX_DEVNO].tx_list);
	grspw_list_clr(&devs[TX_DEVNO].tx_buf_list);
	devs[RX_DEVNO].rx_list_cnt = 0;
	devs[TX_DEVNO].tx_list_cnt = 0;
	devs[TX_DEVNO].tx_buf_list_cnt = 0;

	for (i = 0, pkt = &pkts[0]; i < NB_PKTS_TO_TRANSMIT; i++, pkt = &pkts[i]) {
		pkt->p.hdr = &pkt->path_hdr[0];
		pkt->p.data = &pkt->ccsds_tc_pkt[0];

		/* RX buffer */
		/* Add to device RX list */
		grspw_list_append(&devs[RX_DEVNO].rx_list, &pkt->p);
		devs[RX_DEVNO].rx_list_cnt++;

		/* TX buffer */
		/* Packet generation */
		CCSDS_PKT ccsds_pkt_tc = create_CCSDS_Pkt_TC(AMBA_LOG_DEST_PORT);
		DBG(("New packet has been created !\n"));
		pkt->p.data = ccsds_pkt_tc;
		pkt->p.dlen = CCSDS_PKT_TC_SIZE;
		/* Add to device TX list */
		grspw_list_append(&devs[TX_DEVNO].tx_buf_list, &pkt->p);
		devs[TX_DEVNO].tx_buf_list_cnt++;

	}

}

void init_ccsds_tm_pkts(struct grspw_device *devs,
			   			struct spw_tm_pkt pkts[NB_PKTS_TO_TRANSMIT])
{
	struct spw_tm_pkt *pkt;
	int i;

	DBG(("Initializing CCSDS TM Packets...\n"));

	/* Reinitializing tx and rx lists */
	grspw_list_clr(&devs[RX_DEVNO].rx_list);
	grspw_list_clr(&devs[TX_DEVNO].tx_list);
	grspw_list_clr(&devs[TX_DEVNO].tx_buf_list);
	devs[RX_DEVNO].rx_list_cnt = 0;
	devs[TX_DEVNO].tx_list_cnt = 0;
	devs[TX_DEVNO].tx_buf_list_cnt = 0;

	for (i = 0, pkt = &pkts[0]; i < NB_PKTS_TO_TRANSMIT; i++, pkt = &pkts[i]) {
		pkt->p.hdr = &pkt->path_hdr[0];
		pkt->p.data = &pkt->ccsds_tm_pkt[0];

		/* RX buffer */
		/* Add to device RX list */
		grspw_list_append(&devs[RX_DEVNO].rx_list, &pkt->p);
		devs[RX_DEVNO].rx_list_cnt++;

		/* TX buffer */
		/* Packet generation */
		CCSDS_PKT ccsds_pkt_tm = create_CCSDS_Pkt_TM(AMBA_LOG_DEST_PORT);
		DBG(("New packet has been created !\n"));
		pkt->p.data = ccsds_pkt_tm;
		pkt->p.dlen = CCSDS_PKT_TM_SIZE;
		/* Add to device TX list */
		grspw_list_append(&devs[TX_DEVNO].tx_buf_list, &pkt->p);
		devs[TX_DEVNO].tx_buf_list_cnt++;
	}
}

int dma_TX(struct grspw_device *dev)
{

	int cnt, rc, i;
	struct grspw_list lst;
	struct grspw_pkt *pkt;
	unsigned char *c;

	/* Reclaim already transmitted buffers */
	cnt = -1; /* as many packets as possible */
	grspw_list_clr(&lst);
	rc = grspw_dma_tx_reclaim(dev->dma[0], 0, &lst, &cnt);
	if (rc != 0) {
		DBG(("tx_reclaim failed %d\n", rc));
		exit(0);
	}
	/* put sent packets in end of send queue for retransmission */
	if (cnt > 0) {
		/*DBG(("GRSPW%d: Reclaimed %d TX packet buffers\n",
			dev->index, cnt);*/
		/* Clear transmission flags */
		pkt = lst.head;
		while (pkt) {
			if ((pkt->flags & TXPKT_FLAG_TX) == 0)
				DBG((" One Packet TX failed\n"));
			else if (pkt->flags & TXPKT_FLAG_LINKERR)
				DBG((" One Packet with TX errors\n"));
			pkt = pkt->next;
		}

		grspw_list_append_list(&dev->tx_buf_list, &lst);
		dev->tx_buf_list_cnt += cnt;
	}

	/* Send packets in the tx_list queue */
	if (dev->tx_list_cnt > 0) {
			DBG(("GRSPW%d: Sending %d packet(s)\n", dev->index,
				dev->tx_list_cnt));
			for (pkt = dev->tx_list.head; pkt; pkt = pkt->next) {
				pkt_tx_breakpoint(dev->index, dev->tx_list_cnt, pkt->dlen);
				DBG_print_pkt((" PKT of length %d bytes: ", pkt->hlen+pkt->dlen));
				for (i = 0; i < pkt->hlen+pkt->dlen /*&& i < 8*/; i++) {
					if (i < pkt->hlen)
						c = i + (unsigned char *)pkt->hdr;
					else
						c = i - pkt->hlen + (unsigned char *)pkt->data;
					DBG_print_pkt((" 0x%02x", *c));

				}
				DBG_print_pkt(("\n\n"));
				get_CCSDS_pkt_fields(pkt->data, "TX");

			}
			rc = grspw_dma_tx_send(dev->dma[0], 0, &dev->tx_list,
							dev->tx_list_cnt);
			if (rc != 0) {
				DBG(("tx_send failed %d\n", rc));
				exit(0);
			}
			dev->tx_list_cnt = 0;
			grspw_list_clr(&dev->tx_list);
	}

	return 0;
}


int dma_RX(struct grspw_device *dev)
{

	int cnt, rc;
	struct grspw_list lst;
	struct grspw_pkt *pkt;
	unsigned char *c;

	//DBG(("entering in dma_RX\n"));

	/* Prepare receiver with packet buffers */
		if (dev->rx_list_cnt > 0) {
			rc = grspw_dma_rx_prepare(dev->dma[0], 0, &dev->rx_list,
								dev->rx_list_cnt);
			if (rc != 0) {
				DBG(("rx_prep failed %d\n", rc));
				return -1;
			}
			/*DBG(("GRSPW%d: Prepared %d RX packet buffers for future "
			       "reception\n", dev->index, dev->rx_list_cnt);*/
			grspw_list_clr(&dev->rx_list);
			dev->rx_list_cnt = 0;
		}

		/* Try to receive packets on receiver interface */
		grspw_list_clr(&lst);
		cnt = -1; /* as many packets as possible */
		rc = grspw_dma_rx_recv(dev->dma[0], 0, &lst, &cnt);
		if (rc != 0) {
			DBG(("rx_recv failed %d\n", rc));
			return -1;
		}
		if (cnt > 0) {
			DBG(("GRSPW%d: Received %d packet(s)\n", dev->index, cnt));
			for (pkt = lst.head; pkt; pkt = pkt->next) {
				if ((pkt->flags & RXPKT_FLAG_RX) == 0) {
					DBG((" PKT not received.. buf ret\n"));
					continue;
				} else if (pkt->flags &
				           (RXPKT_FLAG_EEOP | RXPKT_FLAG_TRUNK)) {
					DBG((" PKT RX errors:"));
					if (pkt->flags & RXPKT_FLAG_TRUNK)
						DBG((" truncated"));
					if (pkt->flags & RXPKT_FLAG_EEOP)
						DBG((" EEP"));
					DBG((" (0x%x)", pkt->flags));
				} else
					DBG_print_pkt((" PKT"));

				pkt_rx_breakpoint(dev->index, cnt, pkt->dlen);
				DBG_print_pkt((" of length %d bytes: ", pkt->dlen));

				c = (unsigned char *)pkt->data;
				/// PA : Ajout d'une boucle pour l'affichage (avant un seul DBG()
				for(int i=0;i<pkt->dlen;i++)
					DBG_print_pkt(("0x%02x ", c[i]));
				DBG_print_pkt(("\n\n"));
				DBG(("\n"));

				get_CCSDS_pkt_fields(pkt->data, "RX");
				DBG(("\n\n"));
			}

			/* Reuse packet buffers by moving packets to rx_list */
			grspw_list_append_list(&dev->rx_list, &lst);
			dev->rx_list_cnt += cnt;
		}


		return 0;
}



