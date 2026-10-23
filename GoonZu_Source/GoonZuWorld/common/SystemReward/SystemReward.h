#pragma once

#include "../../CommonLogic/CommonLogic.h"
#define MAX_SYSTEMREWARD_LIST		20		// 최대 받을수 있는 리스트의 갯수(DB쪽에서는 이 디파인값과 별개로 20으로 고정 # 수정하지마요)

enum eSystemRewardInfoType
{
	SystemRewardInfoType_DormancyEquipItem = 1	//	휴면 계정 장비 보급 상자
};

class cltSystemRewardInfo
{
public:
	SI32		m_siIndex;
	_SYSTEMTIME	m_sGiveTime;
	_SYSTEMTIME	m_sExpireTime;
	GMONEY		m_siMoney;
	cltItem		m_clItem;
	TCHAR		m_szReason[256];

	cltSystemRewardInfo();
	cltSystemRewardInfo( cltSystemRewardInfo* pclInfo );
	~cltSystemRewardInfo();

	void Init();
	void Set( cltSystemRewardInfo* pclInfo );
};


// [종호_NHN->NDOORS] 팅팅 보상리스트 갱신 요청
class cltRequestRewardList
{
public:
	SI32 m_siCurPage;

public:
	cltRequestRewardList(SI32 siCurPage)
	{
		ZeroMemory(this, sizeof(cltRequestRewardList));

		m_siCurPage = siCurPage;
	}
};