
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserShipDlg.h"

#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/InterfaceMgr/Interface/Static.h"
#include "../Client/InterfaceMgr/Interface/Edit.h"
#include "../Client/InterfaceMgr/Interface/ListView.h"
#include "../Client/InterfaceMgr/Interface/Button.h"
#include "../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../Client/InterfaceMgr/Interface/List.h"
#include "../Client/InterfaceMgr/Interface/OutLine.h"

#include "../Client/InterfaceMgr/InterfaceFile.h"
#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../Client/Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"

//#include "../../FontManager/FontManager.h"
//#include "../../../lib/WebHTML/WebHTML.h"

#include "../../CommonLogic/MsgType-House.h"
#include "../../CommonLogic/Msg/MsgType-Port.h"
#include "../../CommonLogic/Port/Port.h"
#include "../../CommonLogic/Village/Village.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "../../Common/Ship/Ship.h"
#include "Char\CharManager\CharManager.h"


extern cltCommonLogic* pclClient;

CNHouseUserShipDlg::CNHouseUserShipDlg() : CDialogController()
{
//	m_pclHouseUnit = NULL;

	m_pclHouseUnitShip = new cltShip();
	memset( m_pclHouseUnitShip, 0, sizeof( cltShip ) );

	m_bShipBuildingCreated = false;
	m_siShipBuildingLevel = 0;
	m_siCurrentShipStepMake = 0;
	m_siCurrentShipKindMake = 0;
	m_siCurrentShipStepMakeDateVary = 0;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pCombo = NULL;
	m_pEdit = NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;
	m_pButton[3] = NULL;
}

CNHouseUserShipDlg::~CNHouseUserShipDlg()
{
	if ( m_pclHouseUnitShip )
	{
		delete m_pclHouseUnitShip;
		m_pclHouseUnitShip = NULL;
	}

	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pCombo);
	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[3]);
}

void CNHouseUserShipDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile("NInterface/Data/NStructureDlg/NHouseUserInfoDlg/DLG_HouseUserShip.ddf");
	file.CreateChildDialog( this, NHOUSEUSERSHIP_DLG, "dialog_HouseUserShip", StaticCallBackDialogNHouseUserShipDlg, pParent );

	NEWCSTATIC(m_pStatic[0]);
	NEWCSTATIC(m_pStatic[1]);
	NEWCSTATIC(m_pStatic[2]);
	NEWCCOMBOBOX(m_pCombo);
	NEWCEDIT(m_pEdit);
	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);
	NEWCBUTTON(m_pButton[2]);
	NEWCBUTTON(m_pButton[3]);

	file.CreateControl( m_pStatic[0], NHOUSEUSERSHIP_DIALOG_STATIC, "statictext_NONAME1" );
	file.CreateControl( m_pStatic[1], NHOUSEUSERITEM_DIALOG_STATIC, "statictext_NONAME2" );
	file.CreateControl( m_pStatic[2], NHOUSEUSERITEM_DIALOG_STATIC, "statictext_NONAME3" );
	file.CreateControl( m_pCombo, NHOUSEUSERSHIP_DIALOG_COMBOBOX, "combobox_NONAME1" );
	file.CreateControl( m_pEdit, NHOUSEUSERSHIP_DIALOG_EIDT, "editbox_NONAME1" );
	file.CreateControl( m_pButton[0], NHOUSEUSERSHIP_DIALOG_BUTTON1, "button_NONAME1" );
	file.CreateControl( m_pButton[1], NHOUSEUSERSHIP_DIALOG_BUTTON2, "button_NONAME2" );
	file.CreateControl( m_pButton[2], NHOUSEUSERSHIP_DIALOG_BUTTON3, "button_NONAME3" );
	file.CreateControl( m_pButton[3], NHOUSEUSERSHIP_DIALOG_BUTTON4, "button_NONAME4" );

	stComboBoxItemData Data;
	for ( SI16 i = 0; i < MAX_SHIP_KIND_NUMBER; ++i )
	{
		if ( pclClient->pclShipManager->pclShipBaseInfoList[ i ] )
		{
			char *ShipKindName = pclClient->pclShipManager->pclShipBaseInfoList[ i ]->GetShipKindName();
			if ( ShipKindName != NULL  )
			{
			//	SendMessage( hComboBox, CB_ADDSTRING, 0, (LPARAM)ShipKindName );
				
				Data.Init();
				Data.Set(ShipKindName);
				m_pCombo->AddItem(&Data);
			}
		}
	}

	m_pCombo->SetCursel(0);
	m_pEdit->SetText("");

	//	SendMessage( hComboBox, CB_SETCURSEL, 0, 0 );
/*
	char Buffer[ 512 ] = "";
	if ( pclClient->pclShipManager->pclShipBaseInfoList[ 0 ]->GetShipInfo( Buffer ) )
	{
		char NeedMaterialText[ 512 ] = "";
		if ( pclClient->pclShipManager->pclShipBaseInfoList[ 0 ]->GetShipNeedMaterialText( NeedMaterialText ) )
		{
			strcat ( Buffer, NeedMaterialText );
		//	SetWindowText( GetDlgItem ( hDlg, IDC_EDIT_HOUSEUNIT_SHIPEXPLAIN ), Buffer );
			m_pEdit->SetText(Buffer);
		}
		else
		{
		//	SetWindowText( GetDlgItem ( hDlg, IDC_EDIT_HOUSEUNIT_SHIPEXPLAIN ), Buffer );
			m_pEdit->SetText(Buffer);
		}
	}
*/
	Hide();
}

void CNHouseUserShipDlg::Hide()
{
	m_pEdit->SetText("");
	CControllerMgr::Show( false );
}

void CNHouseUserShipDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNHouseUserShipDlg::Action()
{
	// 시전 소유주가 아닌경우 타이머 돌 필요 없으므로...
	if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
	{
		return;
	}

	if ( ! IsShipDataChanged() )
	{
		return;
	}

	LoadShipData();
}

void CALLBACK CNHouseUserShipDlg::StaticCallBackDialogNHouseUserShipDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHouseUserShipDlg *pThis = (CNHouseUserShipDlg *)pControllerMgr;
	pThis->CallBackDialogNHouseUserShipDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHouseUserShipDlg::CallBackDialogNHouseUserShipDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;
	char Nbuffer[512]=" ";

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
		}
		break;

	case NHOUSEUSERSHIP_DIALOG_COMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					// SelectedIndex 는 Ref 순서로 들어가게 되어 있다. ( CB_ADDSTRING ) 을 그렇게 했음.
					SI32 SelectedIndex = m_pCombo->GetSelectedIndex();// SendMessage( GetDlgItem( hDlg, IDC_COMBO_SELECT_SHIP ), CB_GETCURSEL, 0, 0 );
					if (SelectedIndex < 0)
					{
						return;
					}

					char Buffer[ 512 ] = "";
					if ( pclClient->pclShipManager->pclShipBaseInfoList[ SelectedIndex ]->GetShipInfo( Buffer ) )
					{
						char NeedMaterialText[ 512 ] = "";
						if ( pclClient->pclShipManager->pclShipBaseInfoList[ SelectedIndex ]->GetShipNeedMaterialText( NeedMaterialText ) )
						{
							strcat ( Buffer, NeedMaterialText );
						//	SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_EDIT_HOUSEUNIT_SHIPEXPLAIN ), Buffer );
							m_pEdit->SetText(Buffer);
						}
						else
						{
						//	SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_EDIT_HOUSEUNIT_SHIPEXPLAIN ), Buffer );
							m_pEdit->SetText(Buffer);
						}
					}
				}
				break;
			}
		}
		break;

	case NHOUSEUSERSHIP_DIALOG_BUTTON1:	// 조선소 짓기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return;
					}

					if( pStgInfo->siHouseUnitShipBuildingLevel == 6)
					{
						char* pTitle = GetTxtFromMgr(1203);
						char* pText = GetTxtFromMgr(3274);


						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return ;

					}
					// 소유주가 틀린경우 그냥 return 함
					if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
					{
						char* pTitle = GetTxtFromMgr(370);
						char* pText = GetTxtFromMgr(1196);


						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return ;
					}

					if ( pStgInfo->bCreatedHouseUnitShipBuilding )
					{
						// 조선소 개조
						SI32 ItemPos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos( 14080 );

						char* pTitle = NULL;
						char* pText = NULL;

						if ( ItemPos < 0 || pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ ItemPos ].siItemNum < HOUSEUNIT_UPGRADESHIPBUILDING_USEITEMNUM )
						{
							pTitle = GetTxtFromMgr(1203);
							pText = GetTxtFromMgr(1204);

							char num[128]= " " ;
							sprintf(num,"%d",HOUSEUNIT_UPGRADESHIPBUILDING_USEITEMNUM);


							Nsprintf(Nbuffer,pText,"item", pclClient->pclItemManager->GetName( 14080 )   ,"num",  num  ,NULL);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,Nbuffer);

							//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, pclClient->pclItemManager->GetName( 14080 ), HOUSEUNIT_UPGRADESHIPBUILDING_USEITEMNUM );
							return;
						}

						char FeeExplain[128] = "";

						pTitle = GetTxtFromMgr(1206);
						pText = GetTxtFromMgr(1205);

						char num[128]= " " ;							
						sprintf(num,"%d",HOUSEUNIT_UPGRADESHIPBUILDING_USEITEMNUM);
						Nsprintf(FeeExplain,pText,"item", pclClient->pclItemManager->GetName( 14080 )   ,"num", num   ,NULL);


						//sprintf( FeeExplain, pText, pclClient->pclItemManager->GetName( 14080 ), HOUSEUNIT_UPGRADESHIPBUILDING_USEITEMNUM );

						stMsgBox MsgBox;
						CControllerMgr *pParent = NULL;
						pParent = this->GetParentDialog();
						if(pParent == NULL ) return;
						MsgBox.Set( pParent, pTitle, FeeExplain, MSGBOX_TYPE_YESNO, 0 );
						pclclient->SetMsgBox( &MsgBox, NULL, 0 );
						//if( MessageBox(NULL, FeeExplain, pTitle, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

						//cltGameMsgRequest_HouseUnitShipBuildingUpgrade clGameMsgRequest_HouseUnitShipBuildingUpgrade( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
						//cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSHIPBUILDINGUPGRADE, sizeof(clGameMsgRequest_HouseUnitShipBuildingUpgrade), (char*)&clGameMsgRequest_HouseUnitShipBuildingUpgrade );
						//pclClient->SendMsgToServer((char*)&clMsg);
					}
					else
					{
						// 조선소 짓기
						SI32 ItemPos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos( 14080 );

						char* pTitle = NULL;
						char* pText = NULL;

						if ( ItemPos < 0 || pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ ItemPos ].siItemNum < HOUSEUNIT_CREATESHIPBUILDING_USEITEMNUM )
						{
							pTitle = GetTxtFromMgr(1207);
							pText = GetTxtFromMgr(1208);
							char num[128]= " " ;
							sprintf(num,"%d",HOUSEUNIT_CREATESHIPBUILDING_USEITEMNUM);
							Nsprintf(Nbuffer,pText,"item", pclClient->pclItemManager->GetName( 14080 )  ,"num", num   ,NULL);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,Nbuffer);



							//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, pclClient->pclItemManager->GetName( 14080 ), HOUSEUNIT_CREATESHIPBUILDING_USEITEMNUM );
							return;
						}


						char FeeExplain[128] = "";

						pTitle = GetTxtFromMgr(811);	// 조선소 짓기
						pText = GetTxtFromMgr(1209);
						char num[128]= " " ;
						sprintf(num,"%d",HOUSEUNIT_CREATESHIPBUILDING_USEITEMNUM);
						Nsprintf(FeeExplain,pText,"item",  pclClient->pclItemManager->GetName( 14080 )  ,"num", num   ,NULL);


						//sprintf( FeeExplain, pText, pclClient->pclItemManager->GetName( 14080 ), HOUSEUNIT_CREATESHIPBUILDING_USEITEMNUM );						

						stMsgBox MsgBox;
						CControllerMgr *pParent = NULL;
						pParent = this->GetParentDialog();
						if(pParent == NULL ) return;
						MsgBox.Set( pParent, pTitle, FeeExplain, MSGBOX_TYPE_YESNO, 1 );
						pclclient->SetMsgBox( &MsgBox, NULL, 0 );
						//if( MessageBox(NULL, FeeExplain, pTitle, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

						//cltGameMsgRequest_HouseUnitShipBuildingCreate clGameMsgRequest_HouseUnitShipBuildingCreate( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
						//cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSHIPBUILDINGCREATE, sizeof(clGameMsgRequest_HouseUnitShipBuildingCreate), (char*)&clGameMsgRequest_HouseUnitShipBuildingCreate );
						//pclClient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			}
		}
		break;

	case NHOUSEUSERSHIP_DIALOG_BUTTON2:	// 배 건조하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 SelectedShipRef = m_pCombo->GetSelectedIndex();// SendMessage( GetDlgItem ( m_hHouseUnitShipDlg, IDC_COMBO_SELECT_SHIP ), CB_GETCURSEL, 0, 0 );

					if ( SelectedShipRef < 0 )
					{
						return;
					}

					char ShipName[128] = "";
				//	SendMessage( GetDlgItem( hDlg, IDC_COMBO_SELECT_SHIP ), CB_GETLBTEXT, SelectedShipRef, (LPARAM)ShipName );
					MStrCpy(ShipName, m_pCombo->GetText(SelectedShipRef), 128);

					if ( pclClient->pclShipManager->pclShipBaseInfoList[ SelectedShipRef ] == NULL )
					{
						return;
					}

					if ( pclClient->pclShipManager->pclShipBaseInfoList[ SelectedShipRef ]->m_clShipBaseInfo.siKind < 1 )
					{
						return;
					}

					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return;
					}

					if ( ! pStgInfo->bCreatedHouseUnitShipBuilding )
					{
						return;
					}


					SI32 Ref = 0;

					if ( pStgInfo->siCurrentShipKindMake > 0 )
					{
						if ( pStgInfo->siCurrentShipKindMake !=  pclClient->pclShipManager->pclShipBaseInfoList[ SelectedShipRef ]->m_clShipBaseInfo.siKind )
						{
							return;
						}

						m_siCurrentShipKindMake = pStgInfo->siCurrentShipKindMake;

						Ref = pclClient->pclShipManager->GetRefFromShipKind( m_siCurrentShipKindMake );

						if ( Ref < 0 )
						{
							return;
						}
					}
					else
					{
						Ref = pclClient->pclShipManager->GetRefFromShipName( ShipName );

						if ( Ref < 0 )
						{
							return;
						}

						m_siCurrentShipKindMake = pclClient->pclShipManager->pclShipBaseInfoList[ Ref ]->m_clShipBaseInfo.siKind;
					}

					cltClient *pclclinet = ( cltClient * )pclClient;

					SI32 CurrentDateVary = pclclinet->clClientDate.GetDateVary();

					if ( CurrentDateVary - pStgInfo->siShipStepMakeDateVary < 1 )
					{
						char* pTitle = GetTxtFromMgr(211);
						char* pText = GetTxtFromMgr(1212);


						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					//if ( pStgInfo->siCurrentShipStepMake < pclClient->pclShipManager->pclShipBaseInfoList[ Ref ]->m_siNeedWork )
					//{
					// 배 건조시에 단계별로 필요한 아이템 체크

					cltMaterialInfo *pShipMaterialInfoListPtr = pclClient->pclShipManager->GetShipMaterialInfoListPtr( pclClient->pclShipManager->pclShipBaseInfoList[ SelectedShipRef ]->m_clShipBaseInfo.siKind );

					if ( pShipMaterialInfoListPtr == NULL )
					{
						return;
					}

					SI16 StgItemPosList[ MAX_SHIP_MATERIAL_NUMBER ];
					SI16 StgItemUniqueList[ MAX_SHIP_MATERIAL_NUMBER ];
					SI16 StgItemNumList[ MAX_SHIP_MATERIAL_NUMBER ];

					memset( StgItemPosList, 0, sizeof( SI16 ) * MAX_SHIP_MATERIAL_NUMBER );
					memset( StgItemUniqueList, 0, sizeof( SI16 ) * MAX_SHIP_MATERIAL_NUMBER );
					memset( StgItemNumList, 0, sizeof( SI16 ) * MAX_SHIP_MATERIAL_NUMBER );


					for ( SI16 i = 0; i < MAX_SHIP_MATERIAL_NUMBER; ++i )
					{
						if ( pShipMaterialInfoListPtr[ i ].siUnique > 0 )
						{
							StgItemPosList[ i ] = m_pclHouseUnit->clStorageInfo.GetStgExistItemPos( pShipMaterialInfoListPtr[ i ].siUnique, pShipMaterialInfoListPtr[ i ].siNeedNum );

							// 제조시 아이템 부족
							if ( StgItemPosList[ i ] < 0 )
							{
								char* pTitle = GetTxtFromMgr(211);
								char* pText = GetTxtFromMgr(1213);


								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
								return;
							}

							StgItemUniqueList[ i ] = pShipMaterialInfoListPtr[ i ].siUnique;
							StgItemNumList[ i ] = pShipMaterialInfoListPtr[ i ].siNeedNum;
						}
					}

					cltGameMsgRequest_HouseUnitShipStepMake clGameMsgRequest_HouseUnitShipStepMake( m_siCurrentVillageIndex, m_siCurrentHouseIndex, pclClient->pclShipManager->pclShipBaseInfoList[ SelectedShipRef ]->m_clShipBaseInfo.siKind, pStgInfo->siCurrentShipStepMake );
					cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSHIPSTEPMAKE, sizeof(clGameMsgRequest_HouseUnitShipStepMake), (char*)&clGameMsgRequest_HouseUnitShipStepMake );
					pclClient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;

	case NHOUSEUSERSHIP_DIALOG_BUTTON3:	// 배 진수 하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 소유주가 틀린경우 그냥 return 함

					char* pTitle = NULL;
					char* pText = NULL;

					if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
					{
						pTitle = GetTxtFromMgr(370);
						pText = GetTxtFromMgr(1196);


						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return ;
					}

					if ( pStgInfo->clShip.siShipID == 0 )
					{
						return;
					}

					/*
					if ( pclClient->pclPortManager->clClientStrInfo.clPortStrInfo.clRentContract.siRentPeriod < 1 )
					{						
					return FALSE;
					}
					*/

					char szExplain[ 256 ] = "";

					pTitle = GetTxtFromMgr(1211);
					pText = GetTxtFromMgr(1210);
					char month[128]= " " ;
					char money[128]= " " ;


					sprintf(month,"%d",pclClient->pclPortManager->clClientStrInfo.clPortStrInfo.clRentContract.siRentPeriod);
					sprintf(money,"%d",pclClient->pclPortManager->clClientStrInfo.clPortStrInfo.clRentContract.siRentFee);
					//sprintf ( szExplain, pText, pclClient->pclPortManager->clClientStrInfo.clPortStrInfo.clRentContract.siRentPeriod, pclClient->pclPortManager->clClientStrInfo.clPortStrInfo.clRentContract.siRentFee );
					Nsprintf(szExplain,pText,"month",  month  ,"money",  money  ,NULL);

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pTitle, szExplain, MSGBOX_TYPE_YESNO, 2 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
					//if ( MessageBox( NULL, szExplain, pTitle, MB_YESNO | MB_ICONQUESTION ) == IDYES )
					//{
					//	cltGameMsgRequest_HouseUnitShipLaunching clGameMsgRequest_HouseUnitShipLaunching( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
					//	cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSHIPLAUNCHING, sizeof(clGameMsgRequest_HouseUnitShipLaunching), (char*)&clGameMsgRequest_HouseUnitShipLaunching );
					//	pclClient->SendMsgToServer((char*)&clMsg);
					//}
				}
				break;
			}
		}
		break;

	case NHOUSEUSERSHIP_DIALOG_BUTTON4:	// 배 건조 취소
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return;
					}

					if ( ! pStgInfo->bCreatedHouseUnitShipBuilding )
					{
						return;
					}

					if ( pStgInfo->siCurrentShipStepMake < 1 )
					{
						return;
					}

					SI32 Ref = pclClient->pclShipManager->GetRefFromShipKind( pStgInfo->siCurrentShipKindMake );

					if ( Ref < 0 )
					{
						return;
					}

					char *pTitle = GetTxtFromMgr(5057);
					char *pText  = GetTxtFromMgr(5058);

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 3 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

					//if( MessageBox(NULL, "배 건조를 취소하시겠습니까?", "배 건조 취소", MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

					//cltGameMsgRequest_HouseUnitShipMakeCancel clGameMsgRequest_HouseUnitShipMakeCancel( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
					//cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSHIPMAKECANCEL, sizeof(clGameMsgRequest_HouseUnitShipMakeCancel), (char*)&clGameMsgRequest_HouseUnitShipMakeCancel );
					//pclClient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;
	}
}

void CNHouseUserShipDlg::EnableButton(CButton* pButton, BOOL Flag)
{
	if(Flag)
	{
		pButton->SetFontColor(RGB(0, 0, 0));
		pButton->Enable(true);
	}
	else
	{
		pButton->SetFontColor(RGB(225, 225, 225));
		pButton->Enable(false);
	}
}

void CNHouseUserShipDlg::SetHouseUserItem(cltHouseUnit *pclHouseUnit)
{
	m_pclHouseUnit	  = pclHouseUnit;
}

void CNHouseUserShipDlg::SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex)
{
	m_siCurrentVillageIndex = siCurrentVillageIndex;
	m_siCurrentHouseIndex = siCurrentHouseIndex;
}

bool CNHouseUserShipDlg::IsShipDataChanged()
{	
	if ( m_pclHouseUnit == NULL )
	{
		return false;
	}

	cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
	if ( pStgInfo == NULL )
	{
		return false;
	}

	if ( m_bShipBuildingCreated != m_pclHouseUnit->clStorageInfo.bCreatedHouseUnitShipBuilding )
	{
		return true;
	}

	if ( m_siShipBuildingLevel != m_pclHouseUnit->clStorageInfo.siHouseUnitShipBuildingLevel )
	{
		return true;
	}

	if ( m_siCurrentShipKindMake != m_pclHouseUnit->clStorageInfo.siCurrentShipKindMake )
	{
		return true;
	}

	if ( m_siCurrentShipStepMake != m_pclHouseUnit->clStorageInfo.siCurrentShipStepMake )
	{
		return true;
	}

	if ( m_siCurrentShipStepMakeDateVary != m_pclHouseUnit->clStorageInfo.siShipStepMakeDateVary )
	{
		return true;
	}

	SI32 Size = sizeof( cltShip );

	char *src = (char *)&m_pclHouseUnit->clStorageInfo.clShip;
	char *dest = (char *)m_pclHouseUnitShip;

	for( int j = 0; j < Size; ++j )
	{
		if( src[j] != dest[j] )
		{
			return true;
		}
	}

	return false;
}

void CNHouseUserShipDlg::LoadShipData()
{	
	if ( m_pclHouseUnit == NULL )
	{
		return;
	}

	SI32 CurrentVillageUnique = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();

	if ( CurrentVillageUnique < 1 )
	{
		return;
	}

	if ( pclClient->pclVillageManager->pclVillageInfo[ CurrentVillageUnique ] == NULL )
	{
		return;
	}

	// 포구가 있는 마을인지 체크 해야 함.
	if ( ! pclClient->pclVillageManager->pclVillageInfo[ CurrentVillageUnique ]->bPortSwitch )
	{
		return;
	}

	//pclPortManager->clClientStrInfo.clPortStrInfo.port


	// 소유주가 틀린경우 그냥 return 함
	if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
	{
		return;
	}

	m_bShipBuildingCreated = m_pclHouseUnit->clStorageInfo.bCreatedHouseUnitShipBuilding;
	m_siShipBuildingLevel = m_pclHouseUnit->clStorageInfo.siHouseUnitShipBuildingLevel;
	m_pclHouseUnitShip->Set( &m_pclHouseUnit->clStorageInfo.clShip );
	m_siCurrentShipKindMake = m_pclHouseUnit->clStorageInfo.siCurrentShipKindMake;
	m_siCurrentShipStepMake = m_pclHouseUnit->clStorageInfo.siCurrentShipStepMake;
	m_siCurrentShipStepMakeDateVary = m_pclHouseUnit->clStorageInfo.siShipStepMakeDateVary;

	if ( m_bShipBuildingCreated )
	{
		char szShipBuildingLevel[ 32 ] = "";
		char* pText2 = GetTxtFromMgr(1186);

		sprintf( szShipBuildingLevel, pText2, m_siShipBuildingLevel );
	//	SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPBUILDINGCREATE ), szShipBuildingLevel );
		m_pButton[0]->SetText(szShipBuildingLevel, DT_CENTER | DT_VCENTER | DT_SINGLELINE); // 배 건조 레벨 표시
		//EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPBUILDINGCREATE ), false );

		// 배가 건조되어 있지 않은 상태( 건조 단계에 있는 상태도 포함 )
		if ( m_pclHouseUnitShip->siShipID == 0 )
		{			
		//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPMAKESTEP ), true );
		//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPLAUNCHING ), false );
			EnableButton(m_pButton[1], TRUE);
			EnableButton(m_pButton[2], FALSE);

			if ( m_siCurrentShipStepMake > 0 )
			{
				char Text[ 128 ] = "";
				pText2 = GetTxtFromMgr(1187);

				sprintf( Text, pText2, m_siCurrentShipStepMake + 1 );
		//		SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPMAKESTEP ), Text );
				m_pButton[1]->SetText(Text, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


				cltDate clDate;
				clDate.MakeFullDateFromDateVary( m_siCurrentShipStepMakeDateVary );

				char Buffer[ 1024 ] = "";
				char *ShipKindName = pclClient->pclShipManager->GetShipKindName( m_siCurrentShipKindMake );

				SI32 Ref = pclClient->pclShipManager->GetRefFromShipKind( m_siCurrentShipKindMake );
				if ( Ref > -1 )
				{
					if ( ShipKindName != NULL )
					{
						char DateText[128] = "";

						if( clDate.GetDateText(DateText) == FALSE)
						{
							strcpy( DateText, "" );
						}

						char ShipInfoText[ 128 ] = "";
						if ( ! pclClient->pclShipManager->pclShipBaseInfoList[ Ref ]->GetShipInfo( ShipInfoText ) )
						{
							strcpy( ShipInfoText, "" );
						}


						SI32 MakeShipSkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_MAKESHIP, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0] );
						SI32 MaxShipStepMake = 0;

						char MakeStep[ 64 ] = "";

						if ( MakeShipSkillLevel < 1 )
						{
							pText2 = GetTxtFromMgr(1188);

							strcpy( MakeStep, pText2 );
						}
						else
						{
							if ( MakeShipSkillLevel < pclClient->pclShipManager->pclShipBaseInfoList[ Ref ]->m_siNeedMakeSkillLevel )
							{
								pText2 = GetTxtFromMgr(1188);

								strcpy( MakeStep, pText2 );
							}
							else
							{
								//SI16 DecreaseStep = (SI16) ( ( MakeShipSkillLevel - pclClient->pclShipManager->pclShipBaseInfoList[ Ref ]->m_siNeedMakeSkillLevel ) * INCREASE_MAKESHIP_EASY_FROM_SKILL_LEVEL );

								// + 1 을 하는 이유는 MakeShipSkillLevel - pclShipManager->pclShipBaseInfoList[ Ref ]->m_siNeedMakeSkillLevel
								// 결과가 0 이면 무조건 DecreaseStep 이 1 이 됨으로.
								SI16 DecreaseStep = (SI16) ( log( (double)(MakeShipSkillLevel - pclClient->pclShipManager->pclShipBaseInfoList[ Ref ]->m_siNeedMakeSkillLevel + 1) )
									/log( (double)(INCREASE_MAKESHIP_EASY_FROM_SKILL_LEVEL_LOGX) ) );

								MaxShipStepMake = pclClient->pclShipManager->pclShipBaseInfoList[ Ref ]->m_siNeedWork - DecreaseStep;

								if ( MaxShipStepMake < 1 )
								{
									MaxShipStepMake = 1;
								}

								pText2 = GetTxtFromMgr(1189);


								sprintf( MakeStep, pText2, m_siCurrentShipStepMake, MaxShipStepMake );
							}
						}

						m_pCombo->SetCursel(Ref);
						m_pCombo->Enable(FALSE);

						char NeedMeterialText[ 256 ] = "";
						if ( pclClient->pclShipManager->GetShipMakeMetrialList( m_siCurrentShipKindMake, NeedMeterialText ) )
						{
							pText2 = GetTxtFromMgr(1190);

							sprintf( Buffer, pText2,
								ShipKindName,
								MakeStep,
								ShipInfoText,
								NeedMeterialText,
								DateText
								);

						//	SetWindowText( GetDlgItem( m_hHouseUnitShipDlg, IDC_EDIT_HOUSEUNIT_SHIPEXPLAIN ), Buffer );
							m_pEdit->SetText(Buffer);
						}
					}

				//	SendMessage( GetDlgItem( m_hHouseUnitShipDlg, IDC_COMBO_SELECT_SHIP ), CB_SETCURSEL, Ref, 0 );
				//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_COMBO_SELECT_SHIP ), false );
					
				}
			}
			else
			{
			//	SI32 SelectedIndex = SendMessage( GetDlgItem( m_hHouseUnitShipDlg, IDC_COMBO_SELECT_SHIP ), CB_GETCURSEL, 0, 0 );
				SI32 SelectedIndex = m_pCombo->GetSelectedIndex();
				if (SelectedIndex == CB_ERR)
				{
					return;
				}

				char Buffer[ 512 ] = "";
				if ( pclClient->pclShipManager->pclShipBaseInfoList[ SelectedIndex ]->GetShipInfo( Buffer ) )
				{
					char NeedMaterialText[ 512 ] = "";
					if ( pclClient->pclShipManager->pclShipBaseInfoList[ SelectedIndex ]->GetShipNeedMaterialText( NeedMaterialText ) )
					{
						strcat ( Buffer, NeedMaterialText );
					//	SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_EDIT_HOUSEUNIT_SHIPEXPLAIN ), Buffer );
						m_pEdit->SetText(Buffer);
					}
					else
					{
					//	SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_EDIT_HOUSEUNIT_SHIPEXPLAIN ), Buffer );
						m_pEdit->SetText(Buffer);
					}
				}

				char* pText = GetTxtFromMgr(1191);

			//	SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPMAKESTEP ), pText);
				m_pButton[1]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_COMBO_SELECT_SHIP ), true );
				m_pCombo->Enable(TRUE);

			//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPMAKESTEP ), true );
			//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPLAUNCHING ), false );
				EnableButton(m_pButton[1], TRUE);	// 배건조 버튼
				EnableButton(m_pButton[2], FALSE);	// 배진수 하기 버튼
			}
		}
		// 어떤 배가 건조되어 있는 상태
		else
		{
			char Buffer[ 256 ] = "";
			if ( pclClient->pclShipManager->GetText( m_pclHouseUnitShip, Buffer ) )
			{
			//	SetWindowText( GetDlgItem( m_hHouseUnitShipDlg, IDC_EDIT_HOUSEUNIT_SHIPEXPLAIN ), Buffer );
				m_pEdit->SetText(Buffer);
			}

			pText2 = GetTxtFromMgr(1191);

			//char Text[ 128 ] = "배 건조하기";
		//	SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPMAKESTEP ), pText2 );
		//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_COMBO_SELECT_SHIP ), false );
		//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPMAKESTEP ), false );
		//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPLAUNCHING ), true );

			m_pButton[1]->SetText(pText2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			m_pCombo->Enable(FALSE);
			EnableButton(m_pButton[1], FALSE);
			EnableButton(m_pButton[2], TRUE);
		}
	}
	else
	{
		char* pText2 = GetTxtFromMgr(1191);


		//char Text[ 128 ] = "배 건조하기";
	//	SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPMAKESTEP ), pText2 );
		m_pButton[1]->SetText(pText2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_COMBO_SELECT_SHIP ), false );
		m_pCombo->Enable(FALSE);

		pText2 = GetTxtFromMgr(811); //조선소 짓기

	//	SetWindowText( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPBUILDINGCREATE ), pText2 );
		//EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPBUILDINGCREATE ), true );
	//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPLAUNCHING ), false );
	//	EnableWindow( GetDlgItem ( m_hHouseUnitShipDlg, IDC_BUTTON_HOUSEUNIT_SHIPMAKESTEP ), false );

		m_pButton[0]->SetText(pText2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		EnableButton(m_pButton[1], FALSE);
		EnableButton(m_pButton[2], FALSE);
	}

	// 조선소 건설되기 전
	// 조선소만 건설된 상태
	// 조선소 건설된 위에 배가 들어간 상태( 배 종류마다 틀리겠지... )
	// 조선소는 건설된 상태에서 배 만들다가 폭파한 상태 <-- 없어도 됨.

	cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
	if ( pStgInfo == NULL )
	{
		return;
	}

	if ( pStgInfo->bCreatedHouseUnitShipBuilding )
	{
		// 조선소에 배가 만들어졌는 경우
		if ( pStgInfo->clShip.siKind > 0 )
		{
			m_pStatic[2]->SetFontIndex(7); // 배그림이다
		/*	char strImagePath[ _MAX_PATH ] = "";
			GetCurrentDirectory( _MAX_PATH, strImagePath );

			int BufLen = strlen( strImagePath );

			char Buffer[256 ] = "";
			if( strImagePath [ BufLen - 1 ] == '\\' )
			{
				//sprintf( Buffer, "Interface\\ShipBuilding\\%d.html", pStgInfo->clShip.siKind );
				sprintf( Buffer, "Interface\\ShipBuilding\\%d.html", KIND_YELLOWBOAT );				
			}
			else
			{				
				//sprintf( Buffer, "\\Interface\\ShipBuilding\\%d.html", pStgInfo->clShip.siKind );
				sprintf( Buffer, "\\Interface\\ShipBuilding\\%d.html", KIND_YELLOWBOAT );
			}

			strcat( strImagePath, Buffer );

			//==================================
			// 화면에 보여준다.
			HWND hwnd = GetDlgItem( m_hHouseUnitShipDlg, IDC_STATIC_HOUSEUNIT_SHIPIMAGE );
			if(hwnd)
			{
				DisplayHTMLPage(hwnd, strImagePath);
			}
			//==================================*/
		}
		// 어떤 배를 건조중인 경우
		else if ( m_siCurrentShipKindMake > 0 )
		{
			m_pStatic[2]->SetFontIndex(7); // 배그림이다
		/*	char strImagePath[ _MAX_PATH ] = "";
			GetCurrentDirectory( _MAX_PATH, strImagePath );

			int BufLen = strlen( strImagePath );
			char Buffer[256 ] = "";
			if( strImagePath [ BufLen - 1 ] == '\\' )
			{
				//sprintf( Buffer, "Interface\\ShipBuilding\\%d.html", m_siCurrentShipKindMake );
				sprintf( Buffer, "Interface\\ShipBuilding\\%d.html", KIND_YELLOWBOAT );
			}
			else
			{				
				//sprintf( Buffer, "\\Interface\\ShipBuilding\\%d.html", m_siCurrentShipKindMake );
				sprintf( Buffer, "\\Interface\\ShipBuilding\\%d.html", KIND_YELLOWBOAT );
			}			

			strcat( strImagePath, Buffer );

			//==================================
			// 화면에 보여준다.
			HWND hwnd = GetDlgItem( m_hHouseUnitShipDlg, IDC_STATIC_HOUSEUNIT_SHIPIMAGE );
			if(hwnd)
			{
				DisplayHTMLPage(hwnd, strImagePath);
			}
			//==================================*/
		}
		// 조선소안에 아무것도 없는 경우.
		else
		{
			m_pStatic[2]->SetFontIndex(6); // 배그림이다
		/*	char strImagePath[ _MAX_PATH ] = "";

			GetCurrentDirectory( _MAX_PATH, strImagePath );

			int BufLen = strlen( strImagePath );

			if( strImagePath [ BufLen - 1 ] == '\\' )
			{
				strcat( strImagePath, "Interface\\ShipBuilding\\ShipBuilding.html" );
			}
			else
			{				
				strcat( strImagePath, "\\Interface\\ShipBuilding\\ShipBuilding.html" );
			}

			//==================================
			// 화면에 보여준다.
			HWND hwnd = GetDlgItem( m_hHouseUnitShipDlg, IDC_STATIC_HOUSEUNIT_SHIPIMAGE );
			if(hwnd)
			{
				DisplayHTMLPage(hwnd, strImagePath);
			}
			//==================================*/
		}
	}
	else
	{
		m_pStatic[2]->SetFontIndex(5); // 배그림이다
	/*	char strImagePath[ _MAX_PATH ] = "";
		GetCurrentDirectory( _MAX_PATH, strImagePath );

		int BufLen = strlen( strImagePath );

		if( strImagePath [ BufLen - 1 ] == '\\' )
		{
			strcat( strImagePath, "Interface\\ShipBuilding\\BeforeShipBuilding.html" );
		}
		else
		{				
			strcat( strImagePath, "\\Interface\\ShipBuilding\\BeforeShipBuilding.html" );
		}


		//==================================
		// 화면에 보여준다.
		HWND hwnd = GetDlgItem( m_hHouseUnitShipDlg, IDC_STATIC_HOUSEUNIT_SHIPIMAGE );
		if(hwnd)
		{
			DisplayHTMLPage(hwnd, strImagePath);
		}
		//==================================*/
	}

	return;
}

void CNHouseUserShipDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, char *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 
		{
			if( RetValue )
			{
				cltGameMsgRequest_HouseUnitShipBuildingUpgrade clGameMsgRequest_HouseUnitShipBuildingUpgrade( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSHIPBUILDINGUPGRADE, sizeof(clGameMsgRequest_HouseUnitShipBuildingUpgrade), (char*)&clGameMsgRequest_HouseUnitShipBuildingUpgrade );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
		}
		break;
	case 1: // 
		{
			if( RetValue )
			{
				cltGameMsgRequest_HouseUnitShipBuildingCreate clGameMsgRequest_HouseUnitShipBuildingCreate( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSHIPBUILDINGCREATE, sizeof(clGameMsgRequest_HouseUnitShipBuildingCreate), (char*)&clGameMsgRequest_HouseUnitShipBuildingCreate );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
		}
		break;
	case 2: // 
		{
			if( RetValue )
			{
				cltGameMsgRequest_HouseUnitShipLaunching clGameMsgRequest_HouseUnitShipLaunching( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSHIPLAUNCHING, sizeof(clGameMsgRequest_HouseUnitShipLaunching), (char*)&clGameMsgRequest_HouseUnitShipLaunching );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
		}
		break;
	case 3: // 
		{
			if( RetValue )
			{
				cltGameMsgRequest_HouseUnitShipMakeCancel clGameMsgRequest_HouseUnitShipMakeCancel( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSHIPMAKECANCEL, sizeof(clGameMsgRequest_HouseUnitShipMakeCancel), (char*)&clGameMsgRequest_HouseUnitShipMakeCancel );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
		}
		break;
	}
}
