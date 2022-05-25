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

namespace ecl
{
namespace core
{
namespace tmtc
{

class Sec_hdr_org : public uintBe4_t
{
public:
	Sec_hdr_org() = default;
	constexpr Sec_hdr_org(uint32_t rawValue);
	constexpr Sec_hdr_org(ServiceTypeValue serviceType,
	                   ServiceSubTypeValue serviceSubType,
	                   SourceIdValue sourceId,
	                   TcAckFlags ack);

	DECLARE_BIT(hasSecondaryHeader, 31)
	DECLARE_CASTED_FIELD(version, 28, 3, PusVersion)
	DECLARE_CASTED_FIELD(ackFlags, 24, 4, TcAckFlagsValue)
	DECLARE_CASTED_FIELD(serviceType, 16, 8, ServiceTypeValue)
	DECLARE_CASTED_FIELD(serviceSubType, 8, 8, ServiceSubTypeValue)
	DECLARE_CASTED_FIELD(sourceId, 0, 8, SourceIdValue)


}
PACKED_DATA_CLASS;

// ================================================================================================
// Sec_hdr_org inline methods bodies
// ------------------------------------------------------------------------------------------------


constexpr Sec_hdr_org::Sec_hdr_org(uint32_t rawValue)
	: uintBe4_t(rawValue)
{
}

/// We add some default values
constexpr Sec_hdr_org::Sec_hdr_org(ServiceTypeValue serviceType = 1,
                             ServiceSubTypeValue serviceSubType = 1,
                             SourceIdValue sourceId = 1,
                             TcAckFlags ack = TcAck::ACCEPTANCE)
							 //TcAck ack = TcAck::ACCEPTANCE)
	: uintBe4_t(0)
{
	
	set_hasSecondaryHeader(false);
	set_version(1);
	set_ackFlags(ack.value());
	//set_ack(ack);
	set_serviceType(serviceType);
	set_serviceSubType(serviceSubType);
	set_sourceId(sourceId);
	

}


class Sec_hdr : public uintBe4_t
{
public:

	Sec_hdr(ServiceTypeValue serviceType,
	                   ServiceSubTypeValue serviceSubType,
	                   SourceIdValue sourceId,
	                   //TcAck ack);
	                   TcAckFlags ack);

	const TcAckFlags& getMAck() const {
	//const TcAck& getMAck() const {
		return m_ack;
	}

	void setMAck(const TcAckFlags &mAck) {
	//void setMAck(const TcAck &mAck) {
		m_ack = mAck;
	}

	ServiceSubTypeValue getMServiceSubType() const {
		return m_serviceSubType;
	}

	void setMServiceSubType(ServiceSubTypeValue mServiceSubType) {
		m_serviceSubType = mServiceSubType;
	}

	ServiceTypeValue getMServiceType() const {
		return m_serviceType;
	}

	void setMServiceType(ServiceTypeValue mServiceType) {
		m_serviceType = mServiceType;
	}

	SourceIdValue getMSourceId() const {
		return m_sourceId;
	}

	void setMSourceId(SourceIdValue mSourceId) {
		m_sourceId = mSourceId;
	}

private:
	ServiceTypeValue m_serviceType;
	ServiceSubTypeValue m_serviceSubType;
	SourceIdValue m_sourceId;
	TcAckFlags m_ack;
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
                             TcAckFlags ack = TcAck::ACCEPTANCE)
							 //TcAck ack = TcAck::ACCEPTANCE)
	: uintBe4_t(0)
{


	setMAck(ack);
	setMServiceSubType(serviceSubType);
	setMServiceType(serviceType);
	setMSourceId(sourceId);
}


} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_CORE_TMTC_HEADERTC_H_ */
