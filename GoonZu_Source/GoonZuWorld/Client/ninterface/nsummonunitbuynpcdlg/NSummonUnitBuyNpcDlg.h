#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

#define TRADE_SUMMONRANK	6

class CNSummonUnitBuyNpcDlg : public CDialogController
{
public:

	CNSummonUnitBuyNpcDlg();
	~CNSummonUnitBuyNpcDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNSummonUnitBuyNpcDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNSummonUnitBuyNpcDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void LoadSummonUnit(SI32 siSummonUnitId);
	void CheckSummonUintLevel();	// 소환수 진화단계 체크
	void CheckTicketNumber(SI32 siSummonUnitRank);
	void SellToNPCSuccess();
	void SetSummonPrice(SI16 siIndex, GMONEY siSummonPrice);

	void Set( SI32 NPCCharUnique );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

private:

	SI16 m_siSummonUnitIndex;		//	소환수 인덱스 0, 1뿐
	SI16 m_siSummonUnitRank;	//	소환수 등급
	SI32 m_siUseTicketNumber;	//	진화이용권 사용수
	SI32 m_siTicketValue;		//  진화이용권값
	GMONEY m_siSelectSummonPrice;	//  소환수가 팔릴 가격
	REAL32 m_fRate;				//  소환수 사는 비율값

	CStatic*	m_pStatic[2];		// 0 : 소환수 사는 상인 얼굴, 1: 소환수 얼굴
	CEdit*		m_pEdit[2];			// 0 : 소환수 상인이 하는말   1: 소환수 능력치

	CButton*	m_pBtnSummonUnitSell;
	CButton*	m_pBtnClose;
};