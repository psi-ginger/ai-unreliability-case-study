#ifndef _NMSGBOXDLG_H_
#define _NMSGBOXDLG_H_

#include "InterfaceMgr.h"
#include "DialogController.h"

class CEdit;
class CButton;

class CNMsgBoxDlg : public CDialogController
{
public:
	CNMsgBoxDlg();
	~CNMsgBoxDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNMsgBoxDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNMsgBoxDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetMsgData( BYTE *pBuffer, BYTE *pData, SI16 DataLen );

private:
	CEdit	*m_pEdit;

	CButton	*m_pOKButton;
	CButton	*m_pCancelButton;

	stMsgBox m_stMsgBox;

	BYTE	m_DataBuffer[ 256 ];
	SI16	m_siDataLen;
};

#endif