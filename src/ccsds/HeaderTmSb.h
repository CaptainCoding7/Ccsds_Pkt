/**
 * @file HeaderTmSb.h
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
 */

#ifndef ECL_CORE_TMTC_HEADERTMSB_H_
#define ECL_CORE_TMTC_HEADERTMSB_H_

#include "fields/Time.h"
#include <ecl/BitFieldInteger.h>
#include "Types.h"

namespace ecl
{
namespace core
{
namespace tmtc
{

class HeaderTmSb : protected uintBe3_t
{
public:
	HeaderTmSb() = default;

	DECLARE_CASTED_FIELD(serviceSubType, 8, 8, ServiceSubTypeValue)
	DECLARE_CASTED_FIELD(serviceType, 16, 8, ServiceTypeValue)
	DECLARE_CASTED_FIELD(version, 28, 3, PusVersion)

private:
	Time::Utc m_packetTime;
}
PACKED_DATA_CLASS;

} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_CORE_TMTC_HEADERTMSB_H_ */
