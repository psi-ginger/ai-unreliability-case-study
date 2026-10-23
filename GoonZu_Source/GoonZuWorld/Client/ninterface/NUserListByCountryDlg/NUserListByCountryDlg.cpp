#include "NUserListByCountryDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client/Client.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/CountryMgr/CountryManager.h"
#include "../../CommonLogic/MsgType-Person.h"

#include "Char/CharManager/CharManager.h"
#include "..\CommonLogic\Msg\MsgType-Friend.h"
#include "..\CommonLogic\Msg/MsgType-ItemMall.h"

extern cltCommonLogic* pclClient;
CNUserListByCountryDlg::CNUserListByCountryDlg()	
{
	m_pStaticContinent		= NULL;
	m_pComboBoxContinent	= NULL;
	m_pStaticCountry		= NULL;
	m_pComboBoxCountry		= NULL;
	m_pListUser				= NULL;
	m_pEditGreet			= NULL;
	m_pButtonWhisper		= NULL;
	m_pButtonAddfriend		= NULL;
	m_pButtonWarp			= NULL;
	m_pCountryMgr			= NULL;
	m_pButtonChange			= NULL;

    for( SI32 i = 0 ; i<MAX_USERLIST_NUM; i++ )
	{
		siPersonID[i]		= 0;
	}
}
CNUserListByCountryDlg::~CNUserListByCountryDlg()
{
	SAFE_DELETE( m_pStaticContinent );
	SAFE_DELETE( m_pComboBoxContinent );
	SAFE_DELETE( m_pStaticCountry );
	SAFE_DELETE( m_pComboBoxCountry );
	SAFE_DELETE( m_pListUser );
	SAFE_DELETE( m_pEditGreet );
	SAFE_DELETE( m_pButtonWhisper );
	SAFE_DELETE( m_pButtonAddfriend );
	SAFE_DELETE( m_pButtonWarp );
	SAFE_DELETE( m_pButtonChange );

}
void CNUserListByCountryDlg::Create()
{
	m_pStaticContinent		= new CStatic( this );
	m_pComboBoxContinent	= new CComboBox( this );
	m_pStaticCountry		= new CStatic( this );
	m_pComboBoxCountry		= new CComboBox( this );
	m_pListUser				= new CList( this );
	m_pEditGreet			= new CEdit( this );
	m_pButtonWhisper		= new CButton( this );
	m_pButtonAddfriend		= new CButton( this );
	m_pButtonWarp			= new CButton( this );
	m_pButtonChange			= new CButton( this );

	CInterfaceFile file;

	file.LoadFile( TEXT( "NInterface/Data/NUserListByCountry/Dlg_NUserListByCountry.ddf" ) );
	file.CreatePopupDialog( this, NUSERLISTBYCOUNTRY_DLG, TEXT("dialog_userlistbycountry"), StaticCallBackDialogNUserListByCountry,false );

	file.CreateControl( m_pStaticContinent, NUSERLISTBYCOUNTRY_STATIC_CONTINENT, TEXT("statictext_continent") );
	file.CreateControl( m_pComboBoxContinent, NUSERLISTBYCOUNTRY_COMBO_CONTINENT, TEXT("combobox_continent") );
	file.CreateControl( m_pStaticCountry, NUSERLISTBYCOUNTRY_STATIC_COUNTRY, TEXT("statictext_country") );
	file.CreateControl( m_pComboBoxCountry, NUSERLISTBYCOUNTRY_COMBO_COUNTRY, TEXT("combobox_country") );
	file.CreateControl( m_pListUser , NUSERLISTBYCOUNTRY_LIST_USER, TEXT("listbox_userlist") );
	file.CreateControl( m_pEditGreet , NUSERLISTBYCOUNTRY_EDIT_GREET, TEXT("editbox_greet") );
	file.CreateControl( m_pButtonWhisper , NUSERLISTBYCOUNTRY_BUTTON_WHISPER, TEXT("button_whisper") );
	file.CreateControl( m_pButtonAddfriend , NUSERLISTBYCOUNTRY_BUTTON_ADDFRIEND, TEXT("button_addfriend") );
	file.CreateControl( m_pButtonWarp , NUSERLISTBYCOUNTRY_BUTTON_WARP, TEXT("button_warp") );
	file.CreateControl( m_pButtonChange, NUSERLISTBYCOUNTRY_BUTTON_CHANGE, TEXT("button_change"));


	stComboBoxItemData	data;
	TCHAR* pText = GetTxtFromMgr(6280);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6281);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6282);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6283);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6284);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6285);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	if ( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		m_pComboBoxContinent->Enable( false );
		m_pComboBoxContinent->Show( false );
		m_pStaticContinent->Show( false );
		m_pCountryMgr = pclClient->pclCountryManagerJapan;
		SetComboBoxCountry();
	}
	else
	{
		m_pCountryMgr = pclClient->pclCountryManager;
	}

	pText = GetTxtFromMgr(1556);
	m_pListUser->SetColumn(0, 140, pText);
	pText= GetTxtFromMgr(1506);
	m_pListUser->SetColumn(1,  60, pText);
	pText= GetTxtFromMgr(3101);
	m_pListUser->SetColumn(2, 140, pText);
	m_pListUser->SetBorder(TRUE);
	m_pListUser->SetBKColorUse(TRUE);
	m_pListUser->Refresh();
}
void CALLBACK CNUserListByCountryDlg::StaticCallBackDialogNUserListByCountry( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNUserListByCountryDlg *pThis = (CNUserListByCountryDlg*)pControllerMgr;
	pThis->CallBackDialogNUserListByCountry( nEvent, nControlID, pControl );

}
void CALLBACK CNUserListByCountryDlg::CallBackDialogNUserListByCountry( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		DeleteDialog();
		break;
	case NUSERLISTBYCOUNTRY_COMBO_CONTINENT:
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			// 대륙을 결정하면 대륙에 속하는 나라를 콤보박스에 입력
			SetComboBoxCountry();
			break;
		}
		break;
	case NUSERLISTBYCOUNTRY_COMBO_COUNTRY:
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			// 나라를 결정하면 나라에 해당하는 유저를 보여준다.
			if ( m_pComboBoxCountry->GetSelectedIndex() != -1 )
			{
				for( SI16 i = 0 ; i<MAX_USERLIST_NUM ; i++ )
				{
					siPersonID[i] = 0;
				}
				m_pListUser->Clear();
				m_pListUser->Refresh();


                ShowUserList();
			}
			break;
		}
		break;
	case NUSERLISTBYCOUNTRY_BUTTON_WHISPER:
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			// 귓말 귓말보내기
			SendWhisper();
			break;
		}
		break;
	case NUSERLISTBYCOUNTRY_BUTTON_ADDFRIEND:
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			// 친구 추가
			AddFriend();
			break;
		}
		break;
	case NUSERLISTBYCOUNTRY_BUTTON_WARP:
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			WarpToPerson();
			break;
		}
		break;
	case NUSERLISTBYCOUNTRY_BUTTON_CHANGE:
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			if ( pclclient->m_pDialog[NSETCOUNTRY_DLG] == NULL )
			{
				pclclient->CreateInterface( NSETCOUNTRY_DLG );
			}
		}
	}
}

void CNUserListByCountryDlg::Action()
{

}

void CNUserListByCountryDlg::SetComboBoxCountry()
{
	m_pComboBoxCountry->Clear();

	// 일본판일경우 대륙은 자동으로 선택한다.
	SI32 continentunique = 0;
	SI32 selectedindex = m_pComboBoxContinent->GetSelectedIndex();

	if ( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		continentunique = 1;
	}
	else
	{
		continentunique = selectedindex + 1;
	}

	stComboBoxItemData	data;
	for( SI16 i=0;  i<MAX_COUNTRY ; i++)
	{
		if( m_pCountryMgr->siContinentUnique[i] == continentunique )	
		{
			data.Set( m_pCountryMgr->szCountryName[i] );
			m_pComboBoxCountry->AddItem( &data );
		}
	}
}
void CNUserListByCountryDlg::ShowUserList()
{
	SI32 selectedindex = m_pComboBoxCountry->GetSelectedIndex();
	TCHAR* country = m_pComboBoxCountry->GetText( selectedindex );
	SI32 countryunique = m_pCountryMgr->GetUniqueFromCountry( country );

	cltGameMsgRequest_UserListByCounty clinfo( countryunique );
	cltMsg clMsg(GAMEMSG_REQUEST_USERLISTBYCOUNTRY, sizeof(cltGameMsgRequest_UserListByCounty), (BYTE*)&clinfo);

	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
}
void CNUserListByCountryDlg::AddFriend()
{
	TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");

	TCHAR *tmpText = NULL;
	tmpText = m_pListUser->GetText( m_pListUser->GetSelectedIndex(), 0 );
	if(tmpText)
	{
		if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
		StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
	}

	Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';

	if ( _tcslen( Buffer ) < 1 )
	{
		return ;
	}					

	if ( _tcscmp( Buffer, pclClient->pclCM->CR[1]->GetName() ) == 0 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2458);
		TCHAR* pText = GetTxtFromMgr(2459);
		((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return ;
	}

	cltGameMsgRequest_FriendAdd clFriendAdd( FRIEND_GROUP_UNKNOWN, Buffer );
	cltMsg clMsg( GAMEMSG_REQUEST_FRIENDADD, sizeof(clFriendAdd), (BYTE*)&clFriendAdd );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}
void CNUserListByCountryDlg::SendWhisper()
{
	TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");
	TCHAR *tmpText = NULL;
	tmpText = m_pListUser->GetText( m_pListUser->GetSelectedIndex(), 0 );
	if(tmpText)
	{
		if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
		StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
	}

	Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';

	SI32 PersonID = siPersonID[m_pListUser->GetSelectedIndex()];

	((cltClient*)pclClient)->RequestSecretChatByPersonID( PersonID );

}
void CNUserListByCountryDlg::WarpToPerson()
{
	TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");
	TCHAR *tmpText = NULL;
	tmpText = m_pListUser->GetText( m_pListUser->GetSelectedIndex(), 0 );
	if(tmpText)
	{
		if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
		StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
	}
	Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';

	SI32 PersonID = siPersonID[ m_pListUser->GetSelectedIndex() ];

	if ( PersonID > 0 )
	{
		cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

		if ( !pclchar->pclCI->clBI.bUseNoLimitTicketWarp &&
			pclClient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false &&
			pclClient->pclCM->CR[1]->pclCI->clBI.uiAngelSwitch == false &&
			pclClient->pclCM->CR[1]->pclCI->clBI.bPCRoom == false &&
			pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_KING  &&
			pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER1 &&
			pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER2 &&
			pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER3 &&
			pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER4 &&
			pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER5 &&
			pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER6 &&
			( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) &&  
			pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FRIENDSHIP) ) == false &&
			(CONST_JOURNALIST_HAT_ITEMUNIQUE == pclchar->pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique)
			) 
		{
			// 순간이동이용권을 보유하고 있는지 확인한다. 
			SI32 itemunique = ITEMUNIQUE(24000) ;  // 순간이동이용권
			SI32 pos = pclchar->pclCI->clCharItem.FindItemInv(itemunique);
			if(pos < 0)
			{
				TCHAR* pTitle = GetTxtFromMgr(1413);
				TCHAR* pText = GetTxtFromMgr(2460);
				((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

				return ;
			}
		}

		// 감옥에서는 워프 불가 
		if( pclchar->clInnerStatus.IsStatus(INNERSTATUS_PRISON))	
		{
			TCHAR* pTitle = GetTxtFromMgr(1413);
			TCHAR* pText = GetTxtFromMgr(2259);
			((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			return  ;
		}	

		// 영자나라에 있으면 불가능
		if ( pclchar->GetMapIndex() == MAPINDEX_MASTERWORLD &&
			pclchar->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
		{
			return ;
		}



		TCHAR TEMP[256];
		TCHAR* pText = GetTxtFromMgr(2461);
		StringCchPrintf(TEMP, 256, pText, Buffer);

		pText = GetTxtFromMgr(1418);

		stMsgBox MsgBox;
		MsgBox.Set( this, pText, TEMP, MSGBOX_TYPE_YESNO, 0 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( PersonID );
		memcpy( TempBuffer, &PersonID, Size );
		((cltClient*)pclClient)->SetMsgBox( &MsgBox, TempBuffer, Size );
	}
}
void CNUserListByCountryDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 친구로 이동
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 PersonID = *((SI32*)pData);
				cltGameMsgRequest_FriendWarp clFriendWarp( PersonID, FRIENDWARP_MODE_FRIEND );
				cltMsg clMsg( GAMEMSG_REQUEST_FRIENDWARP, sizeof( clFriendWarp ), (BYTE*)&clFriendWarp );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}