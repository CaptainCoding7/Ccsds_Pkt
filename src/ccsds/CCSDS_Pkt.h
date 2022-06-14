/*
 * CCSDS_Pkt.h
 *
 *  Created on: 23 mai 2022
 *      Author: pa267340
 */

#ifndef CCSDS_CCSDS_PKT_H_
#define CCSDS_CCSDS_PKT_H_

#include "Pkt_data.h"
#include "Spw_hdr.h"
#include "Prim_hdr.h"


#ifdef __cplusplus

#include "../debug_print.h"

	class CCSDS_Pkt_TC
	{
	public:

		CCSDS_Pkt_TC(
				unsigned char addr,
				unsigned char protid,
				unsigned char spare,
				unsigned char user_app)
		:		m_spw_hdr(addr,protid,spare,user_app),
				m_prim_hdr(),
				m_pkt_data()
		{
		}

		void set_prim_hdr(Prim_hdr *ph)
		{
			m_prim_hdr = *ph;
		}

		Prim_hdr *get_prim_hdr()
		{
			return &m_prim_hdr;
		}

		void set_pkt_data(Pkt_data *pd)
		{
			m_pkt_data = *pd;
		}

		Pkt_data *get_pkt_data()
		{
			return &m_pkt_data;
		}

		void set_spw_hdr(Spw_hdr *spwh)
		{
			m_spw_hdr = *spwh;
		}


		Spw_hdr *get_spw_hdr()
		{
			return &m_spw_hdr;
		}

	private:
		Spw_hdr m_spw_hdr;
		Prim_hdr m_prim_hdr;
		Pkt_data m_pkt_data;
	};


#endif

/*************************  API - C/C++ functions  *******************/

typedef void* CCSDS_PKT;
typedef void* SPW_HDR;
typedef void* PRIM_HDR;
typedef void* PKT_DATA;
typedef void* SEC_HDR_TC;

typedef void* IDFIELD;
typedef void* COUNTERFIELD;
typedef void* LENFIELD;


#ifdef __cplusplus
extern "C"
{
#endif


/*** Sizes constants ***/

#define CCSDS_PKT_SIZE SPW_HDR_SIZE + PRIM_HDR_SIZE + PKT_DATA_SIZE
// dividing by 8 since long long = 64bits = 8 bytes
#define CCSDS_PKT_LONG_LONG_SIZE CCSDS_PKT_SIZE/8+1 // 518/8~64


/** Test functions **/
void test_create_CCSDS_Pkt();


/*** Creation of C++ objects (+return) ***/
CCSDS_PKT create_CCSDS_Pkt_TC(unsigned char dest_port_addr);

/// delete
void delete_CCSDS_Pkt(CCSDS_PKT ccsds_pkt, int no);


/*** Getter calls ***/
/// CCSDS_Pkt
SPW_HDR call_CCSDS_Pkt_get_spw_hdr(CCSDS_PKT ccsds_pkt);
PRIM_HDR call_CCSDS_Pkt_get_prim_hdr(CCSDS_PKT ccsds_pkt);
PKT_DATA call_CCSDS_Pkt_get_pkt_data(CCSDS_PKT ccsds_pkt);
/// SPW_HDR
unsigned char call_Spw_hdr_get_addr(SPW_HDR spw_hdr);
unsigned char call_Spw_hdr_get_protid(SPW_HDR spw_hdr);
unsigned char call_Spw_hdr_get_spare(SPW_HDR spw_hdr);
unsigned char call_Spw_hdr_get_user_app(SPW_HDR spw_hdr);
/// PRIM_HDR
enum Id call_Prim_hdr_get_id(PRIM_HDR prim_hdr);
uint16_t call_Prim_hdr_get_counter(PRIM_HDR prim_hdr);
uint16_t call_Prim_hdr_get_len(PRIM_HDR prim_hdr);
/// CCSDS_PKT_DATA
SEC_HDR_TC call_Pkt_data_get_sec_hdr(PKT_DATA pkt_data);
uint8_t *call_Pkt_data_get_app_data(PKT_DATA pkt_data);
uint16_t call_Pkt_data_get_crc(PKT_DATA pkt_data);
/// SEC_HDR
int call_Sec_hdr_get_pus_version(SEC_HDR_TC sec_hdr);
uint8_t call_Sec_hdr_get_ackflag(SEC_HDR_TC sec_hdr);
uint8_t call_Sec_hdr_get_serviceType(SEC_HDR_TC sec_hdr);
uint8_t call_Sec_hdr_get_serviceSubType(SEC_HDR_TC sec_hdr);
uint16_t call_Sec_hdr_get_sourceId(SEC_HDR_TC sec_hdr);
uint8_t call_Sec_hdr_get_spare(SEC_HDR_TC sec_hdr);

#ifdef __cplusplus
}
#endif

/**********************************/


#endif /* CCSDS_CCSDS_PKT_H_ */
