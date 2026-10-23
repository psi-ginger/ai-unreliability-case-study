#include "NFishingDlg.h"


#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "../../CommonLogic/Msg/MsgType-Fishing.h"
#include "../../CommonLogic/Msg/MsgType-Agriculture.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

#include "../../Music/Music.h"
#include "../../interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Resource.h"

#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/Interface/CheckBox.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"


#include "../../../lib/WebHTML/WebHTML.h"
#include "../NLib/NUpdateTimer.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "../common/Char/CharCommon/Char-Common.h"


#include <wininet.h>

extern cltCommonLogic* pclClient;


CNFishingDlg::CNFishingDlg()
{	     	
	m_pFishingImageStatic = NULL;
	m_pFishingStatic1 = NULL;
	m_pFishingStatic2 = NULL;
	m_pFishingStatic3 = NULL;
	m_pFishingStatic4 = NULL;
	m_pFishingStatic5 = NULL;
	m_pFishingCheckBox = NULL;
	m_pFishingEdit1 = NULL;
	m_pFishingEdit2 = NULL;
	m_pFishingEdit3 = NULL;
	m_pOutline = NULL;

	m_bAutoFishing = false;	
	m_bStopFishing = FALSE;

	m_GlobalImageFont = -1;
	m_GlobalImageIndex = -1;

	m_PreClock = 0;

	GP.LoadSprFromFile( TEXT("GImg\\MakeItemDrawPart1.Spr"),	&m_SprFishGage );

	m_pToolInfo = new DBToolInfo[ MAX_FISHING_TOOL_NUMBER ];
	memset( m_pToolInfo, 0, sizeof( DBToolInfo ) * MAX_FISHING_TOOL_NUMBER );
	memset(m_strFishName, 0, sizeof(m_strFishName));
	memset(m_strBuffer, 0, sizeof(m_strBuffer));
	memset(m_siFishNum, 0, sizeof(m_siFishNum));
	Init();	
}

CNFishingDlg::~CNFishingDlg()
{
	SAFE_DELETE_A(m_pToolInfo);
	SAFE_DELETE(m_pFishingImageStatic);
	SAFE_DELETE(m_pFishingStatic1);
	SAFE_DELETE(m_pFishingStatic2);
	SAFE_DELETE(m_pFishingStatic3);
	SAFE_DELETE(m_pFishingStatic4);
	SAFE_DELETE(m_pFishingStatic5);
	SAFE_DELETE(m_pFishingCheckBox);
	SAFE_DELETE(m_pFishingEdit1);
	SAFE_DELETE(m_pFishingEdit2);
	SAFE_DELETE(m_pFishingEdit3);
	SAFE_DELETE(m_pOutline);

	if ( m_SprFishGage.pImage )
	{
		GP.FreeSpr( m_SprFishGage );
	}
}

void CNFishingDlg::Init()
{
	m_bStopFishing = false;
	m_siFishingBaitItemUnique = 0;
	m_GlobalImageFont = -1;
	m_GlobalImageIndex = -1;
	memset(m_strFishName, 0, sizeof(m_strFishName));
	memset(m_strBuffer, 0, sizeof(m_strBuffer));
	memset(m_siFishNum, 0, sizeof(m_siFishNum));
}

void CNFishingDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NFishing/DLG_NFishing.ddf"));
	file.CreatePopupDialog( this, NFISHING_DLG , TEXT("dialog_NFishing"), StaticNFishingtDlgProc);

	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		m_pFishingImageStatic  = new CImageStatic( this ); 
		m_pFishingStatic2  = new CStatic( this ); 
		m_pFishingStatic3  = new CStatic( this );
		m_pFishingStatic4  = new CStatic( this );
		m_pFishingStatic5  = new CStatic( this );
		m_pFishingCheckBox = new CCheckBox( this );
		m_pFishingEdit1 = new CEdit( this );
		m_pFishingEdit2 = new CEdit( this );
		m_pFishingEdit3 = new CEdit( this );
		m_pOutline = new COutLine( this );

		file.CreateControl(m_pFishingImageStatic, NFISHING_IMAGESTATIC , TEXT("imagestatic_NONAME2"));
		file.CreateControl(m_pFishingStatic2, NFISHING_STATIC2 , TEXT("statictext_NFishing2"));
		file.CreateControl(m_pFishingStatic3, NFISHING_STATIC3 , TEXT("statictext_NFishing3"));
		file.CreateControl(m_pFishingStatic4, NFISHING_STATIC4 , TEXT("statictext_NFishing4"));
		file.CreateControl(m_pFishingStatic5, NFISHING_STATIC5 , TEXT("statictext_NFishing5"));
		file.CreateControl(m_pFishingCheckBox, NFISHING_CHECKBOX , TEXT("checkbox_NFishing"));	
		file.CreateControl(m_pFishingEdit1, NFISHING_EDIT1 , TEXT("editbox__NFishing1"));
		file.CreateControl(m_pFishingEdit2, NFISHING_EDIT2 , TEXT("editbox__NFishing2"));
		file.CreateControl(m_pFishingEdit3, NFISHING_EDIT3 , TEXT("editbox__NFishing3"));
		file.CreateControl(m_pOutline, NFISHING_OUTLINE, TEXT("outline_NONAME2"));

		m_pFishingImageStatic->SetFontIndex(-1);
	}
	else
	{	
		m_pFishingStatic1  = new CStatic( this );
		m_pFishingStatic2  = new CStatic( this ); 
		m_pFishingStatic3  = new CStatic( this );
		m_pFishingCheckBox = new CCheckBox( this );
		m_pFishingEdit1 = new CEdit( this );
		m_pFishingEdit2 = new CEdit( this );
		
		file.CreateControl(m_pFishingStatic1, NFISHING_STATIC1 , TEXT("statictext_NFishing1"));
		file.CreateControl(m_pFishingStatic2, NFISHING_STATIC2 , TEXT("statictext_NFishing2"));
		file.CreateControl(m_pFishingStatic3, NFISHING_STATIC3 , TEXT("statictext_NFishing3"));
		file.CreateControl(m_pFishingCheckBox, NFISHING_CHECKBOX , TEXT("checkbox_NFishing"));	
		file.CreateControl(m_pFishingEdit1, NFISHING_EDIT1 , TEXT("editbox__NFishing1"));
		file.CreateControl(m_pFishingEdit2, NFISHING_EDIT2 , TEXT("editbox__NFishing2"));
	}

	Hide();
	return;
}

void CALLBACK CNFishingDlg::StaticNFishingtDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNFishingDlg *pThis = (CNFishingDlg*) pControllerMgr;
	pThis->NFishingtDlgProc( nEvent, nControlID, pControl );	
}

void CALLBACK CNFishingDlg::NFishingtDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{	
	cltClient *pclclient = (cltClient*)pclClient;
	switch( nControlID )
	{		
	case COMMON_DIALOG_CLOSEBUTTON:	
		{
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
				m_bStopFishing = TRUE;
				if(m_bAutoFishing == TRUE)
				{
					m_bAutoFishing = FALSE;
					m_pFishingCheckBox->SetCheck( false, CONTROLLER_KIND_CHECKBOX, false );	// 값만 설정하고 이벤트는 던지지 않는다.
				}
				m_pFishingEdit3->Clear();
				pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
				pclclient->SetSiSelectedOrder(ORDER_STOP);

				pclclient->bStopDoingSwitch = FALSE;
			}
			else
			{
				Hide();
			}
		}			
		break;

	case NFISHING_CHECKBOX:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{			
					if(m_pFishingCheckBox->IsCheck()){
						cltGameMsgRequest_AgricultureAutoGain  clinfo( AGRICULTURE_TYPE_FISH, true );
						cltMsg clMsg(GAMEMSG_REQUEST_AGRICULTUREAUTOGAIN, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);		
					}
					else
					{
						cltGameMsgRequest_AgricultureAutoGain clinfo( AGRICULTURE_TYPE_FISH, false );
						cltMsg clMsg(GAMEMSG_REQUEST_AGRICULTUREAUTOGAIN, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);			
					}					
				}
				break;
				
			}
		}
		break;
	}

	return;
}

bool CNFishingDlg::IsShow()
{
	return m_bShow;
}

void CNFishingDlg::Action()
{	
	SetTimerOnOff( true );
	SetActionTimer( 1000 );

	kUpdateTimer_DrawImage.Init( 0.1 * 1000 );
	kUpdateTimer_DrawImage.SetActive( true, GetTickCount() );

	if ( pclClient->pclCM->CR[1]->GetCurrentOrder() == ORDER_FISH )	
	{	
		SetFishingExplain();
		Show();
		pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FISHING_NOW")), 0, 0 );
	}
	else
	{
		Hide();	
	}
}

void CNFishingDlg::Show()
{
	if ( m_bShow )
	{
		return;
	}

	TCHAR Buffer[256 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(2300);

	StringCchPrintf( Buffer, 256, pText, 
		pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FISH, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 ));
		 
	SetTitle(Buffer);

	SetFishingExplain();
	SetFishingPlaceInfoExplain();
	
	m_bShow = true;	
	
	CDialogController::Show( true );	
	SetTimerOnOff( true );
	SetActionTimer( 1000 );

	kUpdateTimer_DrawImage.Init( 0.1 * 1000 );
	kUpdateTimer_DrawImage.SetActive( true, GetTickCount() );

	return;
}

void CNFishingDlg::Hide()
{
	m_bShow = false;

	CDialogController::Show( false );
	
	return;
}

void CNFishingDlg::Set( SI32 FishingBaitItemUnique, DBToolInfo *pclToolInfo )
{
	m_siFishingBaitItemUnique = FishingBaitItemUnique;

	for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
	{
		if ( pclToolInfo[i].siPos > 0 )
		{
			m_pToolInfo[ i ].Set( &pclToolInfo[ i ] );
		}
	}
	
	cltItem clItem;
	clItem.siItemNum = 1;
	clItem.siUnique = FishingBaitItemUnique;

	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		m_pFishingImageStatic->Show(true);
		//m_pFishingImageStatic->SetFileName(TEXT("GImg/ItemImg03.SPR"));	

		m_pFishingEdit3->Clear();
	}

	Show();

	return;
}

void CNFishingDlg::SetAutoFishing( bool AutoFishing )
{
	m_bAutoFishing = AutoFishing;

	// 설정값에 따른 컨트롤 설정 - 값만 설정하고 이벤트는 보내지 않는다.
	m_pFishingCheckBox->SetCheck( m_bAutoFishing, CONTROLLER_KIND_CHECKBOX, false );

	if ( AutoFishing )
	{
	//	m_bAutoFishing=true;
	//	SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_AUTOFISHING ), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else
	{
	//	m_bAutoFishing=false;
	//	SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_AUTOFISHING ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}

	return;
}

void CNFishingDlg::SetFishingExplain()
{
	if ( m_siFishingBaitItemUnique < 1 )
	{
		return;
	}

	TCHAR *FishingBaitName = (TCHAR*)pclClient->pclItemManager->GetName( m_siFishingBaitItemUnique );

	if ( FishingBaitName == NULL || _tcslen ( FishingBaitName ) < 1 )
	{
		return;
	}

	TCHAR Buffer[ 1024 ] = TEXT("");

	StringCchPrintf( Buffer, 1024, TEXT("%s\r\n"), FishingBaitName );

	for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
	{
		if ( m_pToolInfo[ i ].clItem.siUnique > 0 )
		{
			TCHAR TempBuffer[256 ] = TEXT("");
			TCHAR *RodName = (TCHAR*)pclClient->pclItemManager->GetName( m_pToolInfo[ i ].clItem.siUnique );
			if ( RodName == NULL || _tcslen( RodName ) < 1)
			{
				continue;
			}
		
			//SI16 ItemPos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos( m_pToolInfo[ i ].clItem.siUnique );

			SI16 ItemPos = -1;
			if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].siUnique == m_pToolInfo[ i ].clItem.siUnique )
			{
				ItemPos = pclClient->pclCM->CR[1]->ActiveWeapon;
			}
			else
			{
				ItemPos = -1;
			}
			
			if ( ItemPos < 0 )
			{
				continue;
			}

			cltItem *pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ ItemPos ];
			if ( pclItem->siUnique < 1 || pclItem->siItemNum < 1 )
			{
				continue;
			}
			
			SI32 Durability = pclItem->GetDurability(  );
			SI32 MaxDurability = pclItem->GetMaxDurability( pclClient->pclItemManager );

			StringCchPrintf( TempBuffer, 256, TEXT("%s\r\n"), RodName );

			StringCchCat( Buffer, 1024, TempBuffer );
			

	
			TCHAR Temp[ 256 ] = TEXT("");
			if (  pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_NECK ].siUnique > 0 )
			{
			/*	if( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_NECK]->GetUpgradeSkillLevel( pclClient->pclItemManager, SKILL_FISH ) > 0 )
				{
					TCHAR *NecklaceName = (TCHAR*)pclClient->pclItemManager->GetName( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_NECK].siUnique );
					if ( NecklaceName )
					{
						StringCchPrintf( Temp, 256, TEXT("[%s]\r\n"), NecklaceName );
						StringCchCat( Buffer, 1024, Temp );	
					}
				}*/
			}
		}
	}

	m_pFishingStatic2->SetBorder(true);
	m_pFishingStatic2->SetBKColorUse(true);
	m_pFishingStatic2->SetText(Buffer, DT_WORDBREAK);
	
	return;
}

void CNFishingDlg::SetFishingPlaceInfoExplain()
{
	cltClient* pclclient = (cltClient*)pclClient;

	if ( m_siFishingBaitItemUnique < 1 )
	{
		return;
	}
	
	//--------------------------------------------------------------
	// 사람용
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.GetMapIndex() != 0 )
	{
		return;
	}

	SI32 XPos = pclClient->pclCM->CR[1]->pclCI->clBI.GetX();
	SI32 YPos = pclClient->pclCM->CR[1]->pclCI->clBI.GetY();

	TCHAR Buffer[ 1024 ] = TEXT("");

	SI16 FishKindList[MAX_FISHKIND_NUMBER];
	SI16 FishCatchPercentList[MAX_FISHKIND_NUMBER];

	SI16 FishSkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FISH, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 );
	
	// 기술 필요없는 낚시 퀘스트 중이라면 낚시 스킬을 10으로 만듦
	if ( FishSkillLevel < 10 )
	{
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_FISHNOSKILL;
		if (pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			FishSkillLevel = 10;
		}
	}
	
	SI16 CatchFishKindCounter = pclClient->pclFishingMgr->GetFishingKindInPos( 
		FishSkillLevel,
		m_siFishingBaitItemUnique,
		XPos,
		YPos,
		FishKindList,
		FishCatchPercentList
		);

	if ( CatchFishKindCounter > 0 )
	{
		for( SI16 i = 0; i < CatchFishKindCounter; ++i )
		{
			if(  FishKindList[ i ] > 0 )
			{	
				TCHAR *FishName = (TCHAR *)pclClient->pclItemManager->GetName( FishKindList[ i ] );
				
				SI16 Ref = pclClient->pclFishingMgr->GetRefFromFishItemUnique( FishKindList[ i ] );
				if ( Ref >= 0 )
				{					
					TCHAR *FishingBaitItemName = (TCHAR *)pclClient->pclItemManager->GetName( pclClient->pclFishingMgr->m_pFishKindList[ Ref ]->m_stFishInfo.siBaitItemUniqueList[ 0 ] );
					TCHAR TempBuffer[256 ] = TEXT("");
					
					//[진성] 이벤트 기간이 지나도 낚시대를 가지고 있으면 능력은 그대로 유지 => 2008-9-5
					//if( pclClient->pclEventTimeManager->InEventTime(TEXT("Fishing_Event"), &pclclient->sTime) )
					{
						//[진성] 황금낚시대. => 2008-8-19
						if( ITEMUNIQUE(17160) == pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[pclClient->pclCM->CR[1]->ActiveWeapon].siUnique)
						{
							if( 100 > FishCatchPercentList[ i ] )
								FishCatchPercentList[ i ] = 100;
						}
					}

					StringCchPrintf( TempBuffer, 256, TEXT("%s - %d%%\r\n"), FishName, FishCatchPercentList[ i ] );
					
					StringCchCat( Buffer, 1024, TempBuffer );
				}
			}
		}

		m_pFishingEdit1->SetText(Buffer);
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(2301);
		m_pFishingEdit1->SetText(pText);	
	}

/*	if ( FishKindList != NULL )
	{
		delete [] FishKindList;
		FishKindList = NULL;
	}

	if ( FishCatchPercentList != NULL )
	{
		delete [] FishCatchPercentList;
		FishCatchPercentList = NULL;
	}*/
	//--------------------------------------------------------------

		//--------------------------------------------------------------
		// 소환수용
		SI32 SummonID = pclClient->pclCM->CR[1]->GetSummonID();
		
		// 소환수가 존재하는 경우.
		if( SummonID > 0 && pclClient->pclCM->IsValidID( SummonID ) )
		{
			// 낚시 가능 소환수인지 체크
			if ( pclClient->pclCM->CR[ SummonID ]->pclKI->IsAtb( ATB_FISH ) == false )
			{
				return;
			}

			if ( pclClient->pclCM->CR[ SummonID ]->pclCI->clBI.GetMapIndex() != 0 )
			{
				return;
			}

			TCHAR* pText = GetTxtFromMgr(2302);
			StringCchCopy( Buffer, 1024, pText );

			XPos = pclClient->pclCM->CR[SummonID]->pclCI->clBI.GetX();
			YPos = pclClient->pclCM->CR[SummonID]->pclCI->clBI.GetY();
			
			//FishKindList = NULL;
			//FishCatchPercentList = NULL;

			SI16 CatchFishKindCounter = pclClient->pclFishingMgr->GetFishingKindInPos( 
				pclClient->pclCM->CR[SummonID]->pclCI->clIP.GetLevel()/5+pclClient->pclCM->CR[SummonID]->pclKI->siSubSkillLevel,
				m_siFishingBaitItemUnique,
				XPos,
				YPos,
				FishKindList,
				FishCatchPercentList
				);
			
			if ( CatchFishKindCounter > 0 )
			{
				for( SI16 i = 0; i < CatchFishKindCounter; ++i )
				{
					if(  FishKindList[ i ] > 0 )
					{	
						TCHAR *FishName = (TCHAR *)pclClient->pclItemManager->GetName( FishKindList[ i ] );
						
						TCHAR TempBuffer[256 ] = TEXT("");
						StringCchPrintf( TempBuffer, 256, TEXT("%s - %d%%\r\n"), FishName, FishCatchPercentList[ i ] );
						
						StringCchCat( Buffer, 1024, TempBuffer );
					}
				}
				m_pFishingEdit2->SetText(Buffer);
			}
			else
			{
				pText = GetTxtFromMgr(2301);
				m_pFishingEdit2->SetText(pText);
			}
		
			/*if ( FishKindList != NULL )
			{
				delete [] FishKindList;
				FishKindList = NULL;
			}
			
			if ( FishCatchPercentList != NULL )
			{
				delete [] FishCatchPercentList;
				FishCatchPercentList = NULL;
			}*/
		}
	//}

	return;
}

void CNFishingDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	switch( MsgIndex )
	{
	case 1:
		{
			if ( RetValue )
			{
				m_bStopFishing = TRUE;
				if(m_bAutoFishing == TRUE)
				{
					m_bAutoFishing = FALSE;
					m_pFishingCheckBox->SetCheck( false, CONTROLLER_KIND_CHECKBOX, false );		// 값만 설정하고 이벤트는 던지지 않는다.
				}

				if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
				{
					m_pFishingEdit3->Clear();
				}
				pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
				pclclient->SetSiSelectedOrder(ORDER_STOP);
			}
			pclclient->bStopDoingSwitch = FALSE;
		}
		break;
	}
	return;
}

void CNFishingDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		DrawFishingDelay( ScreenRX, ScreenRY );
	}
}

void CNFishingDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	// 랜덤으로 획득할수 있는 물고기의 이미지를 바꾸면서 보여준다.

	SI16 FishKindList[MAX_FISHKIND_NUMBER];
	SI16 FishCatchPercentList[MAX_FISHKIND_NUMBER];

	SI32 XPos = pclClient->pclCM->CR[1]->pclCI->clBI.GetX();
	SI32 YPos = pclClient->pclCM->CR[1]->pclCI->clBI.GetY();

	SI16 FishSkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FISH, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 );

	SI16 CatchFishKindCounter = pclClient->pclFishingMgr->GetFishingKindInPos( 
		FishSkillLevel,
		m_siFishingBaitItemUnique,
		XPos,
		YPos,
		FishKindList,
		FishCatchPercentList
		);

	SI32	siFishItemUnique = 0;

	if ( CatchFishKindCounter > 0 )
	{
		// 2초마다 바뀌게한다.

		SI32 siCurrentTime = pclClient->CurrentClock;
		
		if(abs(siCurrentTime - m_PreClock) > 2000)
		{
			m_PreClock = siCurrentTime;
			m_siNowFontNum = (m_siNowFontNum+1) % CatchFishKindCounter;
		}

		siFishItemUnique = FishKindList[ m_siNowFontNum ];

		if( siFishItemUnique  > 0 )
		{	
			SI16 Ref = pclClient->pclFishingMgr->GetRefFromFishItemUnique( siFishItemUnique );
			pclClient->pclItemManager->GetItemGImageFileIndexAndFont(siFishItemUnique, &m_GlobalImageIndex, &m_GlobalImageFont );
			if ( Ref >= 0 )
			{	
				cltClient *pclclient = (cltClient *)pclClient;
				TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_GlobalImageIndex );

				if ( pSpr )
				{
					GP.PutSpr(pSpr, ScreenRX + GetX() + 25, ScreenRY + GetY() + 35, m_GlobalImageFont );
				}
			}
		}
	}
}

void CNFishingDlg::SetFishingInfo(TCHAR* strFishName, SI16 nFishNum)
{
	TCHAR Buffer[ 1024 ] = TEXT("");
	TCHAR TempBuffer[ 64 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(8403);

	for(SI16 i=0;i<MAX_FISH_NUMBER;i++)
	{
		if ( _tcscmp(m_strFishName[i], TEXT("")) == 0 )
		{
			StringCchCopy( m_strFishName[i], MAX_FISH_NAME_LENGTH, strFishName );
			m_siFishNum[i] = m_siFishNum[i] + nFishNum;
			break;
		}
		else if ( _tcscmp(m_strFishName[i], strFishName) == 0 )
		{
			m_siFishNum[i] += nFishNum;
			break;
		}
	}

	for ( SI16 siCount=0; siCount<MAX_FISH_NUMBER; siCount++ )
	{
		if ( _tcscmp(m_strFishName[siCount], TEXT("")) == 0 )
		{
			break;
		}
		else
		{
			StringCchPrintf( TempBuffer, 64, pText, m_strFishName[siCount], m_siFishNum[siCount]);
			StringCchCat(Buffer, 1024, TempBuffer );
		}
	}

	StringCchCopy(m_strBuffer, 1024, Buffer);
	m_pFishingEdit3->SetBorder(true);
	m_pFishingEdit3->SetBKColorUse(true);
	m_pFishingEdit3->SetText(Buffer, DT_WORDBREAK);

	return;
}

BOOL CNFishingDlg::ReturnAutoFishing()
{
	return m_bAutoFishing;
}

void CNFishingDlg::DrawFishingDelay(SI32 rx, SI32 ry)
{
	SI32 x = 85 + GetX() + rx;
	SI32 y = 23 + GetY() + ry;
	SI32 siWidth = 210;
	SI32 siHeight = 20;
	SI32 siMaxFishingClock = 100000;

	// 총 낚시시간 : 1분 40초 (100초)
	// 총 낚시시간을 참조하여 게이지가 올라가는 비율을 구한다.
	REAL32	f32FishRate		= 0.0f;

	f32FishRate = ( pclClient->CurrentClock - m_dwFishingStartClock ) / (siMaxFishingClock * 1.0f);

	// 게이지를 표시한다.

	SI32 siDrawWidth = (SI32)(siWidth * f32FishRate);

	for ( SI32 siCount=0; siCount<siDrawWidth; siCount+=2  )
	{
		if(pclClient->CurrentClock - m_dwFishingStartClock >= (DWORD)siMaxFishingClock)
			break;
		GP.PutSpr( &m_SprFishGage, (x+siCount), y );
	}	
}

bool CNFishingDlg::GetCheckBox()
{
	if(m_pFishingCheckBox->IsCheck() == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CNFishingDlg::ShowFishinginfo()
{
	m_pFishingEdit3->SetBorder(true);
	m_pFishingEdit3->SetBKColorUse(true);
	m_pFishingEdit3->SetText(m_strBuffer, DT_WORDBREAK);
}