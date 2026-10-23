
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../common/SystemNPC/RPMerchantNPC/RPMerchantNPC.h"


class CNRPMerchant : public CDialogController
{
public:

	CNRPMerchant();
	~CNRPMerchant();

	void	Initialize();
	void	Destroy();
	
	void	Create( CControllerMgr *pParent );

	static void CALLBACK StaticNRPMerchantDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NRPMerchantDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl);
	
	void	SetRecommandProductList(); 
	void	VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );
	

	void	SetItemImageData( SI32 ItemUnique );
	
	void	RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void	Hide();
	void	Show();
	

	InterfaceFactory m_InterfaceFactory;
	
private:
	
	SI32	m_SelectedListIndex;	
	
	SI32	m_siImageType;
	SI32	m_siImageIndex;

};

