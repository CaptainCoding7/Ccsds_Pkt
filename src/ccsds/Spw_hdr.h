/*
 * Spw_hdr.h
 *
 *  Created on: 31 mai 2022
 *      Author: pa267340
 */

#ifndef CCSDS_SPW_HDR_H_
#define CCSDS_SPW_HDR_H_

#define SPW_HDR_SIZE 4

#ifdef __cplusplus

#include "../debug_print.h"


	class Spw_hdr
	{
	public:
		Spw_hdr();
		Spw_hdr(unsigned char addr,
				unsigned char protid,
				unsigned char spare,
				unsigned char user_app);
		~Spw_hdr();

		unsigned char get_addr()
		{
			return m_addr;
		}

		unsigned char get_protid()
		{
			return m_protid;
		}

		unsigned char get_spare()
		{
			return m_spare;
		}

		unsigned char get_user_app()
		{
			return m_user_app;
		}

		void set_addr(unsigned char mAddr) {
			m_addr = mAddr;
		}

		void set_protid(unsigned char mProtid) {
			m_protid = mProtid;
		}

		void set_spare(unsigned char mSpare) {
			m_spare = mSpare;
		}

		void set_user_app(unsigned char mUserApp) {
			m_user_app = mUserApp;
		}

	private:
		unsigned char m_addr;
		unsigned char m_protid;
		unsigned char m_spare;
		unsigned char m_user_app;

	};

//////////////////

Spw_hdr::Spw_hdr()
	{
		set_addr(3);
		set_protid(2);
		set_spare(0);
		set_user_app(0);
	}

Spw_hdr::Spw_hdr(unsigned char addr=6,
			unsigned char protid=2,
			unsigned char spare=0,
			unsigned char user_app=0)
	:		m_addr(addr),
			m_protid(protid),
			m_spare(spare),
			m_user_app(user_app)
	{
	}

Spw_hdr::~Spw_hdr()
{
	DBG(("spw_hdr destr\n"));
}

#endif

#endif /* CCSDS_SPW_HDR_H_ */
