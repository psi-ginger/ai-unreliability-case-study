#include "TaxPaymentListDlg.h" 
#include "../../resource.h"

#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"
#include "Map\FieldObject\FieldObject.h"

#include "MsgType-Structure.h"


HWND				cltTaxPaymentListDlg::m_hWnd;
cltStructureMngBasic* cltTaxPaymentListDlg::m_pclStructureManager;
TCHAR				cltTaxPaymentListDlg::m_szStructure[];
cltTaxInfo			cltTaxPaymentListDlg::m_taxInfo;
BOOL				cltTaxPaymentListDlg::m_bTaxDisp;

extern cltCommonLogic* pclClient;

cltTaxPaymentListDlg::cltTaxPaymentListDlg()
{
	m_hWnd = NULL;
}

cltTaxPaymentListDlg::~cltTaxPaymentListDlg()
{

}

void cltTaxPaymentListDlg::Create( HINSTANCE hInst, HWND hParent, cltStructureMngBasic *pclStructureManager, TCHAR *szStructure )
{

	
	m_pclStructureManager = pclStructureManager;

	StringCchCopy( m_szStructure, 32, szStructure );


	TCHAR buf[ 256 ];

	TCHAR* pText = GetTxtFromMgr(1658);
	StringCchPrintf( buf, 256, pText, m_szStructure );

	SetWindowText( m_hWnd, buf );
}

