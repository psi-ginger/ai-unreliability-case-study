#ifndef _NLOGIN_H_
#define _NLOGIN_H_

#include "../../InterfaceMgr/DialogController.h"
#include "../Client/Interface/LoginClient/LoginClient.h"

class CButton;
class CComboBox;
class CEdit;
class CStatic;
class CCheckBox;

class CNLoginDlg : public CDialogController
{
public:

	CNLoginDlg();
	~CNLoginDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNLoginDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNLoginDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show( TCHAR *strErrorMsg = NULL );
	void Hide();

	bool	GetExistLoginID();

	void	SetLoginData( TCHAR *strID, TCHAR *strPassword );
	void	GetLoginData( stLoginData *pLoginData );

	int		GetStatus();

	void	SmallShow();
	void	BigShow();


	void	ShowMiniHomePage();

	//cyj SI16		m_siCurrentSelectedSiteIndex;
	//cyj stRegistSiteInfo *m_pRegistSiteInfoList;

public:

	stLoginData	m_stLoginData;

	TCHAR		m_strErrorMsg[ LOGIN_ERROR_MSG_LENGTH ];

	bool		m_bErrorExist;

	SI32		m_si32Status;

	bool		m_bFlag;
	bool		m_bIdCheck;
	
	CEdit*		m_pEdit[2];		// 0: 아이디 1: 패스워드
	CButton*	m_pButton[3];	// 0: 확인 1: 취소 2:회원가입
	CStatic*	m_pStatic;
	CCheckBox*  m_pCheckBox;

	CStatic*	m_pStaticID;
	CStatic*	m_pStaticPW;
};

#endif