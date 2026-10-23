#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../../common/item/ItemCommon/ItemUnit.h"



class CNShowGetItemDlg : public CDialogController
{
public:
	//김영훈 수정 2007.12.10 [20391] 아이템 획득 UI개선
	CImageStatic*	m_pImageStaticBackGround;
	CImageStatic*	m_pImageStaticItemImage;
	CStatic*		m_pStatic_itemName;
	CStatic*		m_pStatic_itemUnits;
	CStatic*		m_pStatic_itemPrice;


	CNShowGetItemDlg();
	~CNShowGetItemDlg();

	void Create();
	void Action();
	void SetItemData( cltItem* pclItem, TCHAR* ptszItemName, GMONEY gmPrice );

	static void CALLBACK StaticCallBackDialogNShowGetItem( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK CallBackDialogNShowGetItem( UINT nEvent, int nControlID, CControllerObj* pControl );

protected:
	enum { ADD_TO_WIDTH = 8, MAX_LIMIT_WIDTH = 90 };

	SI16	m_siCurrentFrame;		// 10초후에 안보여지기 위한 ..
	SI32	m_siImageType;			// 무기 이미지 종류
	SI32	m_siImageIndex;			// 무기 이미지 인덱스
	
	SI16	m_siDialogWidth;		//다이얼로그 기본 크기
	SI16	m_siItemWidth;			//아이템 이름 표시 컨트롤 기본 크기

};