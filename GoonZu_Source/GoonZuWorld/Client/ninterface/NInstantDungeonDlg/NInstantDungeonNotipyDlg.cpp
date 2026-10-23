/* ==========================================================================
클래스 :		CNRareItemDlg

작성일 :		05/03/17
작성자 :		김태양

목  적 :		

용  법 :		
변경사항 :		없음

차후작업 :		cpp 작업 완료 요

========================================================================*/

#include "../Client/ninterface/NInstantDungeonDlg/NInstantDungeonNotipyDlg.h"

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

InstanstDungeonNotipyDlg::InstanstDungeonNotipyDlg()
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

InstanstDungeonNotipyDlg::~InstanstDungeonNotipyDlg()
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


void InstanstDungeonNotipyDlg::Create()
{
	// 길드 마크 초기화
	m_siGuildMarkIndex = -1;	


	m_siImageType = -1;
	m_siImageIndex = -1;

	SetActionTimer( POPUP_TIME );		
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NRareItem/DLG_NRareItem.ddf"));
		file.CreatePopupDialog( this, NINSTANST_DUNGEON_NOTIPY, TEXT("dialog_NRareItemDlg"), StatiCNInstanstDungeonNotipyDlgProc);	
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

	return;
}



void InstanstDungeonNotipyDlg::Action()
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

void InstanstDungeonNotipyDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	if(-1 != m_siGuildMarkIndex)
	{
		cltClient* pclclient = (cltClient*)pclClient;	

		// 길드마크 뿌리기.
		SI32  guildicon = -1 ;
		TSpr* guildspr	= NULL;

		pclclient->m_pMarkManager->GetMarkSprAndFontIndex( m_siGuildMarkIndex, &guildspr, &guildicon );

		if( guildicon >= 0 && NULL != guildspr)
		{
			SI32 x = GetX() + ScreenRX + m_pNRareItemImageStatic2->GetX()+10;
			SI32 y = GetY() + ScreenRY + m_pNRareItemImageStatic2->GetY()+2;  

			GP.PutSpr(guildspr, x, y, guildicon);
		}	
	}
}



void CALLBACK InstanstDungeonNotipyDlg::StatiCNInstanstDungeonNotipyDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl)
{
	InstanstDungeonNotipyDlg *pThis = (InstanstDungeonNotipyDlg*) pControllerMgr;
	pThis->NInstanstDungeonNotipyDlgProc( nEvent, nControlID, pControl);
}

void CALLBACK InstanstDungeonNotipyDlg::NInstanstDungeonNotipyDlgProc(  UINT nEvent, int nControlID, CControllerObj* pControl  )
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


// [추가 : 황진성 2008. 1. 11 => 인던 보상창 세팅.]
void InstanstDungeonNotipyDlg::SetInstantDungeon(SI32 _siGuildMarkIndex, TCHAR* _szGuildName, TCHAR* _szGuildMasterName)
{
	// 길드 마크의 번호. 그릴때 사용함.
	m_siGuildMarkIndex = _siGuildMarkIndex;

	memcpy(m_strPlayerName, _szGuildMasterName, sizeof(m_strPlayerName));

	SetActionTimer( 200 );
	cltClient *pclclient = (cltClient *)pclClient;	

	NTCHARString256	kBuf(GetTxtFromMgr(7836));
	kBuf.Replace("#GuildName#", _szGuildName);

	m_pNRareItemStatic1->SetBorder(true);
	m_pNRareItemStatic1->SetBKColorUse(true);
	m_pNRareItemStatic1->SetText(kBuf, DT_WORDBREAK );	

	m_pNRareItemStatic2->SetBKColorUse(true);
	m_pNRareItemStatic2->SetText(_szGuildName, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 자신의 길드이면 최강으로.
	if(strcmp(_szGuildName , pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.szGuildName) == 0)
	{
		pclClient->pclCM->CR[1]->uiBestGuild = true;
	}
	else
	{
		pclClient->pclCM->CR[1]->uiBestGuild = false;
	}

	// 수정필요.
	SetTitle(GetTxtFromMgr(5614));
}

//[추가 : 황진성 2008. 1. 17 => 인던 최고 길드변경 알림.]
void  InstanstDungeonNotipyDlg::SetInstantDungeonTopChange(SI32 _siGuildMarkIndex, TCHAR* _szGuildName, SI32 _Score, TCHAR* _szGuildMasterName)
{
	// 길드 마크의 번호. 그릴때 사용함.
	m_siGuildMarkIndex = _siGuildMarkIndex;

	memcpy(m_strPlayerName, _szGuildMasterName, sizeof(m_strPlayerName));

	SetActionTimer( 200 );
	cltClient *pclclient = (cltClient *)pclClient;	

	NTCHARString256	kBuf(GetTxtFromMgr(7835));
	kBuf.Replace("#GuildName#", _szGuildName);
	kBuf.Replace("#Score#",		SI32ToString(_Score));

	m_pNRareItemStatic1->SetBorder(true);
	m_pNRareItemStatic1->SetBKColorUse(true);
	m_pNRareItemStatic1->SetText(kBuf, DT_WORDBREAK );	

	m_pNRareItemStatic2->SetBKColorUse(true);
	m_pNRareItemStatic2->SetText(_szGuildName, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	
	// 수정필요.
	SetTitle(GetTxtFromMgr(5614));
}
