
/* ==========================================================================
	클래스 :		CNFarmingDlg

	작성일 :		05/03/21
	작성자 :		김태양

	목  적 :		CNFarmingDlg 클래스는  전체 화면 모드 실행시 NInterface에
					따라 게임 화면 안에서 몬스터 정보 다이얼로그를 생성한다.

	용  법 :		Client.h에 선언된 CNFarmingDlg			*m_pNFarmingDlg로
					컨트롤 한다.

	변경사항 :		없음

	차후작업 :		보류 
   ========================================================================*/

#include "NFarmingDlg.h"

#include "../../Client/client.h"
#include "..\CommonLogic\MsgType.h"
#include "..\CommonLogic\MsgType-System.h"
#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "../../CommonLogic/Msg/MsgType-Agriculture.h"
#include "../../CommonLogic/Msg/MsgType-Farming.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "../../Common/Farming/FarmingMgr.h"

#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/RadioButton.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"



#include "../../Music/Music.h"
#include "../../interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../../lib/WebHTML/WebHTML.h"

#include <wininet.h>

#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;
cltMsg* farmpclMsg;

	
CNFarmingDlg::CNFarmingDlg()
{
	m_bShow = false;
	m_bAutoFarming = false;
	m_pNFarmingItemUniqueListPtr = NULL;
	m_siFarmingItemUnique = 0;
	m_siDelayTime = 0;

    m_pNFarmingImageStatic = NULL;
	m_pNFarmingCombo = NULL;
	m_pNFarmingButton = NULL;
	m_pNFarmingEdit = NULL;
	m_pNFarmingStatic1 = NULL;
	m_pNFarmingStatic2 = NULL;
	m_pNFarmingStatic3 = NULL;
	m_pNFarmInfoEdit = NULL;
	m_pOutline = NULL;

	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		m_bStopFarming = TRUE;
	}
	else
	{
		m_bStopFarming = FALSE;
	}

	m_uiLeaseLeftTime = 0;
	m_siSuccessPercent = 0;
	m_siNowFontNum = 0;

	GP.LoadSprFromFile( TEXT("GImg\\MakeItemDrawPart1.Spr"),	&m_SprFarmGage );
	memset(m_strFarmName, 0, sizeof(m_strFarmName));
	memset(m_siFarmNum, 0, sizeof(m_siFarmNum));
}

CNFarmingDlg::~CNFarmingDlg()
{
	SAFE_DELETE(m_pNFarmingImageStatic);
	SAFE_DELETE(m_pNFarmingCombo);
	SAFE_DELETE(m_pNFarmingButton);
	SAFE_DELETE(m_pNFarmingEdit);
	SAFE_DELETE(m_pNFarmingStatic1);
	SAFE_DELETE(m_pNFarmingStatic2);	
	SAFE_DELETE(m_pNFarmingStatic3);	
	SAFE_DELETE(m_pNFarmInfoEdit);
	SAFE_DELETE(m_pOutline);

	NDelete_Array(m_pNFarmingItemUniqueListPtr);
	SetTimerOnOff( false );

	if ( m_SprFarmGage.pImage )
	{
		GP.FreeSpr( m_SprFarmGage );
	}

	return;
}

void CNFarmingDlg::Init()
{
	memset(m_strFarmName, 0, sizeof(m_strFarmName));
	memset(m_siFarmNum, 0, sizeof(m_siFarmNum));
	m_pNFarmInfoEdit->Clear();
}

void CNFarmingDlg::Create()
{
	cltClient *pclclient=(cltClient *)pclClient; 
	TCHAR* pText= GetTxtFromMgr(3364);
	SetActionTimer( 500 );

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/DLG_NFarming.ddf"));
		file.CreatePopupDialog( this, NFARMING_DLG, TEXT("dialog_FarmingDlg"), StatiCNFarmingDlgProc);	
		
		if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
		{
			m_pNFarmingImageStatic = new CImageStatic( this );		
			m_pNFarmingButton= new CButton( this );
			m_pNFarmingEdit= new CEdit( this );
			m_pNFarmingStatic1 =  new CStatic( this );
			m_pNFarmingStatic2 =  new CStatic( this );
			m_pNFarmingCombo= new CComboBox( this );
			m_pNFarmingStatic3= new CStatic( this );
			m_pNFarmInfoEdit= new CEdit( this );
			m_pOutline = new COutLine( this );

			file.CreateControl( m_pNFarmingImageStatic, NFARMING_DIALOG_IMAGESTATIC, TEXT("imagestatic_FarmingImage") );		
			file.CreateControl( m_pNFarmingButton, NFARMING_DIALOG_BUTTON, TEXT("button_FarmingButton") );
			file.CreateControl( m_pNFarmingEdit, NFARMING_DIALOG_EDIT, TEXT("editbox_FarmingEdit") );
			file.CreateControl( m_pNFarmingStatic1,NFARMING_DIALOG_STATIC1, TEXT("statictext_FarmingStatic1") );
			file.CreateControl( m_pNFarmingStatic2,  NFARMING_DIALOG_STATIC2, TEXT("statictext_FarmingStatic2") );
			file.CreateControl( m_pNFarmingCombo, NFARMING_DIALOG_COMBO, TEXT("combobox_FarmingCombo") );
			file.CreateControl( m_pNFarmingStatic3, NFARMING_DIALOG_STATIC3, TEXT("statictext_NONAME1") );
			file.CreateControl( m_pNFarmInfoEdit, NFARMING_DIALOG_FARMINFO_EDIT, TEXT("editbox_NONAME1") );
			file.CreateControl( m_pOutline, NFARMING_DIALOG_OUTLINE, TEXT("outline_NONAME1"));
		}
		else
		{
			m_pNFarmingImageStatic = new CImageStatic( this );		
			m_pNFarmingButton= new CButton( this );
			m_pNFarmingEdit= new CEdit( this );
			m_pNFarmingStatic1 =  new CStatic( this );
			m_pNFarmingStatic2 =  new CStatic( this );
			m_pNFarmingCombo= new CComboBox( this );

			file.CreateControl( m_pNFarmingImageStatic, NFARMING_DIALOG_IMAGESTATIC, TEXT("imagestatic_FarmingImage") );		
			file.CreateControl( m_pNFarmingButton, NFARMING_DIALOG_BUTTON, TEXT("button_FarmingButton") );
			file.CreateControl( m_pNFarmingEdit, NFARMING_DIALOG_EDIT, TEXT("editbox_FarmingEdit") );
			file.CreateControl( m_pNFarmingStatic1,NFARMING_DIALOG_STATIC1, TEXT("statictext_FarmingStatic1") );
			file.CreateControl( m_pNFarmingStatic2,  NFARMING_DIALOG_STATIC2, TEXT("statictext_FarmingStatic2") );
			file.CreateControl( m_pNFarmingCombo, NFARMING_DIALOG_COMBO, TEXT("combobox_FarmingCombo") );
		}

		pclClient->pclItemManager->GetFarmItemList( &m_siFarmingItemNum, &m_pNFarmingItemUniqueListPtr );
	
		stComboBoxItemData tmpComboBoxItemData;	
			for ( SI16 i = 0; i < m_siFarmingItemNum; ++i )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pNFarmingItemUniqueListPtr[i] );		

				m_pNFarmingCombo->GetItemData(NFARMING_DIALOG_COMBO  );					
				tmpComboBoxItemData.Init();
				tmpComboBoxItemData.Set( ItemName );
				m_pNFarmingCombo->AddItem( &tmpComboBoxItemData );

			}
			 Hide();			
	}

  	if ((pclclient->m_pDialog[ NFARMING_DLG ]) != NULL )							
   {
  	   Show();        
   }
  
	return;
}

void CALLBACK CNFarmingDlg::StatiCNFarmingDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNFarmingDlg *pThis = (CNFarmingDlg*) pControllerMgr;
	pThis->NFarmingDlgProc( nEvent, nControlID, pControl);
}

void CALLBACK CNFarmingDlg::NFarmingDlgProc(  UINT nEvent, int nControlID, CControllerObj* pControl )
{
	TCHAR *ProductName=NULL;
	cltClient *pclclient=(cltClient *)pclClient;
	stComboBoxItemData tmpComboBoxItemData;		

	pclClient->pclItemManager->GetFarmItemList( &m_siFarmingItemNum, &m_pNFarmingItemUniqueListPtr );
			
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
				m_bStopFarming = TRUE;
				pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
				pclclient->SetSiSelectedOrder(ORDER_STOP);
				//// 농경 종료시 종료시간을 기록하여 보낸다.
				//cltGameMsgRequest_FarmingEndTime clinfo( pclclient->clClientDate.GetDateVary());
				//cltMsg cltMsg(GAMEMSG_REQUEST_FARMINGENDTIME, sizeof(clinfo), (BYTE*)&clinfo);
				//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&cltMsg);
				pclclient->bStopDoingSwitch = FALSE;
			}
			else
			{
				Hide();
			}
		}
		break;

	case NFARMING_DIALOG_COMBO:
	   {
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{							
					m_siSelectedIndex = m_pNFarmingCombo->GetSelectedIndex();

					if( m_siSelectedIndex < 0 ) 
					{
						DeleteDialog();
						break;
					}

					tmpComboBoxItemData=m_pNFarmingCombo->GetItemData(m_siSelectedIndex);
					
					if ( m_siSelectedIndex > -1 )
					{
						TCHAR ProductItemName[ 128 ] = TEXT("");
						StringCchCopy(ProductItemName, 128, m_pNFarmingCombo->GetText(m_siSelectedIndex) );													
					}
				}
				break; 	
			}		
	   }
		break;
	
     case NFARMING_DIALOG_BUTTON:
		{
			switch( nEvent )
			{		

				case EVENT_BUTTON_CLICKED:
					{
						//cyj 콤보 박스 선택 안했을시 빠져나감
						if (m_siSelectedIndex < 0 || m_siSelectedIndex >= m_siFarmingItemNum)
							break;
	
						//m_pNFarmingImageStatic->SetFileName(TEXT("GImg/ItemImg02.SPR"));
						//m_pNFarmingImageStatic->SetFontIndex(m_siSelectedIndex+9);

						if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
						{
							if(m_bStopFarming == TRUE)
							{
								m_bStopFarming = FALSE;
								TCHAR* pText = GetTxtFromMgr(8281);
								m_pNFarmingButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
								cltGameMsgRequest_FarmingStart clinfo( m_pNFarmingItemUniqueListPtr[m_siSelectedIndex] );
								cltMsg clMsg(GAMEMSG_REQUEST_FARMINGSTART, sizeof(clinfo), (BYTE*)&clinfo);
								pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);	
							}
							else
							{
								m_bStopFarming = TRUE;
								TCHAR* pText = GetTxtFromMgr(4923);
								m_pNFarmingButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
								pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
								pclclient->SetSiSelectedOrder(ORDER_STOP);
								//// 농경 종료시 종료시간을 기록하여 보낸다.
								//cltGameMsgRequest_FarmingEndTime clinfo( pclclient->clClientDate.GetDateVary());
								//cltMsg cltMsg(GAMEMSG_REQUEST_FARMINGENDTIME, sizeof(clinfo), (BYTE*)&clinfo);
								//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&cltMsg);
								Init();
							}	
						}
						else
						{
							cltGameMsgRequest_FarmingStart clinfo( m_pNFarmingItemUniqueListPtr[m_siSelectedIndex] );
							cltMsg clMsg(GAMEMSG_REQUEST_FARMINGSTART, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);							
						}
					
					}break;			
				}
		}break;
	}
	
	if ((pclclient->m_pDialog[ NFARMING_DLG ]) != NULL )							
   {
	   Show();        
   }  
   
	return ;
}

void CNFarmingDlg::Show()
{	
	if ( m_bShow )
	{
		return;
	}

	TCHAR Buffer[256 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(2445);
	StringCchPrintf( Buffer, 256, pText, 
		pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FARM, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0] ) );	
	SetTitle(Buffer);	
	m_bShow = true;

	CDialogController::Show( true);	
	SetTimerOnOff( true );
	 	
	return;
}

void CNFarmingDlg::Hide()
{
	CDialogController::Show( false);
}

void CNFarmingDlg::SetFarmingItem( SI16 ItemUnique, SI32 DelayTime, SI32 SuccessPercent )
{	
	m_siFarmingItemUnique = (SI32)ItemUnique;
	m_siDelayTime = DelayTime;
	cltItem clItem;
	clItem.siItemNum = 1;
	clItem.siUnique = ItemUnique;
	m_siSuccessPercent = SuccessPercent;
	
	SetFarmingExplain();
	
	return;
}

void CNFarmingDlg::SetFarmingExplain()
{
	TCHAR Temp[ 256 ] = TEXT("");
	TCHAR Buffer[256 ] = TEXT("");
	
	if ( m_siFarmingItemUnique > 0 )
	{
		TCHAR *ProductItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_siFarmingItemUnique );

		if ( ProductItemName )
		{
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
				TCHAR* pText = GetTxtFromMgr(2446);
				StringCchPrintf( Buffer, 256, TEXT("[%s]\r\n"), ProductItemName );
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(2446);
				StringCchPrintf( Buffer, 256, pText, ProductItemName, ( m_siDelayTime / 1000 ) / 60, ( m_siDelayTime / 1000 ) % 60 );
			}
		}

		if ( m_siSuccessPercent > 0 )
		{
			TCHAR* pText = GetTxtFromMgr(3221);
			StringCchPrintf( Temp, 256, pText, m_siSuccessPercent );
			StringCchCat( Buffer, 256, Temp );
		}
	}


	if (  pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].siUnique > 0 )
	{
		TCHAR *MachineName = (TCHAR*)pclClient->pclItemManager->GetName( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].siUnique );

		if ( MachineName )
		{
			StringCchPrintf( Temp, 256, TEXT("[%s]\r\n"), MachineName);
			StringCchCat( Buffer, 256, Temp );		
		}	
			
	}

#ifdef _DEBUG
		
	if (  pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_NECK ].siUnique > 0 )
	{
		TCHAR *NecklaceName = (TCHAR*)pclClient->pclItemManager->GetName( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_NECK].siUnique );
		if ( NecklaceName )
		{
			StringCchPrintf( Temp, 256, TEXT("[%s]\r\n"), NecklaceName);
			StringCchCat( Buffer, 256, Temp );	
		}
	}
		
#endif

	if ( m_uiLeaseLeftTime > 0 && m_bStopFarming == FALSE )
	{
		TCHAR* pText = GetTxtFromMgr(6497);
		TCHAR leftTime[MAX_PATH] = TEXT("");
		StringCchPrintf( leftTime, MAX_PATH, pText, m_uiLeaseLeftTime / 60 , m_uiLeaseLeftTime % 60 );
		StringCchCat( Buffer, MAX_PATH, leftTime );
	}
	
	m_pNFarmingEdit->SetBorder(true);
	m_pNFarmingEdit->SetBKColorUse(true);
	m_pNFarmingEdit->SetText(Buffer);
	m_pNFarmingEdit->SetReadOnly(true);
	Show();

	return;
}

bool CNFarmingDlg::IsFarmingStatus()
{
	if ( pclClient->pclCM->CR[1]->GetCurrentOrder() == ORDER_FARM )
	{		
		return true;
	}

	return false;
}

void CNFarmingDlg::Reset()
{
	m_siFarmingItemUnique = 0;
	cltClient *pclclient=(cltClient *)pclClient;	
	stComboBoxItemData tmpComboBoxItemData;	

	m_pNFarmingCombo->Clear();

	for ( SI16 i = 0; i < m_siFarmingItemNum; ++i )
	{
		TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pNFarmingItemUniqueListPtr[i] );		

		m_pNFarmingCombo->GetItemData(NFARMING_DIALOG_COMBO  );					
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( ItemName );
		m_pNFarmingCombo->AddItem( &tmpComboBoxItemData );

	}

	m_pNFarmingImageStatic->SetFileName(TEXT("GImg/ItemImg02.SPR"));
	m_pNFarmingImageStatic->SetFontIndex(192);
	m_pNFarmingEdit->SetText(TEXT(" "));
	m_pNFarmingEdit->SetBKColorUse(true);
	m_pNFarmingEdit->SetBorder(true);

	m_siFarmNum[0] = 0;
	memset(m_strFarmName, 0, sizeof(char) * MAX_FARM_NUMBER * (MAX_FARM_NAME_LENGTH+1));
}

void CNFarmingDlg::Action()
{	
	if ( IsFarmingStatus() )			
	{	
		SetFarmingExplain();		
		Show();				
	}		
	else	
	{	
		if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
		{
			Init();
		}

		m_siSuccessPercent = 0;
		Hide();	
		Reset();
	}	
}

BOOL CNFarmingDlg::ShowItemListInComboBox(CComboBox* pComboBox, SI32 typeindex, bool btradeinterface)
{		
	pclClient->pclItemManager->GetFarmItemList( &m_siFarmingItemNum, &m_pNFarmingItemUniqueListPtr );
	
		for ( SI16 i = 0; i < m_siFarmingItemNum; ++i )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pNFarmingItemUniqueListPtr[i] );
			m_pNFarmingCombo->GetItemData(NFARMING_DIALOG_COMBO  );

			stComboBoxItemData tmpComboBoxItemData;
			StringCchCopy(tmpComboBoxItemData.strText, MAX_COMBOBOXITEM_TEXT_LEN,ItemName);
			tmpComboBoxItemData.siParam=m_pNFarmingItemUniqueListPtr[i];

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( ItemName );
			m_pNFarmingCombo->AddItem( &tmpComboBoxItemData );
		}

		Show();

	return TRUE;
}

void CNFarmingDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = ( cltClient * )pclClient;
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
				GMONEY* siRentPrice= (GMONEY*)pData;
				SI32* siItemUnique= (SI32*)&pData[8];

				cltGameMsgRequest_FarmingPayRentFee clinfo( *siRentPrice, *siItemUnique );
				cltMsg clMsg(GAMEMSG_REQUEST_FARMINGPAYRENTFEE, sizeof(clinfo), (BYTE*)&clinfo);
				pclclient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 1:
		{
			if ( RetValue )
			{
				m_bStopFarming = TRUE;
				pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
				pclclient->SetSiSelectedOrder(ORDER_STOP);
			}

			pclclient->bStopDoingSwitch = FALSE;
		}
		break;
	}

	return;
}

void CNFarmingDlg::LentLand(cltMsg* pclMsg, TCHAR* VillageName, GMONEY siRentPrice, SI32 siItemUnique)
{
	//memcpy(farmpclMsg,pclMsg,sizeof(farmpclMsg));
	cltClient *pclclient = ( cltClient * )pclClient;
	TCHAR Buffer[ 256 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(1258);

	TCHAR money[128]= TEXT(" ") ;
	StringCchPrintf(money, 128, TEXT("%I64d"),siRentPrice );

	Nsprintf(Buffer,pText,TEXT("village1"), VillageName   ,TEXT("village2"), VillageName   ,TEXT("money"), money   ,NULL);
	//sprintf( Buffer, pText, VillageName, VillageName, siRentPrice  );

	pText = GetTxtFromMgr(1259);

    stMsgBox MsgBox;
	MsgBox.Set( this,  pText, Buffer, MSGBOX_TYPE_YESNO, 0 );
	BYTE TempBuffer[ 128 ];
	SI16 Size = sizeof( siRentPrice ) + sizeof(siItemUnique );
	memcpy( TempBuffer, &siRentPrice, sizeof(siRentPrice ) );
	memcpy( &TempBuffer[ sizeof( siRentPrice ) ], &siItemUnique, sizeof( siItemUnique ) );

	if ( pclclient->pclCM->CR[1]->pclCI->clIP.GetMoney() < siRentPrice )
	{
		TCHAR* pTitle = GetTxtFromMgr(1260);
		pText = GetTxtFromMgr(1261);

		(pclclient->m_pDialog[ NOTIFYMSG_DLG ])->SetText( pText, pTitle );
		return;
	}

	pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
}

void CNFarmingDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		DrawFarmingDelay( ScreenRX, ScreenRY );
	}
}

void CNFarmingDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		if(m_siFarmingItemUnique)
		{
			SI32 GlobalImageIndex = -1;
			SI32 GlobalImageFont = -1;

			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_siFarmingItemUnique, &GlobalImageIndex,  &GlobalImageFont ) )
			{
				cltClient *pclclient = (cltClient *)pclClient;
				TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( GlobalImageIndex );

				if ( pSpr )
				{
					GP.PutSpr(pSpr, ScreenRX + GetX() + 35, ScreenRY + GetY() + 40, GlobalImageFont );
				}
			}
		}
	}
}

void CNFarmingDlg::SetFarmingInfo(TCHAR* strFarmName, SI16 nFarmNum)
{
	TCHAR Buffer[ 1024 ] = TEXT("");
	TCHAR TempBuffer[ 64 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(8401);

	for(SI16 i=0;i<MAX_FARM_NUMBER;i++)
	{
		if ( _tcscmp(m_strFarmName[i], TEXT("")) == 0 )
		{
			StringCchCopy( m_strFarmName[i], MAX_FARM_NAME_LENGTH, strFarmName );
			m_siFarmNum[i] = m_siFarmNum[i] + nFarmNum;
			break;
		}
		else if ( _tcscmp(m_strFarmName[i], strFarmName) == 0 )
		{
			m_siFarmNum[i] += nFarmNum;
			break;
		}
	}

	for ( SI16 siCount=0; siCount<MAX_FARM_NUMBER; siCount++ )
	{
		if ( _tcscmp(m_strFarmName[siCount], TEXT("")) == 0 )
		{
			break;
		}
		else
		{
			StringCchPrintf( TempBuffer, 64, pText, m_strFarmName[siCount], m_siFarmNum[siCount]);
			StringCchCat(Buffer, 1024, TempBuffer );
		}
	}

	m_pNFarmInfoEdit->SetBorder(true);
	m_pNFarmInfoEdit->SetBKColorUse(true);
	m_pNFarmInfoEdit->SetText(Buffer, DT_WORDBREAK);

	return;
}

BOOL CNFarmingDlg::ReturnAutoFarming()
{
	return m_bStopFarming;
}

void CNFarmingDlg::DrawFarmingDelay(SI32 rx, SI32 ry)
{
	SI32 x = 100 + GetX() + rx;
	SI32 y = 23 + GetY() + ry;
	SI32 siWidth = 210;
	SI32 siHeight = 20;

	// 총 농경시간을 참조하여 게이지가 올라가는 비율을 구한다.
	REAL32	f32FarmRate		= 0.0f;

	f32FarmRate = ( pclClient->CurrentClock - m_dwFarmingStartClock ) / (m_siDelayTime * 1.0f);

	// 게이지를 표시한다.

	SI32 siDrawWidth = (SI32)(siWidth * f32FarmRate);

	for ( SI32 siCount=0; siCount<siDrawWidth; siCount+=2  )
	{
		if( ((DWORD)pclClient->CurrentClock - m_dwFarmingStartClock) >= (DWORD)m_siDelayTime)
			break;
		GP.PutSpr( &m_SprFarmGage, (x+siCount), y );
	}	
}
