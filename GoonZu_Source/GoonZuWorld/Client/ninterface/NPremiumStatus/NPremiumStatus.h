#pragma once


#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;

#define NPREMINUMSTATUS_MAX_PREMINUMSTATUS	20

class CNPremiumStatus : public CDialogController
{
public:

	CNPremiumStatus();
	~CNPremiumStatus();

	void Initialize();
	void Destroy();

	void Create();

	static void CALLBACK StaticNPremiumProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NPremiumDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void Action();	

	bool IsPremiumShow();

	void SetToolTip(SI32 _siStatusIndex, SI32 _siToolTipTextNum, SI32 _siPara1);

public:

	SI16			m_siImageNum;
	SI16			m_siImageXSize;
	SI16			m_siImageYSize;

	CImageStatic	* m_StatusStatic[NPREMINUMSTATUS_MAX_PREMINUMSTATUS] ;

	TSpr			m_PremiumStatusImg;

	bool			m_PremiumShow;

};
