// DEditor.cpp : implementation file
//

#include "stdafx.h"
#include "StdGrfx.h"
//#include "DialogEditorDemo.h"
#include "DEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDEditor

void CDEditor::DrawBackground( CDC* dc, CRect rect, double /*zoom*/ ) //const
{
	CStdGrfx::drawframed3dBox( dc, rect );

	//if(m_pbufferDC)
	//CStdGrfx::drawframed3dBox( m_pbufferDC, rect );

	CDiagramEditor::DrawBackground( dc, rect, 1);
}

void CDEditor::DrawGrid( CDC* dc, CRect /*rect*/, double zoom ) const
{

	COLORREF gridcol = GetGridColor();

	dc->SelectStockObject( BLACK_PEN );

	int stepx = GetVirtualSize().cx / GetGridSize().cx;
	int stepy = GetVirtualSize().cy / GetGridSize().cy;

	for( int x = 0 ; x <= stepx ; x++ )
		for( int y = 0; y <= stepy ; y++ )
			dc->SetPixel( round( ( double ) ( GetGridSize().cx * x ) * zoom ), round( ( double ) ( GetGridSize().cy * y ) * zoom ), gridcol );


}

