#ifndef _TAXPAYMENTLISTDLG_H
#define _TAXPAYMENTLISTDLG_H

#include "CommonHeader.h"
#include "StructureClass/StructureClass.h"

class cltStructureMngBasic;

class cltTaxPaymentListDlg
{
public:
	cltTaxPaymentListDlg();
	~cltTaxPaymentListDlg();

	void						Create( HINSTANCE hInst, HWND hParent, cltStructureMngBasic *pclStructureManager, TCHAR *szStructure );

	static HWND						m_hWnd;

	static cltStructureMngBasic*	m_pclStructureManager;
	
	static TCHAR						m_szStructure[ 32 ];

	static cltTaxInfo				m_taxInfo;

	static BOOL						m_bTaxDisp;
};

#endif


