#pragma once

//****************************************************************************** 
// 
// * 기부 관련 이벤트들은 다 이것을 상속받게 한다.
//
// * 기부에 기본인 기부 아이템의 종류, 기부 받는 아이템  최대수, 기부를 받는 NPC의 카인드를 기본정보로 가진다.
//
//******************************************************************************

#include "CommonHeader.h"
#include "CommonLogic.h"

#define CONTRIBUTION_ITEM_NUM	  6	// 기부 아이템의 최대 개수.
#define CONTRIBUTION_INFO_NUM	100	// 기부 관련 정보 최대 개수.

class cltContributionInfo
{
public:
	SI32	m_siNPCKind;
	SI32	m_siMakeNPCKind;
	SI32	m_siItemUnique[CONTRIBUTION_ITEM_NUM];	// 기부가 가능한 아이템의 유니크.
	SI16	m_siItemNum[CONTRIBUTION_ITEM_NUM];		// 기부가 가능한 아이템의 수.
	bool	m_bMyVillage;							// 내 마을에만 기부와 NPC(구조물) 만들기가 가능하나.
};


class cltContributionEvent_Common
{
public:
	cltContributionEvent_Common( void );
	~cltContributionEvent_Common( void );

	bool					FileLoad( void );
	void					Clear( void );
	cltContributionInfo*	FindContributionInfo( SI32 siNPCKind );
	SI32					GetMakeNPC( SI32 siNPCKind );									// 해당 NPC로 만들수 있는 NPC를 리턴.

	bool					Insert( SI32 _siNPCKind, 
									void* _pClass, 
									bool (*_pMakeNpcFunc)( void* _pClass, cltCharServer* _pclChar ), 
									bool (*_pCompleteMakeNpcFunc)( void* _pClass, cltCharServer* _pclChar ), 
									bool (*_pContributionFunc)( void* _pClass, cltCharServer* _pclChar ) );

	bool					IsMakeNpc( SI32 _siNPCKind, cltCharServer* _pclChar );									// npc를 만들때 조건 여부 확인에 함수 사용.
	bool					CompleteMakeNpc( SI32 _siNPCKind, cltCharServer* _pclChar );							// npc를 만들었을 때 취하는 행동 함수.
	bool					IsContribution( SI32 _siNPCKind, cltCharServer* _pclChar );								// 기부시 기부 가능 여부를 확인에 사용.	
	void					ContributionItemListDelete( SI32 m_siVillageUnique, SI32 m_siNPCKind );					// 기부한 아이템을 삭제한다.
	
private:
	static bool					m_bCreate;
	static cltContributionInfo* m_pclContributionInfo[CONTRIBUTION_INFO_NUM];

	static void*				m_pClass[MAX_KIND_NUMBER];										// 각 클레스. 
	static bool					(*m_pMakeNpcFunc[MAX_KIND_NUMBER])( void* _pClass, cltCharServer* _pclChar );			// npc를 만드는게 가능한지 검사할 함수가 등록. (건물 완성 전 확인)
	static bool					(*m_pCompleteMakeNpcFunc[MAX_KIND_NUMBER])( void* _pClass, cltCharServer* _pclChar );	// npc를 만드는데 성공하면 호출될 함수가 등록. (건물 완성 시)
	static bool					(*m_pContributionFunc[MAX_KIND_NUMBER])( void* _pClass, cltCharServer* _pclChar );		// 기부가 가능한지 검사할 함수가 등록.
};

