#include "NSystemRewardListDlg.h"

#include "Char\CharManager\CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"
#include "../../CommonLogic/Msg/MsgType-SystemReward.h"

extern cltCommonLogic* pclClient;

CNSystemRewardListDlg::CNSystemRewardListDlg()
{	
	m_siCurPage = 0;
	m_siMaxPage = 0;
	Initialize();
}

CNSystemRewardListDlg::~CNSystemRewardListDlg()
{
	Destroy();
}


void CNSystemRewardListDlg::Init()
{

}

void CNSystemRewardListDlg::Destroy()
{

}

void CNSystemRewardListDlg::Create()
{
	if ( IsCreate() )
		return;


	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NSystemRewardList/DLG_NSystemRewardList.ddf"));
	file.CreatePopupDialog( this, NSYSTEMREWARDLIST_DLG , TEXT("dialog_systemreward"), StaticSystemRewardListDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NSYSTEMREWARDLIST_IMAGE_PORTRAIT , this ), NSYSTEMREWARDLIST_IMAGE_PORTRAIT,   "imagestatic_portrait");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NSYSTEMREWARDLIST_STATIC_EXPLAIN , this ), NSYSTEMREWARDLIST_STATIC_EXPLAIN,   "statictext_explain");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST, NSYSTEMREWARDLIST_LIST_REWARD , this ), NSYSTEMREWARDLIST_LIST_REWARD,   "listbox_reward");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NSYSTEMREWARDLIST_BUTTON_REFRESH , this ), NSYSTEMREWARDLIST_BUTTON_REFRESH,   "button_refresh");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NSYSTEMREWARDLIST_BUTTON_RECIEVE , this ), NSYSTEMREWARDLIST_BUTTON_RECIEVE,   "button_recieve");

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NSYSTEMREWARDLIST_BUTTON_PREV, this ), NSYSTEMREWARDLIST_BUTTON_PREV,   "button_systemreward_prev");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NSYSTEMREWARDLIST_BUTTON_NEXT , this ), NSYSTEMREWARDLIST_BUTTON_NEXT,   "button_systemreward_next");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NSYSTEMREWARDLIST_STATIC_PAGE , this ), NSYSTEMREWARDLIST_STATIC_PAGE,   "statictext_systemreward_page");

#ifdef _CHINA // exs
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NSYSTEMREWARDLIST_BUTTON_EXCHANGE , this ), NSYSTEMREWARDLIST_BUTTON_EXCHANGE,   "button_exchange");
#endif 
	//KHY - 1115 - 보상NPC 초상 및 텍스트 추가.
	CImageStatic* pImage  = m_InterfaceFactory.GetImageStatic(NSYSTEMREWARDLIST_IMAGE_PORTRAIT);
	if( pImage )
	{
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash( TEXT("KIND_REWARD")), szPortrait, sizeof(szPortrait), &siPortraitFont);
		pImage->SetFileName(szPortrait);
		pImage->SetFontIndex(siPortraitFont);				
	}

	CStatic* pStatic = m_InterfaceFactory.GetStatic( NSYSTEMREWARDLIST_STATIC_EXPLAIN );
	if( pStatic )
	{
		TCHAR* pText = GetTxtFromMgr(7612);
		pStatic->SetText( pText );
	}

	
	CList* pList = m_InterfaceFactory.GetList(NSYSTEMREWARDLIST_LIST_REWARD );
	if ( pList )
	{
		SI16 siNumberWidth		= 30;
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_USA | ConstServiceArea_EUROPE) )
		{
			siNumberWidth		= 40;
		}

		SI16 siGiveDayWidth		= 80;
		SI16 siLimitDayWidht	= 90;
		SI16 siReasonWidth		= pList->GetWidth() - siNumberWidth - siGiveDayWidth - siLimitDayWidht;
		
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr( 1641 );
		pList->SetColumn(0, siNumberWidth,		pText);
		pText = GetTxtFromMgr( 3469 );
		pList->SetColumn(1, siReasonWidth,		pText);
		pText = GetTxtFromMgr( 10183 );
		pList->SetColumn(2, siGiveDayWidth,		pText);
		pText = GetTxtFromMgr( 10184 );
		pList->SetColumn(3, siLimitDayWidht,	pText);
		pList->Refresh();
	}

	UpdatePageInfo(m_siCurPage, m_siMaxPage);
	RequestRewardList();
}


// [종호_NHN->NDOORS] 팅팅 페이징 추가
void CNSystemRewardListDlg::UpdatePageInfo(SI32  siCurPage, SI32  siMaxPage)
{
	m_siCurPage = siCurPage;
	m_siMaxPage = siMaxPage;

	CStatic* pStaticPage =  m_InterfaceFactory.GetStatic(NSYSTEMREWARDLIST_STATIC_PAGE);

	if(pStaticPage)
	{
		NTCHARString128 strPage = GetTxtFromMgr(40183);

		strPage.Replace(TEXT("#curpage"),SI32ToString(m_siCurPage+1));
		strPage.Replace(TEXT("#maxpage"),SI32ToString(m_siMaxPage));

		pStaticPage->SetText(strPage, DT_CENTER);
	}
}


void CALLBACK CNSystemRewardListDlg::StaticSystemRewardListDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNSystemRewardListDlg *pThis = (CNSystemRewardListDlg*) pControllerMgr;
	pThis->NSystemRewardListDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNSystemRewardListDlg::NSystemRewardListDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NSYSTEMREWARDLIST_BUTTON_REFRESH:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				RequestRewardList();
			}
		}
		break;

	// [종호_NHN->NDOORS] 팅팅 페이지 버튼		
	case NSYSTEMREWARDLIST_BUTTON_PREV:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				m_siCurPage--;
				
				if(m_siCurPage < 0)
					m_siCurPage = 0;

				RequestRewardList();
			}
		}
		break;

	// [종호_NHN->NDOORS] 팅팅 페이지 버튼		
	case NSYSTEMREWARDLIST_BUTTON_NEXT:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				m_siCurPage++;
				
				if(m_siCurPage >= m_siMaxPage)
					m_siCurPage = m_siMaxPage-1;

				RequestRewardList();
			}
		}
		break;

	case NSYSTEMREWARDLIST_BUTTON_RECIEVE:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				cltItem clRewardItem;
				SI16 itempos;
				SI32 rtnvalue;
                
				if (clRewardItem.siUnique != 0 ||
					clRewardItem.siUnique != NULL){
					//인벤토리가 꽉 차면 안된다는 메세지와 함께 리턴 시킨다.
					if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_ALL,
						&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], NULL, &itempos, &clRewardItem, &rtnvalue)== false)
					{//인벤토리에 아이템 널을 자리가 없다고 표시
						TCHAR* pTitle = GetTxtFromMgr(422);
						TCHAR* pText = GetTxtFromMgr(6981);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}
				}
				RequestRecieve();
			}
		}
		break;
	case NSYSTEMREWARDLIST_LIST_REWARD:
		{
			if ( EVENT_LISTBOX_SELECTION == nEvent )
			{
				ShowDetailInfo();
			}
		}
		break;
	case NSYSTEMREWARDLIST_BUTTON_EXCHANGE:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				pclclient->CreateInterface(NEXCHANGESTOCK_DLG);
			}
		}
		break;
	}
}

void CNSystemRewardListDlg::SetRewardList( cltSystemRewardInfo* clinfo )
{
	CList* pList = m_InterfaceFactory.GetList( NSYSTEMREWARDLIST_LIST_REWARD );

	pList->Clear();

	stListBoxItemData tempItemData;

	for ( SI32 i=0; i<MAX_SYSTEMREWARD_LIST; ++i )
	{
		if ( clinfo[i].m_clItem.siUnique == 0 && clinfo[i].m_siMoney == 0 )
			continue;

		m_clSystemRewardInfo[i].Init();
		tempItemData.Init();

		TCHAR buf[64];

		tempItemData.Set( 0, _itot( (m_siCurPage* MAX_SYSTEMREWARD_LIST) + i+1, buf ,10) );
		tempItemData.Set( 1, clinfo[i].m_szReason, i );
		
		MakeDateText( &clinfo[i].m_sGiveTime, buf, true );
		tempItemData.Set( 2, buf );
		MakeDateText( &clinfo[i].m_sExpireTime, buf, true );
		tempItemData.Set( 3, buf );

		pList->AddItem( &tempItemData );

		m_clSystemRewardInfo[i].Set( &clinfo[i] );
	}

	pList->Refresh();

	m_InterfaceFactory.GetButton( NSYSTEMREWARDLIST_BUTTON_REFRESH )->Enable( true );
}

void CNSystemRewardListDlg::RequestRewardList()
{
	cltRequestRewardList clinfo(m_siCurPage);
	cltMsg clMsg( GAMEMSG_REQUEST_SYSTEMREWARDLIST, sizeof(clinfo), (BYTE*)&clinfo);
	((cltClient*)pclClient)->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );

	m_InterfaceFactory.GetButton( NSYSTEMREWARDLIST_BUTTON_REFRESH )->Enable( false );
}

void CNSystemRewardListDlg::RequestRecieve()
{
	CList* pList = m_InterfaceFactory.GetList( NSYSTEMREWARDLIST_LIST_REWARD );
	if ( pList == NULL ) return;

	SI32 selectedIndex = pList->GetSelectedIndex();
	if ( selectedIndex < 0 ) return;

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

	SI32 invpos = 0;
	cltItem cltoitem;
	SI32 rtnvalue = 0;

	cltSystemRewardInfo clinfo( &m_clSystemRewardInfo[selectedIndex] );
	cltMsg clMsg( GAMEMSG_REQUEST_RECIEVESYSTEMREWARD, sizeof(clinfo), (BYTE*)&clinfo );
	((cltClient*)pclClient)->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );

}

void CNSystemRewardListDlg::ShowDetailInfo()
{
	CList* pList = m_InterfaceFactory.GetList( NSYSTEMREWARDLIST_LIST_REWARD );
	if ( pList == NULL ) return;

	SI32 selectedIndex = pList->GetSelectedIndex();
	if ( selectedIndex < 0 ) return;

	TCHAR buf[512] = {0,};
	TCHAR reward[256] = {0,};
	TCHAR *pText = NULL;
	if ( m_clSystemRewardInfo[selectedIndex].m_clItem.siUnique )
	{
		TCHAR itemName[256] = {0,};
		TCHAR itemInfo[256] = {0,};

		m_clSystemRewardInfo[selectedIndex].m_clItem.GetFullName( pclClient->pclItemManager, itemName, 256 );
		pText = GetTxtFromMgr( 10171 );
		StringCchPrintf( itemInfo, 256, pText, itemName, m_clSystemRewardInfo[selectedIndex].m_clItem.siItemNum ); 
		StringCchCopy( reward, 256, itemInfo );
	}
	if ( m_clSystemRewardInfo[selectedIndex].m_siMoney )
	{
		TCHAR money[64] = {0,};
		pText = GetTxtFromMgr( 10172 );
		StringCchPrintf( money, 64, pText, m_clSystemRewardInfo[selectedIndex].m_siMoney );
		StringCchCat( reward, 256, money );
	}

	pText = GetTxtFromMgr(10170);
	TCHAR giveDay[64] = {0,};
	TCHAR expireDay[64] = {0,};
	MakeDateText( &m_clSystemRewardInfo[selectedIndex].m_sGiveTime, giveDay, true );
	MakeDateText( &m_clSystemRewardInfo[selectedIndex].m_sExpireTime, expireDay, true );
	StringCchPrintf( buf, 512, pText, m_clSystemRewardInfo[selectedIndex].m_szReason, reward, giveDay, expireDay );

	CStatic* pStatic = m_InterfaceFactory.GetStatic( NSYSTEMREWARDLIST_STATIC_EXPLAIN );
	if( pStatic )
	{
		pStatic->SetText( buf );
	}
}

void CNSystemRewardListDlg::ExchangeStock()
{
}
