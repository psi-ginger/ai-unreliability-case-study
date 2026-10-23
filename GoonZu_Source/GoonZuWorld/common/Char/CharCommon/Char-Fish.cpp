//---------------------------------
// 2004/8/3 김태곤
//---------------------------------
#include "..\CharManager\CharManager.h"
#include "..\Common\Order\Order.h"
#include "..\Common\Util\Util.h"

#include "..\..\Client\Client.h"
#include "..\..\Server\Server.h"

//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

// 낚시가능 지역인가 ?
bool cltCharCommon::IsFishArea(SI32 x, SI32 y, SI32* pdir)
{
   SI32 i, j;
   SI32 tempx, tempy;
   SI32 tempdir = 0;

	  for(i = -2;i <= 2;i++)
		  for(j = -2;j<= 2;j++)
		  {
			  tempx	=	x	+	j;
			  tempy	=	y	+	i;
			  
			  // 캐릭터를 배치할 수 있는 여유가 있는 공간인지 확인한다. 
			  if(pclMap->IsColl(x	+	j, y	+	i, BLOCK_DEEPWATER | BLOCK_SHALLOWWATER))
			  {

				  cltPos clpos1(x, y);
				  cltPos clpos2(x	+	j, y	+	i);

				  bool bcheckswitch = false; 

				  SI32 curdir = DecideDirection8(&clpos1, &clpos2);
				  if(tempdir == 0 || tempdir == (NORTH|WEST) || tempdir == (SOUTH|EAST))
				  {
					  bcheckswitch = true;
				  }
				  else if((curdir == (SOUTH|WEST) || curdir == (NORTH|EAST)) && (tempdir == WEST || tempdir == NORTH))
				  {
					  bcheckswitch = true;
				  }
					  


				  if(bcheckswitch)
				  {
					  tempdir = curdir;
				  }
			  }
		  }

  if(tempdir)
  {
	  *pdir = tempdir;
	  return true;
  }
		  
   return false;
}

// 이 캐릭터 주변의 적절한 낚시 위치를 찾는다. 
bool cltCharCommon::FindFishArea(SI32* px, SI32* py)
{
	SI32 i, j;
	SI32 dir;
	SI32 range = 3;
	SI32 minx = 0, miny = 0;
	SI32 minlength = 1000;
	SI32 length;

	for(i = -range ; i <= range;i++)
		for(j = -range;j <= range;j++)
		{
			if(i == 0 && j == 0)continue;

			if(IsFishArea(GetX() + j, GetY() + i, &dir))
			{
				length = max(TABS(j), TABS(i));
				if(length < minlength)
				{
					minlength = length;
					minx = GetX() + j;
					miny = GetY() + i;
				}
			}
		}

		if(minlength < 1000)
		{
			*px = minx;
			*py	= miny;
			return true;
		}

	return false;

}

bool cltCharCommon::Fish()
{
	SetAnimation(ANITYPE_FISH);
	DoAnimationDelay();
	
	bBusySwitch = false;
	
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		cltCharServer* pclchar = (cltCharServer*)this;

		// 소환수이고 주인이 낚시를 중단했다면 자신도 낚시를 중단한다. 
		if ( clIdentity.clSummonInfo.siParentCharUnique > 0  )
		{
			SI32 OwnerCharID = pclClient->pclCM->GetIDFromCharUnique ( clIdentity.clSummonInfo.siParentCharUnique );
			
			if (pclCM->IsValidID( OwnerCharID ) )
			{
				if(pclCM->CR[OwnerCharID ]->GetCurrentOrder() != ORDER_FISH)
				{
					OrderForceStop(GetID(), BY_PLAYER);
					return false;
				}
			}
		}

	
		// 여기다가 낚시 루틴 넣을 것. 
		if ( pclchar->CatchFishAction( IsPC( GetCharUnique() ) ) )
		{
			if ( IsPC( GetCharUnique() ) )
			{	
				// 낚시 미끼 없어지는 경우 ( 낚았거나, 실패했거나 둘중 하나 )
				// 자동 낚시
				if ( pclchar->bAutoFishing )
				{
					if ( ! pclchar->FishingStart( pclchar->siFishingBaitItemUnique ) )
					{
						pclchar->FishingStop();
					}
				}
				else
				{
					pclchar->FishingStop();
				}
			
			}


			else		// 소환수가 낚은 경우. 미끼 아이템만 하나 삭제 시켜주면 됨.
			{
				if ( clIdentity.clSummonInfo.siParentCharUnique > 0  )
				{
					SI32 OwnerCharID = pclClient->pclCM->GetIDFromCharUnique ( clIdentity.clSummonInfo.siParentCharUnique );
					
					if ( OwnerCharID < 1 || pclClient->pclCM->IsValidID( OwnerCharID ) == FALSE )
					{
						return false;
					}

					cltCharServer *pclChar = (cltCharServer*)pclClient->pclCM->CR[OwnerCharID];
					if ( pclChar == NULL )
					{
						return false;
					}
					
					cltItem clItem;
					clItem.siItemNum = 1;
					clItem.siUnique = pclChar->siFishingBaitItemUnique;
					
					SI32 InvPos = pclChar->pclCI->clCharItem.GetItemPos( pclChar->siFishingBaitItemUnique );
					if ( InvPos < 0 || InvPos >= MAX_ITEM_PER_PERSON )
					{
						return false;
					}
					
					cltItem *pclFrom = &pclChar->pclCI->clCharItem.clItem[ InvPos ];
					
					//cyj 예약아이템에 위치 정보 추가
					while( pclChar->clItemUseReserveInfo.Push( clItem.siUnique, -clItem.siItemNum, InvPos ) == false )
					{
						pclChar->ItemUseReserveAction();
					}
					
					// 클라이언트로 업데이트 된 정보를 보낸다. 
					if( pclFrom->siItemNum == 1 )
					{
						pclChar->ItemUseReserveAction();
					}
					// pclchar->ItemUseReserveAction(); 밑에 있어야 한다. 
					// 그렇지 않으면 인벤에서 물품의 위치를 찾을 수 없다.!
					pclFrom->Decrease(1);
					
					// 클라이언트로 사용후 정보를 보낸다. 
					cltGameMsgResponse_PersonItemInfo clinfo( InvPos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclFrom, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclChar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
	}

	return true;
}

