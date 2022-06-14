/**
 * @file Sec_hdr_TC.h
 * @date 25 juin 2020
 * @author fchateau
 * @note
 *
 * Contributors: fchateau
 *
 * This file is part of the UGTS software project.
 *
 * @copyright © Commissariat a l'Energie Atomique et aux Energies Alternatives (CEA)
 *
 *
 * PREVIOUSLY NAMED HeaderTc.h
 *
 */

#ifndef SEC_HDR_H_
#define SEC_HDR_H_

#define SEC_HDR_SIZE 6

#ifdef __cplusplus

#include <ecl/WordBe.h>
#include <ecl/BitFieldInteger.h>
#include "Types.h"
#include "../debug_print.h"

using namespace ecl;
using namespace ecl::core::tmtc;


class Sec_hdr_TC : public uintBe6_t
{
public:

	//using AckField = uintBe1_t;
	using ServiceTypeField = uintBe1_t;
	using ServiceSubTypeField = uintBe1_t;
	using SourceIdField = uintBe2_t;
	using SpareField = uintBe1_t;

	constexpr Sec_hdr_TC(
		    TcAckFlags ackFlag,
			ServiceTypeValue serviceType,
		    ServiceSubTypeValue serviceSubType,
		    SourceIdValue sourceId,
			SpareValue spare);

	~Sec_hdr_TC();


	DECLARE_BIT(hasSecondaryHeader, 47)
	DECLARE_CASTED_FIELD(m_version, 44, 3, PusVersion)
	DECLARE_CASTED_FIELD(m_ackFlags, 40, 4, TcAckFlagsValue)
	DECLARE_CASTED_FIELD(m_serviceType, 32, 8, ServiceTypeValue)
	DECLARE_CASTED_FIELD(m_serviceSubType, 24, 8, ServiceSubTypeValue)
	DECLARE_CASTED_FIELD(m_sourceId, 8, 16, SourceIdValue)
	DECLARE_CASTED_FIELD(m_spare, 0, 8, SpareValue)

private:
//	//AckField m_ack;
//	TcAckFlags m_ack;
//	ServiceTypeField m_serviceType;
//	ServiceSubTypeField m_serviceSubType;
//	SourceIdField m_sourceId;
//	SpareField m_spare;


};


// ================================================================================================
// Sec_hdr_TC inline methods bodies
// ------------------------------------------------------------------------------------------------

/// We add some default values
constexpr Sec_hdr_TC::Sec_hdr_TC(//TcAckValue ack = 0,
		   	   	   	   TcAckFlags ack= TcAck::START,
					   ServiceTypeValue serviceType = 1,
                       ServiceSubTypeValue serviceSubType = 1,
                       SourceIdValue sourceId = 1,
					   SpareValue spare = 0)
: uintBe6_t(0)

{

	set_hasSecondaryHeader(false);
	set_m_version(2);
	set_m_ackFlags(ack.value());
	set_m_serviceType(serviceType);
	set_m_serviceSubType(serviceSubType);
	set_m_sourceId(sourceId);

	DBG(("size of m_ack = %d\n",sizeof(m_ack)));
}

Sec_hdr_TC::~Sec_hdr_TC()
{
	DBG(("sec_hdr destr\n"));
}

#endif

#endif /* SEC_HDR_H_ */
