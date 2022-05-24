/*
 * CCSDS_Pkt.h
 *
 *  Created on: 23 mai 2022
 *      Author: pa267340
 */

#ifndef CCSDS_CCSDS_PKT_H_
#define CCSDS_CCSDS_PKT_H_


#ifdef __cplusplus

#include "ecl/core/tmtc/PktCcsds.h"
#include "ecl/core/tmtc/HeaderTc.h"

namespace ecl
{
namespace core
{
namespace tmtc
{

	class Spw_hdr
	{
	public:
		Spw_hdr(unsigned char addr,
				unsigned char protid,
				unsigned char spare = 0,
				unsigned char user_app = 0)
		:		m_addr(addr),
				m_protid(protid),
				m_spare(spare),
				m_user_app(user_app)
		{
		}

		unsigned char get_addr()
		{
			return m_addr;
		}

	private:
		unsigned char m_addr;
		unsigned char m_protid;
		unsigned char m_spare;
		unsigned char m_user_app;

	};


	class CCSDS_Pkt_TC
	{
	public:
		CCSDS_Pkt_TC(Spw_hdr spw_hdr,
				Prim_hdr prim_hdr,
				Sec_hdr sec_hdr)
		:		m_spw_hdr(spw_hdr),
				m_prim_hdr(prim_hdr),
				m_sec_hdr(sec_hdr)
		{
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

/*************************  FOR ACCESSING THROUGH C CODE  *******************/


typedef void* SPW_HDR;

#ifdef __cplusplus
extern "C"
{
#endif

//CCSDS_Pkt_TC *createCCSDS_Pkt();
SPW_HDR spw_hdr_new();
void createCCSDS_Pkt();
unsigned char call_Spw_hdr_get_addr(SPW_HDR spw_hdr);

#ifdef __cplusplus
}
#endif

/**********************************/


#endif /* CCSDS_CCSDS_PKT_H_ */
