#pragma once

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\Resource.h"
#include "../InterfaceMgr/DialogController.h"

class CNItemMallGiveStatus : public CDialogController
{
public:
	CNItemMallGiveStatus();
	~CNItemMallGiveStatus();

	void					Create();

	static void CALLBACK	StatiCNItemMallGiveStatusProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NItemMallGiveStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	void                    Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );


private:

	CStatic                 *m_pStatic_ItemImage;
	CStatic                 *m_pStatic_ItemExplain;
	CStatic                 *m_pStatic_StatusExplain;
	CStatic                 *m_pStatic_Str;
	CStatic                 *m_pStatic_Vit;
	CStatic                 *m_pStatic_Dex;
	CStatic                 *m_pStatic_Hnd;
	CStatic                 *m_pStatic_Mag;
	CStatic                 *m_pStatic_Luk;
	CButton                 *m_pButton_Ok;
	CButton                 *m_pButton_Cancel;

	COutLine				*m_pOutLine;

	CButton                 *m_pButton_Str_Plus;
	CButton                 *m_pButton_Vit_Plus;
	CButton                 *m_pButton_Dex_Plus;
	CButton                 *m_pButton_Hnd_Plus;
	CButton                 *m_pButton_Mag_Plus;
	CButton                 *m_pButton_Luk_Plus;
	CButton                 *m_pButton_Str_Minus;
	CButton                 *m_pButton_Vit_Minus;
	CButton                 *m_pButton_Dex_Minus;
	CButton                 *m_pButton_Hnd_Minus;
	CButton                 *m_pButton_Mag_Minus;
	CButton                 *m_pButton_Luk_Minus;



};
