

//#include <Windows.h>
//#include <Directives.h>
#include "Client.h"
#include "NItemMallManager.h"
//#include "Client.h"
//#include "../../../Client/ItemInfo/ItemInfo.h"
//#include "../../NInterface/NItemInfoDlg/ItemInfoDlg.h"

   
#include "Char\CharManager\CharManager.h"
#include "CommonLogic.h"
#include "../../InterfaceMgr/Interface/Button.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

CNItemMallButtonManager::CNItemMallButtonManager()
{
	m_pNItemMallButton= NULL;
	m_siBitmapAnimationIndex = 0 ;
	SetTimerOnOff( true );

	m_bFlash = false;
}


CNItemMallButtonManager::~CNItemMallButtonManager()
{
	m_siBitmapAnimationIndex = 0;
    SAFE_DELETE(m_pNItemMallButton);
	SetTimerOnOff( false );
	m_bFlash = false;
}

void CNItemMallButtonManager::Action()
{	
	if( m_bFlash == true )
	{
		m_pNItemMallButton->SetFontIndex(m_siBitmapAnimationIndex ++);

		if ( m_siBitmapAnimationIndex >= 2 )
			m_siBitmapAnimationIndex = 0;
	}
	
}

void CNItemMallButtonManager::Create()
{
	SetActionTimer(200);

//	중국도 아이템몰이 들어가서 
//    if ( pclClient->siServiceArea == ConstServiceArea_China )
//		return;

	if(!IsCreate())
	{
		CInterfaceFile file;

		if ( g_SoundOnOffDlg.m_bFullScreenMode )
		{
			file.LoadFile(TEXT("NInterface/Data/NItemMall/DLG_NItemMall.ddf"));


 			file.CreatePopupDialog( this, NITEMMALLMANAGER_DLG, TEXT("dialog_NItemMall"), StaticNItemMallButtonMsgProc);		
	       
			m_pNItemMallButton = new CButton(this);
			
			file.CreateControl(m_pNItemMallButton, NITEMMALLMANAGER_BUTTON, TEXT("button_NItemMall"));
			MovePos(342,48);

			/*
			// 군주 스페셜은 당분간 아이템몰이 없다.
			if(pclClient->siServiceArea == ConstServiceArea_Korea)
			{
				m_pNItemMallButton->Show( false );
			}*/

			m_pNItemMallButton->SetFileName(TEXT("ItemMall/itemmall.SPR"));	

			SetTopMostWindow(true);
		}
	}
}



void CALLBACK CNItemMallButtonManager::StaticNItemMallButtonMsgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNItemMallButtonManager *pThis = (CNItemMallButtonManager*) pControllerMgr;
	pThis->NItemMallButtonMsgProc( nEvent, nControlID, pControl );
}


void CALLBACK CNItemMallButtonManager::NItemMallButtonMsgProc(  UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient*)pclClient;
	switch( nControlID )
	{
	
	case NITEMMALLMANAGER_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{                    
					
				 //강제로 받는 페이지 부른다


					if ( g_SoundOnOffDlg.m_bFullScreenMode )
					{						
						if(!pclclient->m_pDialog[NITEMMALL_DLG])
						{
							pclclient->CreateInterface(NITEMMALL_DLG);

							cltGameMsgRequest_ItemMallItemList clinfo(1);
							cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
							//CNItemMallManager1* ItemManager= (CNItemMallManager1*)pclclient->m_pDialog[NITEMMALL_DLG] ;
							
						}
					}
					
					else
					{
/*
						if(pclClient->pclItemMallManager->CreateItemMallDlg( pclclient->GetHwnd(), pclclient->GetInstance(), 1 ) )
						{				
							cltGameMsgRequest_ItemMallItemList clinfo(1);
							cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
*/
					}
					
					

					
				}
				break;
			}

		}
		break;
			

	}

	return;
	
}
