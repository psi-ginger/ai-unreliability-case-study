#ifndef _PARTYOBJ_H_
#define _PARTYOBJ_H_

#include "PartyBaseInfo.h"

#include "..\SMLib\LibList\Obj.h"
#include "../Common/CommonHeader.h"
#include "../CommonLogic/PartyQuestMgr/PartyQuestMgr_Common.h"

#define ITEM_GETPATTERN_NORMAL		0
#define ITEM_GETPATTERN_ROTATION	1
#define ITEM_GETPATTERN_FINISH		2
#define ITEM_GETPATTERN_RANDOM		3

class cltItem;

struct stPartyUser
{
	bool bPartyStatus;			// 파티에 참여한 유저냐, 아니면 빈 리스트의 유저이냐를 구분짓기 위해서...
	bool bIsPartyLeader;		// 파티장이냐, 파티원이냐
	SI32 siPersonID;			// PersonID
	SI32 siCharUnique;			// 캐릭터 unique 값	
	SI32 siExp;					// 경험치 획득량
	SI32 siLevel;				// 레벨
	
	bool bItemGetChance;				// 파티 플레이시 아이템을 획득할 기회가 남아있따는 플래그. 유효한 파티원이 전부 아이템을 한번씩 획득하면 초기화된다.
	//cltCharPos clCharPos;			// 위치 (맵인덱스, 맵좌표)
	
	stPartyUser()
	{
		Initialize();
	}

	void Initialize()
	{
		bPartyStatus = false;	// 파티에 참여중이지 않은 상태이므로...
		bIsPartyLeader = false;
		siPersonID = 0;
		siCharUnique = 0;
		siLevel = 0;
		siExp = 0;
		bItemGetChance = true;
	//	clCharPos.Init();
	}

	bool IsSamePersonID( SI32 PersonID )
	{
		if ( bPartyStatus )
		{
			if ( siPersonID == PersonID ) 
			{
				return true;
			}
		}
		
		return false;
	}

	void Join( SI32 PersonID, SI32 CharUnique, bool IsPartyLeader, SI32 Level )
	{
		bPartyStatus = true;	// 파티에 누군가(PersonID) 참여했음.
		bIsPartyLeader = IsPartyLeader;
		siPersonID = PersonID;
		siCharUnique = CharUnique;
		siLevel = Level;
	};

	void Set( stPartyUser *pUser )
	{
		memcpy( this, pUser, sizeof( stPartyUser ) );
	}
};

class CPartyObj : public CObj
{
public:
	CPartyObj( SI32 PartyObjIndex );
	~CPartyObj();
	
	void Initialize();
	void Destroy();	

	bool IsEmptyUserListExist();
	SI16 GetEmptyPartyUserListIndex();
	
	SI32 GetPartyIndex();	
	bool IsSamePartyIndex( SI32 PartyIndex );

	SI16 GetSamePersonIDIndex( SI32 PersonID );

	bool IsPartyLeader( SI32 PersonID );

	SI16 GetChangedLeaderIndex();

	SI32 CreateParty( SI32 PersonID, SI32 CharUnique, SI32 Level );
	SI16 JoinParty( SI32 PersonID, SI32 CharUnique, SI32 Level );
	SI16 LeaveParty( SI32 PersonID, SI32 *pRet );
	SI16 ForceLeaveParty( SI32 PersonID );

	stPartyUser *GetPartyUser( SI16 UserListIndex );

	SI16 GetPartyUserListIndex( SI32 CharUnique );
	SI32 GetCharUnique( SI32 PersonID );

	SI32 GetPartyLeaderUnique();

	SI16 GetPartyUserNum();
	
	void SetItemGetPattern( SI16 ItemGetPattern ){m_siItemGetPattern = ItemGetPattern;};
	SI16 GetItemGetPattern(){return m_siItemGetPattern;};
	
	SI32 GetItemGetCharUnique(SI32 * PartyList, SI16 arayMaxIndex);

	TCHAR *GetPartyName();

	bool SetPartyName( TCHAR *PartyName );
	 
	void SendPartyChat( SI32 SendCharUnique, TCHAR *Name, TCHAR *ChatData );

	void IncreaseExp( SI32 PersonID, SI32 Exp );
	void NotifyExp();
	void NotifyMovePosToPartyMember(SI32 MovedCharUnique,SI16 Xpos, SI16 Ypos,BOOL bWarpSwitch = TRUE ); // 캐릭터의 위치를 파티원에게 알린다.[ 영진 ]
	void NotifyGetIemToPartyMember(SI32 ItemGetCharUnique,cltItem * pclItem);
	void NotifyItemGetPatternToPartyMember(SI16 ItemGetPattern );
	void NotifyLevelUpToPatryMember(SI32 LevelUpCharUnique,SI32 Level);
private:
	SI32		m_siPartyIndex;								// 파티 각각에 대한 인덱스

	stPartyUser m_stPartyUserList[ MAX_PARTY_USER_NUM ];	// 파티 유저 리스트, 메모리 차지 안하므로 그냥 멤버로 넣었음.	
	bool		bIsPartyStaus;								// 파티중이냐, 파티중이 아니냐.

	SI16		m_siPartyUserNum;

	SI16		m_siItemGetPattern;							// [영진] 파티의 아이템 획득 방식
	//SI16		m_siItemGetPersonIndex;					// 순환방식에서 다음 아이템을 받아야할 퍼슨의 파티내  아이디

	TCHAR		m_strPartyName[ MAX_PARTY_NAME_LENGTH ];	// 파티 이름

public:
	// [종호] 파티퀘스트 정보
	PartyQuest_UnitData		m_PartyQuestInfo;	
	DWORD					m_dwStartTime;
	DWORD					m_dwElapseTime;
	DWORD					m_dwEndTime;
};

#endif