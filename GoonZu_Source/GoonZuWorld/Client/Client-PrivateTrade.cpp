#include "Client.h"
#include "..\Common\PrivateTradeOrderMgr\PrivateTradeOrderObj.h"
//#include "Resource.h"

#include "NInterface\NPrivateTradeDlg\NPrivateTradeDlg.h"
#include "NInterface\NPrivateTradeDlg\NPrivateTradeAcceptDlg.h"
#include "NInterface/NPrivateRepair/NPrivateRepairDlg.h"
#include "..\CommonLogic\Msg\MsgType-PrivateTrade.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "InterfaceMgr/NMsgBoxDlg.h"
#include "NInterface/NNewMakeItem/NNewMakeItemDlg.h"
#include "NInterface/NNewEnchantOther/NNewEnchantOther.h"

#include "NInterface/NCharStatus/NCharStatus.h"
#include "NInterface/NCharStatus/NCharHorse.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ACCEPT(cltMsg* pclMsg, SI32 id)//[영진]
{   
	
	// 상대방이 거래를 요구해오는데 응하겠냐는 것과 
	// 상대방이 거래를 거부해서 거래를 못한다 라는
	//두가지 경우의 메시지를 핸들링 한다.

	cltGameMsgResponse_PrivateTrade_Accept *pclPrivateTrade_Accept = (cltGameMsgResponse_PrivateTrade_Accept *) pclMsg->cData;
	
	//거래 거부 메시지가 온경우 끝냅
	TCHAR buffer[255];
	TCHAR* pText = NULL;

	// PRIVATETRADE_TRADE_MODE_REJECT_ACCEPT 등과 같은 메시지 플래그는 하나의 메시지로, 다음과 같은 과정으로 도착. 나->서버->상대유저->서버->나 

	if( pclPrivateTrade_Accept->siTradeMode == PRIVATETRADE_TRADE_MODE_REJECT_ACCEPT || pclPrivateTrade_Accept->siTradeMode == PRIVATETRADE_TRADE_MODE_ALREADYTRADE 
		|| pclPrivateTrade_Accept->siTradeMode == PRIVATE_ENCHANT_MODE_REJECT_ACCEPT || pclPrivateTrade_Accept->siTradeMode == PRIVATE_ENCHANT_MODE_ALREADYTRADE )
	{
		
		if(pclPrivateTrade_Accept->siTradeMode == PRIVATETRADE_TRADE_MODE_REJECT_ACCEPT)
		{
			pText = GetTxtFromMgr(5755);
			
		}
		else if(pclPrivateTrade_Accept->siTradeMode == PRIVATETRADE_TRADE_MODE_ALREADYTRADE)
		{
			pText = GetTxtFromMgr(5756);
			
		}
		else if(pclPrivateTrade_Accept->siTradeMode == PRIVATE_ENCHANT_MODE_REJECT_ACCEPT)
		{
			pText = GetTxtFromMgr(6040);

		}
		else if(pclPrivateTrade_Accept->siTradeMode == PRIVATE_ENCHANT_MODE_ALREADYTRADE)
		{
			pText = GetTxtFromMgr(6041);

		}
		StringCchPrintf(buffer, 255, pText,pclPrivateTrade_Accept->strCharName);	
		pclMessage->SetMsg(buffer);
		return;
	}
	// 상대방이 인첸트의뢰를 승낙해 주었다
	else if( pclPrivateTrade_Accept->siTradeMode == PRIVATE_ENCHANT_MODE_AGREE_ACCEPT )
	{	 
		// 이제 승낙 받았으니까 조합 의뢰를 시작한다.
 		cltGameMsgRequest_PrivateTrade clPrivateTrade( pclPrivateTrade_Accept->siOtherCharUnique, PRIVATETRADE_ENCHANT_MODE );
		cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE,
			sizeof(clPrivateTrade), (BYTE*)&clPrivateTrade);
		SendMsgToServer((sPacketHeader*)&clMsg);

	}

	// 트레이드 요청이 왔을때
	else if(pclPrivateTrade_Accept->siTradeMode == PRIVATETRADE_TRADE_MODE )
	{
		// 이미  인첸트창, 인첸트 의뢰창 ,거래요구창 혹은 거래창이 존재시 거래중으로 반환
		if(m_pDialog[NPRIVATETRADEACCEPT_DLG] || m_pDialog[NPRIVATETRADE_DLG] )
		{
			cltGameMsgRequest_PrivateTrade_Accept clinfo(pclPrivateTrade_Accept ->siOtherCharUnique,PRIVATETRADE_TRADE_MODE_ALREADYTRADE );

			cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT, sizeof(clinfo), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);	// 거래중 메시지 전달
			return;
		}
		else if( m_pDialog[NNEWENCHANTOTHERACCEPT_DLG] ||  m_pDialog[NNEWENCHANTOTHER_DLG]) 
		{	
			cltGameMsgRequest_PrivateTrade_Accept clinfo(pclPrivateTrade_Accept ->siOtherCharUnique,PRIVATE_ENCHANT_MODE_ALREADYTRADE );

			cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT, sizeof(clinfo), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);	// 인첸트 의뢰중 메시지 전달
			return;				
		}
		// 거래 에 응하겠냐는 메시지 박스 출력
		else
		{
			CreateInterface(NPRIVATETRADEACCEPT_DLG);
			CreateInterface( NPRIVATETRADEACCEPT_MSGBOX_DLG);
			TCHAR *pTitle = GetTxtFromMgr(2329);

			pText = GetTxtFromMgr(5754);


			TCHAR pBuffer[255];
			StringCchPrintf(pBuffer, 255, pText,pclPrivateTrade_Accept->strCharName);
			stMsgBox MsgBox;

			MsgBox.Set( m_pDialog[NPRIVATETRADEACCEPT_DLG], pTitle, pBuffer, MSGBOX_TYPE_YESNO, 0 );

			((CNMsgBoxDlg*)m_pDialog[NPRIVATETRADEACCEPT_MSGBOX_DLG])->SetMsgData((BYTE*)&MsgBox,
				(BYTE*)pclPrivateTrade_Accept,sizeof(cltGameMsgResponse_PrivateTrade_Accept));
		}
	}	
	// 인첸트 해달라는 요청이 왔을때
	else if( pclPrivateTrade_Accept->siTradeMode == PRIVATETRADE_ENCHANT_MODE  )
	{

		if(m_pDialog[NPRIVATETRADEACCEPT_DLG] || m_pDialog[NPRIVATETRADE_DLG] )
		{
			cltGameMsgRequest_PrivateTrade_Accept clinfo(pclPrivateTrade_Accept ->siOtherCharUnique,PRIVATETRADE_TRADE_MODE_ALREADYTRADE );

			cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT, sizeof(clinfo), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);	// 거래중 메시지 전달
			return;
		}
		// 이미 인첸트 중이라면 인첸트 중 메시지 도로 보냄
		else if (  m_pDialog[NNEWENCHANTOTHERACCEPT_DLG] ||  m_pDialog[NNEWENCHANTOTHER_DLG]  ) 
		{	
			cltGameMsgRequest_PrivateTrade_Accept clinfo(pclPrivateTrade_Accept ->siOtherCharUnique,PRIVATE_ENCHANT_MODE_ALREADYTRADE );

			cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT, sizeof(clinfo), (BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);	// 인첸트 중 메시지 전달
			return;				
		}
		// 인첸트 의뢰가 왔는데 응하겠냐는 메시지 표시
		else
		{
			CreateInterface( NNEWENCHANTOTHERACCEPT_DLG );
			CreateInterface( NPRIVATETRADEACCEPT_MSGBOX_DLG);
			TCHAR *pTitle = GetTxtFromMgr(6039);

			pText = GetTxtFromMgr(6038);

			TCHAR pBuffer[255];
			StringCchPrintf(pBuffer, 255, pText,pclPrivateTrade_Accept->strCharName);
			stMsgBox MsgBox;

			MsgBox.Set( m_pDialog[NNEWENCHANTOTHERACCEPT_DLG], pTitle, pBuffer, MSGBOX_TYPE_YESNO, 0 );

			((CNMsgBoxDlg*)m_pDialog[NPRIVATETRADEACCEPT_MSGBOX_DLG])->SetMsgData((BYTE*)&MsgBox,
				(BYTE*)pclPrivateTrade_Accept,sizeof(cltGameMsgResponse_PrivateTrade_Accept));
		}
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade *pclPrivateTrade = (cltGameMsgResponse_PrivateTrade *) pclMsg->cData;


	if(pclPrivateTrade->siTradeMode == PRIVATETRADE_TRADE_MODE)
	{
		if(( m_pDialog[ NPRIVATETRADE_DLG ])==NULL)
		{
			CreateInterface(NPRIVATETRADE_DLG);
		}
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->Show(pclPrivateTrade->siOtherCharUnique, pclPrivateTrade->siTradeMode, pclPrivateTrade->siOtherCharLevel, pclPrivateTrade->strCharName, pclPrivateTrade->strAccountID, pclPrivateTrade->siPrivateTradeOrderMgrIndex, pclPrivateTrade->siPrivateTradeOrderCountIndex, pclPrivateTrade->siOtherCharLevel);
	}

	else if(pclPrivateTrade->siTradeMode == PRIVATETRADE_ENCHANT_MODE)
	{
		if(( m_pDialog[ NNEWENCHANTOTHER_DLG ])==NULL)
		{
			CreateInterface(NNEWENCHANTOTHER_DLG);
		}
		((CNNewEnchantOther*)m_pDialog[NNEWENCHANTOTHER_DLG])->Show(pclPrivateTrade->siOtherCharUnique, pclPrivateTrade->siTradeMode,  pclPrivateTrade->siOtherCharLevel, pclPrivateTrade->strCharName, pclPrivateTrade->strAccountID, pclPrivateTrade->siPrivateTradeOrderMgrIndex, pclPrivateTrade->siPrivateTradeOrderCountIndex, pclPrivateTrade->siOtherCharLevel);


		if(m_pDialog[NNEWITEMMAKE_DLG])
		{
			CNewMakeItemDlg* makeItem = (CNewMakeItemDlg*)m_pDialog[NNEWITEMMAKE_DLG];
			makeItem->NewEnchantTab->SetEnchantCharUnique(pclPrivateTrade->siOtherCharUnique);
		//	makeItem->NewEnchantTab->SetEnchantOtherState(TRUE);

		}            
	}


	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_CANCEL(cltMsg* pclMsg, SI32 id)
{

	if( m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->DeleteDialog();

		TCHAR* pText = GetTxtFromMgr(5758);

		pclMessage->SetMsg(pText);

	}
		

	if( m_pDialog[ NNEWENCHANTOTHER_DLG ])
	{
		((CNNewEnchantOther*)m_pDialog[NNEWENCHANTOTHER_DLG])->DeleteDialog();
		
		if( m_pDialog[ NNEWITEMMAKE_DLG ] )
		{
			 ( (CNewMakeItemDlg* )m_pDialog[ NNEWITEMMAKE_DLG ] )->NewEnchantTab->SetInitInchant();
			//  ( (CNewMakeItemDlg* )m_pDialog[ NNEWITEMMAKE_DLG ] )->NewEnchantTab->SetEnchantOtherState(TRUE);
		}
//		TCHAR* pText = GetTxtFromMgr(6031);
	//	pclMessage->SetMsg(pText);
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_AddItem *pclPrivateTradeAddItem = ( cltGameMsgResponse_PrivateTrade_AddItem * ) pclMsg->cData;

	if(m_pDialog[NPRIVATETRADE_DLG])
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeAddItem( pclPrivateTradeAddItem->siCharUnique, pclPrivateTradeAddItem->siAddedIndex, &pclPrivateTradeAddItem->clItem );

	if( m_pDialog[ NPTRADEREPRAIR_DLG ])
		((CNPrivateRepairDlg*)m_pDialog[NPTRADEREPRAIR_DLG])->PrivateTradeAddItem( pclPrivateTradeAddItem->siCharUnique, pclPrivateTradeAddItem->siAddedIndex, &pclPrivateTradeAddItem->clItem );
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_DELLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_DelList *pclPrivateTradeDelList = ( cltGameMsgResponse_PrivateTrade_DelList * ) pclMsg->cData;
	
	if(m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeDelList( pclPrivateTradeDelList->siCharUnique, pclPrivateTradeDelList->siDelType, pclPrivateTradeDelList->siParam );
	}
	if( m_pDialog[ NPTRADEREPRAIR_DLG ])
	{
		((CNPrivateRepairDlg*)m_pDialog[NPTRADEREPRAIR_DLG])->PrivateTradeDelList( pclPrivateTradeDelList->siCharUnique, pclPrivateTradeDelList->siDelType, pclPrivateTradeDelList->siParam );

	}
	

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_DELITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_DelItem *pclPrivateTradeDelItem = ( cltGameMsgResponse_PrivateTrade_DelItem * ) pclMsg->cData;

	if(m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeDelItem( pclPrivateTradeDelItem->siCharUnique, pclPrivateTradeDelItem->siDelIndex );
	}


	if( m_pDialog[ NPTRADEREPRAIR_DLG ])
	{
		((CNPrivateRepairDlg*)m_pDialog[NPTRADEREPRAIR_DLG])->PrivateTradeDelItem( pclPrivateTradeDelItem->siCharUnique, pclPrivateTradeDelItem->siDelIndex );
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDPRICE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_AddPrice *pclPrivateTradeAddPrice = ( cltGameMsgResponse_PrivateTrade_AddPrice * ) pclMsg->cData;

	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		if(m_pDialog[ NPRIVATETRADE_DLG ])
		{
			((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeAddPrice( pclPrivateTradeAddPrice->siOtherCharUnique, pclPrivateTradeAddPrice->siPrice );
		}
	}

	if( m_pDialog[ NPTRADEREPRAIR_DLG ])
	{
		((CNPrivateRepairDlg*)m_pDialog[NPTRADEREPRAIR_DLG])->PrivateTradeAddPrice( pclPrivateTradeAddPrice->siOtherCharUnique, pclPrivateTradeAddPrice->siPrice );
		//CreateInterface( NPRIVATETRADE_DLG);
	}


	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDSTOCK(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_AddStock *pclPrivateTradeAddStock = ( cltGameMsgResponse_PrivateTrade_AddStock * ) pclMsg->cData;

	if(m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeAddStock( pclPrivateTradeAddStock->siOtherCharUnique, pclPrivateTradeAddStock->siVillageUnique, pclPrivateTradeAddStock->siStockNum );
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDHOUSEUNIT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_AddHouseUnit *pclPrivateTradeAddHouseUnit = ( cltGameMsgResponse_PrivateTrade_AddHouseUnit * ) pclMsg->cData;

	if(m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeAddHouseUnit( pclPrivateTradeAddHouseUnit->siOtherCharUnique,
			pclPrivateTradeAddHouseUnit->siHouseVillageUnique,
			pclPrivateTradeAddHouseUnit->siHouseUnitSlot,
			&pclPrivateTradeAddHouseUnit->clRentContract
			);
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDHORSE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_AddHorse *pclPrivateTradeAddHorse = ( cltGameMsgResponse_PrivateTrade_AddHorse* ) pclMsg->cData;

	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		if(m_pDialog[ NPRIVATETRADE_DLG ])
		{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeAddHorse( pclPrivateTradeAddHorse->siOtherCharUnique, pclPrivateTradeAddHorse->siHorseIndex, &pclPrivateTradeAddHorse->clHorseInfo );
		}
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_MYACCEPT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_MyAccept *pclPrivateTrade_MyAccept = ( cltGameMsgResponse_PrivateTrade_MyAccept * ) pclMsg->cData;

	if(m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeMyAccept();
	}

	if( m_pDialog[ NPTRADEREPRAIR_DLG ])
	{
		((CNPrivateRepairDlg*)m_pDialog[NPTRADEREPRAIR_DLG])->PrivateTradeMyAccept();

	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_MyCancel *pclPrivateTrade_MyCancel = ( cltGameMsgResponse_PrivateTrade_MyCancel * ) pclMsg->cData;

	if(m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeMyCancel();
	}

	if( m_pDialog[ NPTRADEREPRAIR_DLG ])
	{
		((CNPrivateRepairDlg*)m_pDialog[NPTRADEREPRAIR_DLG])->PrivateTradeMyCancel();
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_OTHERACCEPT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_OtherAccept *pclPrivateTrade_OtherAccept = ( cltGameMsgResponse_PrivateTrade_OtherAccept * ) pclMsg->cData;

	if(m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeOtherAccept( pclPrivateTrade_OtherAccept->siResult );
	}


	if( m_pDialog[ NPTRADEREPRAIR_DLG ])
	{
		((CNPrivateRepairDlg*)m_pDialog[NPTRADEREPRAIR_DLG])->PrivateTradeOtherAccept( pclPrivateTrade_OtherAccept->siResult );
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_OtherCancel *pclPrivateTrade_OtherCancel = ( cltGameMsgResponse_PrivateTrade_OtherCancel * ) pclMsg->cData;

	if(m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeOtherCancel();
	}


	if( m_pDialog[ NPTRADEREPRAIR_DLG ])
	{
		((CNPrivateRepairDlg*)m_pDialog[NPTRADEREPRAIR_DLG])->PrivateTradeOtherCancel();

	}

	

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_END(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PrivateTrade_End *pclPrivateTrade_End = ( cltGameMsgResponse_PrivateTrade_End * ) pclMsg->cData;
	
	if(m_pDialog[ NPRIVATETRADE_DLG ])
	{
		((CNPrivateTradeDlg*)m_pDialog[NPRIVATETRADE_DLG])->PrivateTradeEnd( pclPrivateTrade_End->siResult,
			pclPrivateTrade_End->siRecvStockVillageUnique, pclPrivateTrade_End->siRecvStockNum,
			pclPrivateTrade_End->siSendStockVillageUnique, pclPrivateTrade_End->siSendStockNum,
			pclPrivateTrade_End->siRecvHouseVillageUnique, pclPrivateTrade_End->siRecvHouseUnitSlot,
			&pclPrivateTrade_End->clRecvRentContract,
			pclPrivateTrade_End->siSendHouseVillageUnique, pclPrivateTrade_End->siSendHouseUnitSlot,
			pclPrivateTrade_End->siHorseIndex, &pclPrivateTrade_End->clHorse 
			);
	}

	if( m_pDialog[ NNEWENCHANTOTHER_DLG ])
	{
		((CNNewEnchantOther*)m_pDialog[NNEWENCHANTOTHER_DLG])->PrivateTradeEnd( pclPrivateTrade_End->siResult,
			pclPrivateTrade_End->siRecvStockVillageUnique, pclPrivateTrade_End->siRecvStockNum,
			pclPrivateTrade_End->siSendStockVillageUnique, pclPrivateTrade_End->siSendStockNum,
			pclPrivateTrade_End->siRecvHouseVillageUnique, pclPrivateTrade_End->siRecvHouseUnitSlot,
			&pclPrivateTrade_End->clRecvRentContract,
			pclPrivateTrade_End->siSendHouseVillageUnique, pclPrivateTrade_End->siSendHouseUnitSlot,
			pclPrivateTrade_End->siHorseIndex, &pclPrivateTrade_End->clHorse 
			);
	}

	if( m_pDialog[ NPTRADEREPRAIR_DLG ])
	{
		((CNPrivateRepairDlg*)m_pDialog[NPTRADEREPRAIR_DLG])->PrivateTradeEnd( pclPrivateTrade_End->siResult,
			pclPrivateTrade_End->siRecvStockVillageUnique, pclPrivateTrade_End->siRecvStockNum,
			pclPrivateTrade_End->siSendStockVillageUnique, pclPrivateTrade_End->siSendStockNum,
			pclPrivateTrade_End->siRecvHouseVillageUnique, pclPrivateTrade_End->siRecvHouseUnitSlot,
			&pclPrivateTrade_End->clRecvRentContract,
			pclPrivateTrade_End->siSendHouseVillageUnique, pclPrivateTrade_End->siSendHouseUnitSlot,
			pclPrivateTrade_End->siHorseIndex, &pclPrivateTrade_End->clHorse 
			);
	}

	//KHY - 1001 - 기승동물 5마리 보유 수정.
	/*if( m_pDialog[ CLTNCHARSTATUS_DLG ] )
	{		
		((cltNCharStatus*) m_pDialog[ CLTNCHARSTATUS_DLG ])->m_pTabDlg1->m_bChangedInitHorseStatus = TRUE;
		((cltNCharStatus*) m_pDialog[ CLTNCHARSTATUS_DLG ])->m_pTabDlg1->InitHorseStatusInfo();
		((cltNCharStatus*) m_pDialog[ CLTNCHARSTATUS_DLG ])->m_pTabDlg1->UpdateHorseStatusInfo();
	}*/
	TCHAR* pText = GetTxtFromMgr(5757);

	pclMessage->SetMsg(pText);

	return;
}