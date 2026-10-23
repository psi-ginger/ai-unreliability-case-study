#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "..\CommonLogic\PostOffice\PostOffice.h"
#include "../../../common/Item/ItemCommon\cltItem.h"

class CButton;
class CStatic;
class CList;
class CComboBox;

class cltItem;
class cltSaveUersItemInfo;

#define BASIC_MARKETCONDITIONS_CHARGE 5000 //

// 유저 아이템 찾기
class CNMarketConditionsDlg : public CDialogController
{
public:

	CStatic*			m_pStatic[2];
	CList*			m_pList;
	CButton*			m_pButton[5];
	CComboBox*		m_pComboBox[2];

	SI32			m_siNPCCharUnique;
	SI32			m_selectWhatShowList; // 1 = 일반검색,  2 = 전날 대비, 3 = 일주일 시세.

	SI32			m_siItemTypeIndex;	// 품종 인덱스
	SI32			m_siItemIndex;	// 품목 인덱스

	SI32			m_arrTypeIndex[MAX_ITEMTYPE_NUMBER]; // 품종 목록.
	SI16			m_arrItemIndex[100]; // 선택된 아이템 목록. - 설마 넘진 않겠지.;;

	// 서버로 부터 전달 받은 정보.
	cltMarketConditionsInfo 		m_MarketConditionsInfo[100];
	_SYSTEMTIME				m_sTimeMarketConditionsInfo[8]; // 3= 일주일 시세만 쓴다.	

	CNMarketConditionsDlg();
	~CNMarketConditionsDlg();

	void Create();

	void Set(SI32 npcunique);

	static void CALLBACK StaticCallBackDialogNMarketConditionsDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNMarketConditionsDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void	 ShowList( cltMarketConditionsInfo * pclMCInfo , _SYSTEMTIME* pStimeMCInfo);

	void	 SetItemTypeInComboBox(BOOL bshowall);
	BOOL	 ShowItemListInComboBox(CComboBox* pComboBox, SI32 typeindex, bool btradeinterface);

	void	 RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

