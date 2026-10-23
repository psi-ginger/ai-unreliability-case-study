#include "NValentineDayNPCDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"
#include "../../CommonLogic/Msg/MsgType-NPC.h"


NValentineDayNPCDlg::NValentineDayNPCDlg()
{
	
}

NValentineDayNPCDlg::~NValentineDayNPCDlg()
{
	
}

void NValentineDayNPCDlg::Destroy()
{ 


}

void NValentineDayNPCDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;

    file.LoadFile("Ninterface/Data/NValentineDay/NValentineDayNPC.ddf");
	file.CreatePopupDialog( this, VALENTINEDAY_NPC_DLG, "dialog_valentinedaynpc" , StaticNValentineDayNPCDlgProc );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_NPC_STATIC_PORTRAIT_IMAGE,	this),  VALENTINEDAY_NPC_STATIC_PORTRAIT_IMAGE,	TEXT("statictext_portrait_image")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_NPC_BUTTON_BUY_ITEM,		this),  VALENTINEDAY_NPC_BUTTON_BUY_ITEM,		TEXT("button_buyitem")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_NPC_BUTTON_PACKING,		this),  VALENTINEDAY_NPC_BUTTON_PACKING,		TEXT("button_packing")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,  	VALENTINEDAY_NPC_OUTLINE_NONAME1,		this),  VALENTINEDAY_NPC_OUTLINE_NONAME1,		TEXT("outline_NONAME1")				);
	

	TCHAR	szPortrait[256];
	SI32	siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_GIFTRACCOON")), szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_InterfaceFactory.GetStatic( VALENTINEDAY_NPC_STATIC_PORTRAIT_IMAGE )->SetFileName(szPortrait);
	m_InterfaceFactory.GetStatic( VALENTINEDAY_NPC_STATIC_PORTRAIT_IMAGE )->SetFontIndex( siPortraitFont );

	SetX( 450 ); 
	SetY( 300 );
}

void NValentineDayNPCDlg::Action()
{

}

void NValentineDayNPCDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	TSpr* pspr = pclClient->GetGlobalSpr( GIMG_VALENTINEDAY_HEART );
	if( pspr ) 
	{
		GP.PutSprScreen( pspr, GetX(),		 GetY(),	  max(0, TABS(pclClient->CurrentClock / 100))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() +  50, GetY() - 20, max(0, TABS(pclClient->CurrentClock / 120))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 120, GetY() + 50, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() + 160, GetY() + 10, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
	}	
}

void CALLBACK NValentineDayNPCDlg::StaticNValentineDayNPCDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NValentineDayNPCDlg *pThis = (NValentineDayNPCDlg *)pControllerMgr;
	pThis->NValentineDayNPCDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NValentineDayNPCDlg::NValentineDayNPCDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case VALENTINEDAY_NPC_BUTTON_BUY_ITEM:
			{
				if( NULL == pclclient->m_pDialog[ VALENTINEDAY_TRADE_DLG ] )
					pclclient->CreateInterface( VALENTINEDAY_TRADE_DLG );
					
				DeleteDialog();
			}
			break;
		case VALENTINEDAY_NPC_BUTTON_PACKING:
			{
				if( NULL == pclclient->m_pDialog[ VALENTINEDAY_PACKING_DLG ] )
					pclclient->CreateInterface( VALENTINEDAY_PACKING_DLG );

				DeleteDialog();
			}
			break;
			
		default:
			{

			}
			break;
	}
}

