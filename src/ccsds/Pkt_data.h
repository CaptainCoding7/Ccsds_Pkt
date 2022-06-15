/*
 * Pkt_data_TC.h
 *
 *  Created on: 9 juin 2022
 *      Author: pa267340
 */

#ifndef PKT_DATA_H_
#define PKT_DATA_H_

/// /!\ MAX 978 bytes
#define APP_DATA_TC_SIZE 500
#define PKT_DATA_TC_SIZE SEC_HDR_TC_SIZE + APP_DATA_TC_SIZE + 2 /// (CRC size)

/// /!\ MAX 4076 bytes
#define APP_DATA_TM_SIZE 2000
#define PKT_DATA_TM_SIZE SEC_HDR_TM_SIZE + APP_DATA_TM_SIZE + 2 /// (CRC size)

#include "Sec_hdr.h"


#ifdef __cplusplus

#include <ecl/WordBe.h>
#include <ecl/BitFieldInteger.h>
#include "Types.h"
#include "../debug_print.h"

// ================================================================================================
// Pkt_data_TC
// ------------------------------------------------------------------------------------------------

class Pkt_data_TC
{
public:


	Pkt_data_TC();

	~Pkt_data_TC();

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
	uint8_t m_app_data[APP_DATA_TC_SIZE];
	uint16_t m_crc;

};


void Pkt_data_TC::generateData()
{
	for (int i = 0; i < APP_DATA_TC_SIZE; i++) {
		m_app_data[i] = i%10;
	}
	DBG(("\n app_data size = %d\n",sizeof(m_app_data)));
}

Pkt_data_TC::Pkt_data_TC():
		m_sec_hdr()
{
	generateData();
	setMCrc(77);
}

Pkt_data_TC::~Pkt_data_TC()
{
	DBG(("pkt_data destr\n"));
}

// ================================================================================================
// Pkt_data_TM
// ------------------------------------------------------------------------------------------------

class Pkt_data_TM
{
public:


	Pkt_data_TM();

	~Pkt_data_TM();

	uint16_t getMCrc() {
		return m_crc;
	}

	void setMCrc(uint16_t mCrc) {
		m_crc = mCrc;
	}

	uint8_t *getMData() {
		return m_app_data;
	}

	void setMSec_hdr(Sec_hdr_TM *sh)
	{
		m_sec_hdr = *sh;
	}

	Sec_hdr_TM *getMSec_hdr()
	{
		return &m_sec_hdr;
	}

	void generateData();

private:
	Sec_hdr_TM m_sec_hdr;
	uint8_t m_app_data[APP_DATA_TM_SIZE];
	uint16_t m_crc;

};


void Pkt_data_TM::generateData()
{
	for (int i = 0; i < APP_DATA_TM_SIZE; i++) {
		m_app_data[i] = i%10;
	}
	DBG(("\n app_data size = %d\n",sizeof(m_app_data)));
}

Pkt_data_TM::Pkt_data_TM():
		m_sec_hdr()
{
	generateData();
	setMCrc(77);
}





#endif

#endif /* PKT_DATA_H_ */
