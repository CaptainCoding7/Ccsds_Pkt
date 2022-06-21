/*
 * CCSDS_Pkt_Test.cpp
 *
 *  Created on: 23 mai 2022
 *      Author: pa267340
 */

#include <iostream>

#include "CCSDS_Pkt.h"
#include "../debug_print.h"


using namespace ecl::core::tmtc;
using namespace std;


#ifdef __cplusplus
extern "C"
{
#endif

/** /\/\/\/\/\/\/\/ API - Wrapper functions implementation /\/\/\/\/\/\/\/\/\**/

/*****************   Creation of C++ objects (+return) ********************/


CCSDS_PKT_TC create_CCSDS_Pkt_TC(unsigned char dest_port_addr)
{
	/// **p = &p
	return {new CCSDS_Pkt_TC(dest_port_addr,2, 0, 0)};
}

CCSDS_PKT_TM create_CCSDS_Pkt_TM(unsigned char dest_port_addr)
{
	/// **p = &p
	return {new CCSDS_Pkt_TM(dest_port_addr,2, 0, 0)};
}

/****** delete  *****/

void delete_CCSDS_Pkt_TC(CCSDS_PKT_TC ccsds_pkt, int no)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	delete pccsds_pkt;
	DBG(("=> Packet n_%d has been deleted.\n",no+1));
}

void delete_CCSDS_Pkt_TM(CCSDS_PKT_TM ccsds_pkt, int no)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TM*>(ccsds_pkt);
	delete pccsds_pkt;
	DBG(("=> Packet n_%d has been deleted.\n",no+1));
}

/*****************   Getter calls ********************/

/// CCSDS_Pkt -------------------
SPW_HDR call_CCSDS_Pkt_get_spw_hdr(CCSDS_PKT ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt*>(ccsds_pkt);
	return pccsds_pkt->get_spw_hdr();
}
PRIM_HDR call_CCSDS_Pkt_get_prim_hdr(CCSDS_PKT ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt*>(ccsds_pkt);
	return pccsds_pkt->get_prim_hdr();
}
PKT_DATA_TC call_CCSDS_Pkt_TC_get_pkt_data(CCSDS_PKT_TC ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_pkt_data();
}
PKT_DATA_TM call_CCSDS_Pkt_TM_get_pkt_data(CCSDS_PKT_TM ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TM*>(ccsds_pkt);
	return pccsds_pkt->get_pkt_data();
}

/// Spw_hdr -------------------
unsigned char call_Spw_hdr_get_addr(SPW_HDR spw_hdr)
{
	auto pspw_hdr = reinterpret_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_addr();
}
unsigned char call_Spw_hdr_get_protid(SPW_HDR spw_hdr)
{
	auto pspw_hdr = reinterpret_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_protid();
}
unsigned char call_Spw_hdr_get_spare(SPW_HDR spw_hdr)
{
	auto pspw_hdr = reinterpret_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_spare();
}
unsigned char call_Spw_hdr_get_user_app(SPW_HDR spw_hdr)
{
	auto pspw_hdr = reinterpret_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_user_app();
}

/// Prim_hdr --------------------
enum Id call_Prim_hdr_get_id(PRIM_HDR prim_hdr)
{
	auto pprim_hdr = reinterpret_cast<Prim_hdr*>(prim_hdr);
	return pprim_hdr->get_ccsdsId_BE();
}
uint16_t call_Prim_hdr_get_counter(PRIM_HDR prim_hdr)
{
	auto pprim_hdr = reinterpret_cast<Prim_hdr*>(prim_hdr);
	return pprim_hdr->get_ccsdsCounter_BE();
}
uint16_t call_Prim_hdr_get_len(PRIM_HDR prim_hdr)
{
	auto pprim_hdr = reinterpret_cast<Prim_hdr*>(prim_hdr);
	return pprim_hdr->get_ccsdsPLength_BE();
}


/// CCSDS_Pkt_data_TC ----------------
SEC_HDR_TC call_Pkt_data_TC_get_sec_hdr(PKT_DATA_TC pkt_data)
{
	auto ppkt_data = reinterpret_cast<Pkt_data_TC*>(pkt_data);
	return ppkt_data->getMSec_hdr_tc();
}
uint8_t *call_Pkt_data_TC_get_app_data(PKT_DATA_TC pkt_data)
{
	auto ppkt_data = reinterpret_cast<Pkt_data<APP_DATA_TC_SIZE>*>(pkt_data);
	return ppkt_data->getMData();
}
uint16_t call_Pkt_data_TC_get_crc(PKT_DATA_TC pkt_data)
{
	auto ppkt_data = reinterpret_cast<Pkt_data<APP_DATA_TC_SIZE>*>(pkt_data);
	return ppkt_data->getMCrc();
}
/// CCSDS_Pkt_data_TM ----------------
SEC_HDR_TM call_Pkt_data_TM_get_sec_hdr(PKT_DATA_TM pkt_data)
{
	auto ppkt_data = reinterpret_cast<Pkt_data_TM*>(pkt_data);
	return ppkt_data->getMSec_hdr_tm();
}
uint8_t *call_Pkt_data_TM_get_app_data(PKT_DATA_TM pkt_data)
{
	auto ppkt_data = reinterpret_cast<Pkt_data<APP_DATA_TM_SIZE>*>(pkt_data);
	return ppkt_data->getMData();
}
uint16_t call_Pkt_data_TM_get_crc(PKT_DATA_TM pkt_data)
{
	auto ppkt_data = reinterpret_cast<Pkt_data<APP_DATA_TM_SIZE>*>(pkt_data);
	return ppkt_data->getMCrc();
}

/// Sec_hdr_TC -----------------------

int call_Sec_hdr_TC_get_pus_version(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	return psec_hdr->m_pus_version();
}
uint8_t call_Sec_hdr_TC_get_ackflag(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	return psec_hdr->m_ackFlags();
}
uint8_t call_Sec_hdr_TC_get_serviceType(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	return psec_hdr->m_serviceType();
}
uint8_t call_Sec_hdr_TC_get_serviceSubType(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	return psec_hdr->m_serviceSubType();
}
uint16_t call_Sec_hdr_TC_get_sourceId(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	return psec_hdr->m_sourceId();
}
uint8_t call_Sec_hdr_TC_get_spare(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	return psec_hdr->m_spare();
}

/// Sec_hdr_TM -----------------------

int call_Sec_hdr_TM_get_pus_version(SEC_HDR_TM sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TM*>(sec_hdr);
	return psec_hdr->m_pus_version();
}
int call_Sec_hdr_TM_get_scTimeRefStatus(SEC_HDR_TM sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TM*>(sec_hdr);
	return psec_hdr->m_scTimeRefStatus();
}
uint8_t call_Sec_hdr_TM_get_serviceType(SEC_HDR_TM sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TM*>(sec_hdr);
	return psec_hdr->m_serviceType();
}
uint8_t call_Sec_hdr_TM_get_serviceSubType(SEC_HDR_TM sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TM*>(sec_hdr);
	return psec_hdr->m_serviceSubType();
}
uint16_t call_Sec_hdr_TM_get_msgTypeCounter(SEC_HDR_TM sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TM*>(sec_hdr);
	return psec_hdr->m_msgTypeCounter();
}
uint16_t call_Sec_hdr_TM_get_destId(SEC_HDR_TM sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TM*>(sec_hdr);
	return psec_hdr->m_destId();
}
uint8_t call_Sec_hdr_TM_get_spare(SEC_HDR_TM sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TM*>(sec_hdr);
	return psec_hdr->m_spare();
}

#ifdef __cplusplus
}
#endif
