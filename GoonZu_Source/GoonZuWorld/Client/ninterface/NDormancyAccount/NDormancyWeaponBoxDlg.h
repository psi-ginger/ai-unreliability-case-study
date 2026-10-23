#pragma once
// NDormancyWeaponBox.h: interface for the NDormancyWeaponBox class.
//
//////////////////////////////////////////////////////////////////////
/* ==========================================================================
클래스 :		CNDormancyWeaponBoxDlg

작성일 :		09/08/01
작성자 :		박기형

목  적 :		휴면계정 무기 박스


용  법 :		
변경사항 :		없음

차후작업 :		

========================================================================*/
#include "Resource.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../Client/Inventory/ItemInventoryBaseDefinition.h"

class CButton;

class CNDormancyWeaponBoxDlg  : public CDialogController
{
public:
	CNDormancyWeaponBoxDlg();
	~CNDormancyWeaponBoxDlg();

	static VOID CALLBACK	StatiCNDormancyWeaponBoxProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	VOID CALLBACK			NDormancyWeaponBoxDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	VOID Create();

	VOID Destroy();

	// 보급 박스 유니크 저장
	VOID SetBoxInfo(SI32	siBoxUnique, SI32 siBoxPos);

	// 선택 완료
	VOID CompleteToSelectItem();

	//	아이템 선택
	VOID SelectItem(SI32 siControllID,CControllerObj* pControl);
private:
	const static SI32	MAX_ITEM_COUNT = 6;
	const static SI32	UNIQUE_BOX = 13354;
	InterfaceFactory m_InterfaceFactory;
	
	SI32		m_siBoxUnique;
	SI32		m_siBoxPos;

	SI32		m_siSelctedOrder;		// 선택된 아이템

	//  아이템 이미지 인덱스
	SI32		m_siImageIndex[MAX_ITEM_COUNT];
	//  아이템 이미지 타입
	SI32		m_siImageType[MAX_ITEM_COUNT];
	//	아이템 리스트뷰 수
	CListView	*m_pItemListView[MAX_ITEM_COUNT];
	//	선택 버튼
	CButton		*m_pOkButton;
	//	설명
	CStatic		*m_pTextStatic;
	CEdit		*m_pTextEdit;
	//	아이템 이미지 값 얻기
	VOID SetItemImage();
};
