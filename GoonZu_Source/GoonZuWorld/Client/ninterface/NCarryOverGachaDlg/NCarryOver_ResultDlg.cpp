
#include "NCarryOver_ResultDlg.h"
#include "NCarryOverGachaDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../../common/Char/CharManager/CharManager.h"
#include "../../Client/ninterface/ngachadlg/NOpenBoxDlg.h"

extern cltCommonLogic* pclClient;

CNCarryOver_ResultDlg::CNCarryOver_ResultDlg( void )
{
	m_pText = NULL;
}

CNCarryOver_ResultDlg::~CNCarryOver_ResultDlg( void )
{

}

void CNCarryOver_ResultDlg::Init( )
{
		
}

void CNCarryOver_ResultDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NCarryOverGacha/DLG_NCarryOver_ResultDlg.ddf" ) );
	file.CreatePopupDialog( this, NGACHARESULT_DLG, TEXT( "dialog_gacharesult" ), StaticCallBackDialogNCarryOver_ResultDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		GACHARESULT_DLG_BUTTON_RESTART,				this), GACHARESULT_DLG_BUTTON_RESTART,				TEXT("button_restart") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		GACHARESULT_DLG_BUTTON_BACK,				this), GACHARESULT_DLG_BUTTON_BACK,					TEXT("button_back") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		GACHARESULT_DLG_EDITBOX_TITLE,				this), GACHARESULT_DLG_EDITBOX_TITLE,				TEXT("editbox_title") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		GACHARESULT_DLG_EDITBOX_ITEMNAME,			this), GACHARESULT_DLG_EDITBOX_ITEMNAME,			TEXT("editbox_itemname") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		GACHARESULT_DLG_EDITBOX_EXPLAIN,			this), GACHARESULT_DLG_EDITBOX_EXPLAIN,				TEXT("editbox_explain") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	GACHARESULT_DLG_IMAGESTATIC_RAREITEM,		this), GACHARESULT_DLG_IMAGESTATIC_RAREITEM,		TEXT("imagestatic_rareitem") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	GACHARESULT_DLG_IMAGESTATIC_GET_RAREITEM,	this), GACHARESULT_DLG_IMAGESTATIC_GET_RAREITEM,	TEXT("imagestatic_get_rareitem") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	GACHARESULT_DLG_IMAGESTATIC_GET_ITEM,		this), GACHARESULT_DLG_IMAGESTATIC_GET_ITEM,		TEXT("imagestatic_get_item") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	GACHARESULT_DLG_IMAGESTATIC_BACKGROUND,		this), GACHARESULT_DLG_IMAGESTATIC_BACKGROUND,		TEXT("imagestatic_background") );

	// 다이얼로그 창 위치 관련 설정
	SetTopMostWindow( true );

	if( g_bFullScreen)	
	{
		MovePos( 80, 40 );
	}
	else		 		
	{
		MovePos( 45, 20 );
	}

	SetWidth( 0 );

	// 기본 텍스트 설정
	CEdit* pclTitle = m_InterfaceFactory.GetEdit( GACHARESULT_DLG_EDITBOX_TITLE );
	if ( pclTitle )
	{
		pclTitle->SetTextPrintOption( DT_VCENTER | DT_CENTER );
		pclTitle->SetFontSize( 13 );
		pclTitle->SetFontWeight( FW_BOLD );
		pclTitle->SetText( GetTxtFromMgr(7026) );
	}

	m_pText = GetTxtFromMgr(6632);
}

void CALLBACK CNCarryOver_ResultDlg::StaticCallBackDialogNCarryOver_ResultDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNCarryOver_ResultDlg* pThis = (CNCarryOver_ResultDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNCarryOver_ResultDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNCarryOver_ResultDlg::CallBackDialogNCarryOver_ResultDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch(nControlID)
	{
	case GACHARESULT_DLG_BUTTON_RESTART:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent)
			{
				OnClickReStart();
			}
		}
		break;

	case GACHARESULT_DLG_BUTTON_BACK:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent)
			{
				OnClickBack();
			}
		}
		break;
	}
}

void CNCarryOver_ResultDlg::DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY )
{
	DrawTitleText( ScreenRX, ScreenRY );
}

void CNCarryOver_ResultDlg::DrawTitleText(SI32 ScreenRX, SI32 ScreenRY )
{
	if ( NULL == m_pText )
	{
		return;
	}

	cltClient *pclclient = ( cltClient * )pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	HDC hdc;
	HFONT hFont;

	if(pclclient->lpBackScreen->GetDC(&hdc) != DD_OK) return;

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) )
	{
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else 
	{
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}

	HFONT hOldFont = NULL;
	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	SetBkMode(hdc, TRANSPARENT);

	// 각 창에 관련된 간략한 설명을 그려준다.
	RECT rect = { 443 , 260 , 725 , 317 };

	OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY - 2 );

	RECT temprc;

	SetTextColor(hdc, RGB(255, 255, 255));

	// 외각선 그리기	
	CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );

	CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );

	SetTextColor(hdc, RGB(50, 146, 255));
	DrawText( hdc, m_pText, lstrlen(m_pText), &rect, DT_CENTER | DT_VCENTER );


	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc, hOldFont)) ;
	} 

	pclclient->lpBackScreen->ReleaseDC(hdc);

}


void CNCarryOver_ResultDlg::OnClickReStart( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		DeleteDialog();
		return;
	}

	cltCharClient* pclchar = pclclient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		DeleteDialog();
		return;
	}

	if ( NULL == pclclient->m_pDialog[NCARRYOVERGACHA_DLG] )
	{
		DeleteDialog();
		return;
	}

	// 더이상 받을 수 없다면
	if ( false == pclchar->IsBlankInv(1, INVMODE_ALL) )
	{
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6634) );

		pclclient->m_pDialog[NCARRYOVERGACHA_DLG]->DeleteDialog();
		return;
	}

	if ( false == ((CNCarryOverGachaDlg*)pclclient->m_pDialog[NCARRYOVERGACHA_DLG])->IsHaveKey( ) )
	{
		(m_InterfaceFactory.GetButton(GACHARESULT_DLG_BUTTON_RESTART))->Enable( false );
		return;
	}

	// 상자 개봉 다이얼로그 연다
	pclclient->CreateInterface( NOPENBOX );
	if ( NULL == pclclient->m_pDialog[NOPENBOX] )
	{
		pclclient->m_pDialog[NCARRYOVERGACHA_DLG]->DeleteDialog();
		return;
	}

	SI32 siKeyKind			= ((CNCarryOverGachaDlg*)pclclient->m_pDialog[NCARRYOVERGACHA_DLG])->GetKeyKind();
	SI32 siProductIndex		= ((CNCarryOverGachaDlg*)pclclient->m_pDialog[NCARRYOVERGACHA_DLG])->GetSelectGachaProductIndex();
	SI32 siProductUnique	= ((CNCarryOverGachaDlg*)pclclient->m_pDialog[NCARRYOVERGACHA_DLG])->GetSelectGachaProductUnique();

	((CNOpenBoxDlg*)pclclient->m_pDialog[NOPENBOX])->SetCarryOverInfo( siKeyKind, siProductIndex, siProductUnique );

	// 재시작 버튼의 포커스를 뺀다
	//CButton* pclButton = m_InterfaceFactory.GetButton( GACHARESULT_DLG_BUTTON_RESTART );
	//if ( pclButton )
	//{
	//	pclButton->SetFocus( false );
	//}

	m_pText = GetTxtFromMgr(6632);
}

void CNCarryOver_ResultDlg::OnClickBack( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		DeleteDialog();
		return;
	}

	if ( NULL == pclclient->m_pDialog[NCARRYOVERGACHA_DLG] )
	{
		DeleteDialog();
		return;
	}

	if ( pclclient->m_pDialog[NGACHAPRODUCT_DLG] )
	{
		pclclient->m_pDialog[NGACHAPRODUCT_DLG]->DeleteDialog();
	}

	((CNCarryOverGachaDlg*)pclclient->m_pDialog[NCARRYOVERGACHA_DLG])->OnEventBack();

	DeleteDialog();

}

void CNCarryOver_ResultDlg::SetGachaGetProduct( cltItem* pclItem, bool bGetRareItem )
{
	if ( NULL == pclItem )
	{
		DeleteDialog();
		return;
	}
	
	cltItemManagerClient* pclItemManager = (cltItemManagerClient*)pclClient->pclItemManager;
	if ( NULL == pclItemManager )
	{
		DeleteDialog();
		return;
	}

	TCHAR* pszItemName = (TCHAR*)pclItemManager->GetName( pclItem->siUnique );
	if ( NULL == pszItemName )
	{
		DeleteDialog();
		return;
	}

	CEdit* pclItemName = m_InterfaceFactory.GetEdit( GACHARESULT_DLG_EDITBOX_ITEMNAME );
	if ( NULL == pclItemName )
	{
		DeleteDialog();
		return;
	}

	pclItemName->SetTextPrintOption( DT_CENTER | DT_VCENTER );
	pclItemName->SetText( pszItemName );
 
	// 최고 상품을 얻었을 경우
	if ( true == bGetRareItem )
	{	
		m_InterfaceFactory.GetImageStatic(GACHARESULT_DLG_IMAGESTATIC_GET_RAREITEM)->Show(true);

		CImageStatic* pclRareImage = m_InterfaceFactory.GetImageStatic( GACHARESULT_DLG_IMAGESTATIC_GET_RAREITEM );
		if ( NULL == pclRareImage )
		{
			return;
		}

		TCHAR*	pszFileName		= NULL;
		SI32	siImageIndex	= 0;
		
		if ( pclItemManager->GetItemImageBigFileName(pclItem->siUnique, &pszFileName, &siImageIndex) )
		{
			pclRareImage->SetFileName( pszFileName );
			pclRareImage->SetFontIndex( siImageIndex );
		}
	}
	else
	{
		m_InterfaceFactory.GetImageStatic(GACHARESULT_DLG_IMAGESTATIC_GET_RAREITEM)->Show(false);

		CImageStatic* pclImage = m_InterfaceFactory.GetImageStatic( GACHARESULT_DLG_IMAGESTATIC_GET_ITEM );
		if ( NULL == pclImage )
		{
			return;
		}

		SI32 siGImageIndex	= 0;
		SI32 siGimageFont	= 0;

		if ( pclItemManager->GetItemGImageFileIndexAndFont(pclItem->siUnique, &siGImageIndex, &siGimageFont) )
		{
			pclImage->SetItemSprType( siGImageIndex );
			pclImage->SetFontIndex( siGimageFont );
		}
	}

	m_pText = GetTxtFromMgr(7023);

}

void CNCarryOver_ResultDlg::SetInfo( SI32 siGachaItemUnique )
{
	
	if ( 0 >= siGachaItemUnique )
	{
		return;
	}

	// 최고 가챠 상품 설명
	CEdit* pclExplain = m_InterfaceFactory.GetEdit( GACHARESULT_DLG_EDITBOX_EXPLAIN );
	if ( NULL == pclExplain )
	{
		return;
	}

	TCHAR* pszItemName = (TCHAR*)pclClient->pclItemManager->GetName( siGachaItemUnique );
	if ( NULL == pszItemName )
	{
		return;
	}

	NTCHARString512 strExplain = GetTxtFromMgr( 7017 );

	strExplain.Replace( TEXT("#gacharareitemname#"), pszItemName );
	pclExplain->SetText( strExplain );

	// 최고 가챠 상품 이미지
	cltItemManagerClient* pclItemManager = (cltItemManagerClient*)pclClient->pclItemManager;
	if ( NULL == pclItemManager )
	{
		return;
	}

	CImageStatic* pclItem = m_InterfaceFactory.GetImageStatic( GACHARESULT_DLG_IMAGESTATIC_RAREITEM );
	if ( NULL == pclItem )
	{
		return;  
	}

	TCHAR*	pszFileName		= NULL;
	SI32	siImageIndex	= 0;

	if ( false == pclItemManager->GetItemImageBigFileName(siGachaItemUnique, &pszFileName, &siImageIndex) )
	{
		return;
	}

	pclItem->SetFileName( pszFileName );
	pclItem->SetFontIndex( siImageIndex );
}

