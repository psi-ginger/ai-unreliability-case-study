
#include "NCharHouseDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../CommonLogic/MsgType-Person.h"

extern cltCommonLogic* pclClient;

CNCharHouseDlg::CNCharHouseDlg( void )
{

}

CNCharHouseDlg::~CNCharHouseDlg( void )
{

}

void CNCharHouseDlg::Init( )
{
		
}

void CNCharHouseDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/DLG_NCharHouse.ddf" ) );
	file.CreatePopupDialog( this, NCHARHOUSE_DLG, TEXT( "dialog_charhouse" ), StaticCallBackDialogNCharHouseDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	CHARHOUSE_DLG_BUTTON_DELETE,	this), CHARHOUSE_DLG_BUTTON_DELETE,		TEXT("button_delete") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	CHARHOUSE_DLG_BUTTON_CHANGE,	this), CHARHOUSE_DLG_BUTTON_CHANGE,		TEXT("button_change") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	CHARHOUSE_DLG_EDITBOX_MESSAGE,	this), CHARHOUSE_DLG_EDITBOX_MESSAGE,	TEXT("editbox_message") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST,	CHARHOUSE_DLG_LISTBOX_CHARLIST,	this), CHARHOUSE_DLG_LISTBOX_CHARLIST,	TEXT("listbox_charlist") );

	// 캐릭터 창 리스트 설정
	CList* pclList = m_InterfaceFactory.GetList( CHARHOUSE_DLG_LISTBOX_CHARLIST );
	if ( NULL == pclList )
	{
		DeleteDialog();
		return;
	}

	pclList->SetColumn( 0, 40, NULL );
	pclList->SetColumn( 1, pclList->GetWidth()-40 , NULL );
	pclList->SetBorder( true );
	pclList->SetBKColorUse( true );
	pclList->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_WORDBREAK );
	pclList->GetScroll()->Enable( true );
	pclList->Refresh();

	// 알림 메시지
	CEdit* pclEdit = m_InterfaceFactory.GetEdit( CHARHOUSE_DLG_EDITBOX_MESSAGE );
	if ( NULL == pclEdit )
	{
		DeleteDialog();
		return;
	}
	
	pclEdit->SetText( GetTxtFromMgr(40184) );


	// 다이얼로그 기본위치
	if ( TRUE == g_bFullScreen )
	{
		SetX( 804 );
		SetY( 552 );
	}
	else
	{
		SetX( 740 );
		SetY( 502 );
	}

	// 다른 창보다 위에 온다
	SetTopMostWindow( true );

	SetCharHouseList();

	SetActionTimer( 500 );
	SetTimerOnOff( true );

}

void CALLBACK CNCharHouseDlg::StaticCallBackDialogNCharHouseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNCharHouseDlg* pThis = (CNCharHouseDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNCharHouseDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNCharHouseDlg::CallBackDialogNCharHouseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case CHARHOUSE_DLG_BUTTON_CHANGE:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )	OnButtonClick_Change();
		}
		break;

	case CHARHOUSE_DLG_BUTTON_DELETE:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )	OnButtonClick_Delete();
		}
		break;
	}

}

void CNCharHouseDlg::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	BOOL bNotClose = FALSE;

	for ( SI32 siIndex=MAX_CHAR_PER_USER; siIndex<MAX_TOTAL_CHAR_PER_USER; siIndex++  )
	{
		if ( 0 < pclclient->clCharListInfo[siIndex].clBI.clPerson.GetPersonID() )
		{
			bNotClose = TRUE;
		}
	}

	if ( FALSE == bNotClose )
	{
		DeleteDialog();
	}

}

void CNCharHouseDlg::SetCharHouseList( void )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	CList* pclList = m_InterfaceFactory.GetList( CHARHOUSE_DLG_LISTBOX_CHARLIST );
	if ( NULL == pclList )
	{
		return;
	}
	
	// 우선 기존 목록이 있으면 지우고
	pclList->Clear();


	// 데이터를 셋팅한다
	stListBoxItemData	listBoxEmptyItemData;
	
	listBoxEmptyItemData.Init();
	listBoxEmptyItemData.SetSmallIconGImg( 0, -1, -1 );
	listBoxEmptyItemData.Set( 1, GetTxtFromMgr(1094) );

	stListBoxItemData	listBoxItemData;
	cltCharListInfo*	pclCharListInfo = NULL;

	NTCHARString256		strText;

	SI32				siGIMGIndex = 0;
	SI32				siFontIndex = 0;

	for ( SI32 siIndex=MAX_CHAR_PER_USER; siIndex<MAX_TOTAL_CHAR_PER_USER; siIndex++ )
	{
		// 초기화 시킨다
		listBoxItemData.Init();

		pclCharListInfo = pclclient->GetCharListInfo( siIndex );
		if ( NULL == pclCharListInfo )
		{
			continue;
		}

		if ( (0 < pclCharListInfo->clBI.GetKind()) && (pclclient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont(pclCharListInfo->clBI.GetKind(), &siGIMGIndex, &siFontIndex)) )
		{
			strText.FormatString( TEXT("%s\r(Lv.%d)"), pclCharListInfo->clBI.GetName(), pclCharListInfo->clIP.GetLevel() );

			listBoxItemData.SetSmallIconGImg( 0, siGIMGIndex, siFontIndex );
			listBoxItemData.Set( 1, strText, pclCharListInfo->clBI.GetIndex() );
			
			pclList->AddItem( &listBoxItemData );
		}
	}

	pclList->Refresh();

}

void CNCharHouseDlg::OnButtonClick_Change( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	CList* pclList = m_InterfaceFactory.GetList( CHARHOUSE_DLG_LISTBOX_CHARLIST );
	if ( NULL == pclList )
	{
		return;
	}

	SI32 siSelectedIndex = pclList->GetSelectedIndex();
	if ( (0 > siSelectedIndex) || (MAX_CHAR_HOUSE_PER_USER <= siSelectedIndex) )
	{
		return;
	}
	
	SI32 siCharIndex = pclList->GetParam( siSelectedIndex, 1 );
	if ( (MAX_CHAR_PER_USER > siCharIndex) || (MAX_TOTAL_CHAR_PER_USER <= siCharIndex) )
	{
		return;
	}

	if ( 0 >= pclclient->clCharListInfo[siCharIndex].clBI.clPerson.GetPersonID() )
	{
		return;
	}

	BOOL bIsEmpty = false;

	// 0,1,2 슬롯에 캐릭터가 전부 있는지 검사한다
	for ( SI32 siIndex=0; siIndex<MAX_CHAR_PER_USER; siIndex++ )
	{
		if ( 0 >= pclclient->clCharListInfo[siIndex].clBI.clPerson.GetPersonID() )
		{
			bIsEmpty = true;
			break;
		}
	}

	// 빈슬롯이 하나도 없다면 바꿀 수 없다
	if ( false == bIsEmpty )
	{
		NTCHARString256	strTitle	= GetTxtFromMgr(4101);
		NTCHARString256	strText		= GetTxtFromMgr(40185);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		return;
	}

	NTCHARString32	strTitle	= GetTxtFromMgr( 4101 );
	NTCHARString256 strText		= GetTxtFromMgr( 40197 );

	stMsgBox		MsgBox;

	MsgBox.Set( this, strTitle, strText, MSGBOX_TYPE_YESNO, 2 );

	pclclient->SetMsgBox( &MsgBox, (BYTE*)&siCharIndex, sizeof(siCharIndex) );

}

void CNCharHouseDlg::OnButtonClick_Delete( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	CList* pclList = m_InterfaceFactory.GetList( CHARHOUSE_DLG_LISTBOX_CHARLIST );
	if ( NULL == pclList )
	{
		return;
	}

	SI32 siSelectedIndex = pclList->GetSelectedIndex();
	if ( (0 > siSelectedIndex) || (MAX_CHAR_HOUSE_PER_USER <= siSelectedIndex) )
	{
		return;
	}

	SI32 siCharIndex = pclList->GetParam( siSelectedIndex, 1 );
	if ( (MAX_CHAR_PER_USER > siCharIndex) || (MAX_TOTAL_CHAR_PER_USER <= siCharIndex) )
	{
		return;
	}

	if ( 0 >= pclclient->clCharListInfo[siCharIndex].clBI.clPerson.GetPersonID() )
	{
		return;
	}

	NTCHARString32	strTitle	= GetTxtFromMgr( 4101 );
	NTCHARString256 strText		= GetTxtFromMgr( 40186 );

	stMsgBox		MsgBox;

	MsgBox.Set( this, strTitle, strText, MSGBOX_TYPE_YESNO, 1 );

	pclclient->SetMsgBox( &MsgBox, (BYTE*)&siCharIndex, sizeof(siCharIndex) );

}

void CNCharHouseDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	switch ( MsgIndex )
	{
	case 1:	// 캐릭터 삭제
		{
			if ( false == RetValue )
			{
				return;
			}
			
			SI32 siFromIndex = *(SI32*)&pData[0];

			cltGameMsgRequest_DelUserChar clinfo(siFromIndex, TEXT(""));
			cltMsg clMsg(GAMEMSG_REQUEST_DELUSERCHAR, sizeof(clinfo), (BYTE*)&clinfo);
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;

	case 2:	// 캐릭터 꺼내기
		{
			if ( false == RetValue )
			{
				return;
			}

			SI32 siCharIndex = *(SI32*)&pData[0];

			cltMsg clMsg(GAMEMSG_REQUEST_PERSONSLOTNUMCHANGE, siCharIndex);
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;
	}
}