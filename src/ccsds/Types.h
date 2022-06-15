/**
 * @file Types.h
 * @date 24 avr. 2020
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

#ifndef ECL_CORE_TMTC_TYPES_H_
#define ECL_CORE_TMTC_TYPES_H_

#include <ecl/FlagSet.h>
#include <ecl/Enum.h>
#include <cstdint>

namespace ecl
{
namespace core
{
namespace tmtc
{

enum class CcsdsGrouping : uint8_t
{
	CONTINUATION = 0,
	FIRST = 1,
	LAST = 2,
	STANDALONE = 3,
};

enum class TcAck : uint8_t
{
	ACCEPTANCE,
	START,
	PROGRESS,
	COMPLETION,
	_COUNT,
	_BEGIN = ACCEPTANCE,
	_END = _COUNT
};

using CcsdsApidValue = uint16_t;
using CcsdsSequence = uint16_t;
using CcsdsCounter = uint16_t;
using CcsdsLength = uint16_t;

using PusVersion = uint8_t;

using TcAckFlags = FlagSet<TcAck>;
using TcAckFlagsValue = UnderlyingType<TcAck>;

using ServiceTypeValue = uint8_t;
using ServiceSubTypeValue = uint8_t;
using SourceIdValue = uint16_t;
//using PacketId = uint8_t;
using SpareValue = uint8_t;

using TfCounter = uint32_t;
using TfCounterSub = uint16_t;
using TimeUtcSeconds = uint32_t;
using TimeUtcMicroSeconds = uint32_t;

} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_TMTC_H_ */
