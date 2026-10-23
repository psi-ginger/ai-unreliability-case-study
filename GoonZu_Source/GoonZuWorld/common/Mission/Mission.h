#ifndef _MISSION_H_
#define _MISSION_H_

#include "../../../NLib/NDataTypes.h"
#include "../../../NLib/NTCHARString.h"
#include "../../../NLib/NArray.h"
#include "Mission_defines.h"

class cltMissionClearCondition
{
public:
	cltMissionClearCondition()
	{
		ZeroMemory(this, sizeof(*this));
	}
	virtual ~cltMissionClearCondition()
	{
	}

public:
	UI16		m_siClearCommand;
	UI16		m_siPara1;
	UI16		m_siPara2;
	UI16		m_siPara3;
};

class cltMissionReward
{
public:
	cltMissionReward()
	{
		ZeroMemory(this, sizeof(*this));
	}
	virtual ~cltMissionReward()
	{
	}

public:
	UI16		m_siRewardType;
	UI16		m_siKindUnique;
	UI32		m_siAmount;

	bool IsEmpty()
	{
		if( m_siRewardType == 0 )		return true;
		if( m_siKindUnique == 0 )		return true;
		if( m_siAmount == 0 )			return true;

		return false;
	}
};

class cltMission
{
public:
	cltMission()
	{
		ZeroMemory(this, sizeof(*this));
	}
	virtual ~cltMission()
	{
	}

protected:
	SI16		m_siMissionUnique;

	UI16		m_siClearCommand;
	UI16		m_siPara1;
	UI16		m_siPara2;
	UI16		m_siPara3;

	UI16		m_siLeftTimeSec;
	SI32		m_siSpecialValue;
};

typedef	NSafeTArray<cltMission, PERSON_MISSION_COUNT>			ARRAY_PERSONMISSIONS;

class cltPersonMissionInfo
{
protected:
	ARRAY_PERSONMISSIONS			m_clMission;

public:
	cltPersonMissionInfo()
	{
		ZeroMemory(this, sizeof(*this));
	}
	virtual ~cltPersonMissionInfo()
	{
	}
};

#endif