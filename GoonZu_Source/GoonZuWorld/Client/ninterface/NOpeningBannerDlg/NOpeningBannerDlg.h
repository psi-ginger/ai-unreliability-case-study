// CNOpeningBannerDlg - 06/07/14[유상]
// 오프닝 배너를 보여준다.

#pragma once

#include "../../InterfaceMgr/DialogController.h"

#define MAX_BANNER_NUMBER	5
#define MAX_URL_LENGTH		256
  
struct stOpeningBanner
{
	TCHAR szUrl[MAX_URL_LENGTH];
	TCHAR szFileName[MAX_PATH];
};

class CNOpeningBannerDlg : public CDialogController
{
public:
	CNOpeningBannerDlg();
	~CNOpeningBannerDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNOpeningBanner( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK CallBackDialogNOpeningBanner( UINT nEvent, int nControlID, CControllerObj* pControl );

	stOpeningBanner m_stBanner[MAX_BANNER_NUMBER];
	SI16			m_siBannerCount;
	SI16			m_siSelectedIndex;

	CImageStatic*	m_pImageBanner;
	CButton*		m_pButtonPage[MAX_BANNER_NUMBER];
	CButton*		m_pButtonExit;


	void LoadBannerText();
	void SetBannerImage( SI16 index );

	void Action();

private:
};