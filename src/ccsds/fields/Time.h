/**
 * @file Time.h
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

#ifndef ECL_CORE_TMTC_FIELDS_TIME_H_
#define ECL_CORE_TMTC_FIELDS_TIME_H_

#include <ecl/WordBe.h>

namespace ecl
{
namespace core
{
namespace tmtc
{

class Time
{
public:
	Time() = delete;

	class Utc;
	class Cpu;
};

/**
 * Time in UTC format, with time in seconds and microseconds
 */
class Time::Utc
{
public:
	using Seconds = uintBe4_t;
	using MicroSeconds = uintBe3_t;

	Utc() = default;
	Seconds      m_TimeSec;
	MicroSeconds m_TimeMicrosec;
}
PACKED_DATA_CLASS;

/**
 * Time read from the CPU timer.
 */
class Time::Cpu
{
public:
	Cpu() = default;
	uintBe4_t m_Time; 			// in units of ?
}
PACKED_DATA_CLASS;

} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_CORE_TMTC_FIELDS_TIME_H_ */
