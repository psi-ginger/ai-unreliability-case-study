
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../common/SystemNPC/RPMerchantNPC/RPMerchantNPC.h"

class CNRPList;
class CNRPMerchant;

class NPromoter_InfoDlg;
class NPromoter_UrlDlg;


class CNRPBase : public CDialogController
{
public:

	CNRPBase();
	~CNRPBase();

	void	Initialize();
	void	Destroy();

	void	Create( );

	static void CALLBACK StatiCNRPBaseDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NRPBaseDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl);

	void	SetRecommandPoint( SI32 RecommandPoint );
	
	void	VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );
	
	SI32	GetRecommandPoint();
	
	void	RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	InterfaceFactory m_InterfaceFactory;

	CNRPList*			RPListTab;
	CNRPMerchant*		RPMerchantTab;

	NPromoter_InfoDlg*	m_NPromoter_InfoDlg;
	NPromoter_UrlDlg*	m_NPromoter_UrlDlg;

	

private:

	SI32		m_siRecommandPoint;

};