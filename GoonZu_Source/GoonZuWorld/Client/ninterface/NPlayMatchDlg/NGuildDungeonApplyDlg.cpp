#include "NGuildDungeonApplyDlg.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../common/Char/CharManager/CharManager.h"
#include "../../Common/Map/MapManager/MapManager.h"
#include "../../Client/Client.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


extern cltCommonLogic* pclClient;

CNGuildDungeonApplyDlg::CNGuildDungeonApplyDlg()
{
	Init();
}

CNGuildDungeonApplyDlg::~CNGuildDungeonApplyDlg()
{
	Destroy();
}

void CNGuildDungeonApplyDlg::Init()
{
	m_siSelectedMapIndex = 0;

}

void CNGuildDungeonApplyDlg::Destroy()
{

}

void CNGuildDungeonApplyDlg::Create()
{
	if ( IsCreate() ) return;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NPlayMatchDlg/Dlg_NGuildDungeonApply.ddf"));
	file.CreatePopupDialog( this, NGUILDDUNGEONAPPLY_DLG, "dialog_guilddungeonapply", StaticGuildDungeonApplyDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX , NGUILDDUNGEONAPPLY_COMBO_DUNGEON, this) ,NGUILDDUNGEONAPPLY_COMBO_DUNGEON, TEXT("combobox_dungeon"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST , NGUILDDUNGEONAPPLY_LIST_STATUS, this) ,NGUILDDUNGEONAPPLY_LIST_STATUS, TEXT("listbox_status"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGUILDDUNGEONAPPLY_BUTTON_APPLY, this)  , NGUILDDUNGEONAPPLY_BUTTON_APPLY , TEXT("button_apply"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGUILDDUNGEONAPPLY_BUTTON_HELP, this)  , NGUILDDUNGEONAPPLY_BUTTON_HELP , TEXT("button_help"));


	CList* pList = m_InterfaceFactory.GetList(NGUILDDUNGEONAPPLY_LIST_STATUS);

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(6548);
	pList->SetColumn(0,100,pText);
	pText = GetTxtFromMgr(6731);	
	pList->SetColumn(1,90,pText);
	pList->Refresh();

	SI32 rankType = pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.GetRankType();
	if ( rankType != GUILDRANKTYPE_MASTER  )
	{
        (m_InterfaceFactory.GetButton(NGUILDDUNGEONAPPLY_BUTTON_APPLY))->Enable( false );
	}

	SetComboBox();
	if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar ))
	{
		 (m_InterfaceFactory.GetButton(NGUILDDUNGEONAPPLY_BUTTON_APPLY))->Show( false );
	}
}

void CALLBACK CNGuildDungeonApplyDlg::StaticGuildDungeonApplyDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGuildDungeonApplyDlg *pThis = (CNGuildDungeonApplyDlg*)pControllerMgr;
	pThis->NGuildDungeonApplyDlgProc( nEvent, nControlID, pControl);
}

void CNGuildDungeonApplyDlg::NGuildDungeonApplyDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NGUILDDUNGEONAPPLY_COMBO_DUNGEON:
		{
			if ( EVENT_COMBOBOX_SELECTION_CHANGED == nEvent )
			{
				m_siSelectedMapIndex = 0;

				CComboBox* pCombo = m_InterfaceFactory.GetComboBox(NGUILDDUNGEONAPPLY_COMBO_DUNGEON);

				SI32 selectedindex = pCombo->GetSelectedIndex();

				if ( selectedindex >= 0 )
				{
					m_siSelectedMapIndex = pCombo->GetParam(selectedindex);
					RefreshApplyList();	
				}
				// 해당 인덱스에 대한 tp10d을 요구 한다.
					
			}
		}
		break;
	case NGUILDDUNGEONAPPLY_BUTTON_APPLY:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				ApplyGuildDungeon();
			}
		}
		break;
	case NGUILDDUNGEONAPPLY_BUTTON_HELP:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				TCHAR* pText =GetTxtFromMgr(6930);
				TCHAR* pTitle = GetTxtFromMgr(6931);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			}
		}
	}

}

void CNGuildDungeonApplyDlg::Action()
{

}

void CNGuildDungeonApplyDlg::SetComboBox()
{
	CComboBox* pCombo = m_InterfaceFactory.GetComboBox(NGUILDDUNGEONAPPLY_COMBO_DUNGEON);
	stComboBoxItemData	ItemData;

	for ( SI32 mapindex = 0; mapindex < MAX_MAP_NUMBER ; mapindex++)
	{
		if ( pclClient->pclMapManager->IsGuildMap(mapindex) )
		{
			TCHAR* mapname = pclClient->pclMapManager->GetMapName(mapindex);

			ItemData.Init();
			ItemData.Set(mapindex, mapname);

			pCombo->AddItem( &ItemData );
		}
	}
	pCombo->Refresh();
}
void CNGuildDungeonApplyDlg::RefreshApplyList()
{
	if ( m_siSelectedMapIndex <=0 )
		return;

	// 서버로 요청
	cltGameMsgRequest_GuildDungeon_ApplyList clInfo(m_siSelectedMapIndex);
	cltMsg clMsg(GAMEMSG_REQUEST_GUILDDUNGEON_APPLYLIST, sizeof(clInfo), (BYTE*)&clInfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CNGuildDungeonApplyDlg::SetApplyList(cltGameMsgResponse_GuildDungeon_ApplyList* pclInfo )
{

	// GP에 따라서 내림차순으로 정렬한다.
	SI32 tempbuf[ MAX_GUILDDUNGEONAPPLY ];			// 정렬된 인덱스를 저장

	for(SI32 i = 0; i < MAX_GUILDDUNGEONAPPLY; ++i )
	{
		tempbuf[ i ] = i;
	}

	for(SI32 i = 0; i < MAX_GUILDDUNGEONAPPLY - 1; i++)
	{
		for(SI32 j = i + 1; j < MAX_GUILDDUNGEONAPPLY; j++)
		{
			int cmpindex1 = tempbuf[ i ];
			int cmpindex2 = tempbuf[ j ];

			if(pclInfo->siGuildPoint[cmpindex1] < pclInfo->siGuildPoint[cmpindex2])
			{
				int temp = tempbuf[ i ];
				tempbuf[ i ] = tempbuf[ j ];
				tempbuf[ j ] = temp;
			}
		}
	}

	CList* pList = m_InterfaceFactory.GetList(NGUILDDUNGEONAPPLY_LIST_STATUS);
	stListBoxItemData itemData;

	TCHAR buffer[16];
	SI32  index = 0;

	pList->Clear();

	for ( SI32 i=0; i<MAX_GUILDDUNGEONAPPLY; i++ )
	{
		index = tempbuf[i];
		
		TCHAR* guildName = pclInfo->szGuildName[index];
		if( guildName == NULL || _tcscmp( guildName, TEXT("")) == 0 )
			continue;

		itemData.Init();
		itemData.Set( 0, guildName );
		_itot(pclInfo->siGuildPoint[index], buffer, 10 );
		itemData.Set( 1, buffer );

		pList->AddItem(&itemData);
	}

	pList->Refresh();
}
void CNGuildDungeonApplyDlg::ApplyGuildDungeon()
{
	cltClient* pclclient = (cltClient*)pclClient;
	TCHAR* pTitle = GetTxtFromMgr(6735);
	TCHAR* pText = GetTxtFromMgr(6736);

	TCHAR buffer[256];
	StringCchPrintf( buffer, 256, pText, pclClient->pclMapManager->GetMapName( m_siSelectedMapIndex ) );

	// TODO : 마스터가 아닐경우 체크 해줘야함

	stMsgBox MsgBox;
	MsgBox.Set( this, pTitle,buffer, MSGBOX_TYPE_YESNO, 0 );
	pclclient->SetMsgBox( &MsgBox, (BYTE*)&m_siSelectedMapIndex, sizeof(SI32) );
}
void CNGuildDungeonApplyDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
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
				SI32 mapindex = *((SI32*)pData);

				cltGameMsgRequest_GuildDungeon_Apply clinfo( mapindex );
				cltMsg clMsg(GAMEMSG_REQUEST_GUILDDUNGEON_APPLY, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
			}
		}
		break;
	}
}
