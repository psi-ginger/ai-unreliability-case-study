//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :		cltNCharStatus

	작성일 :		05/03/17
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#include "..\Client\Client.h"
#include "NCharStatus.h"
//#include "..\Father\Father.h"

#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "HintDlg/HintDlg.h"
#include "Char\CharManager\CharManager.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "..\CommonLogic\msgType-Person.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "./NCharPerson.h"
#include "./NCharHorse.h"
#include "./NCharSummon.h"
#include "./NCharAsset.h"
#include "./NCharQuest.h"
#include "./NCharSoulGuard.h"

#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Msg/MsgType-ItemMall.h"
#include "Msg/MsgType-Quest.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../Client/ninterface/NCommonMsgDlg/NCommonMsgDlg.h"
extern cltCommonLogic* pclClient;



cltNCharStatus::cltNCharStatus()
{
	m_timerCount			= 0;	// Timer Counting!

	// 현재 선택한 텝
	m_siCurrentStatusPage	= 0;
	
	// 텝 다이얼로그
	m_pTabDlg0				= NULL;
	m_pTabDlg1				= NULL;
	m_pTabDlg2				= NULL;
	m_pTabDlg3				= NULL;
	m_pTabDlg4				= NULL;

	// 버튼들
	m_pButton_person		= NULL; 
	m_pButton_horse			= NULL; 
	m_pButton_summon		= NULL; 
	m_pButton_stock			= NULL;
	m_pButton_soulguard		= NULL;
	m_pButton_ok			= NULL; 
	m_pButton_warp			= NULL;

	//
	DlgRectInitSwitch		= FALSE;

	//
	pclCM					= NULL; // => Create 파라미터로 옮김

	//
	CharUnique						= 0;
	BonusSummonButtonEnableSwitch	= FALSE;
	BonusHorseButtonEnableSwitch	= FALSE;

	// 소환수 창 관련
	m_LastTakeInOutTime				= 0;
	m_bChangedInitSummonStatus		= TRUE;
	m_siLastSummonComboList			= 0;
	m_siPortraitSummonKind			= 0;	

	// 각종이름들	
	memset( strHorseName, 0, MAX_HORSE_NAME );
	memset( strSummonName, 0, MAX_SUMMON_NAME );

}

cltNCharStatus::~cltNCharStatus()
{
	// 텝 다이얼로그
	SAFE_DELETE( m_pTabDlg0 );
	SAFE_DELETE( m_pTabDlg1 );
	SAFE_DELETE( m_pTabDlg2 );
	SAFE_DELETE( m_pTabDlg3 );
	SAFE_DELETE( m_pTabDlg4 );

	// 버튼
	SAFE_DELETE( m_pButton_person );
	SAFE_DELETE( m_pButton_horse );
	SAFE_DELETE( m_pButton_summon );
	SAFE_DELETE( m_pButton_stock );
	SAFE_DELETE( m_pButton_ok );
	SAFE_DELETE( m_pButton_soulguard );
	SAFE_DELETE(m_pButton_warp);

	SetTimerOnOff( false );

	cltClient *pclclient = (cltClient*)pclClient;

	if( pclClient->IsCountrySwitch( Switch_Elixir ) )
	{
		//[진성] 엘릭서 스테이터스 설정 창 제거.
		if( pclclient->m_pDialog[NELIXIR_STATUS_DLG] ) 
		{
			pclclient->DestroyInterface( pclclient->m_pDialog[NELIXIR_STATUS_DLG] );
		}
	}

	// 수호정령 관련 창을 닫는다
	if ( pclClient->IsCountrySwitch(Switch_SoulGuard) )
	{
		// 상세 정보창
		if ( pclclient->m_pDialog[CLTNSOULGUARDHOUSESTATUS_DLG] )
		{
			pclclient->DestroyInterface( pclclient->m_pDialog[CLTNSOULGUARDHOUSESTATUS_DLG] );
		}
		
		// 강화 정보창
		if ( pclclient->m_pDialog[NCHARSOULGUARDINFO_DLG] )
		{
			pclclient->DestroyInterface( pclclient->m_pDialog[NCHARSOULGUARDINFO_DLG] );
		}
	}
}

void cltNCharStatus::Create()
{
	stTabTextInfo tmp_stTabTextInfo;
	
	if ( ! IsCreate() )
	{
		cltClient *pclclient = (cltClient*)pclClient;

		pclCM	= pclclient->pclCM;
		CharUnique = pclclient->MyCharUnique;

		if ( pclClient->IsCountrySwitch(Switch_SoulGuard) )
		{
			m_siChildDlgXPos = 10;
			m_siChildDlgYPos = 75;
		}
		else
		{
			m_siChildDlgXPos = 10;
			m_siChildDlgYPos = 50;
		}

		CInterfaceFile file;
		if ( pclClient->IsCountrySwitch(Switch_SoulGuard) )
		{
			file.LoadFile(TEXT("NInterface/Data/NCharStatusDlg/DLG_NCharStatus_Parent_SoulGuard.ddf"));
		}
		else
		{
			file.LoadFile(TEXT("NInterface/Data/NCharStatusDlg/DLG_NCharStatus_Parent.ddf"));
		}

		file.CreatePopupDialog( this, CLTNCHARSTATUS_DLG, TEXT("dialog_charstatus"), NCharStatusDlgStaticProc );


		NEWCBUTTON( m_pButton_person );
		NEWCBUTTON( m_pButton_horse );
		NEWCBUTTON( m_pButton_summon );
		NEWCBUTTON( m_pButton_stock );
		NEWCBUTTON( m_pButton_ok );
		NEWCBUTTON( m_pButton_warp );

		file.CreateControl( m_pButton_person,	CLTNCHARSTATUS_DLG_BUTTON_PERSON,	TEXT("button_person") );
		file.CreateControl( m_pButton_horse,	CLTNCHARSTATUS_DLG_BUTTON_HORSE,	TEXT("button_horse") );
		file.CreateControl( m_pButton_summon,	CLTNCHARSTATUS_DLG_BUTTON_SUMMON,	TEXT("button_summon") );
		file.CreateControl( m_pButton_stock,	CLTNCHARSTATUS_DLG_BUTTON_STOCK,	TEXT("button_stock") );
		file.CreateControl( m_pButton_ok,		CLTNCHARSTATUS_DLG_BUTTON_OK,		TEXT("button_ok") );
		file.CreateControl( m_pButton_warp,		CLTNCHARSTATUS_DLG_BUTTON_WARP,		TEXT("button_Warp") );
	
		TCHAR* pTitle = GetTxtFromMgr(3722);
		SetTitle(pTitle);

        TCHAR* pText = GetTxtFromMgr(3723);
		 m_pButton_horse->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		 pText = GetTxtFromMgr(449);
		 m_pButton_summon->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		 pText = GetTxtFromMgr(600);
		 m_pButton_stock->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		 //[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
		 if(pclClient->IsCountrySwitch(Switch_CharStatusReform))
		 {
			pText = GetTxtFromMgr(8297);
		 }
		 else
		 {
			pText = GetTxtFromMgr(3889);
		 }
		 m_pButton_ok->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);



		SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
		if(id == 0)
		{
			DeleteDialog();
			return;
		}

		// 사용자 
		TCHAR* pTmpChar = NULL;
		pTmpChar = (TCHAR*)pclCM->CR[id]->GetName();

		// [영훈] 수호 정령
		if ( pclClient->IsCountrySwitch(Switch_SoulGuard) )
		{
			// 캐릭터 이름
			m_pButton_person->SetText( pTmpChar, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

			// 버튼 생성
			NEWCBUTTON( m_pButton_soulguard );
			file.CreateControl( m_pButton_soulguard,	CLTNCHARSTATUS_DLG_BUTTON_SOULGUARD,	TEXT("button_soulguard") );

			// 수호 정령
			TCHAR* pszText = GetTxtFromMgr( 30146 );
			m_pButton_soulguard->SetText( pszText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
			m_pButton_soulguard->Refresh();

			// 텝 다이얼로그 생성
			if( !m_pTabDlg4 )
			{
				m_pTabDlg4 = new cltNSoulGuardStatus( pclCM );
			}

			if( !m_pTabDlg4->IsCreate() )
			{
				m_pTabDlg4->Create( this );
				m_pTabDlg4->m_siCharUnique = CharUnique;
			}
		
		}
		else
		{
			SI16 siTmp = 12;
			SI16 siLength = 0;

			siLength = _tcslen(pTmpChar);
			if( siLength > 10 )
			{
				switch ( pclclient->siServiceArea )
				{
				case ConstServiceArea_English:
				case ConstServiceArea_NHNChina:
				case ConstServiceArea_USA:
				case ConstServiceArea_EUROPE:
					siTmp = 12;
					break;
				case ConstServiceArea_China:
				case ConstServiceArea_Korea:
				case ConstServiceArea_Taiwan:
				case ConstServiceArea_Japan:
					siTmp = 10;
					break;
				}

				m_pButton_person->SetFontSize( siTmp );

				if( siLength > 12 ) 
					m_pButton_person->SetText( pTmpChar, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
				else
					m_pButton_person->SetText( pTmpChar, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK );

				// 사람

			}
			else
			{
				switch ( pclClient->siServiceArea )
				{
				case ConstServiceArea_English:
				case ConstServiceArea_NHNChina:
				case ConstServiceArea_USA:
				case ConstServiceArea_EUROPE:
					siTmp = 13;
					break;
				case ConstServiceArea_China:
				case ConstServiceArea_Korea:
				case ConstServiceArea_Taiwan:
				case ConstServiceArea_Japan:
					break;
				}

				m_pButton_person->SetFontSize( siTmp );
				m_pButton_person->SetText( pTmpChar, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK );
			}
		}


		// 말 
		pText = GetTxtFromMgr(372);
		m_pButton_horse->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

		// 소환수
		pText = GetTxtFromMgr(449);
		m_pButton_summon->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

		// 자산  
		pText = GetTxtFromMgr(600);
		m_pButton_stock->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );


		m_siCurrentStatusPage	= 0;

		if( !m_pTabDlg0 )
		{
			m_pTabDlg0 = new cltNPersonStatus( pclCM );
		}
		if( !m_pTabDlg1 )
		{
			m_pTabDlg1 = new cltNHorseStatus( pclCM );
		}
		if( !m_pTabDlg2 )
		{
			m_pTabDlg2 = new cltNSummonStatus( pclCM );
		}
		if( !m_pTabDlg3 )
		{
			m_pTabDlg3 = new cltNAssetStatus( pclCM );
		}

		if( !m_pTabDlg0->IsCreate() )
		{
			m_pTabDlg0->Create( this );
			m_pTabDlg0->CharUnique = CharUnique;
		}
		if( !m_pTabDlg1->IsCreate() )
		{
			m_pTabDlg1->Create( this );
			m_pTabDlg1->CharUnique = CharUnique;
		}
		if( !m_pTabDlg2->IsCreate() )
		{
			m_pTabDlg2->Create( this );
			m_pTabDlg2->CharUnique = CharUnique;
		}
		if( !m_pTabDlg3->IsCreate() )
		{
			m_pTabDlg3->Create( this );
			m_pTabDlg3->CharUnique = CharUnique;
		}
		
		ShowCurrentTab( 0 );
	
		m_pButton_horse->Refresh();
		m_pButton_summon->Refresh();
		m_pButton_stock->Refresh();
		m_pButton_person->Refresh();

		SetActionTimer( 800 );
		SetTimerOnOff( true );
		
		m_timerCount = 0;

		Hide();
	}
	else
	{
		if( IsShow() )
			Hide();
		else
			Show();
		//DeleteDialog();
	}
}

void CALLBACK cltNCharStatus::NCharStatusDlgStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
    cltNCharStatus *pThis = (cltNCharStatus*)pControllerMgr;
	pThis->NCharStatusDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK cltNCharStatus::NCharStatusDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
	stTabTextInfo tmp_stTabTextInfo;
  
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case CLTNCHARSTATUS_DLG_BUTTON_PERSON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowCurrentTab( 0 );
				}
				break;
			}
		}
		break;
	case CLTNCHARSTATUS_DLG_BUTTON_HORSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowCurrentTab( 1 );
				}
				break;
			}
		}
		break;
	case CLTNCHARSTATUS_DLG_BUTTON_SUMMON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowCurrentTab( 2 );
				}
				break;
			}
		}
		break;
	case CLTNCHARSTATUS_DLG_BUTTON_STOCK:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowCurrentTab( 3 );
				}
				break;
			}
		}
		break;
	case CLTNCHARSTATUS_DLG_BUTTON_SOULGUARD:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowCurrentTab( 4 );
				}
				break;
			}
		}
		break;
	
	case COMMON_DIALOG_CLOSEBUTTON:
		{

			//// 윈도우의 위치를 기억한다. 
			//GetWindowRect(m_hDlg, &DlgRect);
			DlgRectInitSwitch = TRUE;

            DeleteDialog();
		}
		break;

	case CLTNCHARSTATUS_DLG_BUTTON_OK:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
					if(pclClient->IsCountrySwitch(Switch_CharStatusReform))
					{
						// 소환수 정보
						if ( 2 == m_siCurrentStatusPage )
						{
							if ( pclclient->m_pDialog[NCHARSUMMONRESET_DLG] )
								break;

							pclclient->CreateInterface( NCHARSUMMONRESET_DLG );
						}
						// 수호정령 강화정보창
						else if ( 4 == m_siCurrentStatusPage )
						{
							if ( NULL == pclclient->m_pDialog[NCHARSOULGUARDINFO_DLG] )
							{
								pclclient->CreateInterface( NCHARSOULGUARDINFO_DLG );
							}
						}
						else
						{
							if ( pclclient->m_pDialog[CLTNCHARGREETINGCHANGE_DLG] )
								break;

							pclclient->CreateInterface( CLTNCHARGREETINGCHANGE_DLG );

						}
					}
					else
					{
						DeleteDialog();
					}
				}
				break;
			}
		}
		break;

	case CLTNCHARSTATUS_DLG_BUTTON_WARP:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 id = 1;

					SI32 questclearcond = QUEST_CLEAR_COND_WARPTOPALACE;
					SI16 questtype, questunique, para1, para2;
					bool bDoingQuest = pclClient->pclCM->CR[id]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2);

					if(bDoingQuest == false)
					{
						//KHY - 0705 - 같은 엔펠리스상에서는 이동 불가.
						if( pclClient->pclCM->CR[id]->GetMapIndex() == MAPINDEX_PALACE)
						{
							TCHAR* pTitle = GetTxtFromMgr(1413);
							TCHAR* pText  = GetTxtFromMgr(5603);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return;
						}

						TCHAR*	pText = TEXT("");
						TCHAR	buffer[256] = {0,};
						SI32	siMapindex = MAPINDEX_PALACE;
 
						if( pclclient->pclCM->CR[1]->CanWarpFrom() == false )
							return;

						if( pclclient->pclCM->CR[1]->CanFreeWarp( siMapindex ) == false)
						{
							//KHY - 0703 - 게임 머니로 귀환하기.
							if(pclclient->m_pDialog[ NCOMMONMSG_DLG ]==NULL)
							{
								pclclient->CreateInterface( NCOMMONMSG_DLG );
							}
							((CNCommonMsgDlg*)pclclient->m_pDialog[ NCOMMONMSG_DLG ])->SetWarpMoney( siMapindex, WarpMoney() );
							((CNCommonMsgDlg*)pclclient->m_pDialog[ NCOMMONMSG_DLG ])->Set( COMMONDLG_TICKETMONEY_WARP );
						}
						else
						{
							cltGameMsgRequest_HuntMapWarp pclInfo(siMapindex, 0);
							cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (BYTE*)&pclInfo );
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
					}
					// 퀘스트 수행중이다 - 퀘스트 수행중에는 공짜로 이동시켜준다.
					else
					{
						// 일본
						//cyj 일본남,   일본여, 서양남, 서양여 일때 일본 거점으로 이동
						if(pclClient->pclCM->CR[id]->pclCI->clBI.GetKind() >= 5 && pclClient->pclCM->CR[id]->pclCI->clBI.GetKind() <= 8)
						{
							if( pclClient->pclCM->CR[id]->GetMapIndex() == MAPINDEX_PALACE)
							{
								TCHAR* pTitle = GetTxtFromMgr(1413);
								TCHAR* pText  = GetTxtFromMgr(5603);

								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
								return;
							}

							cltGameMsgRequest_HuntMapWarp pclInfo(MAPINDEX_PALACE, 0);
							// 왜구마을? -> 군주 해외용으로 변경
							//cltMsg clMsg( GAMEMSG_REQUEST_WARJOIN, sizeof( pclInfo ), (BYTE*)&pclInfo );
							cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (BYTE*)&pclInfo );
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
						else
						{
							if(pclClient->pclCM->CR[id]->pclCI->clBI.GetKind() == 1 || pclClient->pclCM->CR[id]->pclCI->clBI.GetKind() == 2)
							{
								if( pclClient->pclCM->CR[id]->GetMapIndex() == 158)
								{
									TCHAR* pTitle = GetTxtFromMgr(1413);
									TCHAR* pText  = GetTxtFromMgr(5603);

									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
									return;
								}

								cltGameMsgRequest_HuntMapWarp pclInfo(MAPINDEX_PALACE, 0);
								cltMsg clMsg( GAMEMSG_REQUEST_WARJOIN, sizeof( pclInfo ), (BYTE*)&pclInfo );
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
							}
							else if(pclClient->pclCM->CR[id]->pclCI->clBI.GetKind() == 3)
							{
								if( pclClient->pclCM->CR[id]->GetMapIndex() == 158)
								{
									TCHAR* pTitle = GetTxtFromMgr(1413);
									TCHAR* pText  = GetTxtFromMgr(5603);

									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
									return;
								}

								cltGameMsgRequest_HuntMapWarp pclInfo(MAPINDEX_PALACE, 0);
								cltMsg clMsg( GAMEMSG_REQUEST_WARJOIN, sizeof( pclInfo ), (BYTE*)&pclInfo );
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
							}
						}

						//한양으로 이동하기 퀘스트가 진행중이라면
						pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_WARPTOPALACE);
					}
				}
				break;
			}
		}
		break;
	}
}

void cltNCharStatus::SetTabTextInfo( stTabTextInfo* in_SelectedText )
{
	if( in_SelectedText && in_SelectedText->st_pControllerObj )
	{
		in_SelectedText->st_pControllerObj->SetFontColor( in_SelectedText->st_Color );
		in_SelectedText->st_pControllerObj->SetFontWeight( in_SelectedText->st_siWeight );
		in_SelectedText->st_pControllerObj->Refresh();
	}
}

void cltNCharStatus::Action()
{
	if( m_timerCount >= LEAST_COMMON_MULTILPLE + 1 )
		m_timerCount = 1;

	switch( m_siCurrentStatusPage )
	{
	case 0:	// 개인탭
		{
			if(m_pTabDlg0 && m_pTabDlg0->IsCreate() && m_pTabDlg0->IsShow())
				m_pTabDlg0->Action();
		}
		break;

	case 1:	// 말 탭
		{
			if(m_pTabDlg1 && m_pTabDlg1->IsCreate() && m_pTabDlg1->IsShow())
				m_pTabDlg1->Action();
		}
		break;

	case 2:	// 소환수 탭
		{
		}
		if(m_pTabDlg2 && m_pTabDlg2->IsCreate() && m_pTabDlg2->IsShow())
		{
			m_pTabDlg2->Action();
		}
		break;
	case 3:	// 자산현황 탭
		if(m_pTabDlg3 && m_pTabDlg3->IsCreate() && m_pTabDlg3->IsShow())
		{
			if( m_timerCount % ASSET_TAB_ACTION_INTERVAL_VALUE == 0 )
			{
				m_pTabDlg3->Action();
			}
		}
		break;
	case 4: // 수호정령 탭
		if ( m_pTabDlg4 && m_pTabDlg4->IsCreate() && m_pTabDlg4->IsShow() )
		{
			m_pTabDlg4->Action();
		}
		break;
	
	}

	m_timerCount++;
}

void cltNCharStatus::Hide()
{
	m_bShow = false;
	CControllerMgr::Show( false );

	if( pclClient->IsCountrySwitch( Switch_Elixir ) )
	{
		//[진성] 엘릭서 스테이터스 설정 창 제거.
		cltClient *pclclient = (cltClient*)pclClient;
		if( pclclient->m_pDialog[NELIXIR_STATUS_DLG] ) 
		{
			pclclient->DestroyInterface( pclclient->m_pDialog[NELIXIR_STATUS_DLG] );
		}
	}
}

void cltNCharStatus::Show()
{
	m_bShow = true;

	switch( m_siCurrentStatusPage )
	{
	case 0:	// 개인탭
		if(m_pTabDlg0 && m_pTabDlg0->IsCreate() && m_pTabDlg0->IsShow())
			m_pTabDlg0->Action();
			break;

	case 1:	// 말 탭
		if(m_pTabDlg1 && m_pTabDlg1->IsCreate() && m_pTabDlg1->IsShow())
			m_pTabDlg1->Action();
			break;
	case 2:	// 소환수 탭
		if(m_pTabDlg2 && m_pTabDlg2->IsCreate() && m_pTabDlg2->IsShow())
		{
			m_pTabDlg2->Action();
		}
		break;
	case 3:	// 자산현황 탭
		if(m_pTabDlg3 && m_pTabDlg3->IsCreate() && m_pTabDlg3->IsShow())
		{
			//if( m_timerCount % ASSET_TAB_ACTION_INTERVAL_VALUE == 0 )
				m_pTabDlg3->Action();
		}
		break;
	case 4: // 수호정령 탭
		if ( m_pTabDlg4 && m_pTabDlg4->IsCreate() && m_pTabDlg4->IsShow() )
		{
			m_pTabDlg4->Action();
		}
		break;
		
	}

	CControllerMgr::Show( true );
}

bool cltNCharStatus::IsShow()
{
    return m_bShow;
}

void cltNCharStatus::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient* pclclient = (cltClient*)pclClient;
	switch( MsgIndex )
	{
	case 0:	// Person Tab => Strength Minus
	case 1:	// Person Tab => Dex Minus
	case 2:	// Person Tab => Maic Minus
	case 3:	// Person Tab => Life Minus
	case 4: // Person Tab => HandSkill Minus
	case 5: // Person Tab => Luk Minus
	case 6: // Person Tab => Init Button
	case 7: // Person Tab => Apply Button
		{
			m_pTabDlg0->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		}
		break;
	case 10:	// 말 능력치 초기화 하기
	case 11:	// 말 능력 향상 하기
	case 12:	// 말 풀어 주기
	case 13:	// 주유하기
		{
			m_pTabDlg1->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		}
		break;
	case 20:	// Summon Tab => Strength Minus
	case 21:	// Summon Tab => Dex Minus
	case 22:	// Summon Tab => Maic Minus
	case 23:	// Summon Tab => Life Minus
	case 24:	// Summon Tab => Summon is Dead!!
	case 25:	// Summon Free!
	case 26:	// 소환수부활이용권 없음 알림창
	case 27:	// summon revive
	case 28:	// 상태초기화이용권
	case 29:	// Summon Tab => Luk Minus
	case 31:	// 소환수 스테이터스 적용
		{
			m_pTabDlg2->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		}
		break;
	//KHY - 0703 - 게임 머니로 귀환하기.
	case 30:	// 경복궁으로 이동
		{
			TCHAR*	pText = TEXT("");
			TCHAR	buffer[256] = {0,};
			
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue ) // 소지금으로 이동.
			{

				//소지금이 순간이동 비용보다 많은지 검사.
				GMONEY mymoney = pclCM->CR[1]->pclCI->clIP.GetMoney();
				SI64		wMoney = (SI64)WarpMoney();
				
				if(wMoney > mymoney)
				{
					pText= GetTxtFromMgr(7540);
					StringCchPrintf(buffer, 256, pText);
					pText= GetTxtFromMgr(7538);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, buffer );
					return;
				}
				else
				{
					//KHY - 0703 - 게임 머니로 귀환하기.
					cltGameMsgRequest_HuntMapWarp pclInfo(*((SI32*)pData),wMoney);
					cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (BYTE*)&pclInfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}

			}
			else //순간이동권으로 이동.
			{

				SI32 itemunique = ITEMUNIQUE(24000);  // 순간이동이용권   14005 -> 24000
				if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
				{
					pText= GetTxtFromMgr(7540);
					StringCchPrintf(buffer, 256, pText);
					pText= GetTxtFromMgr(7538);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, buffer );
					return;
				}
				else
				{
					cltGameMsgRequest_HuntMapWarp pclInfo(*((SI32*)pData),0);
					cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (BYTE*)&pclInfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);					
				}

			}
		}
		break;
	// 수호정령 다이얼로그
	case 40:	// 수호정령 풀어주기
	case 41:	// 수호정령 보관소 추가 생성
		{
			m_pTabDlg4->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		}
		break;
	}
}

//KHY - 0703 - 게임 머니로 귀환하기.
SI32 cltNCharStatus::WarpMoney()
{
	cltClient* pclclient = (cltClient*)pclClient;

	REAL32	fMoney1;
	SI32	siMoney2 ;
	SI32	glevel = pclclient->pclCM->CR[1]->pclCI->clIP.GetLevel();
	REAL32	npcrate = (REAL32)pclclient->pclItemManager->clItemStatistic.siNPCPriceRate;

	//KHY - 0711 - 게임 머니로 귀환하기.
	npcrate = npcrate / (REAL32)100;

	fMoney1 = ((REAL32)5.0 * (glevel / (REAL32)10.0));
	fMoney1 = fMoney1 * fMoney1;
	
	siMoney2 = (SI32)((REAL32)fMoney1* npcrate);

	siMoney2 = min(siMoney2, (SI32)fMoney1*2);		// MAX 200%만 시세 적용.
	siMoney2 = max(siMoney2, 1);					//최소값은 1이다.
	
	return siMoney2;
}

void cltNCharStatus::ChangeButtonText()
{
	if(false == pclClient->IsCountrySwitch(Switch_SummonReform))
		return;
	
	TCHAR *pText = NULL;
	
	if ( 2 == m_siCurrentStatusPage )
	{
		pText = GetTxtFromMgr(8298);
	}
	else if ( 4 == m_siCurrentStatusPage)
	{
		pText = GetTxtFromMgr(8301);
	}
	else
	{
		pText = GetTxtFromMgr(8297);
	}

	if ( pText != NULL )
	{
		m_pButton_ok->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	
}

void cltNCharStatus::ShowCurrentTab( SI32 siTabNumber )
{
	if ( pclClient->IsCountrySwitch(Switch_SoulGuard) )
	{
		if ( (0 > siTabNumber) || (4 < siTabNumber) )	return;
	}
	else
	{
		if ( (0 > siTabNumber) || (3 < siTabNumber) )	return;
	}

	m_timerCount			= 0;	// Timer Counting Initialization!

	m_siCurrentStatusPage	= siTabNumber;


	// 텝 다이얼로그 Show/Hide 설정
	if ( 0 == siTabNumber )	m_pTabDlg0->Show();
	else					m_pTabDlg0->Hide();

	if ( 1 == siTabNumber )	m_pTabDlg1->Show();
	else					m_pTabDlg1->Hide();

	if ( 2 == siTabNumber )	m_pTabDlg2->Show();
	else					m_pTabDlg2->Hide();		

	if ( 3 == siTabNumber )	m_pTabDlg3->Show();
	else					m_pTabDlg3->Hide();

	if ( pclClient->IsCountrySwitch(Switch_SoulGuard) )
	{
		if ( 4 == siTabNumber )	m_pTabDlg4->Show();
		else					m_pTabDlg4->Hide();
	}

	// 텝 버튼 설정
	SI32			siNowIndex = 0;
	stTabTextInfo	tmp_stTabTextInfo;

	COLORREF		colorButton[MAX_CHARSTATUS_PAGE_NUMBER]	= {0,};
	SI16			siWeight[MAX_CHARSTATUS_PAGE_NUMBER]	= {0,};

	for ( SI32 siIndex=0; siIndex<MAX_CHARSTATUS_PAGE_NUMBER; siIndex++ )
	{
		if ( siIndex == siTabNumber )
		{
			colorButton[siIndex]	= CONTROLLER_BUTTONTEXT_COLOR;
			siWeight[siIndex]		= FW_BOLD;
		}
		else
		{
			colorButton[siIndex]	= CONTROLLER_BACK_BUTTONTEXT_COLOR;
			siWeight[siIndex]		= FW_NORMAL;
		}
	}
	

	tmp_stTabTextInfo.Init();
	tmp_stTabTextInfo.Set( m_pButton_person,	colorButton[siNowIndex],	siWeight[siNowIndex] );	siNowIndex++;
	SetTabTextInfo( &tmp_stTabTextInfo );

	tmp_stTabTextInfo.Init();
	tmp_stTabTextInfo.Set( m_pButton_horse,		colorButton[siNowIndex],	siWeight[siNowIndex] );	siNowIndex++;
	SetTabTextInfo( &tmp_stTabTextInfo );

	tmp_stTabTextInfo.Init();
	tmp_stTabTextInfo.Set( m_pButton_summon,	colorButton[siNowIndex],	siWeight[siNowIndex] );	siNowIndex++;
	SetTabTextInfo( &tmp_stTabTextInfo );

	tmp_stTabTextInfo.Init();
	tmp_stTabTextInfo.Set( m_pButton_stock,		colorButton[siNowIndex],	siWeight[siNowIndex] );	siNowIndex++;
	SetTabTextInfo( &tmp_stTabTextInfo );

	if ( pclClient->IsCountrySwitch(Switch_SoulGuard) )
	{
		tmp_stTabTextInfo.Init();
		tmp_stTabTextInfo.Set( m_pButton_soulguard,	colorButton[siNowIndex],	siWeight[siNowIndex] );	siNowIndex++;
		SetTabTextInfo( &tmp_stTabTextInfo );
	}

	ChangeButtonText();
}
//#endif

