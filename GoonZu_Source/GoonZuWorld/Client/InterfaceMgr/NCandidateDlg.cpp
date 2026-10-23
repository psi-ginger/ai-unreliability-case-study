#include "NCandidateDlg.h"

#include "Interface/Button.h"
#include "Interface/Static.h"

#include <Client.h>
extern cltCommonLogic* pclClient;


CNCandidateDlg::CNCandidateDlg()
{
	
	for(int i=0;i<MAX_CANDIDATE_TEXT;++i)
	{
		m_pCandidateStatic[i]= NULL;
	}

	m_pCandidatePButton= NULL;
	m_pCandidateNButton= NULL;

	CandidateFlag= 0;
	CandidateSize=0;

}

CNCandidateDlg::~CNCandidateDlg()
{
	for(int i=0;i<MAX_CANDIDATE_TEXT;++i)
	{
		SAFE_DELETE(m_pCandidateStatic[i]);

	}


	
	SAFE_DELETE(m_pCandidatePButton);
	SAFE_DELETE(m_pCandidateNButton);

	CandidateFlag = 0;
	CandidateSize = 0;

}

void CNCandidateDlg::Create()
{	
	if( ! IsCreate() )
	{
		CDialogController::Create( NCANDIDATE_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCNCandidateDlgProc, NULL, TEXT("NInterface/Interface/Interface_306x30_00_000.spr"), 0, false, 0, 306, 100, 306, 30, false, false, false );
		
		for(int i=0;i<MAX_CANDIDATE_TEXT;++i)
		{ 
			m_pCandidateStatic[i] = new CStatic( this ); 
			m_pCandidateStatic[i]->Create( NCANDIDATE_DIALOG_TEXT1+i, NULL, NULL, 0, false, 0, 30*i+30 , 10, 120, 50, 120, 50 );
			
		}
		
		m_pCandidatePButton = new CButton(this);
		m_pCandidatePButton->Create( NCANDIDATE_DIALOG_PBUTTON, NULL, TEXT("NInterface/Button/BUTTON_13x22_00_000.SPR"), 0, false, 0, 5, 3, 13, 22, 120, 50 );
		
		m_pCandidateNButton = new CButton(this);
		m_pCandidateNButton->Create( NCANDIDATE_DIALOG_PBUTTON, NULL, TEXT("NInterface/Button/BUTTON_13x22_00_000.SPR"), 4, false, 0, 290, 3, 13, 22, 120, 50 );

		


		/*
		file.CreateControl(m_pCandidateImage, NCANDIDATE_DIALOG_IMAGE, TEXT("imagestatic_death_info1"));
		file.CreateControl(m_pCandidateStatic, NCANDIDATE_DIALOG_TEXT, TEXT("statictext_NCandidate"));
		file.CreateControl(m_pCandidatePButton, NCANDIDATE_DIALOG_PBUTTON, TEXT("button_button_NCandidatep"));
		file.CreateControl(m_pCandidateNButton, NCANDIDATE_DIALOG_NBUTTON, TEXT("button_button_NCandidaten"));
		
		m_pCandidatePButton->SetFileName(TEXT("NInterface/Button/BUTTON_13x22_00_000.SPR"));
		m_pCandidatePButton->SetFontIndex(0);

		m_pCandidateNButton->SetFileName(TEXT("NInterface/Button/BUTTON_13x22_00_000.SPR"));
		m_pCandidateNButton->SetFontIndex(4);
		*/
	}
}

void CALLBACK CNCandidateDlg::StatiCNCandidateDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNCandidateDlg *pThis = (CNCandidateDlg*) pControllerMgr;
	pThis->NCandidateDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNCandidateDlg::NCandidateDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
		
	
	switch( nControlID )
	{
	
	case NCANDIDATE_DIALOG_PBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{  
					++CandidateFlag;
					if(CandidateFlag<0)
						break;

					SetNextOrPre(CandidateFlag);
					
				}
				break;
			}
		}
		break;
	case NCANDIDATE_DIALOG_NBUTTON:
		{			
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{     
					--CandidateFlag;
					if(CandidateSize < CandidateFlag*8)
						break;

					SetNextOrPre(CandidateFlag);
                    
				}
				break;
			}
		}
		break;
	}

	return;
}


void CNCandidateDlg::SetCandidate(TCHAR *Candidate)
{
	StringCchCopy(CandidateBuffer, 256, Candidate);
	
	CandidateSize= _tcslen(Candidate);
	TCHAR buffer[256]=TEXT("");
	TCHAR temp[10];
	StringCchCopy(temp, 10, TEXT("abcdefghig"));
	SI32 i=0;
	StringCchCopy(buffer, 256, Candidate);
	

	for(i=0; (buffer[i] != NULL &&  i<8 ) ; ++i)
	{
		temp[i]= buffer[i];

	}	
	temp[i]=NULL;

	for(int i=0 ;i <MAX_CANDIDATE_TEXT; ++i)
	{
		m_pCandidateStatic[i]->SetText(TEXT("a"),DT_WORDBREAK);
	}
}


void CNCandidateDlg::SetNextOrPre(SI32 CandidateFlag)
{
	/*

	TCHAR temp[10];
	_tcscpy(temp, TEXT("abcdefghig"));
	SI32 i=0;
	for( i=0; (CandidateBuffer[CandidateFlag * 8 + i] != NULL &&  i<8 ) ; ++i)
	{
		temp[i]= CandidateBuffer[CandidateFlag*8 + i];

	}	
	temp[i]=NULL;
	*/
	for(int i=0 ;i <MAX_CANDIDATE_TEXT; ++i)
	{
		m_pCandidateStatic[i]->SetText(TEXT("b"),DT_WORDBREAK);
	}
}
