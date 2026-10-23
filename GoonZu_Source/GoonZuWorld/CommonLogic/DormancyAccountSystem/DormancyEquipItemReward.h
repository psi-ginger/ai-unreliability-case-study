#pragma once

//#include "../../Client/Client.h"
#include "../CommonLogic/CommonLogic.h"

#include "DormancyEquipItem.h"
#include "DormancyBox.h"

class CDormancyBoxMgr;
class CDormancyEquipItemMgr;
class cltClient;
class cltServer;

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.1
// 클래스명 : CDormancyEquipItemReward
// 설명 : 휴면 계정 시스템의 장비 보상 관련 컨트롤러 클래스
//================================================================================
class CDormancyEquipItemReward
{
// 맴버 변수
public:
	// 보상 장비 아이템 관리 클래스
	CDormancyEquipItemMgr*	m_pclDormancyEquipItemMgr;
	// 보급 상자 관리 클래스
	CDormancyBoxMgr*	m_pclDormancyBoxMgr;

private:
	// 클라이언트 객체
	cltCommonLogic*	m_pclCommon;

// 맴버 메소드
public:
	// 생성자
	CDormancyEquipItemReward();
	// 소멸자
	virtual ~CDormancyEquipItemReward(void);

	//생성
	BOOL		Create();

	// 휴면 장비 아이템인지 확인한다.
	BOOL		IsDormancyEquipItem(SI32 siItemUnique);

	//보급 상자에 들어 있는 아이템을 인덱스 순서 대로 본다.
	cltItemInfo	*GetItemInfoInBox(SI32 siBoxUnique, SI32 siIndex);

	//보급 상자에 들어 있는 아이템을 인덱스 순서 대로 본다.
	SI32		GetItemUniqueInBox(SI32 siBoxUnique, SI32 siIndex);

	//장비 아이템 변화하기(인벤토리에 있는 아이템을 모두 검색 해서)
	BOOL	UpgradeItemInven(cltCharCommon *pclChar);
	//장비 아이템 변화하기(특정 아이템)
	BOOL	UpgradeItemInfo(cltCharCommon *pclChar,cltItem *pclItem);
	//장비 아이템 변화하기(인벤토리에 있는 특정 아이템)
	virtual BOOL	UpgradeItem(cltCharCommon *pclChar, SI32 siItemUnique, SI32 siItemPos, SI32 siType);
};


//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.2
// 클래스명 : CDormancyEquipItemRewardClient
// 설명 : 휴면 계정 시스템의 장비 보상 관련 컨트롤러 클래스(클라이언트용)
//================================================================================
class CDormancyEquipItemRewardClient : public CDormancyEquipItemReward
{
// 맴버 변수
public:

private:
	// 클라이언트 객체
	cltClient*	m_pclClient;

// 맴버 메소드
public:
	// 생성자
	CDormancyEquipItemRewardClient();
	// 소멸자
	virtual ~CDormancyEquipItemRewardClient(void);

	//	아이템 선택
	BOOL	SelectItemInBox(SI32 siItemIndex,SI32 siBoxUnique, SI32 siBoxPos);

	//박스 사용
	BOOL	UseBox(SI32 siItemUnique, SI32 siPos);
};

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.2
// 클래스명 : CDormancyEquipItemRewardServer
// 설명 : 휴면 계정 시스템의 장비 보상 관련 컨트롤러 클래스(서버용)
//================================================================================
class CDormancyEquipItemRewardServer : public CDormancyEquipItemReward
{
// 맴버 변수
public:

private:
	// 서버 객체
	cltServer*	m_pclServer;

// 맴버 메소드
public:
	// 생성자
	CDormancyEquipItemRewardServer();
	// 소멸자
	virtual ~CDormancyEquipItemRewardServer(void);

	//장비 아이템 지급
	BOOL	GiveItemToPlayer(SI32 siBoxUnique, SI32 siItemUnique, SI32 siIBoxPos,cltCharServer *pclChar, SI32 siItemIndex);	

	//시스템 보상 : 장비 보급 상자 2종 주기
	BOOL	GiveEquipBox(cltCharServer *pclCharServer);

	//장비 아이템 변화하기(인벤토리에 있는 특정 아이템)
	virtual BOOL	UpgradeItem(cltCharCommon *pclChar, SI32 siItemUnique, SI32 siItemPos, SI32 siType);
};