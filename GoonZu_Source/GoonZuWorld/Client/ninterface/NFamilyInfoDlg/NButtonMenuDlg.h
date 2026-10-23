#ifndef _NBUTTONMENUDLG_H_
#define _NBUTTONMENUDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;

#define BUTTONMENU_MAX_BUTTON_COUNT		10
#define BUTTONMENU_BASE_WIDTH_SIZE		100

class CNMouseButton
{
public:
	CButton*		m_pkButton_Command;
	bool			m_bUse;

public:
	CNMouseButton()
	{
		m_pkButton_Command	= NULL;
		m_bUse				= FALSE;
	}
	~CNMouseButton()
	{
		if(m_pkButton_Command)		delete m_pkButton_Command;
	}

	void SetUse(bool bUse)
	{
		if(m_pkButton_Command == NULL)		return;

		m_bUse		= bUse;
		m_pkButton_Command->Show(m_bUse);
	}
};

class CNButtonMenuDlg : public CDialogController
{
public:
	CNButtonMenuDlg();
	~CNButtonMenuDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNButtonMenuDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNButtonMenuDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Set( SI32 Button_Roc_X, SI32 Button_Roc_Y, SI16 Mode, SI32 siButton_width, SI32 siButton_Height, SI32 siTargetPeronId,TCHAR* szTargrtCharName );

	void Action();

	void ClearFocus();
	void SetFocus( bool Focus );

	void CalcWidth( SIZE sizeString );
	void SetPosDlg( SI32 siRoc_X, SI32 siRoc_Y , SI32 width, SI32 Height)	;

private:
	CNMouseButton			m_pkButton_Command[BUTTONMENU_MAX_BUTTON_COUNT];
	CImageStatic*			m_pkImageStatic_Name;
	CImageStatic*			m_pkImageStatic_Back;

	CStatic*				m_pkStatic_Name;

	SI32	m_siTargetPeronId;	// 텔레포트 해갈 대상 personIid
	TCHAR*	m_szTargetName;		// 귓말 넣을 대상 케릭터 이름
	SI32	m_siCharUnique;

	SI16	m_siMode;
};
#endif