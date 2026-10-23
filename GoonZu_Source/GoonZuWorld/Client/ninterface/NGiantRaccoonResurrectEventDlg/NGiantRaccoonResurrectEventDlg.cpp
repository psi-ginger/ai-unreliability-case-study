#include "NGiantRaccoonResurrectEventDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/MsgType-Item.h"


NGiantRaccoonResurrectEventDlg::NGiantRaccoonResurrectEventDlg()
{

}


NGiantRaccoonResurrectEventDlg::~NGiantRaccoonResurrectEventDlg()
{
}

void NGiantRaccoonResurrectEventDlg::Create()
{
	if( IsCreate() )	return;

	// 기부 관련된 인터페이스를 생성한다.
	NContributionEvent_CommonDlg::Create( NGIANTRACCOONRESURRECTEVENTDLG, this, StaticNGiantRaccoonResurrectEventDlgProc );

	SetTitle( GetTxtFromMgr(7653) );	
	
	// 설명문 설정.
	SetExplainText( GetTxtFromMgr(40427) );
	SetActionButtonText( GetTxtFromMgr(40787) );

	// 관련 기부 아이템의 리스트를 얻어온다.
	GetContributionItemList( pclClient->GetUniqueFromHash( "KIND_RACCOONTREEBASE" ) );
} 

void CALLBACK NGiantRaccoonResurrectEventDlg::StaticNGiantRaccoonResurrectEventDlgProc( CControllerMgr *pControllerMgr, UINT nEvent )
{
	NGiantRaccoonResurrectEventDlg *pThis = (NGiantRaccoonResurrectEventDlg *)pControllerMgr;
	pThis->NGiantRaccoonResurrectEventDlgProc( nEvent );
}


void CALLBACK NGiantRaccoonResurrectEventDlg::NGiantRaccoonResurrectEventDlgProc( UINT nEvent )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch( nEvent )
	{
		case EVENT_CONTRIBUTION_BUTTON_CLICK:
			{
								
			}
			break;
		case EVENT_ACTION_BUTTON_CLICK:
			{

			}
			break;
			
		case EVENT_ACTION_CONTRIBUTION_COMPLETE:
			{
				int a = 0;
			}
			break;
		default:
			{

			}
			break;
	}

}

