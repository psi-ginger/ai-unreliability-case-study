#include "NGoonzuSiege.h"
#include "../nvillagedetailinfo/NVillageDetailInfo.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"

CNGoonzuSiegeDlg::CNGoonzuSiegeDlg() : CDialogController()
{
	m_pVillageRankDlg = NULL;
	m_pSiegeListDlg = NULL;
	m_pButtonVillageRank = NULL;
	m_pButtonSiegeList = NULL;
}

CNGoonzuSiegeDlg::~CNGoonzuSiegeDlg()
{
	SAFE_DELETE( m_pVillageRankDlg );
	SAFE_DELETE( m_pSiegeListDlg );
	SAFE_DELETE( m_pButtonSiegeList );
	SAFE_DELETE( m_pButtonVillageRank );
}

void CNGoonzuSiegeDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		NEWCBUTTON( m_pButtonVillageRank );
		NEWCBUTTON( m_pButtonSiegeList );

		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NGoonzuDlg/DLG_GoonzuSiege.ddf");
		file.CreatePopupDialog( this, NGOONZUSIEGE_DLG, "dialog_goonzuSiege", StaticCallBackDialogNGoonzuSiegeDlg );

		file.CreateControl( m_pButtonVillageRank, NGOONZUSIEGE_BUTTON_VILLAGERANK, "button_villagerank"); 
		file.CreateControl( m_pButtonSiegeList, NGOONZUSIEGE_BUTTON_SIEGELIST, "button_siegelist"); 

		m_pVillageRankDlg = new CNGoonzuSiegeRankDlg();
		m_pVillageRankDlg->Create( this );
		m_pVillageRankDlg->Show( true );

		m_pSiegeListDlg = new CNGoonzuSiegeListDlg();
		m_pSiegeListDlg->Create( this );
		m_pSiegeListDlg->Show( false );

		Show( true );
	}
	else
	{
		DeleteDialog();
	}
}

void CALLBACK CNGoonzuSiegeDlg::StaticCallBackDialogNGoonzuSiegeDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNGoonzuSiegeDlg *pThis = (CNGoonzuSiegeDlg *)pControllerMgr;
	pThis->CallBackDialogNGoonzuSiegeDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNGoonzuSiegeDlg::CallBackDialogNGoonzuSiegeDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NGOONZUSIEGE_BUTTON_VILLAGERANK:
		{
			m_pVillageRankDlg->Show( true );
			m_pSiegeListDlg->Show( false );
		}
		break;
	case NGOONZUSIEGE_BUTTON_SIEGELIST:
		{
			m_pVillageRankDlg->Show( false );
			m_pSiegeListDlg->Show( true );
			m_pSiegeListDlg->ShowList();
		}
		break;
	default:
		break;
	}
}