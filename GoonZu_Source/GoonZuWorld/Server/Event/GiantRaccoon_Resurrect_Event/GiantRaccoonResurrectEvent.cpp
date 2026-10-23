#include "../../../common/Event/event.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Cityhall.h"

#include "../Server/Server.h"
#include "../CommonLogic/MsgType-person.h"
#include "../CommonLogic/Cityhall/Cityhall.h"
#include "../CommonLogic/MsgRval-Define.h"
#include "../ContributionEvent_Common/ContributionEvent_Common.h"

#include "GiantRaccoonResurrectEvent.h"


//==============================================================================
// - 자이언트 라쿤 부활 축제 정보.
//==============================================================================
cltGiantRaccoonResurrectEvent::cltGiantRaccoonResurrectEvent( void )
{
	Clear();

	// 상속받은 cltContributionEvent_Common에 나를 등록한다.
	if( false == Insert( pclClient->GetUniqueFromHash( "KIND_RACCOONTREEBASE" ), 
						 this, 
						 static_IsMakeNpc, 
						 static_CompleteMakeNpc,
						 static_IsContribution )		)
	{
		MessageBox( NULL, TEXT("Error"), TEXT("cltGiantRaccoonResurrectEvent() Insert() Fail"), 0 );
	}

	
	if( false == FileLoad() ) 
		MessageBox( NULL, TEXT("Error"), TEXT("cltGiantRaccoonResurrectEvent() Fail"), 0 );

	m_pclContributionInfo = FindContributionInfo( pclClient->GetUniqueFromHash( "KIND_RACCOONTREEBASE" ) );
	if( NULL == m_pclContributionInfo ) 
		MessageBox( NULL, TEXT("Error"), TEXT("cltGiantRaccoonResurrectEvent() ContributionInfo Fail"), 0 );

}

cltGiantRaccoonResurrectEvent::~cltGiantRaccoonResurrectEvent( void )
{
	SAFE_DELETE( m_pclGRREVillageInfo );
	SAFE_DELETE_ARRAY( m_pclGRREHuntMapMonsterInfo );
}


void	cltGiantRaccoonResurrectEvent::Clear( void )
{
	ZeroMemory( this, sizeof(*this) );
}

bool	cltGiantRaccoonResurrectEvent::FileLoad( void )
{
	bool		bLoadFile = false;
	NDataLoader	dataloader;
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( "Data/GiantRaccoonResurrectEvent.txt" );		
	}
	else
	{
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
		{
			bLoadFile = dataloader.LoadDataFromCryptFile( "Data/GiantRaccoonResurrectEvent.txt" );
		}
		else
		{
			bLoadFile = dataloader.LoadDataFromFile( "Data/GiantRaccoonResurrectEvent.txt" );
		}
	}
	
	TCHAR	szMonsterKind[128]		 = "";
	TCHAR	szMap[512]				 = "";
	TCHAR	szEventHour[256]		 = "";
	SI32	siMapRegenMonsterNum	 = 0;
	SI32	siMapOnceRegenMonsterNum = 0;
	SI32	siMonsterRegenTickCount	 = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szMonsterKind,				sizeof(szMonsterKind),
		NDATA_MBSTRING,	szMap,						sizeof(szMap),
		NDATA_MBSTRING,	szEventHour,				sizeof(szEventHour),
		NDATA_INT32,	&siMapRegenMonsterNum,		sizeof(siMapRegenMonsterNum),
		NDATA_INT32,	&siMapOnceRegenMonsterNum,	sizeof(siMapOnceRegenMonsterNum),
		NDATA_INT32,	&siMonsterRegenTickCount,	sizeof(siMonsterRegenTickCount),
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );
	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData() )
		{
			m_dwMonsterRegenTickCount = (DWORD)siMonsterRegenTickCount * 1000; // 초
			
			if( false == Init(  szMap, 
								szEventHour,
								pclClient->GetUniqueFromHash( szMonsterKind ), 
								siMapRegenMonsterNum, 
								siMapOnceRegenMonsterNum ) ) 
			{
				return false;
			}
		}
	}

	return bLoadFile;
}

bool	cltGiantRaccoonResurrectEvent::Init( TCHAR* _pszMap, TCHAR* _pszEventHour, SI32 _siRegenMonsterKind, SI32 siMapRegenMonsterNum, SI32 siMapOnceRegenMonsterNum )
{
	SI32 siMapCount		 = 0;
	SI32 siMapIndex[128] = { 0, };
	TCHAR* pszData = _tcstok(_pszMap, ":");
	while( pszData )
	{
		siMapIndex[siMapCount++] = _ttoi(pszData);
		
		pszData = _tcstok(NULL, ":");
	}

	m_siTotalMapNum			 = siMapCount;
	m_siTotalRegenMonsterNum = m_siTotalMapNum * siMapRegenMonsterNum;

	m_pclGRREVillageInfo		 = new cltGRREVillageInfo;
	if( NULL == m_pclGRREVillageInfo ) return false;

	m_pclGRREHuntMapMonsterInfo  = new cltGRREHuntMapMonsterInfo[m_siTotalMapNum];
	if( NULL == m_pclGRREHuntMapMonsterInfo ) return false;

	for( SI32 siCount = 0; siCount < m_siTotalMapNum; ++siCount )   
	{
		m_pclGRREHuntMapMonsterInfo[siCount].Init( siMapIndex[siCount], siMapOnceRegenMonsterNum );
	}
	
	m_pclGRREHuntMapMonsterInfo->SetRegenMonsterKind( _siRegenMonsterKind );
	m_pclGRREHuntMapMonsterInfo->SetMapRegenMonsterNum( siMapRegenMonsterNum );
	m_pclGRREHuntMapMonsterInfo->SetMapOnceRegenMonsterNum( siMapOnceRegenMonsterNum );

	pszData = _tcstok(_pszEventHour, ",");
	pszData++;
	while( pszData )
	{
		m_bEventHour[_ttoi(pszData)] = true;

		pszData = _tcstok(NULL, ",");
	}
	
	
	return true;
}

void	cltGiantRaccoonResurrectEvent::Action( void )
{
	// DB 초기화 완료될때까지 기다림
	if( ((cltServer *)pclClient)->bCompleteDBInitAll == FALSE ) return;
	
	// 서버가 최초 실행되면 그동안 기부되었던 아이템을 삭제한다.
	static bool bInputItemListDel = false;
	if( false == bInputItemListDel && false == IsEventActiveTime() ) 
	{
		InputItemListDel();
	}
	bInputItemListDel = true;
	

	cltServer* pclserver = (cltServer*)pclClient;
	if( pclserver->pclEventTimeManager->InEventTime("GiantRaccoon_Resurrect_Event", &pclserver->sTime ) )
	{
		// 생성된 NPC 생성
		if( 0 >= m_siNpcID )
		{
			m_siNpcID = pclserver->SetNPCChar( pclClient->GetUniqueFromHash("KIND_POTRACCOON"), 
											VILLAGE_NONE, 
											NULL, 
											1, 
											0, 
											242, 
											313, 
											0, 
											NPCITEMMODE_NONE, 
											NULL );
		}

		bool bEventActiveTime = IsEventActiveTime();

		// 이벤트 시간이고 이벤트가 진행중이 아니면 시작.
		if( true == bEventActiveTime && false == m_bEventActive ) 
		{
			Start();		
		}
		// 이벤트 시간이 아니고 이벤트가 진행중이면 종료.
		else if( false == bEventActiveTime && true == m_bEventActive )
		{
			End();
		}
		// 이벤트 시간이고 이벤트가 진행 중이면.
		else if( true == bEventActiveTime && true == m_bEventActive )
		{
			Run();
		}
	}
	else
	{
		// 생성된 NPC 삭제
		if( 0 < m_siNpcID )
		{
			cltCharServer* pclChar = pclserver->pclCM->GetCharServer( m_siNpcID );
			if( pclChar )
			{
				pclChar->DeleteInCell();
				pclserver->pclCM->DeleteChar( pclChar->GetID() );

				m_siNpcID = 0;
			}
		}
	}

	CheckNPC();
}
void	cltGiantRaccoonResurrectEvent::CheckNPC( void )
{
	static DWORD TickCount = 0;
	if( TABS(GetTickCount() - TickCount) < 10000 ) return;
	TickCount = GetTickCount();

	cltServer*	pclserver = (cltServer*)pclClient;

	bool bEventTime = false;
	if( pclserver->pclEventTimeManager->InEventTime("GiantRaccoon_Resurrect_Event", &pclserver->sTime ) )
	{
		bEventTime = true;
	}
			
	bool		bNPC			= true;
	cltStrInfo* pclStrInfo		= NULL;
	SI32		siHireNpcIndex	= -1;
	SI32		siFireNpcIndex	= -1;
	for( SI32 i = 0; i < MAX_VILLAGE_NUMBER; i++ )
	{
		if( i == Const_Beginner_Village ) continue;

		pclStrInfo = pclserver->pclCityHallManager->GetStrInfo( i );
		if( pclStrInfo == NULL )	continue;

		bNPC			= false;
		siHireNpcIndex	= -1;		// 고용이 가능한 인덱스 번호.
		siFireNpcIndex	= -1;		// 고용을 해제하는 인덱스 번호.
        for( SI32 siCount = 0; siCount < MAX_HIRED_EVENTNPC_NUM; siCount++ )
		{
			// 고용이 가능한 인덱스 번호는 한번 ~세팅하면 세팅하지 않아도 된다.
			if( 0 > siHireNpcIndex && 0 >= pclStrInfo->clCityhallStrInfo.clHiredEventNPC[siCount].siNPCKind )
				siHireNpcIndex = siCount;

			if(		pclStrInfo->clCityhallStrInfo.clHiredEventNPC[siCount].siNPCKind == m_pclContributionInfo->m_siNPCKind
				||	pclStrInfo->clCityhallStrInfo.clHiredEventNPC[siCount].siNPCKind == m_pclContributionInfo->m_siMakeNPCKind ) 
			{
				bNPC			= true;
				siFireNpcIndex	= siCount;
				break;
			}
		}

		// 이벤트 기간이면서 마을에 구조물 npc가 있고 하지만 이벤트 시간이 아니면.
		if( bEventTime && bNPC && false == IsEventActiveTime() )
		{
			// 그리고 그게 완성된 NPC라면.
			if( pclStrInfo->clCityhallStrInfo.clHiredEventNPC[siFireNpcIndex].siNPCKind == m_pclContributionInfo->m_siMakeNPCKind )
			{
				sDBRequest_HireNPC clFireMsg(	0, 
												i, 
												siFireNpcIndex, 
												0, 
												0, 
												0, 
												&pclserver->pclTime->clDate, 
												HIRENPC_HIREEVENTNPC );

				pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clFireMsg );
				
				sDBRequest_HireNPC clHireMsg(	0, 
												i, 
												siFireNpcIndex, 
												m_pclContributionInfo->m_siNPCKind, 
												0, 
												0, 
												&pclserver->pclTime->clDate, 
												HIRENPC_HIREEVENTNPC );

				pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clHireMsg );
			}
		}
		// 이벤트 기간이면서 마을에 구조물 npc가 없을 경우 만든다.
		if( bEventTime && false == bNPC )
		{
			sDBRequest_HireNPC clHireMsg(	0, 
											i, 
											siHireNpcIndex, 
											m_pclContributionInfo->m_siNPCKind, 
											0, 
											0, 
											&pclserver->pclTime->clDate, 
											HIRENPC_HIREEVENTNPC );
			
			pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clHireMsg );
		}

		// 이벤트 기간 아닌데 마을에 구조물 npc가 있을 경우 삭제한다.
		if( false == bEventTime && bNPC )
		{
			sDBRequest_HireNPC clFireMsg(	0, 
											i, 
											siFireNpcIndex, 
											0, 
											0, 
											0, 
											&pclserver->pclTime->clDate, 
											HIRENPC_HIREEVENTNPC );

			pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clFireMsg );
		}
	}
}

void	cltGiantRaccoonResurrectEvent::Start( void )
{
	m_bInputItem					= true;
	m_bEventActive					= true;
	m_dwBeginMonsterRegenTickCount	= GetTickCount();
	
	MonsterRegen();

	m_pclGRREVillageInfo->Clear();

	cltServer*	pclserver = (cltServer*)pclClient;
	cltMsg clMsg( GAMEMSG_RESPONSE_GIANTRACCOON_RESURRECT_EVENT_START, PACKET_EMPTY );
	pclserver->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
}

void	cltGiantRaccoonResurrectEvent::End( void )
{
	m_bEventActive = false;

	MonsterDel();
	InputItemListDel();
}
void	cltGiantRaccoonResurrectEvent::Run( void )
{
	if( m_dwMonsterRegenTickCount > TABS( GetTickCount() - m_dwBeginMonsterRegenTickCount) ) return;
	
	m_dwBeginMonsterRegenTickCount = GetTickCount();

	MonsterRegen();	
}

bool	cltGiantRaccoonResurrectEvent::IsEventActiveTime( void )
{
	cltServer*	pclserver = (cltServer*)pclClient;
	
	return m_bEventHour[pclserver->sTime.wHour];
}

bool	cltGiantRaccoonResurrectEvent::IsInputItem( void )
{
	return m_bInputItem;
}

void	cltGiantRaccoonResurrectEvent::SetFirstMakeVillage( SI32 _siFirstMakeVillage )
{
	m_bInputItem = false;

	m_pclGRREVillageInfo->SetFirstMakeVillage( _siFirstMakeVillage );
}

SI32	cltGiantRaccoonResurrectEvent::GetFirstMakeVillage( void )
{
	return m_pclGRREVillageInfo->GetFirstMakeVillage();
}

bool	cltGiantRaccoonResurrectEvent::IsFirstMakeVillage( SI32 _siFirstMakeVillage )
{
	return m_pclGRREVillageInfo->IsFirstMakeVillage( _siFirstMakeVillage );
}

void	cltGiantRaccoonResurrectEvent::MonsterDel( void )
{
	for( SI32 siCount = 0; siCount < m_siTotalMapNum; ++siCount )
	{
		m_pclGRREHuntMapMonsterInfo[siCount].MonsterDel();
	}
}

void	cltGiantRaccoonResurrectEvent::InputItemListDel( void )
{
	cltServer*	pclserver  = (cltServer*)pclClient;
	cltStrInfo* pclStrInfo = NULL;
	for( SI32 siVillageCount = 0; siVillageCount < MAX_VILLAGE_NUMBER; siVillageCount++ )
	{
		pclStrInfo = pclserver->pclCityHallManager->GetStrInfo( siVillageCount );
		if( pclStrInfo == NULL )	continue;

		for( SI32 siCount = 0; siCount < MAX_HIRED_EVENTNPC_NUM; siCount++ )
		{
			if(		pclStrInfo->clCityhallStrInfo.clHiredEventNPC[siCount].siNPCKind == m_pclContributionInfo->m_siNPCKind
				||	pclStrInfo->clCityhallStrInfo.clHiredEventNPC[siCount].siNPCKind == m_pclContributionInfo->m_siMakeNPCKind ) 
			{
				if( IsFirstMakeVillage( siVillageCount ) )
					ContributionItemListDelete( siVillageCount, m_pclContributionInfo->m_siNPCKind );
				break;
			}
		}
	}
}

void	cltGiantRaccoonResurrectEvent::MonsterRegen( void )
{
	if( GetTotalRegenMonsterCount() >= m_siTotalRegenMonsterNum ) return;

	SI32 siTotalRegenMonsterCount = 0;
	for( SI32 siCount = 0; siCount < m_siTotalMapNum; ++siCount )
	{
		m_pclGRREHuntMapMonsterInfo[siCount].MonsterRegen();
	}
}

SI32	cltGiantRaccoonResurrectEvent::GetTotalRegenMonsterCount( void )
{
	SI32 siTotalRegenMonsterCount = 0;
	for( SI32 siCount = 0; siCount < m_siTotalMapNum; ++siCount )
	{
		siTotalRegenMonsterCount += m_pclGRREHuntMapMonsterInfo[siCount].GetRegenMonsterCount();
	}

	return siTotalRegenMonsterCount;
}

bool	cltGiantRaccoonResurrectEvent::static_IsMakeNpc( void* _pclGiantRaccoonResurrectEvent, cltCharServer* _pclChar )
{
	return ((cltGiantRaccoonResurrectEvent*)_pclGiantRaccoonResurrectEvent)->IsMakeNpc( _pclChar );
}

bool	cltGiantRaccoonResurrectEvent::static_CompleteMakeNpc( void* _pclGiantRaccoonResurrectEvent, cltCharServer* _pclChar )
{
	return ((cltGiantRaccoonResurrectEvent*)_pclGiantRaccoonResurrectEvent)->CompleteMakeNpc( _pclChar );
}

bool	cltGiantRaccoonResurrectEvent::static_IsContribution( void* _pclGiantRaccoonResurrectEvent, cltCharServer* _pclChar )
{
	return ((cltGiantRaccoonResurrectEvent*)_pclGiantRaccoonResurrectEvent)->IsContribution( _pclChar );
}

bool	cltGiantRaccoonResurrectEvent::IsMakeNpc( cltCharServer* _pclChar )
{
	// 이벤트 시간이 아니면 만들기가 불가능.
	if( false == IsEventActiveTime() ) return false;

	// 기부가 가능한가. 만들기가 불가능하면 이건 어떤 마을에서 이미 부활탑을 건설했기 때문이다.
	if( false == IsInputItem()		  ) return false;

	return true;
}

bool	cltGiantRaccoonResurrectEvent::CompleteMakeNpc( cltCharServer* _pclChar )
{
	SetFirstMakeVillage( _pclChar->GetCurrentVillageUnique() );

	cltServer*	pclserver  = (cltServer*)pclClient;
	
	SI32			siID		= 0;
	SI32			siIndex		= 0;
	cltCharServer*	pclChar		= NULL;
	SI32			siUsetime	= pclserver->pclTime->clDate.GetHourVary() + 24; // 현실시간 48분간.
	
	while( siID = pclserver->pclCM->pclIDManager->GetCharOrder(siIndex) )
	{
		siIndex++;
		pclChar = pclserver->pclCM->GetCharServer( siID );
		if( NULL== pclChar || false == IsPC(siID) ) continue;

		if( IsFirstMakeVillage( pclChar->pclCI->clBI.siHomeVillage ) )
		{
			SI32 siSlot = pclChar->pclCI->clBI.clBufInfo.FindEmptySlot( BUF_GIANTRACCOON_RESURRECT_EVENT_FIRST_VILLAGE, false );	
			if( -1 >= siSlot ) continue;
			
			stBuf addBuf( BUF_GIANTRACCOON_RESURRECT_EVENT_FIRST_VILLAGE, 5, siUsetime );
			pclChar->AddBufThroughDB( siSlot, &addBuf , false, 0, NULL, 0 );
		}
	}

	return true;
}

bool	cltGiantRaccoonResurrectEvent::IsContribution( cltCharServer* _pclChar )
{
	// 이벤트 시간이 아니면 기부가 불가능.
	if( false == IsEventActiveTime() ) 
	{
		((cltServer*)pclClient)->SendServerResponseMsg( 0, SRVAL_GIANTRACCOONRESURRECTEVENT_NO_TIME, 0, 0, _pclChar->GetID() );
		return false;
	}

	// 기부가 가능한가. 기부가 불가능하면 이건 어떤 마을에서 이미 부활탑을 건설했기 때문이다.
	if( false == IsInputItem()		  ) 
	{
		((cltServer*)pclClient)->SendServerResponseMsg( 0, SRVAL_GIANTRACCOONRESURRECTEVENT_ALREAY_MAKE_NPC, 0, 0, _pclChar->GetID() );
		return false;
	}

	return true;
}


//==============================================================================
// - 자이언트 라쿤 부활 축제 때 리젠될 몬스터 정보.
//==============================================================================
SI32 cltGRREHuntMapMonsterInfo::m_siRegenMonsterKind	   = 0;
SI32 cltGRREHuntMapMonsterInfo::m_siMapRegenMonsterNum     = 0;
SI32 cltGRREHuntMapMonsterInfo::m_siMapOnceRegenMonsterNum = 0;

cltGRREHuntMapMonsterInfo::cltGRREHuntMapMonsterInfo( void )
{
	Clear();
}

cltGRREHuntMapMonsterInfo::~cltGRREHuntMapMonsterInfo( void )
{
	if( m_psiMonsterID ) delete[] m_psiMonsterID;
}

void	cltGRREHuntMapMonsterInfo::Init( SI32 _siMapIndex, SI32 _siMapOnceRegenMonsterNum )
{
	m_siMapIndex	= _siMapIndex;
	m_psiMonsterID	= new SI32[_siMapOnceRegenMonsterNum];
	ZeroMemory( m_psiMonsterID, sizeof(SI32)*_siMapOnceRegenMonsterNum );
}

void	cltGRREHuntMapMonsterInfo::Clear( void )
{
	ZeroMemory( this, sizeof(*this) );
}

SI32	cltGRREHuntMapMonsterInfo::GetMapIndex( void )
{
	return m_siMapIndex;
}

void	cltGRREHuntMapMonsterInfo::MonsterDel( void )
{
	cltServer* pclserver = (cltServer*)pclClient;

	cltCharServer*	pclMonsterChar = NULL;
	for( SI32 siCount = 0; m_siMapOnceRegenMonsterNum > siCount; ++siCount )
	{
		pclMonsterChar = pclserver->pclCM->GetCharServer( m_psiMonsterID[siCount] ); 
		if( pclMonsterChar && (m_siRegenMonsterKind == pclMonsterChar->GetKind()) )
		{
			cltGameMsgResponse_IncLife clInfo( pclMonsterChar->GetCharUnique(), 0, 0, 0, 0, INCLIFE_REASON_EVENT_END );
			cltMsg clMsg( GAMEMSG_RESPONSE_INCLIFE, PACKET(clInfo) );
			pclMonsterChar->SendNeighbourMsg( &clMsg, true );

			pclMonsterChar->SetLife(0);
		}
	}

	ZeroMemory( m_psiMonsterID, sizeof(m_psiMonsterID) * m_siMapOnceRegenMonsterNum );	// 해당 맵에 리젠 된 몬스터의 ID 제거.
	m_siRegenMonsterCount = 0;															// 리젠 된 몬스터의 누적 수 0.
}

void	cltGRREHuntMapMonsterInfo::MonsterRegen( void )
{
	if( m_siMapRegenMonsterNum <= m_siRegenMonsterCount ) return;
	
	cltServer* pclserver = (cltServer*)pclClient;

	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);   

	bool			bFindLocate			= false;
	SI32 			tempx 				= (rand() % 5);
	SI32 			tempy 				= (rand() % 5);
	SI32 			x 					= 0;
	SI32 			y 					= 0;
	SI32			siExitCnt			= 0;
	const SI32		Const_Limit_Count	= 30;
	
	cltCharServer*	pclMonsterChar		= NULL;

	for( SI32 siCount = 0; m_siMapOnceRegenMonsterNum > siCount; ++siCount )
	{
		pclMonsterChar = pclserver->pclCM->GetCharServer( m_psiMonsterID[siCount] ); 
		if( pclMonsterChar && (m_siRegenMonsterKind == pclMonsterChar->GetKind()) ) continue; 

		// 위치를 못찾아 낸다면 최대 30번까지 루프를 돈다
		do 
		{
			if ( siExitCnt++ > Const_Limit_Count ) break;

			tempx = 1 + rand() % pclClient->pclMapManager->pclMapArray[m_siMapIndex]->GetXsize();
			tempy = 1 + rand() % pclClient->pclMapManager->pclMapArray[m_siMapIndex]->GetYsize();

			bFindLocate	= pclClient->pclMapManager->pclMapArray[m_siMapIndex]->FindEmptyAreaForCharLocate(	m_siMapIndex, 
																											tempx, 
																											tempy, 
																											10, 
																											&x, 
																											&y, 
																											true);
		}while( false == bFindLocate );
		
				
		if ( true == bFindLocate )
		{
			m_psiMonsterID[siCount] = pclserver->SetNPCChar(m_siRegenMonsterKind, 
															VILLAGE_NONE, 
															NULL, 
															90, 
															0, 
															x, 
															y, 
															m_siMapIndex, 
															NPCITEMMODE_NORMAL, 
															&clinnerstatus);
			if( m_psiMonsterID[siCount] )
			{ 
				if( m_psiMonsterID[siCount] ) ++m_siRegenMonsterCount;

				// 생성된 몬스터가 자이언트 라쿤 부활 이벤트 때문에 생성된 몬스터임을 세팅.
				cltCharServer* pclNewMonsterChar = pclClient->pclCM->GetCharServer( m_psiMonsterID[siCount] );
				if( pclNewMonsterChar ) 
				{
                	pclNewMonsterChar->clIdentity.MakeGiantRaccoonResurrectEvent();
				
					//리젠된 몬스터에게 아이템을 등록한다.
					pclNewMonsterChar->pclCI->clCharItem.clItem[PERSONITEM_INV0]= ITEMUNIQUE(13064);
					pclNewMonsterChar->pclCI->clCharItem.clItem[PERSONITEM_INV0]= 1;
				}
			}
		}
	}
}

SI32	cltGRREHuntMapMonsterInfo::GetRegenMonsterCount( void )
{
	return m_siRegenMonsterCount;
}

void	cltGRREHuntMapMonsterInfo::SetRegenMonsterKind( SI32 _siRegenMonsterKind )
{
	m_siRegenMonsterKind = _siRegenMonsterKind;
}

void	cltGRREHuntMapMonsterInfo::SetMapRegenMonsterNum( SI32 _siMapRegenMonsterNum )
{
	m_siMapRegenMonsterNum = _siMapRegenMonsterNum;
}

void	cltGRREHuntMapMonsterInfo::SetMapOnceRegenMonsterNum( SI32 _siMapOnceRegenMonsterNum )
{
	m_siMapOnceRegenMonsterNum = _siMapOnceRegenMonsterNum;
}


//==============================================================================
// - 각 마을의 자이언트 라쿤 부활 축제 정보.
//==============================================================================
cltGRREVillageInfo::cltGRREVillageInfo( void )
{
	Clear();
}

cltGRREVillageInfo::~cltGRREVillageInfo( void )
{

}

void	cltGRREVillageInfo::Clear( void )
{
	ZeroMemory( this, sizeof(*this) );
}

void	cltGRREVillageInfo::SetFirstMakeVillage( SI32 _siFirstMakeVillage )
{
	m_siFirstMakeVillage = _siFirstMakeVillage;
}

SI32	cltGRREVillageInfo::GetFirstMakeVillage( void )
{
	return m_siFirstMakeVillage;
}

bool	cltGRREVillageInfo::IsFirstMakeVillage( SI32 _siFirstMakeVillage )
{
	return m_siFirstMakeVillage ? m_siFirstMakeVillage == _siFirstMakeVillage : false;
}

