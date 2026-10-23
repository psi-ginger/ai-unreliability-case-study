#include "../client/NInterface/NPartyDlg/NPartyDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ChatEdit.h"
#include "../../InterfaceMgr/Interface/RadioButton.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "Char\CharManager\CharManager.h"
#include "..\..\..\CommonLogic\Msg\MsgType-Party.h"
#include "..\..\..\Common\SMLib\LibList\LibList.h"
#include "../../../common/SMLib/LibList/LibListMgr.h"

#include "../../ninterface/nprivatechatdlg/NPrivateChatDlgMgr.h"
#include "../../ninterface/nprivatechatdlg/NPrivateChatDlg.h"

#include "../../ninterface/nletterdlg/NLetterDlg.h"

#include "../../CommonLogic/Msg/MsgType-ItemMall.h"

#include "../../common/Item/ItemCommon/ItemUnit.h"

#include "../NHelpButton/NHelpButtonDlg.h"

#include "../NMarriageLoveLevelDlg/NMarriageLoveLevelDlg.h"
#include "../NFamilyCookieDlg/NFamilyCookieDlg.h"
#include "../NFamilyInfoDlg/NFamilyInfoDlg.h"
#include "../NDormancyAccount/NDormancyPointDlg.h"

#include "../../../CommonLogic/MsgType-PartyQuest.h"

#include "../../../client/PartyQuestMgr/PartyQuestMgr_Client.h"
#include "../../../common/PartyMgr/PartyMgr.h"
#include "../../../CommonLogic/PartyMatchingMgr/Client_PartyMatchingMgr.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;

CNPartyDlg::CNPartyDlg() :CDialogController()
{
	m_bMini = FALSE;
	m_siOldPosX = 0;
	m_siOldPosY = 0;
	m_siPartyUserNum = 0;
	m_siMinimizedSlot = 0;
	m_strViewData = new TCHAR[ MAX_RICHEDIT_OBJ_SIZE ];
	memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );

	m_strChatData = new TCHAR[ MAX_CHAT_DATA_LENGTH ];
	memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );

	m_siSelectedUserListIndex = -1;

	m_pChatDataList = new CLibListMgr();
	m_pChatDataList->CreateMaxEmptyObj( 2, 200 );

	Initialize();

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pList = NULL;
	m_pButton = NULL;
	m_pMoveButton = NULL;
	m_pEdit = NULL;
	m_pChatEdit = NULL;
	m_pRadioItemGetPattern[0] = NULL;
	m_pRadioItemGetPattern[1] = NULL;
	m_pRadioItemGetPattern[2] = NULL;
	m_pRadioItemGetPattern[3] = NULL;
	m_pImageMinimized = NULL;
	m_pButtonMinimize = NULL;
	m_pOutLine[0] = NULL;
	m_pOutLine[1] = NULL;
	
	ColorIndex[0] =RGB(210,15,15);
	ColorIndex[1] = RGB(255,100,10);
	ColorIndex[2] = RGB(40,150,0);
	ColorIndex[3] = RGB(0,200,210);
	ColorIndex[4] = RGB(30,30,240);
	ColorIndex[5] = RGB(130,20,240);

	m_pBtnStartQuest = NULL;
	m_siInviteCharUniqueForPartyQuest = 0;

	m_bIsPartyQuestParty = false;
	m_siCategory = -1;
	m_siMission = -1;
	
}

CNPartyDlg::~CNPartyDlg()
{
	if( m_strViewData )
	{
		delete [] m_strViewData;
		m_strViewData = NULL;
	}

	if ( m_strChatData )
	{
		delete [] m_strChatData;
		m_strChatData = NULL;
	}
	SAFE_DELETE(m_pOutLine[0]);	
	SAFE_DELETE(m_pOutLine[1]);
	SAFE_DELETE(m_pButtonMinimize);
	SAFE_DELETE(m_pImageMinimized);

	SAFE_DELETE(m_pChatDataList);

	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pList);
	SAFE_DELETE(m_pButton);
	SAFE_DELETE(m_pMoveButton);
	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pChatEdit);
	SAFE_DELETE(m_pRadioItemGetPattern[3]);
	SAFE_DELETE(m_pRadioItemGetPattern[2]);
	SAFE_DELETE(m_pRadioItemGetPattern[1]);
	SAFE_DELETE(m_pRadioItemGetPattern[0]);
	SAFE_DELETE(m_pBtnStartQuest);
}

void CNPartyDlg::Initialize()
{
	m_bIsPartyLeader = false;
	m_bPartyStatus = false;
	m_siMinimizedSlot = 0;
	m_siItemGetPattern = ITEM_GETPATTERN_ROTATION;

	//	m_bShow = false;

	for ( int i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		m_stClientPartyUser[ i ].Init();
	}

	return;
}

void CNPartyDlg::SendPartyChat()
{
	CObj *pTemp = m_pChatDataList->PopFrontToEmptyListAndPushBackToExistList();
	if ( pTemp == NULL )
	{
		return;
	}

#ifdef _IAF_EXPORT
	if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
	{
		TCHAR ResultBuf[MAX_CHAT_DATA_LENGTH] = "";
		IAF::Filtering( m_strChatData, ResultBuf, sizeof(ResultBuf) );
		memcpy( m_strChatData, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
	}
#endif

	stPartyChatSend PartyChatSend( m_strChatData ) ;
	pTemp->SetData( (BYTE*)&PartyChatSend, sizeof( stPartyChatSend ) );
}

void CNPartyDlg::AddChat( TCHAR *strUserID, TCHAR *strChatData)
{
//	SI32 PreLineCount = 0, NextLineCount = 0;
//	PreLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

	stChatEditData ChatEditData;

	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_BLACK;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_BLUE;

	StringCchCopy( m_strViewData, MAX_RICHEDIT_OBJ_SIZE, TEXT("") );	
	//	strcat( m_strViewData, TEXT("[") );
	//	strcat( m_strViewData, strUserID );
	//	strcat( m_strViewData, TEXT("] : ") );
	StringCchCopy( ChatEditData.strNameData, CHATEDIT_NAMEDATA_LENGTH, strUserID );
	//	AddName( m_strViewData, NameColor );

	StringCchCopy( m_strViewData, MAX_RICHEDIT_OBJ_SIZE, TEXT("") );
	StringCchCat( m_strViewData, MAX_RICHEDIT_OBJ_SIZE, strChatData );
//KHY - 0625 - 파티채팅 버그수정.
//	StringCchCat( m_strViewData, MAX_RICHEDIT_OBJ_SIZE, TEXT("\r\n") );

	StringCchCopy( ChatEditData.strChatData, CHATEDIT_CHATDATA_LENGTH, m_strViewData );
	m_pChatEdit->Insert(&ChatEditData);
	//	AddText( m_strViewData, TextColor );

	//	NextLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

	//	SendMessage( m_hRichEdit, EM_LINESCROLL, 0, NextLineCount - PreLineCount );
	return;
/*
	_tcscpy( m_strViewData, TEXT("") );	
	strcat( m_strViewData, TEXT("[") );
	strcat( m_strViewData, strUserID );
	strcat( m_strViewData, TEXT("] : ") );
	AddName( m_strViewData, NameColor );

	_tcscpy( m_strViewData, TEXT("") );
	strcat( m_strViewData, strChatData );
	strcat( m_strViewData, TEXT("\r\n") );
	AddText( m_strViewData, TextColor );
*/	
//	NextLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

//	SendMessage( m_hRichEdit, EM_LINESCROLL, 0, NextLineCount - PreLineCount );
	return;
}

void CNPartyDlg::AddChat( TCHAR *strChatData)
{
//	SI32 PreLineCount = 0, NextLineCount = 0;
//	PreLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

	stChatEditData ChatEditData;

	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_BLACK;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_BLUE;

	StringCchCopy( m_strViewData, MAX_RICHEDIT_OBJ_SIZE, TEXT("") );
	StringCchCat( m_strViewData, MAX_RICHEDIT_OBJ_SIZE, strChatData );
//KHY - 0625 - 파티채팅 버그수정.
//	StringCchCat( m_strViewData, MAX_RICHEDIT_OBJ_SIZE, TEXT("\r\n") );

	StringCchCopy( ChatEditData.strChatData, CHATEDIT_CHATDATA_LENGTH, m_strViewData );
	m_pChatEdit->Insert(&ChatEditData);
//	AddText( m_strViewData, TextColor );

//	NextLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

//	SendMessage( m_hRichEdit, EM_LINESCROLL, 0, NextLineCount - PreLineCount );
	return;
}

void CNPartyDlg::AddName( TCHAR *PlayerName)
{	
/*	SetFont( Color );

	int iTotalTextLength = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL, iTotalTextLength, iTotalTextLength );
	int iStartPos = iTotalTextLength;
	SendMessage( m_hRichEdit, EM_REPLACESEL, 0, (LPARAM)PlayerName );
	
	int iEndPos = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL, iStartPos, iEndPos );

	SendMessage( m_hRichEdit, EM_HIDESELECTION, 1, 0 );
	*/
//	m_pEdit[0]->Insert() >SetText(PlayerName);
	return;
}

void CNPartyDlg::AddText( TCHAR *ChatText)
{	
/*	SetFont( Color );

	int iTotalTextLength = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL, iTotalTextLength, iTotalTextLength );
	int iStartPos = iTotalTextLength;	
	SendMessage( m_hRichEdit, EM_REPLACESEL, 0, (LPARAM)ChatText );	
		
	int iEndPos = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL, iStartPos, iEndPos );
	
	SendMessage( m_hRichEdit, EM_HIDESELECTION, 1, 0 );	
*/
//	m_pEdit[0]->SetText(ChatText);
	return;
}


bool CNPartyDlg::IsPartyStatus()
{
	return m_bPartyStatus;
}

bool CNPartyDlg::IsPartyLeader()
{
	return m_bIsPartyLeader;
}

bool CNPartyDlg::IsPartyUser( SI32 CharUnique )
{
	if ( m_bPartyStatus )
	{
		for ( int i = 0; i < MAX_PARTY_USER_NUM; ++i )
		{
			if ( m_stClientPartyUser[ i ].bPartyStatus && m_stClientPartyUser[ i ].siCharUnique == CharUnique )
			{
				return true;
			}
		}
	}

	return false;
}

SI32 CNPartyDlg::GetPartyIndex()
{
	return m_siPartyIndex;
}

/*
// 파티징으로서 파티에 참가
void CNPartyDlg::PartyCreate( SI32 PartyIndex )
{
	m_siSelectedUserListIndex = -1;
	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LB_RESETCONTENT, 0, 0 );

	m_siPartyIndex = PartyIndex;

	m_bPartyStatus = true;
	m_bIsPartyLeader = true;

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_PARTY_EXPLAIN ), TEXT("파티를 만드셨습니다."));

//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LB_ADDSTRING, 0, (LPARAM)pclClient->pclCM->CR[1]->GetName() );
	stListBoxItemData Data;
	Data.Set(0, pclClient->pclCM->CR[1]->GetName());
	m_pList->AddItem(&Data);
	m_pList->Refresh();


	m_siPartyUserNum = 1;
	m_stClientPartyUser[ 0 ].bPartyStatus = true;
	m_stClientPartyUser[ 0 ].bIsPartyLeader = true;
	m_stClientPartyUser[ 0 ].siCharUnique = pclClient->pclCM->CR[1]->GetCharUnique();
	_tcscpy ( m_stClientPartyUser[ 0 ].strCharName, pclClient->pclCM->CR[1]->GetName() );

	Show();

	PrintUserNum();

	return;
}
*/

void CNPartyDlg::PartyCreateJoin( SI32 PartyIndex, SI32 LeaderPersonID, SI32 LeaderCharUnique, TCHAR *strLeaderChatName, SI32 LeaderLevel, cltCharPos* LeaderCharPos , SI32 MemberPersonID, SI32 MemberCharUnique, TCHAR *strMemberCharName, SI32 MemberLevel, cltCharPos* MemberCharPos )
{
	m_siSelectedUserListIndex = -1;
//	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );
	m_pList->Clear();

	m_siPartyIndex = PartyIndex;

	m_bPartyStatus = true;

	if ( pclClient->pclCM->CR[1]->pclCI->GetPersonID() == LeaderPersonID )
	{
		m_bIsPartyLeader = true;
		ItemGetPatternButtonEnable(true);
		TCHAR* pText = GetTxtFromMgr(1990);
		AddChat( pText);
	}
	else
	{
		m_bIsPartyLeader = false;
		//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), TEXT("파티에 참가하셨습니다. 파티원 추가 1명당 10%의 경험치를 더 얻을 수 있습니다."));
		TCHAR* pText = GetTxtFromMgr(1991);
		AddChat(pText);
	}

	m_stClientPartyUser[ 0 ].bPartyStatus = true;
	m_stClientPartyUser[ 0 ].bIsPartyLeader = true;
	m_stClientPartyUser[ 0 ].siCharUnique = LeaderCharUnique;
	m_stClientPartyUser[ 0 ].siLevel = LeaderLevel;
	StringCchCopy( m_stClientPartyUser[ 0 ].strCharName, MAX_PLAYER_NAME, strLeaderChatName );
	m_stClientPartyUser[ 0 ].clCharPos.Set(LeaderCharPos);

	m_stClientPartyUser[ 1 ].bPartyStatus = true;
	m_stClientPartyUser[ 1 ].bIsPartyLeader = false;
	m_stClientPartyUser[ 1 ].siCharUnique = MemberCharUnique;
	m_stClientPartyUser[ 1 ].siLevel = MemberLevel;
	StringCchCopy( m_stClientPartyUser[ 1 ].strCharName, MAX_PLAYER_NAME, strMemberCharName );
	m_stClientPartyUser[ 1 ].clCharPos.Set(MemberCharPos);

	stListBoxItemData Data;

	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	

	Li.iItem = 0;
	Li.iSubItem = 0;
	Li.pszText = m_stClientPartyUser[ 0 ].strCharName;
//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_INSERTITEM, 0, (LPARAM)&Li );
	
	Data.Set(0, Li.pszText);
	
	TCHAR strLevel1[32] = TEXT("");
	TCHAR strLevel2[32] = TEXT("");

	Li.iSubItem = 1;
	StringCchPrintf( strLevel1, 32, TEXT("%d"), m_stClientPartyUser[0].siLevel );
	Li.pszText = strLevel1;
//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );

	Data.Set(1, Li.pszText);

	Li.iSubItem = 2;
	Li.pszText = TEXT("0");
//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );
	Data.Set(2, Li.pszText);
	

	// 위치의 기본값은 맵인덱스 0(야외지역) 의 이름이다.
	// 위치알림막음

	//내가 리더이면 첫번째 행의 위치 열에 내 맵 인덱스의 이름을 넣어준다.

	Li.iSubItem = 3; 
	Li.pszText = pclClient->pclMapManager->GetMapName( LeaderCharPos->GetMapIndex() );
	Data.Set(3, Li.pszText)	;
	
	Data.ColumnColor[0] = ColorIndex[0];
	Data.ColumnColor[1] = ColorIndex[0];
	Data.ColumnColor[2] = ColorIndex[0];
	Data.ColumnColor[3] = ColorIndex[0];
	
	m_pList->AddItem(&Data);

	Li.iItem = 1;
	Li.iSubItem = 0;
	Li.pszText = m_stClientPartyUser[ 1 ].strCharName;
	
//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_INSERTITEM, 0, (LPARAM)&Li );
	Data.Set(0, Li.pszText);

	Li.iSubItem = 1;
	StringCchPrintf( strLevel2, 32, TEXT("%d"), m_stClientPartyUser[1].siLevel );
	Li.pszText = strLevel2;
//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );

	Data.Set(1, Li.pszText);


	Li.iSubItem = 2;
	Li.pszText = TEXT("0");
//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );

	Data.Set(2, Li.pszText);
	// 위치알림막음

	//내가 멤버라면 두번째 행의 위치 열에 맵이름을 넣어준다.
	Li.iSubItem = 3; 
	Li.pszText = pclClient->pclMapManager->GetMapName( MemberCharPos->GetMapIndex() );
	Data.Set(3, Li.pszText)	;
	
	Data.ColumnColor[0] = ColorIndex[1];
	Data.ColumnColor[1] = ColorIndex[1];
	Data.ColumnColor[2] = ColorIndex[1];
	Data.ColumnColor[3] = ColorIndex[1];

	m_pList->AddItem(&Data);

	m_pList->Refresh();

	m_siPartyUserNum = 2;
	PrintUserNum();

	pclClient->pclCM->CR[1]->siPartyIndex = PartyIndex;

	Show();

	return;
}

void CNPartyDlg::PartyJoin( SI32 PartyIndex, SI16 UserListIndex ,SI16 ItemGetPattern )
{
	m_siSelectedUserListIndex = -1;
	m_pList->Clear();
//	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );

	m_siPartyIndex = PartyIndex;

	m_bPartyStatus = true;
	m_bIsPartyLeader = false;

	m_siPartyUserNum = 0;
	//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), TEXT("파티에 참가 되셨습니다. 파티원 추가 1명당 5%의 경험치를 더 얻을 수 있습니다.") );
	TCHAR* pText = GetTxtFromMgr(1992);
	AddChat( pText);

	cltGameMsgRequest_Party_AllUserList clPartyAllUserList( m_siPartyIndex );
	cltMsg clMsg(GAMEMSG_REQUEST_PARTY_ALLUSERLIST, sizeof(clPartyAllUserList), (BYTE*)&clPartyAllUserList);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	pclClient->pclCM->CR[1]->siPartyIndex = PartyIndex;
	
	ChangeItemGetPattern(ItemGetPattern);

	Show();

	return;
}

void CNPartyDlg::PartyDelete( SI32 PartyIndex )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_siPartyIndex == PartyIndex )
	{
		TCHAR* pTitle	= GetTxtFromMgr(1993);
		TCHAR* pText	= GetTxtFromMgr(1994);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		pclclient->pclCM->CR[1]->siPartyIndex = 0;
		Hide();

		// [영훈] 해적선 : 해적선 내에서 파티가 해제되었을 경우 엔펠리스로 이동시킨다
		SI32 siMapIndex		= pclclient->pclCM->CR[1]->GetMapIndex();

		if ( pclclient->pclMapManager->IsPartyMap( siMapIndex ) == true )
		{
			cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);
			pclclient->pclCM->CR[1]->SetWarpInfo(&clWarpInfo, 56);
			pclclient->pclCM->CR[1]->Warp();
		}

	}

	return;
}

void CNPartyDlg::PartyAddUser( bool PartyLeader, SI32 CharUnique, TCHAR *CharName, SI16 UserListIndex, SI32 Level , cltCharPos* pclCharPos)
{
	m_stClientPartyUser[ UserListIndex ].bPartyStatus = true;
	m_stClientPartyUser[ UserListIndex ].bIsPartyLeader = PartyLeader;
	m_stClientPartyUser[ UserListIndex ].siCharUnique = CharUnique;
	StringCchCopy( m_stClientPartyUser[ UserListIndex ].strCharName, MAX_PLAYER_NAME, CharName );
	m_stClientPartyUser[ UserListIndex ].siLevel = Level;
	m_stClientPartyUser[ UserListIndex ].clCharPos.Set(pclCharPos);

	TCHAR Buffer[ 1024 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(1995);
	StringCchPrintf( Buffer, 1024, pText, CharName );
//	int len = sizeof(Buffer);
	//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), Buffer );
	AddChat( Buffer);

	++m_siPartyUserNum;

	ReloadPartyUserList();

	PrintUserNum();

	return;
}

void CNPartyDlg::PartyDelUser( SI32 CharUnique, SI16 UserListIndex )
{
	if ( m_stClientPartyUser[ UserListIndex ].siCharUnique == CharUnique )
	{
		if ( CharUnique == pclClient->pclCM->CR[1]->GetCharUnique() )
		{
			m_siSelectedUserListIndex = -1;
		//	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );
			m_pList->Clear();

			m_stClientPartyUser[ UserListIndex ].Init();

			--m_siPartyUserNum;

			Hide();
			return;
		}

		TCHAR Buffer[256 ] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(1996);
		StringCchPrintf( Buffer, 256, pText, m_stClientPartyUser[ UserListIndex ].strCharName );
		//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), Buffer );
		AddChat( Buffer);
		m_stClientPartyUser[ UserListIndex ].Init();

		--m_siPartyUserNum;

		ReloadPartyUserList();
	}

	if ( m_siPartyUserNum < 1 )
	{
		Hide();
	}

	PrintUserNum();

	return;
}

void CNPartyDlg::PartyForceDelUser( SI32 CharUnique, SI16 UserListIndex )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_stClientPartyUser[ UserListIndex ].siCharUnique == CharUnique )
	{
		if ( CharUnique == pclClient->pclCM->CR[1]->GetCharUnique() )
		{
			m_siSelectedUserListIndex = -1;
		//	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );
			m_pList->Clear();
			Hide();
			TCHAR* pTitle = GetTxtFromMgr(1997);
			TCHAR* pText = GetTxtFromMgr(1998);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}

		TCHAR Buffer[256 ] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(1999);
		StringCchPrintf( Buffer, 256, pText, m_stClientPartyUser[ UserListIndex ].strCharName );

		//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), Buffer );
		AddChat( Buffer);
		m_stClientPartyUser[ UserListIndex ].Init();
		--m_siPartyUserNum;

		ReloadPartyUserList();
	}

	PrintUserNum();

	return;
}

void CNPartyDlg::PartyChangeLeader( SI32 CharUnique, SI16 UserListIndex )
{
	if ( m_stClientPartyUser[ UserListIndex ].siCharUnique == CharUnique )
	{
		m_stClientPartyUser[ UserListIndex ].bIsPartyLeader = true;
		m_stClientPartyUser[ 0 ].Set( &m_stClientPartyUser[ UserListIndex ] );
		m_stClientPartyUser[ UserListIndex ].Init();

		// 이 클라이언트가 파티장이 된 경우
		if ( m_stClientPartyUser[0].siCharUnique == pclClient->pclCM->CR[1]->GetCharUnique() )
		{
			m_bIsPartyLeader = true; 
			ItemGetPatternButtonEnable(true);
		}

		ReloadPartyUserList();
	}

	return;
}

void CNPartyDlg::PartyNotifyExp( SI32 CharUnique, SI16 UserListIndex, SI32 Exp )
{
	if ( m_stClientPartyUser[ UserListIndex ].siCharUnique == CharUnique && Exp > 0 )
	{
		m_stClientPartyUser[ UserListIndex ].siExp = Exp;
		ReloadPartyUserList();
	}

	return;
}

SI32 CNPartyDlg::FindUserSelectedIndex( TCHAR *CharName )
{
	for ( int i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stClientPartyUser[ i ].bPartyStatus )
		{
			if ( _tcscmp( m_stClientPartyUser[ i ].strCharName, CharName ) == 0 )
			{
				return i;
			}
		}
	}

	return -1;
}

void CNPartyDlg::PrintUserNum()
{
	TCHAR Buffer[256] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(2000);
	StringCchPrintf( Buffer, 256, pText, m_siPartyUserNum, MAX_PARTY_USER_NUM );

//	SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_PARTY_USER_NUM ), Buffer );
	m_pStatic[0]->SetText(Buffer, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	return;
}

void CNPartyDlg::ReloadPartyUserList()
{
	m_siSelectedUserListIndex = -1;
	m_pList->Clear();
//	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );
	
	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	

	Li.iItem = 0;	
	Li.iSubItem = 0;

	SI32 Count = 0;
	TCHAR strExp[32];
	TCHAR strLevel[32];
	stListBoxItemData Data;
	for ( int i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stClientPartyUser[ i ].bPartyStatus )
		{
			Li.iItem = Count;
			Li.iSubItem = 0;			
			Li.pszText = m_stClientPartyUser[ i ].strCharName;
			Data.Set(0, Li.pszText);
		//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_INSERTITEM, 0, (LPARAM)&Li );

			Li.iSubItem = 1;
			StringCchPrintf( strLevel, 32, TEXT("%d"), m_stClientPartyUser[ i ].siLevel );
			Li.pszText = strLevel;
			Data.Set(1, Li.pszText);
	
			Li.iSubItem = 2;
			StringCchPrintf( strExp, 32, TEXT("%d"), m_stClientPartyUser[ i ].siExp );
			Li.pszText = strExp;
			Data.Set(2, Li.pszText);
	

			Li.iSubItem = 3; 

			if(m_stClientPartyUser[ i ].siCharUnique == pclClient->pclCM->CR[1]->GetCharUnique())
				Li.pszText = pclClient->pclMapManager->GetMapName( pclClient->pclCM->CR[1]->GetMapIndex() );
			else
				Li.pszText = pclClient->pclMapManager->GetMapName( m_stClientPartyUser[i].clCharPos.GetMapIndex() );
				
			Data.Set(3, Li.pszText)	;
			
			Data.ColumnColor[0] = ColorIndex[Count];
			Data.ColumnColor[1] = ColorIndex[Count];
			Data.ColumnColor[2] = ColorIndex[Count];
			Data.ColumnColor[3] = ColorIndex[Count];
	
			m_pList->AddItem(&Data);
			

			Count++;
		}
	}
	m_pList->Refresh();

	return;
}

bool CNPartyDlg::PopPartyChatSendData( stPartyChatSend *pPartyChatSend )
{
	CObj *pFirst = m_pChatDataList->GetExistList()->GetFirst();

	if ( pFirst == NULL )
	{
		return false;
	}

	UI32 uiSize;
	pFirst->GetData( (BYTE*)pPartyChatSend, &uiSize );

	m_pChatDataList->GetExistList()->Pop( pFirst );
	m_pChatDataList->PushBackToEmptyList( pFirst );

	return true;
}

void CNPartyDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/DLG_NParty.ddf"));
	file.CreatePopupDialog( this, NPARTY_DLG, TEXT("dialog_Party"), StaticCallBackDialogNPartyDlg );

	if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
	{
		MovePos( 620, 220 );		
	}	
	else
	{
		MovePos( 554, 66 );
	}
	
	m_pOutLine[0] = new COutLine(this);
	m_pOutLine[1] = new COutLine(this);
	m_pButtonMinimize = new CButton(this);
	m_pImageMinimized = new CImageStatic(this);

	m_pRadioItemGetPattern[0] = new CRadioButton(this);
	m_pRadioItemGetPattern[1] = new CRadioButton(this);
	m_pRadioItemGetPattern[2] = new CRadioButton(this);
	m_pRadioItemGetPattern[3] = new CRadioButton(this);

	m_pStatic[0] = new CStatic( this );
	m_pStatic[1] = new CStatic( this );
	m_pStatic[2] = new CStatic( this );
	m_pList = new CList( this );
	m_pButton = new CButton( this );
	NEWCBUTTON(m_pMoveButton);
	m_pEdit = new CEdit( this );

	m_pChatEdit = new CChatEdit( this );
	m_pBtnStartQuest = new CButton(this);
	
	
	file.CreateControl(m_pButtonMinimize,NPRATY_DIALOG_BUTTON_MINIMIZE, TEXT("button_minimize"));
	file.CreateControl(m_pImageMinimized, NPARTY_DIALOG_IMAGESTATIC_MINIMIZED, TEXT("imagestatic_minimize"));
	file.CreateControl( m_pRadioItemGetPattern[0], NPARTY_DIALOG_RADIO1, TEXT("radiobutton_NONAME1"));
	file.CreateControl( m_pRadioItemGetPattern[1], NPARTY_DIALOG_RADIO2, TEXT("radiobutton_NONAME2"));
	file.CreateControl( m_pRadioItemGetPattern[2], NPARTY_DIALOG_RADIO3, TEXT("radiobutton_NONAME3"));
	file.CreateControl( m_pRadioItemGetPattern[3], NPARTY_DIALOG_RADIO4, TEXT("radiobutton_NONAME4"));

	file.CreateControl( m_pStatic[0], NPARTY_DIALOG_STATIC1, TEXT("statictext_Party_User_Num"));  
	file.CreateControl( m_pStatic[1], NPARTY_DIALOG_STATIC2, TEXT("statictext_NONAME1"));  // 그냥 파티 채팅 글자
	file.CreateControl( m_pStatic[2], NPARTY_DIALOG_STATIC3, TEXT("statictext_NONAME2")); // 그냥 획득 방식 글자
	file.CreateControl( m_pList,   NPARTY_DIALOG_LIST,   TEXT("listbox_Party_list"));  
	file.CreateControl( m_pButton, NPARTY_DIALOG_BUTTON, TEXT("button_Party_Leave"));  
	file.CreateControl( m_pMoveButton, NPARTY_DIALOG_MOVE_BUTTON, TEXT("button_NONAME1"));  
	file.CreateControl( m_pEdit, NPARTY_DIALOG_EDIT1, TEXT("editbox_NONAME1"));	// 파티말 쓰기 표시표시
	file.CreateControl( m_pChatEdit, NPARTY_DIALOG_CHATEDIT, TEXT("chatedit_NONAME1"));	// 파티말 쓰기 표시표시
	file.CreateControl(m_pOutLine[0],NPARTY_DIALOG_OUTLINE, TEXT("outline_NONAME1"));
	file.CreateControl(m_pOutLine[1],NPARTY_DIALOG_OUTLINE, TEXT("outline_NONAME2"));
	file.CreateControl(m_pBtnStartQuest,NPARTY_DIALOG_STARTQUEST, TEXT("button_Party_quest"));

	m_pChatEdit->SetBorder( true );
	m_pChatEdit->SetBKColorUse( true );
	m_pChatEdit->Show(TRUE);

	// 파티퀘스트 버튼은 안보이는 상태로 생성
	m_pBtnStartQuest->Show( false );
	m_pBtnStartQuest->Refresh();

	TCHAR* pText = GetTxtFromMgr(3413);
	m_pList->SetColumn(0, 100, pText);

	pText = GetTxtFromMgr(4008);
	m_pList->SetColumn(1, 30,  pText);

	pText = GetTxtFromMgr(1985);
	m_pList->SetColumn(2, 100, pText);

	pText = GetTxtFromMgr(1611);
	m_pList->SetColumn(3, 100, pText);

	m_pList->SetBorder(TRUE);
	m_pList->SetBKColorUse(TRUE);
	m_pList->Refresh();
	
	m_pRadioItemGetPattern[0]->SetNonEventCheck();
	
	m_pImageMinimized->Show(false);
	ItemGetPatternButtonEnable(false);
	CControllerMgr::Show( false );

	SetActionTimer( 200 );	
	SetTimerOnOff( true );


}


void CALLBACK CNPartyDlg::StaticCallBackDialogNPartyDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNPartyDlg *pThis = (CNPartyDlg *)pControllerMgr;
	pThis->CallBackDialogNPartyDlg( nEvent, nControlID, pControl );

}

void CALLBACK CNPartyDlg::CallBackDialogNPartyDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;


	switch ( nControlID )
	{
//	case COMMON_DIALOG_CLOSEBUTTON:
//		{
//			DeleteDialog();
//		}
//		break;
	//case COMMON_DIALOG_CLOSEBUTTON:		Minimi(TRUE);		break;

	case NPARTY_DIALOG_RADIO1:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pRadioItemGetPattern[0]->IsCheck())
					{   // 이미 설정되어 있는 상태거나, 파티의 리더가 아니라면, 버튼이 비활성화 되어있다면 메시지 안보냄
						if(m_siItemGetPattern == ITEM_GETPATTERN_ROTATION || m_bIsPartyLeader == false 
							|| m_pRadioItemGetPattern[0]->IsEnable() == false) 
							break;
						
						m_siItemGetPattern = ITEM_GETPATTERN_ROTATION;

						// 설정이 바뀌는것이며 파티의 리더이며 버튼이 활성화 되어있다면 파티원 전원에게 메시지 보냄(자신은 빼고?)
						cltGameMsgRequest_Party_ChangeItemGetPattern clinfo( ITEM_GETPATTERN_ROTATION );
						cltMsg clMsg(GAMEMSG_REQUEST_PARTY_CHANGEITEMGETPATTERN, sizeof(clinfo) ,(BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);			
					}
				}
				break;
			}
		}
		break;
	case NPARTY_DIALOG_RADIO2:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pRadioItemGetPattern[1]->IsCheck())
					{   // 이미 설정되어 있는 상태거나, 파티의 리더가 아니라면, 버튼이 비활성화 되어있다면 메시지 안보냄
						if(m_siItemGetPattern == ITEM_GETPATTERN_NORMAL || m_bIsPartyLeader == false 
							|| m_pRadioItemGetPattern[1]->IsEnable() == false) 
							break;

						// 설정이 바뀌는것이며 파티의 리더이며 버튼이 활성화 되어있다면 파티원 전원에게 메시지 보냄(자신은 빼고?)                 
						m_siItemGetPattern = ITEM_GETPATTERN_NORMAL;
						cltGameMsgRequest_Party_ChangeItemGetPattern clinfo( ITEM_GETPATTERN_NORMAL );
						cltMsg clMsg(GAMEMSG_REQUEST_PARTY_CHANGEITEMGETPATTERN, sizeof(clinfo) ,(BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);			
					}
				}
				break;
			}
		}
		break;
	case NPARTY_DIALOG_RADIO3:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pRadioItemGetPattern[2]->IsCheck())
					{ // 이미 설정되어 있는 상태거나, 파티의 리더가 아니라면, 버튼이 비활성화 되어있다면 메시지 안보냄
						if(m_siItemGetPattern == ITEM_GETPATTERN_FINISH || m_bIsPartyLeader == false 
							|| m_pRadioItemGetPattern[2]->IsEnable() == false) 
							break;

						// 설정이 바뀌는것이며 파티의 리더이며 버튼이 활성화 되어있다면 파티원 전원에게 메시지 보냄(자신은 빼고?)                   
						m_siItemGetPattern = ITEM_GETPATTERN_FINISH;
						cltGameMsgRequest_Party_ChangeItemGetPattern clinfo( ITEM_GETPATTERN_FINISH );
						cltMsg clMsg(GAMEMSG_REQUEST_PARTY_CHANGEITEMGETPATTERN, sizeof(clinfo) ,(BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);			
					}
				}
				break;
			}
		}
		break;
	case NPARTY_DIALOG_RADIO4:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pRadioItemGetPattern[3]->IsCheck())
					{ // 이미 설정되어 있는 상태거나, 파티의 리더가 아니라면, 버튼이 비활성화 되어있다면 메시지 안보냄
						if(m_siItemGetPattern == ITEM_GETPATTERN_RANDOM || m_bIsPartyLeader == false 
							|| m_pRadioItemGetPattern[3]->IsEnable() == false) 
							break;

						// 설정이 바뀌는것이며 파티의 리더이며 버튼이 활성화 되어있다면 파티원 전원에게 메시지 보냄(자신은 빼고?)                    
						m_siItemGetPattern = ITEM_GETPATTERN_RANDOM;
						cltGameMsgRequest_Party_ChangeItemGetPattern clinfo( ITEM_GETPATTERN_RANDOM );
						cltMsg clMsg(GAMEMSG_REQUEST_PARTY_CHANGEITEMGETPATTERN, sizeof(clinfo) ,(BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);			
					}
				}
				break;
			}
		}
		break;
	case NPARTY_DIALOG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI32 TempSelectIndex = m_pList->GetSelectedIndex();

					if(TempSelectIndex < 0)
						break;

					m_siSelectedUserListIndex = TempSelectIndex;
				}
				break;
			}
			
		}
		break;

	case NPRATY_DIALOG_BUTTON_MINIMIZE: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Minimi(TRUE);
				}
				break;
			}
		}
		break;
	
	case NPARTY_DIALOG_IMAGESTATIC_MINIMIZED:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					Minimi(FALSE);
				}
				break;
			}
		}
		break;


	case NPARTY_DIALOG_MOVE_BUTTON:	// 파티원에게 순간이동
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_siSelectedUserListIndex >= 0 )
					{
						SI16 siSelectedPartyIndex = -1;
						TCHAR* szSelectedUserName = m_pList->GetText(m_siSelectedUserListIndex,0);

						// 이름이 없으면 불가능
						if( szSelectedUserName == NULL) 
							return;						

						// 자신에게로의 워프라면 불가능
						if(_tcscmp(pclclient->pclCM->CR[1]->GetName(),szSelectedUserName) == 0  ) 
							return;
						
												
						for(SI16 i = 0; i < MAX_PARTY_USER_NUM; i++)
						{
							// 동일한 이름을 찾아 파티 인덱스를 찾는다.
							if(  _tcscmp(m_stClientPartyUser[i].strCharName , szSelectedUserName ) == 0 )
							{
								siSelectedPartyIndex = i;
								break;
							}
						}	

						// 맙소사. 선택된 파티원이 없다.
						if( siSelectedPartyIndex < 0 ) 
							return;											

						TCHAR buffer[256] = TEXT("") ;
						TCHAR * pText = NULL ;

						// 캐릭터가 워프를 할 수 있는가?
						if( pclclient->pclCM->CR[1]->CanWarpFrom() == false )
							return;

						// 공짜 워프 체크
						if( pclclient->pclCM->CR[1]->CanFreeWarp( 0 ) == false )
						{
							SI32 itemunique = ITEMUNIQUE(24000) ;  // 순간이동이용권
							if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
							{
								TCHAR* pText = GetTxtFromMgr(3756);
								StringCchPrintf(buffer, 256, pText);
								pText = GetTxtFromMgr(3326);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pText, buffer);
								return;
							}

							pText = GetTxtFromMgr(5184);
							StringCchPrintf(buffer, 256, pText,m_stClientPartyUser[ siSelectedPartyIndex ].strCharName);
						}
						else
						{
							pText = GetTxtFromMgr(5185);
							StringCchPrintf(buffer, 256, pText,m_stClientPartyUser[ siSelectedPartyIndex ].strCharName);
						}

						stMsgBox MsgBox;
						pText = GetTxtFromMgr(5183);
						MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 2);
												
						pclclient->SetMsgBox( &MsgBox, (BYTE*)m_stClientPartyUser[ siSelectedPartyIndex ].strCharName , MAX_PLAYER_NAME );
					}
				}
				break;
			}
		}
		break;

	case NPARTY_DIALOG_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:	// 강퇴 확인 다이얼로그 팝업
				{		// 파티장 이면서 자기 자신은 아닌 것을 선택했을때
					SI16 siSelectedPartyIndex = 0;
					TCHAR* szSelectedUserName = m_pList->GetText(m_siSelectedUserListIndex,0);
					if( szSelectedUserName == NULL) 
					{	// 선택된 인덱스가 없거나 이름이 없다면 자기 자신을 파티탈퇴시킨다.
						siSelectedPartyIndex = 0;
					}
					else
					{
						for(SI16 i = 0; i < MAX_PARTY_USER_NUM; i++)
						{
							// 동일한 이름을 찾아 파티 인덱스를 찾아준다
							if(  _tcscmp(m_stClientPartyUser[i].strCharName , szSelectedUserName ) == 0 )
							{
								siSelectedPartyIndex = i;
								break;
							}
						}
					}
					if ( m_bIsPartyLeader && siSelectedPartyIndex > 0 )
					{
						TCHAR Buffer[256 ] = TEXT("");
						TCHAR* pText = GetTxtFromMgr(1986);
						StringCchPrintf( Buffer, 256, pText, m_stClientPartyUser[ siSelectedPartyIndex ].strCharName );

						TCHAR* pTitle = GetTxtFromMgr(1987);

						stMsgBox MsgBox;
						MsgBox.Set( this,  pTitle, Buffer, MSGBOX_TYPE_YESNO, 0 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( m_siPartyIndex ) + sizeof(m_stClientPartyUser[ siSelectedPartyIndex ].siCharUnique);
						memcpy( TempBuffer, &m_siPartyIndex, sizeof(m_siPartyIndex) );
						memcpy( &TempBuffer[ sizeof(m_siPartyIndex) ] , &m_stClientPartyUser[ siSelectedPartyIndex ].siCharUnique, sizeof(m_stClientPartyUser[ siSelectedPartyIndex ].siCharUnique) );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
					}
					// 파티장이 아닐때 ( 자기 자신만 나갈때 )
					else
					{
						TCHAR* pText = GetTxtFromMgr(1988);
						TCHAR* pTitle = GetTxtFromMgr(1987);

						stMsgBox MsgBox;
						MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 1 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( m_siPartyIndex );
						memcpy( TempBuffer, &m_siPartyIndex, Size );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
				
					}
				}
				break;
			}
			
		}
		break;

	case NPARTY_DIALOG_EDIT1:
		{
			switch( nEvent )
			{
//			case EVENT_EDITBOX_CHANGE:
			case EVENT_EDITBOX_STRING:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				//	GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH );
				//	m_strChatData = m_pEdit->GetText();
					MStrCpy(m_strChatData, m_pEdit->GetText(), MAX_CHAT_DATA_LENGTH);

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( _tcslen( m_strChatData ) > 0 )
					{
						SendPartyChat();
					//	SetWindowText( m_hChatDataInputEdit, TEXT("") );
						m_pEdit->SetText(TEXT(""));

					}
				//	else if (wParam == VK_ESCAPE )
				//	{
				//		SetWindowText( m_hChatDataInputEdit, TEXT("") );
				//	}
				}
				break;
			}
		}
		break;

	case NPARTY_DIALOG_STARTQUEST:		// 파티퀘스트 시작
		{
			switch(nEvent)
			{
				case EVENT_BUTTON_CLICKED:
					{
						// 파티퀘스트 임무를 부여 받을수 있는 파티인지 체크 합니다.
						// 해당 체크는 클라이언트와 서버 양쪽에서 체크 합니다.

						cltClient* pClient = (cltClient*)pclClient;

						bool bResult = pClient->m_pPartyQuestMgr->DoCheckPartyQuest(m_siPartyUserNum, m_stClientPartyUser);

						if(bResult == true)
						{
							cltGameMsgRequest_PartyQuest_CheckStart clData(GetPartyIndex());
							cltMsg clMsg(GAMEMSG_REQUEST_PARTY_QUEST_CHECKSTART, sizeof(cltGameMsgRequest_PartyQuest_CheckStart), (BYTE*)&clData);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);														
						}
						else
						{
							StartPartyQuestMessage(false);
						}
					}
					break;
			}
		}
		break;
	}
}

void CNPartyDlg::Hide()
{
	Initialize();				// 파티가 종료되었으므로 기존에 있던 정보들을 초기화 해줌
	
	ItemGetPatternButtonEnable(false);
	m_pRadioItemGetPattern[0]->SetNonEventCheck();

	m_siPartyUserNum = 0;

	m_bIsPartyLeader = false;
	m_bPartyStatus = false;

	m_pList->Clear();

	if ( m_strViewData )
	{	
		memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );
	}

	if ( m_strChatData )
	{
		memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
	}	

	m_siSelectedUserListIndex = -1;

	TCHAR* pText = GetTxtFromMgr(1989);
//	SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_PARTY_USER_NUM ), pText );
	m_pStatic[0]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	CControllerMgr::Show( false );
}

void CNPartyDlg::Show()
{	
	if(IsMinimi() == TRUE) Minimi(FALSE);
	
	
	///////////
	
	//파티획득막음
	//m_pRadioItemGetPattern[1]->SetNonEventCheck();
	//ItemGetPatternButtonEnable(false);


	CControllerMgr::Show( true );	
	SetFocus(m_pEdit);
}

void CNPartyDlg::EnableButton(CButton* pButton, TCHAR* pText, BOOL Flag /* = false */)
{
	if(Flag)
	{
		pButton->SetFontColor(RGB(0, 0, 0));
		pButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pButton->Enable(true);
	}
	else
	{
		pButton->SetFontColor(RGB(225, 225, 225));
		pButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pButton->Enable(false);
	}
}

void CNPartyDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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
			if ( RetValue )
			{
				cltGameMsgRequest_Party_ForceLeave clPartyForceLeave( *((SI32*)pData), *((SI32*)&pData[4]) );
				cltMsg clMsg(GAMEMSG_REQUEST_PARTY_FORCELEAVE, sizeof(clPartyForceLeave), (BYTE*)&clPartyForceLeave);
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
				cltGameMsgRequest_Party_Leave clPartyLeave( *((SI32*)pData) );
				cltMsg clMsg(GAMEMSG_REQUEST_PARTY_LEAVE, sizeof(clPartyLeave), (BYTE*)&clPartyLeave);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				Hide();
			}
		}
		break;

	case 2:
		{
			if( RetValue )
			{
				cltGameMsgRequest_PartyWarp clinfo((TCHAR*)pData );
				cltMsg clMsg(GAMEMSG_REQUEST_PARTYWARP,sizeof(clinfo),(BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else
				return;
		}
		break;

	case 3:
		{
			if( RetValue )
			{
				// 파티퀘스트 시작 확인시 구현부 입니다.

				// 1.서버로 파티퀘스트 임무 부여 요청 합니다.
				cltGameMsgRequest_PartyQuest_Start clData(GetPartyIndex());
				cltMsg clMsg(GAMEMSG_REQUEST_PARTY_QUEST_START, sizeof(cltGameMsgRequest_PartyQuest_Start), (BYTE*)&clData);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);															
			}
		}
		break;

	case 4:
		{
			if( RetValue )
			{
				// 파티원 초대로 파티퀘스트가 포기될수 있다는 메세지를 보여줍니다.

				cltGameMsgRequest_Party_Invite clPartyInvite( m_siInviteCharUniqueForPartyQuest, GetPartyIndex() );
				cltMsg clMsg( GAMEMSG_REQUEST_PARTY_INVITE, sizeof( clPartyInvite ), (BYTE*)&clPartyInvite );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}

void CNPartyDlg::ItemGetPatternButtonEnable(bool bEnable)
{
	m_pRadioItemGetPattern[0]->Enable(bEnable);
	m_pRadioItemGetPattern[1]->Enable(bEnable);
	m_pRadioItemGetPattern[2]->Enable(bEnable);
	m_pRadioItemGetPattern[3]->Enable(bEnable);

}

void CNPartyDlg::ChangeItemGetPattern( SI16 ItemGetPattern )
{
  	cltClient* pclclient = (cltClient*)pclClient;

	// 설정이 바뀐것이 아니라면 아무 변화 필요 없이 끝냄
	/*
	if( m_siItemGetPattern == ItemGetPattern )
		return;
	*/
	m_siItemGetPattern = ItemGetPattern;

	TCHAR DestTxt[256] = TEXT("");
	TCHAR *pPatternChangeMsg = GetTxtFromMgr(5850);	
	TCHAR *pText = NULL;

	if(ItemGetPattern == ITEM_GETPATTERN_ROTATION)
	{
		m_pRadioItemGetPattern[0]->SetNonEventCheck();
		pText = GetTxtFromMgr(5845);
		StringCchPrintf(DestTxt,256,pPatternChangeMsg,pText);
	}
	else if(ItemGetPattern == ITEM_GETPATTERN_NORMAL)
	{
		m_pRadioItemGetPattern[1]->SetNonEventCheck();
		pText = GetTxtFromMgr(5848);
		StringCchPrintf(DestTxt,256,pPatternChangeMsg,pText);
	} 
	else if(ItemGetPattern == ITEM_GETPATTERN_FINISH)
	{
		m_pRadioItemGetPattern[2]->SetNonEventCheck();
		pText = GetTxtFromMgr(5846);
		StringCchPrintf(DestTxt,256,pPatternChangeMsg,pText);
	}
	else if(ItemGetPattern == ITEM_GETPATTERN_RANDOM)
	{
		m_pRadioItemGetPattern[3]->SetNonEventCheck();
		pText = GetTxtFromMgr(5847);
		StringCchPrintf(DestTxt,256,pPatternChangeMsg,pText);
	}

	pclclient->pclMessage->SetMsg(DestTxt,0,RGB(170,255,170),RGB(0,0,0));//   RGB(50,145,50));

}

void CNPartyDlg::PartyNotifyItemGet(SI32 CharUnique, cltItem* pclItem)
{
	cltClient* pclclient = (cltClient*)pclClient;
		
	for(SI16 i = 0; i<MAX_PARTY_USER_NUM; i++ )
	{
		if(m_stClientPartyUser[i].bPartyStatus && m_stClientPartyUser[i].siCharUnique == CharUnique )
		{
			
			TCHAR strBuffer[256] = TEXT("");		//문장을 만들기 위해서	
			TCHAR* pMsgSrc = GetTxtFromMgr(5855);	//문장을 만들기 위해서	
			TCHAR strItemName[128] = TEXT("");		//문장을 만들기 위해서	
			pclItem->GetFullName( pclClient->pclItemManager ,strItemName, 128);	//문장을 만들기 위해서	
			StringCchPrintf(strBuffer, 256,pMsgSrc,m_stClientPartyUser[i].strCharName, strItemName, pclItem->GetItemNum());//문장완성

			if( pclClient->IsCountrySwitch( Switch_PartyMassageimprovement ) ){
				stChatEditData ChatEditData;			// 파티 챗팅 다이얼로그 안으로 넣기 위해서
				ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_BLACK;
				ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_BLUE;

				StringCchCopy( ChatEditData.strChatData, CHATEDIT_CHATDATA_LENGTH, strBuffer );
				m_pChatEdit->Insert(&ChatEditData);			//파티 챗팅안으로 집어 넣는다.
			}
			else{
				pclclient->pclMessage->SetMsg(strBuffer,0, RGB(220,220,220),RGB(0,0,0) );
			}
			
			break;
		}	
	}
}

void CNPartyDlg::SetPartyUserPos(SI32 CharUnique, cltCharPos* pclCharPos)
{
	BOOL bReroadFlg = FALSE;

	for(SI16 i = 0; i<MAX_PARTY_USER_NUM; i++ )
	{
		if(m_stClientPartyUser[i].bPartyStatus && m_stClientPartyUser[i].siCharUnique == CharUnique )
		{
			if(m_stClientPartyUser[i].clCharPos.GetMapIndex() != pclCharPos->GetMapIndex() )
				bReroadFlg = TRUE;

			m_stClientPartyUser[i].clCharPos.Set(pclCharPos);	

			if(bReroadFlg)	// 파티원의 맵인덱스가 변경되었다면 파티다이얼로그의 리스트 갱신
				ReloadPartyUserList();
			break;
		}	
	}
}


void CNPartyDlg::SetPartyUserLevel(SI32 CharUnique, SI32 Level)
{
	for(SI16 i = 0; i < MAX_PARTY_USER_NUM; i++ )
	{
		if(m_stClientPartyUser[i].bPartyStatus && m_stClientPartyUser[i].siCharUnique == CharUnique )
		{			
			m_stClientPartyUser[i].siLevel = Level;
			break;
		}	
	}
	ReloadPartyUserList();
}

cltCharPos* CNPartyDlg::GetPartyUserPos(SI16 partyuserindex)
{
	if( MAX_PARTY_USER_NUM <= partyuserindex || partyuserindex < 0 ) return NULL;

	return &m_stClientPartyUser[partyuserindex].clCharPos;    	
}

void CNPartyDlg::Minimi(BOOL bMini )
{	
	if(m_bMini == bMini) return; 

	cltClient *pclclient = (cltClient*)pclClient;

	if( bMini )
	{	
		m_siOldPosX = GetX();
		m_siOldPosY = GetY();
		SetMoveable(FALSE);

		// 최소화한 다이얼로그의 위치를 다른 최소화다이얼로그의 위치를 따져 설정한다.
	
		SI16 LetterMinimizedSlot = 0, PrivateChatMinimizedSlot = 0, HelpButtonMiniMizedSlot = 0, LoveLevelMiniMizedSlot = 0 , FamilyCookieEffect = 0 , FamilyMini = 0 , siDormancyMiniMizedSlot = 0;
		
		if( pclclient->m_pDialog[NLETTER_DLG])
			LetterMinimizedSlot = ((CNLetterDlg*)pclclient->m_pDialog[NLETTER_DLG])->GetMinimizedSlot();
		
		if( pclclient->m_pNPrivateChatDlgMgr)
			PrivateChatMinimizedSlot = pclclient->m_pNPrivateChatDlgMgr->GetMinimizedSlot();

		if(pclClient->IsCountrySwitch(Switch_FreshManHelpDialog))
		{
			if( pclclient->m_pDialog[NHELPBUTTON_DLG])
				HelpButtonMiniMizedSlot = ((CNHelpButtonDlg*)pclclient->m_pDialog[NHELPBUTTON_DLG])->GetMinimizedSlot();
		}

		if ( pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG] )
			LoveLevelMiniMizedSlot = ((CNMarriageLoveLevelDlg*)pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG])->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG] )
			FamilyCookieEffect = ((CNFamilyCookieDlg*)pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG])->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NFAMILYINFO_DLG] )
			FamilyCookieEffect = ((CNFamilyInfoDlg*)pclclient->m_pDialog[NFAMILYINFO_DLG])->GetMinimizedSlot();
		
		if ( pclclient->m_pDialog[NDORMANCYPOINT_DLG] )
			siDormancyMiniMizedSlot = ((CNDormancyPointDlg*)pclclient->m_pDialog[NDORMANCYPOINT_DLG])->GetMinimizedSlot();


		// 빈 공간을 찾는다.
		for(SI16 i = 1; i <= MAX_MINIMIZE_SLOT; i++)
		{
			if( i == LetterMinimizedSlot || i == PrivateChatMinimizedSlot || i == HelpButtonMiniMizedSlot || i == LoveLevelMiniMizedSlot || i == FamilyCookieEffect || i == FamilyMini || i == siDormancyMiniMizedSlot)
				continue;
			
			m_siMinimizedSlot = i;
			break;
		}
		
		// 슬롯에 따른 x 좌표로 움직여 준다.
		SetDialogPos( pclclient->GetScreenXSize() - 40 - (m_siMinimizedSlot-1) * 45 , 215, 34, 34 );
	
//		SetDialogPos( pclclient->GetScreenXSize() - 102 , 210, 32, 34 );
		SetFileName(NULL);
		SetTitle(NULL);
		
		m_pImageMinimized->Show(TRUE);	

		m_pStatic[0]->Show(FALSE);
		m_pStatic[1]->Show(FALSE);
		m_pStatic[2]->Show(FALSE);
		m_pList->Show(FALSE);
		m_pButton->Show(FALSE);
		m_pMoveButton->Show(FALSE);
		m_pEdit->Show(FALSE);
		m_pRadioItemGetPattern[0]->Show(FALSE);
		m_pRadioItemGetPattern[1]->Show(FALSE);
		m_pRadioItemGetPattern[2]->Show(FALSE);
		m_pRadioItemGetPattern[3]->Show(FALSE);
		m_pChatEdit->Show(FALSE);
		m_pOutLine[0]->Show(FALSE);
		m_pOutLine[1]->Show(FALSE);
	}
	else
	{
		SetMoveable(TRUE);
		SetDialogPos( m_siOldPosX, m_siOldPosY, 347, 416 );
		SetFileName("ninterface/interface/interface_20x20_00_109.spr");
		SetTitle(GetTxtFromMgr(1221));	// 타이틀 재설정
		m_pImageMinimized->Show(FALSE);	

		m_pStatic[0]->Show(TRUE);
		m_pStatic[1]->Show(TRUE);
		m_pStatic[2]->Show(TRUE);
		m_pList->Show(TRUE);
		m_pButton->Show(TRUE);
		m_pMoveButton->Show(TRUE);
		m_pEdit->Show(TRUE);
		m_pRadioItemGetPattern[0]->Show(TRUE);
		m_pRadioItemGetPattern[1]->Show(TRUE);
		m_pRadioItemGetPattern[2]->Show(TRUE);
		m_pRadioItemGetPattern[3]->Show(TRUE);
		m_pChatEdit->Show(TRUE);
		m_pOutLine[0]->Show(TRUE);
		m_pOutLine[1]->Show(TRUE);
		
		m_siMinimizedSlot = 0;
	}
	m_bMini = bMini;

	UpdatePartyQuestBtn();

}

// [영훈] 해적선 : 해적선 안에 들어 왔을때는 탈퇴버튼 비활성화 시키기
void CNPartyDlg::DoNotLeaveParty()
{
	if ( m_bMini == TRUE ) return;

	SI32 siMapIndex		= pclClient->pclCM->CR[1]->GetMapIndex();
	bool bConditionA	= pclClient->pclMapManager->IsPartyMap( siMapIndex );

	if ( bConditionA == true )
	{
		m_pButton->Enable( false );
	}
	else
	{
		m_pButton->Enable( true );
	}
}

bool CNPartyDlg::IsPartyMember( const SI32 siFindCharID )
{
	if ( 0 >= siFindCharID )
	{
		return false;
	}

	for ( SI32 siIndex=0; siIndex<MAX_PARTY_USER_NUM; siIndex++ )
	{
		if ( (true == m_stClientPartyUser[siIndex].bPartyStatus) && (siFindCharID == m_stClientPartyUser[siIndex].siCharUnique) )
		{
			return true;
		}
	}
	
	return false;
}

void CNPartyDlg::Action()
{
	DoNotLeaveParty();
	UpdatePartyQuestBtn();
}

void CNPartyDlg::StartPartyQuestMessage(SI32 siMessage)
{
	// 파티퀘스트 시작시 조건체크 결과 관련된 메세지 출력 부분 입니다.

	switch(siMessage)
	{
	case 	PARTYQUEST_MESSAGE_CAN_START:
		{
			// 파티퀘스트 가능

			if ( true == m_bIsPartyQuestParty )
			{
				TCHAR* pTitle	= GetTxtFromMgr(40428);
				NTCHARString256	pText(GetTxtFromMgr(40429));

				cltClient *pClient = (cltClient*)pclClient;

				SI32 siMinLevel = pClient->m_pPartyQuestMgr->GetPartyQuestCategoryData(m_siCategory)->m_siMinLevel;
				SI32 siMaxLevel = pClient->m_pPartyQuestMgr->GetPartyQuestCategoryData(m_siCategory)->m_siMaxLevel;

				NTCHARString64	pNeedLevel("");
				TCHAR pTemp[64] = "";

				pNeedLevel += itoa(siMinLevel, pTemp, 10);
				ZeroMemory(pTemp, sizeof(pTemp));
				pNeedLevel += " ~ ";
				pNeedLevel += itoa(siMaxLevel, pTemp, 10);

				pText.Replace("#LEVEL#", pNeedLevel);


				stMsgBox MsgBox;
				MsgBox.Set( this,  pTitle, pText, MSGBOX_TYPE_YESNO, 3);		
				pClient->SetMsgBox(&MsgBox, NULL, 0);
			}
		}
		break;

	case PARTYQUEST_MESSAGE_CANNOT_START:
		{
			// 파티퀘스트 조건 불충분
			TCHAR*	pTitle	= GetTxtFromMgr(512);
			TCHAR*	pBody	= GetTxtFromMgr(40446);

			cltClient *pClient = (cltClient*)pclClient;
			pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);	
		}
		break;

	case PARTYQUEST_MESSAGE_FAILED_LESS_USER:
		{
			// 파티퀘스트 조건 불충분
			TCHAR*	pTitle	= GetTxtFromMgr(512);
			TCHAR*	pBody	= GetTxtFromMgr(40801);

			cltClient *pClient = (cltClient*)pclClient;
			pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);	
		}
		break;

	case PARTYQUEST_MESSAGE_ANSWER_FAILED:
		{
			// 파티퀘스트 가능

			TCHAR* pTitle	= GetTxtFromMgr(512);
			TCHAR* pText	= GetTxtFromMgr(40802);

			stMsgBox MsgBox;
			MsgBox.Set( this,  pTitle, pText, MSGBOX_TYPE_YESNO, 4);		
			cltClient* pClient = (cltClient*)pclClient;
			pClient->SetMsgBox(&MsgBox, NULL, 0);
		}
		break;

	case PARTYQUEST_MESSAGE_FAILED_ADDED_USER:
		{
			// 파티퀘스트 조건 불충분
			TCHAR*	pTitle	= GetTxtFromMgr(512);
			TCHAR*	pBody	= GetTxtFromMgr(40803);

			cltClient *pClient = (cltClient*)pclClient;
			pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);	
		}
		break;

	case PARTYQUEST_MESSAGE_TIMEOUT:
		{
			// 파티퀘스트 조건 불충분
			TCHAR*	pTitle	= GetTxtFromMgr(512);
			TCHAR*	pBody	= GetTxtFromMgr(1257);

			cltClient *pClient = (cltClient*)pclClient;
			pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);	
		}
		break;

	default:
		break;
	}
}


void CNPartyDlg::UpdatePartyQuestBtn()
{
	cltClient* pClient = (cltClient*) pclClient;

	// 파티매칭 switch 체크
	if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == false)
	{ m_pBtnStartQuest->Show(false); return; }
	if ( false == m_bIsPartyQuestParty ) { m_pBtnStartQuest->Show(false); return; }
	if ( 0 > m_siCategory ) { m_pBtnStartQuest->Show(false); return; }

	// 창이 최소화 되어 있지 않을때만 버튼이 보인다.
	if(m_bMini == false)
	{
		if ( 0 > m_siCategory ) { m_pBtnStartQuest->Show(false); return; }

		if(m_bIsPartyLeader == true && pClient->m_pPartyQuestMgr->m_PartyQuestData.m_bGoingQuest == false )
			m_pBtnStartQuest->Show(true);
		else
			m_pBtnStartQuest->Show(false);
	}
	else
	{
		m_pBtnStartQuest->Show(false);
	}


	//// 파티퀘스트 시작 버튼은 파티장이고 아직 파티퀘스트를 시작하지않은 플레이어만 활성화 됩니다. (파티매칭에 등록되어있지않아도 보이지 않습니다.)
	//if(m_bMini == false)
	//{
	//	if(m_bIsPartyLeader == true && pClient->m_pPartyQuestMgr->m_PartyQuestData.m_bGoingQuest == false && pClient->m_pClient_PartyMatchingMgr->Get_Category() > -1)
	//		m_pBtnStartQuest->Show(true);
	//	else
	//		m_pBtnStartQuest->Show(false);
	//}
	//else
	//	m_pBtnStartQuest->Show(false);

}

void CNPartyDlg::Set_PartyQuestInfo(bool isPartyQuest, SI32 siCategory, SI32 siMission)
{
	if( pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching)  == true )
	{
		cltClient* pClient = (cltClient*) pclClient;

		if( false == m_bIsPartyLeader ) return;
		if (false == isPartyQuest )	 return;
		if(  0 > siCategory && 0 > siMission ) return;

		// 버튼은 일단 안보이는 상태가 디폴트. 그리고 파티매칭에서 넘어온 값 세팅 먼저 한다
		m_pBtnStartQuest->Show(false);

		m_bIsPartyQuestParty  = isPartyQuest;
		m_siCategory = siCategory;
		m_siMission = siMission;
		
		//// 파티퀘스트 시작 버튼은 파티장이고 아직 파티퀘스트를 시작하지않은 플레이어만 활성화 됩니다. (파티매칭에 등록되어있지않아도 보이지 않습니다.)
		if(m_bMini == false)
		{
			if(m_bIsPartyLeader == true && pClient->m_pPartyQuestMgr->m_PartyQuestData.m_bGoingQuest == false )
				m_pBtnStartQuest->Show(true);
		}
	}	
}


