#include "NPostOfficeGroupListDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"
#include "Char\CharManager\CharManager.h"

NPostOfficeGroupListDlg::NPostOfficeGroupListDlg()
{

}

NPostOfficeGroupListDlg::~NPostOfficeGroupListDlg()
{

}



void NPostOfficeGroupListDlg::Create()
{
	if( IsCreate() )	
		return;
		
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NStructureDlg/DLG_PostOfficeGroupList.ddf" ) );
	file.CreatePopupDialog( this, NPOSTOFFICE_GROUP_LIST_DLG, TEXT( "dialog_PostOfficeGroupList" ), StaticNPostOfficeGroupListDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,	NPOSTOFFICE_GROUP_LIST_USER_LIST,				this ), NPOSTOFFICE_GROUP_LIST_USER_LIST,				TEXT( "listbox_user_list" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NPOSTOFFICE_GROUP_LIST_RECV_NAME_STATIC,		this ), NPOSTOFFICE_GROUP_LIST_RECV_NAME_STATIC,		TEXT( "statictext_recv_name" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NPOSTOFFICE_GROUP_LIST_RECV_NAME_EDIT,			this ), NPOSTOFFICE_GROUP_LIST_RECV_NAME_EDIT,			TEXT( "editbox_recv_user" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NPOSTOFFICE_GROUP_LIST_ADD_BUTTON,				this ), NPOSTOFFICE_GROUP_LIST_ADD_BUTTON,				TEXT( "button_add" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NPOSTOFFICE_GROUP_LIST_DEL_BUTTON,				this ), NPOSTOFFICE_GROUP_LIST_DEL_BUTTON,				TEXT( "button_del" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NPOSTOFFICE_GROUP_LIST_REQUEST_STATIC,			this ), NPOSTOFFICE_GROUP_LIST_REQUEST_STATIC,			TEXT( "statictext_request_group" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NPOSTOFFICE_GROUP_LIST_REQUEST_FRIEND_BUTTON,	this ), NPOSTOFFICE_GROUP_LIST_REQUEST_FRIEND_BUTTON,	TEXT( "button_ friend" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NPOSTOFFICE_GROUP_LIST_REQUEST_GUILD_BUTTON,	this ), NPOSTOFFICE_GROUP_LIST_REQUEST_GUILD_BUTTON,	TEXT( "button_guild" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NPOSTOFFICE_GROUP_LIST_REQUEST_VILLAGE_BUTTON,	this ), NPOSTOFFICE_GROUP_LIST_REQUEST_VILLAGE_BUTTON,	TEXT( "button_village" ) );	

	CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);

	TCHAR* pText = GetTxtFromMgr(1556);
	pList->SetColumn( 0, 130, pText );

	pText = GetTxtFromMgr(1506);
	pList->SetColumn( 1,  70, pText );

	pText = GetTxtFromMgr(3101);
	pList->SetColumn( 2, 110, pText );

	pList->Refresh();
}



void CALLBACK NPostOfficeGroupListDlg::StaticNPostOfficeGroupListDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NPostOfficeGroupListDlg *pThis = (NPostOfficeGroupListDlg*)pControllerMgr;
	pThis->NPostOfficeGroupListDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NPostOfficeGroupListDlg::NPostOfficeGroupListDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )

{
	switch( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case NPOSTOFFICE_GROUP_LIST_USER_LIST:
			{
				switch( nEvent )
				{
					case EVENT_LISTBOX_SELECTION:
						{
							CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);
							if(NULL == pList)	return;

							CEdit* pEdit = m_InterfaceFactory.GetEdit(NPOSTOFFICE_GROUP_LIST_RECV_NAME_EDIT);
							if(NULL == pEdit)	return;

							stListBoxItemData* pItemData = pList->GetItemData( pList->GetSelectedIndex() );

							pEdit->SetText(pItemData->strText[0]);
						}
						break;
				}
			}
			break;
		case NPOSTOFFICE_GROUP_LIST_ADD_BUTTON:	// 직접입력.
			{
				CEdit* pEdit = m_InterfaceFactory.GetEdit(NPOSTOFFICE_GROUP_LIST_RECV_NAME_EDIT);

				if( 0 >= _tcslen( pEdit->GetText() ) ) 
					return;

				if( 0 ==_tcsicmp( pclClient->pclCM->CR[1]->GetName(), pEdit->GetText()) )
					return;

				CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);
				
				// 같은 이름은 중복으로 등록이 안되게.
				stListBoxItemData* pItemData = NULL;
				for(SI32 Count = 0; Count < pList->GetListNum(); ++Count )
				{
					pItemData = pList->GetItemData( Count );
					if( 0 ==_tcscmp( pEdit->GetText(), pItemData->strText[0]) )
						return;
				}

				cltGameMsgRequest_Char_Name_Check sendMsg(pEdit->GetText());
				cltMsg clMsg( GAMEMSG_REQUEST_CHAR_NAME_CHECK, sizeof(cltGameMsgRequest_Char_Name_Check), (BYTE*)&sendMsg);
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
			break;
		case NPOSTOFFICE_GROUP_LIST_DEL_BUTTON:	// 리스트에서 삭제.
			{
				CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);
				if(NULL == pList)	return;

				CEdit* pEdit = m_InterfaceFactory.GetEdit(NPOSTOFFICE_GROUP_LIST_RECV_NAME_EDIT);
				if(NULL == pEdit)	return;

				NTCHARString64 strName( pEdit->GetText() );
				// 이름입력칸에 이름이 없으면 선택된 리스트를 지우고
				if ( true == strName.IsEmpty() )
				{
					pList->DelItemIndex( pList->GetSelectedIndex() );
				}
				// 이름이 있을때는 같은 이름을 지운다
				else
				{
					stListBoxItemData*	pItemData		= NULL;
					SI32				siSelectedIndex = -1;
					for(SI32 Count=0; Count<pList->GetListNum(); ++Count )
					{
						pItemData = pList->GetItemData( Count );
						if ( 0 == strName.Compare(pItemData->strText[0]) )
						{
							siSelectedIndex = Count;
							break;
						}
					}

					pList->DelItemIndex( siSelectedIndex );
				}
				
				pList->Refresh();
				pEdit->Clear();  
			}
			break;
		case NPOSTOFFICE_GROUP_LIST_REQUEST_FRIEND_BUTTON:
			{
				CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);
				pList->Clear();
				pList->Refresh();

				cltGameMsgRequest_PostOffice_User_List_Group sendMsg( cltGameMsgRequest_PostOffice_User_List_Group::FRIEND ); 
				cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_USER_LIST_GROUP, sizeof(cltGameMsgRequest_PostOffice_User_List_Group), (BYTE*)&sendMsg);
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
			break;
		case NPOSTOFFICE_GROUP_LIST_REQUEST_GUILD_BUTTON:
			{
				CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);
				pList->Clear();
				pList->Refresh();

				cltGameMsgRequest_PostOffice_User_List_Group sendMsg( cltGameMsgRequest_PostOffice_User_List_Group::GUILD ); 
				cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_USER_LIST_GROUP, sizeof(cltGameMsgRequest_PostOffice_User_List_Group), (BYTE*)&sendMsg);
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
			break;
		case NPOSTOFFICE_GROUP_LIST_REQUEST_VILLAGE_BUTTON:
			{
				CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);
				pList->Clear();
				pList->Refresh();

				cltGameMsgRequest_PostOffice_User_List_Group sendMsg( cltGameMsgRequest_PostOffice_User_List_Group::VILLAGE ); 
				cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_USER_LIST_GROUP, sizeof(cltGameMsgRequest_PostOffice_User_List_Group), (BYTE*)&sendMsg);
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
			break;
		}
}

void NPostOfficeGroupListDlg::AddRecvUser( cltGameMsgResponse_Char_Add_Name_Check* pUser )
{
	CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);

	if( 0 > _tcslen(pUser->m_szAddPersonName) )	return;

	// 같은 이름은 중복으로 등록이 안되게.
	stListBoxItemData* pItemData = NULL;
	for(SI32 Count = 0; Count < pList->GetListNum(); ++Count )
	{
		pItemData = pList->GetItemData( Count );
		if( 0 == _tcsicmp( pUser->m_szAddPersonName, pItemData->strText[0]) )
			return;
	}
	
	TCHAR				buffer[8] = "";
	stListBoxItemData	tmpListBoxItemData;
	tmpListBoxItemData.Init();
	
	if(pUser->m_bConnect)
	{
		tmpListBoxItemData.siSmallIconFileIndex[ 0 ] = 0;
		tmpListBoxItemData.siSmallIconFontIndex[ 0 ] = 12; // 로그온 이미지 폰트 번호
	}
	else
	{
		tmpListBoxItemData.siSmallIconFileIndex[ 0 ] = 0;
		tmpListBoxItemData.siSmallIconFontIndex[ 0 ] = 11; // 로그온 이미지 폰트 번호
	}
	
	tmpListBoxItemData.Set( 0, pUser->m_szAddPersonName );
	
	_itot( pUser->m_CheckPersonLevel, buffer, 10 );
	tmpListBoxItemData.Set( 1, buffer );
	
	tmpListBoxItemData.Set( 2, pclClient->pclVillageManager->GetName(pUser->m_CheckPersonVillage) );
	pList->AddItem( &tmpListBoxItemData );
	pList->Refresh();
}

void NPostOfficeGroupListDlg::AddRecvUserGroup( cltGameMsgResponse_PostOffice_User_List_Group* pListGroup )
{
	CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);

	// List의 Item 모두 지우기
	pList->Clear();

	TCHAR			  buffer[8] = "";
	stListBoxItemData tmpListBoxItemData;
	for( SI16 i = 0; i < MAX_SEND_USER_NUM; i++)
	{
		if( 0 >= _tcslen(pListGroup->m_szAddPersonNameGroup[i]) )	
			break;		

		if( 0 ==_tcsicmp( pclClient->pclCM->CR[1]->GetName(), pListGroup->m_szAddPersonNameGroup[i]) )
			continue;
		
		tmpListBoxItemData.Init();

		if(pListGroup->m_bConnect[i])
		{
			tmpListBoxItemData.siSmallIconFileIndex[ 0 ] = 0;
			tmpListBoxItemData.siSmallIconFontIndex[ 0 ] = 12; // 로그온 이미지 폰트 번호
		}
		else
		{
			tmpListBoxItemData.siSmallIconFileIndex[ 0 ] = 0;
			tmpListBoxItemData.siSmallIconFontIndex[ 0 ] = 11; // 로그온 이미지 폰트 번호
		}

		tmpListBoxItemData.Set( 0, pListGroup->m_szAddPersonNameGroup[i] );
		
		_itot( pListGroup->m_siLevel[i], buffer, 10 );
		tmpListBoxItemData.Set( 1, buffer );

		tmpListBoxItemData.Set( 2, pclClient->pclVillageManager->GetName(pListGroup->m_siVillage[i]) );

		pList->AddItem( &tmpListBoxItemData );
	}

	pList->Refresh();
}

void NPostOfficeGroupListDlg::SendItem( SI16 ItemIndex, SI32 ItemUnique, SI16 ItemNum )
{
	TCHAR	m_ReceiverPersonNameGroup[MAX_SEND_USER_NUM][MAX_PLAYER_NAME];
	ZeroMemory( m_ReceiverPersonNameGroup, sizeof(m_ReceiverPersonNameGroup) );

	CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);
	if( NULL == pList )				return;
	if( 0 >= pList->GetListNum() )	return;

	stListBoxItemData* pItemData = NULL;

	for(SI32 Count = 0; Count < pList->GetListNum(); ++Count )
	{
		pItemData = pList->GetItemData( Count );
		memcpy( m_ReceiverPersonNameGroup[Count], pItemData->strText[0], sizeof(m_ReceiverPersonNameGroup[Count]) );
	}
	
	// 내가 전체 보내야할 아이템 수보다 보내려는 아이템 수가 많으면 리턴.
	SI16 UseItemTotalNum = pList->GetListNum() * ItemNum;
	SI16 MyInvenItemNum  = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ ItemIndex ].siItemNum;
	if( MyInvenItemNum < UseItemTotalNum )
	{
		cltClient *pclclient = (cltClient*)pclClient;	
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(8684) );
		return;
	}

	cltGameMsgRequest_PostOffice_SendItem_Group sendMsg( m_ReceiverPersonNameGroup[0], 
														 ItemIndex,
														 ItemUnique, 
														 ItemNum );
	cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDITEM_GROUP, sizeof(cltGameMsgRequest_PostOffice_SendItem_Group), (BYTE*)&sendMsg);
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

}

void NPostOfficeGroupListDlg::SendMessage( TCHAR* pMsg )
{
	TCHAR	m_ReceiverPersonNameGroup[MAX_SEND_USER_NUM][MAX_PLAYER_NAME];
	ZeroMemory( m_ReceiverPersonNameGroup, sizeof(m_ReceiverPersonNameGroup) );

	CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);
	if( NULL == pList )				return;
	if( 0 >= pList->GetListNum() )	return;

	stListBoxItemData* pItemData = NULL;

	for(SI32 Count = 0; Count < pList->GetListNum(); ++Count )
	{
		pItemData = pList->GetItemData( Count );
		memcpy( m_ReceiverPersonNameGroup[Count], pItemData->strText[0], sizeof(m_ReceiverPersonNameGroup[Count]) );
	}

	cltGameMsgRequest_PostOffice_SendMsg_Group sendMsg( m_ReceiverPersonNameGroup[0], pMsg );
	cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDMSG_GROUP, sizeof(cltGameMsgRequest_PostOffice_SendMsg_Group), (BYTE*)&sendMsg);
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
}

SI32 NPostOfficeGroupListDlg::GetListCountNum( void )
{
	CList* pList = m_InterfaceFactory.GetList(NPOSTOFFICE_GROUP_LIST_USER_LIST);
	if( NULL == pList )
	{
		return 0;
	}
		
	return pList->GetListNum();

}