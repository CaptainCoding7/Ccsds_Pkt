/**
 * @file Sec_hdr.h
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

#ifndef ECL_CORE_TMTC_HEADERTC_H_
#define ECL_CORE_TMTC_HEADERTC_H_

#include <ecl/WordBe.h>
#include <ecl/BitFieldInteger.h>
#include "Types.h"
#include "../debug_print.h"

namespace ecl
{
namespace core
{
namespace tmtc
{

class Sec_hdr
{
public:

	using ServiceTypeField = uintBe1_t;
	using ServiceSubTypeField = uintBe1_t;
	using SourceIdField = uintBe1_t;

	Sec_hdr(ServiceTypeValue serviceType,
		    ServiceSubTypeValue serviceSubType,
		    SourceIdValue sourceId,
		    //TcAck ack);
		    //TcAckFlags ack);
		    int ackFlag);

	~Sec_hdr();

	//const TcAckFlags& getMAck() const {
	//const TcAck& getMAck() const {
	int getMAck() {
		return m_ack;
	}

	//void (const TcAckFlags &mAck) {
	//void setMAck(const TcAck &mAck) {
	void setMAck(int mAck){
		m_ack = mAck;
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

private:
	ServiceTypeField m_serviceType;
	ServiceSubTypeField m_serviceSubType;
	SourceIdField m_sourceId;
	int m_ack;
	//TcAckFlags m_ack;
	//TcAck m_ack;
}
PACKED_DATA_CLASS;

// ================================================================================================
// Sec_hdr inline methods bodies
// ------------------------------------------------------------------------------------------------

/// We add some default values
Sec_hdr::Sec_hdr(ServiceTypeValue serviceType = 1,
                             ServiceSubTypeValue serviceSubType = 1,
                             SourceIdValue sourceId = 1,
                             int ack = 0)
							 //TcAckFlags ack = TcAck::ACCEPTANCE)
							 //TcAck ack = TcAck::ACCEPTANCE)
{

	setMAck(ack);
	setMServiceSubType(serviceSubType);
	setMServiceType(serviceType);
	setMSourceId(sourceId);
}

Sec_hdr::~Sec_hdr()
{
	DBG(("sec_hdr destr\n"));
}


} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_CORE_TMTC_HEADERTC_H_ */
