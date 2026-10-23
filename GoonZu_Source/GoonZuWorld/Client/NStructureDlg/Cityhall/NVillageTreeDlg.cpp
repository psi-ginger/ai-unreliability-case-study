#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Tree.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/Cityhall/Cityhall.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"


#include "NVillageTreeDlg.h"

extern cltCommonLogic* pclClient;

void CALLBACK NVillageTreeDlg::StaticCallBackVillageTreeDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	NVillageTreeDlg *pThis = (NVillageTreeDlg *)pControllerMgr;
	pThis->CallBackVillageTreeDlg( nEvent, nControlID, pControl );

	return;
}


void CALLBACK NVillageTreeDlg::CallBackVillageTreeDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{

	cltClient *pclclient = (cltClient*)pclClient;
	switch( nControlID )
	{	
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case BUTTON_DECLAREWAR:
		{
			TCHAR* pText = GetTxtFromMgr(1631);
			TCHAR* pTitle = GetTxtFromMgr(1632);


			//if( MessageBox(NULL, pText, pTitle, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

			
			stMsgBox MsgBox;
			MsgBox.Set(this, pTitle ,pText,  MSGBOX_TYPE_YESNO, 0 );		

			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof(m_selectedVillage );
			memcpy( TempBuffer, &m_selectedVillage, Size );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
			

			/*
			SI32 CharUnique = m_pclStructureManager->CharUnique;
			SI32 id = 1;

			if( m_selectedVillage > 0 && m_selectedVillage < MAX_VILLAGE_NUMBER ) {

				cltMsg clMsg( GAMEMSG_REQUEST_DECLAREWAR, m_selectedVillage );
				pclClient->pclCM->CR[id]->SendNetMsg( (BYTE*)&clMsg );
			}
			*/
		}
		break;

	case BUTTON_CANCEL:
		{
			TCHAR* pText = GetTxtFromMgr(1633);
			TCHAR* pTitle = GetTxtFromMgr(1632);
			
			stMsgBox MsgBox;
			MsgBox.Set(this, pTitle ,pText,  MSGBOX_TYPE_YESNO, 1 );		

			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof(m_selectedVillage );
			memcpy( TempBuffer, &m_selectedVillage, Size );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );


			//if( MessageBox(NULL, pText, pTitle, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;
	
			//SI32 CharUnique = m_pclStructureManager->CharUnique;				
			//SI32 id = 1;

			//if( m_selectedVillage > 0 && m_selectedVillage < MAX_VILLAGE_NUMBER ) {

			//	cltMsg clMsg( GAMEMSG_REQUEST_CANCELDECLAREWAR, m_selectedVillage );
			//	pclClient->pclCM->CR[id]->SendNetMsg( (TCHAR*)&clMsg );
			//}
		}
		break;

	case TREE_VILLAGE:
		{
			switch ( nEvent )
			{
			case EVENT_TREE_SELECTION:
				{
					m_selectedVillage = -1;

					SI16 SelectedIndex = m_pTree->GetSelectedItemIndex();
					if ( SelectedIndex > 0 )
					{
						stTreeItem *pTreeItem = m_pTree->GetTreeItemFromIndex( SelectedIndex );
						if ( pTreeItem )
						{
							m_selectedVillage = pTreeItem->siParam;
						}
					}
				}
				break;
			}
		}
		break;
	}

}


NVillageTreeDlg::NVillageTreeDlg()
{
	m_pTree = NULL;
	m_pEdit = NULL;

	m_pDeclareWarButton = NULL;
	m_pCancelButton = NULL;

	m_pclStructureManager = NULL;

	m_selectedVillage = -1;
}

NVillageTreeDlg::~NVillageTreeDlg()
{
	SAFE_DELETE( m_pTree );
	SAFE_DELETE( m_pEdit );

	SAFE_DELETE( m_pDeclareWarButton );
	SAFE_DELETE( m_pCancelButton );

	SetTimerOnOff( false );
}

void NVillageTreeDlg::Create()
{
	m_pclStructureManager = (cltStructureMngBasic*)pclClient->pclCityHallManager;

	if( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile( TEXT("NInterface/Data/NVillageTree/DLG_VillageTree.ddf") );
		file.CreatePopupDialog( this, NVILLAGETREE_DLG, TEXT("dialog_villagetree"), StaticCallBackVillageTreeDlg );

		m_pTree = new CTree( this );
		m_pTree->Create( TREE_VILLAGE, TEXT("NInterface/Common/Common_24x24_00_109.Spr"), 9, true, 9, 6, 22, 300, 178, 300, 178 );

		m_pDeclareWarButton = new CButton( this );
		m_pDeclareWarButton->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );

		m_pCancelButton = new CButton( this );
		m_pCancelButton->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		m_pCancelButton->Show(false);

		m_pEdit = new CEdit( this );
		m_pEdit->SetTextPrintOption( DT_TOP | DT_LEFT | DT_WORDBREAK );

		file.CreateControl( m_pDeclareWarButton, BUTTON_DECLAREWAR, TEXT("button_declarewar" ));
		file.CreateControl( m_pCancelButton, BUTTON_CANCEL, TEXT("button_cancel" ));

		file.CreateControl( m_pEdit, EDITBOX_VILLAGE, TEXT("editbox_village" ));

		m_pDeclareWarButton->Enable( false );
		m_pCancelButton->Enable( false );

		m_pDeclareWarButton->Show( false );
		m_pCancelButton->Show( false );
		m_pEdit->Show( false );


		Init();

		SetActionTimer( 200 );	
		SetTimerOnOff( true );


	}
	else
	{
		DeleteDialog();
	}

	return;
}

void NVillageTreeDlg::Init()
{
	// 마을 정보 요청
	SI32 CharUnique = m_pclStructureManager->CharUnique;
	SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;

	SI32 id = 1;
	if(id)
	{
		cltMsg clMsg( GAMEMSG_REQUEST_GETVILLAGEINFO, 0 );
		
		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
	}

	cltSimpleRank clrank(RANKTYPE_CITYHALL, siVillageUnique);
	if(pclClient->pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		m_pDeclareWarButton->Enable( true );
		m_pCancelButton->Enable( true );

		m_pDeclareWarButton->Show( true );
		m_pCancelButton->Show( true );
		m_pEdit->Show( true );

	}

}


void NVillageTreeDlg::Action()
{
	int i;
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pclStructureManager);

	if( pCityhallManager == NULL ) return;

	if( pCityhallManager->UpdateWarTreeSwitch ) {
		pCityhallManager->UpdateWarTreeSwitch = FALSE;

		m_pTree->Clear();

		cltVillageInfo *pVillInfo;

		for( i = 1; i < MAX_VILLAGE_NUMBER; ++i ) {
			

			pVillInfo = pclClient->pclVillageManager->pclVillageInfo[ i ];

			if( pVillInfo == NULL ) continue;

			if( pVillInfo->clVillageVary.siParentVillage == 0 ) {

				InsertTreeItem( 0, pVillInfo );
			}

		}

		m_pTree->Refresh();
	}

}


void NVillageTreeDlg::InsertTreeItem( SI32 parentref, cltVillageInfo *pParentVill )
{
	if( pParentVill == NULL ) return;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pclStructureManager);

	stTreeItem	sTreeItem;

	TCHAR buf[ 128 ];

	if( pParentVill->clVillageVary.siParentVillage == 0 ) {


		switch( pCityhallManager->clClientStrInfo.clCityhallStrInfo.bDeclareWarVillage[ pParentVill->siVillageUnique ] )
		{
		case 1:
			{
				TCHAR* pText = GetTxtFromMgr(1634);
				StringCchPrintf( buf, 128, pText, pParentVill->szName );
			}
			break;

		case 2:
			{
				TCHAR* pText = GetTxtFromMgr(1635);
				StringCchPrintf( buf, 128, pText, pParentVill->szName );
			}
			break;

		default:
			StringCchCopy( buf, 128, pParentVill->szName );
			break;
		}
/*
		ti.hParent = 0;
		ti.hInsertAfter = TVI_LAST;
		ti.item.mask = TVIF_TEXT | TVIF_PARAM;
		ti.item.pszText = buf;
		ti.item.lParam = pParentVill->siVillageUnique;

		pNode = TreeView_InsertItem( htree, &ti );
*/

		sTreeItem.siIndex = pParentVill->siVillageUnique;
		sTreeItem.siFileIndex = -1;
		sTreeItem.siImageIndex = 0;
		sTreeItem.siParam = pParentVill->siVillageUnique;
		sTreeItem.siParentIndex = 0;
		StringCchCopy( sTreeItem.strText, MAX_TREE_TEXT_LEN, buf );

		m_pTree->InsertItem( &sTreeItem );
	}

	cltVillageInfo *pclVill;

	for( int i = 1; i < MAX_VILLAGE_NUMBER; ++i )
	{
		pclVill = pclClient->pclVillageManager->pclVillageInfo[ i ];

		if( pclVill == NULL ) continue;

		if( pParentVill->siVillageUnique == pclVill->clVillageVary.siParentVillage ) {
	
			switch( pCityhallManager->clClientStrInfo.clCityhallStrInfo.bDeclareWarVillage[ i ] )
			{
			case 1:
				{
					TCHAR* pText = GetTxtFromMgr(1634);
					StringCchPrintf( buf, 128, pText, pclVill->szName );
				}
				break;

			case 2:
				{
					TCHAR* pText = GetTxtFromMgr(1635);
					StringCchPrintf( buf, 128, pText, pclVill->szName );
				}
				break;

			default:
				StringCchCopy( buf, 128, pclVill->szName );
				break;
			}

/*			
			ti.hParent = pNode;
			ti.hInsertAfter = TVI_LAST;
			ti.item.mask = TVIF_TEXT | TVIF_PARAM;
			ti.item.pszText = buf;
			ti.item.lParam = i;

			node = TreeView_InsertItem( htree, &ti );

			InsertTreeItem( htree, node, pclVill );
*/
			sTreeItem.siIndex = i;
			sTreeItem.siFileIndex = -1;
			sTreeItem.siImageIndex = 0;
			sTreeItem.siParam = i;
			sTreeItem.siParentIndex = pclVill->clVillageVary.siParentVillage;
			StringCchCopy( sTreeItem.strText, MAX_TREE_TEXT_LEN, buf );

			m_pTree->InsertItem( &sTreeItem );

			InsertTreeItem( pclVill->clVillageVary.siParentVillage, pclVill );

		}

	}


}



void NVillageTreeDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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

				SI32 CharUnique = m_pclStructureManager->CharUnique;
				SI32 id = 1;

				if( m_selectedVillage > 0 && m_selectedVillage < MAX_VILLAGE_NUMBER )
				{

					cltMsg clMsg( GAMEMSG_REQUEST_DECLAREWAR, m_selectedVillage );
					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
				}
			}
		}
		break;	
	case 1:	//선전포고 취소
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{

				SI32 CharUnique = m_pclStructureManager->CharUnique;
				SI32 id = 1;

				if( m_selectedVillage > 0 && m_selectedVillage < MAX_VILLAGE_NUMBER ) {

					cltMsg clMsg( GAMEMSG_REQUEST_CANCELDECLAREWAR, m_selectedVillage );
					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
				}
			}
		}
		break;

	}
	return;
}