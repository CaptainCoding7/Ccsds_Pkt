/*
 * CCSDS_Pkt.h
 *
 *  Created on: 23 mai 2022
 *      Author: pa267340
 */

#ifndef CCSDS_CCSDS_PKT_H_
#define CCSDS_CCSDS_PKT_H_


#ifdef __cplusplus

//#include "ecl/core/tmtc/PktCcsds.h"
#include "ecl/core/tmtc/HeaderTc.h"
#include "Prim_hdr.h"
#include "Sec_hdr.h"
#include "Spw_hdr.h"


namespace ecl
{
namespace core
{
namespace tmtc
{

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
				m_sec_hdr()
		{
		}

		~CCSDS_Pkt_TC()
		{
		std::cout << "ccsds pkt destr" << std::endl;

		}

		void set_prim_hdr(Prim_hdr *ph)
		{
			m_prim_hdr = *ph;
		}

		Prim_hdr *get_prim_hdr()
		{
			return &m_prim_hdr;
		}

		void set_sec_hdr(Sec_hdr *sh)
		{
			m_sec_hdr = *sh;
		}


		Sec_hdr *get_sec_hdr()
		{
			return &m_sec_hdr;
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
		Sec_hdr m_sec_hdr;

	};

}
}
}

#endif

/*************************  API - C/C++ functions  *******************/


typedef void* CCSDS_PKT;
typedef void* SPW_HDR;
typedef void* PRIM_HDR;
typedef void* SEC_HDR;

typedef void* IDFIELD;
typedef void* COUNTERFIELD;
typedef void* LENFIELD;


#ifdef __cplusplus
extern "C"
{
#endif

// C compatible enum for TcAck
enum TcAck
{
	ACCEPTANCE,
	START,
	PROGRESS,
	COMPLETION,
	_COUNT,
	_BEGIN = ACCEPTANCE,
	_END = _COUNT
};

/** Test functions **/
void test_create_CCSDS_Pkt();


/*** Creation of C++ objects (+return) ***/
//CCSDS_PKT create_CCSDS_Pkt(SPW_HDR spw_hdr,PRIM_HDR prim_hdr,SEC_HDR sec_hdr);
CCSDS_PKT create_CCSDS_Pkt(unsigned char dest_port_addr);
SPW_HDR create_spw_hdr(unsigned char dest_port_addr);
PRIM_HDR create_prim_hdr();
SEC_HDR create_sec_hdr();

/// delete
void delete_CCSDS_Pkt(CCSDS_PKT ccsds_pkt);

/// sp
//void *create_sp_CCSDS_Pkt(unsigned char dest_port_addr);


/*** Getter calls ***/
/// CCSDS_Pkt
PRIM_HDR call_CCSDS_Pkt_get_prim_hdr(CCSDS_PKT ccsds_pkt);
SEC_HDR call_CCSDS_Pkt_get_sec_hdr(CCSDS_PKT ccsds_pkt);
SPW_HDR call_CCSDS_Pkt_get_spw_hdr(CCSDS_PKT ccsds_pkt);
/// SPW_HDR
unsigned char call_Spw_hdr_get_addr(SPW_HDR spw_hdr);
unsigned char call_Spw_hdr_get_protid(SPW_HDR spw_hdr);
unsigned char call_Spw_hdr_get_spare(SPW_HDR spw_hdr);
unsigned char call_Spw_hdr_get_user_app(SPW_HDR spw_hdr);
/// PRIM_HDR
enum Id call_Prim_hdr_get_id(PRIM_HDR prim_hdr);
uint16_t call_Prim_hdr_get_counter(PRIM_HDR prim_hdr);
uint16_t call_Prim_hdr_get_len(PRIM_HDR prim_hdr);
/// SEC_HDR
uint8_t call_Sec_hdr_get_serviceType(SEC_HDR sec_hdr);
uint8_t call_Sec_hdr_get_serviceSubType(SEC_HDR sec_hdr);
uint8_t call_Sec_hdr_get_sourceId(SEC_HDR sec_hdr);
uint8_t call_Sec_hdr_get_ackflag(SEC_HDR sec_hdr);

#ifdef __cplusplus
}
#endif

/**********************************/


#endif /* CCSDS_CCSDS_PKT_H_ */
