// CSplitterWnd_Base.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DLGEditorS.h"
#include "SplitterWnd_Base.h"
#include ".\splitterwnd_base.h"


// CSplitterWnd_Base

IMPLEMENT_DYNAMIC(CSplitterWnd_Base, CWnd)

//CSplitterWnd_Base::CSplitterWnd_Base()
//{
//}

CSplitterWnd_Base::CSplitterWnd_Base(int nLevel) : m_nLevel(nLevel)
{
	m_pSubSplitterWnd[0] = NULL;
	m_pSubSplitterWnd[1] = NULL;

	m_nCurrentView[0] = 0;
	m_nCurrentView[1] = 0;

}

CSplitterWnd_Base::~CSplitterWnd_Base()
{
}

BOOL CSplitterWnd_Base::Create(CWnd *pParentWnd, CRuntimeClass* pView1, CRuntimeClass* pView2, 
						  CCreateContext* pContext, BOOL bVertical, int nID)
{
    int nRow, nCol;
	m_bVertical = bVertical;

	if(bVertical) 
	{
		nRow = 1;
		nCol = 2;
	}
	else
	{
		nRow = 2;
		nCol = 1;
	}
	VERIFY( CreateStatic( pParentWnd, nRow, nCol, WS_CHILD | WS_VISIBLE | WS_BORDER, nID ) );

	if(  pView1 != NULL )
	{
		VERIFY( CreateView(0, 0, pView1, CSize(100, 100), pContext ) );
	}
	
	if( pView2 != NULL )
	{
		if( bVertical )
		{
			VERIFY( CreateView(0, 1, pView2, CSize(100, 100), pContext ) );
		}
		else
		{
			VERIFY( CreateView(1, 0, pView2, CSize(100, 100), pContext ) );
		}
	}

	return true;
}

int CSplitterWnd_Base::AddView( int nSide, CRuntimeClass* pViewClass, CCreateContext* pContext )
{
	int nRow, nCol;
	SideToRowCol( nSide, &nRow, &nCol );

	if( GetDlgItem(IdFromRowCol(nRow, nCol)) )
	{
        HideView( nRow, nCol );
	}

	if( CreateView(nRow, nCol, pViewClass, CSize(10, 10), pContext) == 0 )
		return -1;

	CWnd* pWnd = GetPane( nRow, nCol );
	m_views[nSide].push_back(pWnd);
	m_nCurrentView[nSide] = m_views[nSide].size() - 1;

	ShowView(nRow, nCol, pWnd);

	RedrawWindow();

	return m_nCurrentView[nSide];
}

BOOL CSplitterWnd_Base::HideView( int nRow, int nCol )
{
	CWnd* pWnd = GetPane( nRow, nCol );

	if( !pWnd ) return false;
	pWnd->SetDlgCtrlID(0);
	pWnd->ShowWindow( SW_HIDE );
	
	return true;
}

BOOL CSplitterWnd_Base::ShowView( int nRow, int nCol, CWnd *pWnd )
{
	pWnd->SetDlgCtrlID( IdFromRowCol(nRow, nCol) );
	pWnd->ShowWindow(SW_SHOW);
	return true;
}

CSplitterWnd_Base* CSplitterWnd_Base::AddSubDivision( int nSide,
		CRuntimeClass* pView1, CRuntimeClass* pView2, CCreateContext* pContext, BOOL bVertical )
{
   ASSERT((nSide == 0) || (nSide == 1));
   ASSERT(m_pSubSplitterWnd[nSide] == NULL);

	int nRow, nCol;
	SideToRowCol(nSide,&nRow,&nCol);

   int nID = IdFromRowCol(nRow,nCol);
   m_pSubSplitterWnd[nSide] = new  CSplitterWnd_Base(m_nLevel+1);
   m_pSubSplitterWnd[nSide]->Create(this,pView1,pView2,pContext,bVertical,nID);
   
   return m_pSubSplitterWnd[nSide];
}

// empty function
void CSplitterWnd_Base::SetInitialStatus()
{
	//int c,r;
	//CString sSubKey;
	//sSubKey.Format(_T("Panes_%d"),m_nLevel);
	//CString s;
	//s = AfxGetApp()->GetProfileString(m_sRegKey,sSubKey,_T("-1 150 -1 150"));
	//	sscanf(s,_T("%d %d %d %d"),&c,&m_nPaneSize[0],&r,&m_nPaneSize[1]);

	//if (IsSplittverticaly()) {
	//	SetColumnInfo(0,m_nPaneSize[0],m_nPaneMinSize[0]);
	//	SetColumnInfo(1,m_nPaneSize[1],m_nPaneMinSize[1]);
	//	if (c > -1) ToggleSide(c); 
	//	if (c == -1)  RecalcLayout();
	//}
	//else {
	//	SetRowInfo(0,m_nPaneSize[0],m_nPaneMinSize[0]);
	//	SetRowInfo(1,m_nPaneSize[1],m_nPaneMinSize[1]);
	//	if (r > -1) ToggleSide(r); 
	//	if (r == -1) RecalcLayout();
	//}
	//if (m_pSubSplitterWnd[0] != NULL) m_pSubSplitterWnd[0]->SetInitialStatus();
	//if (m_pSubSplitterWnd[1] != NULL) m_pSubSplitterWnd[1]->SetInitialStatus();

}