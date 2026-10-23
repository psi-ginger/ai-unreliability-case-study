#pragma once

#include "../CommonLogic/CommonLogic.h"

namespace REWARDTYPE
{
	enum ENUM 
	{
		START = 0,
		//==================================================================
		BATTLEROYAL_WINNER,				// 배틀로얄 - 우승자
		BATTLEROYAL_KILLPLAYER,			// 배틀로얄 - 플레이어 죽이기
		//==================================================================
		END,
		COUNT = END -1
	};
}

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.09.28
// 클래스명 : CRewardItemByProb
// 설명 : 확률 보상 아이템 
//================================================================================
class CRewardItemByProb
{	 
	// 맴버 메소드
public:
	CRewardItemByProb()
	{
		m_uiUnique = 0;
		m_uiProb = 0;
		m_uiNum = 0;
		m_uiMinNum = 0;
	}
	~CRewardItemByProb()
	{};

	void	SetMinNum(UI32 uiMinNum) { m_uiMinNum = uiMinNum; }
	void	SetNum(UI32 uiNum) { m_uiNum = uiNum; }
	void	SetProb(UI32 uiProb) { m_uiProb = uiProb; }
	void	SetUnique(UI32 uiUnique) { m_uiUnique = uiUnique; }

	UI32	GetMinNum() { return m_uiMinNum; }
	UI32	GetNum() { return m_uiNum; }
	UI32	GetProb() { return m_uiProb; }
	UI32	GetUnique() { return m_uiUnique; }

protected:
	UI32	m_uiUnique;				// 유니크
	UI32	m_uiProb;				// 확률
	UI32	m_uiNum;				// 개수
	UI32	m_uiMinNum;				// 최소 개수
};

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.09.28
// 클래스명 : CRewardTypeItemByProbMgr
// 설명 : 확률 보상 아이템 관리자 
//================================================================================
class CRewardTypeItemByProbMgr
{	 
// 맴버 메소드
public:
	CRewardTypeItemByProbMgr();
	~CRewardTypeItemByProbMgr();
	
	BOOL	Create(UI32 uiRewardType, UI32 uiCount);	// 생성
	BOOL	Destroy();													// 제거
	BOOL	GetRandItemUnique(UI32* puiUnique,UI32* puiNum);			// 아이템 보상
	UI32	GetType();
	BOOL	AnalyzeItemInfo(TCHAR* pszData);

	const static SI32	INIT_ITEMCOUNT = 20;
protected:
	const static SI32	MAX_RANDVALUE = 10000;
		
	UI32							m_uiRewardType;		// 보상 종류
	UI32							m_uiItemCount;		// 아이템 수
	NkVector<CRewardItemByProb>*	m_pVecRewardItem;	// 보상 아이템 리스트
};

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.10.14
// 클래스명 : CRewardItemByProbMgr
// 설명 : 확률 보상 타입별 아이템 리스트 관리자 
//================================================================================
class CRewardItemByProbMgr
{	 
	// 맴버 메소드
public:
	CRewardItemByProbMgr();
	~CRewardItemByProbMgr();

	BOOL	Create();			// 생성
	BOOL	Destroy();			// 제거
	CRewardTypeItemByProbMgr*	GetRewardTypeItemByProbMgr(UI32 uiRewardType);

protected:
	// 파일 데이터 로드
	BOOL	LoadFileData();

	NkVector<CRewardTypeItemByProbMgr>	m_pVecRewardItemMgr;	// 보상 타입 아이템 매니저 리스트
	
};

