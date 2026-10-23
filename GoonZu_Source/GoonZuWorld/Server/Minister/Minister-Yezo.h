#ifndef _MINISTER_YEZO_H_
#define _MINISTER_YEZO_H_

#define MAX_YEZO_ANGELLIST		50

struct YezoAngelList
{
	TCHAR	szAccountID[32] ;
	TCHAR	szName[MAX_PLAYER_NAME] ;
	SI32	siLevel ;
	SI32	siFameLevel ;

	SI32	siPersonID ;
};

class CMinisterYezo: public CMinisterBase
{
public:
	SI32		m_siJapanSwordSolNum;	// 이도왜구 설정 수	KIND_JAPANSWORDSOL
	SI32		m_siJapanGunSolNum;		// 조총왜구 설정 수	KIND_JAPANGUNSOL
	SI32		m_siJapanCommanderNum;	// 왜구장수 설정 수 KIND_JAPANCOMMANDER
	SI32		m_siJapanPorterNum;		// 짐꾼왜구 설정 수 KIND_JAPANPORTERSOL

	SI32		m_siWarVillageUnique;		// 짐꾼왜구가 쳐들어갈 마을 유니크

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_YEZO_ANGELLIST>	m_siAngel;
#else
	SI32		m_siAngel[MAX_YEZO_ANGELLIST];
#endif

#ifdef _SAFE_MEMORY
	NSafeTArray<YezoAngelList, MAX_YEZO_ANGELLIST>	m_stYezoAngelList;
	NSafeTArray<YezoAngelList, MAX_YEZO_ANGELLIST>	m_stYezoAngelApplyerList;
#else
	YezoAngelList	m_stYezoAngelList[MAX_YEZO_ANGELLIST]; // 수호천사들
	YezoAngelList	m_stYezoAngelApplyerList[MAX_YEZO_ANGELLIST]; // 신청자들
#endif

	bool IsValidAngelFromPersonID(SI32 personid);
	bool IsValidAngelFromCharID(SI32 charid);
	bool IsValidAngelFromCharName(const TCHAR * name);

	bool IsValidApplyerFromPersonID(SI32 personid);
	bool IsValidApplyerFromFromCharID(SI32 charid);
	bool IsValidApplyerFromFromCharName(const TCHAR * name);

	SI32 GetAngelNumber();

	CMinisterYezo();
	~CMinisterYezo();


	void 	InitializeDB();
	void 	DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket);
	
	void 	DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID);

	void 	DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID);
	
	void 	DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket);

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