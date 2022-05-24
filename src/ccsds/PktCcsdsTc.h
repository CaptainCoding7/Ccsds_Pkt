/**
 * @file PktCcsdsTc.h
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

#ifndef ECL_CORE_TMTC_PKTCCSDSTC_H_
#define ECL_CORE_TMTC_PKTCCSDSTC_H_

#include "Prim_hdr.h"
#include "Sec_hdr.h"
#include "TrailerTc.h"

namespace ecl
{
namespace core
{
namespace tmtc
{

class PktCcsdsTc : public Prim_hdr, public Sec_hdr
{
public:
	PktCcsdsTc() = default;

	template<class Payload>
	struct Packet;

	template<class... ServicesDispatchers>
	struct Dispatcher;

	template<ServiceTypeValue SERVICE_TYPE, class... TcTypes>
	struct ServiceDispatcher;

	template<class TcType>
	struct SubServiceDispatcher;

private:
}
PACKED_DATA_CLASS;

template<class Payload>
struct PktCcsdsTc::Packet : PktCcsdsTc, Payload, TrailerTc
{
}
PACKED_DATA_CLASS;

template<class... ServicesDispatchers>
struct PktCcsdsTc::Dispatcher
{
	template<class Visitor>
	static DispatchResult dispatch(const Prim_hdr& packet, CcsdsLength length, Visitor&& v)
	{
		if(packet.get_ccsdsId_BE() == ID_TC)
		{
			const PktCcsdsTc& tc = static_cast<const PktCcsdsTc&>(packet);
			return DispatchIterator<ServicesDispatchers...>::dispatch(tc, length, v);
		}
		else
		{
			return {DispatchStatus::UNKNOWN_APID, ExecutionStatus::UNKNOWN_TC};
		}
	}
};

template<class TcType>
struct PktCcsdsTc::SubServiceDispatcher
{
	template<class Visitor>
	static DispatchResult dispatch(const PktCcsdsTc& tc, CcsdsLength length, Visitor&& visitor)
	{
		if(tc.serviceSubType() == TcType::SERVICE_SUBTYPE)
		{
			if(length == sizeof(TcType))
			{
				const TcType& decodedTc = static_cast<const TcType&>(tc);
				Prim_hdr::ExecutionStatus execStatus = visitor(decodedTc);
				return {DispatchStatus::OK, execStatus};
			}
			else
			{
				return {DispatchStatus::OK, ExecutionStatus::INVALID_LENGTH};
			}
		}
		else
		{
			return {DispatchStatus::UNKNOWN_SUBSERVICE, ExecutionStatus::UNKNOWN_TC};
		}
	}
};

template<ServiceTypeValue SERVICE_TYPE, class... SubservicesDispatchers>
struct PktCcsdsTc::ServiceDispatcher
{
	template<class Visitor>
	static DispatchResult dispatch(const PktCcsdsTc& tc, CcsdsLength length, Visitor&& v)
	{
		if(tc.serviceType() == SERVICE_TYPE)
		{
			return Prim_hdr::DispatchIterator<SubServiceDispatcher<SubservicesDispatchers>...>::dispatch(tc, length, v);
		}
		else
		{
			return {DispatchStatus::UNKNOWN_SERVICE, ExecutionStatus::UNKNOWN_TC};
		}
	}
};

} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_CORE_TMTC_PKTCCSDSTC_H_ */
