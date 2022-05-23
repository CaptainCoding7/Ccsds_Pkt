#include "pkt.h"


/* Initialize packet header, and Data payload header */
void pkt_init_hdr(struct grspw_pkt *pkt, struct route_entry *route, int idx)
{
	int i;
	//struct pkt_hdr *pkt_hdr = (struct pkt_hdr *)pkt->data;
	struct my_pkt_hdr *my_pkt_hdr = (struct my_pkt_hdr *)pkt->data;
	unsigned char *hdr = pkt->hdr;

	/* If path addressing we put non-first Destination Addresses in
	 * header. route->dstadr[0] is always non-zero.
	 */
	i = 0;
	while ( route->dstadr[i+1] != 0 ) {
		hdr[i] = route->dstadr[i];
		i++;
	}
	/* Put last address in pkthdr->addr */
	pkt->hlen = i;
	my_pkt_hdr->addr = route->dstadr[i];
	//pkt_hdr->addr = route->dstadr[i];
	my_pkt_hdr->protid = SPW_PROT_ID;
	//pkt_hdr->port_src = idx;
	//pkt_hdr->resv2 = 0;
}

void init_pkt_data(int *decs, char *word)
{

	char *pstr = &word[0];
	int lw = strlen(word)/2;
	//int decs[lw];
    char twoLetters[2];

    memset(twoLetters, '\0', sizeof(twoLetters));

	for(int i=0;i<lw;i++)
	{
		strncpy(twoLetters, pstr, 2);
		decs[i] = (int) strtol(twoLetters, NULL, 16);
		//memset(pkt->p.data+4, dec, PKT_SIZE-4);
		pstr+=2;
	}
	
}

void init_pkts(struct grspw_device *devs, struct spwpkt pkts[DEVS_MAX][DATA_MAX])
{
	struct spwpkt *pkt;
	int i, j;

	int decs[4];
	char *word_test[DEVS_MAX];

	word_test[0] = "DEADFACE";
	word_test[1] = "CAFEFADE";
	word_test[2] = "BEAUCADE";
	word_test[3] = "ACE0FCEA";

	memset(&pkts[0][0], 0, sizeof(pkts));

	for (i = 0; i < DEVS_MAX; i++) {

		init_pkt_data(decs, word_test[i]);
		//for(int i=0;i<4;i++)
			//printf("%d\n",decs[i]);

		grspw_list_clr(&devs[i].rx_list);
		grspw_list_clr(&devs[i].tx_list);
		grspw_list_clr(&devs[i].tx_buf_list);
		devs[i].rx_list_cnt = 0;
		devs[i].tx_list_cnt = 0;
		devs[i].tx_buf_list_cnt = 0;

		for (j = 0, pkt = &pkts[i][0]; j < DATA_MAX; j++, pkt = &pkts[i][j]) {
			pkt->p.pkt_id = (i << 8)+ j; /* unused */
			pkt->p.hdr = &pkt->hdr[0];
			pkt->p.data = &pkt->data[0];
			if (j < 120+8) {
				/* RX buffer */

				/* Add to device RX list */
				grspw_list_append(&devs[i].rx_list, &pkt->p);
				devs[i].rx_list_cnt++;
			} else {
				/* TX buffer */
				pkt->p.dlen = PKT_SIZE;
				/// From the doc:
				/// "Prototype: void * memset (void *block, int c, size_t size)
				/// Description:
				/// This function copies the value of c (converted to an unsigned char) into each of the first size bytes of the
				/// object beginning at block. It returns the value of block. "
				//memset(pkt->p.data+8, i, PKT_SIZE-4);
				for (int k = 0; k < pkt->p.dlen; k++)
				{
					//printf("addr = %d\n",&(pkt->p.data)+4+k);
					//memcpy(&(pkt->p.data)+4+k, &decs[k%4], 1);
					memset(pkt->p.data+2+k, decs[k%4], 1);

				}
				/* Add to device TX list */
				grspw_list_append(&devs[i].tx_buf_list, &pkt->p);
				devs[i].tx_buf_list_cnt++;
			}
		}
	}

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
			printf("GRSPW%d: Received %d packets\n", dev->index, cnt);
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
				printf(" of length %d bytes: ", pkt->dlen);
				/// PA : Ajout d'une boucle pour l'affichage (avant un seul printf)
				for(int i=0;i<pkt->dlen;i++)
					printf("0x%02x ", c[i]);
				printf("\n\n");
			}

			/* Reuse packet buffers by moving packets to rx_list */
			grspw_list_append_list(&dev->rx_list, &lst);
			dev->rx_list_cnt += cnt;
		}

		return 0;
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
			printf("GRSPW%d: Sending %d packets\n", dev->index,
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
				printf("...\n");
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

