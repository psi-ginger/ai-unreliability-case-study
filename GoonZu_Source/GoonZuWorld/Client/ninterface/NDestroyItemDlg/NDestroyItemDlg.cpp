#include "../Client/ninterface/NDestroyItemDlg/NDestroyItemDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../CommonLogic/MsgType-Item.h"
#include "..\..\Common\Item\ItemCommon\cltitem.h"
#include "..\Client\Client.h"
#include "../../Client/NInterface/NNewInventory/NNewInventory.h"
#include "../../Server/GachaManager/GachaManager.h"
#include "../../Server/Gacha2Manager/Gacha2Manager.h"
#include "../CommonLogic/MsgType-Item.h"

NDestroyItemDlg::NDestroyItemDlg() 
{
}

NDestroyItemDlg::~NDestroyItemDlg()
{
}
void NDestroyItemDlg::Create( void )
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NDestroyItemDlg/DLG_NDestroyItemDlg.ddf" ) );
	file.CreatePopupDialog( this, NDESTROYITEM_DLG, TEXT( "dialog_DestroyItem" ), StaticNDestroyItemDlgProc, true);
	// 설명.
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NDESTROY_STATIC_NOTICE,	this ), NDESTROY_STATIC_NOTICE,	TEXT( "statictext_Notice" ) );
	// 파괴될 아이템 아이템 이름
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,  NDESTROY_EDIT_ITEMNAME,	this ), NDESTROY_EDIT_ITEMNAME,	TEXT( "editbox_ItemName" ) );
	// 버튼 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  NDESTROY_BUTTON_OK,	this ), NDESTROY_BUTTON_OK,	TEXT( "button_Ok" ) );// 파괴한다
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  NDESTROY_BUTTON_CANCEL,	this ), NDESTROY_BUTTON_CANCEL,	TEXT( "button_Cancel" ) );//파괴하지않는다.

	if ( pclclient )
	{
		SI16 siPosX = ( pclclient->GetScreenXSize() - GetWidth() + MAP_TILE_XSIZE ) / 2;
		SI16 siPosY = ( pclclient->GetScreenYSize() - GetHeight() + MAP_TILE_YSIZE ) / 2;

		SetDialogPos( siPosX, siPosY, GetWidth(), GetHeight() );
	}

    SetActionTimer( 100 );    
	SetTimerOnOff( true );
}


void CALLBACK NDestroyItemDlg::StaticNDestroyItemDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NDestroyItemDlg *pThis = (NDestroyItemDlg*) pControllerMgr;
	pThis->NNDestroyItemDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK NDestroyItemDlg::NNDestroyItemDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	// 아이템 이름 쓰고 OK를 눌렀다.
	case NDESTROY_BUTTON_OK:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					CEdit* peItemName = NULL ;
					peItemName = m_InterfaceFactory.GetEdit( NDESTROY_EDIT_ITEMNAME );
					// 이름 넣는 창이 보여지면
					if (peItemName->IsShow() == true) {
						pItemName = peItemName->GetText();// 아이템 이름을 받아 온다.

						if( _tcsicmp(ItemNameToDestroy , pItemName) == 0) 
						{
							// 적은 이름과 같다면
							// 파괴하라는 메세지를 보낸다 함수는 - cltGameMsgRequest_InvDrop 파괴메세지 - GAMEMSG_REQUEST_BELONGITEM_DESTROY
							// 귀속아이템은 파괴하도록 요청한다.
							cltGameMsgRequest_InvDrop cldrop(DropItemIndex, &dropitemtoDestroy);
							cltMsg clMsg(GAMEMSG_REQUEST_BELONGITEM_DESTROY, sizeof(cldrop), (BYTE*)&cldrop);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
							
							// 게임서버로 드랍하도록 요청한다. 
							/*
							cltGameMsgRequest_InvDrop cldrop(DropItemIndex, &dropitemtoDestroy);
							cltMsg clMsg(GAMEMSG_REQUEST_INVDROP, sizeof(cldrop), (BYTE*)&cldrop);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
							*/
							
							DeleteDialog();
						}
						else{ 
							return;//아이템 같지 않으면 리턴( 나중에 메세지 추가.)							
						}
					}
					else{
						// 이제는 드롭 해도 삭제 하지 말고 드롭 메세지만 띄우고 드롭 시킨다.
						cltGameMsgRequest_InvDrop cldrop(DropItemIndex, &dropitemtoDestroy);
						cltMsg clMsg(GAMEMSG_REQUEST_INVDROP, sizeof(cldrop), (BYTE*)&cldrop);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						/*
						cltGameMsgRequest_InvDrop cldrop(DropItemIndex, &dropitemtoDestroy);
						cltMsg clMsg(GAMEMSG_REQUEST_BELONGITEM_DESTROY, sizeof(cldrop), (BYTE*)&cldrop);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						*/
						DeleteDialog();
					}	
					
				}				
			}
		}
		break;
	case NDESTROY_BUTTON_CANCEL:{
		switch( nEvent ){
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
			}
		}
		break;
	}
}

void NDestroyItemDlg::Action(void)
{
}
void NDestroyItemDlg:: SetDropItem( SI32 ItemIndex , cltItem DropItem )
{
	cltClient *pclclient = (cltClient*)pclClient;
	dropitemtoDestroy.Set( &DropItem );
	ItemNameToDestroy = (TCHAR*)pclClient->pclItemManager->GetName( dropitemtoDestroy.siUnique ); // 떨어뜨린 아이템의 이름을 찾아서 저장한다.
	if(ItemNameToDestroy == NULL )	
	{
		DeleteDialog();
		return	;
	}
	if(dropitemtoDestroy.siUnique <= 0 )
	{
		DeleteDialog()	;
		return	;
	}
	DropItemIndex = ItemIndex; //떨어뜨린 아이템의 인덱스를 저장한다.
	if(DropItemIndex < 0 )
	{
		DeleteDialog()	;
		return	;
	}
	// 파괴되는 아이템을 알아본다.
	bool bCondition1 = pclClient->pclItemManager->IsCanDropItem(dropitemtoDestroy.siUnique);// 떨어뜨려야 하는 아이템인지 아닌지 결정 
	// 귀속장비 인지 알아본다.
	bool bCondition2 = dropitemtoDestroy.IsBelonging( pclClient->pclItemManager );//귀속장비인지

	// 귀속장비는 모조리 파괴 한다.
	if ( true == bCondition2 ) 
	{
		SI16 siItemnum = 0;
		CStatic* peNotice = NULL ;

		NTCHARString256	kText(GetTxtFromMgr(8573));
		kText.Replace(TEXT("#itemname#"),pclClient->pclItemManager->GetName( dropitemtoDestroy.siUnique ) );
		kText.Replace(TEXT("#num#"),SI16ToString(dropitemtoDestroy.siItemNum) );
		peNotice = m_InterfaceFactory.GetStatic(NDESTROY_STATIC_NOTICE );
		peNotice->SetText( kText );
	}
	// 그냥 드롭되야 하는 아이템
	else if ( (true == bCondition1) ) 
	{
		CEdit* peItemName = NULL ;
		CStatic* peNotice = NULL ;
		peItemName = m_InterfaceFactory.GetEdit( NDESTROY_EDIT_ITEMNAME );
		peItemName->Show(false);

		NTCHARString256	kText(GetTxtFromMgr(8571));
		kText.Replace(TEXT("#itemname#"),pclClient->pclItemManager->GetName( dropitemtoDestroy.siUnique ) );
		kText.Replace(TEXT("#num#"),SI16ToString(dropitemtoDestroy.siItemNum) );
		peNotice = m_InterfaceFactory.GetStatic(NDESTROY_STATIC_NOTICE );
		peNotice->SetText( kText );
	}
	// 거래불가 아이템은 파괴
	else
	{
		SI16 siItemnum = 0;
		CStatic* peNotice = NULL ;

		NTCHARString256	kText(GetTxtFromMgr(8573));
		kText.Replace(TEXT("#itemname#"),pclClient->pclItemManager->GetName( dropitemtoDestroy.siUnique ) );
		kText.Replace(TEXT("#num#"),SI16ToString(dropitemtoDestroy.siItemNum) );
		peNotice = m_InterfaceFactory.GetStatic(NDESTROY_STATIC_NOTICE );
		peNotice->SetText( kText );

		/*SI16 siItemnum = 0;
		CStatic* peNotice = NULL ;
		NTCHARString256	kText(GetTxtFromMgr(8571));
		kText.Replace(TEXT("#itemname#"),pclClient->pclItemManager->GetName( dropitemtoDestroy.siUnique ) );
		kText.Replace(TEXT("#num#"),SI16ToString(dropitemtoDestroy.siItemNum) );
		peNotice = m_InterfaceFactory.GetStatic(NDESTROY_STATIC_NOTICE );
		peNotice->SetText( kText );*/
		
		/*
		TCHAR* pTitle = GetTxtFromMgr(3266);
		TCHAR* pText = GetTxtFromMgr(3267);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		DeleteDialog();
		return;
		*/
	}
}