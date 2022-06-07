/*
 * CCSDS_Pkt_Test.cpp
 *
 *  Created on: 23 mai 2022
 *      Author: pa267340
 */

#include <iostream>

#include "HeaderTmSb.h"
#include "PktCcsdsTc.h"
#include "PktCcsdsTmSb.h"
#include "TrailerTc.h"
#include "Types.h"
#include "Apid.h"

#include "Prim_hdr.h"
#include "Sec_hdr.h"
#include "CCSDS_Pkt.h"


using namespace ecl::core::tmtc;
using namespace std;

/*******************  TEST FUNCTIONS *******************/

void print_size(string s, int size)
{
	cout << "Size of " << s << " = " << size << " bytes." << endl;
}

extern "C" void test_create_CCSDS_Pkt()
{
	Spw_hdr *spw_hdr = new Spw_hdr(3, 2, 0, 0);
	Prim_hdr *prim_hdr = new Prim_hdr();
	//enum TcAck ack;
	//TcAckFlags ackFlags = new ecl::FlagSet<TcAck>();
	TcAckFlags ackFlags;
	Sec_hdr *sec_hdr = new Sec_hdr(1, 1, 1, 0);//ackFlags);

	//CCSDS_Pkt_TC *ccsds_pkt_tc = new CCSDS_Pkt_TC(*spw_hdr, *prim_hdr, *sec_hdr);
	CCSDS_Pkt_TC *ccsds_pkt_tc = new CCSDS_Pkt_TC(3, 2, 0, 0);

	print_size("spw_hdr", sizeof(*spw_hdr));
	print_size("prim_hdr", sizeof(*prim_hdr));
	print_size("sec_hdr", sizeof(*sec_hdr));
	print_size("ccsds_pkt_tc", sizeof(*ccsds_pkt_tc));
}


///\/\/\/\/\/\/\/\/\/\/\/\/ API - C/C++ functions  /\/\/\/\/\/\/\/\/\/\/\/\/\/\/


/*****************   Creation of C++ objects (+return) ********************/


/// UPDATE: Use of smart pointers=> need intermediary functions to actually create the smart pointer
unique_ptr<CCSDS_Pkt_TC> get_CCSDS_Pkt_sp(unsigned char dest_port_addr) {
	return make_unique<CCSDS_Pkt_TC>(dest_port_addr,2,0,0);
	//unique_ptr<CCSDS_Pkt_TC> ccsds_pkt(new CCSDS_Pkt_TC(dest_port_addr, 2, 0,0));
	//return ccsds_pkt;
}

extern "C" CCSDS_PKT create_CCSDS_Pkt(unsigned char dest_port_addr)
{
	/// **p = &p
	//return new CCSDS_Pkt_TC(*pspw_hdr,*pprim_hdr,*psec_hdr);
	auto pkt = get_CCSDS_Pkt_sp(dest_port_addr);
    //return (new unique_ptr<CCSDS_Pkt_TC>( get_CCSDS_Pkt_sp(dest_port_addr)))->get();
	return pkt.release();
}


/*****************   Getter calls ********************/

/// CCSDS_Pkt -------------------
extern "C" SPW_HDR call_CCSDS_Pkt_get_spw_hdr(CCSDS_PKT ccsds_pkt)
{
	//auto &pccsds_pkt = *static_cast<unique_ptr<CCSDS_Pkt_TC>*>(ccsds_pkt);
	auto pccsds_pkt = static_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_spw_hdr();
}
extern "C" PRIM_HDR call_CCSDS_Pkt_get_prim_hdr(CCSDS_PKT ccsds_pkt)
{
	//auto &pccsds_pkt = *static_cast<unique_ptr<CCSDS_Pkt_TC>*>(ccsds_pkt);
	auto pccsds_pkt = static_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_prim_hdr();
}
extern "C" SEC_HDR call_CCSDS_Pkt_get_sec_hdr(CCSDS_PKT ccsds_pkt)
{
	//auto &pccsds_pkt = *static_cast<unique_ptr<CCSDS_Pkt_TC>*>(ccsds_pkt);
	auto pccsds_pkt = static_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_sec_hdr();
}

/// Spw_hdr -------------------
extern "C" unsigned char call_Spw_hdr_get_addr(SPW_HDR spw_hdr)
{
	auto pspw_hdr = static_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_addr();
}
extern "C" unsigned char call_Spw_hdr_get_protid(SPW_HDR spw_hdr)
{
	auto pspw_hdr = static_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_protid();

}
extern "C" unsigned char call_Spw_hdr_get_spare(SPW_HDR spw_hdr)
{
	auto pspw_hdr = static_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_spare();

}
extern "C" unsigned char call_Spw_hdr_get_user_app(SPW_HDR spw_hdr)
{
	auto pspw_hdr = static_cast<Spw_hdr*>(spw_hdr);
	return pspw_hdr->get_user_app();

}

/// Prim_hdr --------------------
extern "C" enum Id call_Prim_hdr_get_id(PRIM_HDR prim_hdr)
{
	auto pprim_hdr = static_cast<Prim_hdr*>(prim_hdr);
	//return static_cast<IDFIELD*>(pprim_hdr->get_ccsdsId_BE());
	return pprim_hdr->get_ccsdsId_BE();
}
extern "C" uint16_t call_Prim_hdr_get_counter(PRIM_HDR prim_hdr)
{
	auto pprim_hdr = static_cast<Prim_hdr*>(prim_hdr);
	//return static_cast<COUNTERFIELD*>(pprim_hdr->get_ccsdsCounter_BE());
	return pprim_hdr->get_ccsdsCounter_BE();
}
extern "C" uint16_t call_Prim_hdr_get_len(PRIM_HDR prim_hdr)
{
	auto pprim_hdr = static_cast<Prim_hdr*>(prim_hdr);
	//return static_cast<LENFIELD*>(pprim_hdr->get_ccsdsPLength_BE());
	return pprim_hdr->get_ccsdsPLength_BE();
}

/// Sec_hdr -----------------------
extern "C" uint8_t call_Sec_hdr_get_serviceType(SEC_HDR sec_hdr)
{
	auto psec_hdr = static_cast<Sec_hdr*>(sec_hdr);
	return psec_hdr->getMServiceType();
}
extern "C" uint8_t call_Sec_hdr_get_serviceSubType(SEC_HDR sec_hdr)
{
	auto psec_hdr = static_cast<Sec_hdr*>(sec_hdr);
	return psec_hdr->getMServiceSubType();
}
extern "C" uint8_t call_Sec_hdr_get_sourceId(SEC_HDR sec_hdr)
{
	auto psec_hdr = static_cast<Sec_hdr*>(sec_hdr);
	return psec_hdr->getMSourceId();
}

extern "C" uint8_t call_Sec_hdr_get_ackflag(SEC_HDR sec_hdr)
{
	auto psec_hdr = static_cast<Sec_hdr*>(sec_hdr);
	return psec_hdr->getMAck();
}
