// 파티매칭
#ifndef _CLIENT_PARTYMATCHING_MANAGER_H_
#define _CLIENT_PARTYMATCHING_MANAGER_H_

#include "../../Common/CommonHeader.h"
#include "Info_PartyMatching.h"
#include "../../CommonLogic/Msg/MsgType-PartyMatching.h"






//////////////////////////////////////////////////////////////////////////
// 클라이언트 파티 매칭 매니저
//////////////////////////////////////////////////////////////////////////


class CClient_PartyMatchingMgr
{
public:
	CClient_PartyMatchingMgr();
	~CClient_PartyMatchingMgr();

public:
	NkVector<CInfo_PartyMatching>	m_PartyList;

public:
	SI32 m_siSelectedCategory;
	SI32 m_siSelectedMission;



	//-------------------------------------------------------------
	// 최초 npc 클릭했을때 보이는 창 처리 부
	//-------------------------------------------------------------
public:
	void Click_PartyMatchingNPC();		
	void Click_DestroyDlgButton();
	void Click_CategoryButton(SI16 siIndex);



	//-------------------------------------------------------------
	// 레벨별 퀘스트 리스트 버튼 클릭했을때 보이게 될 창 처리부
	//-------------------------------------------------------------
public:
	void Click_PartyEnterRequestButton(SI16 siSelectedIndex);
	void Click_WhisperButton(SI16 siSelectedIndex);

	void Click_PartyMakeButton(SI16 siTmpCate);

	

	//-------------------------------------------------------------
	// 파티매칭 새로 생성하기 눌렀을때 보이게 될 창 처리부
	//-------------------------------------------------------------
public:
	void Click_PartyMakeButton2();					// 버튼 클릭시 대화상자의 각 컨트롤로부터 파티매칭 등록에 필요한 정보들을 읽어 적절하면 서버로 보내서 생성하도록 한다.



	// 일반 인터페이스
public:
	bool Get_Info_PartyMemberInfo(CInfo_PartyMatching& rInfo);

	void Set_Category(SI32 siCategory);
	void Set_Mission(SI32 siMission);
	SI32 Get_Category();
	SI32 Get_Mission();

	void Action();

	CInfo_PartyMatching* Get_PartyMatchingInfo(SI16 siSelectedIndex);
	SI32 AlreadyExist(SI32 siMyCharUnique);

	// 대화상자가 아닌 일반액션에 대한 인터페이스들 (주로 서버로 부터 날라온 패킷에 대한 처리가 주를 이룸)
public:
	// request ( 클라 -> 서버 )
	void MsgProcess_Request_CreatePartyMatching(CInfo_PartyMatching& rInfo);
	void MsgProcess_Request_JoinPartyMatching(SI16 siSelectedIndex);
	void MsgProcess_Request_PartyMatchingList(SI16 siCategory);

	// response ( 서버 -> 클라 )
	void MsgProcess_Response_PartyMatchingLevel(SI32 siCharID, cltGameResponse_PartyMatchingLevel* pInfo);
	void MsgProcess_Response_PartyMatchingCreate(SI32 siCharID, bool bResult);
	void MsgProcess_Response_PartyMatchingJoin();
};

#endif				//_CLIENT_PARTYMATCHING_MANAGER_H_