/* ==========================================================================
	클래스 :		CNRareItemDlg

	작성일 :		05/03/17
	작성자 :		김태양

	목  적 :		

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/

#include "NRareItemDlg.h"

#include "../../CommonLogic/MsgType-System.h"
#include "../../StatusBar/ChatStatusBar/ChatStatusBarBaseInfo.h"
#include "../../../lib/WebHTML/WebHTML.h"

#include "../../common/HintDlg/HintDlg.h"
#include "Char\CharManager\CharManager.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../GlobalImgFile/GlobalImgFile.h"

#include "../Client/MarkManager/MarkManager.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;
//CNRareItemDlg		g_RareItemMsg;

CNRareItemDlg::CNRareItemDlg()
{
	
	m_pNRareItemStatic1 = NULL;  
	m_pNRareItemStatic2 = NULL;  
    m_pNRareItemImageStatic1 = NULL;
	m_pNRareItemImageStatic2 = NULL;
	m_pNRareItemButton1 = NULL;
	m_pNRareItemButton2 = NULL;	
	 m_pNRareItemOutLine=NULL;
	m_siBitmapAnimationIndex = 0 ;
	StringCchCopy( m_strPlayerName, MAX_PLAYER_NAME, TEXT(""));	
	m_siHintDlg = 0 ;
	Popupflag =0;
	SetTimerOnOff( true );
	
}

CNRareItemDlg::~CNRareItemDlg()
{
	SAFE_DELETE(m_pNRareItemStatic1);
	SAFE_DELETE(m_pNRareItemStatic2);
	SAFE_DELETE(m_pNRareItemImageStatic1);
	SAFE_DELETE(m_pNRareItemImageStatic2);
	SAFE_DELETE(m_pNRareItemButton1);
	SAFE_DELETE(m_pNRareItemButton2);
	SAFE_DELETE(m_pNRareItemOutLine);
	Popupflag =0;
	SetTimerOnOff( false );
	

	return;

}


void CNRareItemDlg::Create()
{
	m_siImageType = -1;
	m_siImageIndex = -1;

    SetActionTimer( POPUP_TIME );		
	if ( ! IsCreate() )
	{
		//CDialogController::Create( NRAREITEM_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCNRareItemDlgProc, TEXT("명품 "), TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 80, 100, 300, 240, true, false, true);

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NRareItem/DLG_NRareItem.ddf"));
		file.CreatePopupDialog( this, NRAREITEM_DLG, TEXT("dialog_NRareItemDlg"),StatiCNRareItemDlgProc);	
		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			MovePos( 720, 66 );		
		}	
		else
		{
			MovePos( 554, 66 );
		}


		
		m_pNRareItemStatic1= new CStatic( this );
		m_pNRareItemStatic2= new CStatic( this );
		m_pNRareItemImageStatic1= new CImageStatic( this );
		m_pNRareItemImageStatic2= new CImageStatic( this );
		m_pNRareItemButton1= new CButton( this );
		m_pNRareItemButton2= new CButton( this );
		m_pNRareItemOutLine= new COutLine(this);

		
		

		file.CreateControl( m_pNRareItemOutLine, NRAREITEM_DIALOG_OUTLINE, TEXT("outline_NRareItem") );	
		file.CreateControl( m_pNRareItemImageStatic1, NRAREITEM_DIALOG_STATICIMAGE1 , TEXT("imagestatic_NRareItemImage1") );
		file.CreateControl( m_pNRareItemImageStatic2, NRAREITEM_DIALOG_STATICIMAGE2 , TEXT("imagestatic_NRareItemImage2") );
		file.CreateControl( m_pNRareItemStatic1, NRAREITEM_DIALOG_STATIC1 , TEXT("statictext_NRareItem1") );
		file.CreateControl( m_pNRareItemStatic2, NRAREITEM_DIALOG_STATIC2 , TEXT("statictext_NRareItem2") );
		file.CreateControl( m_pNRareItemButton1, NRAREITEM_DIALOG_BUTTON1, TEXT("button_RareItem1") );
		file.CreateControl( m_pNRareItemButton2, NRAREITEM_DIALOG_BUTTON2, TEXT("button_RareItem2") );

	}

	m_pNRareItemButton2->Show(false);
	return;
}



void CNRareItemDlg::Action()
{
	m_pNRareItemImageStatic1->SetFileName(TEXT("Interface/EventNotice/rareitem.SPR"));
	m_pNRareItemImageStatic1->SetFontIndex(m_siBitmapAnimationIndex++);
	
	if ( m_siBitmapAnimationIndex >= 2 )
		m_siBitmapAnimationIndex = 0;

	Popupflag++;
		
	if(Popupflag>200)
	{
		DeleteDialog();
	}

}

void CALLBACK CNRareItemDlg::StatiCNRareItemDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl)
{
	CNRareItemDlg *pThis = (CNRareItemDlg*) pControllerMgr;
	pThis->NRareItemDlgProc( nEvent, nControlID, pControl);
}

void CALLBACK CNRareItemDlg::NRareItemDlgProc(  UINT nEvent, int nControlID, CControllerObj* pControl  )
{
	
	cltClient *pclclient = (cltClient*)pclClient;

	switch(nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	
	case NRAREITEM_DIALOG_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:					
				cltClient *pclclient = (cltClient*)pclClient;				
				if ( _tcslen (m_strPlayerName) > 0 && m_strPlayerName != NULL )
					{						
						TCHAR *Name = (TCHAR*)pclClient->pclCM->CR[1]->GetName();
						// 자기 자신에게는 귓속말을 보내지 못한다.					
						if ( Name != NULL && !_tcscmp( m_strPlayerName, Name ) )
						{
							break;
						}

						pclclient->RequestSecretChatByCharName(m_strPlayerName);
					}
			}break;
		}




	case NRAREITEM_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[256] = TEXT("") ;
					switch(m_siHintDlg)
					{
					case HITBUTTON_RECOMMEND:
						{
							StringCchCopy(buffer, 256, TEXT("recommend"));
						}
						break;
					case HITBUTTON_SUMMON:
						{
							StringCchCopy(buffer, 256, TEXT("summonbreed"));
						}
						break;
					case HITBUTTON_RAREITEM:
						{
							StringCchCopy(buffer, 256, TEXT("goodproduct"));
						}
					}

					if ( m_siHintDlg > 0 )
					{
						

						//if ( g_SoundOnOffDlg.m_bFullScreenMode )
						//{
						pclclient->ShowHelpWindow(buffer);
							//if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
							//{
							//	pclclient->CreateInterface( NHINT_DLG );
							//}
							//((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(buffer);

						//}
						//else
						//	pclclient->m_pHintDlg->Show(buffer);
					}
				}
				break;
			
			case IDC_BUTTON_WHISPERCHAT:
				{
					cltClient *pclclient = (cltClient*)pclClient;

					if ( _tcslen (m_strPlayerName) > 0 && m_strPlayerName != NULL )
					{
						TCHAR *Name = (TCHAR*)pclClient->pclCM->CR[1]->GetName();

						// 자기 자신에게는 귓속말을 보내지 못한다.
						if ( Name != NULL && !_tcscmp( m_strPlayerName, Name ) )
						{
							break;
						}

						pclclient->RequestSecretChatByCharName(m_strPlayerName);
					}
				}
				break;
			}
		}
		break;
	
		
	}


	return;
}

void CNRareItemDlg::Set( cltItem *pclItem, TCHAR *PlayerName, TCHAR *strTitleData, TCHAR *strPrintData,
						   SI32 siHorse,SI32 siSummon,SI32 siHintButton )
{

	if(!IsCreate())
	{
		Create();
	}
	
	SetActionTimer( 200 );
	cltClient *pclclient = (cltClient *)pclClient;		

	if( pclclient->bWatchModeSwitch )
	{
		return;
	}


	memset( m_strPrintMsg, 0, 2048 );
	memset( m_strTitleMsg, 0, 64 );
	StringCchCopy( m_strTitleMsg, 64, strTitleData );
	StringCchCopy( m_strPrintMsg, 2048, strPrintData ) ;


	m_pNRareItemStatic1->SetBorder(true);
	m_pNRareItemStatic1->SetBKColorUse(true);
	m_pNRareItemStatic1->SetText(strPrintData,DT_WORDBREAK );
	
	



	if ( PlayerName != NULL && _tcslen( PlayerName ) > 0 )
	{
		StringCchCopy( m_strPlayerName, MAX_PLAYER_NAME, PlayerName );
		StringCchCat( m_strPlayerName, MAX_PLAYER_NAME, TEXT("") );
	}
	else
	{
		memset ( m_strPlayerName, 0, MAX_PLAYER_NAME );
	}

	TCHAR *Name = (TCHAR*)pclClient->pclCM->CR[1]->GetName();
	m_pNRareItemStatic2->SetBKColorUse(true);
	
	m_pNRareItemStatic2->SetText(PlayerName,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
    
	
	if ( Name )
	{
		if ( !_tcscmp( m_strPlayerName, Name ) )
		{
		    m_pNRareItemButton1->Enable( false );	
		}
		else
		{		
			m_pNRareItemButton1->Enable( true );
		}
	}
	


	// 아이템 SPR 그림파일 1,2,3 에서 필요한 파일로 들어가서 출력해주기   -by taeyang
	if ( pclItem != NULL && siHorse == 0 && siSummon == 0 )
	{
		SI32 itemunique= pclItem->siUnique;
		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) == false )
			return ;

		m_pNRareItemImageStatic2->MoveControl(35,142);
		m_pNRareItemImageStatic2->SetItemSprType(m_siImageType);
		m_pNRareItemImageStatic2->SetFontIndex((SI16)m_siImageIndex);

		

	}


	else if ( siHorse != 0 && siSummon == 0 && pclItem == NULL )
	{
		//[2007.12.21 김영훈 : 철기마 이미지 안나타나는 버그 수정]
		int horse_index;		
		switch(siHorse)
		{	    
		case 1: 	horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR01YOUNG"))  ;	break;
		case 2:	    horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR02YOUNG"))  ;  break;
		case 3:	    horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR03YOUNG"))  ;  break;
		case 4:     horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR04YOUNG"))  ;  break;
		case 5:     horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR05YOUNG"))  ;	break;
		case 6:     horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR06YOUNG"))  ;	break;
		case 7:     horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR07YOUNG"))  ;  break;
		case 8:     horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR08YOUNG"))  ;  break;
		case 10:     horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR10YOUNG"))  ;  break;
		case 11:     horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR11YOUNG"))  ;  break;
		case 12:     horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR12YOUNG"))  ;  break;
		default:    horse_index = pclClient->GetUniqueFromHash(TEXT("KIND_HOR01YOUNG"))  ;	break;

		}

		m_pNRareItemImageStatic2->MoveControl(13,127);
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(horse_index, szPortrait, sizeof(szPortrait), &siPortraitFont);
		m_pNRareItemImageStatic2->SetFileName(szPortrait);
		m_pNRareItemImageStatic2->SetFontIndex(siPortraitFont);
		m_pNRareItemButton2->SetText(TEXT(""),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		siHorse =-1;


	}
	else if ( siSummon > 0 && siSummon < MAX_KINDINFO_NUMBER && siHorse == 0 && pclItem == NULL )
	{
		TCHAR strFileName[ 128 ] = TEXT("");
        m_pNRareItemImageStatic2->MoveControl(13,127);
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siSummon, szPortrait, sizeof(szPortrait), &siPortraitFont);
		m_pNRareItemImageStatic2->SetFileName(szPortrait);
		m_pNRareItemImageStatic2->SetFontIndex(siPortraitFont);
		
	
	}
	else
	{
		m_pNRareItemImageStatic2->MoveControl(35,142);
		m_pNRareItemImageStatic2->SetFileName(TEXT("GImg/eventitem.spr"));
		m_pNRareItemImageStatic2->SetFontIndex(0);
	}

	m_siHintDlg = siHintButton ;

	switch(m_siHintDlg)
	{
	case HITBUTTON_RECOMMEND:
		{	
			m_pNRareItemButton2->Enable( true );
			TCHAR* pText = GetTxtFromMgr(3891);
			  m_pNRareItemButton2->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		break;
	case HITBUTTON_SUMMON:
		{

			m_pNRareItemButton2->Enable( true );
			TCHAR* pText = GetTxtFromMgr(3892);
			m_pNRareItemButton2->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		}
		break;
	case HITBUTTON_RAREITEM:
		{
			TCHAR* pText = GetTxtFromMgr(3893);
			m_pNRareItemButton2->Enable( true );
			m_pNRareItemButton2->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		break;
	default:
		{
			m_pNRareItemButton2->Enable( false );	
			m_pNRareItemButton2->SetText(TEXT("    "),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		break;
	}



	SetTitle(strTitleData);
	
	
	return;
}


void	CNRareItemDlg::SetBestGacha( cltItem *pclItem, TCHAR *PlayerName,TCHAR *strTitleData, TCHAR *strPrintData )
{

	SetActionTimer( 200 );
	cltClient *pclclient = (cltClient *)pclClient;	


	memset( m_strPrintMsg, 0, 2048 );
	memset( m_strTitleMsg, 0, 64 );
	StringCchCopy( m_strTitleMsg, 64, strTitleData );
	StringCchCopy( m_strPrintMsg, 2048, strPrintData ) ;


	m_pNRareItemStatic1->SetBorder(true);
	m_pNRareItemStatic1->SetBKColorUse(true);
	m_pNRareItemStatic1->SetText(strPrintData,DT_WORDBREAK );

	// 아이템 SPR 그림파일 1,2,3 에서 필요한 파일로 들어가서 출력해주기   -by taeyang
	if ( pclItem != NULL )
	{
		SI32 itemunique= pclItem->siUnique;
		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) == false )
			return ;

		m_pNRareItemImageStatic2->MoveControl(35,142);
		m_pNRareItemImageStatic2->SetItemSprType(m_siImageType);
		m_pNRareItemImageStatic2->SetFontIndex((SI16)m_siImageIndex);
	}
	m_pNRareItemStatic2->SetBKColorUse(true);

	m_pNRareItemStatic2->SetText(PlayerName,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	m_pNRareItemButton2->Show(false);

	SetTitle(strTitleData);

	StringCchCopy(m_strPlayerName,256 , PlayerName);
}

void CNRareItemDlg::SetVillageEventGetItem(TCHAR *strTitleData, TCHAR *strPrintData, SI16 _siConstructType,SI32 siItemunique,TCHAR* charname )
{
	SetActionTimer( 200 );
	cltClient *pclclient = (cltClient *)pclClient;	

	memset( m_strPrintMsg, 0, 2048 );
	memset( m_strTitleMsg, 0, 64 );
	StringCchCopy( m_strTitleMsg, 64, strTitleData );
	StringCchCopy( m_strPrintMsg, 2048, strPrintData ) ;

	m_pNRareItemButton1->Show(false);//귓속말버튼 삭제.
	m_pNRareItemStatic1->SetBorder(true);
	m_pNRareItemStatic1->SetBKColorUse(true);
	m_pNRareItemStatic1->SetText(strPrintData,DT_WORDBREAK );
	cltItem				clGetItem;
	SI16				siRareSwitch = 0;
	SI32	GlobalImageIndex = -1;
	SI32	GlobalImageFont = -1;
	if ( pclClient->pclItemManager->MakeRandItemUnique( siItemunique, &clGetItem, 0, 0, &siRareSwitch, 0, 0, 0, 0, 0, 1 ) == false )
	{
		return;
	}
	else
	{
		SI32 itemunique= clGetItem.siUnique;
		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) == false )
		{
			return ;
		}
		switch(_siConstructType)
		{
			case EVENT_STRUCTURE_HALLOWEEN:	//할로윈 이벤트 //Switch_HalloweenEvent
				{
					SetTitle(GetTxtFromMgr(8880));				
				}
				break;
			case EVENT_STRUCTURE_CHRISTMASTREEBASE:	// 일단 임시로.
				{
					SetTitle(GetTxtFromMgr(7660));				
				}
				break;
			case EVENT_PVP_LEAGUE :
				{
					SetTitle( GetTxtFromMgr(9581) );	// 무한대전
				}
				break;
			default:							// 구조물은 말들어지되 이미지가 필요없는것은 텍스트만 뿌려라.
				{
					SetTitle(GetTxtFromMgr(1623));
				}
		}
		m_pNRareItemImageStatic2->MoveControl(35,142);
		m_pNRareItemImageStatic2->SetItemSprType(m_siImageType);
		m_pNRareItemImageStatic2->SetFontIndex((SI16)m_siImageIndex);
	}
	m_pNRareItemStatic2->SetBKColorUse(true);
	m_pNRareItemButton2->Show(false);
}
void CNRareItemDlg::SetVillageEvent( SI32 siVillageUnique, TCHAR *strTitleData, TCHAR *strPrintData, SI16 _siConstructType, SI32 _siKind )
{

	SetActionTimer( 200 );
	cltClient *pclclient = (cltClient *)pclClient;	

	memset( m_strPrintMsg, 0, 2048 );
	memset( m_strTitleMsg, 0, 64 );
	StringCchCopy( m_strTitleMsg, 64, strTitleData );
	StringCchCopy( m_strPrintMsg, 2048, strPrintData ) ;

	m_pNRareItemButton1->Show(false);//귓속말버튼 삭제.
	m_pNRareItemStatic1->SetBorder(true);
	m_pNRareItemStatic1->SetBKColorUse(true);
	m_pNRareItemStatic1->SetText(strPrintData,DT_WORDBREAK );


	TCHAR*	pImageName = NULL;
	SI32	ImageIndex = 0;
	switch(_siConstructType)
	{	
	case EVENT_STRUCTURE_CHRISTMASTREEBASE:		    //크리스마스 트리.
	case EVENT_STRUCTURE_TREE:		    //크리스마스 트리.
		{
			pImageName = "GIMG\\CHRISTMASBUF.SPR";
			SetTitle(strTitleData);
			ImageIndex = 1;
		}
		break;
	case EVENT_STRUCTURE_HERO_BRONZE:	// 영웅동상
		{
			pImageName = "GIMG\\BufImage.SPR";
			SetTitle(GetTxtFromMgr(1623));
			ImageIndex = 18;
		}
		break;
	case EVENT_STRUCTURE_HALLOWEEN:	//할로윈 이벤트 //Switch_HalloweenEvent
		{
			m_pNRareItemButton1->Show(false);//귓속말버튼 삭제.
			pImageName = "GIMG\\BufImage.SPR";
			SetTitle(GetTxtFromMgr(1623));
			ImageIndex = 40;
		}
		break;
	case EVENT_STRUCTURE_NEWYEARTREEBASE:	// 신년이벤트 구조물
		{
			pImageName = "GIMG\\BufImage.SPR";
			SetTitle(GetTxtFromMgr(1623));
			ImageIndex = 31;
		}
		break;
	default:							// 구조물은 말들어지되 이미지가 필요없는것은 텍스트만 뿌려라.
		{
			SetTitle(GetTxtFromMgr(1623));
		}
	}

	if( _siKind == pclClient->GetUniqueFromHash("KIND_RACCOONTREE") )
	{
		pImageName = "GIMG\\ITEMIMG07.SPR";
		ImageIndex = 186;
	}

	if( pImageName )
	{
		m_pNRareItemImageStatic2->SetFileName(TEXT(pImageName));
		m_pNRareItemImageStatic2->MoveControl(35,142);
		m_pNRareItemImageStatic2->SetFontIndex(ImageIndex);
		m_pNRareItemStatic2->SetBKColorUse(true);
	}
	
	TCHAR* pszVillageName = pclClient->pclVillageManager->GetName(siVillageUnique);
	if(pszVillageName)
		m_pNRareItemStatic2->SetText(pszVillageName, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	m_pNRareItemButton2->Show(false);


	StringCchCopy(m_strPlayerName, 256 , "");
}



void CNRareItemDlg::SetCharName(TCHAR* szName)
{
	StringCchCopy(m_strPlayerName, sizeof(m_strPlayerName) / sizeof(m_strPlayerName[0]), szName);
}

void CNRareItemDlg::SetDescription(TCHAR* szDescription)
{
	m_pNRareItemStatic1->SetBorder(true);
	m_pNRareItemStatic1->SetBKColorUse(true);
	m_pNRareItemStatic1->SetText(szDescription, DT_WORDBREAK );
}

void CNRareItemDlg::SetImage(TCHAR* ImagePath, SI32 siIndex)
{
	if(ImagePath == NULL)
		return;

	m_pNRareItemImageStatic2->MoveControl(35,142);
	m_pNRareItemImageStatic2->SetFileName(ImagePath);
	m_pNRareItemImageStatic2->SetFontIndex(siIndex);
}

void CNRareItemDlg::SetMainImage(TCHAR* ImagePath, SI32 siIndex)
{
	if(ImagePath == NULL)
		return;

	m_pNRareItemImageStatic1->SetFileName(ImagePath);
	m_pNRareItemImageStatic1->SetFontIndex(siIndex);
}

void CNRareItemDlg::SetResize(SI32 siWidth, SI32 siHeight)
{
	this->SetHeight(siHeight);
	this->SetWidth(siWidth);

	return;
}

void CNRareItemDlg::SetPosition(SI32 siX, SI32 siY) 
{
	this->SetX(siX);
	this->SetY(siY);

	return;
}