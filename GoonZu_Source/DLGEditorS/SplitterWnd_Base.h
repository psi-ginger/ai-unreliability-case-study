#pragma once


// CSplitterWnd_Base

#include <vector>

#define LEFT_SIDE     0
#define RIGHT_SIDE    1
#define TOP_SIDE      LEFT_SIDE    
#define BOTTOM_SIDE   RIGHT_SIDE

//class CSplitterWnd_Base : public CWnd
class CSplitterWnd_Base : public CSplitterWnd
{
	DECLARE_DYNAMIC(CSplitterWnd_Base)

public:
	CSplitterWnd_Base(int nLevel = 0);
	virtual ~CSplitterWnd_Base();

protected:
	//DECLARE_MESSAGE_MAP()

// dev member function
public:
	BOOL Create( CWnd* pParentWnd, CRuntimeClass* pView1, CRuntimeClass* pView2,
		CCreateContext* pContext, BOOL bVertical = true, int nID = AFX_IDW_PANE_FIRST );

	int AddView( int nSide, CRuntimeClass* pViewClass, CCreateContext* pContext );

protected:

	inline void SideToRowCol( int nSide, int* nRow, int* nCol )
	{
        if( m_bVertical )
		{
			*nRow = 0;
			*nCol = nSide;
		}
		else
		{
			*nRow = nSide;
			*nCol = 0;
		}
	}

	BOOL HideView( int nRow, int nCol );
	BOOL ShowView( int nRow, int nCol, CWnd* pWnd );

public:

	void SetInitialStatus();

	CSplitterWnd_Base* AddSubDivision( int nSide,
		CRuntimeClass* pView1, CRuntimeClass* pView2, CCreateContext* pContext, BOOL bVertical );

public:
	BOOL m_bVertical;
	std::vector<CWnd*> m_views[2];
	int m_nCurrentView[2]; // side as index
	CSplitterWnd_Base* m_pSubSplitterWnd[2];

	int m_nLevel;

};


