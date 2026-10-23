//---------------------------------
// 2004/5/11 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\..\Server\Server.h"
#include "Char-Server.h"


void cltUseItemReserveInfo::Init()
{
	for(SI32 i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		clInfo[i].Init();
	}
}


bool cltUseItemReserveInfo::Push(SI32 unique, SI32 itemnum, SI32 itempos)
{
	SI32 i;
	
	for(i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		if(clInfo[i].siUnique == unique && (itempos == -1 || clInfo[i].siItemPos == itempos))
		{
			clInfo[i].siItemNum += itemnum;
			return true;
		}
	}
	
	for(i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		if(clInfo[i].siUnique == 0)
		{
			clInfo[i].siUnique	= unique;
			clInfo[i].siItemNum = itemnum;
			clInfo[i].siItemPos = itempos;
			return true;
		}
	}

	return false;
}

// 예약되어 있는 것이 있는가. 
bool cltUseItemReserveInfo::IsReserved()
{
	SI32 i;
	
	for(i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		if(clInfo[i].siUnique)
		{
			return true;
		}
	}

	return false;
}


// 화살 및 탄환 관리 클래스
void cltBulletUseReserveInfo::Init()
{
	for(SI32 i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		clInfo[i].Init();
	}
}


bool cltBulletUseReserveInfo::Push(SI32 unique, SI32 itemnum, SI32 itempos)
{
	SI32 i;

	for(i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		if(clInfo[i].siUnique == unique && clInfo[i].siItemPos == itempos)
		{
			clInfo[i].siItemNum += itemnum;
			return true;
		}
	}

	for(i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		if(clInfo[i].siUnique == 0)
		{
			clInfo[i].siUnique	= unique;
			clInfo[i].siItemNum = itemnum;
			clInfo[i].siItemPos = itempos;
			return true;
		}
	}

	return false;
}

// 예약되어 있는 것이 있는가. 
bool cltBulletUseReserveInfo::IsReserved()
{
	SI32 i;

	for(i = 0;i < MAX_USEITEM_RESERVE_NUM;i++)
	{
		if(clInfo[i].siUnique)
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------
// 소환수 화살/총알 아이템 사용 예약관리
//-----------------------------------------------
cltSummonBulletUseReserveInfo::cltSummonBulletUseReserveInfo()
{
	Init();

}

void cltSummonBulletUseReserveInfo::Init()
{
	for ( SI32 siIndex=0; siIndex<MAX_USEITEM_RESERVE_NUM; siIndex++ )
	{
		m_clInfo[ siIndex ].Init();
	}

}

bool cltSummonBulletUseReserveInfo::Push( SI08 siSummonIndex, SI16 siItemUnique, SI16 siItemNum, SI16 siItemPos )
{
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN<=siSummonIndex) )
	{
		return false;
	}

	SI32 siIndex = 0;

	//  기존의 같은 아이템이 예약되어 있는지 검사해서 있으면 그곳에 넣는다
	for ( siIndex=0; siIndex<MAX_USEITEM_RESERVE_NUM; siIndex++ )
	{
		if ( (siSummonIndex		== m_clInfo[siIndex].m_siSummonIndex)	// 소환수 인덱스검사
			&& (siItemUnique	== m_clInfo[siIndex].m_siItemUnique)	// 아이템 유니크검사
			&& (siItemPos		== m_clInfo[siIndex].m_siItemPos)		// 아이템 위치검사
		)
		{
			m_clInfo[ siIndex ].m_siItemNum += siItemNum;

			return true;
		}

		if ( 0 > m_clInfo[siIndex].m_siSummonIndex )
		{
			break;
		}
	}

	// 기존 예약된 아이템이 없다면 새로운 곳에 추가한다
	for ( siIndex=0; siIndex<MAX_USEITEM_RESERVE_NUM; siIndex++ )
	{
		if ( 0 > m_clInfo[siIndex].m_siSummonIndex )
		{
			m_clInfo[ siIndex ].m_siSummonIndex	= siSummonIndex;
			m_clInfo[ siIndex ].m_siItemUnique	= siItemUnique;
			m_clInfo[ siIndex ].m_siItemNum		= siItemNum;
			m_clInfo[ siIndex ].m_siItemPos		= siItemPos;

			return true;
		}
	}

	return false;

}

bool cltSummonBulletUseReserveInfo::IsReserved()
{
	for ( SI32 siIndex=0; siIndex<MAX_USEITEM_RESERVE_NUM; siIndex++ )
	{
		if ( 0 <= m_clInfo[siIndex].m_siSummonIndex )
		{
			return true;
		}
	}

	return false;

}

