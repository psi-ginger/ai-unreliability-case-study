#ifndef _NDELETECHAR_H_
#define _NDELETECHAR_H_

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNDeleteCharDlg : public CDialogController
{
public:

	CNDeleteCharDlg();
	~CNDeleteCharDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNDeleteCharDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNDeleteCharDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

	void Init();
	void set(TCHAR * szName,TCHAR * szLevel,SI32 currentchar);

	inline BOOL GetCheck()			{ return m_bCheck; }
	inline void SetCheck()			{ m_bCheck = FALSE; }
	inline TCHAR* GetResidenceNum()	{ return m_szResidenceNum; }

private:
	
	BOOL	m_bCheck;
	TCHAR	m_szResidenceNum[12];
	SI32	m_siCurrentCharInfoList;


	CStatic*	m_pStatic[5];
	CEdit*		m_pEdit;
	CButton*	m_pButton[2];

};

#endif