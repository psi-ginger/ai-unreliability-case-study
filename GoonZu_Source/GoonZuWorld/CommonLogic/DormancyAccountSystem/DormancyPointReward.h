#pragma once

#include "../CommonLogic/CommonLogic.h"

#include "DormancyPointItem.h"
#include "DormancyPointProcess.h"

class CDormancyPointItemMgr;
class CDormancyPointProcess;
class cltClient;
class cltServer;

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.4
// 클래스명 : CDormancyPointReward
// 설명 : 휴면 계정 시스템의 포인트 보상 관련 컨트롤러 클래스
//================================================================================
class CDormancyPointReward
{
	// 맴버 변수
public:
	// 포인트 아이템 관리 클래스
	CDormancyPointItemMgr*	m_pclDormancyPointItemMgr;
	// 휴면 계정 기간
	const static SI32 DURATIONDATE = 30;
	// 휴면 계정 만료 경고 플레그 값
	const static SI32 WARING_EXPIRATION_FLAG = 5;

private:
	// 클라이언트 객체
	cltCommonLogic*	m_pclCommon;

	// 맴버 메소드
public:
	// 생성자
	CDormancyPointReward();
	// 소멸자
	virtual ~CDormancyPointReward(void);

	//생성
	BOOL		Create();

	//포인트 아이템 정보를 매니저의 인덱스로 알아낸다.
	cltItemInfo	*GetItemInfo(SI32 siIndex);
	//포인트 아이템 유니크를 매니저의 인덱스로 알아낸다.
	SI32		GetItemUnique(SI32 siIndex);
	//아이템의 휴면 포인트를 매니저의 인덱스로 알아낸다.
	SI32		GetItemPoint(SI32 siIndex);

	//포인트 아이템 종류 갯수
	SI32		GetItemCount();
};


//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.4
// 클래스명 : CDormancyPointRewardClient
// 설명 : 휴면 계정 시스템의 포인트 보상 관련 컨트롤러 클래스(클라이언트용)
//================================================================================
class CDormancyPointRewardClient : public CDormancyPointReward
{
	// 맴버 변수
public:

private:
	// 클라이언트 객체
	cltClient*	m_pclClient;

	// 맴버 메소드
public:
	//	생성자
	CDormancyPointRewardClient(void);
	// 소멸자
	virtual ~CDormancyPointRewardClient(void);

	// 휴면계정상태 일 경우 휴면 포인트 다이얼로그를 생성한다.
	BOOL	CreatePointDlg();

	// 포인트 아이템 구입 게임서버로 요청
	BOOL	BuyPointItemRequest(SI32 siItemUnique);
	
	// 포인트 아이템 구입 게임서버로 부터 응답
	//	BuyPointItemResponse(cltCharServer *pclChar, SI32 siItemUnique);

	// 휴면 포인트 정보 수신
	BOOL	RecvDormancyInfo(cltCharClient*	pclChar, SI32 siPoint, SI32 siReaminedPlayTime, BOOL bDormancyState, SI16 siStartedYear, SI16 siStartedMonth, SI16 m_siStartedDay);
};

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.2
// 클래스명 : CDormancyPointRewardServer
// 설명 : 휴면 계정 시스템의 포인트 보상 관련 컨트롤러 클래스(서버용)
//================================================================================
class CDormancyPointRewardServer : public CDormancyPointReward
{
	// 맴버 변수
public:

	// 맴버 메소드
private:
	CDormancyPointProcess m_PointProcess;
	// 클라이언트 객체
	cltServer*	m_pclServer;
public:
	//	생성자
	CDormancyPointRewardServer(void);
	// 소멸자
	virtual ~CDormancyPointRewardServer(void);

	// 휴면 계정 상태 변경하기 요청
	BOOL	SendDBChangeDormancyState(cltCharServer *pclServer, bool bDormancyState);

	// 휴면 계정 상태 변경하기 응답
	BOOL	RecvDBChangeDormancyState(cltCharServer *pclServer, bool bDormancyState);

	// 포인트 계산
	BOOL	CalcPoint(cltCharServer *pclServer);

	// 처리 할지 여부 결정
	BOOL	DeterminProcessing(SI32	siCurrentime);
	
	// 포인트 아이템 선택
	BOOL	SelectPointItem(cltCharServer *pclChar, SI32 siItemUnique);

	// 포인트 아이템 구입 DB로 요청
	BOOL	BuyPointItemRequest(SI32 siCharId , cltCharServer *pclChar, SI32 siItemUnique);
	// 포인트 아이템 구입 DB로 부터 응답
	BOOL	BuyPointItemResponse(cltCharServer*	pclChar, cltItem *pclItem, SI32 siItempos, SI32 siItemPoint);

	// 휴면 포인트 정보 전송
	BOOL	SendClientDormancyInfo(cltCharServer*	pclChar);

	// DB로 휴면 포인트 정보 수신
	BOOL	RecvDBDormancyInfo(cltCharServer*	pclChar, SI32 siPoint);
	// DB로 휴면 포인트 정보 전송
	BOOL	SendDBDormancyInfo(cltCharServer*	pclChar, SI32 siDPoint);

	// 기간이 만료 되었는지
	BOOL	IsExpiration(cltCharServer *pclServer, _SYSTEMTIME &rclCurrentDate);

	// 휴면 계정 종료를 경고 한다.
	BOOL	WaringExpiration(cltCharServer *pclServer);
	// 휴면 계정 상태를 유지한 기간
	SI32	GetDateFromExpiration(cltCharServer *pclServer, _SYSTEMTIME &rclCurrentDate);
};