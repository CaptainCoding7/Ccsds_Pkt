/**
 * @file TrailerTc.h
 * @date Aug 5, 2021
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

#ifndef ECL_CORE_TMTC_TRAILERTC_H_
#define ECL_CORE_TMTC_TRAILERTC_H_

#include <ecl/Integrity.h>
#include <ecl/WordBe.h>

namespace ecl
{
namespace core
{
namespace tmtc
{

class TrailerTc
{
public:
	TrailerTc() = default;

	Integrity::Crc16 crc() const;

private:
	uintBe2_t m_crc;
}
PACKED_DATA_CLASS;

inline Integrity::Crc16 TrailerTc::crc() const
{
	return m_crc.value();
}

} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_CORE_TMTC_TRAILERTC_H_ */
