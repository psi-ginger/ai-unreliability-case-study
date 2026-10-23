
#include "..\..\Message\Message.h"
#include "..\..\..\Server\time\mytime.h"
#include "..\..\Client\Music\Music.h"
#include <CommonLogic.h>
#include "..\..\..\Server\Server.h"
#include "../../PartyMgr/PartyMgr.h"
#include "../../PartyMgr/PartyObj.h"
//------------------------------------
// Common
//------------------------------------
#include "PersonInfo.h"
#include "..\CharManager\CharManager.h"

#include "..\..\Order\Order.h"
#include "..\..\Util\Util.h"
#include "char\CharCommon\Char-Common.h"

//------------------------------------
// CommonLogic
//------------------------------------
#include "..\..\CommonLogic\MagicMgr\MagicMgr.h"
#include "MsgRval-Define.h"
#include "../../CommonLogic/Msg/MsgType-Party.h"


extern cltCommonLogic* pclClient;

// 명령을 새로 수여하기 전에 해야할 초기화 
void cltCharCommon::InitPreGetOrder()
{
	// 내가 공격하는 캐릭터를 초기화한다.
	clAttackTarget.Init();
	
	// 피격 정보를 초기화한다. 
	clBeHitInfo.Init();

}


// 비축된 명령을 꺼내서 수행한다. 
BOOL cltCharCommon::GetOrderBuffer()
{
	int id=0;
	DWORD flag = 0;
	SI32 returnval = 0;

	//-------------------------------------
	// 다음 명령이 없으면 리턴한다. 
	//-------------------------------------
	if(ReserveOrder.siOrder==ORDER_WAIT)return FALSE;

	// 죽은 캐릭터이고 부활명령이 아니라면 명령을 수행할 수 없다. 
	if(ReserveOrder.siOrder != ORDER_REVIVE)
	{
		if(GetLife() < 0 )return FALSE;
		if(bBusySwitch	==	true)return FALSE;
	}
	

	//----------------------------------------------------------------
	// 포박 기술을 당하고 있는 중에는 정지 명령 이외에는 받지 않는다. 얼거나 스턴이 걸리면 움직일수 없다. 
	//----------------------------------------------------------------
	if(clInnerStatus.IsStatus(INNERSTATUS_ICE) || clInnerStatus.IsStatus(INNERSTATUS_FROZEN) || clInnerStatus.IsStatus(INNERSTATUS_STUN) )
	{
		if(ReserveOrder.siOrder != ORDER_STOP && ReserveOrder.siOrder != ORDER_FORCESTOP)
		{
			return FALSE;
		}
	}
	//----------------------------------------------------------
	// 사용자 명령이 입력된 경우 워프를 하지 못하게 초기화한다. 
	//----------------------------------------------------------
	if(ReserveOrder.siHostMode == BY_PLAYER)
	{
		InitWarpInfo();
	}

	//---------------------------------------------------------
	// 서버인 경우 예약된 공격 명령이 있으면 수행한다. 
	//---------------------------------------------------------
	// 서버인 경우에만. 
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{

		if(ReserveOrder.siOrder == ORDER_STOP)
		{
			cltOrderDataInfo clorder;
			if(((cltCharServer*)this)->DoReservedAttackAction(&clorder) == true)
			{
				ReserveOrder.Set(&clorder);
			}
		}
	}

	switch(ReserveOrder.siOrder)
	{
	case ORDER_STOP:
		InitPreGetOrder();
		clCurOrder.Set(&ReserveOrder);
		break;
	case ORDER_FORCESTOP:
		InitPreGetOrder();
		clCurOrder.Set(&ReserveOrder);

		// 서버인 경우에만. 
		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			//------------------------------------
			// 이웃들에게 정지하라는 지시를 보낸다. 
			//-------------------------------------
			cltOrderDataInfo clOrderDataInfo;
			if( pclClient->pclOrderManager->MakeOrder_FORCESTOP(&clOrderDataInfo, BY_PLAYER) == true)
			{
				cltOrderData clOrderData;
				clOrderData.Make(GetCharUnique(), &clOrderDataInfo);
				
				cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
				((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
			}
		}	
		break;

	case ORDER_REVIVE:
		bBusySwitch			= false;
		
		// 클라이언트에서 임시로 살린다. 
		if(pclClient->GameMode != GAMEMODE_SERVER)
		{
			pclCI->clIP.SetLife(1);
		}

		InitPreGetOrder();
		clCurOrder.Set(&ReserveOrder);
		SetCurrentOrder(ORDER_STOP);
		break;

		
	case ORDER_MOVE:

		if(IsAtb(ATB_MOVE))
		{
			SI32 x =	LOWORD(ReserveOrder.siPara1);
			SI32 y =	HIWORD(ReserveOrder.siPara1);
			
			// 이동해 가야 할 곳의 맵 인덱스. 
			siMoveMapIndex	= GetMapIndex();


			InitPreGetOrder();
			clCurOrder.Set(&ReserveOrder);

			SetMove(x, y);

			// 워프 게이트로의 이동인지 확인한다. 

			// 서버인 경우에만. 
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				//------------------------------------
				// 이웃들에게 이동하라는 지시를 보낸다. 
				//-------------------------------------
				cltOrderDataInfo clOrderDataInfo;
				if( pclClient->pclOrderManager->MakeOrder_MOVE(&clOrderDataInfo, x, y, BY_PLAYER) == true)
				{
					cltOrderData clOrderData;
					clOrderData.Make(GetCharUnique(), &clOrderDataInfo);
					
					cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
					((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
				}

				((cltCharServer*)this)->CharServerMoveByGate(x, y);


				if( siPartyIndex > 0)
				{
					CPartyObj* pParty =	pclClient->pclPartyMgr->GetPartyObj(siPartyIndex);
					if( pParty ) 
						pParty->NotifyMovePosToPartyMember(GetCharUnique(),x,y,false);
				}						
			}
			
		}
		
		break;
		
	case ORDER_STRAIGHTMOVE:
		if(IsAtb(ATB_MOVE))
		{

			SI32 x =	LOWORD(ReserveOrder.siPara1);
			SI32 y =	HIWORD(ReserveOrder.siPara1);

			// 이동해 가야 할 곳의 맵 인덱스. 
			siMoveMapIndex	= GetMapIndex();



			InitPreGetOrder();
			clCurOrder.Set(&ReserveOrder);

			SetMove(x, y);

			// 서버인 경우에만. 
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				//------------------------------------
				// 이웃들에게 이동하라는 지시를 보낸다. 
				//-------------------------------------
				cltOrderDataInfo clOrderDataInfo;
				if( pclClient->pclOrderManager->MakeOrder_STRAIGHTMOVE(&clOrderDataInfo, x, y, BY_PLAYER) == true)
				{
					cltOrderData clOrderData;
					clOrderData.Make(GetCharUnique(), &clOrderDataInfo);

					cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
					((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
				}
			}
			
		}
		
		break;
	
	case ORDER_ATTACK:
		{
			// 공격이 가능한 캐릭터만 공격할 수 있다. 
			// 적을 공격할 수 있다. 
			SI32 targetid  = ReserveOrder.siPara2;
			bool pvpFlag = ReserveOrder.siPara4 > 0 ? true : false;
			if(CanAttack(targetid, pvpFlag))
			{
				InitPreGetOrder();
				clCurOrder.Set(&ReserveOrder);
				siSubOrder	=	CHARWORKSTEP_DECIDELENGTH;
				
				SetAttack(targetid,pvpFlag);
				
				//-------------------------------------
				// 이웃들에게 공격하라는 요청을 보낸다. 
				//-------------------------------------
				// 서버인 경우에만. 
				if(pclClient->GameMode == GAMEMODE_SERVER)
				{
					cltOrderDataInfo clOrderDataInfo;
					if( pclClient->pclOrderManager->MakeOrder_ATTACK(&clOrderDataInfo, targetid, BY_PLAYER, ReserveOrder.siPara4 ) == true)
					{
						cltOrderData clOrderData;
						clOrderData.Make(GetCharUnique(), &clOrderDataInfo);
						
						cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
						if( IsPC( GetCharUnique() ) )
							((cltCharServer*)this)->SendNeighbourExceptMeMsg(&clMsg);
						else
							((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
					}
				}
				
			}
			
		}
		break;
	case ORDER_MAGIC:
		{
			SI32 magickind	= ReserveOrder.siPara3;
			bool pvpFlag	= ReserveOrder.siPara4 > 0 ? true : false;

			SI32 returnval = 0;

			stMagicAttributeData* pMagicAttributeData = pclClient->pclMagicManager->GetMagicAttribute( magickind );
			if ( NULL == pMagicAttributeData )
			{
				break;
			}

			SI32 targetid	= ReserveOrder.siPara2;
			if ( MAGIC_CASTTYPE_BUF == pMagicAttributeData->si08MagicCastType )
			{
				targetid = GetID();
			}

			bool bCanMagic = false;
			if ( MAGIC_CASTTYPE_BUF == pMagicAttributeData->si08MagicCastType )
			{
				bCanMagic = CanMagicBuf( magickind, &returnval );
			}
			else
			{
				bCanMagic = CanMagic(targetid, magickind, &returnval, pvpFlag);
			}

			if ( false == bCanMagic )
			{
				if(pclClient->GameMode == GAMEMODE_SERVER)
				{
					switch(returnval)
					{
					case CANUSEMAGIC_ERROR_NEEDMANA:	
					case CANUSEMAGIC_ERROR_FARDISTANCE:	
					case CANUSEMAGIC_ERROR_NEEDTIMEDELAY:
					case CANUSEMAGIC_ERROR_NOTENOUGHSKILL:
						((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_CANUSEMAGIC_FAIL,  returnval, 0, GetCharUnique()); 
						break;
					}
				}
				break;
			}

			//------------------------------------------
			// 서버인 경우 기존의 공격 정보를 보관한다. 
			//-----------------------------------------
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				if( IsPC(GetCharUnique()) == false && ( GetCurrentOrder() == ORDER_ATTACK || GetCurrentOrder() == ORDER_ATTACK_CLIENT ) )
				{
					((cltCharServer*)this)->SetReservedAttackTargetID(clAttackTarget.siAttackID);
					
				}
			}

			InitPreGetOrder();
			clCurOrder.Set(&ReserveOrder);

			//-----------------------------------
			// 캐릭터가 목표를 향하도록 설정한다.
			//-----------------------------------
			if ( (MAGIC_CASTTYPE_BUF != pMagicAttributeData->si08MagicCastType) && (pclCM->IsValidID(targetid)) )
			{
				SI32 dir = DecideDirection8(GetX(), GetY(), pclCM->CR[targetid]->GetX(), pclCM->CR[targetid]->GetY());
				if(dir)
				{
					ChangeDirectionNormal(dir);
				}

				if( pvpFlag )
				{
					// by LEEKH 2007.10.10 - PVP 국가스위치를 ServerInfo로 변경
					//if( pclClient->IsWhereServiceArea(ConstSwitch_PVP) )
					if(pclClient->siEnablePVP == 1)
					{
						clInnerStatus.clEnemyType.siEnemyType |= ENEMYTYPE_PVP;
						pclCM->CR[targetid]->clInnerStatus.clEnemyType.siEnemyType |= ENEMYTYPE_PVP;
					}
				}
			}

			//-------------------------------------
			// 이웃들에게 마법을 사용함을 통보한다. 
			//-------------------------------------
			// 서버인 경우에만. 
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				cltOrderDataInfo clOrderDataInfo;
				if( pclClient->pclOrderManager->MakeOrder_MAGIC(&clOrderDataInfo, targetid, magickind, BY_PLAYER, ReserveOrder.siPara4) == true)
				{
					cltOrderData clOrderData;
					clOrderData.Make(GetCharUnique(), &clOrderDataInfo);
					
					cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
					((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
				}
			}
			else
			{
				// 마법을 사용해보라는 퀘스트가 있는지 체크한다
				DoQuestCheckAndAction( QUEST_CLEAR_COND_USEMAGIC );

			}
		}
		break;
		
	case ORDER_FISH:
		{
			// Hero속성을 가진 사용자만 가능하다. 
			if(pclKI == NULL || pclKI->IsAtb(ATB_FISH) == false)return FALSE;


			// 낚시 방향을 잡는다. 
			if(ReserveOrder.siPara1)
			{
				ChangeDirection(ReserveOrder.siPara1);
			}
			
			InitPreGetOrder();
			clCurOrder.Set(&ReserveOrder);

			//-------------------------------------
			// 이웃들에게 낚시를 사용함을 통보한다. 
			//-------------------------------------
			// 서버인 경우에만. 
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				cltOrderDataInfo clOrderDataInfo;
				if( pclClient->pclOrderManager->MakeOrder_FISH(&clOrderDataInfo, ReserveOrder.siPara1, BY_PLAYER) == true)
				{
					cltOrderData clOrderData;
					clOrderData.Make(GetCharUnique(), &clOrderDataInfo);
					
					cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
					((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
				}
			}
			
		}
		break;
		
	case ORDER_FARM:
		{
			if(pclKI == NULL || pclKI->IsAtb(ATB_FARM) == false)return FALSE;

			SI32 x =	LOWORD(ReserveOrder.siPara1);
			SI32 y =	HIWORD(ReserveOrder.siPara1);

			SI32 rtnval = 0;
			if(CanFarm(x, y, &rtnval))
			{
				
				InitPreGetOrder();
				clCurOrder.Set(&ReserveOrder);
				
				SetFarm(x, y);
				
				//-------------------------------------
				// 이웃들에게 농사를 지음을 통보한다. 
				//-------------------------------------
				// 서버인 경우에만. 
				if(pclClient->GameMode == GAMEMODE_SERVER)
				{
					cltOrderDataInfo clOrderDataInfo;
					if( pclClient->pclOrderManager->MakeOrder_FARM(&clOrderDataInfo, x, y, BY_PLAYER) == true)
					{
						cltOrderData clOrderData;
						clOrderData.Make(GetCharUnique(), &clOrderDataInfo);
						
						cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
						((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
					}
				}
			}
			else
			{
				cltServer *pclServer = (cltServer *)pclClient;

				pclServer->SendServerResponseMsg( 0, SRVAL_FARM_FAIL, rtnval, 0, GetCharUnique() );
			}
		}
		break;

	case ORDER_MINE:
		{
			if(pclKI == NULL || pclKI->IsAtb(ATB_MINE) == false)return FALSE;

			SI32 x =	LOWORD(ReserveOrder.siPara1);
			SI32 y =	HIWORD(ReserveOrder.siPara1);

			SI32 rtnval = 0;
			if(CanMine(x, y, &rtnval))
			{
				InitPreGetOrder();
				clCurOrder.Set(&ReserveOrder);

				SetMine(x, y);

				//-------------------------------------
				// 이웃들에게 농사를 지음을 통보한다. 
				//-------------------------------------
				// 서버인 경우에만. 
				if(pclClient->GameMode == GAMEMODE_SERVER)
				{
					cltOrderDataInfo clOrderDataInfo;
					if( pclClient->pclOrderManager->MakeOrder_MINE(&clOrderDataInfo, x, y, BY_PLAYER) == true)
					{
						cltOrderData clOrderData;
						clOrderData.Make(GetCharUnique(), &clOrderDataInfo);

						cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
						((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
					}
				}
			}
			else
			{
				cltServer *pclServer = (cltServer *)pclClient;
				pclServer->SendServerResponseMsg( 0, SRVAL_MINE_FAIL, rtnval, 0, GetCharUnique() );
			}
		}
		break;

	case ORDER_SITDOWN:
		// 낚시 방향을 잡는다. 
		if(ReserveOrder.siPara1)
		{
			ChangeDirection(ReserveOrder.siPara1);
		}

		InitPreGetOrder();
		clCurOrder.Set(&ReserveOrder);

		//-------------------------------------
		// 이웃들에게 앉음을 통보한다.
		//-------------------------------------
		// 서버인 경우에만. 
		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			cltOrderDataInfo clOrderDataInfo;

			if( pclClient->pclOrderManager->MakeOrder_SITDOWN(&clOrderDataInfo, ReserveOrder.siPara1, BY_PLAYER) == true)
			{
				//KHY - 1109 - 가챠레어- 강아지방석추가
				if( IsHaveCushionItem() > 0 )
				{
					/*cltGameMsgResponse_IshaveCushion clMsg(true);
					
					cltMsg clMsg( , sizeof(clOrderData), (BYTE*)&clOrderData);
					((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);*/

					//clOrderDataInfo.siPara2 = 1;
					clOrderDataInfo.siPara2 = (SI16)IsHaveCushionItem(); //Para1으로 ->유니크타입. - 고민..
				}

				cltOrderData clOrderData;
				clOrderData.Make(GetCharUnique(), &clOrderDataInfo);

				cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
				((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);

				
			}
		}
		break;

	case ORDER_DIE:
		InitPreGetOrder();
		clCurOrder.Set(&ReserveOrder);
		break;

	case ORDER_ATTACK_CLIENT:
		{
			// 공격이 가능한 캐릭터만 공격할 수 있다. 
			// 적을 공격할 수 있다. 
			SI32 targetid  = ReserveOrder.siPara2;
			if(CanAttack(targetid))
			{
				InitPreGetOrder();
				clCurOrder.Set(&ReserveOrder);
				siSubOrder	=	CHARWORKSTEP_DECIDELENGTH;

				SetAttack(targetid);

				//-------------------------------------
				// 이웃들에게 공격하라는 요청을 보낸다. 
				//-------------------------------------
				// 서버인 경우에만. 
				if(pclClient->GameMode == GAMEMODE_SERVER)
				{
					cltOrderDataInfo clOrderDataInfo;
					if( pclClient->pclOrderManager->MakeOrder_ATTACK_CLIENT(&clOrderDataInfo, targetid, BY_PLAYER) == true)
					{
						cltOrderData clOrderData;
						clOrderData.Make(GetCharUnique(), &clOrderDataInfo);

						cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
                        ((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
					}
				}

			}
		}
		break;

	case ORDER_SPECIAL_ACTION:
		{
			InitPreGetOrder();
			clCurOrder.Set(&ReserveOrder);

			clAniInfo.Init1();

			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				cltOrderDataInfo clOrderDataInfo;
				if( pclClient->pclOrderManager->MakeOrder_SPECIAL_ACTION(&clOrderDataInfo, ReserveOrder.siPara1, BY_PLAYER) == true)
				{
					cltOrderData clOrderData;
					clOrderData.Make(GetCharUnique(), &clOrderDataInfo);

					cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
					((cltCharServer*)this)->SendNeighbourMsg(&clMsg, true);
				}
			}
		}
		break;

	default:
		break;
		}
		
		ReserveOrder.siOrder	=	ORDER_WAIT;
		
		return FALSE;
}

// 명령을 버퍼에 넣는다. 
BOOL cltCharCommon::PushOrder(cltOrderDataInfo order)
{

	if(ReserveOrder.siOrder		!=	ORDER_WAIT 
	&& ReserveOrder.siHostMode	!=	BY_COMPUTER
	&& order.siHostMode			==	BY_COMPUTER)
		return FALSE;
	
	ReserveOrder.Set(&order);
	
	return TRUE;
}
