#include "NotifyGetItemDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

// 약 10 초 보여줌
#define NOTIFYGETITME_SHOW_FRAME	5

CNotifyGetItemDlg::CNotifyGetItemDlg() : CDialogController()
{
	m_siCurrentFrame = 0;

	m_pTextStatic = NULL;
	
	m_pItem = new cltItem();
}

CNotifyGetItemDlg::~CNotifyGetItemDlg()
{
	if ( m_pTextStatic )
	{
		delete m_pTextStatic;
		m_pTextStatic = NULL;
	}
	
	if ( m_pItem )
	{
		delete m_pItem;
		m_pItem = NULL;
	}
}

void CNotifyGetItemDlg::Create()
{
	//cyj 캐릭터 상태 바로 밑으로 위치 조정
	//CDialogController::Create( NOTIFYGETITEM_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNotifyGetItem, TEXT("물품 획득"), TEXT("Interface/NotifyGetItemDlg/NotifyGetItemDlg.Spr"), 0, false, 0, 425, 100, 254, 77 );
	CDialogController::Create( NOTIFYGETITEM_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNotifyGetItem, TEXT("물품 획득"), TEXT("Interface/NotifyGetItemDlg/NotifyGetItemDlg.Spr"), 0, false, 0, 80, 140, 254, 77 );
		
	m_pTextStatic = new CStatic( this );
	m_pTextStatic->Create( NOTIFYGETITEM_DIALOG_TEXT_STATIC, NULL, TEXT("Interface/NotifyGetItemDlg/NotifyGetItemDlgTextStatic.Spr"), 0, false, 0, 57, 22, 192, 48, 192, 48 );
	
	Hide();

	return;
}

void CALLBACK CNotifyGetItemDlg::StaticCallBackDialogNotifyGetItem( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNotifyGetItemDlg *pThis = (CNotifyGetItemDlg *)pControllerMgr;
	pThis->CallBackDialogNotifyGetItem( nEvent, nControlID, pControl );
}

void CALLBACK CNotifyGetItemDlg::CallBackDialogNotifyGetItem( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Hide();
				}
				break;
			default:
				break;
			}
		}
		break;
	}

	return;
}

void CNotifyGetItemDlg::Hide()
{
	CControllerMgr::Show( false );

	m_siCurrentFrame = 0;

	SetTimerOnOff( false );
}

void CNotifyGetItemDlg::Show( cltItem *pclItem, SI64 pricevalue )
{
	SetTimerOnOff( true );

	if ( pclItem == NULL )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->bWatchModeSwitch )
	{
		return;
	}

	CControllerMgr::Show( true );
	m_siCurrentFrame = 0;

	TCHAR szString[ 512 ] = TEXT("");
	
	TCHAR itemname[128] = TEXT("") ;
	TCHAR moneybuf[256] = TEXT("") ;

	m_pItem->Set( pclItem );

	if( pclItem->GetFullName( pclClient->pclItemManager , itemname, 128 ) == true )
	{
		bool bPirce = DATA_TO_BOOL(pricevalue) ;

		if ( bPirce )
		{
			TCHAR* pText = GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit(pricevalue,moneybuf,256, pText );
			pText = GetTxtFromMgr(2393);
			StringCchPrintf( szString, 512, pText, itemname, pclItem->GetItemNum() ,moneybuf);
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(2394);
			StringCchPrintf( szString, 512, pText, itemname, pclItem->GetItemNum() );
		}
		
		TCHAR Title[ 128 ] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(3909);
		StringCchPrintf( Title, 128, pText, itemname );
		
		SetText( Title, szString );
	}
}

void CNotifyGetItemDlg::SetText( TCHAR *Title, TCHAR *strPrintData, ... )
{
	CControllerMgr::Show( true );
	
	SetTitle( Title );

	va_list ap;
	TCHAR PrintMsg[ MAX_PATH ] = TEXT("");
	memset( PrintMsg, 0, MAX_PATH );	
	va_start( ap, strPrintData );
	StringCchVPrintf( PrintMsg, MAX_PATH, strPrintData, ap );
	va_end( ap );

	/*
	if ( WINVER >= 0x0400 )
	{
		m_pTextStatic->SetText( PrintMsg, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EDITCONTROL );
	}
	else
	{
		m_pTextStatic->SetText( PrintMsg, DT_LEFT | DT_TOP | DT_WORDBREAK );
	}
	*/
	
	m_pTextStatic->SetText( PrintMsg, DT_LEFT | DT_TOP | DT_WORDBREAK );

	m_siCurrentFrame = 0;
	
	return;
}

void CNotifyGetItemDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	SI32 Data[]=
	{
		GIMG_ITEMIMG01, 200,
		GIMG_ITEMIMG02, 191,
		GIMG_ITEMIMG03, 210,
		GIMG_ITEMIMG04, 200,
		-1
	};

	//SI32 Data[]=
	//{
	//	GIMG_ITEMIMG01, 40,
	//		GIMG_ITEMIMG02, 40,
	//		GIMG_ITEMIMG03, 40,
	//		GIMG_ITEMIMG04, 40,
	//		GIMG_ITEMIMG05, 40,
	//		GIMG_ITEMIMG06, 40,
	//		GIMG_ITEMIMG07, 40,
	//		GIMG_ITEMIMG08, 40,
	//		GIMG_ITEMIMG09, 40,
	//		GIMG_ITEMIMG10, 31,
	//		GIMG_ITEMIMG11, 40,
	//		GIMG_ITEMIMG12, 40,
	//		GIMG_ITEMIMG13, 40,
	//		GIMG_ITEMIMG14, 40,
	//		GIMG_ITEMIMG15, 40,
	//		GIMG_ITEMIMG16, 40,
	//		GIMG_ITEMIMG17, 40,
	//		GIMG_ITEMIMG18, 40,
	//		GIMG_ITEMIMG19, 40,
	//		GIMG_ITEMIMG20, 40,
	//		-1
	//};

	cltItemInfo* pcliteminfo = pclClient->pclItemManager->FindItemInfo(m_pItem->siUnique);
	if( pcliteminfo == NULL )
		return ;

	SI16 Counter = 0;
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageIndexFont = 0;
	SI32 AccumulationImageCounter = 0;
	
	bool loop = true;
	while( loop )
	{
		if ( Data[ Counter * 2 ] != -1 )
		{
			if ( AccumulationImageCounter + Data[ Counter * 2 + 1 ] - 1 >=  pcliteminfo->siImageIndex )
			{
				GlobalImageIndex = Data[ Counter * 2 ];
				GlobalImageIndexFont = pcliteminfo->siImageIndex - AccumulationImageCounter;
				break;
			}
			else
			{
				AccumulationImageCounter += Data[ Counter * 2 + 1 ];
				++Counter;
			}
		}
		else
		{
			break;
		}
	}

	g_graphics.FillBox16C( 
		ScreenRX + GetX() + 5, 
		ScreenRY + GetY() + 22, 
		ScreenRX + GetX() + 5 + 48, 
		ScreenRY + GetY() + 22 + 48, 
		//g_graphics.MakeRGB16( 239, 255, 255 )
		g_graphics.MakeRGB16( 233, 255, 255 )
		);

	if ( GlobalImageIndex != -1 )
	{
		cltClient *pclclient = ( cltClient * )pclClient;
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( GlobalImageIndex );

		if ( pSpr )
		{
			GP.PutSpr(pSpr, ScreenRX + GetX() + 5, ScreenRY + GetY() + 22, GlobalImageIndexFont );
		}
	}
	else
	{
		cltClient *pclclient = ( cltClient * )pclClient;
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( GIMG_ITEMIMG01 );
		
		if ( pSpr )
		{
			GP.PutSpr(pSpr, ScreenRX + GetX() + 5, ScreenRY + GetY() + 22, 0 );
		}
	}
	
	// 5, 22 이미지 출력
}

void CNotifyGetItemDlg::Action()
{
	// 자동으로 사라지게 하는 코드
	if( IsShow() )
	{
		if ( m_siCurrentFrame++ > NOTIFYGETITME_SHOW_FRAME )
		{
			Hide();
		}
	}

	return;
}