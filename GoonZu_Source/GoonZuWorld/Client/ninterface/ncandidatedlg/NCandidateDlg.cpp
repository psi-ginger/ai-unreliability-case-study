

#include "NCandidateDlg.h"

#include "Client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"


extern cltCommonLogic* pclClient;




CNCandidateDlg::CNCandidateDlg()
{
	m_pCandidateImage= NULL;
    m_pCandidateStatic= NULL;
	m_pCandidateNButton= NULL;
	m_pCandidatePButton= NULL;
	CandidateFlag= 0;
	CandidateSize=0;

}

CNCandidateDlg::~CNCandidateDlg()
{

	SAFE_DELETE(m_pCandidateImage);
	SAFE_DELETE(m_pCandidateStatic);
	SAFE_DELETE(m_pCandidateNButton);
	SAFE_DELETE(m_pCandidatePButton);

	CandidateFlag= 0;
	CandidateSize=0;

}

void CNCandidateDlg::Create()
{
	
	if(!IsCreate())
	{		
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NCandidate/DLG_NCandidate.ddf");
		file.CreatePopupDialog( this, NCANDIDATE_DLG, "dialog_NCandidate", StatiCNCandidateDlgProc);		

		m_pCandidateImage = new CStatic( this );             
		m_pCandidateStatic = new CStatic( this );  
		m_pCandidateNButton = new CButton(this);
		m_pCandidatePButton = new CButton(this);


		
		file.CreateControl(m_pCandidateImage, NCANDIDATE_DIALOG_IMAGE, "imagestatic_death_info1");
		file.CreateControl(m_pCandidateStatic, NCANDIDATE_DIALOG_TEXT, "statictext_NCandidate");
		file.CreateControl(m_pCandidatePButton, NCANDIDATE_DIALOG_PBUTTON, "button_button_NCandidatep");
		file.CreateControl(m_pCandidateNButton, NCANDIDATE_DIALOG_NBUTTON, "button_button_NCandidaten");
		
		m_pCandidatePButton->SetFileName("NInterface/Button/BUTTON_13x22_00_000.SPR");
		m_pCandidatePButton->SetFontIndex(0);

		m_pCandidateNButton->SetFileName("NInterface/Button/BUTTON_13x22_00_000.SPR");
		m_pCandidateNButton->SetFontIndex(4);
		
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


void CNCandidateDlg::SetCandidate(char *Candidate)
{


	strcpy(CandidateBuffer,Candidate);
	
	CandidateSize= strlen(Candidate);
	char buffer[256]="";
	char temp[10]=" ";
	SI32 i=0;
	strcpy(buffer,Candidate);
	

	for(i=0; (buffer[i] != NULL &&  i<8 ) ; ++i)
	{
		temp[i]= buffer[i];

	}	
	temp[i]=NULL;

	m_pCandidateStatic->SetText(temp,DT_WORDBREAK);
}


void CNCandidateDlg::SetNextOrPre(SI32 CandidateFlag)
{
	char temp[10]=" ";
	SI32 i=0;
	for( i=0; (CandidateBuffer[CandidateFlag * 8 + i] != NULL &&  i<8 ) ; ++i)
	{
		temp[i]= CandidateBuffer[CandidateFlag*8 + i];

	}	
	temp[i]=NULL;
	m_pCandidateStatic->SetText(temp,DT_WORDBREAK);
}
