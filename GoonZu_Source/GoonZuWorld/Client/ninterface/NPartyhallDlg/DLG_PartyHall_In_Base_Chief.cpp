#include "../Client/ninterface/NPartyhallDlg/DLG_PartyHall_In_Base.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/NInterface/NNewInventory/NNewInventory.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

NPartyHall_In_BaseDlg::NPartyHall_In_BaseDlg()
{

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
	file.CreatePopupDialog( this, PARTYHALL_IN_COSTUME_USER_DLG, TEXT( "dialog_partyhall_in_base" ), StaticNPartyHall_In_BaseDlgProc );



}

void CALLBACK NPartyHall_In_BaseDlg::StaticNPartyHall_In_BaseDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NPartyHall_In_BaseDlg *pThis = (NPartyHall_In_BaseDlg*) pControllerMgr;
	pThis->NPartyHall_In_BaseDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NPartyHall_In_BaseDlg::NPartyHall_In_BaseDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

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
