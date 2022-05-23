/**
 * @file PktCcsdsTmSb.h
 * @date 26 juin 2020
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

#ifndef ECL_CORE_TMTC_PKTCCSDSTMSB_H_
#define ECL_CORE_TMTC_PKTCCSDSTMSB_H_

#include <ecl/core/tmtc/HeaderTmSb.h>
#include <ecl/core/tmtc/PktCcsds.h>

namespace ecl
{
namespace core
{
namespace tmtc
{

class PktCcsdsTmSb : public Prim_hdr, public HeaderTmSb
{
public:
	PktCcsdsTmSb() = default;
}
PACKED_DATA_CLASS;

} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_CORE_TMTC_PKTCCSDSTMSB_H_ */
