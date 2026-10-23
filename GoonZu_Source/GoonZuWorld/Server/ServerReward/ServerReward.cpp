#include "ServerReward.h"
#include "../Server.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgRval-Define.h"
#include "../../common/Char/CharManager/CharManager.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"

#include "../../Lib/dxutil.h"



extern cltCommonLogic* pclClient;

//----------------------------------------------------------------------------------
// 캐릭터 생성시 보상 클레스
//----------------------------------------------------------------------------------
cltCreateCharReward::cltCreateCharReward()
{
	Init();
}

cltCreateCharReward::~cltCreateCharReward()
{
	for ( SI32 siIndex=0; siIndex<Const_Max_CreateCharReward_Item; ++siIndex )
	{
		SAFE_DELETE( m_pclCreateCharReward[siIndex] );
	}
}

void cltCreateCharReward::Init( void )
{
	ZeroMemory( this, sizeof(cltCreateCharReward) );
}

bool cltCreateCharReward::LoadRewardItemList( void )
{
	TCHAR		szFileName[MAX_PATH] = { '\0', };
	if( pclClient->siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\CreateCharRewardK.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\CreateCharRewardJ.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\CreateCharRewardC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )  // 영어
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\CreateCharRewardE.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\CreateCharRewardT.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\CreateCharRewardU.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )  //NHN중국
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\CreateCharRewardC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\CreateCharRewardG.txt"));
	}

	NDataLoader	dataloader;
	BOOL		bLoadFile				= FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( szFileName );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( szFileName );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("cltCreateCharReward::LoadRewardItemList()"), TEXT("LoadError:[%s]"), szFileName);
		return false;
	}


	SI32 siItemUnique	= 0;
	SI32 siItemNum		= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siItemUnique,	4,		
		NDATA_INT32,	&siItemNum,		4,		
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			cltRewardItem clRewardItem( siItemUnique, siItemNum );

			if ( AddRewardItem( &clRewardItem ) )
			{
				m_siCreateCharRewardCount++;
			}

			if ( Const_Max_CreateCharReward_Item <= m_siCreateCharRewardCount )
			{
				break;
			}
		}
	}

	return true;
}

void cltCreateCharReward::GiveRewardItem( SI32 siPersonID )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	if ( 0 >= siPersonID )
	{
		return;
	}

	bool bRewardItem = false;

	for ( SI32 siIndex=0; siIndex<Const_Max_CreateCharReward_Item; ++siIndex )
	{
		if ( NULL == m_pclCreateCharReward[siIndex] )
		{
			continue;
		}

		SI32 siItemUnique	= m_pclCreateCharReward[siIndex]->GetItemUnique();
		SI32 siItemNum		= m_pclCreateCharReward[siIndex]->GetItemNum();
		
		if ( (0 >= siItemUnique) || (0 >= siItemNum) )
		{
			continue;
		}

		cltItem clGiveItem;
		if ( false == GetRewardItem(siItemUnique, siItemNum, &clGiveItem) )
		{
			continue;
		}
		
		cltSystemRewardInfo clRewardInfo;

		// 지급일
		clRewardInfo.m_sGiveTime = pclserver->sTime;

		// 보관기간
		TimeAddDate( &clRewardInfo.m_sGiveTime, 30, &clRewardInfo.m_sExpireTime );

		// 아이템 설정
		clRewardInfo.m_clItem.Set( &clGiveItem );

		// 지급사유
		TCHAR* pszReason = GetTxtFromMgr( 8931 );
		StringCchCopy( clRewardInfo.m_szReason, 256, pszReason );

		// 실제 보내기
		sDBRequest_GiveSystemReward clMsg( GIVEREWARD_CHAR, siPersonID, 0, &clRewardInfo );
		pclserver->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );

		bRewardItem = true;
	}

	// 유저에게 아이템을 보냈다는 메시지를 보냄
	if ( true == bRewardItem )
	{
		cltLetterMsg_CreateCharReward clletter;
		sDBRequest_PushLetter clMsg( siPersonID, (cltLetterUnit*)&clletter );
		pclserver->SendDBMsg( DBSELECT_LETTER, (sPacketHeader*)&clMsg );
	}

}

bool cltCreateCharReward::GetRewardItem( IN SI32 siItemUnique, IN SI32 siItemNum, OUT cltItem* pclGiveItem )
{
	if ( (0 >= siItemUnique) || (0 >= siItemNum) || (NULL == pclGiveItem) )
	{
		return false;
	}

	SI32 siRef = pclClient->pclItemManager->FindItemRefFromUnique( siItemUnique );
	if ( 0 >= siRef )
	{
		return false;
	}

	cltItem	clGiveItem;
	SI16	siUseDate		= pclClient->pclItemManager->pclItemInfo[siRef]->clItem.uiDateUseDay;
	SI16	siRareSwitch	= 0;

	bool	bCanMakeItem	= pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clGiveItem, 0, 0, &siRareSwitch, 0, 0, 0, 0, 0, siUseDate );
	if ( false == bCanMakeItem )
	{
		return false;
	}

	// 개수를 조정한다. 
	// 중첩 가능한 아이템은 원하는 개수만큼 만들 수 있다. 
	if ( true == clGiveItem.CanPile(pclClient->pclItemManager) )
	{
		clGiveItem.SetItemNum(siItemNum);
	}
	else
	{
		// 중첩 불가 아이템은 1개만 만들 수 있다. 
		clGiveItem.SetItemNum(1);
	}

	// 기간이 있는 아이템은 밀봉해서 넣어준다
	if ( clGiveItem.uiDateUseDay )
	{
		clGiveItem.uiDateYear	= 0;
		clGiveItem.uiDateMonth	= 0;
		clGiveItem.uiDateDay	= 0;
	}

	pclGiveItem->Init();
	pclGiveItem->Set( &clGiveItem );

	if ( 0 >= pclGiveItem->siUnique )
	{
		return false;
	}

	return true;

}

bool cltCreateCharReward::AddRewardItem( cltRewardItem* pclRewardItem )
{
	if ( NULL == pclRewardItem )
	{
		return false;
	}

	if ( Const_Max_CreateCharReward_Item <= m_siCreateCharRewardCount )
	{
		return false;
	}

	m_pclCreateCharReward[m_siCreateCharRewardCount] = new cltRewardItem;
	if ( NULL == m_pclCreateCharReward )
	{
		return false;
	}

	m_pclCreateCharReward[m_siCreateCharRewardCount]->Set( pclRewardItem );

	return true;

}

//----------------------------------------------------------------------------------
// 캐릭터 추천인 보상 클레스
//----------------------------------------------------------------------------------
cltRecommendReward::cltRecommendReward()
{
	Init();
}

cltRecommendReward::~cltRecommendReward()
{
	for ( SI32 siIndex=0; siIndex<Const_Max_RecommandReward_Item; ++siIndex )
	{
		SAFE_DELETE( m_pclRecommendReward[siIndex] );
		SAFE_DELETE( m_pclRecommendeeReward[siIndex] );
	}
}

void cltRecommendReward::Init( void )
{
	ZeroMemory( this, sizeof(cltRecommendReward) );
}

bool cltRecommendReward::LoadRewardItemList( void )
{
	if( false == LoadRecommendRewardItemList() )	return false;
	if( false == LoadRecommendeeRewardItemList() )	return false;

	return true;
}

bool cltRecommendReward::LoadRecommendRewardItemList( void )
{
	TCHAR		szFileName[MAX_PATH] = { '\0', };
	if( pclClient->siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendRewardK.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendRewardJ.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendRewardC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )  // 영어
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendRewardE.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendRewardT.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendRewardU.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )  //NHN중국
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendRewardC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendRewardG.txt"));
	}

	NDataLoader	dataloader;
	BOOL		bLoadFile				= FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( szFileName );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( szFileName );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("cltRecommendReward::LoadRewardItemList()"), TEXT("LoadError:[%s]"), szFileName);
		return false;
	}


	SI32 siLevel		= 0;
	SI32 siItemUnique	= 0;
	SI32 siItemNum		= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siLevel,		4,
		NDATA_INT32,	&siItemUnique,	4,
		NDATA_INT32,	&siItemNum,		4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			cltRewardItem clRewardItem( siItemUnique, siItemNum, siLevel );

			AddRecommendRewardItem( &clRewardItem );
		}
	}

	/***************************************************************************************
	*
	*	[진성] 중국에서 핸드폰 인증이 안됐을 때 줄 아이템 정보 로드
	*
	***************************************************************************************/
	if( pclClient->siServiceArea != ConstServiceArea_NHNChina )  return true;
	
	StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendRewardNoAuthC.txt"));
	
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( szFileName );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( szFileName );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("cltRecommendReward::LoadRewardItemList()"), TEXT("LoadError:[%s]"), szFileName);
		return false;
	}


	NDataTypeInfo pDataTypeInfo2[] =
	{
		NDATA_INT32,	&siLevel,		4,
		NDATA_INT32,	&siItemUnique,	4,
		NDATA_INT32,	&siItemNum,		4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo2 );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			cltRewardItem clRewardItem( siItemUnique, siItemNum, siLevel );

			AddRecommendRewardItemNoAuth( &clRewardItem );
		}
	}

	return true;
}

bool cltRecommendReward::LoadRecommendeeRewardItemList( void )
{
	TCHAR		szFileName[MAX_PATH] = { '\0', };
	if( pclClient->siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendeeRewardK.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendeeRewardJ.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendeeRewardC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )  // 영어
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendeeRewardE.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendeeRewardT.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendeeRewardU.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )  //NHN중국
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendeeRewardC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendeeRewardG.txt"));
	}

	NDataLoader	dataloader;
	BOOL		bLoadFile				= FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( szFileName );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( szFileName );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("cltRecommendeeReward::LoadRecommendeeRewardItemList()"), TEXT("LoadError:[%s]"), szFileName);
		return false;
	}


	SI32 siLevel		= 0;
	SI32 siItemUnique	= 0;
	SI32 siItemNum		= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&siLevel,		4,
			NDATA_INT32,	&siItemUnique,	4,
			NDATA_INT32,	&siItemNum,		4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			cltRewardItem clRewardItem( siItemUnique, siItemNum, siLevel );

			AddRecommendeeRewardItem( &clRewardItem );
		}
	}



	/***************************************************************************************
	*
	*	[진성] 중국에서 핸드폰 인증이 안됐을 때 줄 아이템 정보 로드
	*
	***************************************************************************************/
	if( pclClient->siServiceArea != ConstServiceArea_NHNChina )  return true;

	StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\RecommendeeRewardNoAuthC.txt"));

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( szFileName );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( szFileName );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("cltRecommendReward::LoadRewardItemList()"), TEXT("LoadError:[%s]"), szFileName);
		return false;
	}

	NDataTypeInfo pDataTypeInfo2[] =
	{
		NDATA_INT32,	&siLevel,		4,
		NDATA_INT32,	&siItemUnique,	4,
		NDATA_INT32,	&siItemNum,		4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo2 );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			cltRewardItem clRewardItem( siItemUnique, siItemNum, siLevel );

			AddRecommendeeRewardItemNoAuth( &clRewardItem );
		}
	}

	return true;
}

bool cltRecommendReward::GiveRewardItem( SI32 siPersonID, SI32 siLevel, bool bGiveMode, TCHAR* pszRecommenderName, bool _bAuth  )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return false;
	}

	if ( (0 >= siPersonID) || (0 >= siLevel) || (Const_Max_RecommandReward_Item <= siLevel) )
	{
		return false;
	}
	
	cltRewardItem* pRewardItem = NULL;
	if( bGiveMode )	pRewardItem	= _bAuth ? m_pclRecommendReward[siLevel]   : m_pclRecommendRewardNoAuth[siLevel];		// 추천한 사람.
	else			pRewardItem	= _bAuth ? m_pclRecommendeeReward[siLevel] : m_pclRecommendeeRewardNoAuth[siLevel];	// 추천을 받은 사람. 		


	if ( NULL == pRewardItem )
	{
		return false;
	}

	SI32 siItemUnique = pRewardItem->GetItemUnique();
	if ( 0 >= siItemUnique )
	{
		return false;
	}

	SI32 siItemNum = pRewardItem->GetItemNum();
	if ( 0 >= siItemNum )
	{
		return false;
	}

	SI32 siRef = pclClient->pclItemManager->FindItemRefFromUnique( siItemUnique );
	if ( 0 >= siRef )
	{
		return false;
	}

	SI32 siDateUseday = pclClient->pclItemManager->pclItemInfo[siRef]->clItem.uiDateUseDay;

	// 이곳에서 유효값 검사는 하지 않는다 왜냐하면 이 값이 없으면 우편으로 보내주기 때문이다
	cltCharServer*	pclchar		= NULL;
	SI32			siCharID	= pclserver->pclCM->GetIDFromPersonID( siPersonID );
	if ( pclClient->pclCM->IsValidID(siCharID) )
	{
		pclchar = pclClient->pclCM->GetCharServer( siCharID );
	}

	bool bGiveItem = false;
	
	// 아이템을 케릭터 인벤토리에 넣는다
	if ( (pclchar) && (pclchar->GiveItem(siItemUnique, siItemNum, INPUTITEMPERSON_OPTION_RECOMMEND, siDateUseday) == GIVEITEM_RESULT_SUCCESS) )
	{
		bGiveItem = true;
	}

	// 아이템을 인벤토리에 넣지 못했다면
	if ( false == bGiveItem )
	{
		cltItem	clGiveItem;
		SI16	siUseDate		= pclClient->pclItemManager->pclItemInfo[siRef]->clItem.uiDateUseDay;
		SI16	siRareSwitch	= 0;
		
		if ( pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clGiveItem, 0, 0, &siRareSwitch, 0, 0, 0, 0, 0, siUseDate ) )
		{
			clGiveItem.SetItemNum( siItemNum );

			pclserver->SendPostOfficeItemToPerson( siPersonID, &clGiveItem, NULL );

			bGiveItem = true;
		}
	}

	// 아이템을 받았다면 메시지를 보내주자
	if ( true == bGiveItem )
	{
		// 추천자
		if ( true == bGiveMode )
		{
			SendRecommenderMessage( pRewardItem, siPersonID, siLevel, _bAuth );

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				cltServer* pclserver = (cltServer*)pclClient;
				// [로그추가 : 황진성 2007. 10. 19] // 피추천인 보상.
				// param1 == 아이템 유니크.
				// param2 == 아이템 개수.
				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RECOMMENDER, LOGCOMMAND_INDEX_COMMENDER_REWARD, 
												0, NULL, NULL, siPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
												siItemUnique, siItemNum, 0, 0, 0, NULL, NULL);
			}
		}
		// 추천을 받은 사람
		else
		{
			SendRecommendMessage( pRewardItem, siPersonID, siLevel, pszRecommenderName, _bAuth );

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				cltServer* pclserver = (cltServer*)pclClient;
				// [로그추가 : 황진성 2007. 10. 19] // 추천인 보상.
				// param1 == 아이템 유니크.
				// param2 == 아이템 개수.
				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RECOMMENDER, LOGCOMMAND_INDEX_RECOMMENDER_REWARD, 
												0, NULL, NULL, siPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
												siItemUnique, siItemNum, 0, 0, 0, NULL, NULL);
			}
		}
	}

	return bGiveItem;

}

bool cltRecommendReward::IsGiveRewardItem( SI32 siPersonID, SI32 siLevel, bool bGiveMode, TCHAR* pszRecommenderName )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return false;
	}

	if ( (0 >= siPersonID) || (0 >= siLevel) || (Const_Max_RecommandReward_Item <= siLevel) )
	{
		return false;
	}

	cltRewardItem* pRewardItem = NULL;
	if( bGiveMode )	pRewardItem	= m_pclRecommendReward[siLevel];	// 추천한 사람.
	else			pRewardItem	= m_pclRecommendeeReward[siLevel];	// 추천을 받은 사람. 		


	if ( NULL == pRewardItem )
	{
		return false;
	}

	SI32 siItemUnique = pRewardItem->GetItemUnique();
	if ( 0 >= siItemUnique )
	{
		return false;
	}

	SI32 siItemNum = pRewardItem->GetItemNum();
	if ( 0 >= siItemNum )
	{
		return false;
	}

	SI32 siRef = pclClient->pclItemManager->FindItemRefFromUnique( siItemUnique );
	if ( 0 >= siRef )
	{
		return false;
	}

	return true;
}

bool cltRecommendReward::AddRecommendRewardItemNoAuth( cltRewardItem* pclRewardItem )
{
	if ( NULL == pclRewardItem )
	{
		return false;
	}

	// 레벨이 실제 배열인덱스의 값과 같다
	SI32 siLevel = pclRewardItem->GetPara1();
	if ( (0 > siLevel) || (Const_Max_RecommandReward_Item <= siLevel) )
	{
		return false;
	}

	//m_siRecommendGiveItemMaxLevel = siLevel;

	SI32 siItemUnique	= pclRewardItem->GetItemUnique();
	SI32 siItemNum		= pclRewardItem->GetItemNum();

	if ( m_pclRecommendRewardNoAuth[siLevel] )
	{
#ifdef _DEBUG
		NTCHARString256 strMessage( TEXT("해당 #level#의 아이템이 이미 설정되어 있습니다") );
		strMessage.Replace( TEXT("#level#"), SI32ToString(siLevel) );

		MsgBox( TEXT("추천인 보상 아이템 설정"), strMessage );
#endif

		return false;
	}

	m_pclRecommendRewardNoAuth[siLevel] = new cltRewardItem;
	if ( NULL == m_pclRecommendRewardNoAuth[siLevel] )
	{
		return false;
	}

	m_pclRecommendRewardNoAuth[siLevel]->Set( pclRewardItem );

	return true;
}

bool cltRecommendReward::AddRecommendRewardItem( cltRewardItem* pclRewardItem )
{
	if ( NULL == pclRewardItem )
	{
		return false;
	}

	// 레벨이 실제 배열인덱스의 값과 같다
	SI32 siLevel = pclRewardItem->GetPara1();
	if ( (0 > siLevel) || (Const_Max_RecommandReward_Item <= siLevel) )
	{
		return false;
	}
	
	m_siRecommendGiveItemMaxLevel = siLevel;

	SI32 siItemUnique	= pclRewardItem->GetItemUnique();
	SI32 siItemNum		= pclRewardItem->GetItemNum();

	if ( m_pclRecommendReward[siLevel] )
	{
		#ifdef _DEBUG
			NTCHARString256 strMessage( TEXT("해당 #level#의 아이템이 이미 설정되어 있습니다") );
			strMessage.Replace( TEXT("#level#"), SI32ToString(siLevel) );

			MsgBox( TEXT("추천인 보상 아이템 설정"), strMessage );
		#endif

		return false;
	}

	m_pclRecommendReward[siLevel] = new cltRewardItem;
	if ( NULL == m_pclRecommendReward[siLevel] )
	{
		return false;
	}

	m_pclRecommendReward[siLevel]->Set( pclRewardItem );

	return true;

}

bool cltRecommendReward::AddRecommendeeRewardItemNoAuth( cltRewardItem* pclRewardItem )
{
	if ( NULL == pclRewardItem )
	{
		return false;
	}

	// 레벨이 실제 배열인덱스의 값과 같다
	SI32 siLevel = pclRewardItem->GetPara1();
	if ( (0 > siLevel) || (Const_Max_RecommandReward_Item <= siLevel) )
	{
		return false;
	}

	//m_siRecommendeeGiveItemMaxLevel = siLevel;

	SI32 siItemUnique	= pclRewardItem->GetItemUnique();
	SI32 siItemNum		= pclRewardItem->GetItemNum();

	if ( m_pclRecommendeeRewardNoAuth[siLevel] )
	{
#ifdef _DEBUG
		NTCHARString256 strMessage( TEXT("해당 #level#의 아이템이 이미 설정되어 있습니다") );
		strMessage.Replace( TEXT("#level#"), SI32ToString(siLevel) );

		MsgBox( TEXT("추천한 사람 보상 아이템 설정"), strMessage );
#endif

		return false;
	}

	m_pclRecommendeeRewardNoAuth[siLevel] = new cltRewardItem;
	if ( NULL == m_pclRecommendeeRewardNoAuth[siLevel] )
	{
		return false;
	}

	m_pclRecommendeeRewardNoAuth[siLevel]->Set( pclRewardItem );

	return true;

}

bool cltRecommendReward::AddRecommendeeRewardItem( cltRewardItem* pclRewardItem )
{
	if ( NULL == pclRewardItem )
	{
		return false;
	}

	// 레벨이 실제 배열인덱스의 값과 같다
	SI32 siLevel = pclRewardItem->GetPara1();
	if ( (0 > siLevel) || (Const_Max_RecommandReward_Item <= siLevel) )
	{
		return false;
	}
	
	m_siRecommendeeGiveItemMaxLevel = siLevel;

	SI32 siItemUnique	= pclRewardItem->GetItemUnique();
	SI32 siItemNum		= pclRewardItem->GetItemNum();

	if ( m_pclRecommendeeReward[siLevel] )
	{
#ifdef _DEBUG
		NTCHARString256 strMessage( TEXT("해당 #level#의 아이템이 이미 설정되어 있습니다") );
		strMessage.Replace( TEXT("#level#"), SI32ToString(siLevel) );

		MsgBox( TEXT("추천한 사람 보상 아이템 설정"), strMessage );
#endif

		return false;
	}

	m_pclRecommendeeReward[siLevel] = new cltRewardItem;
	if ( NULL == m_pclRecommendeeReward[siLevel] )
	{
		return false;
	}

	m_pclRecommendeeReward[siLevel]->Set( pclRewardItem );

	return true;

}

void cltRecommendReward::SendRecommenderMessage( cltRewardItem* pclRewardItem, SI32 siPersonID, SI32 siLevel, bool _bAuth )
{
	if ( (NULL == pclRewardItem) || (0 >= siPersonID) || (0 >= siLevel) )
	{
		return;
	}

	cltServer*		pclserver	= (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	cltCharServer*	pclchar		= NULL;
	SI32			siCharID	= pclClient->pclCM->GetIDFromPersonID( siPersonID );
	if ( pclClient->pclCM->IsValidID(siCharID) )
	{
		pclchar = pclClient->pclCM->GetCharServer(siCharID);
	}

	SI32 siItemUnique	= pclRewardItem->GetItemUnique();
	SI32 siItemNum		= pclRewardItem->GetItemNum();

	// 접속 중일 때
	if ( pclchar )
	{
#ifdef USE_PROMOTOR_SERVER 
		//pclserver->SendServerResponseMsg(0, SRVAL_PROMOTER_NOT_CELLULAR_PHONE_AUTH, siItemUnique, 1, pclchar->GetCharUnique()); 
		TCHAR*	pszItemName	= (TCHAR*)pclClient->pclItemManager->GetName( siItemUnique );
		NTCHARString64 strItemName;
		
		SI16 siSrval = 0;
		if( _bAuth )	
		{
			siSrval = SRVAL_RECOMMEND_GIVE_ITEM_SUCCEED;
			strItemName = "Y";
		}
		else			
		{
			siSrval = SRVAL_RECOMMEND_GIVE_ITEM_SUCCEED_NO_AUTH;
			strItemName = "N";
		}

		strItemName += pszItemName;

		pclserver->SendServerResponseCharMsg2( 0, siSrval, NULL, strItemName, SI32ToString(siItemNum), pclchar->GetCharUnique() );
#else
		pclserver->SendServerResponseMsg( 0, SRVAL_GIVEITEM_RECOMMANDER, siLevel, siItemUnique, pclchar->GetCharUnique() );
#endif
		
	}
	// 접속 중이 아닐때
	else
	{
		cltLetterMsg_LevelUpREcommandReward clletter( siLevel, siItemUnique, siItemNum );
		pclserver->SendLetterMsg( siPersonID, (cltLetterHeader*)&clletter );			
	}

	if ( pclClient->IsWhereServiceArea(ConstSwitch_NewLog) ) 
	{
		// [로그추가 : 황진성 2007. 10. 19] // 피추천인 보상.
		// param1 == 아이템 유니크.
		// param2 == 아이템 개수.
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RECOMMENDER, LOGCOMMAND_INDEX_COMMENDER_REWARD, 
			0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			siItemUnique, siItemNum, 0, 0, 0, NULL, NULL);
	}
}

void cltRecommendReward::SendRecommendMessage( cltRewardItem* pclRewardItem, SI32 siPersonID, SI32 siLevel, TCHAR* pszRecommenderName, bool _bAuth  )
{
	if ( (NULL == pclRewardItem) || (0 >= siPersonID) || (0 >= siLevel) )
	{
		return;
	}

	cltServer*		pclserver	= (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	cltCharServer*	pclchar		= NULL;
	SI32			siCharID	= pclClient->pclCM->GetIDFromPersonID( siPersonID );
	if ( pclClient->pclCM->IsValidID(siCharID) )
	{
		pclchar = pclClient->pclCM->GetCharServer(siCharID);
	}

	SI32 siItemUnique	= pclRewardItem->GetItemUnique();
	SI32 siItemNum		= pclRewardItem->GetItemNum();

	TCHAR*	pszItemName	= (TCHAR*)pclClient->pclItemManager->GetName( siItemUnique );

	NTCHARString64 strRecommenderName( pszRecommenderName );

	// 접속 중일 때
	if ( pclchar )
	{
		NTCHARString64 strItemName;
#ifdef USE_PROMOTOR_SERVER 
		//pclserver->SendServerResponseMsg(0, SRVAL_PROMOTER_NOT_CELLULAR_PHONE_AUTH, 1, 1, pclchar->GetCharUnique()); 
	
		SI16 siSrval = 0;
		if( _bAuth )	
		{
			strItemName = "Y";
			siSrval = SRVAL_RECOMMENDEE_GIVE_ITEM_SUCCEED;
		}
		else			
		{
			strItemName = "N";
			siSrval = SRVAL_RECOMMENDEE_GIVE_ITEM_SUCCEED_NO_AUTH;
		}

		strItemName += pszItemName;

		pclserver->SendServerResponseCharMsg2( 0, siSrval, pszRecommenderName, strItemName, SI32ToString(siItemNum), pclchar->GetCharUnique() );		
#else
		pclserver->SendServerResponseCharMsg2( 0, SRVAL_GIVEITEM_RECOMMAND, pszRecommenderName, SI32ToString(siLevel), strItemName, pclchar->GetCharUnique() );		
#endif
	}
	// 접속 중이 아닐때
	else
	{
		cltLetterMsg_LevelUpREcommandRewardForRecommender clletter( strRecommenderName, siLevel, siItemUnique, siItemNum );
		pclserver->SendLetterMsg( siPersonID, (cltLetterHeader*)&clletter );
	}

}

//----------------------------------------------------------------------------------
// 각종 보상관련 메니져
//----------------------------------------------------------------------------------
cltRewardMgr::cltRewardMgr()
{
	Init();
}

cltRewardMgr::~cltRewardMgr()
{

}

void cltRewardMgr::Init()
{
	ZeroMemory( this, sizeof(cltRewardMgr) );
}

void cltRewardMgr::LoadRewardList()
{
	m_clCreateCharReward.LoadRewardItemList();
	m_clRecommendReward.LoadRewardItemList();
}

void cltRewardMgr::GiveCreateCharRewardItem( SI32 siPersonID )
{
	m_clCreateCharReward.GiveRewardItem( siPersonID );
}

bool cltRewardMgr::GiveRecommendRewardItem( SI32 siPersonID, SI32 siLevel, bool bGiveMode, TCHAR* pszRecommenderName, bool _bAuth  )
{
	// 중국이 아닌 국가들은 모두 핸드폰 인증을 받은것으로 하자.
	if(false == pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) ) _bAuth = true;

	return m_clRecommendReward.GiveRewardItem( siPersonID, siLevel, bGiveMode, pszRecommenderName, _bAuth );
}

bool cltRewardMgr::IsGiveRecommendRewardItem( SI32 siPersonID, SI32 siLevel, bool bGiveMode, TCHAR* pszRecommenderName )
{
	return m_clRecommendReward.IsGiveRewardItem( siPersonID, siLevel, bGiveMode, pszRecommenderName );
}

cltRewardItem**	cltRewardMgr::GetRecommendRewardItemList( void )
{
	return m_clRecommendReward.GetRecommendRewardList();
}

cltRewardItem** cltRewardMgr::GetRecommendeeRewardItemList( void )
{
	return m_clRecommendReward.GetRecommendeeRewardList();
}


