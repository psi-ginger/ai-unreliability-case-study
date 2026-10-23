#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
class COutLine;

class CNIzoPriceValueDlg : public CDialogController
{
public:

	CNIzoPriceValueDlg();
	~CNIzoPriceValueDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNIzoPriceValueDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNIzoPriceValueDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	inline void SetPriceValue(SI32 PriceValue)	{ m_siPriceValue = PriceValue; }
	inline void SetPriceTime(SI32 PriceTime)	{ m_siPriceTime = PriceTime;   }
	inline void SetTotalMoney(GMONEY TotalMoney)	{ m_siTotalMoney = TotalMoney; }
	inline void SetCalcTime(SI32 CalcTime)		{ m_siCalcTime = CalcTime;	   }
	inline void SetTinTin(SI32 TinTin)			{ m_siTinTin = TinTin;			}
	
	inline SI32	GetPriceValue()	{ return m_siPriceValue;		}
	inline SI32	GetPriceTime()	{ return m_siPriceTime;			}
	inline GMONEY GetTotalMoney()	{ return m_siTotalMoney;	}
	inline SI32 GetCalcTime()	{ return m_siCalcTime;			}
	inline SI32	GetTinTin()		{ return m_siTinTin;			}

public:

	SI32		m_siPriceValue;
	SI32		m_siTinTin;
	SI32		m_siPriceTime;
	GMONEY		m_siTotalMoney;
	SI32		m_siCalcTime;
    
	CStatic*	m_pStatic[11];
	CEdit*		m_pEdit[2];
	CButton*	m_pButton[4];
	COutLine*	m_pOutLine;

	CEdit*		m_pEdit_TinTin;

};