#include "../Client/ninterface/NPartyhallDlg/DLG_PartyHall_In_Invite_Chief.h"
#include "../../Client/NInterface/NNewInventory/NNewInventory.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

NPartyHall_In_Invite_Chief_Dlg::NPartyHall_In_Invite_Chief_Dlg()
{

}

NPartyHall_In_Invite_Chief_Dlg::~NPartyHall_In_Invite_Chief_Dlg()
{

}

void NPartyHall_In_Invite_Chief_Dlg::Create( void )
{
	if ( IsCreate() )
		return;


	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NStructureDlg/NPartyHall/DLG_PartyHall_In_Invite_Chief.ddf" ) );
	file.CreatePopupDialog( this, PARTYHALL_IN_INVITE_CHIEF_DLG, TEXT( "dialog_invite_chief" ), StaticNPartyHall_In_Invite_Chief_DlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,		PARTYHALL_INVITE_LIST,		this ), PARTYHALL_INVITE_LIST,		TEXT( "listbox_invite_list" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		PARTYHALL_INVITE_DELETE,	this ), PARTYHALL_INVITE_DELETE,	TEXT( "button_delete" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		PARTYHALL_INVITE_BUTTON,	this ), PARTYHALL_INVITE_BUTTON,	TEXT( "button_invite" ) );	

	CList* pList = m_InterfaceFactory.GetList(PARTYHALL_INVITE_LIST);

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText[2];
	pText[0] = "아이디";	GetTxtFromMgr(1831);
	pText[1] = "레벨";		GetTxtFromMgr(1831);

	pList->SetColumn( 0,  140,   pText[ 0 ] );  
	pList->SetColumn( 1,  72,    pText[ 1 ] );        

	pList->Refresh();
}

void CALLBACK NPartyHall_In_Invite_Chief_Dlg::StaticNPartyHall_In_Invite_Chief_DlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NPartyHall_In_Invite_Chief_Dlg *pThis = (NPartyHall_In_Invite_Chief_Dlg*) pControllerMgr;
	pThis->NPartyHall_In_Invite_Chief_DlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NPartyHall_In_Invite_Chief_Dlg::NPartyHall_In_Invite_Chief_DlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient*	pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case PARTYHALL_INVITE_LIST:
			{
				
			}
			break;
		case PARTYHALL_INVITE_DELETE:
			{
				
			}
			break;
		case PARTYHALL_INVITE_BUTTON:
			{
				CList* pList = m_InterfaceFactory.GetList(PARTYHALL_INVITE_LIST);
								
				SI32 Index = pList->GetSelectedIndex();
				
				// 연회에 참여하려는 신청자 목록을 요청.
				cltMsg clMsg(GAMEMSG_REQUEST_PARTYHALL_INVITATION, sizeof(SI32), (BYTE*)&m_ResvPersonID[Index] );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);	
			}
			break;
	}
}

bool NPartyHall_In_Invite_Chief_Dlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	cltClient* pclclient = (cltClient*)pclClient;

	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	void*	pvoidControl  = m_InterfaceFactory.GetControl( pclClickAndDrop->m_siDstControlID );

	switch ( pclClickAndDrop->m_siDstControlID )
	{
		default:	break;
	}

	return true;
}

void NPartyHall_In_Invite_Chief_Dlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	default:	break;
	}
}

void NPartyHall_In_Invite_Chief_Dlg::SendServerMsgBufferDelete( SI32 _ClassType, void* _ClassData )
{
	switch( _ClassType )
	{
		//case XXX:	delete XXX_ClassData; break;	
		default: break;
	}
}

void NPartyHall_In_Invite_Chief_Dlg::SetEntranceApplicationList( Entrance_Application_Info* Entrance_Application_List )
{
	CList* pList = m_InterfaceFactory.GetList( PARTYHALL_INVITE_LIST );
	pList->Clear();

	TCHAR szbuffer[4];
	stListBoxItemData tmpListBoxItemData;
	for( SI16 i = 0; i < MAX_PARTYHALL_APPLICATION_LIST; i++)
	{
		if( 0 == Entrance_Application_List[i].m_siLevel )
		{		
			break;
		}

		m_ResvPersonID[i] = Entrance_Application_List[i].m_siPersonID;
   
		tmpListBoxItemData.Init();

		tmpListBoxItemData.Set( 0, Entrance_Application_List[i].m_szID );

		StringCchPrintf( szbuffer, 10, TEXT("%d"), Entrance_Application_List[i].m_siLevel );
		tmpListBoxItemData.Set( 1, szbuffer );
		
		pList->AddItem( &tmpListBoxItemData );
	}
	pList->Refresh();
}
