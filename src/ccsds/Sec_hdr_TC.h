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


class Sec_hdr_TC
{
public:

	//using AckField = uintBe1_t;
	using ServiceTypeField = uintBe1_t;
	using ServiceSubTypeField = uintBe1_t;
	using SourceIdField = uintBe2_t;
	using SpareField = uintBe1_t;

	Sec_hdr_TC(
		    TcAckFlags ackFlag,
			ServiceTypeValue serviceType,
		    ServiceSubTypeValue serviceSubType,
		    SourceIdValue sourceId,
			SpareValue spare);

	~Sec_hdr_TC();

	TcAckFlagsValue getMAck() {
		return m_ack.value();
	}

	void setMAck(TcAckFlagsValue mAck) {
		m_ack.setValue(mAck);
	}

	ServiceSubTypeValue getMServiceSubType() const {
		return m_serviceSubType.value();
	}

	void setMServiceSubType(ServiceSubTypeValue mServiceSubType) {
		m_serviceSubType.setValue(mServiceSubType);
	}

	ServiceTypeValue getMServiceType() const {
		return m_serviceType.value();
	}

	void setMServiceType(ServiceTypeValue mServiceType) {
		m_serviceType.setValue(mServiceType);
	}

	SourceIdValue getMSourceId() const {
		return m_sourceId.value();
	}

	void setMSourceId(SourceIdValue mSourceId) {
		m_sourceId.setValue(mSourceId);
	}

	SpareValue getMSpare() const {
		return m_spare;
	}

	void setMSpare(SpareValue mSpare) {
		m_spare = mSpare;
	}

private:
	//AckField m_ack;
	TcAckFlags m_ack;
	ServiceTypeField m_serviceType;
	ServiceSubTypeField m_serviceSubType;
	SourceIdField m_sourceId;
	SpareField m_spare;
};


// ================================================================================================
// Sec_hdr_TC inline methods bodies
// ------------------------------------------------------------------------------------------------

/// We add some default values
Sec_hdr_TC::Sec_hdr_TC(//TcAckValue ack = 0,
		   	   	   	   TcAckFlags ack= TcAck::START,
					   ServiceTypeValue serviceType = 1,
                       ServiceSubTypeValue serviceSubType = 1,
                       SourceIdValue sourceId = 1,
					   SpareValue spare = 0)
{

	setMAck(ack.value());
	setMServiceSubType(serviceSubType);
	setMServiceType(serviceType);
	setMSourceId(sourceId);
	setMSpare(spare);

	DBG(("size of m_ack = %d\n",sizeof(m_ack)));
}

Sec_hdr_TC::~Sec_hdr_TC()
{
	DBG(("sec_hdr destr\n"));
}

#endif

#endif /* SEC_HDR_H_ */
