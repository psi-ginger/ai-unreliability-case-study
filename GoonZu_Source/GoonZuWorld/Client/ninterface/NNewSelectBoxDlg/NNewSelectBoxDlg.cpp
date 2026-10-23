
#include "NNewSelectBoxDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../CommonLogic/MsgType-Item.h"
#include "../../../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

CNNewSelectBoxDlg::CNNewSelectBoxDlg( void )
{
	m_siBoxItemUnique	= 0;
	m_siSelectIndex		= 0;
	m_siGetItemUnique	= 0;
	m_siMode			= 0;
	m_siDrawIndex		= 0;
	m_fSpeed			= 0.0;
	m_dwDrawTickCount	= 0;

	ZeroMemory( m_siSelectItemList, sizeof(m_siSelectItemList) );

}

CNNewSelectBoxDlg::~CNNewSelectBoxDlg( void )
{

}

void CNNewSelectBoxDlg::Init( )
{
		
}

void CNNewSelectBoxDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NNewSelectBoxDlg/DLG_NNewSelectBox.ddf" ) );
	file.CreatePopupDialog( this, NEWSELECTBOX_DLG, TEXT( "dialog_newselectbox" ), StaticCallBackDialogNNewSelectBoxDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	NEWSELECTBOX_DLG_IMAGESTATIC_GETITEM,	this),	NEWSELECTBOX_DLG_IMAGESTATIC_GETITEM,	TEXT("imagestatic_getitem") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	NEWSELECTBOX_DLG_IMAGESTATIC_CENTER,	this),	NEWSELECTBOX_DLG_IMAGESTATIC_CENTER,	TEXT("imagestatic_center") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	NEWSELECTBOX_DLG_IMAGESTATIC_NONAME3,	this),	NEWSELECTBOX_DLG_IMAGESTATIC_NONAME3,	TEXT("imagestatic_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	NEWSELECTBOX_DLG_IMAGESTATIC_NONAME1,	this),	NEWSELECTBOX_DLG_IMAGESTATIC_NONAME1,	TEXT("imagestatic_NONAME1") );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME1,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME1,		TEXT("button_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME2,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME2,		TEXT("button_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME3,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME3,		TEXT("button_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME4,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME4,		TEXT("button_NONAME4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME5,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME5,		TEXT("button_NONAME5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME6,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME6,		TEXT("button_NONAME6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME7,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME7,		TEXT("button_NONAME7") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME8,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME8,		TEXT("button_NONAME8") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME9,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME9,		TEXT("button_NONAME9") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME10,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME10,		TEXT("button_NONAME10") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME11,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME11,		TEXT("button_NONAME11") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_NONAME12,		this),	NEWSELECTBOX_DLG_BUTTON_NONAME12,		TEXT("button_NONAME12") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NEWSELECTBOX_DLG_BUTTON_CLOSE,			this),	NEWSELECTBOX_DLG_BUTTON_CLOSE,			TEXT("button_close") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NEWSELECTBOX_DLG_EDITBOX_NONAME1,		this),	NEWSELECTBOX_DLG_EDITBOX_NONAME1,		TEXT("editbox_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NEWSELECTBOX_DLG_EDITBOX_GETITEM,		this),	NEWSELECTBOX_DLG_EDITBOX_GETITEM,		TEXT("editbox_getitem") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,		NEWSELECTBOX_DLG_OUTLINE_NONAME1,		this),	NEWSELECTBOX_DLG_OUTLINE_NONAME1,		TEXT("outline_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,		NEWSELECTBOX_DLG_OUTLINE_NONAME2,		this),	NEWSELECTBOX_DLG_OUTLINE_NONAME2,		TEXT("outline_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,		NEWSELECTBOX_DLG_OUTLINE_NONAME3,		this),	NEWSELECTBOX_DLG_OUTLINE_NONAME3,		TEXT("outline_NONAME3") );

	m_timerSendGetItem.Init( 4 * 1000 );

	SetActionTimer( 100 );
	SetTimerOnOff( true );



}

void CALLBACK CNNewSelectBoxDlg::StaticCallBackDialogNNewSelectBoxDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNNewSelectBoxDlg* pThis = (CNNewSelectBoxDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNNewSelectBoxDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNNewSelectBoxDlg::CallBackDialogNNewSelectBoxDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
		case NEWSELECTBOX_DLG_BUTTON_NONAME1:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 1 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME2:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 2 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME3:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 3 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME4:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 4 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME5:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 5 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME6:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 6 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME7:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 7 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME8:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 8 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME9:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 9 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME10:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 10 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME11:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 11 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_NONAME12:
			{
				if ( EVENT_BUTTON_CLICKED == nEvent )	SelectButtonIndex( 12 );
			}
			break;

		case NEWSELECTBOX_DLG_BUTTON_CLOSE:
			{
				DeleteDialog();
			}
			break;

		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;

	}
}

void CNNewSelectBoxDlg::Action()
{
	if ( m_timerSendGetItem.IsTimed(GetTickCount(), false) )
	{
		cltGameMsgRequest_NewSelectBox_GetItem clInfo( m_siBoxItemUnique, m_siSelectIndex );
		cltMsg clMsg( GAMEMSG_REQUEST_NEWSELECTBOX_GETITEM, sizeof(clInfo), (BYTE *)&clInfo );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
}

void CNNewSelectBoxDlg::DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY )
{
	if ( NEWSELECTBOX_MODE_OPEN == m_siMode )
	{
		return;
	}

	SI32	siGimgIndex	= 0;
	SI32	siFontIndex	= 0;
	TSpr*	pSPR		= NULL;

	SI16	siStartPosX	= GetX() + ScreenRX + 20;
	SI16	siStartPosY	= GetY() + ScreenRY + 260;

	SI32	siRealIndex = 0;
	

	for ( SI32 siIndex=0; siIndex<7; ++siIndex )
	{

		siRealIndex = (siIndex+m_siDrawIndex) % MAX_NEWSELECTBOX_ITEM_LIST;
		//siRealIndex = rand() % MAX_NEWSELECTBOX_ITEM_LIST;

		if ( true == pclClient->pclItemManager->GetItemGImageFileIndexAndFont(m_siSelectItemList[siRealIndex], &siGimgIndex, &siFontIndex) )
		{
			pSPR = pclClient->GetGlobalSpr( siGimgIndex );
			if ( pSPR )
			{
				GP.PutSpr( pSPR, siStartPosX + (siIndex*40) + 20, siStartPosY, siFontIndex );
			}
		}
	}

	if ( (NEWSELECTBOX_MODE_SELECT == m_siMode) && (m_dwDrawTickCount < GetTickCount()) )
	{
		m_fSpeed = m_fSpeed + (REAL32)0.0003;

		m_dwDrawTickCount	= GetTickCount() + (10000.0 * m_fSpeed);
		m_siDrawIndex		= ++m_siDrawIndex % MAX_NEWSELECTBOX_ITEM_LIST;	
	}
	else if ( (NEWSELECTBOX_MODE_GETITEM == m_siMode) && (m_dwDrawTickCount < GetTickCount()) )
	{
		SI16 siCompareIndex = ((m_siDrawIndex+3) % MAX_NEWSELECTBOX_ITEM_LIST);
		if ( m_siSelectItemList[siCompareIndex] != m_siGetItemUnique )
		{
			m_dwDrawTickCount	= GetTickCount() + (10000.0 * m_fSpeed);
			m_siDrawIndex		= ++m_siDrawIndex % MAX_NEWSELECTBOX_ITEM_LIST;
		}
		else 
		{
			ShowGetItemInfo();

			m_siMode = NEWSELECTBOX_MODE_SUCCESS;
		}
	}


}

void CNNewSelectBoxDlg::SetSelectInfo( SI16 siSelectIndex, SI16* psiItemList )
{
	if ( (0 >= siSelectIndex) || (MAX_NEWSELECTBOX_SELECTINDEX < siSelectIndex) )
	{
		return;
	}

	if ( NULL == psiItemList )
	{
		return;
	}

	m_siSelectIndex = siSelectIndex;

	memcpy( m_siSelectItemList, psiItemList, sizeof(m_siSelectItemList) );

	m_siMode = NEWSELECTBOX_MODE_SELECT;

	m_timerSendGetItem.SetActive( true, GetTickCount() );
	
}

void CNNewSelectBoxDlg::SetGetItemInfo( cltItem* pclGetItem )
{
	if ( NULL == pclGetItem )
	{
		return;
	}

	SI16 siGetItemUnique	= pclGetItem->siUnique;
	SI16 siGetItemNum		= pclGetItem->siItemNum;
	if ( (0 >= siGetItemUnique) || (0 >= siGetItemNum) )
	{
		return;
	}

	m_siGetItemUnique	= siGetItemUnique;
	m_siGetItemNum		= siGetItemNum;

	m_siMode = NEWSELECTBOX_MODE_GETITEM;

	InsertGetItem();
}

void CNNewSelectBoxDlg::ShowGetItemInfo( void )
{
	if ( (0 >= m_siGetItemUnique) || (0 >= m_siGetItemNum) )
	{
		return;
	}

	ChangeAllSelectButtonEnableMode( true );

	//-----------------------------------------------------------
	// 아이템 이미지
	//-----------------------------------------------------------
	SI32 siGImgIndex = 0;
	SI32 siFontIndex = 0;
	if ( false == pclClient->pclItemManager->GetItemGImageFileIndexAndFont(m_siGetItemUnique, &siGImgIndex, &siFontIndex) )
	{
		return;
	}

	CImageStatic* pclItemImgCtrl = m_InterfaceFactory.GetImageStatic( NEWSELECTBOX_DLG_IMAGESTATIC_GETITEM );
	if ( NULL == pclItemImgCtrl )
	{
		return;
	}

	pclItemImgCtrl->SetItemSprType( siGImgIndex );
	pclItemImgCtrl->SetFontIndex( siFontIndex );

	//-----------------------------------------------------------
	// 아이템 메시지
	//-----------------------------------------------------------
	CEdit* pclItemTextCtrl = m_InterfaceFactory.GetEdit( NEWSELECTBOX_DLG_EDITBOX_GETITEM );
	if ( NULL == pclItemTextCtrl )
	{
		return;
	}

	NTCHARString512 strGetItemInfo( GetTxtFromMgr(9779) );

	TCHAR* pszItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_siGetItemUnique );
	
	strGetItemInfo.Replace( TEXT("#itemname#"),	pszItemName );
	strGetItemInfo.Replace( TEXT("#itemnum#"),	SI32ToString(m_siGetItemNum) );

	pclItemTextCtrl->SetText( strGetItemInfo );

}

void CNNewSelectBoxDlg::InsertGetItem( void )
{
	for ( SI32 siIndex=0; siIndex<7; ++siIndex )
	{
		SI16 siCompareIndex = (((m_siDrawIndex+3)+siIndex) % MAX_NEWSELECTBOX_ITEM_LIST);
		if ( m_siGetItemUnique == m_siSelectItemList[siCompareIndex] )
		{
			return;
		}
	}

	SI32 siInsertIndex = (m_siDrawIndex+7) % MAX_NEWSELECTBOX_ITEM_LIST;
	m_siSelectItemList[siInsertIndex] = m_siGetItemUnique;
}
void CNNewSelectBoxDlg::OpenNewSelectBox( SI16 siItemUnique )
{
	switch ( siItemUnique )
	{
		case ITEMUNIQUE(13340):
			{
				OpenNewYearSelectBox( siItemUnique );
			}
			break;

	}
}

void CNNewSelectBoxDlg::OpenNewYearSelectBox( SI16 siItemUnique )
{
	// 모드변수
	m_siMode = NEWSELECTBOX_MODE_OPEN;

	// 상자 아이템 유니크 저장
	m_siBoxItemUnique = siItemUnique;

	// 대화장자 제목
	NTCHARString32 strTitle( GetTxtFromMgr(9777) );
	SetTitle( strTitle );

	// 설명
	CEdit* pclEdit = m_InterfaceFactory.GetEdit( NEWSELECTBOX_DLG_EDITBOX_NONAME1 );
	if ( NULL == pclEdit )
	{
		return;
	}

	NTCHARString512 strExplain( GetTxtFromMgr(9778) );
	
	pclEdit->SetText( strExplain );

	// 버튼 이미지
	//
}

void CNNewSelectBoxDlg::SelectButtonIndex( SI16 siSelectIndex )
{
	if ( (0 >= siSelectIndex) || (MAX_NEWSELECTBOX_SELECTINDEX < siSelectIndex) )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
	if ( NULL == pclchar )
	{
		return;
	}

	//박스 아이템이 없는 상태에서도 버튼을 누를 수 있기때문에
	SI32 siBoxItemPos = pclchar->pclCI->clCharItem.FindItemInv( m_siBoxItemUnique );
	if ( PERSONITEM_INV0 > siBoxItemPos )
	{
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(10304) );

		// 자신도 닫아 버린다
		DeleteDialog();
		return;
	}

	//일단 인벤에 1칸의 빈칸은 있어야 한다
	if ( false == pclchar->IsBlankInv(1, INVMODE_ALL) )
	{ 
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6634) );
		return;
	}

	// 그리기 관련 변수 초기화
	InitForDraw();

	ChangeAllSelectButtonEnableMode( false );

	cltGameMsgRequest_NewSelectBox_Select clInfo( m_siBoxItemUnique, siSelectIndex );
	cltMsg clMsg( GAMEMSG_REQUEST_NEWSELECTBOX_SELECT, sizeof(clInfo), (BYTE *)&clInfo );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CNNewSelectBoxDlg::InitForDraw()
{
	m_siMode			= NEWSELECTBOX_MODE_OPEN;
	m_siDrawIndex		= 0;
	m_fSpeed			= 0.0;
	m_dwDrawTickCount	= 0;
	m_siGetItemUnique	= 0;
	m_siGetItemNum		= 0;
	ZeroMemory( m_siSelectItemList, sizeof(m_siSelectItemList) );

	CImageStatic* pclItemImgCtrl = m_InterfaceFactory.GetImageStatic( NEWSELECTBOX_DLG_IMAGESTATIC_GETITEM );
	if ( pclItemImgCtrl )
	{
		pclItemImgCtrl->SetItemSprType( 0 );
		pclItemImgCtrl->SetFontIndex( 0 );	
	}

	CEdit* pclItemTextCtrl = m_InterfaceFactory.GetEdit( NEWSELECTBOX_DLG_EDITBOX_GETITEM );
	if ( pclItemTextCtrl )
	{
		pclItemTextCtrl->SetText( TEXT("") );
	}
	
}

void CNNewSelectBoxDlg::ChangeAllSelectButtonEnableMode( bool bEnable )
{
	SI32 siButtonTypeID[MAX_NEWSELECTBOX_SELECTINDEX] = {
		NEWSELECTBOX_DLG_BUTTON_NONAME1,
		NEWSELECTBOX_DLG_BUTTON_NONAME2,
		NEWSELECTBOX_DLG_BUTTON_NONAME3,
		NEWSELECTBOX_DLG_BUTTON_NONAME4,
		NEWSELECTBOX_DLG_BUTTON_NONAME5,
		NEWSELECTBOX_DLG_BUTTON_NONAME6,
		NEWSELECTBOX_DLG_BUTTON_NONAME7,
		NEWSELECTBOX_DLG_BUTTON_NONAME8,
		NEWSELECTBOX_DLG_BUTTON_NONAME9,
		NEWSELECTBOX_DLG_BUTTON_NONAME10,
		NEWSELECTBOX_DLG_BUTTON_NONAME11,
		NEWSELECTBOX_DLG_BUTTON_NONAME12
	};
	
	CButton* pclButton = NULL;

	for ( SI32 siIndex=0; siIndex<MAX_NEWSELECTBOX_SELECTINDEX; ++siIndex )
	{
		pclButton = m_InterfaceFactory.GetButton( siButtonTypeID[siIndex] );
		if ( NULL == pclButton )
		{
			continue;
		}

		pclButton->Enable( bEnable );
	}
	
}
