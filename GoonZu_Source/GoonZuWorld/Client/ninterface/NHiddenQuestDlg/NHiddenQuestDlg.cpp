#include "./NHiddenQuestDlg.h"

#include "..\..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "../../Client.h"

#include <CommonLogic.h>

#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/ListView.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../common/DailyQuest3/DailyQuest3Mgr.h"
#include "../../common/DailyQuest2/DailyQuest2Mgr.h"
#include " ../../../CommonLogic/Msg/MsgType-DailyQuest2.h"
#include " ../../../CommonLogic/Msg/MsgType-DailyQuest3.h"
#include "../../../common/DailyQuest2/DailyQuest2Obj.h"
#include "../../../common/DailyQuest3/DailyQuest3Obj.h"

extern cltCommonLogic* pclClient;


NHiddenQuestDlg::NHiddenQuestDlg()
{
	m_pDailyQuest2ObjDataList = new CDailyQuest2ObjData[ MAX_AVAILABLE_DAILYQUEST2_NUM ];
	m_pDailyQuest3ObjDataList = new CDailyQuest3ObjData[ MAX_AVAILABLE_DAILYQUEST3_NUM ];
	m_siCount = 0;
	m_bShow = false;
	m_pListView_item_image = NULL;
	m_pEdit_explain = NULL;
	m_pButton_OK= NULL;
	m_pButton_Cancel= NULL;

	m_bMoneySelected = FALSE;

	m_siImageType = -1;
	m_siImageIndex = -1;
	flag2 = 0;
	flag3 = 0;
	
}

NHiddenQuestDlg::~NHiddenQuestDlg()
{
	if ( m_pDailyQuest2ObjDataList )
	{
		ZeroMemory(m_pDailyQuest2ObjDataList,sizeof(m_pDailyQuest2ObjDataList)) ;
	}
	if ( m_pDailyQuest3ObjDataList )
	{
		ZeroMemory(m_pDailyQuest3ObjDataList,sizeof(m_pDailyQuest3ObjDataList)) ;
	}

	if(m_pListView_item_image)
		delete m_pListView_item_image;
	if(m_pEdit_explain)
		delete m_pEdit_explain;

	if(m_pButton_OK)
		delete m_pButton_OK;
	if(m_pButton_Cancel)
		delete m_pButton_Cancel;

	
}

void NHiddenQuestDlg::Create( )
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient *)pclClient;
	
		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NDailyQuestDlg2/DLG_HiddenQuest.ddf"));
		file.CreatePopupDialog( this, HIDDENQUEST2_DLG, TEXT("dialog_nhiddenquest"), NHiddenQuestDlgProc);

		m_pListView_item_image	=	new 	CListView	( this )	;
		m_pEdit_explain			=	new 	CEdit	( this )	;
		m_pButton_OK			=	new CButton (this);
		m_pButton_Cancel		=	new CButton (this);

		file.CreateControl( 	m_pListView_item_image	, 	NHIDDENQUEST_ITEM_IMAGE	, TEXT("listview_item_image") )	;
		file.CreateControl( 	m_pEdit_explain	, 	NHIDDENQUEST_EDIT_EXPLAIN	, TEXT("editbox_explain") )	;
		file.CreateControl( 	m_pButton_OK	, 	NHIDDENQUEST_BUTTON_OK	, TEXT("button_OK") )	;
		file.CreateControl( 	m_pButton_Cancel	, 	NHIDDENQUEST_BUTTON_CANCEL	, TEXT("button_Cancel") )	;

	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK NHiddenQuestDlg::NHiddenQuestDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
    NHiddenQuestDlg *pThis = (NHiddenQuestDlg*)pControllerMgr;
	pThis->NHiddenQuestDlgitemProc( nEvent, nControlID, pControl );
}

void CALLBACK NHiddenQuestDlg::NHiddenQuestDlgitemProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NHIDDENQUEST_BUTTON_OK:
		{
			if ( flag2 ==1 ) 
			{
				if (m_pDailyQuest2ObjDataList[ siIndex ].m_siMonsterKind == NULL)	return;
				m_pDailyQuest2ObjDataList[ siIndex ].m_bHiddenQuest = true ;

				cltGameMsgRequest_DailyQuest2Select clDailyQuest2Select( &m_pDailyQuest2ObjDataList[ siIndex ], m_bMoneySelected,siIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST2SELECT, sizeof( clDailyQuest2Select ), (BYTE*)&clDailyQuest2Select );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_OK")), 0, 0 );
			}
			else if( flag3 == 1 )
			{
				if (m_pDailyQuest3ObjDataList[ siIndex ].m_siItemUnique == NULL)	return;
				m_pDailyQuest3ObjDataList[ siIndex ].m_bHiddenQuest = true ;

				cltGameMsgRequest_DailyQuest3Select clDailyQuest3Select( &m_pDailyQuest3ObjDataList[ siIndex ], m_bMoneySelected,siIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST3SELECT, sizeof( clDailyQuest3Select ), (BYTE*)&clDailyQuest3Select );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_OK")), 0, 0 );
			}
			DeleteDialog();


		}
		break;
	case NHIDDENQUEST_BUTTON_CANCEL:
		{
			if ( flag2 ==1 ) {

				m_pDailyQuest2ObjDataList[ siIndex ].m_bHiddenQuest = false ;
				cltGameMsgRequest_DailyQuest2Select clDailyQuest2Select( &m_pDailyQuest2ObjDataList[ siIndex ], m_bMoneySelected,siIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST2SELECT, sizeof( clDailyQuest2Select ), (BYTE*)&clDailyQuest2Select );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_OK")), 0, 0 );
			}
			else if( flag3 ==1 ){

				m_pDailyQuest3ObjDataList[ siIndex ].m_bHiddenQuest = false ;
				cltGameMsgRequest_DailyQuest3Select clDailyQuest3Select( &m_pDailyQuest3ObjDataList[ siIndex ], m_bMoneySelected,siIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST3SELECT, sizeof( clDailyQuest3Select ), (BYTE*)&clDailyQuest3Select );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_OK")), 0, 0 );
			}			
			DeleteDialog();
		}
		break;		
	}
}
void NHiddenQuestDlg::Show()
{
	m_bShow = true;

	CDialogController::Show( true );
}

void NHiddenQuestDlg::Hide()
{
	m_siCount = 0;
	m_bShow = false;

	CDialogController::Show( false );
}

bool NHiddenQuestDlg::IsShow()
{
	return m_bShow;
}


void NHiddenQuestDlg::SetPramformDailyQuest2(SI16 para , CDailyQuest2ObjData *pDailyQuest2ObjDataList)
{
	siIndex = para;
	SetNInventoryInListView( m_pListView_item_image);
	memcpy( m_pDailyQuest2ObjDataList, pDailyQuest2ObjDataList, sizeof( CDailyQuest2ObjData )*30 );
	flag2 = 1;

}
void NHiddenQuestDlg::SetPramformDailyQuest3(SI16 para , CDailyQuest3ObjData *pDailyQuest3ObjDataList)
{
	siIndex = para;
	SetNInventoryInListView2( m_pListView_item_image);
	memcpy( m_pDailyQuest3ObjDataList, pDailyQuest3ObjDataList, sizeof( CDailyQuest3ObjData )*100 );
	flag3 = 1;
}
void NHiddenQuestDlg::SetNInventoryInListView(CListView* in_pListView)
{
	SI16 StartPos = 0;

	SI16 i = 0, j = 0;

	SI32 id = 1;

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

	in_pListView->DeleteAllItems();


	if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ITEMUNIQUE(13321), &GlobalImageIndex, &GlobalImageFont ) )
	{
		TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( ITEMUNIQUE(13321) );
        if ( ItemName )
			{
				TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
				//pclClient->pclItemManager->GetExplain( in_pCltItem, ToolTipMsg );
				stListViewItemData ListViewItemData;
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,1, ItemName, ToolTipMsg );
				in_pListView->SetStartItemPos( 0 );
				in_pListView->InsertItem( &ListViewItemData );

                TCHAR * pText = GetTxtFromMgr(8216);//아이템은 경험치랑 등등 2배가 된다고 설명 써주는 시간
				m_pEdit_explain->SetText(pText);
			}
	}
}

void NHiddenQuestDlg::SetNInventoryInListView2(CListView* in_pListView)
{
	SI16 StartPos = 0;

	SI16 i = 0, j = 0;

	SI32 id = 1;

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

	in_pListView->DeleteAllItems();


	if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ITEMUNIQUE(13321), &GlobalImageIndex, &GlobalImageFont ) )
	{
		TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( ITEMUNIQUE(13321) );
		if ( ItemName )
		{
			TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
			//pclClient->pclItemManager->GetExplain( in_pCltItem, ToolTipMsg );
			stListViewItemData ListViewItemData;
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,1, ItemName, ToolTipMsg );
			in_pListView->SetStartItemPos( 0 );
			in_pListView->InsertItem( &ListViewItemData );

			TCHAR * pText = GetTxtFromMgr(8368);//히든 퀘스트를 할때는 경험치 2배 및 신기 한개를 만들어야 한다고 표시
			m_pEdit_explain->SetText(pText);
		}
	}
}
