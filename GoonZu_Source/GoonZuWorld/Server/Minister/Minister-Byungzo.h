#ifndef _MINISTER_BYUNGZO_H_
#define _MINISTER_BYUNGZO_H_

// by LEEKH 2008-04-03 : 흑의군단 개최가능 회수 10회로 변경
#define MAX_SETWARCOUNT	10

class CMinisterByungzo : public CMinisterBase
{
public:
	SI32				m_siPersonID;

	SI32				m_siSetTime;				// 전쟁설정시간
	SI32				m_siWarStartTime;			// 전쟁시작시간
	SI16				m_siSetWarCount;			// 전쟁설정 횟수
	UI32				m_siBlackArmyNum;			// 전쟁 몬스터 설정 수

	SI32				m_siVillageWarTime;	// 공성시간 분 
	
	CMinisterByungzo();
	~CMinisterByungzo();


	void 	InitializeDB();
	void 	DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket);

	void 	DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID);

	void 	DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID);

	void 	DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket);
	
};

#endif