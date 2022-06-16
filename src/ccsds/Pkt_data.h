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
// Pkt_data parent class
// ------------------------------------------------------------------------------------------------

template <size_t app_data_size>
class Pkt_data
{
public:
	Pkt_data()
	:m_crc{77}
	{
		for (int i = 0; i < app_data_size; i++) {
			m_app_data[i] = i%10;
		}
		DBG(("\n app_data size = %d\n",sizeof(m_app_data)));
	}
	~Pkt_data()
	{
		DBG(("pkt_data base destr\n"));
	}
	uint16_t getMCrc() {
		return m_crc;
	}
	void setMCrc(uint16_t mCrc) {
		m_crc = mCrc;
	}
	uint8_t *getMData() {
		return m_app_data;
	}

private:
	uint8_t m_app_data[app_data_size];
	uint16_t m_crc;
	// sec_hdr in derived class
};

// ================================================================================================
// Pkt_data_TC derived class
// ------------------------------------------------------------------------------------------------

class Pkt_data_TC : Pkt_data<APP_DATA_TC_SIZE>
{
public:

	Pkt_data_TC()
	:Pkt_data{},
	 m_sec_hdr_tc{}
	{}

	~Pkt_data_TC()
	{
		DBG(("pkt_data TC destr\n"));
	}
	void setMSec_hdr_tc(Sec_hdr_TC *sh)
	{
		m_sec_hdr_tc = *sh;
	}

	Sec_hdr_TC *getMSec_hdr_tc()
	{
		return &m_sec_hdr_tc;
	}

private:
	Sec_hdr_TC m_sec_hdr_tc;
};


// ================================================================================================
// Pkt_data_TM derived class
// ------------------------------------------------------------------------------------------------

class Pkt_data_TM : Pkt_data<APP_DATA_TM_SIZE>
{
public:

	Pkt_data_TM()
	:Pkt_data{},
	 m_sec_hdr_tm{}
	{}

	~Pkt_data_TM()
	{
		DBG(("pkt_data TM destr\n"));
	}
	void setMSec_hdr_tm(Sec_hdr_TM *sh)
	{
		m_sec_hdr_tm = *sh;
	}

	Sec_hdr_TM *getMSec_hdr_tm()
	{
		return &m_sec_hdr_tm;
	}

private:
	Sec_hdr_TM m_sec_hdr_tm;
};



#endif

#endif /* PKT_DATA_H_ */
