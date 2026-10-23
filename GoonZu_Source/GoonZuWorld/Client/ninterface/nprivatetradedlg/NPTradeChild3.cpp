#include "NPrivateTradeDlg.h"
#include "NPTradeChild3.h"

#include "..\..\..\Common\SMLib\LibList\LibList.h"
#include "..\..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderObj.h"

//#include "..\..\Interface\PrivateChatDlg\PrivateChatDlgMgr.h"

#include "..\..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"


#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/House/House.h"

#include "../../../lib/WebHTML/WebHTML.h"



#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"




#include "Client.h"

extern cltCommonLogic* pclClient;

//CNPrivateTradeDlg g_NPrivateTradeDlg;

CNTradeChild3::CNTradeChild3()
{	
	m_pNPTrade3Button = NULL;
	m_pNPTrade3Static = NULL;
	m_pParent= NULL;


}

CNTradeChild3::~CNTradeChild3()
{	
	SAFE_DELETE(m_pNPTrade3Button);
	SAFE_DELETE(m_pNPTrade3Static);
	//SAFE_DELETE( m_pParent);
}

void CNTradeChild3::Create(CControllerMgr *pParent)
{
	if(!IsCreate())
	{
	
		m_pParent =(CNPrivateTradeDlg*)pParent;
		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NPrivateTrade/DLG_PTradeChild3.ddf"));
		file.CreateChildDialog( this, NPTRADECHILD3_DLG, TEXT("dialog_PTradeChild3"),StaticNTradeChild3Proc, pParent);
		

		
		m_pNPTrade3Button =  new CButton(this);
		m_pNPTrade3Static =  new CStatic(this);
		

		file.CreateControl(m_pNPTrade3Button, NPTRADECHILD3_BUTTON , TEXT("button_PTradeChild3"));	
		file.CreateControl(m_pNPTrade3Static, NPTRADECHILD3_STATIC , TEXT("statictext_PTradeChild3"));	
	
		Hide();
	}
}

	
void  CNTradeChild3::Show()
{
	CDialogController::Show( true );
}

void  CNTradeChild3::Hide()
{
	CDialogController::Show( false );

}

void CALLBACK CNTradeChild3::StaticNTradeChild3Proc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNTradeChild3 *pThis = (CNTradeChild3*) pControllerMgr;
	pThis->NTradeChild3Proc( nEvent, nControlID, pControl );

}
	
void CALLBACK CNTradeChild3::NTradeChild3Proc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
    cltClient *pclclient = ( cltClient *)pclClient;

	switch (nControlID  )
	{	
	

	case NPTRADECHILD3_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( (m_pParent->m_siOtherCharLevel) < MIN_LEVEL_FOR_HORSE || pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < MIN_LEVEL_FOR_HORSE )
					{
						TCHAR* pTitle = GetTxtFromMgr(2317);
						TCHAR* pText = GetTxtFromMgr(2318);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, MIN_LEVEL_FOR_HORSE );						
					}
					else
						m_pParent->PushHorse();

				}
				break;
			}
		}break;

	}

	return;

}


