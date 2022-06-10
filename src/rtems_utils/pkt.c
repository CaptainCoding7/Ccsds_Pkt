#include "pkt.h"

void print_CCSDS_pkt(void *ccsds_pkt)
{
	PRIM_HDR prim_hdr = call_CCSDS_Pkt_get_prim_hdr(ccsds_pkt);
	PKT_DATA pkt_data = call_CCSDS_Pkt_get_pkt_data(ccsds_pkt);
	SPW_HDR spw_hdr = call_CCSDS_Pkt_get_spw_hdr(ccsds_pkt);

	//getting spw_hdr fields
	unsigned char spw_addr = call_Spw_hdr_get_addr(spw_hdr);
	unsigned char spw_protid = call_Spw_hdr_get_protid(spw_hdr);
	unsigned char spw_spare = call_Spw_hdr_get_spare(spw_hdr);
	unsigned char spw_user_app = call_Spw_hdr_get_user_app(spw_hdr);

	// getting prim hdr fields
	enum Id prim_id= call_Prim_hdr_get_id(prim_hdr);
	uint16_t prim_seqCount = call_Prim_hdr_get_counter(prim_hdr);
	uint16_t prim_len = call_Prim_hdr_get_len(prim_hdr);

	// getting pkt data fields
	SEC_HDR_TC sec_hdr_TC = call_Pkt_data_get_sec_hdr(pkt_data);
	uint8_t *app_data = call_Pkt_data_get_app_data(pkt_data);
	uint16_t crc = call_Pkt_data_get_crc(pkt_data);

	//getting sec hdr fields
	uint8_t sec_ackFlag = call_Sec_hdr_get_ackflag(sec_hdr_TC);
	uint8_t sec_serviceType = call_Sec_hdr_get_serviceType(sec_hdr_TC);
	uint8_t sec_serviceSubType = call_Sec_hdr_get_serviceSubType(sec_hdr_TC);
	uint16_t sec_sourceId = call_Sec_hdr_get_sourceId(sec_hdr_TC);
	uint8_t sec_spare = call_Sec_hdr_get_spare(sec_hdr_TC);

	DBG_print_pkt(("\n ______________________________________________________\n"));
	DBG_print_pkt(("| --------------------  CCSDS packet  -----------------\n"));
	DBG_print_pkt(("|           Field           |          Value           \n"));
	DBG_print_pkt(("|______________________________________________________\n"));
	DBG_print_pkt(("|     Spw hdr addr (1b)     |           %d             \n"
			, spw_addr));
	DBG_print_pkt(("|     Spw hdr protid (1b)   |           %d             \n"
			, spw_protid));
	DBG_print_pkt(("|     Spw hdr spare (1b)    |           %d             \n"
			, spw_spare));
	DBG_print_pkt(("|     Spw hdr user_app (1b) |           %d             \n"
			, spw_user_app));
	DBG_print_pkt(("|        Prim hdr ID (2b)   |           %d             \n"
			, prim_id));
	DBG_print_pkt(("|    Prim hdr seqCount (2b) |           %d             \n"
			, prim_seqCount));
	DBG_print_pkt(("|        Prim hdr len (2b)  |           %d             \n"
			, prim_len));
	DBG_print_pkt(("|    Sec hdr ackFlag (1b)   |           %d             \n"
			, sec_ackFlag));
	DBG_print_pkt(("|  Sec hdr serviceType (1b) |           %d             \n"
			, sec_serviceType));
	DBG_print_pkt(("|Sec hdr serviceSubType (1b)|           %d             \n"
			, sec_serviceSubType));
	DBG_print_pkt(("|    Sec hdr sourceID (2b)  |           %d             \n"
			, sec_sourceId));
	DBG_print_pkt(("|    Sec hdr spare (1b)     |           %d             \n"
			, sec_spare));
	DBG_print_pkt(("|      App data 1 (1b)      |           %d             \n"
			, app_data[0]));
	DBG_print_pkt(("|      App data 2 (1b)      |           %d             \n"
			, app_data[1]));
	DBG_print_pkt(("|      App data 3 (1b)      |           %d             \n"
			, app_data[2]));
	DBG_print_pkt(("|                 (... 496 bytes of app data ...)      \n"));
	DBG_print_pkt(("|      App data 500 (1b)    |           %d             \n"
			, app_data[499]));
	DBG_print_pkt(("|           crc (2b)        |           %d             \n"
			, crc));
	DBG_print_pkt(("|______________________________________________________\n\n"));

}


void init_pkts(struct grspw_device *devs,
			   int tx_devno,
			   int rx_devno,
			   int dest_log_addr,
			   size_t nb_pkts,
			   struct spwpkt pkts[nb_pkts])//,
			   //void **pkts_to_del)
{
	struct spwpkt *pkt;
	int i;

	DBG(("Initializing CCSDS TC Packets...\n"));

	/* Reinitializing tx and rx lists */
	grspw_list_clr(&devs[rx_devno].rx_list);
	grspw_list_clr(&devs[tx_devno].tx_list);
	grspw_list_clr(&devs[tx_devno].tx_buf_list);
	devs[rx_devno].rx_list_cnt = 0;
	devs[tx_devno].tx_list_cnt = 0;
	devs[tx_devno].tx_buf_list_cnt = 0;

	for (i = 0, pkt = &pkts[0]; i < nb_pkts; i++, pkt = &pkts[i]) {
		pkt->p.hdr = &pkt->path_hdr[0];
		pkt->p.data = &pkt->ccsds_pkt[0];

		/* RX buffer */
		/* Add to device RX list */
		grspw_list_append(&devs[rx_devno].rx_list, &pkt->p);
		devs[rx_devno].rx_list_cnt++;

		/* TX buffer */
		/* Packet generation */
		CCSDS_PKT ccsds_pkt = create_CCSDS_Pkt(dest_log_addr);
		DBG(("New packet has been created !\n"));
		pkt->p.data = ccsds_pkt;
		pkt->p.dlen = CCSDS_PKT_SIZE;
		/* Add to device TX list */
		grspw_list_append(&devs[tx_devno].tx_buf_list, &pkt->p);
		devs[tx_devno].tx_buf_list_cnt++;

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
				DBG_print_pkt((" PKT of length %d bytes: ", pkt->hlen+pkt->dlen));
				for (i = 0; i < pkt->hlen+pkt->dlen /*&& i < 8*/; i++) {
					if (i < pkt->hlen)
						c = i + (unsigned char *)pkt->hdr;
					else
						c = i - pkt->hlen + (unsigned char *)pkt->data;
					DBG_print_pkt((" 0x%02x", *c));

				}
				print_CCSDS_pkt(pkt->data);

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
				c = (unsigned char *)pkt->data;
				DBG_print_pkt((" of length %d bytes: ", pkt->dlen));
				/// PA : Ajout d'une boucle pour l'affichage (avant un seul DBG()
				for(int i=0;i<pkt->dlen;i++)
					DBG_print_pkt(("0x%02x ", c[i]));
				DBG(("\n"));

				print_CCSDS_pkt(pkt->data);
				DBG(("\n\n"));
			}

			/* Reuse packet buffers by moving packets to rx_list */
			grspw_list_append_list(&dev->rx_list, &lst);
			dev->rx_list_cnt += cnt;
		}


		return 0;
}



