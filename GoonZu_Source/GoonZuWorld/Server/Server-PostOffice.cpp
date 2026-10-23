#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"
#include "TraceBoot.h"

#include "Msg\MsgType-List.h"
#include "Msg\MsgType-PostOffice.h"
#include "MsgRval-Define.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-PostOffice.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h "
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "gachamanager\GachaManager.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PostOfficeSendItem *pclinfo = (cltGameMsgRequest_PostOfficeSendItem *)pclMsg->cData;

	// Person�� ��ġ�� ������ VillageUnique�� Ȯ���Ѵ�
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	
	//---------------------------------------------
	// ���� �Ǿ� �ִ� ������ ����� �ִٸ� ó���Ѵ�. (ó������ ������ ����� �������� �ٽ� ������ �� �ִ�.) 
	//---------------------------------------------
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// ���� �Ǿ� �ִ� źȯ������ ����� �ִٸ� ó���Ѵ�. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	pclinfo->szReceiverPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;

	// �ڱ� �ڽſ��Դ� ���� �� ����

	TCHAR personname[ MAX_PLAYER_NAME ];	
	int startpos = -1; 
	int endpos = -1;
	
	TCHAR ch;
	int i = 0;

	bool loop = true;
	while( loop ) 
	{
		ch = pclinfo->szReceiverPersonName[ i ];
		if( ch == NULL ) break;

		if( ch != ' ' ) 
		{
			if( startpos == -1 ) startpos = i;
			endpos = i + 1;
		}

		i++;
	}

	personname[ 0 ] = NULL;

	if( startpos != -1 ) 
	{
		TMemCpy( personname, &pclinfo->szReceiverPersonName[ startpos ], endpos - startpos + 1 );
	}

	personname[ MAX_PLAYER_NAME - 1 ] = NULL;


	if( _tcsicmp( personname, pclCM->CR[id]->pclCI->clBI.GetName() ) == 0 ) 
	{
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_SAMEPERSON,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	if ( pclinfo->siItemInventoryIndex < PERSONITEM_INV0 || pclinfo->siItemInventoryIndex >= MAX_ITEM_PER_PERSON )
	{
		return;
	}

	if( pclinfo->siItemUnique < 1 )
	{
		return;
	}


	if ( pclinfo->siItemNum < 1 || pclinfo->siItemNum > MAX_ITEM_PILE_NUMBER )
	{
		return;
	}
	
	// ������ unique Ȯ��
	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemInventoryIndex ].siUnique != pclinfo->siItemUnique )
	{
		return;
	}

	// ������ ������ �������� Ȯ��
	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemInventoryIndex ].siItemNum < pclinfo->siItemNum )
	{
		return;
	}

	bool isGacha = false;
	if ( ( pclItemManager->IsItemInfoAtb(pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].siUnique,ITEMINFOATB_NOTTRADE) ))
	{
		SI32 checkUnique = pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].siUnique;
		// ��í �������� �ƴϸ� �� ������
		if( false == pclGachaManager->IsGachaRareItem( checkUnique ) )
		{
			SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return ;
		}
		else
		{
			isGacha = true;
		}
	}

	// cyj �ͼӾ������� �������� ���������� ����
	// by LEEKH - 2007.10.01 - ������������ �ͼ�üũ�� ���� �ʴ´�.
	if (isGacha == false && 
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].IsBelonging(pclItemManager))
	{
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return ;
	}
	
	// ���λ��� �������̸� ���λ����� ��ǰ�� �������̶�� �ȼ� ����.
	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclinfo->siItemInventoryIndex	)
			{
				// ���� �������� ������ �����ϴ��� üũ 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}

		// �ͼ����� ���λ���
		if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclinfo->siItemInventoryIndex	)
		{
			// ���� �������� ������ �����ϴ��� üũ 
			if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
			{
				SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
				return ;
			}
		}
	}

	// [����]�Ⱓ�� ���������� üũ �� �Ⱓ�� �������̶�� GetItemUseAbleDay �� ����� ���ɼ� üũ
	if(pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].IsHaveUseDay())
	{	
		SI32 useableDay = 0;
		BOOL rVal = GetItemUseAbleDay(&pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex], &useableDay);
			
		if( rVal && useableDay < 0 ) 
		{	
			//KHY - 0723 - �Ⱓ������ �ı� ����.
			if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
			{
				SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_USEDATEOVER,  0, 0, pclCM->CR[id]->GetCharUnique() );				
				return;
			}
			else
			{
				// ���Ⱓ�� ���� ����ǰ�� ���� �� ����. - by LEEKH
				if( pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5 )
				{
					// �޽��� ���
					return;
				}
				else
				{
					// ���Ⱓ�� ���� �������� ������ �Ѵ�. ����� ���� �޽��� Ŭ���̾�Ʈ��
					// DB ���� ��û
					sDBRequest_UseItem useitem(	id,pclCM->CR[id]->pclCI->GetPersonID(),pclinfo->siItemInventoryIndex
												,&pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex],USEITEM_REASON_DATEOVER);

					pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);


					SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_USEDATEOVER,  0, 0, pclCM->CR[id]->GetCharUnique() );
					return;
				}
			}
		}
	}
	// ������
	GMONEY fee = 0;
	cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice( pclinfo->siItemUnique );
	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_POSTOFFICE );
	if(pStrInfo == NULL)return ;

	//KHY - 0614 - ���� �������� ���� ������ ĳ���Ϳ� ���Ͽ� �������� �ְ� ���� �� �ִ�. �����ᰡ �ٸ�. 
	//		fee =  pclClient->pclItemManager->clItemStatistic.siNPCPriceRate *1000;

	if(isGacha ==true)
		fee  = 0;
	else
		fee = pStrInfo->clPostOfficeStrInfo.clItemFee.CalcFee( pclprice->siPrice ) * pclinfo->siItemNum;

	if( fee > pclCM->CR[id]->pclCI->clIP.GetMoney() ) 
	{
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHFEE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	SI32 posttype = 0;
	if( pclCM->CR[id]->GetGameMasterLevel() >= 100 && pclCM->CR[id]->pclCI->clBI.uiGMMode == GMMODE_ATB_GMSHOW)
	{
		posttype = 1;		
	}

	//KHY - 0614 - ���� �������� ���� ������ ĳ���Ϳ� ���Ͽ� �������� �ְ� ���� �� �ִ�. 
	sDBRequest_PostOfficeSendItem	clMsg(  id, posttype, villageunique, personid, personname,
											pclinfo->siItemInventoryIndex, pclinfo->siItemUnique, pclinfo->siItemNum,
											&pclTime->clDate, fee ,isGacha);

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, fee);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDMONEY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PostOfficeSendMoney *pclinfo = (cltGameMsgRequest_PostOfficeSendMoney *)pclMsg->cData;

	// Person�� ��ġ�� ������ VillageUnique�� Ȯ���Ѵ�
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER) return;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	
	pclinfo->szReceiverPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;

	// �ڱ� �ڽſ��Դ� ���� �� ����
	if( _tcsicmp( pclinfo->szReceiverPersonName, pclCM->CR[id]->pclCI->clBI.GetName() ) == 0 ) {
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_SAMEPERSON,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	// �۱� ���� �ݾ�
	if( pclinfo->siMoney > pclClient->GetGlobalValue("GV_PostOfficeMoneyLimit") )	return ;

	if( pclinfo->siMoney > pclCM->CR[id]->pclCI->clIP.GetMoney() ) {

		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHMONEY,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	// ������
	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_POSTOFFICE );
	if(pStrInfo == NULL)return ;
	GMONEY fee = pStrInfo->clPostOfficeStrInfo.clMoneyFee.CalcFee( pclinfo->siMoney );

	if( pclinfo->siMoney + fee > pclCM->CR[id]->pclCI->clIP.GetMoney() ) {

		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHFEE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	SI32 posttype = 0;
	if( pclCM->CR[id]->GetGameMasterLevel() >= 100 && pclCM->CR[id]->pclCI->clBI.uiGMMode == GMMODE_ATB_GMSHOW)
	{
		posttype = 1;		
	}	

	sDBRequest_PostOfficeSendMoney	clMsg( id, posttype, villageunique, personid, 0, pclinfo->szReceiverPersonName,
		pclinfo->siMoney, &pclTime->clDate, fee );

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, fee);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDMSG(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PostOfficeSendMsg *pclinfo = (cltGameMsgRequest_PostOfficeSendMsg *)pclMsg->cData;

	// Person�� ��ġ�� ������ VillageUnique�� Ȯ���Ѵ�
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)
	{
		SendServerResponseMsg( 0, SRVAL_POSTOFFICE_FAIL_SEND_NOTPOSTOFFICE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return ;
	}

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// �ڱ� �ڽſ��Դ� ���� �� ����
	if( _tcsicmp( pclinfo->szReceiverPersonName, pclCM->CR[id]->pclCI->clBI.GetName() ) == 0 ) {
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_SAMEPERSON,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	// ������
	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_POSTOFFICE );
	if(pStrInfo == NULL)
	{
		SendServerResponseMsg( 0, SRVAL_POSTOFFICE_FAIL_SEND_NOTPOSTOFFICE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return ;
	}
	GMONEY fee = pStrInfo->clPostOfficeStrInfo.siMsgFee;

	if( fee > pclCM->CR[id]->pclCI->clIP.GetMoney() ) {

		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHFEE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	
	SI32 posttype = 0;
	if( pclCM->CR[id]->GetGameMasterLevel() >= 100 && pclCM->CR[id]->pclCI->clBI.uiGMMode == GMMODE_ATB_GMSHOW)
	{
		posttype = 1;		
	}

	sDBRequest_PostOfficeSendMsg	clMsg( id, posttype, villageunique, personid, 0, pclinfo->szReceiverPersonName,
		pclinfo->szMsg, &pclTime->clDate, fee );
		
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	

	pclDelMoneyStatistics->Add(DELMONEY_POSTOFFICE, fee);
}

// ������, ����, �޽��� ������ ������ ��û 
void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_RECVITEMLIST(cltMsg* pclMsg, SI32 id)
{
	// ������ �޽��� ���� �ð��� üũ�Ѵ�. 
	if(id && pclCM->CR[id]->CheckLastMsgClock(2000) == true)
	{
		return ;
	}

	cltGameMsgResponse_ListDeleteAll		sendMsg( LISTID_POSTOFFICE_RECVITEM );
	cltMsg clMsg(GAMEMSG_RESPONSE_LIST_DELETEALL, sizeof(cltGameMsgResponse_ListDeleteAll), (BYTE*)&sendMsg);
	
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);


	// Person�� ��ġ�� ������ VillageUnique�� Ȯ���Ѵ�
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER) return;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// ������ 
	{
		sDBRequest_PostOfficeRecvItemList	clMsg( id, villageunique, personid );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

	// ���� 
	{		
		sDBRequest_PostOfficeRecvMoneyList	clMsg( id, villageunique, personid );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

	// �޽���
	{
		sDBRequest_PostOfficeRecvMsgList	clMsg( id, villageunique, personid );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}


}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_RECVMONEYLIST(cltMsg* pclMsg, SI32 id)
{
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_RECVMSGLIST(cltMsg* pclMsg, SI32 id)
{
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_GETITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PostOfficeGetItem *pclinfo = (cltGameMsgRequest_PostOfficeGetItem *)pclMsg->cData;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 personid = pclchar->pclCI->GetPersonID();
	SI16 siInventoryIndex;

	//---------------------------------------------
	// ���� �Ǿ� �ִ� ������ ����� �ִٸ� ó���Ѵ�. (ó������ ������ ����� �������� �ٽ� ������ �� �ִ�.) 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// ���� �Ǿ� �ִ� źȯ������ ����� �ִٸ� ó���Ѵ�. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	if(pclClient->IsCountrySwitch(Switch_FourLeaf))
	{
		//[����] ����ũ�ι��� ���� ����Ҽ� ���� �Ÿ� ������ ���Ѵ�. => 2008-8-14
		if( ITEMUNIQUE(2755) == pclinfo->clItem.siUnique || ITEMUNIQUE(2754) == pclinfo->clItem.siUnique )	
		{	
			if( pclinfo->clItem.siUnique != siFourLeafUnique )	
			{
				SendServerResponseMsg( 0, SRVAL_RETURN_NOT_USE_FOURLEAF, 0, 0, 0 );
				return;	
			}
		}
	}

	cltItem cltoitem;
	SI32 rtnvalue;
	if( pclchar->CanAddInv(INVMODE_ALL, &pclinfo->clItem, &siInventoryIndex, &cltoitem, &rtnvalue) ) {

		sDBRequest_PostOfficeGetItem		clMsg( id, personid, pclinfo->siMsgID, siInventoryIndex, pclinfo->clItem.CanPile( pclItemManager ) );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_GETITEM_FULLINVEN,  0, 0, pclCM->CR[id]->GetCharUnique());
	}        
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_GETMONEY(cltMsg* pclMsg, SI32 id)
{
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	sDBRequest_PostOfficeGetMoney		clMsg( id, personid, pclMsg->siData32[ 0 ] );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_DELMSG(cltMsg* pclMsg, SI32 id)
{
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	{		
		sDBRequest_PostOfficeDelMsg			clMsg( id, personid, pclMsg->siData32[ 0 ] );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

	{
		LPARAM lParam = MAKELPARAM( pclMsg->siData32[ 0 ], LPARAMHIWORD_MSG );

		cltGameMsgResponse_ListDelItem		sendMsg( LISTID_POSTOFFICE_RECVITEM, lParam );
		cltMsg clMsg(GAMEMSG_RESPONSE_LIST_DELITEM, sizeof(cltGameMsgResponse_ListDelItem), (BYTE*)&sendMsg);
		
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SETFEE(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_PostOfficeSetFee *pclinfo = (cltGameMsgRequest_PostOfficeSetFee *)pclMsg->cData;

	// Person�� ��ġ�� ������ VillageUnique�� Ȯ���Ѵ�
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;
	
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	if( pclinfo->clMoneyFee.IsValid(1000) == FALSE ||
		pclinfo->clItemFee.IsValid(1000) == FALSE ||
		pclinfo->siMsgFee < 0 || pclinfo->siMsgFee > 10000 ||
		pclinfo->siRecvFee < 0 || pclinfo->siRecvFee > 10000 ) {

		// ������ ���� ����
		SendServerResponseMsg(RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SETFEE,  0, 0, pclCM->CR[id]->GetCharUnique());

		return;
		}
		

	sDBRequest_PostOfficeSetFee		clMsg( id, personid, villageunique, 
		&pclinfo->clMoneyFee, &pclinfo->clItemFee, pclinfo->siMsgFee, pclinfo->siRecvFee );

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::SendPostOfficeItemToPerson( TCHAR *szPersonName, cltItem *pclItem, TCHAR *szMsg, GMONEY money )
{
	if( szMsg && szMsg[ 0 ] != NULL ) {
		sDBRequest_PostOfficeSendMsg clMsg( 0, 0, 0, 0, 0, szPersonName, szMsg, &pclTime->clDate, 0 );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}	

	if( pclItem ) {
		sDBRequest_PostOfficeMasterSendItem clMsg( 0, 0, szPersonName, pclItem, &pclTime->clDate );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

	if( money > 0 ) {
		sDBRequest_PostOfficeSendMoney	clMsg( 0, 0, 0, 0, 0, szPersonName, money, &pclTime->clDate, 0 );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	

	}
	
}

void cltServer::SendPostOfficeItemToPerson( SI32 personid, cltItem *pclItem, TCHAR *szMsg, GMONEY money )
{
	
	if( szMsg && szMsg[ 0 ] != NULL ) {
		sDBRequest_PostOfficeSendMsg clMsg( 0, 0, 0, 0, personid, NULL, szMsg, &pclTime->clDate, 0 );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

	if( pclItem && pclItem->GetItemNum() > 0 ) {
		sDBRequest_PostOfficeMasterSendItem clMsg( 0, personid, NULL, pclItem, &pclTime->clDate );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

	if( money > 0 ) {
		sDBRequest_PostOfficeSendMoney	clMsg( 0, 0, 0, 0, personid, NULL, money, &pclTime->clDate, 0 );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
	}
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_GETRECORD(cltMsg* pclMsg, SI32 id)
{
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	sDBRequest_PostOfficeGetRecord	clMsg( id, personid );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GET_SAVEUSERITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Get_SaveUserItem* pclInfo = (cltGameMsgRequest_Get_SaveUserItem*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 personid = pclchar->pclCI->GetPersonID();
	SI16 siInventoryIndex;

	//---------------------------------------------
	// ���� �Ǿ� �ִ� ������ ����� �ִٸ� ó���Ѵ�. (ó������ ������ ����� �������� �ٽ� ������ �� �ִ�.) 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// ���� �Ǿ� �ִ� źȯ������ ����� �ִٸ� ó���Ѵ�. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	if(pclInfo->clItem.siUnique <= 0)
		return;

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclInfo->clItem.siUnique) ;
	if ( ref < 0 ) 
		return ;

	cltItem cltoitem;
	SI32 rtnvalue;
	if( pclchar->CanAddInv(INVMODE_ALL, &pclInfo->clItem, &siInventoryIndex, &cltoitem, &rtnvalue) )
	{
		SI32 itemunique = pclInfo->clItem.siUnique;

		cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclItemManager, itemunique);

		SI64 UserMoney = pclCM->CR[id]->pclCI->clIP.GetMoney();

		SI64 tempTax = ((pclprice->siPrice/100)*pclInfo->clItem.GetItemNum());

		// Ŭ��� ���� ��ǰ ȸ�� ������ ����
		if(bNoChargeGetItemFromNPC)				tempTax = 0;

		if( UserMoney < tempTax )
			return;

		sDBRequest_Get_SaveUserItem		clMsg( id, personid, pclInfo->siIndex, siInventoryIndex, pclInfo->clItem.CanPile( pclItemManager ), tempTax );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		pclDelMoneyStatistics->Add(DELMONEY_TAX, tempTax);
	}
}

//KHY - 1127 - Ŭ��� ��ǰ ���� ����
void cltServer::DoMsg_GAMEMSG_REQUEST_GET_SAVEUSERHORSE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Get_SaveUserHorse* pclInfo = (cltGameMsgRequest_Get_SaveUserHorse*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 personid = pclchar->pclCI->GetPersonID();

	if(pclInfo->clHorse.siHorseUnique<= 0)
		return;


	//���� ���� �� �� ��ִ��?�˻� �ؾ� �Ѵ�. - �󽽷����� �Ǵ�.

	SI32 enptyHorseindex = pclCM->CR[id]->pclCI->clHorseInfo.FindEmptyHorseIndex();

	if( enptyHorseindex >= 0 )
	{
		REAL32	npcrate = (REAL32)pclItemManager->clItemStatistic.siNPCPriceRate;
		npcrate = npcrate / (REAL32)100;

		SI64 serviceCharge = (SI64)((REAL32)20000 * npcrate);

		// Ŭ��� ���� ��ǰ ȸ�� ������ ����
		if(bNoChargeGetItemFromNPC)				serviceCharge = 0;
	
		sDBRequest_Get_SaveUserHorse clMsg( id, personid, pclInfo->siIndex, enptyHorseindex,  serviceCharge );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);			

		pclDelMoneyStatistics->Add(DELMONEY_TAX, serviceCharge);
	}
}
void cltServer::DoMsg_GAMEMSG_REQUEST_GET_SAVEUSERITEMLIST(cltMsg* pclMsg, SI32 id)
{
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	sDBRequest_Get_SaveUserItemList	clMsg( id, personid );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

// [��] �ü�Ȯ�� 2008-07-18
void cltServer::DoMsg_GAMEMSG_REQUEST_MARKETCONDITIONS(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_MarketConditions* pclInfo = (cltGameMsgRequest_MarketConditions*)pclMsg->cData;

	cltCharServer* pclchar = pclCM->GetCharServer(id);
	if ( NULL == pclchar )
	{
		return;
	}

	SI32 personid = pclchar->pclCI->GetPersonID();

	//-- ������ ��� -  �����ᰡ ���ٸ� �ƹ��͵� �� �ʿ䰡 ����.
	SI32 NPCCharUnique = pclInfo->siNPCCharUnique;
	if ( NPCCharUnique <= 0 ) return ;

	SI32 NPCID = pclCM->GetIDFromCharUnique( NPCCharUnique ) ;

	if ( NPCID <= 0 ) return ;

	if ( pclCM->IsValidID( NPCID ) == false ) return ;

	SI32 villageunique = pclCM->CR[NPCID]->GetCurrentVillageUnique();

	if ( villageunique < 0 || villageunique > CUR_VILLAGE_LIMIT ) return ;


	GMONEY serviceCharge = 0;
	
	REAL32	npcrate = (REAL32)pclItemManager->clItemStatistic.siNPCPriceRate;
	//npcrate = npcrate / (REAL32)100;

	if(pclInfo->selectWhatShowList ==1)
	{
		serviceCharge = (SI64)((REAL32)5000 /100 * npcrate);

	}
	else if(pclInfo->selectWhatShowList ==2)
	{
		serviceCharge = 2 *(SI64)((REAL32)5000 /100 * npcrate);
	}
	else if(pclInfo->selectWhatShowList ==3)
	{
		serviceCharge = 3 *(SI64)((REAL32)5000 /100 * npcrate);
	}

	if( pclchar->pclCI->clIP.GetMoney() < serviceCharge)
		return;

	//-----------------------------------------------------------------------------------------

	SI16 x = 0; // �ƽ� ī��Ʈ. - ������ġ.
	SI16 i =0;
	SI32 index  = 0;
	
	// ���� �ؾ� �� ����.
	cltMarketConditionsInfo rsInfo[100]; 
	_SYSTEMTIME			rsTime[8];

	if(pclInfo->selectWhatShowList ==1 ) // 1 = �Ϲ� �˻�
	{
		for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
		{
			if(pclItemManager->pclItemInfo[i] == NULL)continue;

			 index = pclItemManager->FindItemRefFromUnique(pclItemManager->pclItemInfo[i]->clItem.siUnique);

			 if(index ==0) continue;

			if((pclItemManager->pclItemInfo[i]->siType) == (pclInfo->siSelectItemTypeIndex))	
			{
				if( x < 100)
				{
					rsInfo[x].siUnique = pclItemManager->pclItemInfo[i]->clItem.siUnique;
					rsInfo[x].siMarketConditionsMoney =  pclItemManager->pclItemPrice->clUnit[index].siPrice; //���� �ü�.

					// ���װ� �ð��� ����.

					x++;
				}				
			}			
		}
	}
	else if(pclInfo->selectWhatShowList ==2 ) //  2 = ���� ���.
	{
		for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
		{
			if(pclItemManager->pclItemInfo[i] == NULL)		continue;

			 index = pclItemManager->FindItemRefFromUnique(pclItemManager->pclItemInfo[i]->clItem.siUnique);
			 if(index <= 0)									continue;

			if((pclItemManager->pclItemInfo[i]->siType) == (pclInfo->siSelectItemTypeIndex))	
			{
				if( x >= 100 )			break;

				rsInfo[x].siUnique					=  pclItemManager->pclItemInfo[i]->clItem.siUnique;
				rsInfo[x].siMarketConditionsMoney	=  pclItemManager->pclItemPrice->clUnit[index].siPrice; //���� �ü�.

				SI64 siBeforePrice = 0;
				if( pclItemManager->clMarketConditions[0] != NULL &&
					pclItemManager->clMarketConditions[0]->pclMarketConditionsB[rsInfo[x].siUnique] != NULL)
				{
					siBeforePrice = pclItemManager->clMarketConditions[0]->pclMarketConditionsB[rsInfo[x].siUnique]->siItemMarketConditions;
				}
				
				if( siBeforePrice == 0 )		siBeforePrice = pclItemManager->pclItemPrice->clUnit[index].siPrice;

				rsInfo[x].siDifferenceMoney =  (pclItemManager->pclItemPrice->clUnit[index].siPrice)
												- siBeforePrice; //����.
				//��¥�� �ʿ����.					

				x++;
			}			
		}
	}
	else if(pclInfo->selectWhatShowList ==3 ) //  3 = ������ �ü�. - ����ǰ�� ���� �����ϰ��� �ü��� �־�  �����ش�.
	{
		SI16 p = 0;

		if(pclInfo->siSelectItemIndex ==0) // 3 = ������ �ü��� ǰ�� ������ �־�� �Ѵ�.
			return ;


		for(p = 0;p < 7 ;p++) // 8��ġ�� ���ƾ� �Ѵ�.
		{
			if(pclItemManager->clMarketConditions[p] == NULL)continue;
			if(pclItemManager->clMarketConditions[p+1] == NULL)continue;
			if(pclItemManager->clMarketConditions[p]->pclMarketConditionsB[ pclInfo->siSelectItemIndex] == NULL)continue;
			if(pclItemManager->clMarketConditions[p+1]->pclMarketConditionsB[ pclInfo->siSelectItemIndex] == NULL)continue;

			if((pclItemManager->clMarketConditions[p]->pclMarketConditionsB[pclInfo->siSelectItemIndex]->siItemUnique) == (pclInfo->siSelectItemIndex))	 // ǰ���� ���ƾ� �Ѵ�.
			{
				if( x < 100)
				{
					rsInfo[x].siUnique =  pclInfo->siSelectItemIndex;
					rsInfo[x].siMarketConditionsMoney =  pclItemManager->clMarketConditions[p]->pclMarketConditionsB[ pclInfo->siSelectItemIndex]->siItemMarketConditions; //���� �ü�.

					rsInfo[x].siDifferenceMoney =  ( pclItemManager->clMarketConditions[p]->pclMarketConditionsB[ pclInfo->siSelectItemIndex]->siItemMarketConditions)
												 - (pclItemManager->clMarketConditions[p+1]->pclMarketConditionsB[ pclInfo->siSelectItemIndex]->siItemMarketConditions); //���� ����

					memcpy(&rsTime[p], &pclItemManager->clMarketConditions[p]->sTimeMarketConditions , sizeof(_SYSTEMTIME));

					x++;
				}				
			}			
		}
	}

	// ������ ����Ͽ�, ���� ���� ���� ���´�. - ������ �����Ͱ� �������� �����̴�.
	if(x == 0) return ;

	// ���� ���δ�
	GMONEY changmoney = serviceCharge ;

	sDBRequest_ChangeMoney clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), CHANGE_MONEY_REASON_MARKETCONDITIONS, -changmoney);
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

	pclDelMoneyStatistics->Add(DELMONEY_MARKETCONDITIONS, changmoney);

	// ��ȭ�� ���� 
	cltMoney clmoney;
	clmoney.Set(changmoney - changmoney/20);
	clCurrencyInfo.clOutputMoney_NPC.Increase(&clmoney);

	// ������ ���ͱ��� �߰��Ѵ�.
	if( villageunique > 0 && villageunique < MAX_VILLAGE_NUMBER)
	{
		sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, villageunique, changmoney/20 );
		pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
	}

	// ����Ʈ ����
	if ( pclInfo->selectWhatShowList == 1 )
	{
		pclchar->IncQuestPara( QUEST_CLEAR_COND_MARKETCONDITION, pclInfo->siSelectItemTypeIndex, 1 );
	}

	cltGameMsgResponse_MarketConditions rspMsg;

#ifdef _SAFE_MEMORY
	rspMsg.clMarketConditionsInfo.MemCpy(rsInfo);
	rspMsg.sTimeMarketConditionsInfo.MemCpy(rsTime);
#else
	memcpy( rspMsg.clMarketConditionsInfo, rsInfo, sizeof( cltMarketConditionsInfo ) * 100 );
	memcpy( rspMsg.sTimeMarketConditionsInfo, rsTime, sizeof( _SYSTEMTIME ) * 8 );
#endif
	cltMsg clMsg2(GAMEMSG_RESPONSE_MARKETCONDITIONS, sizeof(cltGameMsgResponse_MarketConditions), (BYTE*)&rspMsg);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);

}

//[����] ��ü������ ��ü �޽��� ������. => 2008-8-4
void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDITEM_GROUP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PostOffice_SendItem_Group *pclinfo = (cltGameMsgRequest_PostOffice_SendItem_Group *)pclMsg->cData;

	if( false == pclCM->IsValidID(id) )	return;
	
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();	// Person�� ��ġ�� ������ VillageUnique�� Ȯ���Ѵ�
	if( villageunique <= 0 )			return;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	pclchar->ItemUseReserveAction();		// ���� �Ǿ� �ִ� ������ ����� �ִٸ� ó���Ѵ�. (ó������ ������ ����� �������� �ٽ� ������ �� �ִ�.) 
	pclchar->BulletUseReserveAction();		// ���� �Ǿ� �ִ� źȯ������ ����� �ִٸ� ó���Ѵ�. 

	if(	pclinfo->siItemInventoryIndex < PERSONITEM_INV0 ||  pclinfo->siItemInventoryIndex >= MAX_ITEM_PER_PERSON )			  
		return;

	if( pclinfo->siItemNum < 1 || pclinfo->siItemNum > MAX_ITEM_PILE_NUMBER ) 
		return;

	// ������ unique Ȯ��
	if( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemInventoryIndex ].siUnique != pclinfo->siItemUnique )
		return;
	
	bool isGacha = false;
	if ( ( pclItemManager->IsItemInfoAtb(pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].siUnique,ITEMINFOATB_NOTTRADE) ))
	{
		SI32 checkUnique = pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].siUnique;
		// ��í �������� �ƴϸ� �� ������
		if( false == pclGachaManager->IsGachaRareItem( checkUnique ) )
		{
			SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return ;
		}
		else
		{
			isGacha = true;
		}
	}

	// cyj �ͼӾ������� �������� ���������� ����
	// by LEEKH - 2007.10.01 - ������������ �ͼ�üũ�� ���� �ʴ´�.
	if (isGacha == false && 
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].IsBelonging(pclItemManager))
	{
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return ;
	}

	// ���λ��� �������̸� ���λ����� ��ǰ�� �������̶�� �ȼ� ����.
	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclinfo->siItemInventoryIndex	)
			{
				// ���� �������� ������ �����ϴ��� üũ 
				if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}

		// �ͼ����� ���λ���
		if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclinfo->siItemInventoryIndex	)
		{
			// ���� �������� ������ �����ϴ��� üũ 
			if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
			{
				SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
				return ;
			}
		}
	}

	// [����]�Ⱓ�� ���������� üũ �� �Ⱓ�� �������̶�� GetItemUseAbleDay �� ����� ���ɼ� üũ
	if(pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].IsHaveUseDay())
	{	
		SI32 useableDay = 0;
		BOOL rVal = GetItemUseAbleDay(&pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex], &useableDay);

		if( rVal && useableDay < 0 ) 
		{	
			//KHY - 0723 - �Ⱓ������ �ı� ����.
			if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
			{
				SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_USEDATEOVER,  0, 0, pclCM->CR[id]->GetCharUnique() );				
				return;
			}
			else
			{
				// ���Ⱓ�� ���� ����ǰ�� ���� �� ����. - by LEEKH
				if( pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5 )
				{
					// �޽��� ���
					return;
				}
				else
				{
					// ���Ⱓ�� ���� �������� ������ �Ѵ�. ����� ���� �޽��� Ŭ���̾�Ʈ��
					// DB ���� ��û
					sDBRequest_UseItem useitem(	id,pclCM->CR[id]->pclCI->GetPersonID(),pclinfo->siItemInventoryIndex
						,&pclCM->CR[id]->pclCI->clCharItem.clItem[pclinfo->siItemInventoryIndex],USEITEM_REASON_DATEOVER);

					pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);


					SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_USEDATEOVER,  0, 0, pclCM->CR[id]->GetCharUnique() );
					return;
				}
			}
		}
	}
	
	// ��ü�� ������ ���� �� ���.
	SI32 UserNum = 0;
	for(UserNum = 0; UserNum < MAX_SEND_USER_NUM; ++UserNum)
	{
		if( 0 >= _tcslen(pclinfo->m_szReceiverPersonNameGroup[UserNum]) )
			break;
	}
	
	// ���� ��ü �������� ������ ������ �������� ������ ���� ������ ����.
	SI16 UseItemTotalNum = UserNum * pclinfo->siItemNum;
	SI16 MyInvenItemNum  = pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemInventoryIndex ].siItemNum;
	if( MyInvenItemNum < UseItemTotalNum )
		return;


		// ������
	GMONEY fee = 0;
	cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice( pclinfo->siItemUnique );
	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_POSTOFFICE );
	if(pStrInfo == NULL)return ;
	
	if(isGacha ==true)  
		fee  = 0;
	else
		fee = pStrInfo->clPostOfficeStrInfo.clItemFee.CalcFee( pclprice->siPrice ) * pclinfo->siItemNum;


	if( fee > pclCM->CR[id]->pclCI->clIP.GetMoney() ) 
	{
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHFEE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	SI32 posttype = 0;
	if( pclCM->CR[id]->GetGameMasterLevel() >= 100 && pclCM->CR[id]->pclCI->clBI.uiGMMode == GMMODE_ATB_GMSHOW)
	{
		posttype = 1;		
	}

	for(SI32 Count = 0; Count < MAX_SEND_USER_NUM; ++Count)
	{
		if( 0 >= _tcslen(pclinfo->m_szReceiverPersonNameGroup[Count]) )
			break;

		if( 0 == _tcsicmp( pclClient->pclCM->CR[id]->GetName(), pclinfo->m_szReceiverPersonNameGroup[Count] ) )
			continue;
		
		//KHY - 0614 - ���� �������� ���� ������ ĳ���Ϳ� ���Ͽ� �������� �ְ� ���� �� �ִ�. 
		sDBRequest_PostOfficeSendItem	clMsg(  id, posttype, villageunique, personid, pclinfo->m_szReceiverPersonNameGroup[Count],
												pclinfo->siItemInventoryIndex, pclinfo->siItemUnique, pclinfo->siItemNum,
												&pclTime->clDate, fee ,isGacha);		

		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	

		pclDelMoneyStatistics->Add(DELMONEY_POSTOFFICE, fee);
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDMONEY_GROUP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PostOffice_SendMoney_Group *pclinfo = (cltGameMsgRequest_PostOffice_SendMoney_Group *)pclMsg->cData;

	if( false == pclCM->IsValidID(id) )					return;
	if( pclCM->CR[id]->GetCurrentVillageUnique() <= 0 )	return;

}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDMSG_GROUP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PostOffice_SendMsg_Group *pclinfo = (cltGameMsgRequest_PostOffice_SendMsg_Group *)pclMsg->cData;
  
	if( false == pclCM->IsValidID(id) )					return;

	// Person�� ��ġ�� ������ VillageUnique�� Ȯ���Ѵ�
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)
	{
		SendServerResponseMsg( 0, SRVAL_POSTOFFICE_FAIL_SEND_NOTPOSTOFFICE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return ;
	}

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// ������
	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_POSTOFFICE );
	if(pStrInfo == NULL)
	{
		SendServerResponseMsg( 0, SRVAL_POSTOFFICE_FAIL_SEND_NOTPOSTOFFICE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return ;
	}
	
	GMONEY fee = 500;

	if( fee > pclCM->CR[id]->pclCI->clIP.GetMoney() ) 
	{
		SendServerResponseMsg( RANKTYPE_POSTOFFICE, SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHFEE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	SI32 posttype = 0;
	if( pclCM->CR[id]->GetGameMasterLevel() >= 100 && pclCM->CR[id]->pclCI->clBI.uiGMMode == GMMODE_ATB_GMSHOW)
	{
		posttype = 1;		
	}

	for(SI32 Count = 0; Count < MAX_SEND_USER_NUM; ++Count)
	{
		if( 0 >= _tcslen(pclinfo->m_szReceiverPersonNameGroup[Count]) )
			break;
		
		sDBRequest_PostOfficeSendMsg	clMsg(  id, 
												posttype, 
												villageunique, 
												personid, 
												0, 
												pclinfo->m_szReceiverPersonNameGroup[Count],
												pclinfo->m_szMsg, 
												&pclTime->clDate, 
												fee );

		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	

		pclDelMoneyStatistics->Add(DELMONEY_POSTOFFICE, fee);
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_POSTOFFICE_USER_LIST_GROUP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PostOffice_User_List_Group *pclinfo = (cltGameMsgRequest_PostOffice_User_List_Group *)pclMsg->cData;

	if( false == pclCM->IsValidID(id) )					return;
	if( pclCM->CR[id]->GetCurrentVillageUnique() <= 0 )	return;

    cltGameMsgResponse_PostOffice_User_List_Group	sendMsg;

	switch(pclinfo->m_siType)
	{
		case cltGameMsgRequest_PostOffice_User_List_Group::FRIEND:
			{
				if( pclCM->CR[id]->pclFriendInfo == NULL)				return;
				if( 0 >= pclCM->CR[id]->pclFriendInfo->GetFriendNum() )	return;	// ģ�� ����.

				sDBRequest_Friend_Info	clMsg( POSTOFFICE_GOURP_MESSAGE, id, pclCM->CR[id]->pclCI->GetPersonID() );  
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
		
			}
			break;
		case cltGameMsgRequest_PostOffice_User_List_Group::GUILD:
			{
				if( false == pclCM->CR[id]->pclCI->clBI.clGuildRank.IsGuildMember() ) return;

				cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo( pclCM->CR[id]->pclCI->clBI.clGuildRank.GetGuildUnitIndex() );
				if(NULL == pGuild)	return;	// ��� ����.
					
				sDBRequest_Guild_Info	clMsg( POSTOFFICE_GOURP_MESSAGE, id, pclCM->CR[id]->pclCI->clBI.clGuildRank.GetGuildUnitIndex() );  
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
			}
			break;
		case cltGameMsgRequest_PostOffice_User_List_Group::VILLAGE:
			{
				SI32 siHomeVillage = pclCM->CR[id]->pclCI->clBI.siHomeVillage;
				if( siHomeVillage <= 0 || siHomeVillage > MAX_VILLAGE_NUMBER ) return;	// ���� ����.
				
				sDBRequest_Village_Resident_Info	clMsg( POSTOFFICE_GOURP_MESSAGE, id, pclCM->CR[id]->pclCI->clBI.siHomeVillage );  
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
			}
			break;
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CHAR_NAME_CHECK(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Char_Name_Check* pclinfo = (cltGameMsgRequest_Char_Name_Check*)pclMsg->cData;

	TraceBoot("NAMECHECK enter slot=%d name='%s' validID=%d villageUnique=%d",
		id, pclinfo->m_szAddPersonName,
		(int)pclCM->IsValidID(id),
		(int)(pclCM->IsValidID(id) ? pclCM->CR[id]->GetCurrentVillageUnique() : -1));

	if( false == pclCM->IsValidID(id) )														return;
	if( pclCM->CR[id]->GetCurrentVillageUnique() <= 0 )
	{
		TraceBoot("NAMECHECK ABORT villageUnique<=0 (slot has no village) - no DB request");
		return;
	}
	if( 0 ==_tcsicmp( pclClient->pclCM->CR[id]->GetName(), pclinfo->m_szAddPersonName) )	return;
	
	
	sDBRequest_Char_Name_Check	clMsg( id, POSTOFFICE_ADD_RECV_USER, pclinfo->m_szAddPersonName );
	TraceBoot("NAMECHECK SEND -> GameDBManager DBSELECT_BASE name='%s'", pclinfo->m_szAddPersonName);
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
}

