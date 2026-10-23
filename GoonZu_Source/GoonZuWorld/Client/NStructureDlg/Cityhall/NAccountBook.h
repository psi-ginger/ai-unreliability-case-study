#pragma once

#include "../../InterfaceMgr/DialogController.h"

// 마을 장부 Part1
class CNAccountBookPart1Dlg : public CDialogController
{
public:

	InterfaceFactory	m_InterfaceFactory_;

	CNAccountBookPart1Dlg();
	~CNAccountBookPart1Dlg();

	void Create( CControllerMgr *pParent );

	void Show();
	void Hide();

	CControllerObj* AddInterface( SI16 Type, SI32 TypeID );

	void Action();

	static void CALLBACK StaticCallBackDialogNAccountBookPart1Dlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNAccountBookPart1Dlg( UINT nEvent, int nControlID, CControllerObj* pControl );
};

// 마을 장부 Part2
class CNAccountBookPart2Dlg : public CDialogController
{
public:

	InterfaceFactory	m_InterfaceFactory_;

	CNAccountBookPart2Dlg();
	~CNAccountBookPart2Dlg();

	void Create( CControllerMgr *pParent );

	void Show();
	void Hide();

	CControllerObj* AddInterface( SI16 Type, SI32 TypeID );

	static void CALLBACK StaticCallBackDialogNAccountBookPart2Dlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNAccountBookPart2Dlg( UINT nEvent, int nControlID, CControllerObj* pControl );
};

// 마을 장부 Part1
class CNAccountBookPart3Dlg : public CDialogController
{
public:

	InterfaceFactory	m_InterfaceFactory_;

	CNAccountBookPart3Dlg();
	~CNAccountBookPart3Dlg();

	void Create( CControllerMgr *pParent );

	void Show();
	void Hide();

	CControllerObj* AddInterface( SI16 Type, SI32 TypeID );

	static void CALLBACK StaticCallBackDialogNAccountBookPart3Dlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNAccountBookPart3Dlg( UINT nEvent, int nControlID, CControllerObj* pControl );
};

// 마을 장부 기초
class CNAccountBookBaseDlg : public CDialogController
{
public:

	InterfaceFactory	m_InterfaceFactory_;

	CNAccountBookPart1Dlg* m_pPart1Dlg_;
	CNAccountBookPart2Dlg* m_pPart2Dlg_;
	CNAccountBookPart3Dlg* m_pPart3Dlg_;

	CNAccountBookBaseDlg();
	~CNAccountBookBaseDlg();

	void Create();
	CControllerObj* AddInterface( SI16 Type, SI32 TypeID );

	void Action();

	static void CALLBACK StaticCallBackDialogNAccountBookBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNAccountBookBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
};