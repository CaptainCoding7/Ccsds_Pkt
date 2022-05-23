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

class Sec_hdr : public uintBe4_t
{
public:
	Sec_hdr() = default;
	constexpr Sec_hdr(uint32_t rawValue);
	constexpr Sec_hdr(ServiceTypeValue serviceType,
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
// Sec_hdr inline methods bodies
// ------------------------------------------------------------------------------------------------

constexpr Sec_hdr::Sec_hdr(uint32_t rawValue)
	: uintBe4_t(rawValue)
{
}

constexpr Sec_hdr::Sec_hdr(ServiceTypeValue serviceType,
                             ServiceSubTypeValue serviceSubType,
                             SourceIdValue sourceId,
                             TcAckFlags ack)
	: uintBe4_t(0)
{
	set_hasSecondaryHeader(false);
	set_version(1);
	set_ackFlags(ack.value());
	set_serviceType(serviceType);
	set_serviceSubType(serviceSubType);
	set_sourceId(sourceId);
}

} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_CORE_TMTC_HEADERTC_H_ */
