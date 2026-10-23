#include "../Client/NInterface/NGoonzuDlg/NGoonzuWorkDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../common/Char/CharCommon/Char-Common.h"
#include "../common/Char/CharManager/CharManager.h"



extern cltCommonLogic* pclClient;

CNGoonZuPolicyDlg::CNGoonZuPolicyDlg() : CDialogController()
{
	m_siSelectNumber = 0;
	m_siSelectedBonusTime = 0;
	m_siSelectedMakeBonusTime = 0;
}

CNGoonZuPolicyDlg::~CNGoonZuPolicyDlg()
{
}

void CNGoonZuPolicyDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NGoonzuDlg/DLG_GoonzuPolicy.ddf");
		file.CreatePopupDialog( this, NGOONZUPOLICY_DLG, "dialog_GoonzuPolicy", StaticCallBackDialogNGoonZuPolicyDlg );

	//	file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC1 ), NGOONZUPOLICY_STATIC1, "statictext_NONAME1"); 
	//	file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC2 ), NGOONZUPOLICY_STATIC2, "statictext_NONAME2"); 
	//	file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC3 ), NGOONZUPOLICY_STATIC3, "statictext_NONAME3"); 
		file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC4 ), NGOONZUPOLICY_STATIC4, "statictext_NONAME4"); 
		file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC5 ), NGOONZUPOLICY_STATIC5, "statictext_NONAME5"); 
		file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC6 ), NGOONZUPOLICY_STATIC6, "statictext_NONAME6"); 
		file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC7 ), NGOONZUPOLICY_STATIC7, "statictext_NONAME7"); 
		file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC8 ), NGOONZUPOLICY_STATIC8, "statictext_NONAME8"); 
		file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC9 ), NGOONZUPOLICY_STATIC9, "statictext_NONAME9"); 
		file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC10 ), NGOONZUPOLICY_STATIC10, "statictext_NONAME10"); 
		file.CreateControl( AddInterface( STATIC, NGOONZUPOLICY_STATIC11 ), NGOONZUPOLICY_STATIC11, "statictext_NONAME11"); 
	//	file.CreateControl( AddInterface( EDIT,   NGOONZUPOLICY_EDIT_LIQUIDATION ), NGOONZUPOLICY_EDIT_LIQUIDATION, "editbox_NONAME1"); 
	//	file.CreateControl( AddInterface( BUTTON, NGOONZUPOLICY_BUTTON_LIQUIDATION ), NGOONZUPOLICY_BUTTON_LIQUIDATION, "button_NONAME1"); 
		file.CreateControl( AddInterface( BUTTON, NGOONZUPOLICY_BUTTON_PROMOTION ), NGOONZUPOLICY_BUTTON_PROMOTION, "button_NONAME2"); 
		file.CreateControl( AddInterface( BUTTON, NGOONZUPOLICY_BUTTON_CLOSE ), NGOONZUPOLICY_BUTTON_CLOSE, "button_NONAME3"); 
		file.CreateControl( AddInterface( BUTTON, NGOONZUPOLICY_BUTTON_SETBONUS ), NGOONZUPOLICY_BUTTON_SETBONUS, "button_NONAME4"); 
	//	file.CreateControl( AddInterface( COMBOBOX, NGOONZUPOLICY_COBOBOX_LIQUIDATION ), NGOONZUPOLICY_COBOBOX_LIQUIDATION, "combobox_NONAME1"); 
		file.CreateControl( AddInterface( COMBOBOX, NGOONZUPOLICY_COBOBOX_PROMOTION ), NGOONZUPOLICY_COBOBOX_PROMOTION, "combobox_NONAME2"); 
		file.CreateControl( AddInterface( COMBOBOX, NGOONZUPOLICY_COBOBOX_BONUS ), NGOONZUPOLICY_COBOBOX_BONUS, "combobox_NONAME3");
		file.CreateControl( AddInterface( COMBOBOX, NGOONZUPOLICY_COBOBOX_MAKEBONUS ), NGOONZUPOLICY_COBOBOX_MAKEBONUS, "combobox_NONAME4");
	//	file.CreateControl( AddInterface( OUTLINE, NGOONZUPOLICY_OUTLINE1 ), NGOONZUPOLICY_OUTLINE1, "outline_NONAME1"); 
		file.CreateControl( AddInterface( OUTLINE, NGOONZUPOLICY_OUTLINE2 ), NGOONZUPOLICY_OUTLINE2, "outline_NONAME2"); 
		file.CreateControl( AddInterface( OUTLINE, NGOONZUPOLICY_OUTLINE3 ), NGOONZUPOLICY_OUTLINE3, "outline_NONAME3"); 

	/*	// 정리 대상 마을 설정
		stComboBoxItemData ComboBoxItemData;

		ComboBoxItemData.Set( "3" );
		m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_LIQUIDATION )->AddItem( &ComboBoxItemData );

		ComboBoxItemData.Set( "4" );
		m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_LIQUIDATION )->AddItem( &ComboBoxItemData );

		ComboBoxItemData.Set( "5" );
		m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_LIQUIDATION )->AddItem( &ComboBoxItemData );

		ComboBoxItemData.Set( "6" );
		m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_LIQUIDATION )->AddItem( &ComboBoxItemData );

		ComboBoxItemData.Set( "7" );
		m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_LIQUIDATION )->AddItem( &ComboBoxItemData );

		ComboBoxItemData.Set( "8" );
		m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_LIQUIDATION )->AddItem( &ComboBoxItemData );
		m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_LIQUIDATION )->SetCursel( 0 );

		// 임시로 버튼 막아놓음
		m_InterfaceFactory_.GetButton( NGOONZUPOLICY_BUTTON_LIQUIDATION )->Enable( false );
	*/



		CComboBox * BonusTimeCombo = m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_BONUS );
		CComboBox * MakeBonusTimeCombo = m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_MAKEBONUS );
		TCHAR buf[32];
		buf[0] = 0;

		for (SI16 i = 0; i < 24; i++)
		{
			stComboBoxItemData tmpComboBoxItemData;

			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );
			BonusTimeCombo->AddItem( &tmpComboBoxItemData );
			MakeBonusTimeCombo->AddItem( &tmpComboBoxItemData );
		}

		// 보너스 시각 설정 콤보 박스는 기본적으로 비활성
		BonusTimeCombo->SetCursel( 0 );
		BonusTimeCombo->Enable( false );
		MakeBonusTimeCombo->SetCursel( 0 );
		MakeBonusTimeCombo->Enable( false );
		// 버튼도 비활성
		m_InterfaceFactory_.GetButton( NGOONZUPOLICY_BUTTON_SETBONUS )->Enable(false);

		// 현재 설정된 보너스 타임 정보를 불러온다

		cltMsg clMsg( GAMEMSG_REQUEST_GOONZU_GETBONUSTIME , 0 , NULL);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

		m_siSelectedBonusTime = 0;
		m_siSelectedMakeBonusTime = 0;

		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENGOONZUDLG_NOTICE);
	}
	else
	{
		DeleteDialog();
	}
}

CControllerObj* CNGoonZuPolicyDlg::AddInterface( SI16 Type, SI32 TypeID )
{
	return m_InterfaceFactory_.CreateUserInterface( Type, TypeID, this );
}


void CALLBACK CNGoonZuPolicyDlg::StaticCallBackDialogNGoonZuPolicyDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNGoonZuPolicyDlg *pThis = (CNGoonZuPolicyDlg *)pControllerMgr;
	pThis->CallBackDialogNGoonZuPolicyDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNGoonZuPolicyDlg::CallBackDialogNGoonZuPolicyDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NGOONZUPOLICY_BUTTON_CLOSE:
		{
			DeleteDialog();
		}
		break;

/*	case NGOONZUPOLICY_BUTTON_LIQUIDATION:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI16 tempSelectIndex = m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_LIQUIDATION )->GetSelectedIndex();
					if( tempSelectIndex < 0 )
						return;
					m_siSelectNumber = atoi( m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_LIQUIDATION )->GetText( tempSelectIndex ) );

					TCHAR *pTitle = GetTxtFromMgr(6055);
					TCHAR Text[256] = {'\0'};
					TCHAR *pText = GetTxtFromMgr(6056);

					sprintf( Text, pText, m_siSelectNumber);

					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, Text, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
					
				}
				break;
			}
		}
		break;*/
	case NGOONZUPOLICY_BUTTON_SETBONUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI16 tempSelectIndex = m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_BONUS )->GetSelectedIndex();
					if( tempSelectIndex < 0 )
						return;
					
					SI16 tempSelectMakeBonusIndex = m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_MAKEBONUS )->GetSelectedIndex();
					if( tempSelectMakeBonusIndex < 0 )
						return;

					
					m_siSelectedBonusTime = atoi( m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_BONUS )->GetText( tempSelectIndex ) );
					m_siSelectedMakeBonusTime = atoi( m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_MAKEBONUS )->GetText( tempSelectMakeBonusIndex ) );

					TCHAR *pTitle = GetTxtFromMgr(6917);
					TCHAR szBuffer[256] = {'\0'};
					TCHAR *pText = GetTxtFromMgr(6918);
					
					StringCchPrintf( szBuffer, 256 , pText, m_siSelectedBonusTime , m_siSelectedMakeBonusTime);

					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, szBuffer, MSGBOX_TYPE_YESNO, 1 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

				}
				break;
			}
		}

	case NGOONZUPOLICY_BUTTON_PROMOTION:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
				}
				break;
			}
		}
		break;

	case NGOONZUPOLICY_COBOBOX_LIQUIDATION:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
				}
				break;
			}
		}
		break;

	case NGOONZUPOLICY_COBOBOX_PROMOTION:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
				}
				break;
			}
		}
		break;
	}
}

void CNGoonZuPolicyDlg::SetGoonzuBonus( SI16 BonusTime , SI16 MakeBonusTime , bool ChangeAble )
{	
	m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_BONUS )->SetCursel( BonusTime );
	m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_BONUS )->Enable(ChangeAble);
	m_InterfaceFactory_.GetButton( NGOONZUPOLICY_BUTTON_SETBONUS )->Enable(ChangeAble);

	m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_MAKEBONUS )->SetCursel( MakeBonusTime );
	m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_MAKEBONUS )->Enable(ChangeAble);
}

void CNGoonZuPolicyDlg::SetLiquidationVillageNumber( SI16 LiquidationVillageNumber )
{
/*	TCHAR tempBuffer[256] = {'\0'};
	TCHAR *pText = GetTxtFromMgr(6057);
	
	cltDate clDate;
	clDate.MakeFullDateFromDateVary(m_siPrizeSetTime);
	StringCchPrintf( tempBuffer, 256, pText, LiquidationVillageNumber, clDate.uiYear, clDate.uiMonth, clDate.uiDay  );
	m_InterfaceFactory_.GetEdit( NGOONZUPOLICY_EDIT_LIQUIDATION )->SetText( tempBuffer );
*/
}

void CNGoonZuPolicyDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:	
		{
			if ( RetValue )
			{
				cltGameMsgRequest_Goonzu_SetLiquidation clinfo( m_siSelectNumber );
				cltMsg clMsg(GAMEMSG_REQUEST_GOONZU_SETLIQUIDATION,sizeof(clinfo),(BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

				TCHAR tempBuffer[256] = {'\0'};
				TCHAR *pText = GetTxtFromMgr(6057);

				sprintf( tempBuffer, pText, m_siSelectNumber );
				m_InterfaceFactory_.GetEdit( NGOONZUPOLICY_EDIT_LIQUIDATION )->SetText( tempBuffer );
			}
		}
		break;

	case 1:	
		{
			if ( RetValue )
			{
				cltGameMsgRequest_Goonzu_SetBonus clinfo( m_siSelectedBonusTime , m_siSelectedMakeBonusTime );
				cltMsg clMsg(GAMEMSG_REQUEST_GOONZU_SETBONUSTIME,sizeof(clinfo),(BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

				m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_BONUS )->Enable(false);
				m_InterfaceFactory_.GetComboBox( NGOONZUPOLICY_COBOBOX_MAKEBONUS )->Enable(false);
				m_InterfaceFactory_.GetButton( NGOONZUPOLICY_BUTTON_SETBONUS )->Enable(false);

			}
		}
		break;
	}


}