#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNMarriageLoveLevelDlg : public CDialogController
{
private:
	SI32			m_siMinimizedSlot;

	CImageStatic*	m_pclBackGround;

public:
	CNMarriageLoveLevelDlg( void );
	~CNMarriageLoveLevelDlg( void );

	void Init();
	void Create();

	void Action();

	static void CALLBACK StaticCallBackDialogNMarriageLoveLevel( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNMarriageLoveLevel( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	SI32 GetMinimizedSlot( void )	{	return m_siMinimizedSlot;	}

};