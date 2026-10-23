#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;

// 선물하기 확인창
class CNPresentInfoWindowDlg : public CDialogController
{
public:
 
	SI32			m_siSelectProductKey;
	SI32			m_siSelectItemPrice;
	SI32			m_siSelectItemEventPrice;
	SI32			m_siSelectBuyItemNum;

	TCHAR			m_szAccountID[20];
	TCHAR			m_szPresentMessage[200];
	TCHAR			m_szItemName[50];
	
	TCHAR			m_szCharName[20];
	SI32			m_siServerUnique;	
	SI32			m_siItemImageFont;

	CButton*        m_pBtn[2];
	CStatic*		m_pStatic[12];

	CNPresentInfoWindowDlg();
	~CNPresentInfoWindowDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNPresentInfoWindowDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNPresentInfoWindowDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	// 선물 정보창 셋팅
	void SetPresentInfoWindow( TCHAR* ServerAndName , TCHAR* message,TCHAR* itemname, SI32 SelectProductKey, SI32 SelectNumber, SI32 SelectPrice, SI32 SelectEventPrice, SI32 SelectFontIndex,TCHAR* accountid, SI16 ServerUnique, TCHAR* CharName);
};