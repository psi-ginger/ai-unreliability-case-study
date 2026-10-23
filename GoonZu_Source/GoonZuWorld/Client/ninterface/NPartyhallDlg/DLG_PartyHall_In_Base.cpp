#include "../Client/ninterface/NPartyhallDlg/DLG_PartyHall_In_Base.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/NInterface/NNewInventory/NNewInventory.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../Client/NStructureDlg/Feast/NFeastDlg.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"

NPartyHall_In_BaseDlg::NPartyHall_In_BaseDlg()
{
	m_siMenu = MENU_USER;
}

NPartyHall_In_BaseDlg::~NPartyHall_In_BaseDlg()
{

}

void NPartyHall_In_BaseDlg::Create( void )
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NStructureDlg/NPartyHall/DLG_PartyHall_In_Base.ddf" ) );
	file.CreatePopupDialog( this, PARTYHALL_IN_BASE_DLG, TEXT( "dialog_partyhall_in_base" ), StaticNPartyHall_In_BaseDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	 PARTYHALL_BASE_EDIT_EXPLAIN,	this ), PARTYHALL_BASE_EDIT_EXPLAIN,	TEXT( "editbox_party_reason" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  PARTYHALL_BASE_BUTTON_MENU,	this ), PARTYHALL_BASE_BUTTON_MENU,		TEXT( "button_chief" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  PARTYHALL_BASE_BUTTON_CLOSE,	this ), PARTYHALL_BASE_BUTTON_CLOSE,	TEXT( "button_close" ) );	

	if( false == m_NPartyHall_In_Costume_User_Dlg.IsCreate() )
	{
		m_NPartyHall_In_Costume_User_Dlg.Create( this );
		m_NPartyHall_In_Costume_User_Dlg.Show( true );
	}

	if( false == m_NPartyHall_In_Firecracker_Chief_Dlg.IsCreate() )
	{
		m_NPartyHall_In_Firecracker_Chief_Dlg.Create( this );
		m_NPartyHall_In_Firecracker_Chief_Dlg.Show( false );
	}

	SetReason(); 
}

void CALLBACK NPartyHall_In_BaseDlg::StaticNPartyHall_In_BaseDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NPartyHall_In_BaseDlg *pThis = (NPartyHall_In_BaseDlg*) pControllerMgr;
	pThis->NPartyHall_In_BaseDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NPartyHall_In_BaseDlg::NPartyHall_In_BaseDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case PARTYHALL_BASE_BUTTON_CLOSE:
		{
			DeleteDialog();
        }
		break;
	case PARTYHALL_BASE_BUTTON_MENU:
		{
			MenuChange();			
		}
		break;
	}
}

bool NPartyHall_In_BaseDlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
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

void NPartyHall_In_BaseDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
		case 0:
			{
				if(RetValue)
				{
					SI16 Index = *pData;
					cltGameMsgRequest_FeastUseEffect sendmsg( &Index, 1 );

					cltMsg clMsg(GAMEMSG_REQUEST_PARTYHALL_USEEFFECT, sizeof(sendmsg), (BYTE*)&sendmsg );
					pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
			break;

		case 1:
			{
				if(RetValue)
				{
					cltGameMsgRequest_PartyHall_GiveItem sendmsg;
					cltMsg clMsg(GAMEMSG_REQUEST_PARTYHALL_GIVEITEM, sizeof(sendmsg), (BYTE*)&sendmsg);
					pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
			break;

			
		default:	break;
	}
}

void NPartyHall_In_BaseDlg::SendServerMsgBufferDelete( SI32 _ClassType, void* _ClassData )
{
	switch( _ClassType )
	{
		//case XXX:	delete XXX_ClassData; break;	
		default: break;
	}
}

void NPartyHall_In_BaseDlg::MenuChange( void )
{
	if( MENU_CHIEF == m_siMenu )
	{
		m_siMenu = MENU_USER;
		
		m_NPartyHall_In_Costume_User_Dlg.Show( true );
		m_NPartyHall_In_Firecracker_Chief_Dlg.Show( false );
		
		m_InterfaceFactory.GetButton( PARTYHALL_BASE_BUTTON_MENU )->SetText(GetTxtFromMgr(3901));
	}
	else
	{	
		cltClient* pclclient = (cltClient*)pclClient;
		
		SI32 MapIndexArray[MAX_PARTYHALL_MAP];
		ZeroMemory(MapIndexArray, sizeof(MapIndexArray));
		
		// 콤보 박스에 아이템의 종류를 설정한다. 
		for( SI32 i = 0; i < MAX_PARTYHALL_MAP; i++ )
		{		
			MapIndexArray[i] = PARTYHALL_START_MAP_INDEX + i;
		}

		SI32 PartyHallMapIndex = -1;
		for( SI32 i = 0; i < MAX_PARTYHALL_MAP; i++ )
		{		
			if(pclClient->pclCM->CR[1]->GetMapIndex() == MapIndexArray[i])
			{
				PartyHallMapIndex = i;
				break;
			}
		}

		if(-1 == PartyHallMapIndex)
			return;
		
		if( pclClient->pclCM->CR[1]->pclCI->GetPersonID() != 
			pclClient->pclFeastManager->clPartyHallResvInfo[PartyHallMapIndex][pclclient->sTime.wHour].m_siPersonID )
		{
			return;   
		}

		m_siMenu = MENU_CHIEF;  
		
		m_NPartyHall_In_Costume_User_Dlg.Show( false );
		m_NPartyHall_In_Firecracker_Chief_Dlg.Show( true );


		m_InterfaceFactory.GetButton( PARTYHALL_BASE_BUTTON_MENU )->SetText(GetTxtFromMgr(1444));
	}
}

void NPartyHall_In_BaseDlg::SetReason(void)
{
	CEdit* pEdit = m_InterfaceFactory.GetEdit(PARTYHALL_BASE_EDIT_EXPLAIN);
	
	
		
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 MapIndexArray[MAX_PARTYHALL_MAP];
	ZeroMemory(MapIndexArray, sizeof(MapIndexArray));
	

	// 콤보 박스에 아이템의 종류를 설정한다. 
	for( SI32 i = 0; i < MAX_PARTYHALL_MAP; i++ )
	{		
		MapIndexArray[i] = PARTYHALL_START_MAP_INDEX + i;
	}

	SI32 PartyHallMapIndex = -1;
	for( SI32 i = 0; i < MAX_PARTYHALL_MAP; i++ )
	{		
		if(pclClient->pclCM->CR[1]->GetMapIndex() == MapIndexArray[i])
		{
			PartyHallMapIndex = i;
			break;
		}
	}

	if(-1 == PartyHallMapIndex)
		return;

	TCHAR* pReason = pclClient->pclFeastManager->clPartyHallResvInfo[PartyHallMapIndex][pclclient->sTime.wHour].m_szReason;	
		
	pEdit->SetText(pReason);
}


