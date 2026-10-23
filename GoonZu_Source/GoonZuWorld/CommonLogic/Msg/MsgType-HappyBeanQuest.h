#ifndef _MSGTYPEHAPPYBEANQUEST_H_
#define _MSGTYPEHAPPYBEANQUEST_H_

//#include "../CommonLogic/Msg/MsgType-HappyBeanQuest.h"
//KHY - 0822 - 해피빈 퀘스트 추가.
class cltGameMsgRequest_HappyBeanQuestStart
{
public:
	SI32 siQuestType;
	SI32 siQuestState;

	cltGameMsgRequest_HappyBeanQuestStart( SI32 QuestType, SI32 QuestState )
	{
		siQuestType = QuestType;
		siQuestState = QuestState;
	}
};

class cltGameMsgResponse_HappyBeanQuestStart_NO
{
public:
	//SI32 siQuestType;  //나중에 필요하면 추가하자.
	SI32 siResult;

	cltGameMsgResponse_HappyBeanQuestStart_NO(SI32 Result )
	{
		//siQuestType = QuestType;
		siResult = Result;
	}
};


//-------------------------------------------------
// 해피빈 퀘스트가 설정되었음을 알린다. 
//-------------------------------------------------
class cltGameMsgResponse_HappyBeanQuestStart_OK
{
public:
	cltQuestInfoCommon	clHappyBeanQuestInfo;

	cltGameMsgResponse_HappyBeanQuestStart_OK(cltQuestInfoCommon*	pclHappyBeanQuestInfo)
	{
		clHappyBeanQuestInfo.Set(pclHappyBeanQuestInfo);
	}
};


//-------------------------------------------------
// 해피빈 퀘스트 포상 내용 통보 메시지. 
//-------------------------------------------------
class cltGameMsgResponse_HappyBeanQuestReward
{
public:
	cltQuestRewardInfo clQuestRewardInfo;

	cltGameMsgResponse_HappyBeanQuestReward(cltQuestRewardInfo* pclinfo)
	{
		clQuestRewardInfo.Set(pclinfo);
	}
};

//-------------------------------------------------
// //KHY - 0906 - 게임방 이벤트 쿠폰.
//-------------------------------------------------
class cltGameMsgResponse_EventCoupon
{
public:
	SI32 siEventCouponType;   //GAMEROOM_COUPON_EVENT =1
	SI32 siResult;

	cltGameMsgResponse_EventCoupon(SI32 eventcouponType, SI32 Result )
	{
		siEventCouponType = eventcouponType;
		siResult = Result;
	}
};
//옥션이벤트 [2007.09.10 손성웅]
//class cltGameMsgResponse_AUCTIONEventCoupon
//{
//public:
//	SI32 siEventCouponType;   //AUCTION_CUPON_EVENT =2
//	SI32 siResult;
//
//	cltGameMsgResponse_AUCTIONEventCoupon(SI32 eventcouponType, SI32 Result )
//	{
//		siEventCouponType = eventcouponType;
//		siResult = Result;
//	}
//};


//-------------------------------------------------
// class : cltGameMsgRequest_MissionQuestStart
// desc  : 미션 퀘스트 시작
//-------------------------------------------------
class cltGameMsgRequest_MissionQuest_Start
{
public:
	SI32 m_siMissionUnique;

	cltGameMsgRequest_MissionQuest_Start( SI32 siMissionUnique )
	{
		m_siMissionUnique = siMissionUnique;
	}
};


class cltGameMsgResponse_MissionQuest_Start
{
public:
	SI32					m_siMissionUnique;
	cltMissionQuestInfo		m_clMissionQuestInfo;

	cltGameMsgResponse_MissionQuest_Start( SI32 siMissionUnique, cltMissionQuestInfo* clMissionQuestInfo )
	{
		m_siMissionUnique	= siMissionUnique;
		m_clMissionQuestInfo.Set(clMissionQuestInfo);
	}
};


//-------------------------------------------------
// class : cltGameMsgRequest_MissionQuest_Accept
// desc  : 미션 퀘스트 수락
//-------------------------------------------------
class cltGameMsgRequest_MissionQuest_Accept
{
public:
	SI32 m_siMissionUnique;

	cltGameMsgRequest_MissionQuest_Accept( SI32 siMissionUnique )
	{
		m_siMissionUnique = siMissionUnique;
	}
};

//-------------------------------------------------
// class : cltGameMsgRequest_MissionQuest_Cancel
// desc  : 미션 퀘스트 취소
//-------------------------------------------------
class cltGameMsgRequest_MissionQuest_Cancel
{
public:
	SI32 m_siMissionUnique;

	cltGameMsgRequest_MissionQuest_Cancel( SI32 siMissionUnique )
	{
		m_siMissionUnique = siMissionUnique;
	}
};

//-------------------------------------------------
// class : cltGameMsgRequest_TreasureEvent_Npc_Kill
// desc  : 보물찾기 이벤트 npc 클릭 했다. 이놈을 죽여라!
//-------------------------------------------------
class cltGameMsgRequest_TreasureEvent_Npc_Kill
{
public:
	SI32 m_siNPC_CharUnique ;

	cltGameMsgRequest_TreasureEvent_Npc_Kill( SI32 siNPC_CharUnique )
	{
		m_siNPC_CharUnique = siNPC_CharUnique;
	}
};

// [지연] : 2010신년이벤트 - 받은 '흉'버프 지우기 퀘스트 거절 패킷 보낼때 쓸 메세지인포 - 파일 따로 빼야 하지만 메세지 인포가 1개 뿐이라 여기에 꼽사리
class cltGameMsgRequest_2010NewYearEvent_QuestCancel
{
public:
	SI32 m_siPersonID;

	cltGameMsgRequest_2010NewYearEvent_QuestCancel( SI32 siPersonID )
	{
		m_siPersonID = siPersonID;
	}
};

class cltGameMsgResponse_GoonzuDayMissionClearReward
{
public:
	SI32 m_siPersonID;
	SI32 m_siExp;

	SI32 m_siItemUnique;
	SI16 m_siItemNum;

	cltGameMsgResponse_GoonzuDayMissionClearReward( SI32 siPersonID, SI32 siExp, SI32 siItemUnique, SI16 siItemNum )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_GoonzuDayMissionClearReward) );

		m_siPersonID = siPersonID;
		m_siExp = siExp;
		m_siItemUnique = siItemUnique;
		m_siItemNum = siItemNum;
	}
};

#endif
