//---------------------------------
// 2007/09/04 김광명
//---------------------------------

#include "Client.h"
#include "Music\Music.h"
#include "..\CommonLogic\MsgType.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "../Resource.h"
#include "Char\CharManager\CharManager.h"
#include "MsgType-Item.h"

#include "MsgType-System.h"
#include "Msg\MsgType-Web.h"
#include "Msg\MsgType-Village.h"
#include "Msg\MsgType-Quest.h"
#include "..\CommonLogic\MsgType-Person.h"
#include "StatusBar\StatusBar.h"


#include "Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "Interface\NotifyMsgDlg\NotifyMsgDlg.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_KINGGURIINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_KingGuriInfo* pclinfo = (cltGameMsgResponse_KingGuriInfo*)pclMsg->cData;

	if( CKingGuriEvent::START == pclinfo->siCmd )
	{
		TCHAR* text = GetTxtFromMgr( 10195 );
		pclMessage->SetMsg( text, 0, RGB(255, 255, 0)  );

		text = GetTxtFromMgr( 10192 );
		TCHAR buffer[256] = {'\0'};
		TCHAR AreaName[64] = {'\0'};

		POINT pos;
		for( SI32 i = 0; i < MAX_KINGGURI_NUM; ++i )
		{
			if(AreaName == NULL )	continue	;
			StringCchCopy( AreaName, 64, pclinfo->szAreaNmae[i] );
			if(strlen(AreaName)<=0)	continue	;
			Nsprintf( buffer, text, TEXT("areaname"), AreaName, NULL );

			//[추가 : 황진성 2007. 11. 13 대왕 구리댁 이벤트 알림 노란색 글자로 보이고 좌표.]
			if(pclClient->IsCountrySwitch( Switch_RaccoonEvent_Change_Goonzu ))
			{
				pos = pclinfo->m_ptPosition[i];
			}
			else
			{
				pos = pclinfo->m_ptPosition[pclinfo->m_siAreaIndex[i]];
			}
			StringCchPrintf( &buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer)  ,"(%d, %d)", pos.x, pos.y );

			//pclMessage->SetMsg( buffer );
			pclMessage->SetMsg( buffer, 0, RGB(255, 255, 0) );
		}
	}
	else if( CKingGuriEvent::ACTION == pclinfo->siCmd )
	{
		TCHAR* text = GetTxtFromMgr( 10192 );
		TCHAR buffer[256] = {'\0'};
		TCHAR AreaName[64] = {'\0'};

		POINT pos;
		for( SI32 i = 0; i < MAX_KINGGURI_NUM; ++i )
		{
			if(AreaName == NULL )	continue	;
			//[추가 : 황진성 2007. 11. 13 맵이름이 없으면 생략.(맵 이름은 대왕 구리댁 죽을 때 삭제됨)]
			if(0 == strlen(pclinfo->szAreaNmae[i]))				continue;

			StringCchCopy( AreaName, 64, pclinfo->szAreaNmae[i] );
			Nsprintf( buffer, text, TEXT("areaname"), AreaName, NULL );
			
			//[추가 : 황진성 2007. 11. 13 대왕 구리댁 이벤트 알림 노란색 글자로 보이고 좌표.]
			if(pclClient->IsCountrySwitch( Switch_RaccoonEvent_Change_Goonzu))
			{
				pos = pclinfo->m_ptPosition[i];
			}
			else
			{
				pos = pclinfo->m_ptPosition[pclinfo->m_siAreaIndex[i]];
			}
			StringCchPrintf( &buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer)  ,"(%d, %d)", pos.x, pos.y );
					
			//pclMessage->SetMsg( buffer );
			pclMessage->SetMsg( buffer, 0, RGB(255, 255, 0) );
		}
	}
	else if( CKingGuriEvent::DIE_KINGGURI ==pclinfo->siCmd )
	{
	//	TCHAR* text = GetTxtFromMgr( 10193 );
	//	pclMessage->SetMsg( text );

		TCHAR* text = GetTxtFromMgr( 10194 );
		TCHAR buffer[256] = {'\0'};
		TCHAR userName[MAX_PLAYER_NAME] = {'\0'};
		StringCchCopy( userName, MAX_PLAYER_NAME, pclinfo->szUserName );
		Nsprintf( buffer, text, TEXT("user"), userName, NULL );
		pclMessage->SetMsg( buffer, 0, RGB(255, 255, 0));
	}
	else if( CKingGuriEvent::END == pclinfo->siCmd )
	{
		TCHAR* text = GetTxtFromMgr( 10193 );
		pclMessage->SetMsg( text, 0, RGB(255, 255, 0));
	}
	//[추가 : 황진성 2007. 11. 14 대왕 구리댁 죽이고 얻은 아이템과 유저 공지.]
	else if( CKingGuriEvent::ITEM == pclinfo->siCmd )
	{
		pclMessage->SetMsg( pclinfo->szBuffer, 0, RGB(255, 255, 0));
	}
}

//[추가 : 황진성 2007. 11. 29 산타라쿤 이벤트 메시지 처리기 추가.]
void cltClient::DoMsg_GAMEMSG_RESPONSE_SANTARACCOONINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SantaRaccoonInfo* pclinfo = (cltGameMsgResponse_SantaRaccoonInfo*)pclMsg->cData;

	if( CSantaRaccoon::START == pclinfo->siCmd )
	{
		// 시작함을 공지하는것을 주석처리함.
		TCHAR* text = GetTxtFromMgr( 6683 );
		//pclMessage->SetMsg( text, 0, RGB(255, 255, 0)  );

		text = GetTxtFromMgr( 7680 );
		TCHAR buffer[256] = {'\0'};
		TCHAR AreaName[64] = {'\0'};

		POINT pos;
		for( SI32 i = 0; i < MAX_SANTARACCOON_NUM; ++i )
		{
			StringCchCopy( AreaName, 64, pclinfo->szAreaNmae[i] );
			Nsprintf( buffer, text, TEXT("areaname"), AreaName, NULL );

			//[추가 : 황진성 2007. 11. 29 산타라쿤 이벤트 알림 노란색 글자로 보이고 좌표.]
			pos = pclinfo->m_ptPosition[pclinfo->m_siAreaIndex[i]];
			StringCchPrintf( &buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer)  ,"(%d, %d)", pos.x, pos.y );

			//pclMessage->SetMsg( buffer );
			pclMessage->SetMsg( buffer, 0, RGB(255, 255, 0) );
		}
	}
	else if( CSantaRaccoon::ACTION == pclinfo->siCmd )
	{
		TCHAR* text = GetTxtFromMgr( 7680 );
		TCHAR buffer[256] = {'\0'};
		TCHAR AreaName[64] = {'\0'};

		POINT pos;
		for( SI32 i = 0; i < MAX_SANTARACCOON_NUM; ++i )
		{	
			//[추가 : 황진성 2007. 11. 29 맵이름이 없으면 생략.(맵 이름은 대왕 구리댁 죽을 때 삭제됨)]
			if(0 == strlen(pclinfo->szAreaNmae[i]))
				continue;

			StringCchCopy( AreaName, 64, pclinfo->szAreaNmae[i] );
			Nsprintf( buffer, text, TEXT("areaname"), AreaName, NULL );

			//[추가 : 황진성 2007. 11. 29 산타 이벤트 알림 노란색 글자로 보이고 좌표.]
			pos = pclinfo->m_ptPosition[pclinfo->m_siAreaIndex[i]];
			StringCchPrintf( &buffer[strlen(buffer)], sizeof(buffer) - strlen(buffer)  ,"(%d, %d)", pos.x, pos.y );

			//pclMessage->SetMsg( buffer );
			pclMessage->SetMsg( buffer, 0, RGB(255, 255, 0) );
		}
	}
	else if( CSantaRaccoon::DIE == pclinfo->siCmd )
	{
		//	TCHAR* text = GetTxtFromMgr( 10193 );
		//	pclMessage->SetMsg( text );

		TCHAR* text = GetTxtFromMgr( 10194 );
		TCHAR buffer[256] = {'\0'};
		TCHAR userName[MAX_PLAYER_NAME] = {'\0'};
		StringCchCopy( userName, MAX_PLAYER_NAME, pclinfo->szUserName );
		Nsprintf( buffer, text, TEXT("user"), userName, NULL );
		//pclMessage->SetMsg( buffer, 0, RGB(255, 255, 0));
	}
	else if( CSantaRaccoon::END == pclinfo->siCmd )
	{
		TCHAR* text = GetTxtFromMgr( 7682 );
		pclMessage->SetMsg( text, 0, RGB(255, 255, 0));
	}
	//[추가 : 황진성 2007. 11. 29 산타라쿤을 죽이고 얻은 아이템과 유저 공지.]
	else if( CKingGuriEvent::ITEM == pclinfo->siCmd )
	{
		pclMessage->SetMsg( pclinfo->szBuffer, 0, RGB(255, 255, 0));
	}
}
// [영훈] 2008.01.13 - 설날이벤트 : 카이트 라쿤 메시지 처리
void cltClient::DoMsg_GAMEMSG_RESPONSE_KITERACCOONINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_KiteRaccoonInfo* pclinfo = (cltGameMsgResponse_KiteRaccoonInfo*)pclMsg->cData;
	
	switch ( pclinfo->m_siType ) 
	{
	// 카이트 라쿤 이벤트가 시잘됨을 알림
	case ( cltEventTimeManager::KITERACCOON_EVENT_START ):
		{
			TCHAR	tszMonster[MAX_PLAYER_NAME]	= {'\0', };
			TCHAR	tszBuffer[256]				= {'\0', };
			TCHAR	*ptsText					= GetTxtFromMgr( 7837 );
			
				
			StringCchCopy( tszMonster, MAX_PLAYER_NAME, pclKindInfoSet->GetName( pclinfo->m_siMonsterKind ) ) ;
			Nsprintf( tszBuffer, ptsText, TEXT("monster"), tszMonster, NULL );

			pclMessage->SetMsg( tszBuffer, 0, RGB(255, 255, 0) );
		}
		break;
	// 누군가 카이트 라쿤을 죽였다고 알림
	case( cltEventTimeManager::KITERACCOON_EVENT_KILL ):
		{
			TCHAR	tszMonster[MAX_PLAYER_NAME] = {'\0', };
			TCHAR	tszItem[MAX_ITEM_NAME]		= {'\0', };
			TCHAR	tszUser[MAX_PLAYER_NAME]	= {'\0', };
			TCHAR	tszBuffer[256]				= {'\0', };
			TCHAR	*ptsText					= GetTxtFromMgr( 7838 );


			StringCchCopy( tszMonster, MAX_PLAYER_NAME, pclKindInfoSet->GetName( pclinfo->m_siMonsterKind ) ) ;
			StringCchCopy( tszItem, MAX_ITEM_NAME, pclItemManager->GetName( pclinfo->m_siItemUnique ) );
			StringCchCopy( tszUser, MAX_PLAYER_NAME, pclinfo->m_tszUserName );

			Nsprintf( tszBuffer, ptsText, TEXT("user"), tszUser, TEXT("monster"), tszMonster, TEXT("item"), tszItem, NULL );

			pclMessage->SetMsg( tszBuffer, 0, RGB(255, 255, 0) );
		}
		break;

	// 카이트 라쿤 이벤트가 종료되었음을 알림
	case( cltEventTimeManager::KITERACCOON_EVENT_END ):
		{
			TCHAR	tszBuffer[256]				= {'\0', };
			TCHAR	*ptsText					= GetTxtFromMgr( 7839 );

			StringCchCopy( tszBuffer, 256, ptsText );

			pclMessage->SetMsg( tszBuffer, 0, RGB(255, 255, 0) );
		}
		break;
	}
}

