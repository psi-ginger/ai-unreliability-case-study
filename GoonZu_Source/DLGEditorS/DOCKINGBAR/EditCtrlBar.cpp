// EditCtrlBar.cpp : implementation file
//
// 게임개발자용 맵에디터
// 저작권자 (c) 2001 신용우 (grace77@hanimail.com)
//
// 이 프로그램의 모든 권리는 저작권자에게 있습니다.
// 저작권자의 동의없이 프로그램을 설치/사용/수정/배포할 수 없습니다.

#include "stdafx.h"
//#include "../MapEdit.h"
//#include "../DataCtrl/SceneInfo.h"
#include "EditCtrlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCtrlBar

CEditCtrlBar::CEditCtrlBar()
{
}

CEditCtrlBar::~CEditCtrlBar()
{
}


BEGIN_MESSAGE_MAP(CEditCtrlBar, CSizingControlBar)
	//{{AFX_MSG_MAP(CEditCtrlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCtrlBar message handlers

int CEditCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_pageBezier.Create(this))
	//	return -1;

	//m_pageCreate.ShowWindow(SW_SHOW);

	// TODO: Add your specialized creation code here
	return 0;
}


void CEditCtrlBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBar::OnSize(nType, cx, cy);
	
	//// TODO: Add your message handler code here
	//CPageCtrl *pWnd = (CPageCtrl*)GetWindow(GW_CHILD);
	//while (pWnd != NULL)
	//{
	//	pWnd->CalcLayout();
	//	pWnd = (CPageCtrl*)pWnd->GetNextWindow();
	//}
}

void CEditCtrlBar::UpdatePage()
{
	//CViewInfo *pView = ((CMapEditApp*)AfxGetApp())->GetViewInfo();
	//if (pView == NULL) return;

	//switch (pView->m_dwSubMode)
	//{
	//case SM_ANIMATION:
	//	m_pageAnim.UpdateUI();
	//	ShowAnimationPage();
	//	break;

	//case SM_NONE:
	//	m_pageCreate.UpdateUI();
	//	m_pageMesh.UpdateUI();
	//	m_pageLight.UpdateUI();
	//	m_pageCamera.UpdateUI();
	//	m_pageTrigger.UpdateUI();
	//	m_pageItem.UpdateUI();
	//	m_pageSelect.UpdateUI();
	//	m_pageBezier.UpdateUI();

	//	ShowActivePage();
	//	break;

	//default:
	//	ShowActivePage();
	//	break;
	//}
}

void CEditCtrlBar::ShowActivePage()
{
	//CSceneInfo *pSI = ((CMapEditApp*)AfxGetApp())->GetSceneInfo();
	//int type = pSI->GetSelectedType();

	//// Hide Window
	//m_pageAnim.ShowWindow(SW_HIDE);

	//if (type != OT_MESH) m_pageMesh.ShowWindow(SW_HIDE);
	//if (type != OT_LIGHT_OMNI && type != OT_LIGHT_SPOT) m_pageLight.ShowWindow(SW_HIDE);
	//if (type != OT_CAMERA) m_pageCamera.ShowWindow(SW_HIDE);
	//if (type != OT_TRIGGER) m_pageTrigger.ShowWindow(SW_HIDE);
	//if (type != OT_ITEM) m_pageItem.ShowWindow(SW_HIDE);
	//if (type != OT_BEZIER) m_pageBezier.ShowWindow(SW_HIDE);
	//if (type != -1) 
	//{
	//	m_pageCreate.ShowWindow(SW_HIDE);
	//	m_pageSelect.ShowWindow(SW_HIDE);
	//}

	//// Show window	
	//switch (type)
	//{
	//case OT_MESH:
	//	if (!m_pageMesh.IsWindowVisible()) 
	//		m_pageMesh.ShowWindow(SW_SHOW);
	//	break;

	//case OT_LIGHT_OMNI:
	//case OT_LIGHT_SPOT:
	//	if (!m_pageLight.IsWindowVisible())
	//		m_pageLight.ShowWindow(SW_SHOW);
	//	break;

	//case OT_CAMERA:
	//	if (!m_pageCamera.IsWindowVisible())
	//		m_pageCamera.ShowWindow(SW_SHOW);
	//	break;

	//case OT_TRIGGER:
	//	if (!m_pageTrigger.IsWindowVisible())
	//		m_pageTrigger.ShowWindow(SW_SHOW);
	//	break;

	//case OT_ITEM:
	//	if (!m_pageItem.IsWindowVisible())
	//		m_pageItem.ShowWindow(SW_SHOW);
	//	break;

	//case OT_BEZIER:
	//	if (!m_pageBezier.IsWindowVisible())
	//		m_pageBezier.ShowWindow(SW_SHOW);
	//	break;

	//case -1:
	//	if (pSI->GetSelectedCount() != 0) 
	//	{
	//		if (!m_pageSelect.IsWindowVisible())
	//		{
	//			m_pageSelect.ShowWindow(SW_SHOW);
	//			m_pageCreate.ShowWindow(SW_HIDE);
	//		}
	//	}
	//	else 
	//	{
	//		if (!m_pageCreate.IsWindowVisible())
	//		{
	//			m_pageCreate.ShowWindow(SW_SHOW);
	//			m_pageSelect.ShowWindow(SW_HIDE);
	//		}
	//	}
	//}
}

void CEditCtrlBar::ShowAnimationPage()
{
	//CSceneInfo *pSI = ((CMapEditApp*)AfxGetApp())->GetSceneInfo();

	//m_pageCamera.ShowWindow(SW_HIDE);
	//m_pageCreate.ShowWindow(SW_HIDE);
	//m_pageLight.ShowWindow(SW_HIDE);
	//m_pageMesh.ShowWindow(SW_HIDE);
	//m_pageTrigger.ShowWindow(SW_HIDE);
	//m_pageItem.ShowWindow(SW_HIDE);
	//m_pageBezier.ShowWindow(SW_HIDE);

	//m_pageAnim.ShowWindow(SW_SHOW);
}

void CEditCtrlBar::ResetPage()
{
	//CMapEditApp *pApp = (CMapEditApp*)AfxGetApp();
	//CViewInfo *pVI = pApp->GetViewInfo();

	//if (!pVI) return;

	//switch (pVI->m_dwSubMode)
	//{
	//case SM_ANIMATION:
	//	break;
	//case SM_NONE:
	//	m_pageCreate.ResetControl();
	//	m_pageMesh.ResetControl();
	//	break;

	//default:
	//	break;
	//}

	//pVI->RedrawView();

}

BOOL CEditCtrlBar::Create(CWnd *pParent)
{
	if (!CSizingControlBar::Create("객체 편집기", pParent, CSize(180, 480), TRUE, AFX_IDW_TOOLBAR + 10)) return FALSE;
	SetBarStyle(GetBarStyle() |
            CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	return TRUE;
}

