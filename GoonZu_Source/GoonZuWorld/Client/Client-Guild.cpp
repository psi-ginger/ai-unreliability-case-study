//---------------------------------
// 2005/06/16 정용래
//---------------------------------

#include "Client.h"
#include "Music\Music.h"
#include "..\CommonLogic\MsgType.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "../Common/Skill/Skill-Manager.h"

#include "MsgType-Guild.h"

#include "./Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "./Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "./NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "NStructureDlg/GuildStr/Guild/NGuildDlg.h"
#include "../CommonLogic/Guild/Guild.h"
#include "../CommonLogic/Guild/GuildRankManager.h"

// 길드의 기본정보를 받았다. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDUNITLIST(cltMsg* pclMsg)
{
	SI32 i;

	// 받은 정보를 확인한다. 
	cltGameMsgResponse_GuildUnitList* pclinfo = (cltGameMsgResponse_GuildUnitList*)pclMsg->cData;
	
	if(pclinfo == NULL ) return;

	// 받은 정보를 길드 관리자에게 전달한다. 
	for(i = 0;i < MAX_GUILDUNIT_NUMBER;i++)
	{
		pclGuildStructManager->clClientUnit[i].Set( &(pclinfo->clListInfo[i]) );
	}
	
	//// 받은 정보를 화면에 표시하도록 한다. 
	pclGuildStructManager->bUpdateGuildStrListShowChiefSwitch = true;
	pclGuildStructManager->bUpdateGuildStrUnitListShowSwitch		= true;
}


// 단위 길드의 상세정보를 받았다. (대행수 용)
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDUNIT_DETAIL(cltMsg* pclMsg)
{
	SI32 i;

	// 받은 정보를 확인한다. 
	cltGameMsgResponse_GuildUnitDetail* pclinfo = (cltGameMsgResponse_GuildUnitDetail*)pclMsg->cData;
	
	SI32 villageunique = 0;
	SI32 guildunitslot = 0;

	if(pclinfo == NULL || pclGuildStructManager == NULL ) return;

	villageunique = pclinfo->siVillageUnique;
	guildunitslot = pclinfo->siGuildUnitSlot;

	if(villageunique <= 0 || villageunique >=MAX_VILLAGE_NUMBER) return;
	if(guildunitslot < 0 || guildunitslot >=MAX_GUILDUNIT_NUMBER ) return;

	cltGuildStruct* pclGuildStruct = NULL;
	pclGuildStruct = ((cltGuildStruct*)(pclGuildStructManager->pclStruct[ villageunique ]));

	if( pclGuildStruct == NULL ) return;

	// 받은 정보를 길드 관리자에게 전달한다. 
    pclGuildStruct->clGuildUnit[ guildunitslot ].Set( &(pclinfo->clGuildUnit) );

	pclGuildRankManager->bUpdateGuildUnitListShowChiefSwitch = true;
}

// 단위 길드의 직급 정보를 받았다. (모든 사용자 용)
void cltClient::DoMsg_GAMEMSG_RESPONSE_GUILDUNITRANK(cltMsg* pclMsg)
{
	cltGameMsgResponse_GuildUnitRank* pclinfo = (cltGameMsgResponse_GuildUnitRank*)pclMsg->cData;

	if(pclinfo == NULL ) return;

	SI32 villageunique = 0;
	SI32 guildunitslot = 0;

	if(pclinfo == NULL || pclGuildStructManager == NULL ) return;

	villageunique = pclinfo->siVillageUnique;
	guildunitslot = pclinfo->siGuildUnitSlot;

	if(villageunique <= 0 || villageunique >=MAX_VILLAGE_NUMBER) return;
	if(guildunitslot < 0 || guildunitslot >=MAX_GUILDUNIT_NUMBER ) return;

	cltGuildRank* pclGuildRank = NULL;
	pclGuildRank = pclinfo->clGuildRank;

	if(pclGuildRank == NULL ) return;

	for(SI32 i=0; i<MAX_GUILDRANKTYPE_NUMBER + 1; i++)
	{
		pclGuildRankManager->clRank[villageunique][guildunitslot][i].Set( pclGuildRank + i );
	}

	pclGuildRankManager->bUpdateGuildUnitListShowSwitch = true;
}

