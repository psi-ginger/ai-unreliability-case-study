#include "NRequestHuntingMapWarDlg.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../common/Char/CharManager/CharManager.h"
#include "../../Common/Map/MapManager/MapManager.h"
#include "../../Client/Client.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


extern cltCommonLogic* pclClient;

CNRequestHuntingMapWarDlgDlg::CNRequestHuntingMapWarDlgDlg()
{
	Init();
}

CNRequestHuntingMapWarDlgDlg::~CNRequestHuntingMapWarDlgDlg()
{
	Destroy();
}

void CNRequestHuntingMapWarDlgDlg::Init()
{
	m_pNNPCMenuStatic1	=	NULL	;
	m_pNNPCMenuStatic2	=	NULL	;
}

void CNRequestHuntingMapWarDlgDlg::Destroy()
{
	NDelete(m_pNNPCMenuStatic1);
	NDelete(m_pNNPCMenuStatic2);
}

void CNRequestHuntingMapWarDlgDlg::Create()
{
	if ( IsCreate() ) return;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NPlayMatchDlg/DLG_NGuildHutmapRequest.ddf"));
	file.CreatePopupDialog( this, NREQUESTHUNTMAPWARREQUEST, "dialog_huntmapwarrequest", StaticRequestHuntingMapWarDlgDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC , NHUNTMAPWARREQUEST_STATIC_NPCFACE, this) ,NHUNTMAPWARREQUEST_STATIC_NPCFACE, TEXT("statictext_npcmenu1"));			// NPC 얼굴
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC , NHUNTMAPWARREQUEST_STATIC_NPCTALK, this) ,NHUNTMAPWARREQUEST_STATIC_NPCTALK, TEXT("statictext_npcmenu2"));			// NPC 말
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX , NHUNTMAPWARREQUEST_COMBO_MAP, this) ,NHUNTMAPWARREQUEST_COMBO_MAP, TEXT("combobox_NONAME1"));					// 사냥터 목록 콤보 박스
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NHUNTMAPWARREQUEST_BUTTON_REQUEST_MAP, this)  , NHUNTMAPWARREQUEST_BUTTON_REQUEST_MAP , TEXT("button_NONAME1"));	// 사냥터 신청 버튼
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST , NHUNTMAPWARREQUEST_LIST_POINTTOP10, this) ,NHUNTMAPWARREQUEST_LIST_POINTTOP10, TEXT("listbox_NONAME2"));				// 길드전 top10 리스트 박스	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC , NHUNTMAPWARREQUEST_STATIC_REMAINTIME, this) ,NHUNTMAPWARREQUEST_STATIC_REMAINTIME, TEXT("statictext_NONAME1"));	// 남은시간
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NHUNTMAPWARREQUEST_BUTTON_REQUEST_WAR, this)  , NHUNTMAPWARREQUEST_BUTTON_REQUEST_WAR , TEXT("button_NONAME2"));	// 길드전 신청 버튼
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NHUNTMAPWARREQUEST_BUTTON_REQUEST_EXIT, this)  , NHUNTMAPWARREQUEST_BUTTON_REQUEST_EXIT , TEXT("button_NONAME3"));	// 닫기 버튼

	// 리스트 박스 셋팅
	CList* pList = m_InterfaceFactory.GetList(NHUNTMAPWARREQUEST_LIST_POINTTOP10);
	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText =GetTxtFromMgr(5322);
	pList->SetColumn(0,110,pText);
	pText = GetTxtFromMgr(10068);
	pList->SetColumn(1,60,pText);
	pText = GetTxtFromMgr(9961);
	pList->SetColumn(2,60,pText);
	pText = GetTxtFromMgr(10070);
	pList->SetColumn(3,60,pText);
	pText = GetTxtFromMgr(6731);
	pList->SetColumn(4,60,pText);
	pList->Refresh();

	//NPC 대사및 얼굴  설정

	CStatic* pStaticFace = m_InterfaceFactory.GetStatic(NHUNTMAPWARREQUEST_STATIC_NPCFACE);
	CStatic* pStaticTalk = m_InterfaceFactory.GetStatic(NHUNTMAPWARREQUEST_STATIC_NPCTALK);
	
	pStaticFace->SetBorder(true);
	pStaticFace->SetBKColorUse(true);
	pStaticTalk->SetBorder(true);
	pStaticTalk->SetBKColorUse(true);
	
	// 대사를 가지고 온다.-기본대사-
	TCHAR *pNPCTalk = GetTxtFromMgr(9956);
	TCHAR buf[256];
	StringCchPrintf(buf, 256, pNPCTalk, BLACKARMY_APPLY_MONEY);
	pStaticTalk->SetText(buf);
	
	//얼굴 가지고 온다
	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_BATTLEZONE")), szPortrait, sizeof(szPortrait), &siPortraitFont);
	pStaticFace->SetFileName(szPortrait);
	pStaticFace->SetFontIndex(siPortraitFont);

	SI32 rankType = pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.GetRankType();
	if ( rankType != GUILDRANKTYPE_MASTER  && rankType != GUILDRANKTYPE_SECONDMASTER)
	{
		// 길드 장이 아니면 패당 버튼은 못쓰도록 한다.
		(m_InterfaceFactory.GetButton(NHUNTMAPWARREQUEST_BUTTON_REQUEST_WAR))->Enable( false ); // 길드전 신청
		(m_InterfaceFactory.GetButton(NHUNTMAPWARREQUEST_BUTTON_REQUEST_MAP))->Enable( false ); // 길드맵 선택
	}
	SetComboBox();
	SetActionTimer(100)	;
	SetTimerOnOff( true );

	cltGameMsgRequest_GuildHuntMapWarWar_Time clinfo( pclClient->pclCM->CR[1]->pclCI->GetPersonID() );	// 정확한 서버 시간을 내놔 초까지.
	cltMsg clMsg(GAMEMSG_REQUEST_GUILDHUNTMAPWAR_SERVERTIME, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	cltGameMsgRequest_GuildHuntMapWarWar_HaveMapIndex clinfo2( pclClient->pclCM->CR[1]->pclCI->GetPersonID() );	// 정확한 서버 시간을 내놔 초까지.
	cltMsg clMsg2(GAMEMSG_REQUEST_GUILDHUNTMAPWAR_HAVEMAP, sizeof(clinfo2), (BYTE*)&clinfo2);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg2);

	
}

void CALLBACK CNRequestHuntingMapWarDlgDlg::StaticRequestHuntingMapWarDlgDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNRequestHuntingMapWarDlgDlg *pThis = (CNRequestHuntingMapWarDlgDlg*)pControllerMgr;
	pThis->NRequestHuntingMapWarDlgDlgProc( nEvent, nControlID, pControl);
}

void CNRequestHuntingMapWarDlgDlg::NRequestHuntingMapWarDlgDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON://오른쪽 끝 X버튼
		{
			DeleteDialog();
		}
		break;
	case NHUNTMAPWARREQUEST_BUTTON_REQUEST_EXIT:	// 닫기 버튼
		{
			switch ( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}break;
		//사냥터 신청 버튼
	case NHUNTMAPWARREQUEST_BUTTON_REQUEST_MAP:
		{
			switch (nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient* pclclient = (cltClient*)pclClient;
					TCHAR* pTitle = GetTxtFromMgr(6735);
					TCHAR* pText = GetTxtFromMgr(6736);

					CComboBox* pCombobox = m_InterfaceFactory.GetComboBox( NHUNTMAPWARREQUEST_COMBO_MAP )	;
					SI32 selectedindex = pCombobox->GetSelectedIndex();

					if ( selectedindex >= 0 )
					{
						m_siSelectedMapIndex = pCombobox->GetParam(selectedindex);
					}
					else	//길드전 맵을 선택하라는 메세지를 띄운다.
					{
						TCHAR* pTitle = TEXT(" "); 
						TCHAR* pText = GetTxtFromMgr(9947);						
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					TCHAR buffer[256];
					StringCchPrintf( buffer, 256, pText, pclClient->pclMapManager->GetMapName( m_siSelectedMapIndex ) );
					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle,buffer, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, (BYTE*)&m_siSelectedMapIndex, sizeof(SI32) );

					
				}break;
			}break;
		}
		//길드전 신청 버튼 
	case NHUNTMAPWARREQUEST_BUTTON_REQUEST_WAR:
		{
			switch ( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient* pclclient = (cltClient*)pclClient;
					TCHAR* pTitle = TEXT(" ");
					TCHAR* pText = GetTxtFromMgr(9954);

					CComboBox* pCombobox = m_InterfaceFactory.GetComboBox( NHUNTMAPWARREQUEST_COMBO_MAP )	;
					SI32 selectedindex = pCombobox->GetSelectedIndex();
					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle,pText, MSGBOX_TYPE_YESNO, 1 );
					pclclient->SetMsgBox( &MsgBox, (BYTE*)&m_siSelectedMapIndex, sizeof(SI32) );

				}
				break;
			}
			break;
		}
	case NHUNTMAPWARREQUEST_COMBO_MAP:
		{
			switch ( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siSelectedMapIndex = 0;
					CComboBox* pCombo = m_InterfaceFactory.GetComboBox(NHUNTMAPWARREQUEST_COMBO_MAP);

					SI32 selectedindex = pCombo->GetSelectedIndex();

					m_siSelectedMapIndex = pCombo->GetParam(selectedindex);

					if ( m_siSelectedMapIndex >= 0 )
					{
						// 톱10을 위해서 메세지를 날린다.
                        cltGameMsgRequest_GuilHuntMapdWar_GetTopTen clInfo(m_siSelectedMapIndex);
						cltMsg clMsg(GAMEMSG_REQUEST_GUILDHUNTMAPWAR_GETTOP10, sizeof(clInfo), (BYTE*)&clInfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
				break;
			}break;
		}

	}

}
void CNRequestHuntingMapWarDlgDlg::SetTime( SYSTEMTIME stTIme )
{
	stOpenTiem.wMinute	=	stTIme.wMinute	;	
	stOpenTiem.wSecond	=	stTIme.wSecond	;

	OpenTime	=	pclClient->CurrentClock	;
}
void CNRequestHuntingMapWarDlgDlg::SetButton(SI32 siMapindex )
{
	if ( siMapindex > 0 )
	{
		(m_InterfaceFactory.GetButton(NHUNTMAPWARREQUEST_BUTTON_REQUEST_MAP))->Enable( false ); // 길드맵 선택
		(m_InterfaceFactory.GetButton(NHUNTMAPWARREQUEST_BUTTON_REQUEST_WAR))->Enable( true ); // 길드전 신청
	}
	else if ( siMapindex <= 0 )
	{
		(m_InterfaceFactory.GetButton(NHUNTMAPWARREQUEST_BUTTON_REQUEST_MAP))->Enable( true ); // 길드맵 선택
		(m_InterfaceFactory.GetButton(NHUNTMAPWARREQUEST_BUTTON_REQUEST_WAR))->Enable( false ); // 길드전 신청
	}
}
void CNRequestHuntingMapWarDlgDlg::Action()
{
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 remainMinute = 0	;
	SI32 remainSecond = 0	;

	SI32 resultminute = 0	;
	SI32 resultsecond = 0	;

	SI32 TimeSection = 0	;


	SI32 siPassTime = TABS(pclClient->CurrentClock - OpenTime) /1000 ;
	if ( siPassTime > 0)
	{
		stOpenTiem.wSecond = stOpenTiem.wSecond  + siPassTime ;	// 시간이 지나간다.
		OpenTime = pclClient->CurrentClock	;
		if ( stOpenTiem.wSecond > 60  )
		{
			stOpenTiem.wSecond	=	0;

			if ( stOpenTiem.wSecond <= 0 )
			{
				cltGameMsgRequest_GuildHuntMapWarWar_Time clinfo( pclClient->pclCM->CR[1]->pclCI->GetPersonID() );	// 정확한 서버 시간을 내놔 초까지.
				cltMsg clMsg(GAMEMSG_REQUEST_GUILDHUNTMAPWAR_SERVERTIME, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
	}

	SI32 siMinute = stOpenTiem.wMinute;
	SI32 siSecont = stOpenTiem.wSecond;
	if (  siSecont > 0 )
	{
		resultsecond = 60 - siSecont	;
	}

	if ( 0 < siMinute && 20 >= siMinute  )
	{
		TimeSection	=	Round_1;
	}
	else if ( 20 < siMinute && 40 >= siMinute )
	{
		TimeSection	=	Round_2;
	}
	else if ( 40 < siMinute && 59 >= siMinute )
	{
		TimeSection	=	Round_3;
	}
	switch ( TimeSection )
	{
	case Round_1:
		{
			resultminute = 20 - siMinute ;
		}break	;
	case Round_2:
		{
			resultminute = 40 - siMinute ;

		}break	;
	case Round_3 :
		{
			resultminute = 59 - siMinute + 1 ;

		}break	;
	}
	//텍스트 만든다
	NTCHARString128 text;
	text = GetTxtFromMgr(9944); 
	text.Replace("#minute#",SI32ToString(resultminute));
	text.Replace("#second#",SI32ToString(resultsecond));
	CStatic* pstTime = m_InterfaceFactory.GetStatic(NHUNTMAPWARREQUEST_STATIC_REMAINTIME);	
	pstTime->SetText(text);
}

void CNRequestHuntingMapWarDlgDlg::SetComboBox()
{
	CComboBox* pCombo = m_InterfaceFactory.GetComboBox(NHUNTMAPWARREQUEST_COMBO_MAP);
	stComboBoxItemData	ItemData;

	for ( SI32 mapindex = 0; mapindex < MAX_MAP_NUMBER ; mapindex++)
	{
		if ( pclClient->pclMapManager->IsGuildMap(mapindex) )
		{
			TCHAR* mapname = pclClient->pclMapManager->GetMapName(mapindex);

			ItemData.Init();
			ItemData.Set(mapindex, mapname);

			pCombo->AddItem( &ItemData );
		}
	}
	pCombo->Refresh();
}
void CNRequestHuntingMapWarDlgDlg::RefreshApplyList()
{
}

void CNRequestHuntingMapWarDlgDlg::SetRankList( cltGameMsgResponse_GuilHuntMapdWar_GetTopTen* pclInfo )
{
	CList* pList = m_InterfaceFactory.GetList(NHUNTMAPWARREQUEST_LIST_POINTTOP10);
	pList->Clear();
	SI32  index = 0;
	stListBoxItemData itemData;
    for ( SI32 i = 0; i< MAX_GUILDHUNTWAR_RANK_NUM; i++)
    {	
		TCHAR* guildName = TEXT("");
		SI32 siGuildUnique = 0	;
		SI32 siWin	=	0	;
		SI32 siLose	=	0	;
		SI32 siDraw =	0	;
		SI32 siBGP	=	0	;

		guildName = pclInfo->Rank[i].GetGuildName();
		if( _tcslen( guildName ) <= 0)	break;

		siWin = pclInfo->Rank[i].siWin	;
		siLose = pclInfo->Rank[i].siLose	;
		siDraw = pclInfo->Rank[i].siDraw	;
		siBGP = pclInfo->Rank[i].GetBGP()	;

		itemData.Init();
		itemData.Set( 0, guildName );
		itemData.Set( 1,  SI32ToString(siWin));
		itemData.Set( 2,  SI32ToString(siDraw));
		itemData.Set( 3,  SI32ToString(siLose));
		itemData.Set( 4,  SI32ToString(siBGP));
		pList->AddItem(&itemData);
    }
	pList->Refresh();
}
void CNRequestHuntingMapWarDlgDlg::ApplyGuildDungeon()
{
}
void CNRequestHuntingMapWarDlgDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 mapindex = *((SI32*)pData);

				cltGameMsgRequest_GuildHuntMapWar_MapRegister clinfo( mapindex );
				cltMsg clMsg(GAMEMSG_REQUEST_GUILDHUNTMAPWAR_MAPREGISTER, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 1:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				//SI32 siGuildindex = pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.GetGuildUnitIndex();	// 선택 맵인덱스
				//SI32 silevel = pclClient->pclGuildManager->clUnit[siGuildindex].clGPoint.siLevel;			// 길드 레벨
				//SI32 siMembernum = pclClient->pclGuildManager->clUnit[siGuildindex].GetMemberNum();			// 길드 원 수
				SI32 GuildIndex = pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.GetGuildUnitIndex();		// 길드 인덱스

				//if(GuildIndex <= 0 || GuildIndex > MAX_GUILD_NUM)
				//	return;
				
				cltGameMsgRequest_GuildHuntMapWarWar_Register clinfo( GuildIndex );
				cltMsg clMsg(GAMEMSG_REQUEST_GUILDHUNTMAPWAR_WARREGISTER, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}break	;
	}
}
