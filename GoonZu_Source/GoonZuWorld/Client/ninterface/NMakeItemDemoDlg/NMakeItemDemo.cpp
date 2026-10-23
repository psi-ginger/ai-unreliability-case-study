#include "NMakeItemDemo.h"

#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/Client.h"
#include "../../../Common/Char/CharManager/CharManager.h"


extern cltCommonLogic* pclClient;

CNMakeItemDemoDlg::CNMakeItemDemoDlg()
{
	Init();
}

CNMakeItemDemoDlg::~CNMakeItemDemoDlg()
{
	Destroy();
}

void CNMakeItemDemoDlg::Init()
{
	m_siTotal	= 0;
	m_siRare1	= 0;
	m_siRare2	= 0;
	m_siRare3	= 0;
	m_bExecute	= FALSE;
}

void CNMakeItemDemoDlg::Destroy()
{

}

void CNMakeItemDemoDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/DLG_MakeItemDemo.ddf"));
	file.CreatePopupDialog( this, NGUILDWARAPPLY_DLG, TEXT("dialog_makeitemdemo"),StaticMakeItemDemoDlgProc );

	// 컨트롤 생성
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMAKEITEMDEMO_STATIC_ITEMUNIQUE, this),	NMAKEITEMDEMO_STATIC_ITEMUNIQUE, TEXT("statictext_ItemUnique"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT   ,NMAKEITEMDEMO_EDIT_ITEMUNIQUE, this),	NMAKEITEMDEMO_EDIT_ITEMUNIQUE,	TEXT("editbox_ItemUnique"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMAKEITEMDEMO_BUTTON_EXECUTE, this),	NMAKEITEMDEMO_BUTTON_EXECUTE,	TEXT("button_Execute"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMAKEITEMDEMO_BUTTON_CANCEL, this),		NMAKEITEMDEMO_BUTTON_CANCEL,	TEXT("button_Cancel"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMAKEITEMDEMO_STATIC_TOTAL, this),	NMAKEITEMDEMO_STATIC_TOTAL, TEXT("statictext_total"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMAKEITEMDEMO_STATIC_RARE1, this),	NMAKEITEMDEMO_STATIC_RARE1, TEXT("statictext_Rare1"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMAKEITEMDEMO_STATIC_RARE2, this),	NMAKEITEMDEMO_STATIC_RARE2, TEXT("statictext_Rare2"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMAKEITEMDEMO_STATIC_RARE3, this),	NMAKEITEMDEMO_STATIC_RARE3, TEXT("statictext_Rare3"));

	CStatic*	pStatic = m_InterfaceFactory.GetStatic(NMAKEITEMDEMO_STATIC_ITEMUNIQUE);
	CEdit*		pEdit	= m_InterfaceFactory.GetEdit(NMAKEITEMDEMO_EDIT_ITEMUNIQUE);
	CButton*	pButton_Execute = m_InterfaceFactory.GetButton(NMAKEITEMDEMO_BUTTON_EXECUTE);
	CButton*	pButton_Cancel = m_InterfaceFactory.GetButton(NMAKEITEMDEMO_BUTTON_CANCEL);

	if(pStatic)
	{
		pStatic->SetText(TEXT("ItemUnique"));
	}

	if(pButton_Execute)
	{
		pButton_Execute->SetText(TEXT("Execute"));
	}

	if(pButton_Cancel)
	{
		pButton_Cancel->SetText(TEXT("Cancel"));
	}

	SetActionTimer( 50 );
	SetTimerOnOff( true );
}

void CNMakeItemDemoDlg::ShowMakeItemDemoDlg(bool bShow)
{
	if(bShow)
	{
	}

	CDialogController::Show( bShow );
	
}

void CALLBACK CNMakeItemDemoDlg::StaticMakeItemDemoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMakeItemDemoDlg *pThis = (CNMakeItemDemoDlg*) pControllerMgr;
	pThis->NMakeItemDemoDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNMakeItemDemoDlg::NMakeItemDemoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}			
		}
		break;
	case NMAKEITEMDEMO_BUTTON_CANCEL:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_bExecute = FALSE;
				}
				break;
			}
		}
		break;
	case NMAKEITEMDEMO_BUTTON_EXECUTE:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siTotal	= 0;
					m_siRare1	= 0;
					m_siRare2	= 0;
					m_siRare3	= 0;
					m_bExecute	= TRUE;

					CEdit*		pEdit	= m_InterfaceFactory.GetEdit(NMAKEITEMDEMO_EDIT_ITEMUNIQUE);
					if(pEdit)
					{
						TCHAR* pItemUnique = pEdit->GetText();
						m_siItemUnique = atoi(pItemUnique);
					}
				}
				break;
			}
		}
		break;
	}

}

void CNMakeItemDemoDlg::Action()
{
	if(m_bExecute && m_siItemUnique > 0)
	{
		m_siTotal++;

		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		SI32 addlevel = 0;
		SI32 qualityrrate = pclitemmanager->GetQualityRate(m_siItemUnique, 1, &addlevel);

		SI32 RareRate[4];
		for ( int k = 0 ; k < 4 ; k ++ )	RareRate[k] = 0;

		cltItem clOutputItem;
		SI16 brareswitch = 0;

		// 아이템을 생성한다.
		if(pclClient->pclItemManager->MakeRandItemUnique(m_siItemUnique, &clOutputItem,
														qualityrrate , addlevel, &brareswitch,
														RareRate[0], RareRate[1], RareRate[2], RareRate[3], 0) == true)
		{
			if(brareswitch == 1 )			// 명품
			{
				clOutputItem.siMakerPersonID = 0;
				clOutputItem.clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_2;

				m_siRare1++;
			}
			else if ( brareswitch == 2 )	// 호품
			{
				clOutputItem.siMakerPersonID = 0;
				clOutputItem.clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_1;

				m_siRare2++;
			}
			else if ( brareswitch == 3 )	// 고급품
			{
				clOutputItem.siMakerPersonID = 0;
				clOutputItem.clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_5;

				m_siRare3++;
			}
		}

		TCHAR		szTotal[64];
		TCHAR		szRare1[64];
		TCHAR		szRare2[64];
		TCHAR		szRare3[64];

		StringCchPrintfA(szTotal, 64, TEXT("Total : %d"), m_siTotal);
		StringCchPrintfA(szRare1, 64, TEXT("Rare1 : %d"), m_siRare1);
		StringCchPrintfA(szRare2, 64, TEXT("Rare2 : %d"), m_siRare2);
		StringCchPrintfA(szRare3, 64, TEXT("Rare3 : %d"), m_siRare3);

		CStatic*	pStatic_Total = m_InterfaceFactory.GetStatic(NMAKEITEMDEMO_STATIC_TOTAL);
		CStatic*	pStatic_Rare1 = m_InterfaceFactory.GetStatic(NMAKEITEMDEMO_STATIC_RARE1);
		CStatic*	pStatic_Rare2 = m_InterfaceFactory.GetStatic(NMAKEITEMDEMO_STATIC_RARE2);
		CStatic*	pStatic_Rare3 = m_InterfaceFactory.GetStatic(NMAKEITEMDEMO_STATIC_RARE3);
        
		if(pStatic_Total)	pStatic_Total->SetText(szTotal);
		if(pStatic_Rare1)	pStatic_Rare1->SetText(szRare1);
		if(pStatic_Rare2)	pStatic_Rare2->SetText(szRare2);
		if(pStatic_Rare3)	pStatic_Rare3->SetText(szRare3);
	}
}
