#pragma once

#include <Directives.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CStatic;

class CNDeathDlg : public CDialogController
{
public:
	CNDeathDlg();
	~CNDeathDlg();

	void					Create( );
	void					set(GMONEY money , SI64 exp, SI16 killmode, TCHAR* pname );

	static void CALLBACK	StatiCNDeathDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NDeathDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	//void					Show();
	//void					Hide();

	inline	void			SetSex(SI16 sex){m_siSex = sex;};

	void					Action();

private:
	CImageStatic	*m_pDeathInfo1Image;
	CStatic			*m_pDeathStatic;
	SI16			m_siBitmapAnimationIndex;
	SI16			m_siSex;
	SI16            Popupflag;
};
