/**
 * @file Prim_hdr.h
 * @date 24 févr. 2020
 * @author schanne
 * @note
 *
 * Contributors: schanne
 *
 * This file is part of the UGTS software project.
 *
 * @copyright © Commissariat a l'Energie Atomique et aux Energies Alternatives (CEA)
 *
 *
 * PREVIOUSLY NAMED PktCcsds.h
 *
 */

#ifndef ECL_CORE_TMTC_PKTCCSDS_H_
#define ECL_CORE_TMTC_PKTCCSDS_H_

#include <ecl/WordBe.h>
#include <ecl/BitFieldHelper.h>

#include "../debug_print.h"
#include "Types.h"
#include "Apid.h"

namespace ecl
{
namespace core
{
namespace tmtc
{

/**
 * CCSDS Packet Header (6 bytes) - base class of all CCSDS Packets
 *
 * \warning \sa class PktBase about not using virtual members.
 *
 */
class Prim_hdr
{
public:
	using IdField = uintBe2_t;
	using SequenceField = uintBe2_t;
	using LengthField = uintBe2_t;

	static constexpr BitCount COUNTER_BITCOUNT = 14;
	static constexpr BitCount GROUPING_BITCOUNT = 2;

	enum class DispatchStatus
	{
		OK,
		UNKNOWN_APID,
		UNKNOWN_SERVICE,
		UNKNOWN_SUBSERVICE,
		UNKNONW_PACKETID,
		_COUNT,
		_BEGIN = OK,
		_END = _COUNT
	};
	enum class ExecutionStatus
	{
		OK,
		UNKNOWN_TC,
		INVALID_LENGTH,
		INVALID_PARAMETER,
		INVALID_CFG_PARAM,
		SERVICE_NOT_READY,
		ACTION_FORBIDDEN,
		COMM_FAILURE,
		EXECUTION_ERROR,
		_COUNT,
		_BEGIN = OK,
		_END = _COUNT
	};

	struct DispatchResult
	{
		DispatchStatus dispatchStatus;
		ExecutionStatus execStatus;
	};

	template<class... ApidsDispatchers>
	struct Dispatcher;

	/**
	 * Constructor without initialization.
	 */
	//Prim_hdr() = default;

	/**
	 * Full constructor with default values
	 * @param id
	 * @param counter
	 * @param length
	 * @param grouping
	 */
	constexpr Prim_hdr(Id id = ID_TC,
	                   CcsdsCounter counter = 1,
	                   CcsdsLength length = 30,
	                   CcsdsGrouping grouping = CcsdsGrouping::STANDALONE)
		: m_CcsdsId(id),
		  m_CcsdsSequence(counter),
		  m_CcsdsPLength(length)
	{
		setCcsdsGrouping(grouping);
	}

	~Prim_hdr()
	{
		DBG(("prim_hdr destr\n"));

	}


	/**
	 * Sets CcsdsId with a value.
	 * @param val
	 */
	void setCcsdsId(Id val)
	{
		m_CcsdsId.setValue(val);
	}

	/**
	 * Returns CcsdsId value.
	 * @return
	 */
	constexpr Id get_ccsdsId_BE() const
	{
		//std::cout << "addr ID = "<< &m_CcsdsId << std::endl;
		return static_cast<Id>(m_CcsdsId.value());
	}

	/**
	 * Set counter field of CcsdsSequence with a value (14 ls-bits).
	 * @param val
	 */
	void setCcsdsCounter(CcsdsCounter val)
	{
		m_CcsdsSequence.setValue(
			BitFieldHelper<CcsdsSequence>
				::setField<0, COUNTER_BITCOUNT>(m_CcsdsSequence.value(), val)
		);
	}

	/**
	 * Get CcsdsCounter value (significant bits only)
	 * @return
	 */
	constexpr CcsdsCounter get_ccsdsCounter_BE() const
	{
		//std::cout << "addr counter = "<< &m_CcsdsSequence << std::endl;
		return BitFieldHelper<CcsdsSequence>
			::getField<0, COUNTER_BITCOUNT>(m_CcsdsSequence.value());
	}

	void setCcsdsGrouping(CcsdsGrouping grouping)
	{
		m_CcsdsSequence.setValue(
			BitFieldHelper<CcsdsSequence>
				::setField<COUNTER_BITCOUNT, GROUPING_BITCOUNT>(m_CcsdsSequence.value(), underlyingValue(grouping))
		);
	}

	constexpr CcsdsGrouping ccsdsGrouping() const
	{
		return static_cast<CcsdsGrouping>(BitFieldHelper<CcsdsSequence>
			::getField<COUNTER_BITCOUNT, GROUPING_BITCOUNT>(m_CcsdsSequence.value()));
	}

	/**
	 * set field m_CcsdsPlength with a value coming from getCcsdsPLengthFromClass.
	 *
	 * Set data length in CCSDS Header field m_CcsdsPLength
	 * considering this packet as of type T.
	 *
	 * \note the data field length value to encode should be 1 fewer than
	 * the actual field length.
	 */
	template<class T>
	void setCcsdsPLength()
	{

		m_CcsdsPLength.setValue(sizeofPktData<T>());
	}

	void setCcsdsPLength(CcsdsLength length)
	{

		m_CcsdsPLength.setValue(length);
	}

	constexpr CcsdsLength get_ccsdsPLength_BE() const
	{
		return m_CcsdsPLength.value();
	}

	/**
	 * \return Ccsds header size if bytes (actually sizeof current class).
	 */
	ByteCount getCcsdsHeaderSize() const
	{
		return sizeof(Prim_hdr);
	}

	/**
	 * Return the full length of the CCSDS packet.
	 *
	 * Header size + total number of fields
	 *
	 * @return
	 */
	ByteCount getPktFullLength() const
	{
		return getCcsdsHeaderSize() + getPktDataFieldLength();
	}

	/**
	 * Return the length of the CCSDS packet data field.
	 * @return
	 */
	ByteCount getPktDataFieldLength() const
	{
		return m_CcsdsPLength.value() + 1;
	}

	/**
	 * Get total lenth of a packet considered as a packet of type T.
	 *
	 * Please note that the data packet actually stores the data length
	 * minus 1.
	 *
	 * \return data packet length.
	 */
	template<class T>
	ByteCount sizeofPktData()
	{
		return sizeof(T) - getCcsdsHeaderSize() - 1 ;
	}

	template<class... DispatcherList>
	struct DispatchIterator;

private:
	// begin Packet Data Fields -----------------------------------

	/**
	 * Packet ID.
	 */
	IdField m_CcsdsId;

	/**
	 * Packet sequence counter (or packet name), 14 ls-bits used, the others set to 1.
	 */
	SequenceField m_CcsdsSequence;

	/**
	 * Packet length: This 16-bit field equals one fewer than the length (in octets)
	 * of the Packet Data Field (i.e. all what follows the Header).
	 * Such that the full length of the CCSDS packet is m_CcsdsPLength+1+6.
	 */
	LengthField m_CcsdsPLength;

	// end Packet Data Fields ------------------------------------

} // class Prim_hdr
PACKED_DATA_CLASS;

template<class FirstDispatcher, class... OtherDispatchers>
struct Prim_hdr::DispatchIterator<FirstDispatcher, OtherDispatchers...>
{
	template<class Packet, class Visitor>
	static DispatchResult dispatch(const Packet& packet, CcsdsLength length, Visitor&& v)
	{
		DispatchResult result = FirstDispatcher::dispatch(packet, length, v);
		if(result.dispatchStatus != DispatchStatus::OK)
		{
			return DispatchIterator<OtherDispatchers...>::dispatch(packet, length, v);
		}
		else
		{
			return result;
		}
	}
};

template<class LastDispatcher>
struct Prim_hdr::DispatchIterator<LastDispatcher>
{
	template<class Packet, class Visitor>
	static DispatchResult dispatch(const Packet& packet, CcsdsLength length, Visitor&& v)
	{
		return LastDispatcher::dispatch(packet, length, v);
	}
};

template<class... ApidsDispatchers>
struct Prim_hdr::Dispatcher
{
	template<class Visitor>
	static DispatchResult dispatch(const Prim_hdr& packet, CcsdsLength length, Visitor&& v)
	{
		return DispatchIterator<ApidsDispatchers...>::dispatch(packet, length, v);
	}
};

} /* namespace tmtc */
} /* namespace core */
} /* namespace ecl */

#endif /* ECL_CORE_TMTC_PKTCCSDS_H_ */
