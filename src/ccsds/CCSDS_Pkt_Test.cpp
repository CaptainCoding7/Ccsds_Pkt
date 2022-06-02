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
	CCSDS_Pkt_TC *ccsds_pkt_tc = new CCSDS_Pkt_TC(*spw_hdr, *prim_hdr, *sec_hdr);

	print_size("spw_hdr", sizeof(*spw_hdr));
	print_size("prim_hdr", sizeof(*prim_hdr));
	print_size("sec_hdr", sizeof(*sec_hdr));
	print_size("ccsds_pkt_tc", sizeof(*ccsds_pkt_tc));
}


///\/\/\/\/\/\/\/\/\/\/\/\/ API - C/C++ functions  /\/\/\/\/\/\/\/\/\/\/\/\/\/\/


/*****************   Creation of C++ objects (+return) ********************/

//CCSDS_Pkt_TC *createCCSDS_Pkt()
extern "C" CCSDS_PKT create_CCSDS_Pkt(unsigned char dest_port_addr)
{
	return new CCSDS_Pkt_TC(dest_port_addr);
}

extern "C"  SPW_HDR create_spw_hdr()
{
    return new Spw_hdr(3,2, 0, 0);
}

extern "C" PRIM_HDR create_prim_hdr()
{
	return new Prim_hdr();
}
extern "C" SEC_HDR create_sec_hdr()
{
	return new Sec_hdr();
}

/****** delete  *****/

/// CCSDS_Pkt -------------------
extern "C" void delete_CCSDS_Pkt(CCSDS_PKT ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	delete pccsds_pkt;
}


/*****************  ... using smart pointer  ***********************/

//CCSDS_Pkt_TC *createCCSDS_Pkt()
/*
extern "C" void *create_sp_CCSDS_Pkt(unsigned char dest_port_addr)
{
	shared_ptr<CCSDS_Pkt_TC> ccsds_pkt_tc(new CCSDS_Pkt_TC(dest_port_addr));
	return ccsds_pkt_tc;

}
*/

/*****************   Getter calls ********************/

/// CCSDS_Pkt -------------------
extern "C" PRIM_HDR call_CCSDS_Pkt_get_prim_hdr(CCSDS_PKT ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_prim_hdr();
}
extern "C" SEC_HDR call_CCSDS_Pkt_get_sec_hdr(CCSDS_PKT ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_sec_hdr();
}

extern "C" SPW_HDR call_CCSDS_Pkt_get_spw_hdr(CCSDS_PKT ccsds_pkt)
{
	auto pccsds_pkt = reinterpret_cast<CCSDS_Pkt_TC*>(ccsds_pkt);
	return pccsds_pkt->get_spw_hdr();
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

/// Sec_hdr -----------------------
extern "C" uint8_t call_Sec_hdr_get_serviceType(SEC_HDR sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr*>(sec_hdr);
	return psec_hdr->getMServiceType();
}
extern "C" uint8_t call_Sec_hdr_get_serviceSubType(SEC_HDR sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr*>(sec_hdr);
	return psec_hdr->getMServiceSubType();
}
extern "C" uint8_t call_Sec_hdr_get_sourceId(SEC_HDR sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr*>(sec_hdr);
	return psec_hdr->getMSourceId();
}

extern "C" uint8_t call_Sec_hdr_get_ackflag(SEC_HDR sec_hdr)
{
	auto psec_hdr = reinterpret_cast<Sec_hdr*>(sec_hdr);
	return psec_hdr->getMAck();
}
