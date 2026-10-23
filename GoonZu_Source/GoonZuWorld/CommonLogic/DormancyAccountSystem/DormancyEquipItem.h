#pragma once

#include "../CommonLogic/CommonLogic.h"

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.07.30
// 클래스명 : CDormancyEquipItem
// 설명 : 휴면 계정 보상 장비 아이템 데이터 클래스
//================================================================================
class CDormancyEquipItem
{
// 맴버 변수
public:

protected:
	// 보상상자 유니크
	SI32	m_siBoxUnique;
	// 아이템 유니크
	SI32	m_siUnique;
	// 배열 크기
	const static SI32 MAX_ARRAY_SIZE  = MAX_CHARACTER_LEVEL+1;

	// 보상대상 아이템 유니크 ( 대상아이템이 업그래이드 될 레벨을 배열의 인덱스와 같게 하여 검색 속도를 줄인다.)
	// 추후 각 레벨에 들어갈 아이템에 대해서도 고려할 문제에 대해서도 해결됨.
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_ARRAY_SIZE>		m_TargetItemUniqueArray;
#else
	SI32 m_TargetItemUniqueArray[MAX_ARRAY_SIZE];
#endif

// 맴버 메소드
public:
	CDormancyEquipItem(void);
	~CDormancyEquipItem(void);

	// 업그레이드 될 타겟 장비 아이템의 유니크를 저장한다.
	BOOL	InsertTargetItem(SI32 siTargetUnique, SI32 siUpgradeLevel);
	// 업그레이드 될 타겟 장비 아이템의 유니크 값 리턴
	SI32	GetTargetItemUnique(SI32 siUpgradeLevel);


	VOID	SetBoxUnique(SI32 siBoxUnique);
	VOID	SetUnique(SI32 siUnique);

	const	SI32	GetBoxUnique() const;
	const	SI32	GetUnique() const;
};

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.07.30
// 클래스명 : CDormancyEquipItemMgr
// 설명 : 휴면 계정 보상 장비 아이템 관리 클래스
//================================================================================
class CDormancyEquipItemMgr
{
// 맴버 변수
public:
	
	// 휴면 계정 보상 장비 아이템의 최대 수
	const static SI32 MAX_ITEM_COUNT = 32;	
private:

	// 아이템 배열
	#ifdef _SAFE_MEMORY
		NSafeTArray<CDormancyEquipItem, MAX_ITEM_COUNT>		m_ItemArray;
	#else
		CDormancyEquipItem	m_ItemArray[MAX_ITEM_COUNT];
	#endif

// 맴버 메소드
public:
	CDormancyEquipItemMgr(void);
	~CDormancyEquipItemMgr(void);

	//	데이터 생성 - 파일 읽고 메모리에 담기
	BOOL	Create();

	// 아이템 유니크로 존재 하는지 여부 
	BOOL	IsExist(SI32 siUnique);

	// 휴면 계정 장비 아이템 
	CDormancyEquipItem *GetDormancyEquipItem(SI32 siUnique);
private:

	// 아이템 정보에 휴면 계정 아이템 인지 아닌지 부울값 입력
	BOOL	SetIsDormancyToItemInfo(SI32 siUnique);
};
