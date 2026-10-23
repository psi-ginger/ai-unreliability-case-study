#pragma once
// NDormancyPointDlg.h: interface for the NDormancyPointDlg class.
//
//////////////////////////////////////////////////////////////////////
/* ==========================================================================
클래스 :		CNHouseNameChangeDlg

작성일 :		09/011/20
작성자 :		박기형

목  적 :		창고 이름 변경
용  법 :		
변경사항 :		없음

차후작업 :		

========================================================================*/
#include "Resource.h"

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;

class CNHouseNameChangeDlg  : public CDialogController
{
public:
	CNHouseNameChangeDlg();
	~CNHouseNameChangeDlg();

	static VOID CALLBACK	StaticCNHouseNameChangeProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	VOID CALLBACK			NHouseNameChangeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	VOID Create();

	VOID Destroy();
	
	VOID SetVillageUnique(SI32 siVillageUnique);
	VOID SetHouseUnitIndex(SI32 siHouseUnitIndex);
	VOID SetOldHouseName(TCHAR *pszOldHouseName);

	VOID RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
private:
	InterfaceFactory m_InterfaceFactory;
	
	//	변경하기 버튼
	CButton		*m_pChangeButton;
	//	취소하기 버튼
	CButton		*m_pCancelButton;
	
	CStatic		*m_pTextStatic;
	CStatic		*m_pText2Static;

	//	기존 이름
	CEdit		*m_pOldNameEdit;
	//	새로운 이름
	CEdit		*m_pNewNameEdit;

	// 마을 유니크
	SI32		m_siVillageUnique;
	// 창고 유닛 인덱스
	SI32		m_siHouseUnitIndex;
};
