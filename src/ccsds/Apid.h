/**
 * @file apid.h
 * @date 21 juin 2018
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

#ifndef ECL_CORE_TMTC_APID_H_
#define ECL_CORE_TMTC_APID_H_

#define MK_APID(tid, pid, pcat) ((((tid) & 1) << 10) | (((pid) & ((1 << 5) - 1)) << 5) | ((pcat) & ((1 << 5) - 1)))
#define MK_ID(isTc, hasDfh, apid) ((((isTc) & 1) << 12) | (((hasDfh) & 1) << 11) | ((apid) & ((1 << 11) - 1)))

#define MK_ID_TC(apid)     MK_ID(PKT_TC, DFH_PRESENT, apid)
#define MK_ID_BC(apid)     MK_ID(PKT_TM_OR_BC, DFH_PRESENT, apid)
#define MK_ID_TM_S(apid)   MK_ID(PKT_TM_OR_BC, DFH_PRESENT, apid)
#define MK_ID_TM_X(apid)   MK_ID(PKT_TM_OR_BC, DFH_ABSENT, apid)
#define MK_ID_TM_VHF(apid) MK_ID(PKT_TM_OR_BC, DFH_ABSENT, apid)

# define DECLARE_ENUM(EnumName) enum EnumName

DECLARE_ENUM(Tid)
{
	TID_REALTIME = 0,
	TID_DEFERRED = 1
};

DECLARE_ENUM(Pid)
{
	PID_BDCAST  = 1,
	PID_ECLAIRS = 18,
	PID_MXT     = 19,
	PID_GRM     = 24
};

DECLARE_ENUM(PcatDtm)
{
	PCAT_TM     = 0,
	PCAT_SCI    = 17,
	PCAT_SCI_HK = 18,
	PCAT_ENG_HK = 19,
	PCAT_AAVPVT = 20,
	PCAT_DUMP   = 21,
	PCAT_TEST   = 22
};

DECLARE_ENUM(PcatRtm)
{
	PCAT_VHF_ALERT    = 0,
	PCAT_VHF_HPLCURVE = 1,
	PCAT_VHF_LPLCURVE = 2,
	PCAT_VHF_ALDESC1  = 3,
	PCAT_VHF_ALDESC2  = 4,
	PCAT_VHF_ALDESC3  = 5,
	PCAT_VHF_ALDESC4  = 6,
	PCAT_VHF_ALDESC5  = 7,
	PCAT_VHF_SUBIMAGE = 8,
	PCAT_VHF_SHADOWG  = 9,
	PCAT_VHF_REC1     = 10,
	PCAT_VHF_REC2     = 11,
	PCAT_RTM1         = 20,
};

DECLARE_ENUM(PcatRtc)
{
	PCAT_TC   = 0,
	PCAT_TIME = 1,
	PCAT_AAV  = 2,
	PCAT_SLEW = 3,
	PCAT_PVT  = 4,
	PCAT_GRM  = 23
};

DECLARE_ENUM(Apid)
{
	APID_TC       = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_TC),
	APID_TM_S     = MK_APID(TID_DEFERRED, PID_ECLAIRS, PCAT_TM),

	APID_MSG_GRM  = MK_APID(TID_REALTIME, PID_GRM,     PCAT_GRM),

	APID_MSG_SLEW = MK_APID(TID_REALTIME, PID_BDCAST,  PCAT_SLEW),
	APID_MSG_TIME = MK_APID(TID_REALTIME, PID_BDCAST,  PCAT_TIME),
	APID_MSG_AAV  = MK_APID(TID_REALTIME, PID_BDCAST,  PCAT_AAV),
	APID_MSG_PVT  = MK_APID(TID_REALTIME, PID_BDCAST,  PCAT_PVT),

	APID_TM_VHF_ALERT    = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_ALERT),
	APID_TM_VHF_HPLCURVE = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_HPLCURVE),
	APID_TM_VHF_LPLCURVE = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_LPLCURVE),
	APID_TM_VHF_ALDESC1  = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_ALDESC1),
	APID_TM_VHF_ALDESC2  = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_ALDESC2),
	APID_TM_VHF_ALDESC3  = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_ALDESC3),
	APID_TM_VHF_ALDESC4  = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_ALDESC4),
	APID_TM_VHF_ALDESC5  = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_ALDESC5),
	APID_TM_VHF_SUBIMAGE = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_SUBIMAGE),
	APID_TM_VHF_SHADOWG  = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_SHADOWG),
	APID_TM_VHF_REC1     = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_REC1),
	APID_TM_VHF_REC2     = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_VHF_REC2),

	APID_TM_S_RTTM   = MK_APID(TID_REALTIME, PID_ECLAIRS, PCAT_RTM1),

	APID_TM_X_SCI    = MK_APID(TID_DEFERRED, PID_ECLAIRS, PCAT_SCI),
	APID_TM_X_SCI_HK = MK_APID(TID_DEFERRED, PID_ECLAIRS, PCAT_SCI_HK),
	APID_TM_X_ENGHK  = MK_APID(TID_DEFERRED, PID_ECLAIRS, PCAT_ENG_HK),
	APID_TM_X_AAVPVT = MK_APID(TID_DEFERRED, PID_ECLAIRS, PCAT_AAVPVT),
	APID_TM_X_DUMP   = MK_APID(TID_DEFERRED, PID_ECLAIRS, PCAT_DUMP),
	APID_TM_X_TEST   = MK_APID(TID_DEFERRED, PID_ECLAIRS, PCAT_TEST),
};

DECLARE_ENUM(PktType)
{
	PKT_TC = 1,
	PKT_TM_OR_BC = 0
};

DECLARE_ENUM(DfhType)
{
	DFH_PRESENT = 1,
	DFH_ABSENT = 0
};

DECLARE_ENUM(Id)
{
	ID_TC       = MK_ID_TC(APID_TC),
	ID_TM_S     = MK_ID_TM_S(APID_TM_S),

	ID_MSG_GRM  = MK_ID_TM_VHF(APID_MSG_GRM),

	ID_MSG_SLEW = MK_ID_BC(APID_MSG_SLEW),
	ID_MSG_TIME = MK_ID_BC(APID_MSG_TIME),
	ID_MSG_AAV  = MK_ID_BC(APID_MSG_AAV),
	ID_MSG_PVT  = MK_ID_BC(APID_MSG_PVT),

	ID_TM_VHF_ALERT    = MK_ID_TM_VHF(APID_TM_VHF_ALERT),
	ID_TM_VHF_HPLCURVE = MK_ID_TM_VHF(APID_TM_VHF_HPLCURVE),
	ID_TM_VHF_LPLCURVE = MK_ID_TM_VHF(APID_TM_VHF_LPLCURVE),
	ID_TM_VHF_ALDESC1  = MK_ID_TM_VHF(APID_TM_VHF_ALDESC1),
	ID_TM_VHF_ALDESC2  = MK_ID_TM_VHF(APID_TM_VHF_ALDESC2),
	ID_TM_VHF_ALDESC3  = MK_ID_TM_VHF(APID_TM_VHF_ALDESC3),
	ID_TM_VHF_ALDESC4  = MK_ID_TM_VHF(APID_TM_VHF_ALDESC4),
	ID_TM_VHF_ALDESC5  = MK_ID_TM_VHF(APID_TM_VHF_ALDESC5),
	ID_TM_VHF_SUBIMAGE = MK_ID_TM_VHF(APID_TM_VHF_SUBIMAGE),
	ID_TM_VHF_SHADOWG  = MK_ID_TM_VHF(APID_TM_VHF_SHADOWG),
	ID_TM_VHF_REC1     = MK_ID_TM_VHF(APID_TM_VHF_REC1),
	ID_TM_VHF_REC2     = MK_ID_TM_VHF(APID_TM_VHF_REC2),

	ID_TM_S_RTTM   = MK_ID_TM_S(APID_TM_S_RTTM),

	ID_TM_X_SCI    = MK_ID_TM_X(APID_TM_X_SCI),
	ID_TM_X_SCI_HK = MK_ID_TM_X(APID_TM_X_SCI_HK),
	ID_TM_X_ENGHK  = MK_ID_TM_X(APID_TM_X_ENGHK),
	ID_TM_X_AAVPVT = MK_ID_TM_X(APID_TM_X_AAVPVT),
	ID_TM_X_DUMP   = MK_ID_TM_X(APID_TM_X_DUMP),
	ID_TM_X_TEST   = MK_ID_TM_X(APID_TM_X_TEST)
};

#undef DECLARE_ENUM

#undef MK_ID_TM_VHF
#undef MK_ID_TM_X
#undef MK_ID_TM_S
#undef MK_ID_BC
#undef MK_ID_TC

#undef MK_ID
#undef MK_APID

#endif /* ECL_CORE_TMTC_APID_H_ */
