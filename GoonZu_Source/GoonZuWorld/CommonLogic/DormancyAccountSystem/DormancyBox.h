#pragma once

#include "../CommonLogic/CommonLogic.h"
#include "DormancyEquipItem.h"

class CDormancyEquipItem;
class CDormancyEquipItemMgr;

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.07.31
// 클래스명 : CDormancyBox
// 설명 : 휴면 계정 보급 상자 데이터 클래스
//================================================================================
class CDormancyBox
{
public:
	// 아이템 지급 가능 기간
	const static SI32 GIVESYSTEMDATE = 30;

// 맴버 변수
protected:
	// 아이템이 저장된 수
	SI32	m_siItemCount;
	// 아이템 유니크
	SI32	m_siUnique;

	// 휴면 계정 보급 상자에 들어가는 장비 아이템의 최대 수
	const static SI32 MAX_ITEM_COUNT = 6;	

	// 아이템 배열
#ifdef _SAFE_MEMORY
	NSafeTArray<CDormancyEquipItem *, MAX_ITEM_COUNT>		m_ItemArray;
#else
	CDormancyEquipItem	*m_ItemArray[MAX_ITEM_COUNT];
#endif

	 
// 맴버 메소드
public:
	CDormancyBox(void);
	~CDormancyBox(void);

	// 상자에 장비 아이템을 넣는다.
	BOOL	InsertItem(CDormancyEquipItem *pclDormancyEquipItem);

	// 상자에 들어 있는 아이템 수
	SI32	GetItemCount() const;

	VOID	SetUnique(SI32 siUnique);
	SI32	GetUnique() const;

	// 인덱스로 장비 아이템 인덱스를 얻는다.
	CDormancyEquipItem*	GetEquipItem(SI32 siIndex);
};
//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.07.31
// 클래스명 : CDormancyBoxMgr
// 설명 : 휴면 계정 보급 상자 관리 클래스
//================================================================================
class CDormancyBoxMgr
{
// 맴버 변수
public:
	
private:
	// 박스 수
	SI32	m_siCount;
	// 휴면 계정 장비 아이템 관리
	CDormancyEquipItemMgr *m_pclDormancyEquipItemMgr;

	// 휴면 계정 보급 상자 아이템의 최대 수
	const static SI32 MAX_ITEM_COUNT = 4;	

	// 아이템 배열
#ifdef _SAFE_MEMORY
	NSafeTArray<CDormancyBox, MAX_ITEM_COUNT>		m_ItemArray;
#else
	CDormancyBox	m_ItemArray[MAX_ITEM_COUNT];
#endif

// 맴버 메소드
public:
	CDormancyBoxMgr(CDormancyEquipItemMgr *pclDormancyEquipItemMgr);
	~CDormancyBoxMgr(void);


	// 아이템 유니크로 보급 상자가 존재 하는지 여부 
	BOOL	IsExist(SI32 siUnique);

	// 휴면 계정 보급 상자
	CDormancyBox *GetDormancyBox(SI32 siUnique);

	//	데이터 생성 - 파일 읽고 메모리에 담기
	BOOL	Create();

	// 박스 수
	SI32	GetCount();

	// 휴면 계정 보급 상자 (매니저의 인덱스로)
	CDormancyBox *GetDormancyBoxByIndex(SI32 siIndex);
protected:
	CDormancyBoxMgr(void);
};
