
#pragma once

#include <Directives.h>
#include <windows.h>
#include <stdio.h>
//#include <string.h>

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"

class CStatic;
class CButton;


class CNCandidateDlg : public CDialogController
{
public:
	CNCandidateDlg();
	~CNCandidateDlg();

	void					Create( );
	

	static void CALLBACK	StatiCNCandidateDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NCandidateDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetCandidate(char *);
	void SetNextOrPre(SI32 Flag);



private:
	CStatic     	*m_pCandidateImage;
	CStatic			*m_pCandidateStatic;
	CButton         *m_pCandidateNButton; 
	CButton         *m_pCandidatePButton; 

	char            CandidateBuffer[256];
	SI32            CandidateFlag;
	SI32            CandidateSize;
};


