#ifndef _NCANDIDATEDLG_H_
#define _NCANDIDATEDLG_H_
#define MAX_CANDIDATE_TEXT  9

#include "InterfaceMgr.h"
#include "DialogController.h"

class CStatic;
class CButton;

class CNCandidateDlg : public CDialogController
{
public:
	CNCandidateDlg();
	~CNCandidateDlg();

	void Create( );
	

	static void CALLBACK	StatiCNCandidateDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NCandidateDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetCandidate(TCHAR *);
	void SetNextOrPre(SI32 Flag);

private:
	
	CStatic			*m_pCandidateStatic[MAX_CANDIDATE_TEXT];

	CButton         *m_pCandidatePButton; 
	CButton         *m_pCandidateNButton; 
	
	TCHAR            CandidateBuffer[256];
	SI32            CandidateFlag;
	SI32            CandidateSize;
};

#endif