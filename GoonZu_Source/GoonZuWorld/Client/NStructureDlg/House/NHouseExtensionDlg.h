#pragma once
// NDormancyPointDlg.h: interface for the NDormancyPointDlg class.
//
//////////////////////////////////////////////////////////////////////
/* ==========================================================================
클래스 :		CNHouseExtensionDlg

작성일 :		09/011/20
작성자 :		박기형

목  적 :		창고 기간 연장
용  법 :		
변경사항 :		없음

차후작업 :		

========================================================================*/
#include "Resource.h"

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;

class CNHouseExtensionDlg  : public CDialogController
{
public:
	CNHouseExtensionDlg();
	~CNHouseExtensionDlg();

	static VOID CALLBACK	StaticCNHouseExtensionProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	VOID CALLBACK			NHouseExtensionDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	VOID Create();

	VOID Destroy();
	
	// 임대 정보 
	VOID SetExtensionInfoText(TCHAR* pszText);
	
	VOID SetVillageUnique(SI32 siVillageUnique);
	VOID SetHouseUnitIndex(SI32 siHouseUnitIndex);

private:
	InterfaceFactory m_InterfaceFactory;
	
	//	예 버튼
	CButton		*m_pYesButton;
	//	아니오 버튼
	CButton		*m_pNoButton;
	//	임대 정보
	CStatic		*m_pContractStatic;
	//	설명
	CStatic		*m_pTextStatic;
	CStatic		*m_pContractStaticLine;
	CStatic		*m_pTextStaticLine;

	// 마을 유니크
	SI32		m_siVillageUnique;
	// 창고 유닛 인덱스
	SI32		m_siHouseUnitIndex;
};
