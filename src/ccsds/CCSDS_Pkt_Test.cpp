/*
 * CCSDS_Pkt_Test.cpp
 *
 *  Created on: 23 mai 2022
 *      Author: pa267340
 */

#include <iostream>

//#include "HeaderTmSb.h"
//#include "PktCcsdsTc.h"
//#include "PktCcsdsTmSb.h"
//#include "TrailerTc.h"
#include "Types.h"
#include "Apid.h"

#include "Prim_hdr.h"
#include "CCSDS_Pkt.h"
#include "../debug_print.h"
#include "Sec_hdr.h"


using namespace ecl::core::tmtc;
using namespace std;

/*******************  TEST FUNCTIONS *******************/

void print_size(string s, int size)
{
	cout << "Size of " << s << " = " << size << " bytes." << endl;
	DBG(("Size of %s = %d bytes.", s, size));
}

extern "C" void test_create_CCSDS_Pkt()
{
	Spw_hdr *spw_hdr = new Spw_hdr(3, 2, 0, 0);
	Prim_hdr *prim_hdr = new Prim_hdr();
	//TcAck ack = 0;
	//TcAckFlags ackFlags = new ecl::FlagSet<TcAck>();
	TcAckFlags ackFlags;
	Sec_hdr_TC *sec_hdr = new Sec_hdr_TC(ackFlags,0, 1, 1, 1);//ackFlags);

	//CCSDS_Pkt_TC *ccsds_pkt_tc = new CCSDS_Pkt_TC(*spw_hdr, *prim_hdr, *sec_hdr);
	CCSDS_Pkt_TC *ccsds_pkt_tc = new CCSDS_Pkt_TC(3, 2, 0, 0);

	print_size("spw_hdr", sizeof(*spw_hdr));
	print_size("prim_hdr", sizeof(*prim_hdr));
	print_size("sec_hdr", sizeof(*sec_hdr));
	print_size("ccsds_pkt_tc", sizeof(*ccsds_pkt_tc));
}


///\/\/\/\/\/\/\/\/\/\/\/\/ API - C/C++ functions  /\/\/\/\/\/\/\/\/\/\/\/\/\/\/


/*****************   Creation of C++ objects (+return) ********************/


extern "C" CCSDS_PKT create_CCSDS_Pkt_TC(unsigned char dest_port_addr)
{
	/// **p = &p
	return {new CCSDS_Pkt_TC(dest_port_addr,2, 0, 0)};
}

extern "C" CCSDS_PKT create_CCSDS_Pkt_TM(unsigned char dest_port_addr)
{
	/// **p = &p
	return {new CCSDS_Pkt_TM(dest_port_addr,2, 0, 0)};
}

/****** delete  *****/

extern "C" void delete_CCSDS_Pkt(CCSDS_PKT ccsds_pkt, int no)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	delete pccsds_pkt;
	DBG(("=> Packet n_%d has been deleted.\n",no+1));
}


/*****************   Getter calls ********************/

/// CCSDS_Pkt -------------------
extern "C" SPW_HDR call_CCSDS_Pkt_get_spw_hdr(CCSDS_PKT ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_spw_hdr();
}
extern "C" PRIM_HDR call_CCSDS_Pkt_get_prim_hdr(CCSDS_PKT ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_prim_hdr();
}
extern "C" PKT_DATA call_CCSDS_Pkt_get_pkt_data(CCSDS_PKT ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_pkt_data();
}


/// Spw_hdr -------------------
extern "C" unsigned char call_Spw_hdr_get_addr(SPW_HDR spw_hdr)
{
	auto pspw_hdr = reinterpret_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_addr();
}
extern "C" unsigned char call_Spw_hdr_get_protid(SPW_HDR spw_hdr)
{
	auto pspw_hdr = reinterpret_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_protid();
}
extern "C" unsigned char call_Spw_hdr_get_spare(SPW_HDR spw_hdr)
{
	auto pspw_hdr = reinterpret_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_spare();
}
extern "C" unsigned char call_Spw_hdr_get_user_app(SPW_HDR spw_hdr)
{
	auto pspw_hdr = reinterpret_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_user_app();
}

/// Prim_hdr --------------------
extern "C" enum Id call_Prim_hdr_get_id(PRIM_HDR prim_hdr)
{
	auto pprim_hdr = reinterpret_cast<Prim_hdr*>(prim_hdr);
	//return reinterpret_cast<IDFIELD*>(pprim_hdr->get_ccsdsId_BE());
	return pprim_hdr->get_ccsdsId_BE();
}
extern "C" uint16_t call_Prim_hdr_get_counter(PRIM_HDR prim_hdr)
{
	auto pprim_hdr = reinterpret_cast<Prim_hdr*>(prim_hdr);
	//return reinterpret_cast<COUNTERFIELD*>(pprim_hdr->get_ccsdsCounter_BE());
	return pprim_hdr->get_ccsdsCounter_BE();
}
extern "C" uint16_t call_Prim_hdr_get_len(PRIM_HDR prim_hdr)
{
	auto pprim_hdr = reinterpret_cast<Prim_hdr*>(prim_hdr);
	//return reinterpret_cast<LENFIELD*>(pprim_hdr->get_ccsdsPLength_BE());
	return pprim_hdr->get_ccsdsPLength_BE();
}


/// CCSDS_Pkt_data ----------------
extern "C" SEC_HDR_TC call_Pkt_data_get_sec_hdr(PKT_DATA pkt_data)
{
	auto ppkt_data = reinterpret_cast<Pkt_data_TC*>(pkt_data);
	return ppkt_data->getMSec_hdr();
}
extern "C" uint8_t *call_Pkt_data_get_app_data(PKT_DATA pkt_data)
{
	auto ppkt_data = reinterpret_cast<Pkt_data_TC*>(pkt_data);
	return ppkt_data->getMData();
}
extern "C" uint16_t call_Pkt_data_get_crc(PKT_DATA pkt_data)
{
	auto ppkt_data = reinterpret_cast<Pkt_data_TC*>(pkt_data);
	return ppkt_data->getMCrc();
}


/// Sec_hdr_TC -----------------------

extern "C" int call_Sec_hdr_get_pus_version(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	//return psec_hdr->getMAck();
	//printf("size of sec_hdr = %d\n", sizeof(psec_hdr));
	return psec_hdr->m_version();
}
extern "C" uint8_t call_Sec_hdr_get_ackflag(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	//return psec_hdr->getMAck();
	return psec_hdr->m_ackFlags();
}
extern "C" uint8_t call_Sec_hdr_get_serviceType(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	//return psec_hdr->getMServiceType();
	return psec_hdr->m_serviceType();
}
extern "C" uint8_t call_Sec_hdr_get_serviceSubType(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	//return psec_hdr->getMServiceSubType();
	return psec_hdr->m_serviceSubType();
}
extern "C" uint16_t call_Sec_hdr_get_sourceId(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	//return psec_hdr->getMSourceId();
	return psec_hdr->m_sourceId();
}
extern "C" uint8_t call_Sec_hdr_get_spare(SEC_HDR_TC sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr_TC*>(sec_hdr);
	//return psec_hdr->getMSpare();
	return psec_hdr->m_spare();
}

