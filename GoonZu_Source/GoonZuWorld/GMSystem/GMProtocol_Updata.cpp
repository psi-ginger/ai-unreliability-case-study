#include "GMProtocol.h"
#include "GMMsg.h"
#include <atlstr.h>

#include "../Client/Client.h"
#include "../Server/Server.h"

#include "../CommonLogic/House/House.h"
#include "../CommonLogic/MsgType-System.h"
#include "../common/skill/Skill-Manager.h"
#include "../common/Util/Util.h"


#include "../Common/Char/CharManager/CharManager.h"
#include "../Common/Char/CharServer/Char-Server.h"

#include "../CommonLogic/CommonLogic.h"
#include "../../Client/ninterface/ncharstatus/NCharPerson.h"
#include "../DBManager/GameDBManager_World/DBMsg-Log.h"

#include "../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../DBManager/GameDBManager_World/DBMsg-WorldMoney.h"
#include "../DBManager/GameDBManager_World/DBMsg-Stock.h"
#include "../DBManager/GameDBManager_World/DBMsg-Item.h"

#include "GMProtocol_Updata.h"
#include "GMDefine.h"


extern cltCommonLogic* pclClient;

SI16 CGMProtocol_Updata::BasicInfoUpdata( sGMRequest_UpdateData* pRecvMsg , SI32 siButtonID , cltCharServer* pclChar )
{
	if ( (pRecvMsg == NULL) || (siButtonID <= 0) )
	{
		return UPDATA_ERROR_MSG_DO_NOT_RUN;
	}

	cltServer*	pclserver	= (cltServer*)pclClient;
	if ( pclserver == NULL )
	{
		return UPDATA_ERROR_MSG_DO_NOT_RUN;
	}

	SI32		siPersonID	= 0;
	SI32		siCharID	= 0;


	pRecvMsg->GetDataInt( "personid", &siPersonID );
	// PersonID가 0 이하면 잘못받아온 아이디
	if ( siPersonID <= 0 )
	{
		return UPDATA_ERROR_MSG_NO_PERSONID;
	}

	// pclChar의 포인터값이 존재 한다면(캐릭터가 접속해 있다면) 케릭터 ID를 받아온다
	if ( pclChar != NULL )
	{
		siCharID = pclChar->GetCharID();
	}


	switch( siButtonID ) 
	{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  스테이터스 초기화
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case UPDATA_ID_BASICINFO_STATUS_RESET: // Status Reset
		{
			// 현재 접속중이 아닌 케릭터만 리셋이 가능하다
			if ( pclChar != NULL )
			{
				return UPDATA_ERROR_MSG_ONLY_DISCONNECT_USER;
			}
			
			sDBRequest_InitStatus clMsg( 0, siPersonID, INITSTATUSMODE_GMTOOL , 0 , NULL );
			pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

			if( pclClient->IsCountrySwitch( Switch_Elixir ) )
			{
				// 스테이터스 초기화시 엘릭서도 같이 초기화 시킨다.
				if( pclChar->pcltElixir && pclChar->pcltElixir->IsElixir() ) 
				{
					cltElixir_Info  clElixirInfo;
					clElixirInfo.m_siEndMinuteVary	= pclChar->pcltElixir->GetMinuteVary();
					clElixirInfo.m_uiMultiple 		= pclChar->pcltElixir->GetMultiple();
					clElixirInfo.m_siPoint	  		= pclChar->pcltElixir->GetTotalPoint();

					if(pclChar->pclCI->GetPersonID() > 0)
					{
						sDBRequest_Elixir_Set clMsg( pclChar->GetID(), pclChar->pclCI->GetPersonID(), &clElixirInfo );
						pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}
				}
			}
		}
		return Const_UpData_Success;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  사용자가 갖고있는 돈에 추가 및 차감하기
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case UPDATA_ID_BASICINFO_MONEY_ADD:
	case UPDATA_ID_BASICINFO_MONEY_SUB:
		{
			SI32	siGetMoney	= 0;
			GMONEY	gmMoney		= 0;


			pRecvMsg->GetDataInt( "money", &siGetMoney );
			if ( siGetMoney <= 0 )
			{
				return UPDATA_ERROR_MSG_WRONG_MONEY;
			}

			// 차감하는 것이라면 -1을 곱해준다
			gmMoney = siGetMoney * ( siButtonID==UPDATA_ID_BASICINFO_MONEY_SUB ? (-1) : (1) );
			
			sDBRequest_ChangeMoney clMsg( siCharID, siPersonID, CHANGE_MONEY_REASON_GAMEMASTER, gmMoney);
			pclserver->SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clMsg );
		}
		return Const_UpData_Success;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  아큐(무역머니) 추가 및 차감하기
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case UPDATA_ID_BASICINFO_ACU_ADD:
	case UPDATA_ID_BASICINFO_ACU_SUB:
		{
			SI32 siGetMoney		= 0;
			SI64 siWorldMoney	= 0;


			pRecvMsg->GetDataInt( "money", &siGetMoney );
			if ( siGetMoney <= 0 ) 
			{
				return UPDATA_ERROR_MSG_WRONG_MONEY;
			}
			
			// 차감하는 것이라면 -1을 곱해준다.
			siWorldMoney = siGetMoney * ( siButtonID==UPDATA_ID_BASICINFO_ACU_SUB ? (-1) : (1) );

			sDBRequest_WorldMoney_Set_Char clMsg( siCharID, siPersonID, siWorldMoney);
			pclserver->SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clMsg );
		}
		return Const_UpData_Success;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  주식 추가하기
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	case UPDATA_ID_BASICINFO_STOCK_ADD:
		{
			SI32 siVillageUnique	= 0;
			SI32 siStockAmount		= 0;


			pRecvMsg->GetDataInt( "villageunique", &siVillageUnique );
			if ( (siVillageUnique <= 0) || (siVillageUnique >= MAX_VILLAGE_NUMBER) )
			{	
				return UPDATA_ERROR_MSG_WRONG_VILLAGE_UNIQUE;
			}

			pRecvMsg->GetDataInt( "stockamount", &siStockAmount );
			if (siStockAmount <= 0)
			{	
				return UPDATA_ERROR_MSG_WRONG_STOCK_AMOUNT;
			}

			sDBRequest_StockDistributionOrder clMsg( siVillageUnique, siPersonID, siCharID, siStockAmount, 0, false );
			pclserver->SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clMsg );	
		}
		return Const_UpData_Success;
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  버프 수정하기
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case UPDATA_ID_BASICINFO_BUF_MODIFY:
		{
			SI32	siBufNumber	= 0;
			SI32	siBufSlot	= -1;
			
			SI32	siBufTime	= 0;
			SI32	siBufMonth	= 0;
			SI32	siBufDay	= 0;
			SI32	siBufHour	= 0;
			
			SI16	siPercent	= 0;
			
			stBuf	bufAdd;

			
			pRecvMsg->GetDataInt( "bufnumber", &siBufNumber );
			if ( (siBufNumber < 1) || (siBufNumber >= BUF_END) )
			{
				return UPDATA_ERROR_MSG_WRONG_BUF_NUMBER;
			}

			pRecvMsg->GetDataInt( "bufslot", &siBufSlot );
			if ( (siBufSlot < 0) || (siBufSlot >= MAX_BUF) )
			{
				return UPDATA_ERROR_MSG_WRONG_BUF_SLOT;
			}

			pRecvMsg->GetDataInt( "bufmonth", &siBufMonth );
			pRecvMsg->GetDataInt( "bufday", &siBufDay );
			pRecvMsg->GetDataInt( "bufhour", &siBufHour );
			if ( ((siBufHour<0) || (siBufHour>24)) && ((siBufDay<0) || (siBufDay>30)) && ((siBufMonth<0) || (siBufMonth>12)) )
			{
				return UPDATA_ERROR_MSG_WRONG_BUF_TIME;
			}

			// 혹시 모를 리턴값이 잘못 되었을때를 대비
			siBufTime = pclserver->pclTime->clDate.GetHourVary() + (siBufHour) + (24*siBufDay) + (24*30*siBufMonth);
			if ( (siBufTime <= 0) || (pclserver->pclTime->clDate.GetHourVary() > siBufTime) )
			{
				return UPDATA_ERROR_MSG_WRONG_BUF_TIME;
			}

			// 버프 타입에따른 효과비율 설정
			switch (siBufNumber)
			{
				case BUF_MAKEEXPUP:
					{
						siPercent = 50;
					}
					break;
				case BUF_ATTACKUP:
				case BUF_ATTACKUP30:
				case BUF_DEFENSEUP:
				case BUF_DEFENSEUP30:
				case BUF_SUMMONATACUP:	
					{
						siPercent = 20;
					}
					break;
				default:
					{
						siPercent = 0;
					}
					break;
			}

			// 최종 버프 설정			
			bufAdd.siBufType	= siBufNumber;
			bufAdd.siPercent	= siPercent;
			//bufAdd.siAdd		= 0;
			bufAdd.siOverTime	= siBufTime;
			//bufAdd.bOverlap	= false;

			bufAdd.siGetType	= 0;
			
			sDBRequest_AddBuf clMsg( siPersonID, siBufSlot, &bufAdd);
			pclserver->SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clMsg );
			
		}
		return Const_UpData_Success;
	}

	return UPDATA_ERROR_MSG_DO_NOT_RUN;

}

SI16 CGMProtocol_Updata::SkillInfoUpdata( sGMRequest_UpdateData* pRecvMsg , SI32 siButtonID , cltCharServer* pclChar )
{
	/*
	if ( (pRecvMsg == NULL) || (siButtonID <= 0) )
	{
		return UPDATA_ERROR_MSG_DO_NOT_RUN;
	}

	cltServer*	pclserver	= (cltServer*)pclClient;
	if ( pclserver == NULL )
	{
		return UPDATA_ERROR_MSG_DO_NOT_RUN;
	}

	SI32		siPersonID	= 0;
	SI32		siCharID	= 0;


	pRecvMsg->GetDataInt( "personid", &siPersonID );
	// PersonID가 0 이하면 잘못받아온 아이디
	if ( siPersonID <= 0 )
	{
		return UPDATA_ERROR_MSG_NO_PERSONID;
	}

	// pclChar의 포인터값이 존재 한다면(캐릭터가 접속해 있다면) 케릭터 ID를 받아온다
	if ( pclChar != NULL )
	{
		siCharID = pclChar->GetCharID();
	}


	switch( siButtonID ) 
	{
		case UPDATA_ID_SKILL_NONE:
			{
				return UPDATA_ERROR_MSG_DO_NOT_RUN;
			}
	
		return Const_UpData_Success;
	}
	*/
	return UPDATA_ERROR_MSG_DO_NOT_RUN;
}

SI16 CGMProtocol_Updata::InventoryUpdata( sGMRequest_UpdateData* pRecvMsg , SI32 siButtonID , cltCharServer* pclChar )
{
	if ( (pRecvMsg == NULL) || (siButtonID <= 0) )
	{
		return UPDATA_ERROR_MSG_DO_NOT_RUN;
	}

	cltServer*	pclserver	= (cltServer*)pclClient;
	if ( pclserver == NULL )
	{
		return UPDATA_ERROR_MSG_DO_NOT_RUN;
	}

	SI32		siPersonID	= 0;
	SI32		siCharID	= 0;


	pRecvMsg->GetDataInt( "personid", &siPersonID );
	// PersonID가 0 이하면 잘못받아온 아이디
	if ( siPersonID <= 0 )
	{
		return UPDATA_ERROR_MSG_NO_PERSONID;
	}

	// pclChar의 포인터값이 존재 한다면(캐릭터가 접속해 있다면) 케릭터 ID를 받아온다
	if ( pclChar != NULL )
	{
		siCharID = pclChar->GetCharID();
	}


	switch( siButtonID ) 
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//  아이템 추가하기
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case UPDATA_ID_INVENTORY_ITEM_ADD:
			{
				char		szItemName[MAX_ITEM_NAME]	= {0, };	// 아이템 이름(혹은 Unique번호)
				SI32		siItemUnique				= 0;		// 실제 지급될 아이템 Unique
				SI32		siItemAmount				= 0;		// 아이템 갯수
				SI32		siInventorySlot				= 0;		// 아이템이 들어갈 케릭터 인벤 번호
				SI32		siReference					= 0;		// 아이템 사용기간을 얻어올때 참조하는 변수
				UI16		uiUseDate					= 0;		// 아이템 사용기간

				cltItem		clItem;									// 실제 만들어질 아이템
				SI16		siRareSwitch				= 0;		// 아이템 만들때 필요한 변수
				bool		bMakeResult					= false;	// 아이템 생성 결과
				bool		bPile						= false;	// 아이템 중첩 가능여부

				// 아이템 이름 가져오기
				pRecvMsg->GetDataStr( "itemname", szItemName, MAX_ITEM_NAME );
				if ( strcmp( szItemName, "" )  == 0 )
				{
					return UPDATA_ERROR_MSG_WRONG_ITEM_NAME;
				}
				
				// 일본이 아닌 다른 국가는 아이템 이름의 '_'표시를 스페이스로 변환한다.(국가처리는 함수안에서 함)
				ConvertUnderLineToSpace( szItemName, MAX_ITEM_NAME );
				
				// 아이템이 unique가 아니라 이름일 경우를 먼저 검사
				siItemUnique = pclserver->pclItemManager->FindUniqueFromName( szItemName );
				if ( siItemUnique <= 0 )
				{
					siItemUnique = atoi( szItemName );
					if ( siItemUnique <= 0 )
					{
						return UPDATA_ERROR_MSG_WRONG_ITEM_NAME;
					}
				}

				// 아이템 갯수 체크
				pRecvMsg->GetDataInt( "itemamount", &siItemAmount );
				if ( (siItemAmount <= 0) || (siItemAmount > MAX_ITEM_PILE_NUMBER) )
				{
					return UPDATA_ERROR_MSG_WRONG_ITEM_NUM;
				}
				
				// 아이템을 지급할 캐릭터 슬롯번호 체크
				pRecvMsg->GetDataInt( "inventoryslot", &siInventorySlot );
				if ( (siInventorySlot < PERSONITEM_INV0) || (siInventorySlot > (PERSONITEM_BAG1+MAX_ITEM_PER_BAG)) )
				{
					return UPDATA_ERROR_MSG_WRONG_INVENTORY_SLOT;
				}
				
				// 아이템 사용기간을 얻어오기위한 사전작업
				siReference = pclClient->pclItemManager->FindItemRefFromUnique( siItemUnique );
				if ( siReference <= 0 )
				{
					return UPDATA_ERROR_MSG_DO_NOT_MAKE_ITEM;
				}
				
				// 아이템 사용기간
				uiUseDate = pclClient->pclItemManager->pclItemInfo[ siReference ]->clItem.uiDateDay;

				clItem.Init();

				bMakeResult = pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clItem, 0, 0, &siRareSwitch, 0, 0, 0, 0, 0, uiUseDate );
				
				// 아이템 생성 실패
				if ( bMakeResult == false )
				{
					return UPDATA_ERROR_MSG_DO_NOT_MAKE_ITEM;
				}

				// 아이템 중첩 여부에 따라 갯수설정을 한다.
				bPile = clItem.CanPile( pclClient->pclItemManager );
				if ( (bPile == false) && (siItemAmount > 1) )
				{
					return UPDATA_ERROR_MSG_WRONG_ITEM_NUM;
				}

				clItem.SetItemNum( siItemAmount );
				
				sDBRequest_InputItemToPerson clMsg(
					siCharID, 
					siPersonID, 
					siInventorySlot, 
					INPUTITEMPERSON_OPTION_MASTER, 
					bPile, 
					&clItem, 
					0, 
					clItem.IsCreateSID(pclClient->pclItemManager));
				pclserver->SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clMsg );
			}
			return Const_UpData_Success;
	}

	return UPDATA_ERROR_MSG_DO_NOT_RUN;

}