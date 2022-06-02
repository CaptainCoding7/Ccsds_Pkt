#include "pkt.h"

CCSDS_PKT ccsds_pkt_global;

void print_CCSDS_pkt(void *data)
{
	PRIM_HDR prim_hdr = call_CCSDS_Pkt_get_prim_hdr(data);
	SEC_HDR sec_hdr = call_CCSDS_Pkt_get_sec_hdr(data);
	SPW_HDR spw_hdr = call_CCSDS_Pkt_get_spw_hdr(data);

	//getting spw_hdr fields
	unsigned char spw_addr = call_Spw_hdr_get_addr(spw_hdr);
	unsigned char spw_protid = call_Spw_hdr_get_protid(spw_hdr);
	unsigned char spw_spare = call_Spw_hdr_get_spare(spw_hdr);
	unsigned char spw_user_app = call_Spw_hdr_get_user_app(spw_hdr);

	// getting prim hdr fields
	enum Id prim_id= call_Prim_hdr_get_id(prim_hdr);
	uint16_t prim_seqCount = call_Prim_hdr_get_counter(prim_hdr);
	uint16_t prim_len = call_Prim_hdr_get_len(prim_hdr);

	//getting sec hdr fields
	uint8_t sec_serviceType = call_Sec_hdr_get_serviceType(sec_hdr);
	uint8_t sec_serviceSubType = call_Sec_hdr_get_serviceSubType(sec_hdr);
	uint8_t sec_sourceId = call_Sec_hdr_get_sourceId(sec_hdr);
	uint8_t sec_ackFlag = call_Sec_hdr_get_ackflag(sec_hdr);

	printf("\n ______________________________________________________\n");
	printf("| --------------------  CCSDS packet  -----------------\n");
	printf("|           Field           |          Value           \n");
	printf("|______________________________________________________\n");
	printf("|     Spw hdr addr (1b)     |           %d             \n"
			, spw_addr);
	printf("|     Spw hdr protid (1b)   |           %d             \n"
			, spw_protid);
	printf("|     Spw hdr spare (1b)    |           %d             \n"
			, spw_spare);
	printf("|     Spw hdr user_app (1b) |           %d             \n"
			, spw_user_app);
	printf("|        Prim hdr ID (2b)   |           %d             \n"
			, prim_id);
	printf("|    Prim hdr seqCount (2b) |           %d             \n"
			, prim_seqCount);
	printf("|        Prim hdr len (2b)  |           %d             \n"
			, prim_len);
	printf("|    Sec hdr sourceID (1b)  |           %d             \n"
			, sec_sourceId);
	printf("|Sec hdr serviceSubType (1b)|           %d             \n"
			, sec_serviceSubType);
	printf("|  Sec hdr serviceType (1b) |           %d             \n"
			, sec_serviceType);
	printf("|    Sec hdr ackFlag (1b)   |           %d             \n"
			, sec_ackFlag);
	printf("|______________________________________________________\n\n");

}


void init_pkts(struct grspw_device *devs,
			   struct spwpkt pkts[DEVS_MAX][DATA_MAX],
			   int dest_port_addr)
{
	struct spwpkt *pkt;
	int i, j;

	memset(&pkts[0][0], 0, sizeof(pkts));

	for (i = 0; i < DEVS_MAX; i++) {

		grspw_list_clr(&devs[i].rx_list);
		grspw_list_clr(&devs[i].tx_list);
		grspw_list_clr(&devs[i].tx_buf_list);
		devs[i].rx_list_cnt = 0;
		devs[i].tx_list_cnt = 0;
		devs[i].tx_buf_list_cnt = 0;

		for (j = 0, pkt = &pkts[i][0]; j < DATA_MAX; j++, pkt = &pkts[i][j]) {
			pkt->p.pkt_id = (i << 8)+ j; /* unused */
			// structures addresses are aligned:
			pkt->p.hdr = &pkt->hdr[0];
			pkt->p.data = &pkt->data[0];
			if (j < 120+8) {
				/* RX buffer */

				/* Add to device RX list */
				grspw_list_append(&devs[i].rx_list, &pkt->p);
				devs[i].rx_list_cnt++;
			} else {
				/* TX buffer */
				pkt->p.dlen = CCSDS_PKT_SIZE; //PKT_SIZE;

				// create a default CCSDS object
				//CCSDS_PKT ccsds_pkt = create_CCSDS_Pkt(dest_port_addr);
				//ccsds_pkt_global = create_CCSDS_Pkt(dest_port_addr);

				//pkt->p.data = ccsds_pkt;
				//pkt->p.data = ccsds_pkt_global

				pkt->p.data = create_CCSDS_Pkt(dest_port_addr);

				/* Add to device TX list */
				grspw_list_append(&devs[i].tx_buf_list, &pkt->p);
				devs[i].tx_buf_list_cnt++;
			}
		}
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
		printf("tx_reclaim failed %d\n", rc);
		exit(0);
	}
	/* put sent packets in end of send queue for retransmission */
	if (cnt > 0) {
		/*printf("GRSPW%d: Reclaimed %d TX packet buffers\n",
			dev->index, cnt);*/
		/* Clear transmission flags */
		pkt = lst.head;
		while (pkt) {
			if ((pkt->flags & TXPKT_FLAG_TX) == 0)
				printf(" One Packet TX failed\n");
			else if (pkt->flags & TXPKT_FLAG_LINKERR)
				printf(" One Packet with TX errors\n");
			pkt = pkt->next;
		}

		grspw_list_append_list(&dev->tx_buf_list, &lst);
		dev->tx_buf_list_cnt += cnt;
	}

	/* Send packets in the tx_list queue */
	if (dev->tx_list_cnt > 0) {
			printf("GRSPW%d: Sending %d packet(s)\n", dev->index,
				dev->tx_list_cnt);
			for (pkt = dev->tx_list.head; pkt; pkt = pkt->next) {
				printf(" PKT of length %d bytes: ", pkt->hlen+pkt->dlen);
				for (i = 0; i < pkt->hlen+pkt->dlen /*&& i < 8*/; i++) {
					if (i < pkt->hlen)
						c = i + (unsigned char *)pkt->hdr;
					else
						c = i - pkt->hlen + (unsigned char *)pkt->data;
					printf(" 0x%02x", *c);

				}
				print_CCSDS_pkt(pkt->data);

			}
			rc = grspw_dma_tx_send(dev->dma[0], 0, &dev->tx_list,
							dev->tx_list_cnt);
			if (rc != 0) {
				printf("tx_send failed %d\n", rc);
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
				printf("rx_prep failed %d\n", rc);
				return -1;
			}
			/*printf("GRSPW%d: Prepared %d RX packet buffers for future "
			       "reception\n", dev->index, dev->rx_list_cnt);*/
			grspw_list_clr(&dev->rx_list);
			dev->rx_list_cnt = 0;
		}

		/* Try to receive packets on receiver interface */
		grspw_list_clr(&lst);
		cnt = -1; /* as many packets as possible */
		rc = grspw_dma_rx_recv(dev->dma[0], 0, &lst, &cnt);
		if (rc != 0) {
			printf("rx_recv failed %d\n", rc);
			return -1;
		}
		if (cnt > 0) {
			printf("GRSPW%d: Received %d packet(s)\n", dev->index, cnt);
			for (pkt = lst.head; pkt; pkt = pkt->next) {
				if ((pkt->flags & RXPKT_FLAG_RX) == 0) {
					printf(" PKT not received.. buf ret\n");
					continue;
				} else if (pkt->flags &
				           (RXPKT_FLAG_EEOP | RXPKT_FLAG_TRUNK)) {
					printf(" PKT RX errors:");
					if (pkt->flags & RXPKT_FLAG_TRUNK)
						printf(" truncated");
					if (pkt->flags & RXPKT_FLAG_EEOP)
						printf(" EEP");
					printf(" (0x%x)", pkt->flags);
				} else
					printf(" PKT");
				c = (unsigned char *)pkt->data;
				//c = (unsigned char *) ccsds_pkt_global;
				printf(" of length %d bytes: ", pkt->dlen);


				/// PA : Ajout d'une boucle pour l'affichage (avant un seul printf)
				for(int i=0;i<pkt->dlen;i++)
					printf("0x%02x ", c[i]);

				printf("\n");

//				for (int i = -2; i < 3; i++)
//					print_CCSDS_pkt(pkt->data +i );
				print_CCSDS_pkt(pkt->data);
				//print_CCSDS_pkt(ccsds_pkt_global);

				printf("\n\n");
			}

			/* Reuse packet buffers by moving packets to rx_list */
			grspw_list_append_list(&dev->rx_list, &lst);
			dev->rx_list_cnt += cnt;
		}

		return 0;
}



