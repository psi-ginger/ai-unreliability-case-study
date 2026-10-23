//---------------------------------
// 20010/11/1 김태곤
//---------------------------------
#ifndef _CHARMANAGER_H
#define _CHARMANAGER_H

class cltIDManager;
class cltCharCommon;
class cltCharClient;
class cltCharServer;
class cltItem;
class cltStatistics;
class cltStatisticsManager;
class cltMonsterGroupManager;

#include "../NetworkLib/ITZNetLib/itzbase.h"
#include "../../../common/CommonHeader.h"


//-----------------------------------------------
// 캐릭터를 유지 관리하는 클래스. 
//-----------------------------------------------
#define MAX_USER_WEAPON_TYPE_NUMBER	14
#define MAX_USER_GIVEITEM_FROM_VILLAGE		1000
class cltCharManager{
public:
	cltIDManager* pclIDManager;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltCharCommon*, MAX_CHAR_NUMBER>	CR;
#else
	cltCharCommon*   CR[MAX_CHAR_NUMBER];				// 캐릭터의 정보를 가리키는 포인터를 저장하는 공간. 
#endif

#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_CHAR_NUMBER>	SessionSwitch;
#else
	bool			 SessionSwitch[MAX_CHAR_NUMBER];	// Session을 갖는 캐릭터인지 여부. 	
#endif

	SI32			siMaxSessionNumber;					// 세션 캐릭터의 개수. 
	SI32			siMaxInstanceNPC;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_USER_WEAPON_TYPE_NUMBER>	siUserWeaponStatistic;
#else
	SI32			siUserWeaponStatistic[MAX_USER_WEAPON_TYPE_NUMBER];			// 사용자 무장 통계. 
#endif

	DWORD			dwParametaBoxActionIntervalClock;	// ParametaBoxAction의 실행 간격. 
	SI32			siIncLifeUnit;						// 체력 자동 회복 단위. 


	cltCharManager(SI32 gamemode);
	~cltCharManager();

	// 캐릭터가 살아 있는지 여부를 알려준다. 
	BOOL IsAlive(SI32 id);

	//personid를 근거로 id를 구한다. 
	SI32 GetIDFromPersonID(SI32 personid);
	// CharUnique를 근거로 id를 구한다.
	SI32 GetIDFromCharUnique(SI32 charunique);
	//이름을 근거로 id를 구한다. 
	SI32 GetIDFromName(TCHAR* pname);
	// KIND를 근거로 id를 구한다.
	SI32 GetIDFromCharKind( SI32 charkind );



	BOOL Action();

	BOOL SetInfo(cltCharCommon* pclchar);
	bool IsValidID(SI32 id);

	SI32 GetCharOrder(SI32 index);

	// Person을 삭제한다. 
	BOOL DeleteChar(SI32 id);
	// 모든 캐릭터를 삭제한다. 
	BOOL DeleteChar();

	// 캐릭터 이름을 만든다.
	void MakeCharName();

	// 캐릭터 이름을 표시한다. 
	void DrawCharName();

	
	// 캐릭터 말풍선을 표시한다. 
	void DrawChatBalloon();

	
	// 모든 Person에게 메시지를 보낸다. 
	void SendAllPersonMsg(sPacketHeader* pmsg);

	// 특정 마을에 있는 사용자들에게 메시지를 보낸다. 
	void SendAllPersonInVillageMsg(sPacketHeader* pmsg, SI16 villageunique);
	
	// 특정 지도에  있는 사용자들에게 메시지를 보낸다. 
	void SendAllPersonInMapMsg(sPacketHeader* pmsg, SI16 mapindex);


	// 특정 마을의 주민에게만 메시지를 보낸다. 
	void SendAllPersonHomeMsg(sPacketHeader* pmsg, SI16 villageunique);
	
	// 특정 정보를 선택한 사용자에게 메시지를 보낸다. 
	void SendNewsMsg(sPacketHeader* pmsg, SI32 newskind);

	// 성인들에게만 메세지를 보낸다.
	void SendAllAdultUserMsg(sPacketHeader* pmsg);

	// 파티원들에게 메세지를 보냅니다.
	void SendAllPartyMemberMsg(sPacketHeader* pmsg, SI32 siPartyIndex);


	//_LEON_GUILD_CHAT
//#if defined(_DEBUG)
	// 특정 길드의 멤버에게만 메시지를 보낸다. 
	void SendAllGuildMemberMsg(sPacketHeader* pmsg,  SI32 GuildUnitIndex ,TCHAR * GuildName );
//#endif
	
	// 모든 사용자를 다 내보낸다. (DB에 저장도 한다. ) 
	void CloseAllSocket();

	// person이름을 가지고 PersonID를 찾는다. 
	SI32 GetPersonIDFromName(TCHAR* pname);

	// PersonID를 근거로 해당 캐릭터를 강제로 아웃 시킨다. 
	bool OutByPersonID(SI32 personid);
	
	// 모든 전쟁 전과를 삭제한다. 
	bool DeleteAllWarKill();

	bool DeleteBlackWarKill();
	
	// 주변의 아군에게 도움을 요청한다. 
	SI32 FindNearAlly(SI32 kind, SI32 mapindex, SI32 x, SI32 y, bool bwarswitch, SI16 villageunique);
	// 주변에서 데미지를 입은 아군을 찾는다. 
	SI32 FindDamagedFriend(SI32 myid, SI32 mapindex, SI32 x, SI32 y, bool bwarswitch);


	// 사용중인 무장에 대한 통계를 낸다. 
	void AddUserWeapon(cltItem* pclitem);
	
	// 특정 맵인덱스에 있는 몬스터를 전부 삭제한다. 
	void DeleteAllNPCInMapIndex(SI32 mapindex);

	// 사용자의 접속 여부와 관계없이 경험치를 준다.
	void IncExp(SI32 personid, SI32 exp, SI32 reason);
	
	// 사용자의 접속 여부와 관계없이 명성 경험치를 준다.
	void IncFameExp(SI32 personid, SI32 exp, SI32 serverreason, SI32 dbreason);


	// 특정 마을에 현재 위치하고 있는 해당 마을 주민들의 리스트. 
	void FindResidentListInVillage(SI32 villageunique);

	// 특정 마을안에 있는 사람들을 마을 입구로 워프시킨다.
	void WarpToVillageGate(SI32 villageunique);

	// 모든 사람들의 블럭을 재설정한다. 
	void InitBlock();

	// 사용자에게 아이템을 준다. 
	void GiveItem(SI16 charunique, SI16 itemunique, SI16 itemnum, SI16 itemgivemode, SI32 inputmode,SI32 usedate, SI32 senderID = 0, SI32 level = 0, SI32 VillageUnique = 0, SI32 EnchantType = 0, SI32 EnchantLevel = 0, SI32 SlotNum = 0 );

	void GiveItemForVillageMember(cltItem clitem , SI32 villageunique,SI32 inputmode = 0 );
	
	// 캐릭터 관련 통계 
	void GetStatistic(cltStatistics* pclstatistic);


	// 흑의군단 전쟁 전공을 소팅한다. 
	bool SortBlackWarRecord(SI32* pchar, SI32 num);
	// 전쟁 전공을 소팅한다. 
	bool SortWarRecord(SI32* pchar, SI32 num, SI32 warmapindex);

	
	// 제조 의뢰 내용을 하나 구한다. 
	bool GetPersonalShopMakeInfo(TCHAR* pname, SI16 txtSize, SI16* pitemunique, SI32* pitemnum, GMONEY* pfee, SI32* pskill, SI32* phand);

	
	cltCharCommon* GetChar(SI32 charid)
	{
		if(IsValidID(charid) == false)		return NULL;

		return CR[charid];
	}

	cltCharClient* GetCharClient(SI32 id);
	cltCharServer* GetCharServer(SI32 id);
};

// LUA Function
int GetCharIDFromName(lua_State* L);
int GetCharNameFromID(lua_State* L);
int MoveChar(lua_State* L);
int WarpChar(lua_State* L);
int GetCharPos(lua_State* L);
int GetCharOrder(lua_State* L);
int GetCharHaveItemNum(lua_State* L);
int GetCharEquipItemNum(lua_State* L);

#endif
