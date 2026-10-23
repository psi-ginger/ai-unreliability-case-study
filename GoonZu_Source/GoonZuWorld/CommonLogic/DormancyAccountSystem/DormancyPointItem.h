#pragma once

#include "../CommonLogic/CommonLogic.h"

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.18
// 클래스명 : CDormancyPointItem
// 설명 : 휴면 계정 포인트 아이템 데이터 클래스
//================================================================================
class CDormancyPointItem
{
	// 맴버 변수
public:

protected:
	// 포인트 점수
	SI32	m_siPoint;
	// 아이템 유니크
	SI32	m_siUnique;
	
	// 맴버 메소드
public:
	CDormancyPointItem(void);
	~CDormancyPointItem(void);

	VOID	SetUnique(SI32 siUnique);
	VOID	SetPoint(SI32 siPoint);

	SI32	GetPoint() const;
	SI32	GetUnique() const;
};

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.2
// 클래스명 : CDormancyPointItemMgr
// 설명 : 휴면 계정 시스템의 포인트 아이템 관리 클래스
//================================================================================

class CDormancyPointItemMgr
{
//맴버 변수
public:
	// 휴면 계정 보상 장비 아이템의 최대 수
	const static SI32 MAX_ITEM_COUNT = 16;	

private:
	// 아이템 수
	SI32	m_siCount;

	// 아이템 배열
	#ifdef _SAFE_MEMORY
		NSafeTArray<CDormancyPointItem, MAX_ITEM_COUNT>		m_ItemArray;
	#else
		CDormancyPointItem	m_ItemArray[MAX_ITEM_COUNT];
	#endif

//맴버 메소드
public:
	CDormancyPointItemMgr(void);
	~CDormancyPointItemMgr(void);

	//	데이터 생성 - 파일 읽고 메모리에 담기
	BOOL	Create();

	// 아이템 유니크로 존재 하는지 여부 
	BOOL	IsExist(SI32 siUnique);

	// 휴면 계정 장비 아이템 
	CDormancyPointItem *GetDormancyPointItem(SI32 siUnique);
	// 인덱스로 아이템 정보 얻기
	CDormancyPointItem *GetDormancyPointItemFromIndex(SI32 siIndex);
	
	// 포인트 아이템 종류 갯수
	SI32	GetCount();
};
