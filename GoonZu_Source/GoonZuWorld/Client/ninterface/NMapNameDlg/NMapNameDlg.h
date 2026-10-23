#ifndef _NMAPNAMEDLG_H_
#define _NMAPNAMEDLG_H_


#include "../../InterfaceMgr/DialogController.h"

class CStatic;

class CNMapNameDlg : public CDialogController
{
enum CMAPNAME_VARIABLE
{
	CMAPNAME_VARIABLE_ENDTIME = 15
};
public:
	CNMapNameDlg();
	~CNMapNameDlg();

	void Create();
	void Action(void);

	static void CALLBACK StaticCallBackDialogNMapName( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNMapName( UINT nEvent, int nControlID, CControllerObj* pControl );

	void ShowMapName( SI32 siMapIndex );

private:
	CStatic*	m_pstaticMapName;
	CStatic*	m_pstaticMapNameShadow;
	CEdit*		m_pGameTip;
	CEdit*		m_pGameTipShadow;
	
	
	SI32		m_siEndValue;

};

#endif