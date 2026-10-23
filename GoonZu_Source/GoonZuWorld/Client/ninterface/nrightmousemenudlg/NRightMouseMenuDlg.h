#ifndef _NRIGHTMOUSEMENUDLG_H_
#define _NRIGHTMOUSEMENUDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class cltCharClient;
#define RIGHTMOUSEMENU_ADD_PERSONSHOP 0x0001
#define RIGHTMOUSEMENU_ADD_REPAIRSHOP 0x0002
#define RIGHTMOUSEMENU_ADD_PVP			0x0004
#define RIGHTMOUSEMENU_ONLY_GIVEPRESENT 0x0008
#define RIGHTMOUSEMENU_ONLY_MAKESHOP 0x0012

#define RIGHTMOUSEMENU_MAX_BUTTON_COUNT		11
#define RIGHTMOUSEMENU_BASE_WIDTH_SIZE		72

class CNRightMouseButton
{
public:
	CButton*		m_pkButton_Command;
	bool			m_bUse;

public:
	CNRightMouseButton()
	{
		m_pkButton_Command	= NULL;
		m_bUse				= FALSE;
	}
	~CNRightMouseButton()
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

class CNRightMouseMenuDlg : public CDialogController
{
public:
	CNRightMouseMenuDlg();
	~CNRightMouseMenuDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNRightMouseMenu( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNRightMouseMenu( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Set( SI32 UnderMouseID, SI32 CharUnique, SI32 MouseXPos, SI32 MouseYPos , SI16 Mode , cltCharClient* pclCharClient = NULL );

	void Action();

	void ClearFocus();
	void SetFocus( bool Focus );

	void CalcWidth( SIZE sizeString );

private:
	CNRightMouseButton		m_pkButton_Command[RIGHTMOUSEMENU_MAX_BUTTON_COUNT];
	CImageStatic*			m_pkImageStatic_Name;
	CImageStatic*			m_pkImageStatic_Back;

	CStatic*				m_pkStatic_Name;

	SI32	m_siUnderMouseID;
	SI32	m_siCharUnique;

	SI16	m_siMode;
};

/*
class CNRightMouseMenuDlg : public CDialogController
{
public:
	CNRightMouseMenuDlg();
	~CNRightMouseMenuDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNRightMouseMenu( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNRightMouseMenu( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Set( SI32 UnderMouseID, SI32 CharUnique, SI32 MouseXPos, SI32 MouseYPos , SI16 Mode );
	
	void ClearFocus();
	void SetFocus( bool Focus );

	void ExecuteCommand( SI16 CommandIndex );
	void ExecuteCommandNoFuchikomi( SI16 CommandIndex );

	void Action();

private:
	CImageStatic	*m_pImageStatic;
	CStatic			*m_pStaticCharName;

	SI32	m_siUnderMouseID;
	SI32	m_siCharUnique;

	SI16	m_siMode;
	
	SI16	m_siStartYPos;
	SI16	m_siLineHeight;

};
*/
#endif