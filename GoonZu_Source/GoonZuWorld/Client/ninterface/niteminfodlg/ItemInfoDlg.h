#ifndef _NITEMINFO_H_
#define _NITEMINFO_H_

#include "../Client/InterfaceMgr/DialogController.h"
#include "../common/Item/ItemCommon/cltItem.h"

#define LEAST_COMMON_MULTILPLE	30
class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class cltItem;


class CNItemInfoDlg : public CDialogController
{
public:

	CNItemInfoDlg();
	~CNItemInfoDlg();

	void Create(CControllerMgr * pParent);

	static void CALLBACK StaticCallBackDialogNItemInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNItemInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	void SetItemTypeInComboBox(BOOL bshowall);
	BOOL ShowItemListInComboBox(CComboBox* pComboBox, SI32 typeindex, bool btradeinterface);

	void ShowItemInfo(TCHAR * pStrname);
	void SetItemInfo(TCHAR* pStrname);	// 선택된 아이템 정보를 표시

	SI32 GetItemInfoPosX();
	SI32 GetItemInfoPosY();

	TCHAR* GetFromTypeFromName(SI16 ItemType);//타입으로 이름 찾기
	void SetMaterialRefresh()	{		m_bRequestMaterialRefresh = true;	}
	
	// 검색 기능을 위해
	void ItemFind(TCHAR* pStrname);	// 선택된 아이템 정보를 표시
	void Find(TCHAR* pItemname);
	void AddListItem(TCHAR* findname);
	void Update();
private:

	SI32			m_siTypeIndex;	// 품종 인덱스
	SI32			m_siItemIndex;	// 품목 인덱스
	SI32			m_siSelectMonsterIndex;

	SI32			m_siImageType;	// 아이템 이미지 타입(이미지가 3장)
	SI32			m_siImageIndex; // 아이템 이미지에서 순서

	SI32			m_arrTypeIndex[MAX_ITEMTYPE_NUMBER];
	
	SI32			m_siCurrentSelectedItemUnique;
	SI32			m_timerCount;

	CComboBox*		m_pComboBox[2];
	CStatic*		m_pStatic[7]; // 0: 품종, 1: 품목, 2: 성능, 3: 재료, 4:생산, 5:몬스터, 6:제조 가능 물품

	CEdit*			m_pEdit[3];
	CImageStatic*	m_pImageStatic;
	
	CList			*m_pList[3];

	BOOL			m_bRequestMaterialRefresh;

	CButton*		m_pQuestButton;
	CButton*		m_pQuestButtonProperty;

	// 검색기능
	CList			*m_pListBox_AutoMake;
	CEdit			*m_pEdit_Name;
	CButton			*m_pButton_Accept;
	CStatic			*m_pStatic_Back;//백그라운드
private:
	NTCHARString64		m_LastInput;
};

#endif