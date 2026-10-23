#ifndef _PARTYMGR_H_
#define _PARTYMGR_H_

#include "..\SMLib\LibList\LibListMgr.h"

#include "PartyBaseInfo.h"

class CPartyObj;
class CPartyMgr : public CLibListMgr
{
public:
	CPartyMgr();
	~CPartyMgr();

	void Initialize();
	void Destroy();

	void CreateMaxEmptyObj();

	void Action();

	SI32 CreateParty( SI32 PersonID, SI32 CharUnique, SI32 Level );			// 파티 번호 리턴	
	//SI16 JoinParty( SI32 FromPersonID, SI32 CharUnique, SI32 PartyIndex );			// 파티 조인된 위치 리턴( m_stPartyUserList[ index ], index 값을 리턴 )
	SI16 LeaveParty( SI32 PersonID, SI32 PartyIndex, SI32 *pRet );					// 파티에서 빠진 위치 리턴( m_stPartyUserList[ index ], index 값을 리턴 )
	//SI16 ForceLeaveParty( SI32 FromPersonID, SI32 ToPersonID, SI32 PartyIndex );	// 파티장이 파티원을 강제 퇴장 시킴


	CPartyObj *GetPartyObj( SI32 PartyIndex );

	// 파티번호에 파티장인지 확인
	bool IsPartyLeader( SI32 FromPersonID, SI32 PartyIndex );

	// 파티번호에 파티 맴버인지 확인
	bool IsPartyMember( SI32 siPersionID, SI32 siPartyIndex );


	// 파티에 유저를 더 추가할수 있는지 확인
	bool IsEmptyUserListExist( SI32 PartyIndex );

	void DeleteParty( CPartyObj *pObj );

	UI32 GetPartyNum();
	UI32 GetPartyTotalUserNum();

	void IncreaseExp( SI32 PartyIndex, SI32 PersonID, SI32 Exp );

	// [영훈] 해적선 : 파티인던에서 파티가 해제되었을 경우 엔펠리스로 워프시킨다
	void InPartyDungeonDeleteParty( SI32 siCharID );

	// [영훈] siPartyIndex맴버중 siClearCondition 퀘스트를 하고있는 유저의 수를 센다
	SI32 CountQuestPalyUserNum( SI32 siPartyIndex, SI32 siClearCondition );

private:

};


#endif