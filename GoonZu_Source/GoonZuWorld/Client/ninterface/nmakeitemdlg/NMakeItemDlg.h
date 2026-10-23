// MakeItemDlg.h: interface for the CNMakeItemDlg class.
//
//////////////////////////////////////////////////////////////////////
/* ==========================================================================
	클래스 :		CNSiegeWinDlg

	작성일 :		05/03/18
	작성자 :		김태양

	목  적 :		
	                

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/


#pragma once
#include <Directives.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"


class CImageStatic;
class CStatic;




class CNMakeItemDlg  : public CDialogController
{
public:
	CNMakeItemDlg();
	 ~CNMakeItemDlg();

	void					Create();
	void					Set( cltItem *pclItem, SI32 baseexp, SI32 skillunique,SI32 skillexp,SI32 monstercorp);

	static void CALLBACK	StatiCNMakeItemDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NMakeItemDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Action();

private:
	CImageStatic			*m_pNMakeitemImage;
	CStatic					*m_pNMakeitemStatic;
	SI32			m_siImageType;	// 아이템 이미지 타입(이미지가 3장)
	SI32			m_siImageIndex; // 아이템 이미지에서 순서

	SI16			m_siCurrentFrame; // 팝업 사라지도록 프레임수 체크
};
