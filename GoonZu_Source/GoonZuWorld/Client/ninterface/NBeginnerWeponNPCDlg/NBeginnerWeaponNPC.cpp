// 안쓴다.. 앞으로도 영원히 쓸일없다!
// CNQeustSelectItem 다이얼로그로 대체..

#include "NBeginnerWeaponNPC.h"

#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/Client.h"
#include "../../../Common/Char/CharManager/CharManager.h"

#include "Msg/MsgType-NPC.h"


extern cltCommonLogic* pclClient;

CNBeginnerWeaponNPCDlg::CNBeginnerWeaponNPCDlg()
{
	Init();
}

CNBeginnerWeaponNPCDlg::~CNBeginnerWeaponNPCDlg()
{
	Destroy();
}

void CNBeginnerWeaponNPCDlg::Init()
{
	m_siItemUnique[0] = ITEMUNIQUE(1);
	m_siItemUnique[1] = ITEMUNIQUE(100);
	m_siItemUnique[4] = ITEMUNIQUE(200);
	m_siItemUnique[2] = ITEMUNIQUE(300);
	m_siItemUnique[3] = ITEMUNIQUE(400);
	m_siItemUnique[5] = ITEMUNIQUE(500);

	m_siSelectedItemUnique = m_siItemUnique[0];
}

void CNBeginnerWeaponNPCDlg::Destroy()
{

}

void CNBeginnerWeaponNPCDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NBeginnerWeaponNPC/DLG_BeginnerWeaponNPC.ddf"));
	file.CreatePopupDialog( this, NBEGINNERWEAPONNPC_DLG, TEXT("dialog_BeginnerWeaponNPC"),StaticBeginnerWeaponNPCDlgProc );

	// 컨트롤 생성
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NBEGINNERWEAPONNPC_STATIC_NPCIMAGE, this) ,NBEGINNERWEAPONNPC_STATIC_NPCIMAGE, TEXT("statictext_npcimage"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NBEGINNERWEAPONNPC_EDIT_EXPLAIN, this) ,NBEGINNERWEAPONNPC_EDIT_EXPLAIN, TEXT("editbox_explain"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NBEGINNERWEAPONNPC_IMAGE_ITEM1, this) ,NBEGINNERWEAPONNPC_IMAGE_ITEM1, TEXT("imagestatic_item1"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NBEGINNERWEAPONNPC_IMAGE_ITEM2, this) ,NBEGINNERWEAPONNPC_IMAGE_ITEM2, TEXT("imagestatic_item2"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NBEGINNERWEAPONNPC_IMAGE_ITEM3, this) ,NBEGINNERWEAPONNPC_IMAGE_ITEM3, TEXT("imagestatic_item3"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NBEGINNERWEAPONNPC_IMAGE_ITEM4, this) ,NBEGINNERWEAPONNPC_IMAGE_ITEM4, TEXT("imagestatic_item4"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NBEGINNERWEAPONNPC_IMAGE_ITEM5, this) ,NBEGINNERWEAPONNPC_IMAGE_ITEM5, TEXT("imagestatic_item5"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NBEGINNERWEAPONNPC_IMAGE_ITEM6, this) ,NBEGINNERWEAPONNPC_IMAGE_ITEM6, TEXT("imagestatic_item6"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NBEGINNERWEAPONNPC_RADIO_ITEM1, this) ,NBEGINNERWEAPONNPC_RADIO_ITEM1, TEXT("radiobutton_itemname1"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NBEGINNERWEAPONNPC_RADIO_ITEM2, this) ,NBEGINNERWEAPONNPC_RADIO_ITEM2, TEXT("radiobutton_itemname2"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NBEGINNERWEAPONNPC_RADIO_ITEM3, this) ,NBEGINNERWEAPONNPC_RADIO_ITEM3, TEXT("radiobutton_itemname3"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NBEGINNERWEAPONNPC_RADIO_ITEM4, this) ,NBEGINNERWEAPONNPC_RADIO_ITEM4, TEXT("radiobutton_itemname4"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NBEGINNERWEAPONNPC_RADIO_ITEM5, this) ,NBEGINNERWEAPONNPC_RADIO_ITEM5, TEXT("radiobutton_itemname5"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NBEGINNERWEAPONNPC_RADIO_ITEM6, this) ,NBEGINNERWEAPONNPC_RADIO_ITEM6, TEXT("radiobutton_itemname6"));

	// 설명 보여주는 부분 임시 삭제
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN1, this) ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN1, TEXT("statictext_explain1"));
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN2, this) ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN2, TEXT("statictext_explain2"));
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN3, this) ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN3, TEXT("statictext_explain3"));
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN4, this) ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN4, TEXT("statictext_explain4"));
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN5, this) ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN5, TEXT("statictext_explain5"));
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN6, this) ,NBEGINNERWEAPONNPC_STATIC_EXPLAIN6, TEXT("statictext_explain6"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NBEGINNERWEAPONNPC_BUTTON_RECEIVE, this) ,NBEGINNERWEAPONNPC_BUTTON_RECEIVE, TEXT("button_receive"));


	SetBeginnerItemList();

}

void CALLBACK CNBeginnerWeaponNPCDlg::StaticBeginnerWeaponNPCDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNBeginnerWeaponNPCDlg *pThis = (CNBeginnerWeaponNPCDlg*) pControllerMgr;
	pThis->NBeginnerWeaponNPCDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNBeginnerWeaponNPCDlg::NBeginnerWeaponNPCDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NBEGINNERWEAPONNPC_BUTTON_RECEIVE:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
                RequestReceiveItem();				
			}
		}
		break;
	case NBEGINNERWEAPONNPC_RADIO_ITEM1:
	case NBEGINNERWEAPONNPC_RADIO_ITEM2:
	case NBEGINNERWEAPONNPC_RADIO_ITEM3:
	case NBEGINNERWEAPONNPC_RADIO_ITEM4:
	case NBEGINNERWEAPONNPC_RADIO_ITEM5:
	case NBEGINNERWEAPONNPC_RADIO_ITEM6:
		{
			if ( EVENT_RADIOBUTTON_CHANGED == nEvent  )
			{
				CRadioButton* pRadio = m_InterfaceFactory.GetRadioButton( nControlID );
				if ( pRadio->IsCheck() )
				{
                    m_siSelectedItemUnique = m_siItemUnique[nControlID - NBEGINNERWEAPONNPC_RADIO_ITEM1]; 
				}
			}
		}
		break;
	default:
		break;
	}
}

void CNBeginnerWeaponNPCDlg::SetBeginnerItemList()
{

	for ( SI16 i=0; i< BIGENNERITEM_NUM; i++  )
	{
		CImageStatic* pImage = m_InterfaceFactory.GetImageStatic(NBEGINNERWEAPONNPC_IMAGE_ITEM1 + i );
		CRadioButton* pRadio = m_InterfaceFactory.GetRadioButton( NBEGINNERWEAPONNPC_RADIO_ITEM1 + i );
        
		// 이미지 설정
		SI32 itemtype = 0;
		SI32 fontindex = -1;
		pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_siItemUnique[i], &itemtype, &fontindex );
		pImage->SetItemSprType( itemtype );
		pImage->SetFontIndex( fontindex );

		// 이름 설정
		pRadio->SetText( (TCHAR*)pclClient->pclItemManager->GetName( m_siItemUnique[i]) , DT_LEFT|DT_VCENTER);

		// TODO : 설명도 보여주면 좋겠지..
		//CStatic* pStatic = m_InterfaceFactory.GetStatic(NBEGINNERWEAPONNPC_STATIC_EXPLAIN1 + i );

	}
}

void CNBeginnerWeaponNPCDlg::RequestReceiveItem()
{
	if ( m_siSelectedItemUnique	)
	{
		cltGameMsgRequest_RecieveBeginnerItem clinfo( m_siSelectedItemUnique );
		cltMsg clMsg( GAMEMSG_REQUEST_RECEIVEBEGINNERITEM, sizeof(clinfo), (BYTE*)&clinfo );
		((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);
	}


	DeleteDialog();
}