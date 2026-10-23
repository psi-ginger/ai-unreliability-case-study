#include "NQuestSelectItemDlg.h"

#include "Char\CharManager\CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"



extern cltCommonLogic* pclClient;

CNQuestSelectItemDlg::CNQuestSelectItemDlg()
{	
	Initialize();
}

CNQuestSelectItemDlg::~CNQuestSelectItemDlg()
{
	Destroy();
}


void CNQuestSelectItemDlg::Init()
{
	m_pclInfo = NULL;
	m_siQuestType = 0;
	m_siQuestUnique = 0;
}

void CNQuestSelectItemDlg::Destroy()
{

}

void CNQuestSelectItemDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NQuestSelectItem/DLG_NQuestSelectItem.ddf"));
	file.CreatePopupDialog( this, NQUESTSELECTITEM_DLG , TEXT("dialog_questselectitem"), StaticQuestSelectItemDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST, NQUESTSELECTITEM_LIST_ITEM , this ), NQUESTSELECTITEM_LIST_ITEM,   "listbox_itemlist");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NQUESTSELECTITEM_BUTTON_RECEIVE , this ), NQUESTSELECTITEM_BUTTON_RECEIVE,   "button_receive");

	CList* pList = m_InterfaceFactory.GetList( NQUESTSELECTITEM_LIST_ITEM );

	if ( pList )
	{
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		pList->SetColumn(0, 180, TEXT("") );
		pList->SetTextPrintOption( DT_VCENTER | DT_SINGLELINE);
	}

	Show( true );
}

void CALLBACK CNQuestSelectItemDlg::StaticQuestSelectItemDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNQuestSelectItemDlg *pThis = (CNQuestSelectItemDlg*) pControllerMgr;
	pThis->NQuestSelectItemDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNQuestSelectItemDlg::NQuestSelectItemDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
		//	DeleteDialog();
		}
		break;
	case NQUESTSELECTITEM_BUTTON_RECEIVE:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				RecvItem();
			}
		}
	}
}

void CNQuestSelectItemDlg::SetItemList( cltQuestRewardInfo* pclinfo, SI16 questtype, SI16 questunique )
{
	if ( m_siQuestUnique == questunique )
		return;

	m_pclInfo = pclinfo;
	m_siQuestType = questtype;
	m_siQuestUnique = questunique;

	stListBoxItemData tempListData;

	CList* pList = m_InterfaceFactory.GetList( NQUESTSELECTITEM_LIST_ITEM );

	if ( pList == NULL )
		return;

	pList->Clear();

	for ( SI32 i=0; i<MAX_QUEST_REWARD_NUMBER; ++i )
	{
		tempListData.Init();

		SI32 itemUnique = pclinfo->siItemUnique[i];
		SI32 itemNum = pclinfo->siItemNum[i];

		if ( itemUnique <= 0 && itemNum <= 0 )
			continue;

		SI32 gimgIndex = 0;
		SI32 fontindex = 0;

		pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemUnique, &gimgIndex, &fontindex );

		if ( gimgIndex )
		{
			tempListData.siSmallIconGImg[ 0 ] = gimgIndex;
			tempListData.siSmallIconFontIndex[ 0 ] = fontindex; // 로그온 이미지 폰트 번호
		}

		TCHAR buf[256] = {0,};
		TCHAR* pText = GetTxtFromMgr( 676 );

		StringCchPrintf( buf, 256, pText, pclClient->pclItemManager->GetName(itemUnique), itemNum );
		tempListData.Set( 0, buf, i );

		pList->AddItem( &tempListData );
	}

	pList->AdjustHeight( 4 );
	pList->Refresh();

	CButton* pButton = m_InterfaceFactory.GetButton( NQUESTSELECTITEM_BUTTON_RECEIVE );
	if ( pButton )
	{
		pButton->MoveController( pButton->GetX(), pList->GetHeight() + 30 );
	}

	cltClient* pclclient = (cltClient*)pclClient;

	// 화면 중심에 출력
	SI32 xPos = ( pclclient->GetScreenXSize() - GetWidth() ) / 2;
	SI32 yPos = ( pclclient->GetScreenYSize() - GetHeight() ) / 2;

	SetDialogPos( xPos, yPos, GetWidth(), pList->GetHeight() + 60 );
}
void CNQuestSelectItemDlg::RecvItem()
{
	CList* pList = m_InterfaceFactory.GetList( NQUESTSELECTITEM_LIST_ITEM );

	if ( pList == NULL )
		return;

	SI32 selectedindex = pList->GetSelectedIndex();
	if ( selectedindex < 0 ) return;

	SI32 index = pList->GetParam( selectedindex, 0 );

	
	cltGameMsgRequest_QuestGoing clInfo(m_siQuestType, m_siQuestUnique, QUEST_STEP_CLEAR, index );
	cltMsg clMsg(GAMEMSG_REQUEST_SETQUESTGOING, sizeof(clInfo), (BYTE*)&clInfo);
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	Show( false );
}