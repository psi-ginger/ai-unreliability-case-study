#include <atlenc.h>

#include "..\Client.h"
#include "..\Music\Music.h"

#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-ItemMall.h"
#include "../CommonLogic/Msg/MsgType-Summon.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface\LoginNoticeDlg\LoginNoticeDlg.h"

#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "NInterface/NItemMallManager/NItemMallManager1.h"

#include "ninterface/nnewitemmalldlg/NBuyWindowDlg.h"
#include "ninterface/nnewitemmalldlg/NNewItemInfoDlg.h"
#include "ninterface/nnewitemmalldlg/NNewItemMall.h"
#include "ninterface/nnewitemmalldlg/NPresentInfoWindowDlg.h"
#include "ninterface/nnewitemmalldlg/NPresentWindowDlg.h"
#include "ninterface/nitemmallmanager/NItemMallManager.h"
#include "ninterface/NMySummonDlg/NMySummonDlg.h"

#include "../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

#include "../../Lib/WebHTML/WebManager.h"


//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"


void cltClient::DoMsg_GAMEMSG_RESPONSE_ITEMMALLEXISTENCE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ItemMallExistence* pclInfo = (cltGameMsgResponse_ItemMallExistence*)pclMsg->cData;

	if( IsWhereServiceArea( ConstServiceArea_China ) )
	{
		pclCM->CR[1]->pclCI->clItemMall.SetExistence(pclInfo->bExistence);

		/*if( true == pclInfo->bExistence )
		{
			if( m_pDialog[NITEMMALLMANAGER_DLG] == NULL )
			{
				CreateInterface(NITEMMALLMANAGER_DLG);
			}

			if( m_pDialog[NITEMMALLMANAGER_DLG])
			{
				((CNItemMallButtonManager*)m_pDialog[NITEMMALLMANAGER_DLG])->SetFlash( pclInfo->bExistence );
			}
		}*/
	}
	else
	{
		pclCM->CR[1]->pclCI->clItemMall.SetExistence(pclInfo->bExistence);

		if( m_pDialog[NITEMMALLMANAGER_DLG] == NULL )
		{
			CreateInterface(NITEMMALLMANAGER_DLG);
		}

		if( m_pDialog[NITEMMALLMANAGER_DLG])
		{
			((CNItemMallButtonManager*)m_pDialog[NITEMMALLMANAGER_DLG])->SetFlash( pclInfo->bExistence );
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_ITEMMALLITEMLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ItemMallItemList* pclInfo = (cltGameMsgResponse_ItemMallItemList*)pclMsg->cData;
	
	SI32 totalitemnum = pclInfo->siTotalItemNum ;
	if ( totalitemnum < 0 ) totalitemnum = 0 ;

	bool bexistence = DATA_TO_BOOL(totalitemnum);

#ifdef _SAFE_MEMORY
	pclCM->CR[1]->pclCI->clItemMall.Set(&pclInfo->clItem[0],&pclInfo->siItemID[0],totalitemnum,pclInfo->siListPage,bexistence);
#else
	pclCM->CR[1]->pclCI->clItemMall.Set(pclInfo->clItem,pclInfo->siItemID,totalitemnum,pclInfo->siListPage,bexistence);
#endif
	
		
	if(!m_pDialog[NITEMMALL_DLG])
	{
		CreateInterface(NITEMMALL_DLG);
	}

	CNItemMallManager1* ItemManager= (CNItemMallManager1*)m_pDialog[NITEMMALL_DLG] ;
	ItemManager->ShowItemList();



	return ;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_ITEMMALLGETITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ItemMallGetItem* pclInfo = (cltGameMsgResponse_ItemMallGetItem*)pclMsg->cData;

	SI32 pos = pclInfo->siInvPos ;

	if ( pos < PERSONITEM_INV0 || pos >= MAX_ITEM_PER_PERSON ) return ;

	cltItem *pclitem = &pclInfo->clItem ;
	if ( pclitem == NULL || pclitem->siUnique <= 0 || pclitem->siItemNum <= 0 ) return ;
	//if ( pclItemManager->CanPile(pclitem->siUnique) == false && pclitem->GetItemNum() != 1 )
	if ( pclitem->CanPile(pclItemManager) == false && pclitem->GetItemNum() != 1 )
		return ;

	pclCM->CR[1]->pclCI->clCharItem.clItem[pos].Set(pclitem);

	// 다시 리스트를 요청한다. 무조건 첫번째 페이지
	Sleep(1000);// 1초 잠근다. 혹시 모를 엉킴 방지
	cltGameMsgRequest_ItemMallItemList clinfo(1);
	cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
	SendMsgToServer((sPacketHeader*)&clMsg);

	return ;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_NOLIMITTICKET_WARP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_NoLimitTicket_Warp* pclinfo = (cltGameMsgResponse_NoLimitTicket_Warp*)pclMsg->cData;

	if( NOLIMIT_TICKET == pclinfo->siType )
	{
		pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp = pclinfo->bUsing ;
		pclCM->CR[1]->pclCI->clBI.siNoLimitTicketWarpDateVary = pclinfo->siDateVary ;
	}
	else if( MARKET_TICKET == pclinfo->siType )
	{
		pclCM->CR[1]->pclCI->clBI.bMarKetPremiumService = pclinfo->bUsing ;
		pclCM->CR[1]->pclCI->clBI.siMarKetPremiumServiceDate = pclinfo->siDateVary ;
	}
	else if( NewMarket_TICKET == pclinfo->siType )
	{
		pclCM->CR[1]->pclCI->clBI.bNewMarketPremiumService		= pclinfo->bUsing ;
		pclCM->CR[1]->pclCI->clBI.siNewMarketPremiumServiceDate	= pclinfo->siDateVary ;
	}
	else if( SPACEBOX_TICKET == pclinfo->siType )
	{
		pclCM->CR[1]->pclCI->clBI.bSpaceBoxPremiumService = pclinfo->bUsing ;
		pclCM->CR[1]->pclCI->clBI.siSpaceBoxPremiumServiceDate = pclinfo->siDateVary ;
	}
	else if( SUMMON_SPACE_TICKET == pclinfo->siType )
	{
		cltClient* pclclient = (cltClient*)pclClient;
		cltKindInfo*	pclKI			=	NULL	;

		pclCM->CR[1]->pclCI->clBI.bSummonExtraSlot = pclinfo->bUsing ;
		pclCM->CR[1]->pclCI->clBI.siSummonExtraSlotDate = pclinfo->siDateVary ;
		if(((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ])) !=	NULL)
		{
			((CNMySummonDlg*)(m_pDialog[ NMYSUMMON_DLG ]))->LodaMyhaveSummonList();
		}
	}
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_GET_SEL_PGLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Get_Sel_PGList* pclinfo = (cltGameMsgResponse_Get_Sel_PGList*)pclMsg->cData;

	if(pclinfo == NULL)
		return;

	if(pclinfo->siResult != 1)
		return;

	if( m_pDialog[ NNEWITEMMALL_DLG ] )
	{
#ifdef _SAFE_MEMORY
		((CNNewItemMallDlg*)m_pDialog[ NNEWITEMMALL_DLG ])->LoadItemMallData(pclinfo->siPage,
			pclinfo->siMaxPage,
			&pclinfo->clItemData[0]);
#else
		((CNNewItemMallDlg*)m_pDialog[ NNEWITEMMALL_DLG ])->LoadItemMallData(pclinfo->siPage,
			pclinfo->siMaxPage,
			pclinfo->clItemData);
#endif

	}
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_GET_ITEMMALL_ITEMINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Get_ItemMall_ItemInfo* pclinfo = (cltGameMsgResponse_Get_ItemMall_ItemInfo*)pclMsg->cData;

	if(pclinfo == NULL)
		return;

	if(pclinfo->siResult != 1)
		return;

#ifdef _SAFE_MEMORY
	if(m_pDialog[ NBUYWINDOW_DLG])
		((CNBuyWindowDlg*)m_pDialog[ NBUYWINDOW_DLG ])->SetBuyDateItemInfo(&pclinfo->clItemData[0]);
#else
	if(m_pDialog[ NBUYWINDOW_DLG])
		((CNBuyWindowDlg*)m_pDialog[ NBUYWINDOW_DLG ])->SetBuyDateItemInfo(pclinfo->clItemData);
#endif

#ifdef _SAFE_MEMORY
	if(m_pDialog[ NPRESENTWINDOW_DLG ])
		((CNPresentWindowDlg*)m_pDialog[ NPRESENTWINDOW_DLG ])->SetPresentDateItemInfo(&pclinfo->clItemData[0]);
#else
	if(m_pDialog[ NPRESENTWINDOW_DLG ])
		((CNPresentWindowDlg*)m_pDialog[ NPRESENTWINDOW_DLG ])->SetPresentDateItemInfo(pclinfo->clItemData);
#endif
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GET_MYCASH_INFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Get_MyCash_Info* pclinfo = (cltGameMsgResponse_Get_MyCash_Info*)pclMsg->cData;

	if(pclinfo == NULL)
		return;

	if(pclinfo->siResult != 1)
		return;

	if(pclinfo->siRealCash < 0 || pclinfo->siBonusCash < 0)
		return;


	if( m_pDialog[ NNEWITEMMALL_DLG ] )
		((CNNewItemMallDlg*)m_pDialog[ NNEWITEMMALL_DLG ])->SetMyCashInfo(pclinfo->siRealCash, pclinfo->siBonusCash);

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_ITEMMALL_BUYITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ItemMall_Buyitem* pclinfo = (cltGameMsgResponse_ItemMall_Buyitem*)pclMsg->cData;

	if(pclinfo == NULL)
		return;

	if( IsWhereServiceArea( ConstServiceArea_China ) )
	{
		if( -6 == pclinfo->siResult )
		{
			TCHAR* pTitle = GetTxtFromMgr(5951);
			TCHAR* pText  = GetTxtFromMgr(7549);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
			m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

			return;
		}
	}

	// [종호] 아이템 구매제한이 걸린 경우라면 메세지를 띄워준다.
	if(pclinfo->siResult == -98)	
	{
		m_pDialog[NOTIFYMSG_DLG]->SetText(GetTxtFromMgr(5614), GetTxtFromMgr(40315));
		m_pDialog[NOTIFYMSG_DLG]->SetTopMostWindow(true);
	}
	
	if(pclinfo->siResult != 1)
		return;

	if(m_pDialog[NBUYITEMINFOWINDOW_DLG] == NULL)
		CreateInterface(NBUYITEMINFOWINDOW_DLG);

	// 사고 난후의 정보
	if( m_pDialog[NBUYITEMINFOWINDOW_DLG] )
		((CNNewItemMallBuyInfoDlg*)m_pDialog[NBUYITEMINFOWINDOW_DLG])->SetBuyItemInfo(pclinfo->clItemData, pclinfo->siProductNum, pclinfo->siRealCash, pclinfo->siBonusCash);


	if( m_pDialog[ NNEWITEMMALL_DLG ] )
		((CNNewItemMallDlg*)m_pDialog[ NNEWITEMMALL_DLG ])->SetMyCashInfo(pclinfo->siRealCash, pclinfo->siBonusCash);

	pclCM->CR[1]->pclCI->clItemMall.Init();
	cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLEXISTENCE,0);
	SendMsgToServer((sPacketHeader*)&clMsg);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_ITEMMALL_GIFTITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ItemMall_Giftitem* pclinfo = (cltGameMsgResponse_ItemMall_Giftitem*)pclMsg->cData;

	if(pclinfo == NULL)
		return;

	if( IsWhereServiceArea( ConstServiceArea_China ) )
	{
		if( -6 == pclinfo->siResult )
		{
			TCHAR* pTitle = GetTxtFromMgr(5951);
			TCHAR* pText  = GetTxtFromMgr(7549);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
			m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

			return;
		}
	}

	// [종호] 아이템 구매제한이 걸린 경우라면 메세지를 띄워준다.
	if(pclinfo->siResult == -98)	
	{
		m_pDialog[NOTIFYMSG_DLG]->SetText(GetTxtFromMgr(5614), GetTxtFromMgr(40315));
		m_pDialog[NOTIFYMSG_DLG]->SetTopMostWindow(true);
	}

	// [종호] 나이제한이 걸린 경우라면 메세지를 띄워준다.
	if(pclinfo->siResult == -95)	
	{
		m_pDialog[NOTIFYMSG_DLG]->SetText(GetTxtFromMgr(5614), GetTxtFromMgr(40316));
		m_pDialog[NOTIFYMSG_DLG]->SetTopMostWindow(true);
	}

	if(pclinfo->siResult != 1)
	{	
		//[진성][USA] 선물하기 실패할 때 메시지 띠우기. => 2008-7-24  
		if(ConstServiceArea_USA == pclClient->siServiceArea)
		{
			TCHAR* pTitle = NULL;
			TCHAR* pText  = NULL;

			// 실패는 했지만 정확한 실패를 알릴 이유가 있다.
			if(20 == pclinfo->siHanFailReason)
			{
				pTitle = GetTxtFromMgr(6127);
				pText  = GetTxtFromMgr(8647);
			}
			else
			{
				pTitle = GetTxtFromMgr(6127);
				pText  = GetTxtFromMgr(6127);
			}
			
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
			m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
		}
		
		return;
	}



	if( m_pDialog[ NNEWITEMMALL_DLG ] )
		((CNNewItemMallDlg*)m_pDialog[ NNEWITEMMALL_DLG ])->SetMyCashInfo(pclinfo->siRealCash, pclinfo->siBonusCash);
	
	TCHAR* pTitle   = GetTxtFromMgr(5953);

	// PCK : 영어권에서 어순이 바뀌어야 해서 %s대신에 [#...#]으로 수정 (09.07.06)
	NTCHARString128	kText( GetTxtFromMgr(5954) );
	NTCHARString64	kItemName		= TEXT("");
	NTCHARString64	kCharName		= TEXT("");
	kItemName = pclinfo->szItemName;
	kCharName = pclinfo->szGetCharName;

	kText.Replace( TEXT("#itemname#"), kItemName);
	kText.Replace( TEXT("#charname#"), kCharName);

	m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, kText );
	m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_CHECK_ACCOUNTID(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Check_AccountID* pclinfo = (cltGameMsgResponse_Check_AccountID*)pclMsg->cData;

	TCHAR* pGetServerName = NULL;
	if( true == IsWhereServiceArea( ConstServiceArea_Japan ) )
	{
		pGetServerName = GetServerNameFromUnique(pclinfo->siGetServerUnique);

		if(pGetServerName == NULL) return;
	}

	if(pclinfo->siResult != 1)
	{
		// 실패
		TCHAR* pTitle =  GetTxtFromMgr(6092);
		TCHAR* pSource = GetTxtFromMgr(6093);		
		TCHAR szText[256] = {'0'};
		
		if( true == IsWhereServiceArea( ConstServiceArea_Japan ) )
		{
			sprintf(szText, pSource, pGetServerName ,pclinfo->szGetCharName );
		}
		else if( true == IsWhereServiceArea( ConstServiceArea_China ) )
		{
			sprintf(szText, pSource, pclinfo->szGetCharName );
		}
		else if (true == IsWhereServiceArea( ConstServiceArea_Korea)) 
		{
			sprintf(szText, pSource, pclinfo->szGetCharName );//국가 예외처리가 빠저서 국가 추가.[2007.07.11 성웅]
		}
		else if (true == IsWhereServiceArea( ConstServiceArea_USA)) 
		{
			sprintf(szText, pSource, pclinfo->szGetCharName );
		}

		m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, szText );
		m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

		return;
	}
	
	//성공
	TCHAR* pTitle = GetTxtFromMgr(6094);
	TCHAR* pSource = GetTxtFromMgr(6095);
	TCHAR szText[256] = {'\0'};

	if( true == IsWhereServiceArea( ConstServiceArea_Japan ) )
	{
		sprintf(szText, pSource, pGetServerName , pclinfo->szGetCharName );
	}
	else if( true == IsWhereServiceArea( ConstServiceArea_China ) )
	{
		sprintf(szText, pSource, pclinfo->szGetCharName );
	}
	else if (true == IsWhereServiceArea( ConstServiceArea_Korea)) 
	{
		sprintf(szText, pSource, pclinfo->szGetCharName );//국가 예외처리가 빠저서 국가 추가.[2007.07.11 성웅]
	}
	else if (true == IsWhereServiceArea( ConstServiceArea_USA)) 
	{
		sprintf(szText, pSource, pclinfo->szGetCharName );
	}
	else
	{
		sprintf(szText, "%s", pclinfo->szGetCharName );
	}	

	m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, szText );
	m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

	// 닉네임을 받아온다!!
	if(m_pDialog[ NPRESENTWINDOW_DLG ])
		((CNPresentWindowDlg*)m_pDialog[ NPRESENTWINDOW_DLG ])->AccountIdCheckResult( pclinfo->szAccountID, pclinfo->szAccountName, pclinfo->siGetServerUnique, pclinfo->szGetCharName );

}

void cltClient::DoMag_GAMEMSG_RESPONSE_BILLPAGE_URL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BillPage_URL* clinfo = (cltGameMsgResponse_BillPage_URL*)pclMsg->cData;
	cltClient* pclclient = (cltClient*)pclClient;

	//TCHAR* szBaseURL = TEXT("https://bill.kunshu.jp/login.ashx?");

	TCHAR BillPageURL[MAX_BILL_PAGGE_URL_LENGTH  ];
    
	
	/*StringCchPrintf(BillPageURL,MAX_BILL_PAGGE_URL_LENGTH  ,TEXT("%s%s"),
								szBaseURL ,clinfo->szBillPageURL  );
	*/
	//KHY - 0617 - 일본 채널링 추가.
	strcpy(BillPageURL, clinfo->szBillPageURL );

	g_LoginNoticeDlg.Set( BillPageURL, 818, 600 );
	
	 /*
	if( g_bFullScreen)
		ShowWindow( pclclient->GetHwnd(), SW_MINIMIZE );
	
	CNdoorsWebManager WebManager;
	CString strUrl = BillPageURL;
	CString strPost ;
	strPost.Empty();

	HWND hWnd;

   	WebManager.Navigate_Popup(strUrl, strPost,980,705 , &hWnd );


	SetFocus( hWnd);
	ShowWindow( hWnd ,SW_RESTORE );
	*/

}

void cltClient::DoMag_GAMEMSG_RESPONSE_LOGINPAGE_URL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_LoginPage_URL* clinfo = (cltGameMsgResponse_LoginPage_URL*)pclMsg->cData;
	cltClient* pclclient = (cltClient*)pclClient;

	TCHAR* szBaseURL = TEXT("https://www.kunshu.jp/login.aspx?p=");

	TCHAR LoginPageURL[MAX_BILL_PAGGE_URL_LENGTH  ];

	
	TCHAR szBASE64[MAX_PATH] = TEXT("");
    
	int nBytes = Base64EncodeGetRequiredLength(_tcslen(clinfo->szUrl), ATL_BASE64_FLAG_NONE);
	if (Base64Encode( (BYTE*)clinfo->szUrl,	_tcslen(clinfo->szUrl),	szBASE64, &nBytes, ATL_BASE64_FLAG_NONE ))
	{

		StringCchPrintf(LoginPageURL, MAX_BILL_PAGGE_URL_LENGTH, "%s%s&q=%s",
			szBaseURL, szBASE64, clinfo->szLoginPageURL );


		//======================================================
		// 게임내 창
		g_LoginNoticeDlg.Set( LoginPageURL, 800, 600 );
		//======================================================
		// 웹페이지

		//if( g_bFullScreen)
		//	ShowWindow( pclclient->GetHwnd(), SW_MINIMIZE );

		//CNdoorsWebManager WebManager;
		//CString strUrl = LoginPageURL;
		//CString strPost ;
		//strPost.Empty();
	
		//HWND hWnd;

		//WebManager.Navigate_Popup(strUrl, strPost,400,400 , &hWnd );

		//SetFocus( hWnd );
		//ShowWindow(hWnd ,SW_RESTORE );
		//======================================================
	}

}

void cltClient::DoMsa_GAMEMSG_RESPONSE_GACHAPAGE_URL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_LoginPage_URL* clinfo = (cltGameMsgResponse_LoginPage_URL*)pclMsg->cData;
	cltClient* pclclient = (cltClient*)pclClient;

	TCHAR* szBaseURL = TEXT("https://www.kunshu.jp/login.aspx?p=");

	TCHAR LoginPageURL[MAX_BILL_PAGGE_URL_LENGTH  ];
	
    TCHAR encodeURL[MAX_PATH] = TEXT("/playgame/slot/index.aspx");
	TCHAR szBASE64[MAX_PATH] = TEXT("");

	int nBytes = Base64EncodeGetRequiredLength(_tcslen(encodeURL), ATL_BASE64_FLAG_NONE);
	if (Base64Encode( (BYTE*)encodeURL,	_tcslen(encodeURL),	szBASE64, &nBytes, ATL_BASE64_FLAG_NONE ))
	{

		StringCchPrintf(LoginPageURL, MAX_BILL_PAGGE_URL_LENGTH, "%s%s&q=%s",
			szBaseURL, szBASE64, clinfo->szLoginPageURL );

		//======================================================
		// 게임내 창
		g_LoginNoticeDlg.Set( LoginPageURL, 800, 600 );
		
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SET_CHANGEHERO(cltMsg* pclMsg, SI32 id)
{/*
	cltGameMsgResponse_Set_ChangeHero* pclinfo = (cltGameMsgResponse_Set_ChangeHero*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clCharItem.clItem[pclinfo->siPos].Set(&pclinfo->clitem);
	pclCM->CR[1]->pclCI->clBI.siChangeHeroCode = pclinfo->siCode ;*/
}
