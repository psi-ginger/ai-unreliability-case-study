

#include "../../../Client/Client.h"
#include "../../../CommonLogic/Msg/MsgType-NPC.h"
#include "../../../Common/SystemNPC/SystemNPC.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "NRPList.h"


extern cltCommonLogic* pclClient;

CNRPList::CNRPList()
{
	m_SelectedListIndex = -1;
	m_CurrentPage = 0;
}	

CNRPList::~CNRPList()
{

}

void CNRPList::Initialize()
{

}

void CNRPList::Destroy()
{

}

void CNRPList::Create( CControllerMgr *pParent )
{

 	cltClient *pclclient = (cltClient *)pclClient;

 	if ( ! IsCreate() )
	{
 		CInterfaceFile file;
		
		file.LoadFile(TEXT("NInterface/Data/NRPMerchant/DLG_RPList.ddf"));
		file.CreateChildDialog( this, NRPLIST_DLG, TEXT("dialog_RPList"), StatiCNRPListDlgProc, pParent);

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST   ,NRPLIST_LIST , this)  , NRPLIST_LIST , TEXT("listbox_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NRPLIST_BUTTON_PRE , this)  , NRPLIST_BUTTON_PRE , TEXT("button_pre"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NRPLIST_BUTTON_NEXT , this)  , NRPLIST_BUTTON_NEXT , TEXT("button_next"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(	STATIC ,NRPLIST_STATIC_PAGE,  this ) , NRPLIST_STATIC_PAGE , TEXT("statictext_noname3"));


		TCHAR* pText = GetTxtFromMgr(3413);	
		m_InterfaceFactory.GetList(NRPLIST_LIST)->SetColumn(0, 225, pText);

		pText = GetTxtFromMgr(4008);
		m_InterfaceFactory.GetList(NRPLIST_LIST)->SetColumn(1, 75,  pText);

		m_InterfaceFactory.GetList(NRPLIST_LIST)->SetBorder(TRUE);
		m_InterfaceFactory.GetList(NRPLIST_LIST)->SetBKColorUse(TRUE);
		m_InterfaceFactory.GetList(NRPLIST_LIST)->AdjustHeight( 10 , true );
		m_InterfaceFactory.GetList(NRPLIST_LIST)->Refresh();

		m_SelectedListIndex = -1;
		m_CurrentPage = 1;
		
		TCHAR strPage[32] = {'\0', };

		StringCchPrintf(strPage,32,TEXT("%d"),m_CurrentPage);

		m_InterfaceFactory.GetStatic( NRPLIST_STATIC_PAGE )->SetText(strPage, TA_CENTER );

		cltGameMsgRequest_RecommandPointList clRPList( m_CurrentPage );
		cltMsg clMsg( GAMEMSG_REQUEST_RECOMMANDPOINTLIST,sizeof(cltGameMsgRequest_RecommandPointList), (BYTE*)&clRPList  );
		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
		
		cltMsg clMsg2( GAMEMSG_REQUEST_GETRECOMMANDPOINT,0,NULL );
		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg2 );
	}

}

void CALLBACK CNRPList::StatiCNRPListDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNRPList *pThis = (CNRPList*) pControllerMgr;
	pThis->NRPListDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNRPList::NRPListDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

 	switch(nControlID)
	{

	case NRPLIST_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI32 TempSelectIndex = m_InterfaceFactory.GetList(NRPLIST_LIST)->GetSelectedIndex();

					if(TempSelectIndex < 0)
						break;

					m_SelectedListIndex = TempSelectIndex;
				}
				break;
			}
		}
		break;
	
	case NRPLIST_BUTTON_PRE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_CurrentPage--;

					if(m_CurrentPage < 1) m_CurrentPage = 1;

					TCHAR strPage[32] = {'\0', };

					StringCchPrintf(strPage,32,TEXT("%d"),m_CurrentPage);

					m_InterfaceFactory.GetStatic( NRPLIST_STATIC_PAGE )->SetText(strPage, TA_CENTER );

					cltGameMsgRequest_RecommandPointList clRPList( m_CurrentPage );
					cltMsg clMsg( GAMEMSG_REQUEST_RECOMMANDPOINTLIST,sizeof(cltGameMsgRequest_RecommandPointList), (BYTE*)&clRPList  );
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

				}
				break;
			}
		}
		break;
	case NRPLIST_BUTTON_NEXT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_CurrentPage++;

					if(m_CurrentPage > 100) m_CurrentPage = 100;

					TCHAR strPage[32] = {'\0', };

					StringCchPrintf(strPage,32,TEXT("%d"),m_CurrentPage);

					m_InterfaceFactory.GetStatic( NRPLIST_STATIC_PAGE )->SetText(strPage, TA_CENTER );

					cltGameMsgRequest_RecommandPointList clRPList( m_CurrentPage );
					cltMsg clMsg( GAMEMSG_REQUEST_RECOMMANDPOINTLIST,sizeof(cltGameMsgRequest_RecommandPointList), (BYTE*)&clRPList  );
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
					
				}
				break;
			}
		}
		break;
	}
}



void CNRPList::SetRecommandPointUserList( stRPListData * pRPListData  )
{	

	m_InterfaceFactory.GetList(NRPLIST_LIST)->Clear();

	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	

	Li.iItem = 0;	
	Li.iSubItem = 0;

	SI32 Count = 0;
	TCHAR strCharName[32] = {'\0',};
	TCHAR strLevel[32] = {'\0',};	
	TCHAR strPoint[32] = {'\0',};
	stListBoxItemData Data;
	for ( int i = 0; i < MAX_RPLISTDATA_PER_PAGE ; ++i )
	{
		if( pRPListData[i].szCharName[0] != '\0' )
		{
			Li.iItem = Count;

			Li.iSubItem = 0;
			//StringCchPrintf( strLevel, 32, TEXT("%d"), pRPListData[i].siLevel );
			Li.pszText = pRPListData[i].szCharName;
			Data.Set(0, Li.pszText);

			Li.iSubItem = 1;
			if(pRPListData[i].siLevel < 1)
			{
				pRPListData[i].siLevel = 1;
			}

			StringCchPrintf( strLevel, 32, TEXT("%d"), pRPListData[i].siLevel );
			Li.pszText = strLevel;
			Data.Set(1, Li.pszText);

			m_InterfaceFactory.GetList(NRPLIST_LIST)->AddItem(&Data);

			Count++;
		}
	}
	m_InterfaceFactory.GetList(NRPLIST_LIST)->Refresh();
}


SI32 CNRPList::GetAccuRPFromLevel( SI32 Level )
{
	SI32 sum = 0;

	for( SI32 i = 21 ; i <= Level; i++ )
	{
		sum += pclClient->GetRecommendPointPerLevel(i);
	}

	return sum;
	
}


void CNRPList::Hide()
{
	CControllerMgr::Show(false);

}

void CNRPList::Show()
{
	CControllerMgr::Show(true);

}
