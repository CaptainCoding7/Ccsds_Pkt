/*
 * Pkt_data.h
 *
 *  Created on: 9 juin 2022
 *      Author: pa267340
 */

#ifndef PKT_DATA_H_
#define PKT_DATA_H_

#define APP_DATA_SIZE 500
#define PKT_DATA_SIZE SEC_HDR_SIZE + APP_DATA_SIZE + 2 // (CRC size)

#include "Sec_hdr_TC.h"


#ifdef __cplusplus

#include <ecl/WordBe.h>
#include <ecl/BitFieldInteger.h>
#include "Types.h"
#include "../debug_print.h"


class Pkt_data
{
public:


	Pkt_data();

	~Pkt_data();

	uint16_t getMCrc() {
		return m_crc;
	}

	void setMCrc(uint16_t mCrc) {
		m_crc = mCrc;
	}

	uint8_t *getMData() {
		return m_app_data;
	}

	void setMSec_hdr(Sec_hdr_TC *sh)
	{
		m_sec_hdr = *sh;
	}

	Sec_hdr_TC *getMSec_hdr()
	{
		return &m_sec_hdr;
	}

	void generateData();

private:
	Sec_hdr_TC m_sec_hdr;
	uint8_t m_app_data[APP_DATA_SIZE];
	uint16_t m_crc;

};


void Pkt_data::generateData()
{
	for (int i = 0; i < APP_DATA_SIZE; i++) {
		m_app_data[i] = i%10;
	}
	DBG(("\n app_data size = %d\n",sizeof(m_app_data)));
}

Pkt_data::Pkt_data():
		m_sec_hdr()
{
	generateData();
	setMCrc(77);
}

Pkt_data::~Pkt_data()
{
	DBG(("pkt_data destr\n"));
}
#endif


#endif /* PKT_DATA_H_ */
