//---------------------------------
// 2004/4/28 김태곤
//---------------------------------
#include "..\..\Server\Server.h"
#include "Skill\Skill-Manager.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"


extern cltCommonLogic* pclClient;

// 사용자가 보유한 말에 대한 행위를 처리한다. 
SI32 cltCharServer::CharHorseAction(cltHorse* pclhorse, bool bselfswitch)
{
	if(pclhorse == NULL)			return HORSEACTION_NONE;

	bool bupdateswitch = false;
	
	if(IsPC(GetCharUnique()) == false)return HORSEACTION_NONE;

	cltServer* pclserver = (cltServer*)pclClient;

	//  사용자의 말조련술 기술레벨을 얻어온다. 
#ifdef _SAFE_MEMORY
	SI32 horseskilllevel = pclCI->clCharSkill.GetTotalSkillLevel(SKILL_HORSETRAINING, 0 , 0, &pclCI->clCharItem.clItem[0]);
#else
	SI32 horseskilllevel = pclCI->clCharSkill.GetTotalSkillLevel(SKILL_HORSETRAINING, 0 , 0, pclCI->clCharItem.clItem);
#endif
	SI32 datevary = pclserver->pclTime->clDate.GetDateVary();
	
	SI32 rewardamount	= 0;
	SI32 horseaction	= pclserver->pclHorseManager->Action(pclserver, pclhorse, horseskilllevel, datevary, bselfswitch, &rewardamount);
	if(horseaction == 1)
	{
		// DB로 말에 대한 정보를 저장해야 한다. 
		bupdateswitch = true;
	}
	else if ( horseaction == 3 ) // 말능력향상이용권때문에
	{
		return HORSEATTION_TICKET;
	}
	else if ( horseaction == 4 ) // 말능력향상이용권때문에
	{
		return HORSEATTION_PREMIUMPARTS;
	}
	//KHY - 1001 - 기승동물 5마리 보유 수정. - 몇번째 말이 죽는지 알기위해서// 200 + 죽은말의 인덱스값으로 반환된다.
	else if(horseaction >= HORSEACTION_DIE)	// 사망처리. 
	{
		return horseaction;
	}


	// 보상이 있는 경우 말 조련술 경험치를 상승 시킨다. 
	if(rewardamount)
	{
		SI32 skillunique	= SKILL_HORSETRAINING;
		SI32 maxskilllevel	= pclClient->pclSkillManager->GetMaxLevel(skillunique);
		if(skillunique >= 0)
		{
			SetSkillUpdate(skillunique, rewardamount * 2);
		}
	}

	if(bupdateswitch)
	{
		return HORSEACTION_UPDATE;
	}

	return HORSEACTION_NONE;
	
}

// 이웃에게 내 말의 상태를 통보한다. 
bool cltCharServer::SendNeighbourMyHorseStatus(SI32 index)
{
	// 소유주 클라이언트에게 말의 정보를 업데이트 한다. 
	SetUpdateSwitch(UPDATE_HORSE, true, 0);
	
	// 이웃에게 말의 정보를 업데이트 한다. 
	if(index == pclCI->clHorseInfo.siCurrentShowHorse)
	{
		cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();

		if(pclhorse)
		{
			cltGameMsgResponse_NeighbourHorse clInfo(GetCharUnique(), pclhorse->siHorseUnique);
			cltMsg clMsg(GAMEMSG_RESPONSE_NEIGHBOURHORSE, sizeof(clInfo), (BYTE*)&clInfo);
			SendNeighbourMsg(&clMsg, false);
		}
	}

	return true;

}

// 자동으로 말 먹이를 준다. 
bool cltCharServer::AutoHorseFood(SI32 itemunique, SI32 itemnum)
{
	
	// 만능사료가 있는지 확인한다. 
	SI32 needitemunique	= ITEMUNIQUE(3855);
	SI32 needitemnum	= 1;
	SI32 pos = pclCI->clCharItem.GetItemPos(needitemunique);

	// 간편사료가 있는지 확인한다.
	if(pos < 0)
	{
		needitemunique = ITEMUNIQUE(3850);
		needitemnum	   = 1;
		pos = pclCI->clCharItem.GetItemPos(needitemunique);

	}

	if(pos < 0)
	{
		pos = pclCI->clCharItem.GetItemPos(itemunique);
		if(pos < 0)return false;

		needitemunique = itemunique;
		needitemnum = itemnum;
	}

	// 적절한 물품을 찾았다. 
	cltItemUseInfo clinfo(pos, needitemunique);
	((cltServer*)pclClient)->UseItemAction(GetID(), &clinfo);

	// 말사료가 없으면 필요한 먹이가 있는지 확인한다. 
	return true;
}

// 말 정보를 DB에 업데이트 한다. 
void cltCharServer::UpdateHorseToDB(cltServer* pclserver, SI32 index, cltHorse* pclhorse, SI32 reason)
{
	sDBRequest_SetHorse cldbmsg(GetID(), pclCI->GetPersonID(), reason, index, pclhorse);
	pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&cldbmsg);
}
