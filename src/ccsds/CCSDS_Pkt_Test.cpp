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

#include "Prim_hdr.h"
#include "Sec_hdr.h"
#include "CCSDS_Pkt.h"

#include "CCSDS_Pkt_Test.h"


using namespace ecl::core::tmtc;
using namespace std;

void print_size(string s, int size)
{
	cout << "Size of " << s << " = " << size << " bytes." << endl;
}


void createCCSDS_Pkt()
{

	Spw_hdr *spw_hdr = new Spw_hdr(3, 2);
	Prim_hdr *prim_hdr = new Prim_hdr(ID_TC, 1, 500, CcsdsGrouping::STANDALONE);
	TcAckFlags ack;
	Sec_hdr *sec_hdr = new Sec_hdr(1, 1, 1, ack);

	CCSDS_Pkt_TC *ccsds_pkt_tc = new CCSDS_Pkt_TC(*spw_hdr, *prim_hdr, *sec_hdr);

	print_size("spw_hdr", sizeof(*spw_hdr));
	print_size("prim_hdr", sizeof(*prim_hdr));
	print_size("sec_hdr", sizeof(*sec_hdr));
	print_size("ccsds_pkt_tc", sizeof(*ccsds_pkt_tc));

}
