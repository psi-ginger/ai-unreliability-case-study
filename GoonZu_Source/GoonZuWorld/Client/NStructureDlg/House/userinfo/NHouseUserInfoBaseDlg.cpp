#include "../Client/NStructureDlg/House/UserInfo/NHouseUserInfoBaseDlg.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserItemDlg.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserHorseDlg.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserSecondhandDlg.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserManagementDlg.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserNewManagementDlg.h"

#include "../Client/InterfaceMgr/InterfaceMgr.h"


#include "../Client/InterfaceMgr/Interface/Static.h"
#include "../Client/InterfaceMgr/Interface/Edit.h"
#include "../Client/InterfaceMgr/Interface/ListView.h"
#include "../Client/InterfaceMgr/Interface/Button.h"
#include "../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../Client/InterfaceMgr/Interface/List.h"

#include "../Client/InterfaceMgr/InterfaceFile.h"
#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../Client/Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include <CommonLogic.h>

#include "../common/Char/CharCommon/Char-Common.h"

#include "../Client/Interface/HouseUnit/HouseUnitBuffer.h"

//#include "../../FontManager/FontManager.h"


#include "../CommonLogic/MsgType-House.h"
#include "../CommonLogic/Village/Village.h"

extern cltCommonLogic* pclClient;

CNHouseUserInfoBaseDlg::CNHouseUserInfoBaseDlg() : CDialogController()
{
	
	m_pHouseUnitBuffer = new CHouseUnitBuffer();

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;
	m_pButton[3] = NULL;

	m_pNHouseUserItemDlg = NULL;
	m_pNHouseUserHorseDlg = NULL;
	m_pNHouseUserSecondhandDlg = NULL;
	m_pNHouseUserManagement = NULL;
	m_pNHouseUserNewManagement = NULL;
}

CNHouseUserInfoBaseDlg::~CNHouseUserInfoBaseDlg()
{
	SetTimerOnOff(FALSE);

	if ( m_pHouseUnitBuffer )
	{
		delete m_pHouseUnitBuffer;
		m_pHouseUnitBuffer = NULL;
	}

	SAFE_DELETE(m_pNHouseUserNewManagement);
	//SAFE_DELETE(m_pNHouseUserManagement);
	SAFE_DELETE(m_pNHouseUserSecondhandDlg);
	SAFE_DELETE(m_pNHouseUserHorseDlg);
	SAFE_DELETE(m_pNHouseUserItemDlg);

	SAFE_DELETE(m_pButton[3]);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[0]);
}

void CNHouseUserInfoBaseDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/NHouseUserInfoDlg/DLG_HouseUserInfoBase.ddf"));
		file.CreatePopupDialog( this, NHOUSEUSERINFOBASE_DLG, TEXT("dialog_HouseUserInfoBase"), StaticCallBackDialogNHouseUserInfoBaseDlg );

		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);
		//NEWCBUTTON(m_pButton[2]);
		NEWCBUTTON(m_pButton[3]);
		
		file.CreateControl( m_pButton[0], NHOUSEUSERINFOBASE_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButton[1], NHOUSEUSERINFOBASE_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		//file.CreateControl( m_pButton[2], NHOUSEUSERINFOBASE_DIALOG_BUTTON3, TEXT("button_NONAME3") );
		file.CreateControl( m_pButton[3], NHOUSEUSERINFOBASE_DIALOG_BUTTON4, TEXT("button_NONAME4") );

		
		m_pNHouseUserItemDlg = new CNHouseUserItemDlg();
		m_pNHouseUserItemDlg->Create( this );
		m_pNHouseUserItemDlg->SetHouseUnitBuffer(m_pHouseUnitBuffer);
		m_pNHouseUserItemDlg->Show();
		

		m_pNHouseUserHorseDlg = new CNHouseUserHorseDlg();
		m_pNHouseUserHorseDlg->Create( this );

		m_pNHouseUserSecondhandDlg = new CNHouseUserSecondhandDlg();
		m_pNHouseUserSecondhandDlg->Create( this );
		m_pNHouseUserSecondhandDlg->SetHouseUnitBuffer(m_pHouseUnitBuffer);

		if(pclClient->IsCountrySwitch(Switch_ChangeHouseList))
		{
			m_pNHouseUserNewManagement = new CNHouseUserNewManagementDlg();
			m_pNHouseUserNewManagement->Create( this );
			m_pNHouseUserNewManagement->SetHouseUnitBuffer(m_pHouseUnitBuffer);
		}
		else
		{
			m_pNHouseUserManagement = new CNHouseUserManagementDlg();
			m_pNHouseUserManagement->Create( this );
			m_pNHouseUserManagement->SetHouseUnitBuffer(m_pHouseUnitBuffer);
		}
		
		SetTimerOnOff(TRUE);
		

		//m_pButton[2]->Enable(false);

		SetButtonTextColor(0);	// 창고 활성 글씨
	}
	else
	{
		DeleteDialog();
	}
}

void CNHouseUserInfoBaseDlg::Action()
{
	if(m_siTimerIndex == 0)
		m_pNHouseUserItemDlg->Action();
	else if(m_siTimerIndex == 1)
		m_pNHouseUserHorseDlg->Action();
	else if(m_siTimerIndex == 2)
		m_pNHouseUserSecondhandDlg->Action();
	else if(m_siTimerIndex == 3)
	{
		if(pclClient->IsCountrySwitch(Switch_ChangeHouseList))
		{
			m_pNHouseUserNewManagement->Action();
		}
		else
		{
			m_pNHouseUserManagement->Action();
		}
	}
		
}

void CALLBACK CNHouseUserInfoBaseDlg::StaticCallBackDialogNHouseUserInfoBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHouseUserInfoBaseDlg *pThis = (CNHouseUserInfoBaseDlg *)pControllerMgr;
	pThis->CallBackDialogNHouseUserInfoBaseDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHouseUserInfoBaseDlg::CallBackDialogNHouseUserInfoBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NHOUSEUSERINFOBASE_DIALOG_BUTTON1:	// 창고
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_pNHouseUserItemDlg->SetHouseUserItem(m_pclHouseUnit);
					m_pNHouseUserItemDlg->LoadWareHouseInventory();

					m_siTimerIndex = 0; 
					SetButtonTextColor(0);
					m_pNHouseUserItemDlg->Show();
					m_pNHouseUserHorseDlg->Hide();
					m_pNHouseUserSecondhandDlg->Hide();
					if(pclClient->IsCountrySwitch(Switch_ChangeHouseList))
					{
						m_pNHouseUserNewManagement->Hide();
					}
					else
					{
						m_pNHouseUserManagement->Hide();
					}
				}
				break;
			}
		}
		break;

	case NHOUSEUSERINFOBASE_DIALOG_BUTTON2:	// 마구간
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_pNHouseUserHorseDlg->SetHouseUserItem(m_pclHouseUnit);
					m_pNHouseUserHorseDlg->LoadHorseList();

					m_siTimerIndex = 1; 
					SetButtonTextColor(1);
					m_pNHouseUserItemDlg->Hide();
					m_pNHouseUserHorseDlg->Show();
					m_pNHouseUserSecondhandDlg->Hide();
					if(pclClient->IsCountrySwitch(Switch_ChangeHouseList))
					{
						m_pNHouseUserNewManagement->Hide();
					}
					else
					{
						m_pNHouseUserManagement->Hide();
					}
				}
				break;
			}
		}
		break;

	//case NHOUSEUSERINFOBASE_DIALOG_BUTTON3:	// 고물상
	//	{
	//		switch( nEvent )
	//		{
	//		case EVENT_BUTTON_CLICKED:
	//			{
	//				// 고물 불러오기
	//				m_pNHouseUserSecondhandDlg->SetHouseUserItem(m_pclHouseUnit); 
	//				m_pNHouseUserSecondhandDlg->LoadWareHouseInventory1();

	//				m_siTimerIndex = 2; 
	//				SetButtonTextColor(2);
	//				m_pNHouseUserItemDlg->Hide();
	//				m_pNHouseUserHorseDlg->Hide();
	//				m_pNHouseUserSecondhandDlg->Show();
	//				m_pNHouseUserManagement->Hide();
	//			}
	//			break;
	//		}
	//	}
	//	break;

	case NHOUSEUSERINFOBASE_DIALOG_BUTTON4:	// 관리
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siTimerIndex = 3; 
					SetButtonTextColor(3);
					m_pNHouseUserItemDlg->Hide();
					m_pNHouseUserHorseDlg->Hide();
					m_pNHouseUserSecondhandDlg->Hide();
					if(pclClient->IsCountrySwitch(Switch_ChangeHouseList))
					{
						m_pNHouseUserNewManagement->LoadMyHouseList();
						m_pNHouseUserNewManagement->Show();
					}
					else
					{
						m_pNHouseUserManagement->Show();
					}
				}
				break;
			}
		}
		break;
	}
}

bool CNHouseUserInfoBaseDlg::Pop( stHouseUnitData * pHouseUnitData )
{
	return m_pHouseUnitBuffer->Pop( pHouseUnitData );
}

void CNHouseUserInfoBaseDlg::SetPersonItemInfo(cltPItemInfo* pPersonItemInfo)
{ 
	m_pNHouseUserItemDlg->SetPersonItemInfo(pPersonItemInfo);
}

void CNHouseUserInfoBaseDlg::SetHouseUserInfoBase(cltHouseUnit *pclHouseUnit, cltPSkillInfo* pclskill, SI32 si32VillageIndex, SI32 si32HouseUnitSlot )
{
	m_pclHouseUnit			= pclHouseUnit;
	m_pclPersonSkillInfo	= pclskill;


//	m_siCurrentVillageIndex = si32VillageIndex;
//	m_siCurrentHouseIndex = si32HouseUnitSlot;

	// 창고
	m_pNHouseUserItemDlg->SetHouseUserItem(m_pclHouseUnit);
	m_pNHouseUserItemDlg->SetHousePositionInfo(si32VillageIndex, si32HouseUnitSlot); // 마을 인덱스, 시전 인덱스
	m_pNHouseUserItemDlg->LoadUserInventory();
	m_pNHouseUserItemDlg->LoadWareHouseInventory();
	
	// 마구간
	m_pNHouseUserHorseDlg->SetHousePositionInfo(si32VillageIndex, si32HouseUnitSlot);
	// 고물
	m_pNHouseUserSecondhandDlg->SetHousePositionInfo(si32VillageIndex, si32HouseUnitSlot);
	// 관리
	if(!pclClient->IsCountrySwitch(Switch_ChangeHouseList))
	{
		m_pNHouseUserManagement->SetHousePositionInfo(si32VillageIndex, si32HouseUnitSlot);
	}
	
	//---------------다이얼로그 창에 소유주가 누구인지 보여줄려고...
	TCHAR szString[ 128 ];
	memset( szString, 0, 128 );
	TCHAR* pText = GetTxtFromMgr(2148);
	StringCchCopy( szString, 128, pText);
	StringCchCat( szString, 128, pclHouseUnit->clPerson.GetName() );
//	SetWindowText( m_hDlg, szString );
	SetTitle(szString);
}

void CNHouseUserInfoBaseDlg::ShowMakeItemFromUsedGoodsItem(cltItem* pcltItem)
{
	m_pNHouseUserSecondhandDlg->ShowMakeItemFromUsedGoodsItem(pcltItem);
}

void CNHouseUserInfoBaseDlg::InitHouse()
{
	m_pNHouseUserItemDlg->SetHouseUserItem(m_pclHouseUnit);
	m_pNHouseUserItemDlg->LoadWareHouseInventory();

	m_siTimerIndex = 0; 
	SetButtonTextColor(0);
	m_pNHouseUserItemDlg->Show();
	m_pNHouseUserHorseDlg->Hide();
	m_pNHouseUserSecondhandDlg->Hide();
	if(pclClient->IsCountrySwitch(Switch_ChangeHouseList))
	{
		m_pNHouseUserNewManagement->Hide();
	}
	else
	{
		m_pNHouseUserManagement->Hide();
	}
}
void CNHouseUserInfoBaseDlg::SetButtonTextColor(SI32 Index)
{
	for( SI16 i = 0; i < 4; ++i )
	{
		if ( m_pButton[ i ] )
		{
			if ( i == Index )
			{
				m_pButton[ i ]->SetFontColor( CONTROLLER_BUTTONTEXT_COLOR  );
				m_pButton[ i ]->Refresh();
			}
			else
			{
				m_pButton[ i ]->SetFontColor( CONTROLLER_BACK_BUTTONTEXT_COLOR );
				m_pButton[ i ]->Refresh();
			}
		}
	}
}

void CNHouseUserInfoBaseDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 4:	// 마굿간 짓기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{

				SI16 i = *((SI32*)pData);

				cltGameMsgRequest_HouseUnitStableCreate clGameMsgRequest_HouseUnitStableCreate( m_pNHouseUserHorseDlg->m_siCurrentVillageIndex, m_pNHouseUserHorseDlg->m_siCurrentHouseIndex, i );
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSTABLECREATE, sizeof(clGameMsgRequest_HouseUnitStableCreate), (BYTE*)&clGameMsgRequest_HouseUnitStableCreate );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				 m_pNHouseUserHorseDlg->m_siSelectHorseIndex = -1;
			}
		}
		break;
	case 5:	// 고물상 짓기
		{
			if( RetValue )
			{
				cltGameMsgRequest_HouseUnitUsedGoodsBuildingCreate clGameMsgRequest_HouseUnitUsedGoodsBuildingCreate( m_pNHouseUserSecondhandDlg->m_siCurrentVillageIndex, m_pNHouseUserSecondhandDlg->m_siCurrentHouseIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITUSEDGOODSBUILDINGCREATE, sizeof(clGameMsgRequest_HouseUnitUsedGoodsBuildingCreate), (BYTE*)&clGameMsgRequest_HouseUnitUsedGoodsBuildingCreate );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 6: // 물품 해체
		{
			if( RetValue )
			{
				cltGameMsgRequest_HouseUnitMakeItemFromUsedGoods clGameMsgRequest_HouseUnitMakeItemFromUsedGoods(m_pNHouseUserSecondhandDlg->m_siCurrentVillageIndex, m_pNHouseUserSecondhandDlg->m_siCurrentHouseIndex, m_pNHouseUserSecondhandDlg->m_siUsedGoodsItemRealSelectedIndex, &m_pNHouseUserSecondhandDlg->m_clWareHouseItem[ m_pNHouseUserSecondhandDlg->m_siUsedGoodsItemRealSelectedIndex ] );
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODS, sizeof(clGameMsgRequest_HouseUnitMakeItemFromUsedGoods), (BYTE*)&clGameMsgRequest_HouseUnitMakeItemFromUsedGoods );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 7:	// 계약 연장
		{
			if( RetValue )
			{
				if(pclClient->IsCountrySwitch(Switch_ChangeHouseList))
				{
					m_pHouseUnitBuffer->Push( HOUSEUNIT_RETAL_EXTENSION, pclClient->pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract, m_pNHouseUserNewManagement->m_siCurrentVillageIndex, m_pNHouseUserNewManagement->m_siCurrentHouseIndex, NULL );
				}
				else
				{
					m_pHouseUnitBuffer->Push( HOUSEUNIT_RETAL_EXTENSION, pclClient->pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract, m_pNHouseUserManagement->m_siCurrentVillageIndex, m_pNHouseUserManagement->m_siCurrentHouseIndex, NULL );
				}
			}
		}
		break;
	}
}

CNHouseUserNewManagementDlg* CNHouseUserInfoBaseDlg::GetNHouseUserNewManagementDlg()
{
	return m_pNHouseUserNewManagement;
}