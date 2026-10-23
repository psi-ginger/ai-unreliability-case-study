#pragma once

class cltGameMsgResponse_SystemRewardList
{
public:
	SI32 m_siCurPage;
	SI32 m_siMaxPage;

	cltSystemRewardInfo	clInfo[MAX_SYSTEMREWARD_LIST];

	cltGameMsgResponse_SystemRewardList()
	{
		m_siCurPage = 0;
		m_siMaxPage = 0;

		for ( SI32 i = 0 ; i < MAX_SYSTEMREWARD_LIST ; ++i )
		{
			clInfo[i].Init();
		}
	}
};

class cltGameMsgResponse_RecieveSystemReward
{
public:
	cltSystemRewardInfo clInfo;

	SI16			siItemPos;			// 받은 아이템의 인벤토리내 위치
	cltItem			clItem;				// 받은 아이템 정보

	GMONEY			siMoney;

	cltGameMsgResponse_RecieveSystemReward( cltSystemRewardInfo* pclinfo, SI16 itempos, cltItem* pclitem, GMONEY money )
	{
		clInfo.Set( pclinfo );
		siItemPos = itempos;
		clItem.Set( pclitem );
		siMoney = money;
	}
};