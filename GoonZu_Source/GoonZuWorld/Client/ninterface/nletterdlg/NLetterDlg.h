

//#include "NLetterDlgBaseInfo.h"
#include "..\..\interface\LetterDlg\LetterDlgBaseInfo.h"
#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"

class CLetterMgr;
class CObj;
class CStatic;
class CButton;
//class CImageStatic;

#define POPUP_TIME		200

class CNLetterDlg  : public CDialogController
{
public:
	CNLetterDlg();
	~CNLetterDlg();

	
	void	Create();
	
	void Minimi(BOOL bMini );

	static	void CALLBACK StatiCNLetterDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void	CALLBACK NLetterDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();
	void ShowMinimizeButton(bool haveMinimizeButton);

	bool	Push( TCHAR *strLetterMsg, SI32 siTitleTxtMgr = 0 );	
	BOOL	IsMinimi(){	return m_bMini;	};
	SI16	GetMinimizedSlot(){return m_siMinimizedSlot;};

private:
	CLetterMgr	*m_pLetterMgr;
	bool		m_bShow;
	CObj		*m_pCurrentLetter;
	
	CStatic			*m_pNLetterStatic;
	CButton	     	*m_pNLetterButton1;
	CButton	     	*m_pNLetterButton2;
	CButton			*m_pImageMinimizedButton;

	SI32			 Popupflag;
	
	SI16			m_siOldPosX;
	SI16			m_siOldPosY;

	TCHAR			m_szHiddenTitle[256];
	//CImageStatic	*m_pImageMinimized;
	
	CButton			*m_pMinimizeButton;

	BOOL			m_bMini;
	SI16			m_siMinimizedSlot;
};




