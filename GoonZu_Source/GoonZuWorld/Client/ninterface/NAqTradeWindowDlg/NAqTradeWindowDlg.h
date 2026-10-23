#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;
class CList;
class COutLine;
class CEdit;

// 아큐 거래창
class CNAqTradeWindowDlg : public CDialogController
{
public:
	CButton*	m_pButton[5];
	CStatic*	m_pStatic[13];	// 2: 서버환율, 5: 보유, 총금액:10
	CList*		m_pList;
	CList*		m_pMyWorldMoneyList;
	COutLine*	m_pOutLine[3];

	CEdit*		m_pEditExplain;	// 아큐 윈도우 설명
	CEdit*		m_pEditSellAq;	// 팔 아큐량
	CEdit*		m_pEditAqPrice;	// 1아큐당 금약
	CEdit*		m_pEditBuyAq;	// 살려는 아큐량

	SI64		m_siSellWorldMoneyNum;	// 팔 아큐량
	SI64		m_siSellWorldMoneyPrice; // 팔 아큐의 금액

	SI64		m_siAverageWorldMoneyPrice; // 아큐의 시세

	CNAqTradeWindowDlg();
	~CNAqTradeWindowDlg();

	SI32 siListIndex[MAX_LISTBOX_ITEM_NUM];
	SI32 siSelectedIndex;

	void Create();

	static void CALLBACK StaticCallBackDialogNAqTradeWindowDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNAqTradeWindowDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetTradeList();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};