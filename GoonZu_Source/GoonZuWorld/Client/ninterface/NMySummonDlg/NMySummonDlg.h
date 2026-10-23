//--------------------------------------------------------------------------------
//	작성자 : 손성웅
//	작성날짜 : 2008 년 08월 12일~
//	제목 : 소환수 보관 시스템
//--------------------------------------------------------------------------------
#pragma once 

#include "../../InterfaceMgr/DialogController.h"
//
class CImageStatic	;
class CStatic		;
class CButton		;
class CEdit			;
class CList			;
class CComboBox		;
class CRadioButton	;
class COutLine		;
//
//#define TRADE_SUMMONRANK	6
//
class CNMySummonDlg : public CDialogController
{
	public:
		CList*		m_plSummonList		;	// 가지고 있는 소환수목록을 나타내는 리스트
		CButton*	m_pBtnOutput		;	// 소환수 슬롯으로 Input
		CButton*	m_pBtnInput			;	// 소환수 슬롯에서 Output

		CButton*	m_pBtSlot1			;	// 슬롯버튼1
		CButton*	m_pBtSlot2			;	// 슬롯버튼2
		CButton*	m_pBtSlot3			;	// 슬롯버튼3
        
		CImageStatic*	m_stSummonSlot1	;	// 활성화된 소환수 얼굴
		CImageStatic*	m_stSummonSlot2	;	// 활성화된 소환수 얼굴
		CImageStatic*	m_stSummonSlot3	;	// 활성화된 소환수 얼굴

		COutLine*	m_OutlineSlot1		;	// 백패널로 쓸 아웃라인
		COutLine*	m_OutlineSlot2		;	// 백패널로 쓸 아웃라인
		COutLine*	m_OutlineSlot3		;	// 백패널로 쓸 아웃라인

		CStatic*	m_pStaticNoName01	;	// 제목
		CStatic*	m_pStaticNoName02	;	// 제목
		CButton*	m_pBtUseTicket		;	// 슬롯버튼1
		COutLine*	m_pOutLine02		;	// 슬롯버튼1
		COutLine*	m_pOutLine01		;	// 슬롯버튼1
		

		CNMySummonDlg()		;
		~CNMySummonDlg()	;

		void Create();
		void Action();
		static void CALLBACK StaticCallBackDialogCNMySummonDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
		void CALLBACK CallBackDialogCNMySummonDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
		void LodaMyOutSummon();
		void LodaMyhaveSummonList();
		void SetListSelectItem( SI16 ListSelectItem ) ;
		void SetSelectSlot( SI16 siSelectSlot ) ;
		SI16 GetListSelectItem(  ) ;
		SI16 GetSelectSlot(  ) ;
	private:
		SI16	siListSelectIndex	;
		SI16	siSlotSelectIndex	;
};