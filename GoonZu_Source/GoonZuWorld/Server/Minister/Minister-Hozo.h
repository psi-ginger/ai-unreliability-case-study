#ifndef _MINISTER_HOZO_H_
#define _MINISTER_HOZO_H_

#include "MinisterBase.h"

#define UNIT_HOZO_SETMONEY			100000

#define MAX_HOZO_FIXATIONMONEY		10
//cyj 정부가 수립되기 전까지 신규유저 정착금 0으로 설정 /[영진] 다시 취소
#define MIN_HOZO_FIXATIONMONEY		1
//#define MIN_HOZO_FIXATIONMONEY	0

#define MAX_HOZO_PRIZEMONEY			50
#define MIN_HOZO_PRIZEMONEY			1

#define MAX_HOZO_CHANGEDNAME_LIST	100

#define MAX_HOZO_ANGELLIST			50

// [영훈] 초보자 마을 : 주주배당금 설정
const SI32 Const_Min_Hozo_Dividend_Money = 1;
const SI32 Const_Max_Hozo_Dividend_Money = 3;

struct ChangedNameChar
{
	SI32	siDate;

	TCHAR	szOldName[MAX_PLAYER_NAME];
	TCHAR	szNewName[MAX_PLAYER_NAME];
};

struct HozoAngelList
{
	TCHAR	szAccountID[32] ;
	TCHAR	szName[MAX_PLAYER_NAME] ;
	SI32	siLevel ;
	SI32	siFameLevel ;

	SI32	siPersonID ;
};

class CMinisterHozo : public CMinisterBase
{
public:
	SI32	m_siResidentNum;	// 주민의 수
	SI32	m_siMaxShop;		// 마을내 총 상점수

	SI32	m_siTotalCharNum;	// 총 백성수
	SI32	m_siCalcTime;		// 총 백성수 계산한 시간
	
	GMONEY	m_siPrizeMoney;		// 추천인에게 주어야 할 포상금 ( 단위 10만 )
	SI32	m_siPrizeSetTime;	// 포상금 설정 시간
	GMONEY	m_siFixationMoney;

	GMONEY	m_siTutorialVillageDivideend;	// [영훈]초보자 마을 주주배당금

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_HOZO_ANGELLIST>		m_siAngel;
#else
	SI32	m_siAngel[MAX_HOZO_ANGELLIST];
#endif

#ifdef _SAFE_MEMORY
	NSafeTArray<HozoAngelList, MAX_HOZO_ANGELLIST>		m_stHozoAngelList;
	NSafeTArray<HozoAngelList, MAX_HOZO_ANGELLIST>		m_stHozoAngelApplyerList;
#else
	HozoAngelList	m_stHozoAngelList[MAX_HOZO_ANGELLIST]; // 수호천사들
	HozoAngelList	m_stHozoAngelApplyerList[MAX_HOZO_ANGELLIST]; // 신청자들
#endif
	SI32	m_siNewCharNum;

	// 파산 마을 관련
	SI16	m_siBankruptcyVillageNumber;
	SI32	m_siSetBankruptcyVillageDate;

	GMONEY	GetPrizeMoney()				{ return m_siPrizeMoney * UNIT_HOZO_SETMONEY; }
	GMONEY	GetFizationMoney()			{ return m_siFixationMoney * UNIT_HOZO_SETMONEY; }
	GMONEY	GetBeginnerVillageMoney()	{ return m_siTutorialVillageDivideend * UNIT_HOZO_SETMONEY; }
	
	SI32	GetMaxShopNum(){return m_siMaxShop;}
	
	bool IsValidAngelFromPersonID(SI32 personid);
	bool IsValidAngelFromCharID(SI32 charid);
	bool IsValidAngelFromCharName(const TCHAR * name);

	bool IsValidApplyerFromPersonID(SI32 personid);
	bool IsValidApplyerFromFromCharID(SI32 charid);
	bool IsValidApplyerFromFromCharName(const TCHAR * name);

	SI32 GetAngelNumber();
	
	CMinisterHozo();
	~CMinisterHozo();

	void 	InitializeDB();
	void 	DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket);

	void 	DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID);

	void 	DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID);

	void 	DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket);

	void	DoMsg_DBMsgResponse_GetDailyNewChar(sPacketHeader* pPacket);

	// Server&Client
	void	DoMsg_GameMsgRequest_GetAngelApplyerList(cltMsg* pclMsg, SI32 siID); // 수호천사 신청자 목록 요청
	void	DoMsg_GameMsgRequest_GetAngelList(cltMsg* pclMsg, SI32 siID); // 수호천사 목록 요청 

	void	DoMsg_GameMsgRequest_SetAngelApplyer(cltMsg* pclMsg, SI32 siID); // 수호천사 등록 신청
	void	DoMsg_GameMsgRequest_SetAngel(cltMsg* pclMsg, SI32 siID); // 수호천사 임명 및 해임

	// Server&DB
	void	DoMsg_DBMsgResponse_GetAngelApplyerList(sPacketHeader* pPacket); // 수호천사 신청자 목록 요청 응답
	void	DoMsg_DBMsgResponse_GetAngelList(sPacketHeader* pPacket); // 수호천사 목록 요청 응답

	void	DoMsg_DBMsgResponse_SetAngelApplyer(sPacketHeader* pPacket); // 수호천사 등록 신청 응답
	void	DoMsg_DBMsgResponse_SetAngel(sPacketHeader* pPacket); // 수호천사 임명 및 해임 응답



};

#endif