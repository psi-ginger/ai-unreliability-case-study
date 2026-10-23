#include "../Client/NStructureDlg/Cityhall/NAccountBook.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "Char\CharManager\CharManager.h"
#include "../../CommonLogic/Cityhall/Cityhall.h"

#include "..\..\Server\Rank\Rank.h"

#include "Map\FieldObject\FieldObject.h"

extern cltCommonLogic* pclClient;

//***************************************************************************************************
//
// CNAccountBookPart1Dlg 시작
//***************************************************************************************************
CNAccountBookPart1Dlg::CNAccountBookPart1Dlg() : CDialogController()
{
}

CNAccountBookPart1Dlg::~CNAccountBookPart1Dlg()
{
}

void CNAccountBookPart1Dlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/NAccountBook/DLG_AccountBookPart1.ddf"));
	file.CreateChildDialog( this, NACCOUNTBOOKPART1_DLG, TEXT("dialog_accountbook_part1"), StaticCallBackDialogNAccountBookPart1Dlg, pParent );

	file.CreateControl( AddInterface( STATIC, NACCOUNTBOOKPART1_STATIC1		 ), NACCOUNTBOOKPART1_STATIC1,		TEXT("statictext_NONAME1")); 
	file.CreateControl( AddInterface( STATIC, NACCOUNTBOOKPART1_STATIC2		 ), NACCOUNTBOOKPART1_STATIC2,		TEXT("statictext_NONAME2")); 
	file.CreateControl( AddInterface( STATIC, NACCOUNTBOOKPART1_STATIC3		 ), NACCOUNTBOOKPART1_STATIC3,		TEXT("statictext_NONAME3")); 
	file.CreateControl( AddInterface( EDIT,   NACCOUNTBOOKPART1_EDIT_CAPITAL ), NACCOUNTBOOKPART1_EDIT_CAPITAL, TEXT("editbox_NONAME1")); 
	file.CreateControl( AddInterface( LIST,   NACCOUNTBOOKPART1_LIST_CAPITAL ), NACCOUNTBOOKPART1_LIST_CAPITAL, TEXT("listbox_NONAME1")); 

	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->SetBKColorUse( true );
	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->SetBorder( true);
	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->Refresh();

	cltClient *pclclient = (cltClient *)pclClient;
	cltCityHallManager *pCityhallManager = pclclient->pclCityHallManager;

	TCHAR* pText[4];

	pText[0] = GetTxtFromMgr(1564);
	pText[1] = GetTxtFromMgr(1565);
	pText[2] = GetTxtFromMgr(1566);
	pText[3] = GetTxtFromMgr(1567);

	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->SetColumn( 0,  90, pText[ 0 ] );
	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->SetColumn( 1, 116, pText[ 1 ] );
	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->SetColumn( 2, 130, pText[ 2 ] );	

	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->Refresh();

	pCityhallManager->UpdateInOutMoneySwitch = FALSE;

	Hide();
}

void CNAccountBookPart1Dlg::Hide()
{
	CControllerMgr::Show( false );
}

void CNAccountBookPart1Dlg::Show()
{
	CControllerMgr::Show( true );
}

CControllerObj* CNAccountBookPart1Dlg::AddInterface( SI16 Type, SI32 TypeID )
{
	return m_InterfaceFactory_.CreateUserInterface( Type, TypeID, this );
}

void CNAccountBookPart1Dlg::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;
	cltCityHallManager *pCityhallManager = pclclient->pclCityHallManager;
	
	stListBoxItemData itemdata;

	int i;
	TCHAR buffer[ 256 ];
	TCHAR buf[ 256 ];
	TCHAR buf2[ 128 ];
	
	cltDate *pclDate=NULL;
	cltCityHallInOutMoneyUnitInfo *pclUnitInfo;

	TCHAR* pText = GetTxtFromMgr(611);
	// 자본금을 보여준다 
	g_JYLibFunction.SetNumUnit( pCityhallManager->clClientStrInfo.clCommonInfo.clMoney.itMoney, buf, 256, pText );
	m_InterfaceFactory_.GetEdit( NACCOUNTBOOKPART1_EDIT_CAPITAL )->SetText( buf );

	int count = pCityhallManager->clClientStrInfo.clCityhallStrInfo.clInOutMoneyInfo.GetCount();

	if( count > 0 && pCityhallManager->UpdateInOutMoneySwitch == FALSE ) {
		pCityhallManager->UpdateInOutMoneySwitch = TRUE;

		m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->Clear();

		for( i = 0; i < count; i++ )
		{
			pclUnitInfo = &pCityhallManager->clClientStrInfo.clCityhallStrInfo.clInOutMoneyInfo.clInOutMoneyUnitInfo[ i ];

			itemdata.Init();
			// 시간 
			pText = GetTxtFromMgr(1508);
			sprintf( buffer, pText, pclUnitInfo->clDate.uiYear, pclUnitInfo->clDate.uiMonth, pclUnitInfo->clDate.uiDay );
			itemdata.Set( 0, buffer );


			// 내용
			buffer[ 0 ] = NULL;
			switch( pclUnitInfo->siInOutCode )
			{
			case INOUTCODE_DIVIDEND:
				pText = GetTxtFromMgr(1568);
				strcpy( buffer, pText );
				break;

			case INOUTCODE_TAX:
				buf2[ 0 ] = NULL;
				if(pclClient->pclVillageStructureSet->GetStructureNameFromRankType(pclUnitInfo->siStrType, buf2, 128 ) == true)
				{
					pText = GetTxtFromMgr(1569);
					sprintf( buffer, pText, buf2 );	
				}
				break;

			case INOUTCODE_NPCPAY:
				strcpy( buf2, pclClient->pclKindInfoSet->GetName( pclUnitInfo->siStrType ) );
				pText = GetTxtFromMgr(1570);
				sprintf( buffer, pText, buf2 );
				break;

			case INOUTCODE_STOCK:
				pText = GetTxtFromMgr(1571);
				sprintf( buffer, pText );
				break;

			case INOUTCODE_GODSTAT:
				pText = GetTxtFromMgr(1572);
				sprintf( buffer, pText );
				break;

			case INOUTCODE_CASTLESTAT:
				pText = GetTxtFromMgr(1573);
				sprintf( buffer, pText );
				break;

			case INOUTCODE_TOPARENT:
				strcpy( buf2, pclClient->pclVillageManager->GetName( pclUnitInfo->siStrType ) );
				pText = GetTxtFromMgr(1574);
				sprintf( buffer, pText, buf2 );
				break;

			case INOUTCODE_FROMCHILD:
				strcpy( buf2, pclClient->pclVillageManager->GetName( pclUnitInfo->siStrType ) );
				pText = GetTxtFromMgr(1575);
				sprintf( buffer, pText, buf2 );
				break;

			case INOUTCODE_EVENTPAY:
				pText = GetTxtFromMgr(1576);
				strcpy( buffer, pText);
				break;

			case INOUTCODE_VILLAGEMARK:
				pText = GetTxtFromMgr(1577);
				strcpy( buffer, pText);
				break;

			case INOUTCODE_RESIDENTSHARE:
				pText = GetTxtFromMgr(1578);
				strcpy( buffer, pText);
				break;

			case INOUTCODE_VILLAGEMAP:
				pText = GetTxtFromMgr(1579);
				strcpy( buffer, pText );
				break;
			case INOUTCODE_INSTANCEPROFIT:
				pText = GetTxtFromMgr(5321);
				StringCchCopy( buffer, 256, pText );
				break;
			case INOUTCODE_DESTROYSTR:
				pText = GetTxtFromMgr(5617);
				StringCchCopy( buffer, 256, pText );
				break;
			case INOUTCODE_NATIONTAX:
				pText = GetTxtFromMgr(6016);
				strcpy( buffer, pText );
				break;
			case INOUTCODE_SETBASEMONEY:
				pText = GetTxtFromMgr(6017);
				strcpy( buffer, pText );
				break;
			case INOUTCODE_VILLAGEWARMONEY:
				
				if( pclUnitInfo->clInOutMoney.itMoney >= 0 )
					pText = GetTxtFromMgr(6480);
				else
					pText = GetTxtFromMgr(6482);

				strcpy( buffer, pText );
				break;
			case INOUTCODE_DONATEMONEY:
				pText = GetTxtFromMgr(10162);
				strcpy( buffer, pText );
				break;
			case INOUTCODE_CREATEEMPIRE:
				pText = "제국건설";
				strcpy( buffer, pText );
				break;
			case INOUTCODE_SETEMPIREMONEY:
				pText = "제국자본금";
				strcpy( buffer, pText );
				break;
			case INOUTCODE_BLACK_WAR:
				pText = GetTxtFromMgr(1335);
				strcpy( buffer, pText );
				break;
			}

			itemdata.Set( 1, buffer );

			// 입출금액
			pText = GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit( pclUnitInfo->clInOutMoney.itMoney, buffer, 256, pText );

			itemdata.Set( 2, buffer );

			m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->AddItem( &itemdata );
		}				

		m_InterfaceFactory_.GetList( NACCOUNTBOOKPART1_LIST_CAPITAL )->Refresh();
	}
}
void CALLBACK CNAccountBookPart1Dlg::StaticCallBackDialogNAccountBookPart1Dlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNAccountBookPart1Dlg *pThis = (CNAccountBookPart1Dlg *)pControllerMgr;
	pThis->CallBackDialogNAccountBookPart1Dlg( nEvent, nControlID, pControl );
}

void CALLBACK CNAccountBookPart1Dlg::CallBackDialogNAccountBookPart1Dlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	return;
}

//***************************************************************************************************
//
// CNAccountBookPart2Dlg 시작 - 비활성화 시킴 차후 백업된 다이얼로그를 이용해서 기능추가및 활성화.
//
//***************************************************************************************************
CNAccountBookPart2Dlg::CNAccountBookPart2Dlg() : CDialogController()
{
}

CNAccountBookPart2Dlg::~CNAccountBookPart2Dlg()
{
}

void CNAccountBookPart2Dlg::Create( CControllerMgr *pParent )
{
/*	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/NAccountBook/DLG_AccountBookPart2.ddf"));
	file.CreateChildDialog( this, NACCOUNTBOOKPART2_DLG, TEXT("dialog_accountbook_part2"), StaticCallBackDialogNAccountBookPart2Dlg, pParent );

	file.CreateControl( AddInterface( STATIC, NACCOUNTBOOKPART2_STATIC1 ), NACCOUNTBOOKPART2_STATIC1, TEXT("statictext_NONAME1")); 
	file.CreateControl( AddInterface( STATIC, NACCOUNTBOOKPART2_STATIC2 ), NACCOUNTBOOKPART2_STATIC2, TEXT("statictext_NONAME2")); 
	file.CreateControl( AddInterface( STATIC, NACCOUNTBOOKPART2_STATIC3 ), NACCOUNTBOOKPART2_STATIC3, TEXT("statictext_NONAME3")); 
	file.CreateControl( AddInterface( EDIT, NACCOUNTBOOKPART2_EDIT_PROFIT ), NACCOUNTBOOKPART2_EDIT_PROFIT, TEXT("editbox_NONAME1")); 
	file.CreateControl( AddInterface( LIST, NACCOUNTBOOKPART2_LIST_PROFIT ), NACCOUNTBOOKPART2_LIST_PROFIT, TEXT("listbox_NONAME1")); 

	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART2_LIST_PROFIT )->SetBKColorUse( true );
	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART2_LIST_PROFIT )->SetBorder( true);
	m_InterfaceFactory_.GetList( NACCOUNTBOOKPART2_LIST_PROFIT )->Refresh();

	Hide();*/
}

void CNAccountBookPart2Dlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNAccountBookPart2Dlg::Show()
{
	CControllerMgr::Show( true );

}

CControllerObj* CNAccountBookPart2Dlg::AddInterface( SI16 Type, SI32 TypeID )
{
	return m_InterfaceFactory_.CreateUserInterface( Type, TypeID, this );
}


void CALLBACK CNAccountBookPart2Dlg::StaticCallBackDialogNAccountBookPart2Dlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNAccountBookPart2Dlg *pThis = (CNAccountBookPart2Dlg *)pControllerMgr;
	pThis->CallBackDialogNAccountBookPart2Dlg( nEvent, nControlID, pControl );
}

void CALLBACK CNAccountBookPart2Dlg::CallBackDialogNAccountBookPart2Dlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	return;
}

//***************************************************************************************************
//
// CNAccountBookPart3Dlg 시작
//***************************************************************************************************
CNAccountBookPart3Dlg::CNAccountBookPart3Dlg() : CDialogController()
{
}

CNAccountBookPart3Dlg::~CNAccountBookPart3Dlg()
{
}

void CNAccountBookPart3Dlg::Create( CControllerMgr *pParent )
{
/*	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NStructureDlg/NAccountBook/DLG_AccountBookBase.ddf");
		file.CreateChildDialog( this, NACCOUNTBOOKBASE_DLG, "dialog_accountbookbase", StaticCallBackDialogNAccountBookPart3Dlg );

		file.CreateControl( AddInterface( BUTTON, NACCOUNTBOOKBASE_BUTTON_PART1 ), NACCOUNTBOOKBASE_BUTTON_PART1, "button_NONAME1"); 
		file.CreateControl( AddInterface( BUTTON, NACCOUNTBOOKBASE_BUTTON_PART2 ), NACCOUNTBOOKBASE_BUTTON_PART2, "button_NONAME2"); 
		file.CreateControl( AddInterface( BUTTON, NACCOUNTBOOKBASE_BUTTON_PART3 ), NACCOUNTBOOKBASE_BUTTON_PART3, "button_NONAME3"); 
		file.CreateControl( AddInterface( BUTTON, NACCOUNTBOOKBASE_BUTTON_CLOSE ), NACCOUNTBOOKBASE_BUTTON_CLOSE, "button_NONAME4"); 
		file.CreateControl( AddInterface( OUTLINE, NACCOUNTBOOKBASE_OUTLINE ), NACCOUNTBOOKBASE_BUTTON_CLOSE, "outline_NONAME1"); 
	}
	else
	{
		DeleteDialog();
	}
*/
}

void CNAccountBookPart3Dlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNAccountBookPart3Dlg::Show()
{
	CControllerMgr::Show( true );

}

CControllerObj* CNAccountBookPart3Dlg::AddInterface( SI16 Type, SI32 TypeID )
{
	return m_InterfaceFactory_.CreateUserInterface( Type, TypeID, this );
}


void CALLBACK CNAccountBookPart3Dlg::StaticCallBackDialogNAccountBookPart3Dlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNAccountBookPart3Dlg *pThis = (CNAccountBookPart3Dlg *)pControllerMgr;
	pThis->CallBackDialogNAccountBookPart3Dlg( nEvent, nControlID, pControl );
}

void CALLBACK CNAccountBookPart3Dlg::CallBackDialogNAccountBookPart3Dlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	return;
}

//***************************************************************************************************
//
// CNAccountBookBaseDlg 시작
//***************************************************************************************************
CNAccountBookBaseDlg::CNAccountBookBaseDlg() : CDialogController()
{
	m_pPart1Dlg_ = NULL;
//	m_pPart2Dlg_ = NULL;
//	m_pPart3Dlg_ = NULL;
}

CNAccountBookBaseDlg::~CNAccountBookBaseDlg()
{
	SAFE_DELETE( m_pPart1Dlg_ );
//	SAFE_DELETE( m_pPart2Dlg_ );
//	SAFE_DELETE( m_pPart3Dlg_ );
}

void CNAccountBookBaseDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/NAccountBook/DLG_AccountBookBase.ddf"));
		file.CreatePopupDialog( this, NACCOUNTBOOKBASE_DLG, TEXT("dialog_accountbookbase"), StaticCallBackDialogNAccountBookBaseDlg );

		file.CreateControl( AddInterface( BUTTON, NACCOUNTBOOKBASE_BUTTON_PART1 ), NACCOUNTBOOKBASE_BUTTON_PART1, TEXT("button_NONAME1")); 
		file.CreateControl( AddInterface( BUTTON, NACCOUNTBOOKBASE_BUTTON_PART2 ), NACCOUNTBOOKBASE_BUTTON_PART2, TEXT("button_NONAME2")); 
		file.CreateControl( AddInterface( BUTTON, NACCOUNTBOOKBASE_BUTTON_PART3 ), NACCOUNTBOOKBASE_BUTTON_PART3, TEXT("button_NONAME3")); 
		file.CreateControl( AddInterface( BUTTON, NACCOUNTBOOKBASE_BUTTON_CLOSE ), NACCOUNTBOOKBASE_BUTTON_CLOSE, TEXT("button_NONAME4")); 
		file.CreateControl( AddInterface( OUTLINE, NACCOUNTBOOKBASE_OUTLINE ), NACCOUNTBOOKBASE_BUTTON_CLOSE, TEXT("outline_NONAME1")); 

		
		m_InterfaceFactory_.GetButton( NACCOUNTBOOKBASE_BUTTON_PART2 )->SetFontColor( BUTTON_DISABLEFONT_COLOR );
		m_InterfaceFactory_.GetButton( NACCOUNTBOOKBASE_BUTTON_PART2 )->Enable(false);


		m_InterfaceFactory_.GetButton( NACCOUNTBOOKBASE_BUTTON_PART3 )->SetFontColor( BUTTON_DISABLEFONT_COLOR );
		m_InterfaceFactory_.GetButton( NACCOUNTBOOKBASE_BUTTON_PART3 )->Enable(false);

		m_pPart1Dlg_ = new CNAccountBookPart1Dlg();
		m_pPart1Dlg_->Create( this );
		m_pPart1Dlg_->Show();



		//m_pPart2Dlg_ = new CNAccountBookPart2Dlg();
		//m_pPart2Dlg_->Create( this );
		//m_pPart1Dlg_->Show();

		//m_pPart2Dlg_ = new CNAccountBookPart3lg();
        //m_pPart2Dlg_->Create( this );
		

		SetTimerOnOff(true);
	}
	else
	{
		DeleteDialog();
	}
}

CControllerObj* CNAccountBookBaseDlg::AddInterface( SI16 Type, SI32 TypeID )
{
	return m_InterfaceFactory_.CreateUserInterface( Type, TypeID, this );
}

void CNAccountBookBaseDlg::Action()
{
	m_pPart1Dlg_->Action();
}

void CALLBACK CNAccountBookBaseDlg::StaticCallBackDialogNAccountBookBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNAccountBookBaseDlg *pThis = (CNAccountBookBaseDlg *)pControllerMgr;
	pThis->CallBackDialogNAccountBookBaseDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNAccountBookBaseDlg::CallBackDialogNAccountBookBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NACCOUNTBOOKBASE_BUTTON_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}

