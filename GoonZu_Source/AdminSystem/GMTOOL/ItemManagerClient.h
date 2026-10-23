#pragma once

#include "../common/item/ItemCommon/ItemUnit.h"
#include "../../NGameDataManager/NGameDataManager.h"
#include "../../NLib/NDataLoader.h"

#define MAX_ITEMINFO_NUMBER		30000

class cltItemManagerClient{
private:
	NGameDataManager*	m_pclGameDataManager;
public :

	int m_siItemType[MAX_ITEMINFO_NUMBER];

	// 클라이언트에서만 필요한 부분. 
//	SI32 CharUnique;
//
	cltItemManagerClient();
	~cltItemManagerClient();

	VOID InIt(NGameDataManager& gameDataManager);
	
	void SetItemData(int siServiceArea);

	int GetType(int siItemUnique);

	// 음식 아이템을 추가한다. 
	void AddFoodItem(TCHAR* datfilename);
	// 수산물아이템을 추가한다. 
	void AddSeaFoodItem(TCHAR* datfilename);
	// 약 아이템을 추가한다. 
	void AddHealItem(TCHAR* datfilename);
	// 칼 아이템을 추가한다. 
	void AddSwordItem(TCHAR* datfilename);
	// 창 아이템을 추가한다. 
	void AddSpearItem(TCHAR* datfilename);
	// 지팡이 아이템을 추가한다. 
	void AddStaffItem(TCHAR* datfilename);
	// 도끼 아이템을 추가한다. 
	void AddAxeItem(TCHAR* datfilename);
	// 활 아이템을 추가한다. 
	void AddBowItem(TCHAR* datfilename);
	// chd 아이템을 추가한다. 
	void AddGunItem(TCHAR* datfilename);
	// 갑옷 아이템을 추가한다. 
	void AddArmourItem(TCHAR* datfilename);
	// 의복 아이템을 추가한다. 
	void AddDressItem(TCHAR* datfilename);
	// 투구 아이템을 추가한다. 
	void AddHelmetItem(TCHAR* datfilename);
	// 요대 아이템을 추가한다. 
	void AddBeltItem(TCHAR* datfilename);
	// 목걸이 아이템을 추가한다. 
	void AddNeckItem(TCHAR* datfilename);
	// 신발 아이템을 추가한다. 
	void AddShoesItem(TCHAR* datfilename);
	// 책 아이템을 추가한다. 
	void AddBookItem(TCHAR* datfilename);
	// 동물 아이템을 추가한다. 
	void AddAnimalItem(TCHAR* datfilename);
	// 기타 아이템을 추가한다. 
	void AddEtcItem(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	// 일반 아이템을 추가한다. 
	void AddItem(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	// 반지 아이템을 추가한다. 
	void AddRingItem(TCHAR* datfilename);
	// 부적 아이템을 추가한다. 
	void AddScrollItem(TCHAR* datfilename);
	// 철물  아이템을 추가한다. 
	void AddIronItem(TCHAR* datfilename);
	// 도기  아이템을 추가한다. 
	void AddDishItem(TCHAR* datfilename);
	// 어구  아이템을 추가한다. 
	void AddFishingRodItem(TCHAR* datfilename);
	// 소환인형 아이템을 추가한다. 
	void AddDollItem(TCHAR* datfilename);
	// 모자 아이템을 추가한다. 
	void AddHatItem(TCHAR* datfilename);
	// 표아이템을 추가한다. 
	void AddTicketItem(TCHAR* datfilename);
	// 가방 아이템을 추가한다.
	void AddBagItem(TCHAR* datfilename);
	// 망토 아이템을 추가한다.
	void AddMantleItem(TCHAR* datfilename);

	// [지연] 아바타 망토 아이템을 추가한다.
	void AddAvatarMantleItem(TCHAR* datfilename);
	// [지연] 아바타 의복 아이템을 추가한다.
	void AddAvatarDressItem(TCHAR* datfilename);
	// [지연] 아바타 모자 아이템을 추가한다.
	void AddAvatarHatItem(TCHAR* datfilename);
	// [지연] 아바타 목걸이 아이템을 추가한다.
	void AddAvatarNeckItem(TCHAR* datfilename);
	// [지연] 아바타 반지 아이템을 추가한다.
	void AddAvatarRingItem(TCHAR* datfilename);

	// 문구 아이템을 추가한다. 
	void AddPaperItem(TCHAR* datfilename);
	// 대포 아이템을 추가한다. 
	void AddCannonItem(TCHAR* datfilename);
	// 어뢰 아이템을 추가한다. 
	void AddTorpedoItem(TCHAR* datfilename);
	// 결정체 아이템을 추가한다. 
	void AddCrystalItem(TCHAR* datfilename);
	//합성 재료 아이템을 추가한다
	void AddCompositionManterial(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	//이벤트 아이템을 추가한다
	void AddEventitem(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	//상자 아이템을 추가한다
	void AddBox(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	//프리미엄 아이템을 추가한다
	void AddPremium(TCHAR* datfilename, TCHAR* pname, SI32 itemtype);
	//광석 아이템을 추가한다
	void AddMineralItem(TCHAR* datfilename);

	void AddQuestItem(TCHAR* datfilename);


//
//	//---------------------------------------------------
//	// Explain
//	//---------------------------------------------------
//	// 아이템에 대한 설명을 얻어온다. 
//	// numswitch : 개수도 표시하는지 여부. 
	BOOL GetExplain(cltItem* pclitem, TCHAR* ptext);

	TCHAR* GetEnchantTypeName( SI32 siEnchantType);
	VOID GetCrystalAbilityTypeName(SI16 siCrystalAbilityType ,SI16 cAbliltyTypeNum,TCHAR* szCrystalReturnString);

	TCHAR* GetSkillNameByUnique( SI32 siSkillUnique);
//
//	// 아이템의 성능 설명을 얻어온다. (unique를 근거로.) 
//	BOOL GetExplain(SI32 itemunique, TCHAR* ptext);
//
//	// 아이템의 생산 정보에 대한 설명을 얻어온다. 
//	BOOL GetProductionExplain(SI32 siunique, cltPSkillInfo* pclskillinfo, cltPItemInfo* pclcharitem, TCHAR* ptext, SI16 txtSize);
//
//	// 아이템의 생산 정보에 대한 물품 정보를 얻어온다.
//	//BOOL GetProductionItemExplain(SI32 siunique, cltPSkillInfo* pclskillinfo, cltPItemInfo* pclcharitem, cltMaterialExplain* pMaterial, SI32 *totalcount);
//
//	// 아이템을 이용한 제작 가능 물품 정보를 얻어온다.
//	//BOOL GetProductionMakeItemExplain(SI32 siunique, cltMakeItem* pMakeItem, SI32 *totalcount);
//
//	//인챈트의 숫자에 해당하는 속성이름을 불러온다.
//	TCHAR* GetEnchantTypeName(TCHAR* szEnchantName, SI32 siEnchantType);
//
//	COLORREF GetItemNameColor(cltItem * pclItem);
//
//
//	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
//	void GetCrystalAbilityTypeName(TCHAR* szCrystalAbilityName, SI16 siCrystalAbilityType ,SI16 cAbliltyTypeNum,TCHAR* szCrystalReturnString);
//	bool GetExplainCrystalClient(SI32 itemunique, TCHAR* ptext);
//
//	void GetCrystalAbilityTypeCondition(SI16 siCrystalAbilityType ,SI16 conditionLevel ,TCHAR* szCrystalReturnString);
//
//protected:
//	//박기형 - 20090615 - 아바타 결정체 설명 문자열( 설명 문자열 전체 버퍼 포인터, 설명 문자열 크기, 아이템 타입, 아이템 참조 값)
//	BOOL	GetExplainEnhanceAvatarItem(TCHAR* pBuffer,SI32 siBufferSize, SI32 siItemType, cltItem* pclitem);
//	//박기형 - 20090615 - 아바타 결정체 세부 강화 정보 문자열(결정체로 최대 누적 할 수 있는 값, 결정체 능력 타입, 누적된 능력치 수치, 리턴 받을 버퍼, 리턴 받을 버퍼 크기)
//	BOOL	GetAvatarCrystalAbilityTypeName(SI16 siMaxCrystalAbility, SI16 siCrystalAbilityType ,SI16 siAbliltyTypeNum,TCHAR* pszCrystalReturnString,SI32 siCrystalReturnStringSize);
//
//	void	GetDurabilityExplain( UI16 curdur, UI16 maxdur, NTCHARString64& kDurExplain, bool bcolorwitch );

	//SI32 GetEnchantGrade(SI32 siEnchantDamage);
};