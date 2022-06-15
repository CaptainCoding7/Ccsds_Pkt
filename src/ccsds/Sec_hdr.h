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

#define SEC_HDR_TC_SIZE 6
#define SEC_HDR_TM_SIZE 8


#ifdef __cplusplus

#include <ecl/WordBe.h>
#include <ecl/BitFieldInteger.h>
#include "Types.h"
#include "../debug_print.h"

using namespace ecl;
using namespace ecl::core::tmtc;

using ServiceTypeField = uintBe1_t;
using ServiceSubTypeField = uintBe1_t;
using SourceIdField = uintBe2_t;
using SpareField = uintBe1_t;

using ScTimeRefStatusValue = uint8_t;
using ServiceType = uint8_t;
using ServiceSubTypeValue = uint8_t;
using MsgTypeCounterValue = uint16_t;
using DestIdValue = uint16_t;
using TimeValue = uint8_t;


// ================================================================================================
// Sec_hdr_TC
// ------------------------------------------------------------------------------------------------

class Sec_hdr_TC : public uintBe6_t
{
public:

	constexpr Sec_hdr_TC(
		    TcAckFlags ackFlag,
			ServiceTypeValue serviceType,
		    ServiceSubTypeValue serviceSubType,
		    SourceIdValue sourceId,
			SpareValue spare);

	~Sec_hdr_TC();


	DECLARE_BIT(hasSecondaryHeader, 47)
	DECLARE_CASTED_FIELD(m_pus_version, 44, 3, PusVersion)
	DECLARE_CASTED_FIELD(m_ackFlags, 40, 4, TcAckFlagsValue)
	DECLARE_CASTED_FIELD(m_serviceType, 32, 8, ServiceTypeValue)
	DECLARE_CASTED_FIELD(m_serviceSubType, 24, 8, ServiceSubTypeValue)
	DECLARE_CASTED_FIELD(m_sourceId, 8, 16, SourceIdValue)
	DECLARE_CASTED_FIELD(m_spare, 0, 8, SpareValue)

};

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
	set_m_pus_version(2);
	set_m_ackFlags(ack.value());
	set_m_serviceType(serviceType);
	set_m_serviceSubType(serviceSubType);
	set_m_sourceId(sourceId);
	set_m_spare(spare);
}

Sec_hdr_TC::~Sec_hdr_TC()
{
	DBG(("sec_hdr tc destr\n"));
}

// ================================================================================================
// Sec_hdr_TM
// ------------------------------------------------------------------------------------------------


class Sec_hdr_TM : public uintBe8_t
{
public:

	constexpr Sec_hdr_TM(
		    ScTimeRefStatusValue scTimeRefStatus,
			ServiceTypeValue serviceType,
		    ServiceSubTypeValue serviceSubType,
			MsgTypeCounterValue msgTypeCounterValue,
		    DestIdValue destId,
			SpareValue spare);

	~Sec_hdr_TM();

	DECLARE_BIT(hasSecondaryHeader, 63)
	DECLARE_CASTED_FIELD(m_pus_version, 60, 3, PusVersion)
	DECLARE_CASTED_FIELD(m_scTimeRefStatus, 56, 4, ScTimeRefStatusValue)
	DECLARE_CASTED_FIELD(m_serviceType, 48, 8, ServiceTypeValue)
	DECLARE_CASTED_FIELD(m_serviceSubType, 40, 8, ServiceSubTypeValue)
	DECLARE_CASTED_FIELD(m_msgTypeCounter, 24, 16, MsgTypeCounterValue)
	DECLARE_CASTED_FIELD(m_destId, 8, 16, DestIdValue)
	//DECLARE_CASTED_FIELD(m_time, 8, 48, TimeValue)
	DECLARE_CASTED_FIELD(m_spare, 0, 8, SpareValue)

};


constexpr Sec_hdr_TM::Sec_hdr_TM(
		ScTimeRefStatusValue scTimeRefStatus=0,
		ServiceTypeValue serviceType=1,
	    ServiceSubTypeValue serviceSubType=1,
		MsgTypeCounterValue msgTypeCounterValue=10,
	    DestIdValue destId=2,
		SpareValue spare=0)
: uintBe8_t(0)

{

	set_hasSecondaryHeader(false);
	set_m_pus_version(2);
	set_m_scTimeRefStatus(scTimeRefStatus);
	set_m_serviceType(serviceType);
	set_m_serviceSubType(serviceSubType);
	set_m_msgTypeCounter(msgTypeCounterValue);
	set_m_destId(destId);
	set_m_spare(spare);

}

Sec_hdr_TM::~Sec_hdr_TM()
{
	DBG(("sec_hdr tm destr\n"));
}


#endif

#endif /* SEC_HDR_H_ */
