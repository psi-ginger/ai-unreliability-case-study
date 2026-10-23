#include "PersonInfo.h"

#include "../Server/Server.h"
#include "Char/CharServer/Char-Server.h"

//===============================
// cltSummonScroll
//===============================
cltSummonScroll::cltSummonScroll()
{
	Init();
}

void cltSummonScroll::Init()
{
	ZeroMemory(this, sizeof(cltSummonScroll));
}

cltSummonScroll::cltSummonScroll(SI16 unique0, SI16 percent0, SI16 unique1, SI16 percent1)
{
	siUnique[0] = unique0;
	siUnique[1] = unique1;

	siPercent[0] = percent0;
	siPercent[1] = percent1;
}

void cltSummonScroll::Set(cltSummonScroll* pclSummonScroll)
{
	memcpy(this, pclSummonScroll, sizeof(cltSummonScroll));				//ok
}

SI16 cltSummonScroll::GetPercent(SI16 scroll)
{
	for(SI32 i=0; i<MAX_SCROLL_SUMMON_PER_KIND; ++i)
	{
		if(siUnique[i] == scroll) return siPercent[i];
	}
	return -1;
}

BOOL cltSummonScroll::FindUnique(SI16 scroll)
{
	for(SI32 i=0; i<MAX_SCROLL_SUMMON_PER_KIND; ++i)
	{
		if(siUnique[i] == scroll) return TRUE;
	}
	return FALSE;
}

BOOL cltSummonScroll::SetPercent(SI16 scroll, SI16 percent)
{
	for(SI32 i=0; i<MAX_SCROLL_SUMMON_PER_KIND; ++i)
	{
		if(siUnique[i] == scroll)
		{
			siPercent[i] = percent;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL cltSummonScroll::AddPercent(SI16 scroll, SI16 percent)
{
	for(SI32 i=0; i<MAX_SCROLL_SUMMON_PER_KIND; ++i)
	{
		if(siUnique[i] == scroll)
		{
			siPercent[i] += percent;
			return TRUE;
		}
	}
	return FALSE;
}

SI32 cltSummonScroll::GetScrollRate(cltItemManagerCommon* pclItemManager, SI32 siScrollFunction)
{ 
	SI32 siScrollRate = 0;

	cltItemInfo* pclscrollinfo = NULL;
	for(SI32 i= 0; i < MAX_SCROLL_SUMMON_PER_KIND; ++i)
	{
		// 부적의 내용을 확인한다. 
		pclscrollinfo = pclItemManager->FindItemInfo(siUnique[i]);
		if(pclscrollinfo != NULL)
		{
			if(pclscrollinfo->siScrollFunction == siScrollFunction)
			{
				siScrollRate += siPercent[i];
			}
		}
	}
	return siScrollRate;
}

//===============================
// cltSummon
//===============================
cltSummon::cltSummon()
{
	Init();
}

void cltSummon::Init()
{
	ZeroMemory(this, sizeof(cltSummon));
}

cltSummon::cltSummon(	SI32				SummonID,
						SI16				kind,			
						TCHAR*				pname, 
						SI08				status,		
						SI08				attacktype,	
						SI08				identity, 
						SI32				takeinminute,
						SI16				attackWearStoneNum,
						SI16				defenseWearStoneNum,
						cltPIPInfoCommon*	pclip,  
						cltSummonScroll*	pscroll,
						SI16				_siTranformStamina				 
)
{
	siSummonID				= SummonID;
	siKind					= kind;
	MStrCpy( szName, pname, MAX_SUMMON_NAME );

	siStatus				= status;
	siAttackType			= attacktype;
	siIdentity				= identity;
	siTakeInMinute			= takeinminute;
	siAttackWearStoneNum	= attackWearStoneNum;
	siDefenseWearStoneNum	= defenseWearStoneNum;
	clIP.Set(pclip);
	clScroll.Set(pscroll);
	m_siTranformStamina		= _siTranformStamina;
}

void cltSummon::Set(cltSummon* pclSummon)
{
	memcpy( this, pclSummon, sizeof(cltSummon) );
}

/*
SI16 cltSummon::GetAttackWearStoneNum( void )
{
	return siAttackWearStoneNum;
}

bool cltSummon::SetAttackWearStoneNum( SI16 attackWearStoneNum )
{
	if ( 0 > attackWearStoneNum )
	{
		return false;
	}

	siAttackWearStoneNum = attackWearStoneNum;

	return true;
}

SI16 cltSummon::AddAttackWearStoneNum( SI16 attackWearStoneNum )
{
	if ( (0 > attackWearStoneNum) || (0 > ( (SI16)(siAttackWearStoneNum + attackWearStoneNum) )) )
	{
		return -1;
	}

	siAttackWearStoneNum += attackWearStoneNum;

	return siAttackWearStoneNum;

}

SI16 cltSummon::GetDefenseWearStoneNum( void )
{
	return siDefenseWearStoneNum;
}

bool cltSummon::SetDefenseWearStoneNum( SI16 defenseWearStoneNum )
{
	if ( 0 > defenseWearStoneNum )
	{
		return false;
	}

	siDefenseWearStoneNum = defenseWearStoneNum;

	return true;
}

SI16 cltSummon::AddDefenseWearStoneNum( SI16 defenseWearStoneNum )
{
	if ( (0 > defenseWearStoneNum) || (0 > ( (SI16)(siDefenseWearStoneNum + defenseWearStoneNum) )) )
	{
		return -1;
	}

	siDefenseWearStoneNum += defenseWearStoneNum;

	return siDefenseWearStoneNum;

}
*/
//===============================
// cltSummon
//===============================
cltSimpleSummon::cltSimpleSummon()
{
	Init();
}

void cltSimpleSummon::Init()
{
	ZeroMemory(this, sizeof(cltSimpleSummon));
}

cltSimpleSummon::cltSimpleSummon( 
					 SI16 kind,
					 SI16 Level,
					 SI32 SummonID,
					 TCHAR* pname
					 )
{
	
	siKind					=	kind;
	siLevel					=	Level;
	siSummonID				=	SummonID;
	MStrCpy( szName, pname, MAX_SUMMON_NAME );
}

void cltSimpleSummon::Set(cltSimpleSummon* pclSummon)
{
	memcpy(this, pclSummon, sizeof(cltSimpleSummon));							//ok
}

void cltSimpleSummon::Set(cltSummon* pclSummon)
{
	siKind					=	pclSummon->siKind;
	siLevel					=	pclSummon->clIP.siLevel;
	siSummonID				=	pclSummon->siSummonID;
	MStrCpy( szName, pclSummon->szName, MAX_SUMMON_NAME );
}

//===============================
// cltSummonInfo
//===============================
cltSummonInfo::cltSummonInfo()
{
	Init();
}

void cltSummonInfo::Init()
{
	ZeroMemory(this, sizeof(cltSummonInfo));

	m_siSummonTranformIndex = -1;
}
cltSummonInfo::~cltSummonInfo()
{
}

void cltSummonInfo::Set(cltSummonInfo* pclSummoninfo)
{
	memcpy( this, pclSummoninfo, sizeof(cltSummonInfo) );					
}

BOOL cltSummonInfo::CanHaveSummon()
{
	//for(SI32 i = 0 ; i < 1; ++i)		// PATCH
	for(SI32 i = 0 ; i < MAX_SUMMON_OWN; ++i)
	{
		if(clSummon[i].siKind == 0) 
			return TRUE;
	}
	return FALSE;
}

// 서범아이디가 있는지 검사해서 해당 인덱스를 리턴해준다.
BOOL cltSummonInfo::GetEmptySummonIndex(SI32 *index)
{
	for(SI32 i = 0 ; i < MAX_SUMMON_OWN; ++i)
	{
		if(clSummon[i].siSummonID == 0) 
		{ 
			*index = i; 
			return TRUE; 
		}
	}

	return FALSE;
}

BOOL cltSummonInfo::GetEmptySummonIndexInHouse(SI32 *index)
{
	for(SI32 i = 0 ; i < MAX_HAVE_SUMMON; ++i)
	{
		if(clSummonBase[i].siSummonID == 0) 
		{ 
			*index = i; 
			return TRUE; 
		}
	}

	return FALSE;
}

BOOL cltSummonInfo::HaveSummon()
{
	for(SI32 i = 0 ; i < MAX_SUMMON_OWN; ++i)
	{
		if(clSummon[i].siKind >0 && clSummon[i].siKind <MAX_KIND_NUMBER) return TRUE;
	}
	return FALSE;
}

void cltSummonInfo::SetSummon(SI16 index, cltSummon *pclSummon)
{
	if(index>=0 && index<MAX_SUMMON_OWN)	clSummon[index].Set(pclSummon);
}

cltSummon* cltSummonInfo::GetSummon(SI16 index)
{
	if(index>=0 && index<MAX_SUMMON_OWN)	return &clSummon[index];
	return NULL;
}
cltSimpleSummon* cltSummonInfo::GetSummonBase(SI16 index)
{
	if(index>=0 && index<MAX_HAVE_SUMMON)	return &clSummonBase[index];
	return NULL;
}

cltSummon* cltSummonInfo::GetSummonFromSummonID(SI32 siSummonID)
{
	for(SI32 i = 0 ; i < MAX_SUMMON_OWN; ++i)
	{
		if( clSummon[i].siSummonID == siSummonID ) return &clSummon[i];
	}
	return NULL;
}

cltSimpleSummon* cltSummonInfo::GetSimpleSummonFromSummonID(SI32 siSummonID)
{
	for(SI32 i = 0 ; i < MAX_HAVE_SUMMON; ++i)
	{
		if( clSummonBase[i].siSummonID == siSummonID ) return &clSummonBase[i];
	}
	return NULL;
}

SI08 cltSummonInfo::GetSummonIndexFromSummonID(SI32 siSummonID)
{
	for(SI32 i = 0 ; i < MAX_SUMMON_OWN; ++i)
	{
		if( clSummon[i].siSummonID == siSummonID ) return i;
	}
	return -1;
}

SI08 cltSummonInfo::GetSummonBaseIndexFromSummonID(SI32 siSummonID)
{
	for(SI32 i = 0 ; i < MAX_HAVE_SUMMON; ++i)
	{
		if( clSummonBase[i].siSummonID == siSummonID ) return i;
	}
	return -1;
}

bool cltSummonInfo::IsSummonTranform( SI32 siTranformIndex )
{
	if( m_siSummonTranformIndex != siTranformIndex ) return false;

	return true;
}

void cltSummonInfo::SetIsSummonTranform( bool _bIsSummonTranform )
{
	m_bIsSummonTranform = _bIsSummonTranform;
}

SI16 cltSummonInfo::GetSummonTranformIndex(void)
{
	return m_siSummonTranformIndex;
}

void cltSummonInfo::SetSummonTranformIndex( SI16 _siSummonTranformIndex )
{
	m_siSummonTranformIndex = _siSummonTranformIndex;
}

bool cltSummonInfo::IsSummonTranformTickCount( void )
{
	return (TABS(GetTickCount() - m_dwSummonTranformTickCount) > 1000) ? true : false;
}

void cltSummonInfo::SetSummonTranformTickCount( void )
{
	m_dwSummonTranformTickCount = GetTickCount();
}

void cltSummonInfo::EndSummonTranformTickCount( void )
{
	m_dwSummonTranformTickCount = 0;
}


void cltSummonInfo::SummonInit(SI16 index)
{
	if(index>=0 && index<MAX_SUMMON_OWN)	clSummon[index].Init();
}

BOOL cltSummonInfo::IsHaveEquipItem( SI16 siSummonIndex )
{
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return FALSE;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return FALSE;
	}

	SI32 siIndex = 0;

	for ( siIndex=0; siIndex<MAX_SHAPE_ITEM; siIndex++ )
	{
		if ( 0 < clItemInfo[ siSummonIndex ].clEquipItem[ siIndex ].siUnique )
		{
			return TRUE;
		}
	}
	
	return FALSE;

}

void cltSummonInfo::SetSummonEquipItem( SI16 siSummonIndex, SI16 siItemPos, cltItem* pclItem )
{
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return;
	}

	if ( (0 > siItemPos) || (MAX_SHAPE_ITEM <= siItemPos) )
	{
		return;
	}

	if ( NULL == pclItem )
	{
		return;
	}

	clItemInfo[ siSummonIndex ].clEquipItem[ siItemPos ].Set( pclItem );
}

cltItem* cltSummonInfo::GetSummonEquipItem( SI16 siSummonIndex, SI16 siItemPos )
{
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return NULL;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return NULL;
	}

	if ( (0 > siItemPos) || (MAX_SHAPE_ITEM <= siItemPos) )
	{
		return NULL;
	}

	return &clItemInfo[ siSummonIndex ].clEquipItem[ siItemPos ];

}

cltItem* cltSummonInfo::GetSummonEquipItemList( SI16 siSummonIndex )
{
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return NULL;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return NULL;
	}

	return &clItemInfo[ siSummonIndex ].clEquipItem[0];
}

BOOL cltSummonInfo::IsHaveEquipItemFromItemUnique( SI16 siSummonIndex, SI32 siItemUnique )
{
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return FALSE;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return FALSE;
	}

	SI32 siIndex=0;

	for( siIndex=0; siIndex<MAX_SHAPE_ITEM; siIndex++ )
	{
		if ( siItemUnique == clItemInfo[ siSummonIndex ].clEquipItem[ siIndex ].siUnique )
		{
			return TRUE;
		}
	}

	return FALSE;
}

SI32 cltSummonInfo::GetHaveItemNumFromItemUnique( SI16 siSummonIndex, SI32 siItemUnique )
{
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return 0;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return 0;
	}

	SI32 siIndex=0;

	for( siIndex=0; siIndex<MAX_SHAPE_ITEM; siIndex++ )
	{
		if ( siItemUnique == clItemInfo[ siSummonIndex ].clEquipItem[ siIndex ].siUnique )
		{
			return clItemInfo[ siSummonIndex ].clEquipItem[ siIndex ].siItemNum;
		}
	}

	return 0;

}

SI32 cltSummonInfo::GetItemPosInEquip( SI16 siSummonIndex, SI32 siItemUnique )
{
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return -1;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return -1;
	}

	SI32 siIndex=0;

	for( siIndex=0; siIndex<MAX_SHAPE_ITEM; siIndex++ )
	{
		if ( siItemUnique == clItemInfo[ siSummonIndex ].clEquipItem[ siIndex ].siUnique )
		{
			return siIndex;
		}
	}

	return -1;
}

SI32 cltSummonInfo::GetSummonEquipPoisonRate( SI16 siSummonIndex, cltItemManagerCommon* pclItemManager, SI16 siActiveweapon )
{
#if defined(_DBMANAGER)
	return 0;
#else
	// 인수가 유효한지 검사 시작
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return 0;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return 0;
	}

	if ( NULL == pclItemManager )
	{
		return 0;
	}

	if ( (0 > siActiveweapon) || (MAX_SHAPE_ITEM <= siActiveweapon) )
	{
		return 0;
	}
	// 인수가 유효한지 검사 끝

	cltItem* pclItem = &clItemInfo[ siSummonIndex ].clEquipItem[ siActiveweapon ];
	if ( (NULL == pclItem) || (0 >= pclItem->siUnique) )
	{
		return 0;
	}

	SI64 siAtb = pclItemManager->GetItemAtb(pclItem->siUnique);
	if ( (siAtb & ITEMATB_NEARWEAPON) == 0 )
	{
		return 0;
	}

	SI32 siTotalRate = 0;

	if ( pclItem->clItemCommon.clOptionInfo.uiType1 == SCROLL_UNIQUE_FUNCTION_POISON)
	{
		siTotalRate += pclItem->clItemCommon.clOptionInfo.uiPara1;
	}

	if ( pclItem->clItemCommon.clOptionInfo.uiType2 == SCROLL_UNIQUE_FUNCTION_POISON)
	{
		siTotalRate += pclItem->clItemCommon.clOptionInfo.uiPara2;
	}

	return siTotalRate;
#endif
}

SI32 cltSummonInfo::GetSummonEquipIceRate( SI16 siSummonIndex, cltItemManagerCommon* pclItemManager, SI16 siActiveweapon)
{
#if defined(_DBMANAGER)
	return 0;
#else
	// 인수가 유효한지 검사 시작
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return 0;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return 0;
	}

	if ( NULL == pclItemManager )
	{
		return 0;
	}

	if ( (0 > siActiveweapon) || (MAX_SHAPE_ITEM <= siActiveweapon) )
	{
		return 0;
	}
	// 인수가 유효한지 검사 끝

	cltItem* pclItem = &clItemInfo[ siSummonIndex ].clEquipItem[ siActiveweapon ];
	if ( (NULL == pclItem) || (0 >= pclItem->siUnique) )
	{
		return 0;
	}

	SI64 siAtb = pclItemManager->GetItemAtb(pclItem->siUnique);
	if ( (siAtb & ITEMATB_FARWEAPON) == 0 )
	{
		return 0;
	}

	SI32 siTotalRate = 0;

	if ( pclItem->clItemCommon.clOptionInfo.uiType1 == SCROLL_UNIQUE_FUNCTION_ICE)
	{
		siTotalRate += pclItem->clItemCommon.clOptionInfo.uiPara1;
	}

	if ( pclItem->clItemCommon.clOptionInfo.uiType2 == SCROLL_UNIQUE_FUNCTION_ICE)
	{
		siTotalRate += pclItem->clItemCommon.clOptionInfo.uiPara2;
	}

	return siTotalRate;
#endif
}

SI32 cltSummonInfo::GetSummonEquipDrainRate( SI16 siSummonIndex, cltItemManagerCommon* pclItemManager,SI16 siActive )
{
#if defined(_DBMANAGER)
	return 0;
#else
	// 인수가 유효한지 검사 시작
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return 0;
	}

	if ( (0 >= clSummon[ siSummonIndex ].siKind) || (MAX_KIND_NUMBER <= clSummon[ siSummonIndex ].siKind) )
	{
		return 0;
	}

	if ( NULL == pclItemManager )
	{
		return 0;
	}

	if ( (0 > siActive) || (MAX_SHAPE_ITEM <= siActive) )
	{
		return 0;
	}
	// 인수가 유효한지 검사 끝

	cltItem* pclItem = &clItemInfo[ siSummonIndex ].clEquipItem[ siActive ];
	if ( (NULL == pclItem) || (0 >= pclItem->siUnique) )
	{
		return 0;
	}

	SI64 siAtb = pclItemManager->GetItemAtb(pclItem->siUnique);
	if ( (siAtb & ITEMATB_SHOES) == 0 )
	{
		return 0;
	}

	SI32 siTotalRate = 0;

	if ( pclItem->clItemCommon.clOptionInfo.uiType1 == SCROLL_UNIQUE_FUNCTION_DRAIN )
	{
		siTotalRate += pclItem->clItemCommon.clOptionInfo.uiPara1;
	}

	if ( pclItem->clItemCommon.clOptionInfo.uiType2 == SCROLL_UNIQUE_FUNCTION_DRAIN )
	{
		siTotalRate += pclItem->clItemCommon.clOptionInfo.uiPara2;
	}

	return siTotalRate;
#endif
}


//void cltSummonInfo::SetCurrentSummon(cltSummon *pclSummon)
//{
//if(siCurrentSummon>=0 && siCurrentSummon<MAX_SUMMON_OWN)	clSummon[siCurrentSummon].Set(pclSummon);
//}
//
//cltSummon* cltSummonInfo::GetCurrentSummon()
//{
//if(siCurrentSummon>=0 && siCurrentSummon<MAX_SUMMON_OWN)	
//{
//if(clSummon[siCurrentSummon].siKind >0 && clSummon[siCurrentSummon].siKind <MAX_KIND_NUMBER) 
//return &clSummon[siCurrentSummon];
//}
//return NULL;
//}
//
//void cltSummonInfo::CurrentSummonInit()
//{
//if(siCurrentSummon>=0 && siCurrentSummon<MAX_SUMMON_OWN)	clSummon[siCurrentSummon].Init();
//}

//===============================
// cltHeroDataBackup
//===============================
cltHeroDataBackup::cltHeroDataBackup()
{
	siWarEnemyKillNumber		= 0;
	siWarKilledNumber			= 0;
	siBlackArmyWarMapIndex		= 0;

	siBlackWarEnemyKillNumber	= 0;
	siBlackWarKilledNumber		= 0;

	bStartMakeItemSwitch		= false;
	dwStartMakeItemClock		= 0;
	dwDelayMakeItemClock		= 0;	
	siOldActiveClothes			= 0;
	siOldActiveHat				= 0;
	siOldActiveWeapon			= 0;
	siOldCurrnetMagic			= 0;
	siOldCurrnetMagicCastType	= 0;

	clOldMagicBufMgr.Init();

	ZeroMemory(&stMuteTime, sizeof(stMuteTime));
	
}

cltHeroDataBackup::~cltHeroDataBackup()
{
}

SI32 GetChannelingUser(TCHAR* pszID)
{
	SI32 siRet = CHANNELING_USER_NONE;
#ifndef _DBMANAGER

	NTCHARString32	kID;
	kID = pszID;
	NTCHARString32	kIdentifier;

	if( kID.Length() <= 0 )					return siRet;

	SI32 siPos = kID.Find("@");
	if( siPos < 0 )
	{
		kIdentifier = "";
	}
	else
	{
		kIdentifier = kID.SubStr( siPos );
	}

	if( pclClient->IsWhereServiceArea(ConstServiceArea_Japan) )
	{
		if		( kIdentifier == "@ND" )			siRet = CHANNELING_USER_JAPAN_NDE;
		else if	( kIdentifier == "@HG" )			siRet = CHANNELING_USER_JAPAN_HANGAME;
		else if	( kIdentifier == "@GP" )			siRet = CHANNELING_USER_JAPAN_GAMEPOT;
		else if	( kIdentifier == "@GA" )			siRet = CHANNELING_USER_JAPAN_GAMANIA;
		else if	( kIdentifier == "@NX" )			siRet = CHANNELING_USER_JAPAN_NEXON;

	}
	else if ( pclClient->IsWhereServiceArea(ConstServiceArea_English) )
	{
		if		( kIdentifier == "@nh" )			siRet = CHANNELING_USER_ENGLISH_USA;
		else										siRet = CHANNELING_USER_ENGLISH_NDI;
	}
#endif

	return siRet;
}

