#pragma once

//****************************************************************************** 
// 
// * 자이언트 라쿤 부활 축제.
//
//******************************************************************************

#include "CommonHeader.h"
#include "CommonLogic.h"

class cltGRREHuntMapMonsterInfo;
class cltGRREVillageInfo;
class cltContributionEvent_Common;
class cltContributionInfo;

//==============================================================================
// - 자이언트 라쿤 부활 축제 정보. GiantRaccoonResurrectEvent => GRRE
//==============================================================================
class cltGiantRaccoonResurrectEvent : public cltContributionEvent_Common
{
public:
	enum { POTRACCOON_CONTRIBUTION_ITEM_NUM = 10, };

public:
	cltGiantRaccoonResurrectEvent( void );
	~cltGiantRaccoonResurrectEvent( void );

	bool	FileLoad( void );
	void	Clear( void );
	bool	Init( TCHAR* _pszMap, TCHAR* _pszEventHour, SI32 _siRegenMonsterKind, SI32 siMapRegenMonsterNum, SI32 siMapOnceRegenMonsterNum );
	void	Action( void );

	bool	IsEventActiveTime( void );
	bool	IsInputItem( void );

	void	SetFirstMakeVillage( SI32 _siFirstMakeVillage );
	SI32	GetFirstMakeVillage( void );
	bool	IsFirstMakeVillage( SI32 _siFirstMakeVillage );

private:
	void	CheckNPC( void );
	void	Start( void );											// 이벤트 시작.
	void	End( void );											// 이벤트 종료.
	void	Run( void );											// 이벤트 진행.

	void	MonsterDel( void );										// 몬스터 삭제.
	void	InputItemListDel( void );								// 현재까지 넣은 기부 아이템을 삭제한다.			
	void	MonsterRegen( void );									// 각 사냥터에 몬스터 리젠.
	SI32	GetTotalRegenMonsterCount( void );						// 사냥터에 리젠 된 몬스터의 전체 누적수

	static bool	static_IsMakeNpc( void* _pclGiantRaccoonResurrectEvent, cltCharServer* _pclChar );
	static bool	static_CompleteMakeNpc( void* _pclGiantRaccoonResurrectEvent, cltCharServer* _pclChar );
	static bool	static_IsContribution( void* _pclGiantRaccoonResurrectEvent, cltCharServer* _pclChar );
	
	bool	IsMakeNpc( cltCharServer* _pclChar );
	bool	CompleteMakeNpc( cltCharServer* _pclChar );
	bool	IsContribution( cltCharServer* _pclChar );

private:
	bool							m_bEventActive;					// 이벤트 시간인가.
	bool							m_bInputItem;					// 아이템 기부가 가능한가.
	SI32							m_siTotalMapNum;				// 위의 맵 개수.
	SI32							m_siTotalRegenMonsterNum;		// 전체 리젠 될 몬스터의 수.
	DWORD							m_dwMonsterRegenTickCount;		// 몬스터 리젠 틱 카운트.
	DWORD							m_dwBeginMonsterRegenTickCount;	// 몬스터 리젠 틱 카운트.
	

	cltGRREHuntMapMonsterInfo*		m_pclGRREHuntMapMonsterInfo;	// 관련 맵 정보.
	cltGRREVillageInfo*				m_pclGRREVillageInfo;			// 이벤트 관련 마을 정보.
	bool							m_bEventHour[24];				// 이벤트 시간인가?

	cltContributionInfo*			m_pclContributionInfo;			// 내 NpcKind에 관련된 기부 정보.			

	SI32							m_siNpcID;						// 빵을 기부하는 NPC
};

//==============================================================================
// - 자이언트 라쿤 부활 축제 때 리젠될 몬스터 정보.
//==============================================================================
class cltGRREHuntMapMonsterInfo
{
public:
	cltGRREHuntMapMonsterInfo( void );
	~cltGRREHuntMapMonsterInfo( void );

	void	Init( SI32 _siMapIndex, SI32 _siRegenMonsterNum );
	void	Clear( void );
	SI32	GetMapIndex( void );											// 맵 인덱스 리턴.
	void	MonsterDel( void );												// 몬스터 삭제.
	void	MonsterRegen( void );											// 해당 맵에 몬스터를 리젠 시킨다.
	SI32	GetRegenMonsterCount( void );									// 리젠 된 몬스터의 누적 수.
	void	SetRegenMonsterKind( SI32 _siRegenMonsterKind );
	void	SetMapRegenMonsterNum( SI32 _siMapRegenMonsterNum );
	void	SetMapOnceRegenMonsterNum( SI32 _siMapOnceRegenMonsterNum );

private:
	SI32	m_siMapIndex;								// 맵 인덱스
	SI32*	m_psiMonsterID;								// 해당 맵에 리젠 된 몬스터의 ID
	SI32	m_siRegenMonsterCount;						// 리젠 된 몬스터의 누적 수.

public:	
	static SI32	m_siRegenMonsterKind;					// 리젠되는 몬스터 종류. 
	static SI32	m_siMapRegenMonsterNum;					// 맵에 리젠되는 전체 몬스터의 수.
	static SI32	m_siMapOnceRegenMonsterNum;				// 맵에 1회 리젠 시 생성되는 몬스터의 수. 
};

//==============================================================================
// - 최초로 부활탑을 건설한 마을.
//==============================================================================
class cltGRREVillageInfo
{
public:
	cltGRREVillageInfo( void );
	~cltGRREVillageInfo( void );

	void	Clear( void );

	void	SetFirstMakeVillage( SI32 _siFirstMakeVillage );
	SI32	GetFirstMakeVillage( void );
	bool	IsFirstMakeVillage( SI32 _siFirstMakeVillage );

private:
	SI32	m_siFirstMakeVillage;
};
