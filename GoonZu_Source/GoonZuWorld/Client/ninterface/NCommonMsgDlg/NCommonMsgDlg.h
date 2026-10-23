//#ifndef _NMSGBOXDLG_H_
//#define _NMSGBOXDLG_H_

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/DialogController.h"

class CEdit;
class CButton;

#define COMMONDLG_NONE			0
#define COMMONDLG_REVIVE		1
#define COMMONDLG_RELOGIN		2
#define COMMONDLG_EXITCOUNT		3
#define COMMONDLG_CLOSE_PREMIUMSHOP		4
#define COMMONDLG_RESURRECTION			5	
#define COMMONDLG_INITLEVEL				6
#define COMMONDLG_MOFUMOFU_ENTER_FIELD1	7
#define COMMONDLG_MOFUMOFU_ENTER_FIELD2	8
#define COMMONDLG_MOFUMOFU_ENTER_FIELD3	9
#define COMMONDLG_MOFUMOFU_LEAVE_FIELD1	10
#define COMMONDLG_MOFUMOFU_LEAVE_FIELD2	11
#define COMMONDLG_MOFUMOFU_LEAVE_FIELD3	12
#define COMMONDLG_MOFUMOFU_AUTO_WARP	13
#define COMMONDLG_REQUEST_SETFATHER		14
#define COMMONDLG_DAILYQUEST_REWARD		15
#define COMMONDLG_CANNOTLOGIN_AGE		16
#define COMMONDLG_TICKETMONEY_WARP			17		//KHY - 0703 - 게임 머니로 귀환하기.
#define COMMONDLG_EVENTCOUPON				18		//KHY - 0906 - 게임방 이벤트 쿠폰.
#define COMMONDLG_AUCTIONEVENTCOUPON		19		//옥션이벤트 [2007.09.10 손성웅]
#define COMMONDLG_GOSTOPPOCKERCOMPLETE		20		//고스톱,포커이벤트
#define COMMONDLG_GOSTOPPOCKERSTART			21
#define COMMONDLG_CANNOTLOGIN_ERROR			22
#define COMMONDLG_HANGAME_VIP_EVENT			23		//KHY - 1024 - 한게임 VIP 이벤트
#define COMMONDLG_HANGAME_VIP_EVENT_ITEM	24		//KHY - 1024 - 한게임 VIP 이벤트
#define COMMONDLG_NOWCOM_EVENT_COUPON		25		//KHY - 1025 - 나우콤 이벤트
#define COMMONDLG_NOWCOM_EVENT_10LEVEL		26		//KHY - 1025 - 나우콤 이벤트
#define COMMONDLG_NOWCOM_EVENT_HAVEFATHER	27		//KHY - 1025 - 나우콤 이벤트
#define COMMONDLG_INVITEEMPIRE				28		// 광명 - 제국초대창
#define COMMONDLG_DAILYQUEST2_REWARD		29
#define COMMONDLG_DAILYQUEST3_REWARD		30
#define COMMONDLG_HALLOWEENMISSION_START	31
#define COMMONDLG_REQUEST_PVP				32
#define COMMONDLG_RESURRECTION_PVP			33
#define COMMONDLG_REQUEST_GUILDHUNTMAPWAR_WARP 34	// 길드맵을 차지하기 위한 길드전에 참여하기위해 워프 할것인가
#define COMMONDLG_REQUEST_TREASUREEVENT 35	// 보물찾기 이벤트 - 퀘스트를 다 했으면 메리엘에게 가서 오카라 10개를 주고 버프를 받는다
#define COMMONDLG_REQUEST_FAMILY_ADD_CHILDREN 36	// 가족 시스템 - 가족 추가
#define COMMONDLG_REQUEST_FAMILY_DELL_CHILDREN 37	// 가족 시스템 - 가족 탈퇴
#define COMMONDLG_REQUEST_FAMILY_REMOVE_TARGET_CHILDREN 38 // 가족시스템 - 가족에서 제외
#define COMMONDLG_REQUEST_FAMILY_PLUS_FAMILYEFFECTDATE 39 // 가족시스템 - 가족효과 연장
#define COMMONDLG_REQUEST_RACOONDODGEEVENT_ANSWER	40	// 라쿤 피하기 이벤트 참여할꺼냐는 물음에 대한 답변//Switch_10MonthEvent_RacoonDodge
#define COMMONDLG_BBB_WAIT_QUESTION			41		// [지연] 리스트에 등록하겠냐는 대화상자
#define COMMONDLG_BBB_DELETE_WAITLIST		42		// [지연] 대기자 리스트로부터 삭제 확인 대화상자
#define COMMONDLG_BBB_WARPMAPQUESTION		43		// [지연] 1순위 대기자에게 뜨는 "워프할래요?" 대화상자
#define COMMONDLG_BBB_NOTENOUGHMONEY		44		// [지연] 소지금이 게임 참가비보다 적다고 알려주는 대화상자
#define COMMONDLG_2010NEWYEARFORTUNE_BUF	45		// [지연] 2010신년이벤트 - 받은 버프 알려주는 대화상자
#define COMMONDLG_2010NEWYEARFORTUNE_QUEST	46		// [지연] 2010신년이벤트 - 받은 퀘스트 알려주는 대화상자
#define COMMONDLG_INSURANCE_POLICY			47		// [성웅]	보험증서



//---------------------------------------------------

#define LOGOUT_DELAYTIME		20

class CNCommonMsgDlg : public CDialogController
{
public:
	CNCommonMsgDlg();
	~CNCommonMsgDlg();

	void Create();
	void Set(SI32 CommonDlg_mode);
	void Action();
	void LogOut();

	SI32 GetMode();
	void SetFather( SI32 PupilPersonID, TCHAR* PupilName );
	void SetInviteEmpire( SI08 InviteType, SI32 EmpireUnique, SI32 FromPersonID, SI32 EmproerVillageUnique );

	void SetText(TCHAR* pText);

	//KHY - 0703 - 게임 머니로 귀환하기.
	void SetWarpMoney(SI32 MapIndex,SI32 Money );
	// PVP 신청한 사람 ID 를 센팅 
	void SetPVPRequrstID( SI32 PVPRequestId );

	// 창이 보이는 시간을 설정한다. 함수 호출시각 이후부터 시간을 잰다.
	void SetShowTimeSec(SI32 siShowTimeSec);

	static void CALLBACK StaticCallBackDialogNCommonMsgDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNCommonMsgDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetGuildMapWar_PersonID( SI32 GuildMapWar_PersonID );

	void SetParentPersonID( SI32 siParentPersonID  )	;
	
private:
	SI32 siCommonDlg_mode;
	SI32 siExitCount;
	SI32 siPupilPersonID;
	TCHAR szPupilName[20];
	//KHY - 0703 - 게임 머니로 귀환하기.
	SI64 siWarpMoney;
	SI32 szMapIndex;

	/// 제국으로 초대하는 종류 
	SI08	siInviteType;
	/// 초대메시지를 보낸 제국 유니크
	SI32	siEmpireUnique;
	/// 초대메시지를 보낸 PersonID
	SI32	siFromPersonID;
	/// 초대메시지를 보내는 제국 마을 이름
	SI32	siEmproerVillageUnique;
	// PVP 신청한 사람 ID
	SI32	siPVPRequestID	;
	CEdit	*m_pEdit;
	CButton	*m_pOKButton;
	CButton	*m_pCancelButton;

	DWORD	m_dwStartTime;		// 창이 보여지는 시작 시각
	DWORD	m_dwShowTimeMS;		// 창이 보여지는 시간 (ms)

	// 길드맵 길드전에 대한 requset을 수락한 PersoID
	SI32	siGuildMapWar_PersonID	;
	// 자식 수락을 위한 부모 personID
	SI32	m_siParentPersonID	;
};

//#endif
