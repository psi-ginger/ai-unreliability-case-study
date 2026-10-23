#ifndef	__ITEMPERIODMGR_H__
#define __ITEMPERIODMGR_H__

#pragma once

//////////////////////////////////////////////////////////////////////////
//  [7/1/2009 종호_ITEM_PERIOD] client에서 아이템 효과및 아이콘을
//								resource를 통해 제어하기 위한 class
//////////////////////////////////////////////////////////////////////////

#include <directives.h>
#include "CommonHeader.h"
#include "../../Client/client.h"

#include <vector>

struct sItemPeriodObj;

class CItemPeriodMgr
{
	private:
	vector<sItemPeriodObj> m_SItemPeriodObj;

public:
	bool			Initialize();						//	manager 초기화 관련
	
	sItemPeriodObj*	GetBuffData(SI32 siBuffIndex);		//	버프인덱스 별 버프정보 가져오기

public:
	CItemPeriodMgr(void);
	~CItemPeriodMgr(void);
};

//	버프 개당 필요한 데이터
struct sItemPeriodObj
{
	TCHAR	m_szBuffName[MAX_PATH];
	SI32	m_siTabIndex;
	SI32	m_siIconIndex;
	SI32	m_siTextindex;

	sItemPeriodObj()
	{
		ZeroMemory(m_szBuffName, MAX_PATH);
		m_siTabIndex	= 0;
		m_siIconIndex	= 0;
		m_siTextindex	= 0;
	}

	TCHAR* GetBuffName(){return m_szBuffName;}
	SI32 GetTabIndex(){return m_siTabIndex;}
	SI32 GetIconIndex(){return m_siIconIndex;}
	SI32 GetTextIndex(){return m_siTextindex;}
};

#endif