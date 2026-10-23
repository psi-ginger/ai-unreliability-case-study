#include "NResidentsDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ChatEdit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../../Common/Char/CharManager/CharManager.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../CommonLogic/MsgType-Person.h"

#include "../../StatusBar/ChatStatusBar/ChatStatusBarBuffer.h"

#include "../../Interface/ChatDlg/ChatDlg.h"
#include "../../Interface/RichEditChatMgrDlg/RichEditChatDataMgr.h"
#include "../../Interface/RichEditChatMgrDlg/RichEditChatMgrDlg.h"
#include "../../Interface/RichEditChatMgrDlg/RichEditChatMgrDlgBaseInfo.h"

#include "../../Interface/ResidentsViewDlg/ResidentsViewDlg.h"

//#include "../../NInterface/NChatDlg/NChatDlg.h"
#include "../../ninterface/NChatbardlg/NChatBarDlg.h"

#include "MsgType-System.h"
#include "Msg/MsgType-Village.h"
#include "MsgType-Person.h"

#include "../../Music/Music.h"

#include "../../Client.h"

#include "../../CommonLogic/Msg/MsgType-ItemMall.h"

extern cltCommonLogic* pclClient;


CNResidentsDlg::CNResidentsDlg()
{
	m_pVillageResidentsList = NULL;

	m_pChatEdit = NULL;
	m_pInputEdit = NULL;
	m_pComboBox = NULL;
	m_pList = NULL;
	m_pOKButton = NULL;
	m_pRefreshButton = NULL;
	m_pMoveToVillageButton = NULL;
	m_pDetailInfoButton = NULL;
	m_pSendWhisperButton = NULL;
	m_pNameStatic = NULL;
}

CNResidentsDlg::~CNResidentsDlg()
{
	SAFE_DELETE_A( m_pVillageResidentsList );

	SAFE_DELETE( m_pChatEdit );
	SAFE_DELETE( m_pInputEdit );
	SAFE_DELETE( m_pComboBox );
	SAFE_DELETE( m_pList );
	SAFE_DELETE( m_pOKButton );
	SAFE_DELETE( m_pRefreshButton );
	SAFE_DELETE( m_pMoveToVillageButton );
	SAFE_DELETE( m_pDetailInfoButton );
	SAFE_DELETE( m_pSendWhisperButton );
	SAFE_DELETE( m_pNameStatic );
}

void CNResidentsDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NResidentsDlg/DLG_NResidents.ddf"));
	file.CreatePopupDialog( this, NRESIDENTS_DLG, TEXT("dialog_NResidents"), StaticCallBackDialogNResidents );

	m_pChatEdit = new CChatEdit(this);
	m_pInputEdit = new CEdit( this );
	m_pList = new CList( this );
	m_pComboBox = new CComboBox( this );
	m_pOKButton = new CButton( this );
	m_pRefreshButton = new CButton( this );
	m_pMoveToVillageButton = new CButton( this );
	m_pDetailInfoButton = new CButton( this );
	m_pSendWhisperButton = new CButton( this );
	m_pNameStatic = new CStatic( this );

	file.CreateControl( m_pChatEdit,			NRESIDENTS_DIALOG_CHATEDIT,				TEXT("chatedit_ResidentsChat") );
	file.CreateControl( m_pInputEdit,			NRESIDENTS_DIALOG_EDIT_INPUT,			TEXT("editbox_InputChatData") );
	file.CreateControl( m_pList,				NRESIDENTS_DIALOG_LIST,					TEXT("listbox_ResidentsList") );
	file.CreateControl( m_pComboBox,			NRESIDENTS_DIALOG_COMBOBOX,				TEXT("combobox_ResidnetsChat") );
	file.CreateControl( m_pOKButton,			NRESIDENTS_DIALOG_BUTTON_OK,			TEXT("button_Close") );
	file.CreateControl( m_pRefreshButton,		NRESIDENTS_DIALOG_BUTTON_REFRESH,		TEXT("button_ResidentsListRefresh") );
	file.CreateControl( m_pMoveToVillageButton, NRESIDENTS_DIALOG_BUTTON_MOVETOVILLAGE,	TEXT("button_MoveToVillage") );
	file.CreateControl( m_pDetailInfoButton,	NRESIDENTS_DIALOG_BUTTON_DETAILINFO,	TEXT("button_DetailInfo") );
	file.CreateControl( m_pSendWhisperButton,	NRESIDENTS_DIALOG_BUTTON_SENDWHISPER,	TEXT("button_SendWhisper") );
	file.CreateControl( m_pNameStatic,			NRESIDENTS_DIALOG_STATIC_NAME,			TEXT("statictext_ResidentsName") );
	
	m_pList->SetColumn( 0, 100, GetTxtFromMgr(1984) );
	m_pList->SetColumn( 1, 38, GetTxtFromMgr(1506) );
	m_pList->SetColumn( 2, 100, GetTxtFromMgr(2447) );
	m_pList->SetColumn( 3, 50, GetTxtFromMgr(8353) );
	
	m_pList->SetBorder( true );
	m_pList->SetBKColorUse( true );

	m_pInputEdit->SetAutoScroll( true );
	
	TCHAR *pText = GetTxtFromMgr(4602);
	
#ifdef _NEW_INTERFACE_
	SetTitle(pText,CONTROLLER_TITLE_COLOR,BASE_FONT_HEIGHT,35,7);
	m_pCloseButton->Move(0,7);
#endif


	stComboBoxItemData ComboBoxItemData;
	
	ComboBoxItemData.Set(GetTxtFromMgr(1216));
	m_pComboBox->AddItem( &ComboBoxItemData );
	
	ComboBoxItemData.Set(GetTxtFromMgr(1218));
	m_pComboBox->AddItem( &ComboBoxItemData );

	m_pComboBox->SetCursel( 0 );
	
	Hide();

	return;
}

void CALLBACK CNResidentsDlg::StaticCallBackDialogNResidents( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNResidentsDlg *pThis = (CNResidentsDlg*)pControllerMgr;
	pThis->CallBackDialogNResidents( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNResidentsDlg::CallBackDialogNResidents( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;
	case NRESIDENTS_DIALOG_EDIT_INPUT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					cltClient *pclclient = (cltClient *)pclClient;
					
					TCHAR *pChatData = m_pInputEdit->GetText();
					if ( pChatData == NULL || _tcslen( pChatData ) < 1 )
					{
						break;
					}
					
					SI32 SelectedIndex = m_pComboBox->GetSelectedIndex();
					switch( SelectedIndex )
					{	
					case 0:	// 주민 채팅
						{
							if ( pclclient->m_pDialog[ NCHATBAR_DLG] )
							{
								(( CNChatBarDlg *)pclclient->m_pDialog[ NCHATBAR_DLG ] )->Push( RESIDENTS_CHAT_DATA_SEND, pChatData );
							}
							
						}
						break;
					case 1:	// 주민 공지
						{
							if ( pclclient->m_pDialog[ NCHATBAR_DLG ] )
							{	
								(( CNChatBarDlg *)pclclient->m_pDialog[ NCHATBAR_DLG ] )->Push( RESIDENTSNOTIFY_CHAT_DATA_SEND, pChatData );
							}
						}
						break;
					}
				}
				break;
			}
		}
		break;
	case NRESIDENTS_DIALOG_BUTTON_OK:
		{
			Hide();
		}
		break;
	case NRESIDENTS_DIALOG_BUTTON_REFRESH:
		{
			cltGameMsgRequest_ResidentsList clResidentsList( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage );	
			cltMsg clMsg( GAMEMSG_REQUEST_RESIDENTSLIST, sizeof(clResidentsList), (BYTE*)&clResidentsList );
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;
	case NRESIDENTS_DIALOG_BUTTON_MOVETOVILLAGE:
		{
			cltClient *pclclient = (cltClient *)pclClient;
			// 주민이 아니다.
			if ( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage < 1 )
			{
				TCHAR* pTitle = GetTxtFromMgr(2451);
				TCHAR* pText = GetTxtFromMgr(2452);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}

			// 현재 마을안에 있다.
			if ( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage == pclClient->pclCM->CR[1]->GetCurrentVillageUnique() )
			{
				TCHAR* pTitle = GetTxtFromMgr(2453);
				TCHAR* pText = GetTxtFromMgr(2454);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}

			TCHAR *HomeVillageName = pclClient->pclVillageManager->GetName(pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage);
			if ( HomeVillageName == NULL )
			{
				return;
			}
		
			SI32 HomeVillageIndex = pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage;

			TCHAR* pText = NULL;
			TCHAR* pTitle = NULL;
			TCHAR buffer[256];

			cltCharClient* pclcharclient = (cltCharClient*)pclclient->pclCM->CR[1];

			// 워프 할 수 있는가?
			if( pclcharclient->CanWarpFrom() == false )
				return;

			// 공짜 워프 체크
			if( pclcharclient->CanFreeWarp( HomeVillageIndex ) == false )
			{
				SI32 itemunique = ITEMUNIQUE(24000) ;  // 순간이동이용권
				SI32 pos = pclclient->pclCM->CR[1]->pclCI->clCharItem.FindItemInv(itemunique);
				if(pos < 0)
				{
					pTitle = GetTxtFromMgr(1413);
					pText  = GetTxtFromMgr(1414);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

					return ;
				}
				pTitle = GetTxtFromMgr(1418);
				pText = GetTxtFromMgr(1417);
				StringCchPrintf(buffer, 256, pText, HomeVillageName);
			}
			else
			{
				pTitle = GetTxtFromMgr(3189);
				pText = GetTxtFromMgr(1893);
				StringCchPrintf(buffer, 256, pText, HomeVillageName);
			}
			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 0 );
			BYTE TempBuffer[ 64 ];
			SI16 Size = sizeof( SI32 );
			memcpy( TempBuffer, &HomeVillageIndex, Size );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
		break;
	case NRESIDENTS_DIALOG_BUTTON_DETAILINFO:
		{
			if ( m_pList->GetSelectedIndex() > -1 )
			{	
				if ( m_pVillageResidentsList[ m_pList->GetSelectedIndex() ].clPerson.siPersonID > 0 )
				{
					cltGameMsgRequest_PersonPrivateInfoByPersonID clinfo( m_pVillageResidentsList[ m_pList->GetSelectedIndex() ].clPerson.siPersonID );
					cltMsg clMsg( GAMEMSG_REQUEST_PERSONPRIVATEINFOBYPERSONID, sizeof(clinfo), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg );
				}
			}
		}
		break;
	case NRESIDENTS_DIALOG_BUTTON_SENDWHISPER:
		{
			if ( m_pList->GetSelectedIndex() > -1 )
			{	
				if ( m_pVillageResidentsList[ m_pList->GetSelectedIndex() ].clPerson.siPersonID > 0 )
				{
					cltClient *pclclient = (cltClient *)pclClient;

					if ( pclClient->pclCM->CR[1]->pclCI->GetPersonID() == m_pVillageResidentsList[ m_pList->GetSelectedIndex() ].clPerson.siPersonID )
					{
						TCHAR* pTitle = GetTxtFromMgr(2449);
						TCHAR* pText = GetTxtFromMgr(2450);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}					
					pclclient->RequestSecretChatByPersonID( m_pVillageResidentsList[ m_pList->GetSelectedIndex() ].clPerson.siPersonID );
				}
			}
		}
		break;
	case NRESIDENTS_DIALOG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI16 SelectedIndex = m_pList->GetSelectedIndex();
					if ( SelectedIndex > -1 )
					{
						stListBoxItemData *pListBoxItemData = m_pList->GetItemData( SelectedIndex , 0 );
						if ( pListBoxItemData )
						{
							if ( pListBoxItemData->strText[0] )
							{
								m_pNameStatic->SetText( pListBoxItemData->strText[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE );
							}
						}
					}
				}
				break;
			}
		}
		break;
	}
	return;
}

void CNResidentsDlg::Show()
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage < 1 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2457);
		TCHAR* pText = GetTxtFromMgr(2452);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return;
	}

	if ( IsShow() )
	{
		Hide();
		return;
	}

	cltGameMsgRequest_ResidentsList clResidentsList( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage );	
	cltMsg clMsg( GAMEMSG_REQUEST_RESIDENTSLIST, sizeof(clResidentsList), (BYTE*)&clResidentsList );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	CControllerMgr::Show( true );

	return;
}

void CNResidentsDlg::Hide()
{
	CControllerMgr::Show( false );

	m_pList->Clear();
	SAFE_DELETE_A( m_pVillageResidentsList );

	return;
}

void CNResidentsDlg::RecvResidentsList( cltVillageResidentsList *pclPersonList, UI32 ListCount )
{
	m_pNameStatic->SetText( TEXT(""), DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	m_pList->Clear();

	SAFE_DELETE_A( m_pVillageResidentsList );

	m_pVillageResidentsList = new cltVillageResidentsList[ ListCount ];
	memcpy( m_pVillageResidentsList, pclPersonList, ListCount * sizeof( cltVillageResidentsList ) );

	stListBoxItemData ListBoxItemData;

	SI32 Count = 0;

	TCHAR strLevel[ 8 ];
	TCHAR strMilitaryMerit[ 8 ];
	TCHAR strRank[ 128 ];
	TCHAR VillageRank[ 128 ];

	for ( UI32 i = 0; i < ListCount; ++i )
	{
		if ( m_pVillageResidentsList[ i ].clPerson.siPersonID > 0 )
		{
			ListBoxItemData.Set( 0, m_pVillageResidentsList[ i ].clPerson.szName );
			StringCchPrintf( strLevel, 8, TEXT("%d"), m_pVillageResidentsList[ i ].siLevel );
			ListBoxItemData.Set( 1, strLevel );
			
			if ( m_pVillageResidentsList[ i ].clSimpleRank.GetVillageUnique() > 0 )
			{
				if ( GetRankName( &m_pVillageResidentsList[ i ].clSimpleRank, VillageRank, 128 ) == TRUE )
				{
					StringCchCopy( strRank, 128, VillageRank );
				}
				else
				{
					TCHAR* pText = GetTxtFromMgr(1216);
					StringCchCopy( strRank, 128, pText );
				}
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(1216);
				StringCchCopy( strRank, 128, pText );
			}
			ListBoxItemData.Set( 2, strRank );
			
			StringCchPrintf( strMilitaryMerit, 8, TEXT("%d"), m_pVillageResidentsList[ i ].siMilitaryMerit );
			ListBoxItemData.Set( 3, strMilitaryMerit );
			
			m_pList->AddItem( &ListBoxItemData );
		}
	}

	m_pList->Refresh();

	return;
}

bool CNResidentsDlg::CheckData( TCHAR *PlayerName, TCHAR *ChatText )
{
	if ( PlayerName == NULL || ChatText == NULL )
	{
		return false;
	}

	if ( _tcslen( PlayerName ) > CHATEDIT_NAMEDATA_LENGTH - 1 || _tcslen( ChatText ) > MAX_CHAT_DATA_LENGTH - 1 )
	{
		return false;
	}

	return true;
}

void CNResidentsDlg::ResidentsChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{	
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	TCHAR buf[256]=TEXT(" ");
	//cyj "마을:%s"을 텍스트 메니저로 뺌
	TCHAR* pText = GetTxtFromMgr(5734);
	StringCchPrintf(buf, 256, pText, PlayerName);
	//strcpy(PlayerName,buf);

	stChatEditData ChatEditData;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_GREEN;

	//MStrCpy( ChatEditData.strNameData, PlayerName, CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strNameData, buf, CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	ChatEditData.siChatKind = MSG_RESIDENTSCHAT_RECV;
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	InsertChatEditData( &ChatEditData );

	cltClient *pclclient = (cltClient *)pclClient;
	if ( ! pclclient->m_pDialog[ NRESIDENTS_DLG ]->IsShow() )
	{	
		TCHAR szVillageChatMsg[ MAX_CHAT_DATA_LENGTH + CHATEDIT_NAMEDATA_LENGTH ] = TEXT("");	// 20 은 기타 표시용 [] :  뭐 요런거랑 공백
		StringCchPrintf( szVillageChatMsg, MAX_CHAT_DATA_LENGTH + CHATEDIT_NAMEDATA_LENGTH, TEXT("[%s]:%s"), PlayerName, ChatText  );
		pclclient->pclMessage->SetVillageChatMsg( szVillageChatMsg );
	}

	return;
}

void CNResidentsDlg::InsertChatEditData( stChatEditData *pChatEditData )
{
	if ( pChatEditData == NULL )
	{
		return;
	}

	pChatEditData->siChatDataColorIndex = CHATEDIT_COLOR_BLACK;
	pChatEditData->siNameDataColorIndex = CHATEDIT_COLOR_BLUE;

	m_pChatEdit->Insert( pChatEditData );

	return;
}

void CNResidentsDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			cltCharClient* pclcharclient = (cltCharClient*)pclclient->pclCM->CR[1];
			if( RetValue )
			{
				SI32 siCurrentVillage = pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage;

				if( pclcharclient->CanFreeWarp( siCurrentVillage ) == false )
				{
					pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FANPRIEST_WARP")), 0, 0 );
					cltMsg clMsg(GAMEMSG_REQUEST_RESIDENTWARPVILLAGE, sizeof(SI32), (BYTE*)pData );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					break;
				}
				else
				{
					//KHY - 0121- 순간이동 이용권 , 정기권 사용자에대한 무한 사용가능.
					cltGameMsgRequest_HuntMapWarp pclInfo( siCurrentVillage, 0 );
					cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (BYTE*)&pclInfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					break;
				}
			}
		}
		break;
	}
}