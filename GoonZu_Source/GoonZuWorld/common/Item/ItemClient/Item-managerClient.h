//---------------------------------
// 2003/8/7 김태곤
//---------------------------------

#ifndef _ITEMMANAGERCLIENT_H
#define _ITEMMANAGERCLIENT_H

#define MAX_ITEM_IMAGE_NUM		2000		// 아이템 이미지의 최대 개수. 

#include "..\ItemCommon\cltItem.h"

//---------------------------------------------
// MAKE ITEM 정보를 얻어오기위한 구조체 //JAYCEE
//---------------------------------------------
class cltMakeItem{
public:
	TCHAR szName[MAX_ITEM_NAME_LENGTH];
	TCHAR szItemNum[20];

	cltMakeItem()
	{
		memset(szName,	  0, sizeof(szName));
		memset(szItemNum, 0, sizeof(szItemNum));
	}	
};


class cltMaterialExplain
{
public:
	SI32	siUnique;
	TCHAR	szName[MAX_ITEM_NAME_LENGTH];		// 재료의 이름  
	TCHAR	szCount[20];						// 재료의 필요 개수. 

	cltMaterialExplain()
	{
		siUnique = 0;
		memset(szName, 0, sizeof(szName));
		memset(szCount, 0, sizeof(szCount));
	}
};

class cltCharManager;


class cltItemManagerClient : public cltItemManagerCommon{
public :

	// 클라이언트에서만 필요한 부분. 
	SI32 CharUnique;
	TCHAR BitmapFileName[MAX_ITEM_IMAGE_NUM][64]; 

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_ITEM_IMAGE_NUM>		m_ItemImageBigUnique;
#else
	SI16  m_ItemImageBigUnique[MAX_ITEM_IMAGE_NUM];	//[진성] 아이템의 큰 이미지의 유니크 저장. => 2008-9-23
#endif

	cltItemManagerClient(HWND hwnd, SI32 gamemode);
	~cltItemManagerClient();

	
	void SetItemImageBigList(void);																		//[진성] 아아템 큰 이미지 세팅.		 => 2008-9-23
	bool GetItemImageBigFileName( SI16 _ItemUnique, TCHAR** _FileName, SI32* _ImageIndex);				//[진성] 아아템 큰 이미지 이름 얻기. => 2008-9-23

	// 아이템의 비트맵 이미지 정보를 설정한다. (클라이언트 전용) 
	void SetBitmapImage();

	// 아이템의 포지션 정보를 가지고 그 아이템의 포인터를 구한다.
	cltItem* GetItemPointerFromPosInfo(SI32 id, SI32 siDragFrom);

	SI32 GetBitmapIndexFromBitmapName(TCHAR* bitmapname);

	TCHAR* GetBitMapImageFileNamePointer(SI32 index){return BitmapFileName[index];}

	HBITMAP MakeDDBFromDIB(HDC hdc, SI32 imageindex);
	HBITMAP MakeDDBFromDIB(HDC hdc, TCHAR* path);
	
	SI32 GetBitmapIndexFromFileName(const TCHAR* filename);

	// 아이템 종류를 보여준다. 
	BOOL SetItemTypeInCombo(HWND hDlg, SI32 dlgitem, bool bshowall);

	// 개인 상점 전용 아이템 종류를 보여준다. (사기방지를 위해서)
	BOOL SetItemTypeInComboPersonalShop(HWND hDlg, SI32 dlgitem, bool bshowall);

	// itemtypeunique에 해당하는 아이템 리스트를 보여준다. 
	BOOL ShowItemListInCombo(HWND hDlg, SI32 dlgitem, SI32 itemtypeunique, bool btradeinterface);

	//------------------------------------------------
	// 효과음. 
	//------------------------------------------------
	//  아이템 관련 효과음을 연주한다. 
	void PushEffect(SI32 effect);

	// 효과음을 연주한다. 
	void PlaySound(SI32 itemunique, SI32 type);

	// 제작자의 이름을 가져옴
	bool GetMadePersonName(SI32 ID, TCHAR* PersonName, SI32 namesize);
	bool GetMadePersonNameNew(SI32 ID, cltPersonNameInfo* _pclPersonInfo, SI32 namesize);

	
	//---------------------------------------------------
	// Explain
	//---------------------------------------------------
	// 아이템에 대한 설명을 얻어온다. 
	// numswitch : 개수도 표시하는지 여부. 
	BOOL GetExplain(cltItem* pclitem, TCHAR* ptext, bool numswitch = true, bool basicswitch = false , bool bcolorwitch = false , SI32 siKind = 0,bool bweightswitch= true);

	// 아이템의 성능 설명을 얻어온다. (unique를 근거로.) 
	BOOL cltItemManagerClient::GetExplain(SI32 itemunique, TCHAR* ptext);

	// 아이템의 생산 정보에 대한 설명을 얻어온다. 
	BOOL GetProductionExplain(SI32 siunique, cltPSkillInfo* pclskillinfo, cltPItemInfo* pclcharitem, TCHAR* ptext, SI16 txtSize);
	
	// 아이템의 생산 정보에 대한 물품 정보를 얻어온다.
	BOOL GetProductionItemExplain(SI32 siunique, cltPSkillInfo* pclskillinfo, cltPItemInfo* pclcharitem, cltMaterialExplain* pMaterial, SI32 *totalcount);
	
	// 아이템을 이용한 제작 가능 물품 정보를 얻어온다.
	BOOL GetProductionMakeItemExplain(SI32 siunique, cltMakeItem* pMakeItem, SI32 *totalcount);
	
	//인챈트의 숫자에 해당하는 속성이름을 불러온다.
	TCHAR* GetEnchantTypeName(TCHAR* szEnchantName, SI32 siEnchantType);

	COLORREF GetItemNameColor(cltItem * pclItem);


	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	void GetCrystalAbilityTypeName(TCHAR* szCrystalAbilityName, SI16 siCrystalAbilityType ,SI16 cAbliltyTypeNum,TCHAR* szCrystalReturnString);
	bool GetExplainCrystalClient(SI32 itemunique, TCHAR* ptext);

	void GetCrystalAbilityTypeCondition(SI16 siCrystalAbilityType ,SI16 conditionLevel ,TCHAR* szCrystalReturnString);

protected:
	//박기형 - 20090615 - 아바타 결정체 설명 문자열( 설명 문자열 전체 버퍼 포인터, 설명 문자열 크기, 아이템 타입, 아이템 참조 값)
	BOOL	GetExplainEnhanceAvatarItem(TCHAR* pBuffer,SI32 siBufferSize, SI32 siItemType, cltItem* pclitem);
	//박기형 - 20090615 - 아바타 결정체 세부 강화 정보 문자열(결정체로 최대 누적 할 수 있는 값, 결정체 능력 타입, 누적된 능력치 수치, 리턴 받을 버퍼, 리턴 받을 버퍼 크기)
	BOOL	GetAvatarCrystalAbilityTypeName(SI16 siMaxCrystalAbility, SI16 siCrystalAbilityType ,SI16 siAbliltyTypeNum,TCHAR* pszCrystalReturnString,SI32 siCrystalReturnStringSize);

	void	GetDurabilityExplain( UI16 curdur, UI16 maxdur, NTCHARString64& kDurExplain, bool bcolorwitch );

	//SI32 GetEnchantGrade(SI32 siEnchantDamage);
};

#endif
