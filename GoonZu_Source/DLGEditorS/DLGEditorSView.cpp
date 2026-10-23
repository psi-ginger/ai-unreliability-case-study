// DLGEditorSView.cpp : CDLGEditorSView 클래스의 구현
//

#include "stdafx.h"
#include "MainFrm.h"
#include "DLGEditorS.h"

#include "DLGEditorSDoc.h"
#include "DLGEditorSView.h"

// --- DiagramEditor ---
#include "./DialogEditor/StdGrfx.h"
#include "./DialogEditor/DialogSettings.h"

#include "./DialogEditor/DiagramButton.h"
#include "./DialogEditor/DiagramEdit.h"
#include "./DialogEditor/DiagramStatic.h"
#include "./DialogEditor/DiagramCheckbox.h"
#include "./DialogEditor/DiagramRadiobutton.h"
#include "./DialogEditor/DiagramCombobox.h"
#include "./DialogEditor/DiagramListbox.h"
#include "./DialogEditor/DiagramGroupbox.h"

/**
leon add
* new control added!!
*/
#include "./DialogEditor/DiagramImageStatic.h"
#include "./DialogEditor/DiagramListView.h"
#include "./DialogEditor/DiagramOutLine.h"
#include "./DialogEditor/DiagramChatEdit.h"

#include "./DialogEditor/DLGDefine.h"
#include ".\dlgeditorsview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDLGEditorSView

IMPLEMENT_DYNCREATE(CDLGEditorSView, CView)

BEGIN_MESSAGE_MAP(CDLGEditorSView, CView)
	//{{AFX_MSG_MAP(CDLGEditorSView)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(IDC_SETTINGS, OnSettings)
	ON_COMMAND(IDC_PROPERTIES, OnProperties)
	ON_COMMAND(IDC_100PERC, On100perc)
	ON_COMMAND(IDC_110PERC, On110perc)
	ON_COMMAND(IDC_120PERC, On120perc)
	ON_COMMAND(IDC_130PERC, On130perc)
	ON_COMMAND(IDC_140PERC, On140perc)
	ON_COMMAND(IDC_150PERC, On150perc)
	ON_COMMAND(IDC_200PERC, On200perc)
	ON_COMMAND(IDC_25PERC, On25perc)
	ON_COMMAND(IDC_50PERC, On50perc)
	ON_COMMAND(IDC_60PERC, On60perc)
	ON_COMMAND(IDC_70PERC, On70perc)
	ON_COMMAND(IDC_80PERC, On80perc)
	ON_COMMAND(IDC_90PERC, On90perc)
	ON_UPDATE_COMMAND_UI(IDC_PROPERTIES, OnUpdateProperties)
	ON_COMMAND(IDC_SELECT_ALL, OnSelectAll)
	ON_COMMAND(ID_ADD_BUTTON, OnAddButton)
	ON_COMMAND(ID_ADD_EDIT, OnAddEdit)
	ON_COMMAND(ID_ADD_STATIC, OnAddStatic)
	ON_COMMAND(ID_ADD_CHECKBOX, OnAddCheckbox)
	ON_COMMAND(ID_ADD_COMBOBOX, OnAddCombobox)
	ON_COMMAND(ID_ADD_GROUPBOX, OnAddGroupbox)
	ON_COMMAND(ID_ADD_LISTBOX, OnAddListbox)
	ON_COMMAND(ID_ADD_RADIOBUTTON, OnAddRadiobutton)

	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, OnUpdateZoomOut)

	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SHOW_GRID, OnShowGrid)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GRID, OnUpdateShowGrid)
	ON_COMMAND(ID_SNAP_TO_GRID, OnSnapToGrid)
	ON_UPDATE_COMMAND_UI(ID_SNAP_TO_GRID, OnUpdateSnapToGrid)
	ON_COMMAND(ID_MARGINS, OnMargins)
	ON_UPDATE_COMMAND_UI(ID_MARGINS, OnUpdateMargins)
	ON_COMMAND(ID_RESTRAIN, OnRestraints)
	ON_UPDATE_COMMAND_UI(ID_RESTRAIN, OnUpdateRestraints)
	ON_UPDATE_COMMAND_UI(IDC_SELECT_ALL, OnUpdateSelectAll)
	ON_COMMAND(ID_BOTTOM, OnBottom)
	ON_UPDATE_COMMAND_UI(ID_BOTTOM, OnUpdateBottom)
	ON_COMMAND(ID_BOTTOM_ALIGN, OnBottomAlign)
	ON_UPDATE_COMMAND_UI(ID_BOTTOM_ALIGN, OnUpdateBottomAlign)
	ON_COMMAND(ID_DOWN, OnDown)
	ON_UPDATE_COMMAND_UI(ID_DOWN, OnUpdateDown)
	ON_COMMAND(ID_FRONT, OnFront)
	ON_UPDATE_COMMAND_UI(ID_FRONT, OnUpdateFront)
	ON_COMMAND(ID_LEFT_ALIGN, OnLeftAlign)
	ON_UPDATE_COMMAND_UI(ID_LEFT_ALIGN, OnUpdateLeftAlign)
	ON_COMMAND(ID_MAKE_SAME_SIZE, OnMakeSameSize)
	ON_UPDATE_COMMAND_UI(ID_MAKE_SAME_SIZE, OnUpdateMakeSameSize)
	ON_COMMAND(ID_RIGHT_ALIGN, OnRightAlign)
	ON_UPDATE_COMMAND_UI(ID_RIGHT_ALIGN, OnUpdateRightAlign)
	ON_COMMAND(ID_TOP_ALIGN, OnTopAlign)
	ON_UPDATE_COMMAND_UI(ID_TOP_ALIGN, OnUpdateTopAlign)
	ON_COMMAND(ID_UP, OnUp)
	ON_UPDATE_COMMAND_UI(ID_UP, OnUpdateUp)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_ADD_NONE, OnAddNone)
	ON_UPDATE_COMMAND_UI(ID_ADD_NONE, OnUpdateAddNone)
	ON_UPDATE_COMMAND_UI(ID_ADD_BUTTON, OnUpdateAddButton)
	ON_UPDATE_COMMAND_UI(ID_ADD_CHECKBOX, OnUpdateAddCheckbox)
	ON_UPDATE_COMMAND_UI(ID_ADD_COMBOBOX, OnUpdateAddCombobox)
	ON_UPDATE_COMMAND_UI(ID_ADD_EDIT, OnUpdateAddEdit)
	ON_UPDATE_COMMAND_UI(ID_ADD_GROUPBOX, OnUpdateAddGroupbox)
	ON_UPDATE_COMMAND_UI(ID_ADD_LISTBOX, OnUpdateAddListbox)
	ON_UPDATE_COMMAND_UI(ID_ADD_RADIOBUTTON, OnUpdateAddRadiobutton)
	ON_UPDATE_COMMAND_UI(ID_ADD_STATIC, OnUpdateAddStatic)
	ON_COMMAND(ID_BUTTON_MULTIDRAW, OnButtonMultidraw)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MULTIDRAW, OnUpdateButtonMultidraw)

	// Leon Add !!
	ON_COMMAND(ID_ADD_IMAGESTATIC, OnAddImageStatic)
	ON_COMMAND(ID_ADD_LISTVIEW, OnAddListView)
	ON_UPDATE_COMMAND_UI( ID_ADD_IMAGESTATIC, OnUpdateAddImageStatic)
	ON_UPDATE_COMMAND_UI( ID_ADD_LISTVIEW, OnUpdateAddListView)

	ON_COMMAND(ID_ADD_OUTLINE, OnAddOutLine )
	ON_UPDATE_COMMAND_UI(ID_ADD_OUTLINE, OnUpdateAddOutLine)

	ON_COMMAND(ID_ADD_CHATEDIT, OnAddChatEdit )
	ON_UPDATE_COMMAND_UI(ID_ADD_CHATEDIT, OnUpdateAddChatEdit)
	//}}AFX_MSG_MAP

	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_DLG_PROPERTY, OnDlgProperty)
	ON_WM_MOUSEMOVE()
//	ON_COMMAND(ID_VIEW_MAGNET_MODE, OnViewMagnetMode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAGNET_MODE, OnUpdateViewMagnetMode)
	ON_COMMAND(ID_VIEW_MAGNET_MODE, OnViewMagnetMode)
	END_MESSAGE_MAP()

// CDLGEditorSView 생성/소멸

CDLGEditorSView::CDLGEditorSView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CDLGEditorSView::~CDLGEditorSView()
{
}

BOOL CDLGEditorSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CDLGEditorSView 그리기

void CDLGEditorSView::OnDraw(CDC* pDC)
{
	CDLGEditorSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// --- DiagramEditor ---
	// An example of how to handle print 
	// and print preview
	if( pDC->IsPrinting() )
	{
		// Print zoom is the difference between screen- 
		// and printer resolution.
		double zoom = pDC->GetDeviceCaps( LOGPIXELSX ) / m_screenResolutionX;

		CRect rect( 0,0, 
			( int )( ( double ) m_editor.GetVirtualSize().cx * zoom ), 
			( int )( ( double ) m_editor.GetVirtualSize().cy * zoom ) );

		m_editor.Print( pDC, rect, zoom );
	}

	// If we are not printing, we don't have do do 
	// anything special, the editor is a child window 
	// of the view and will be redrawn automatically.

}


// CDLGEditorSView 인쇄

BOOL CDLGEditorSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CDLGEditorSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CDLGEditorSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CDLGEditorSView 진단

#ifdef _DEBUG
void CDLGEditorSView::AssertValid() const
{
	CView::AssertValid();
}

void CDLGEditorSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDLGEditorSDoc* CDLGEditorSView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDLGEditorSDoc)));
	return (CDLGEditorSDoc*)m_pDocument;
}
#endif //_DEBUG


// CDLGEditorSView 메시지 처리기
void CDLGEditorSView::OnInitialUpdate() 
{
	
	CView::OnInitialUpdate();

	// --- DiagramEditor ---
	if( !m_editor.m_hWnd )
	{

		// Creating the editor window
		CDLGEditorSDoc* pDoc = GetDocument();

		CRect rect;
		GetClientRect( rect );

		/**
		leon add
		* m_editor 처음 생성시
		* 다이얼로그 에디터와 CDLGEditorSDoc의 CDiagramEntityContainer 타입
		* 멤버 변수와 연결함
		* 모든 컨트롤 데이터와 다이얼로그 에디터 배경 이미지를 CDEditor로 가져옴
		*/
		m_editor.Create( WS_CHILD | WS_VISIBLE, rect, this, pDoc->GetData() );

		m_editor.m_pFrame = (CMainFrame*)AfxGetMainWnd();

		m_editor.ShowGrid( FALSE );
		m_editor.ShowMargin( TRUE );
		m_editor.SetGridColor( ::GetSysColor( COLOR_3DDKSHADOW ) );

		m_editor.SetVirtualSize( CSize( DLG_SIZE_X, DLG_SIZE_Y ) );
		m_editor.SetResize( TRUE );
		m_editor.SetModified( FALSE );

		// We get the screen resolution, which we will use 
		// for scaling to printer. See also OnDraw.
		CClientDC dc( this );
		m_screenResolutionX = dc.GetDeviceCaps( LOGPIXELSX );

		m_editor.SetRestraints(RESTRAINT_MARGIN);
	}
	else
	{
		// m_editor가 이미 생성되었고
		// View가 업데이트 될 때마다.. 
		// m_editor.Clear() =>
		// CDiagramEditor::RedrawWindow() =>
		// CDiagramEditor::OnPaint() =>
		// CDiagramEditor::Draw() =>
		// CDiagramEditor::DrawObject() =>
		// m_objs->DrawObject() : 각 Entity를 그림
		m_editor.Clear();
	}

}


void CDLGEditorSView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	// --- DiagramEditor ---
	// We resize the editor as well
	if( m_editor.m_hWnd )
		m_editor.MoveWindow(0,0,cx,cy);
	
}

BOOL CDLGEditorSView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	// --- DiagramEditor ---
	// To avoid flicker. The editor
	// covers all of the client area
	return TRUE;
}

///////////////////////////////////////////////////////////
// Copy/paste/undo commands and update handlers
//

void CDLGEditorSView::OnEditCopy() 
{
	m_editor.Copy();
}

void CDLGEditorSView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
    //m_editor.UpdateCopy( pCmdUI );	
}

void CDLGEditorSView::OnEditCut() 
{
	m_editor.Cut();
}

void CDLGEditorSView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	//m_editor.UpdateCut( pCmdUI );	
}

void CDLGEditorSView::OnEditPaste() 
{
	m_editor.Paste();
}

void CDLGEditorSView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	//m_editor.UpdatePaste( pCmdUI );
}

void CDLGEditorSView::OnEditUndo() 
{
	m_editor.Undo();
	
}

void CDLGEditorSView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
    //m_editor.UpdateUndo( pCmdUI );
}

void CDLGEditorSView::OnSelectAll() 
{
	m_editor.SelectAll();	
}

void CDLGEditorSView::OnUpdateSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetObjectCount() );
}

///////////////////////////////////////////////////////////
// Misc dialogs
//

void CDLGEditorSView::OnProperties() 
{
	// Showing the property for the 
	// selected object, if any.
	m_editor.ShowProperties();
}

void CDLGEditorSView::OnUpdateProperties(CCmdUI* pCmdUI) 
{
	// We allowing showing of the property dialog
	// only if one and only one object is selected.
	BOOL res = FALSE;
	if( m_editor.GetSelectCount() == 1 )
		res = TRUE;
	pCmdUI->Enable( res );
}

void CDLGEditorSView::OnSettings() 
{
	CDialogSettings	dlg;

	dlg.m_gridWidth = m_editor.GetGridSize().cx;
	dlg.m_gridHeight = m_editor.GetGridSize().cy;
	m_editor.GetMargins( dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom );
	// Setting Property 창에 디폴트 이미지 경로 속성 추가
	//dlg.m_defaultImagePath = m_editor.m_defaultImagePath;
	if(m_editor.m_objs != NULL)
	{
		dlg.m_defaultImagePath = m_editor.m_objs->GetDefaultFilePath();
		dlg.m_defaultTxtPath = m_editor.m_objs->GetDefaultTxtFilePath();
	}


	if( dlg.DoModal() == IDOK )
	{

		m_editor.SetGridSize( CSize( dlg.m_gridWidth, dlg.m_gridHeight ) );

		CString dlgWidth, dlgHeight;

		m_editor.SetMargins( dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom );
		
		//m_editor.m_defaultImagePath = dlg.m_defaultImagePath;
		if(m_editor.m_objs != NULL)
		{
			m_editor.m_objs->SetDefaultFilePath(dlg.m_defaultImagePath);
			m_editor.m_objs->SetDefaultTxtFilePath( dlg.m_defaultTxtPath);
		}

		// 파일 패스 역슬래쉬 => 슬래쉬로 바꾸기
		m_editor.CheckIntegrity();

		m_editor.RedrawWindow();

	}
}

///////////////////////////////////////////////////////////
// Adding controls. The functions creates an object of the 
// correct type and call StartDrawingObject on the editor.
//

/**
leon add
* new Control
*/
void CDLGEditorSView::OnAddChatEdit()
{
	CDiagramChatEdit* obj = new CDiagramChatEdit;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_CHATEDIT;
}

void CDLGEditorSView::OnUpdateAddChatEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CHATEDIT );
}

void CDLGEditorSView::OnAddOutLine()
{
	CDiagramOutLine* obj = new CDiagramOutLine;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_OUTLINE;
}

void CDLGEditorSView::OnUpdateAddOutLine(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_OUTLINE );
}

void CDLGEditorSView::OnAddImageStatic()
{
	CDiagramImageStatic* obj = new CDiagramImageStatic;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_IMAGESTATIC;
}

void CDLGEditorSView::OnAddListView()
{
	CDiagramListView* obj = new CDiagramListView;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_LISTVIEW;
}

void CDLGEditorSView::OnUpdateAddImageStatic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_IMAGESTATIC );
}

void CDLGEditorSView::OnUpdateAddListView(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LISTVIEW );
}

void CDLGEditorSView::OnAddButton() 
{
	CDiagramButton* obj = new CDiagramButton;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_BUTTON;
}

void CDLGEditorSView::OnAddEdit() 
{
	CDiagramEdit* obj = new CDiagramEdit;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_EDIT;
}

void CDLGEditorSView::OnAddStatic() 
{
	CDiagramStatic* obj = new CDiagramStatic;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_STATIC;
}

void CDLGEditorSView::OnAddCheckbox() 
{
	CDiagramCheckbox* obj = new CDiagramCheckbox;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_CHECKBOX;
}

void CDLGEditorSView::OnAddCombobox() 
{
	CDiagramCombobox* obj = new CDiagramCombobox;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_COMBOBOX;
}

void CDLGEditorSView::OnAddGroupbox() 
{
	CDiagramGroupbox* obj = new CDiagramGroupbox;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_GROUPBOX;
}

void CDLGEditorSView::OnAddListbox() 
{
	CDiagramListbox* obj = new CDiagramListbox;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_LISTBOX;
}

void CDLGEditorSView::OnAddRadiobutton() 
{
	CDiagramRadiobutton* obj = new CDiagramRadiobutton;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_RADIOBUTTON;
}

void CDLGEditorSView::OnAddNone() 
{
	m_editor.StartDrawingObject( NULL );
	m_drawObject = DRAW_OBJECT_NONE;
}

void CDLGEditorSView::OnUpdateAddButton(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_BUTTON );
}

/**
leon add
* CheckBox 사용 - 2005-03-27
*/
void CDLGEditorSView::OnUpdateAddCheckbox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CHECKBOX );
	//pCmdUI->Enable(FALSE);
}

/**
leon add
* RaidoButton 사용 안함
*/
void CDLGEditorSView::OnUpdateAddRadiobutton(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_RADIOBUTTON );
	//pCmdUI->Enable(FALSE);
}

/**
leon add
* GroupBox 사용 안함
*/
void CDLGEditorSView::OnUpdateAddGroupbox(CCmdUI* pCmdUI) 
{
	//pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_GROUPBOX );
	pCmdUI->Enable(FALSE);
}

void CDLGEditorSView::OnUpdateAddCombobox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_COMBOBOX );
}

void CDLGEditorSView::OnUpdateAddEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_EDIT );
}

void CDLGEditorSView::OnUpdateAddListbox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LISTBOX );
}

void CDLGEditorSView::OnUpdateAddStatic(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_STATIC );
}

void CDLGEditorSView::OnUpdateAddNone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( !m_editor.IsDrawing() );
}

///////////////////////////////////////////////////////////
// Zoom handling
//

void CDLGEditorSView::OnZoomIn() 
{
	m_editor.SetZoom( m_editor.GetZoom() + m_editor.GetZoomFactor() * 10 );
}

void CDLGEditorSView::OnZoomOut() 
{
	m_editor.SetZoom( m_editor.GetZoom() - m_editor.GetZoomFactor() * 10 );
}
void CDLGEditorSView::On100perc() 
{
	m_editor.SetZoom( 1.0 );
}

void CDLGEditorSView::On110perc() 
{
	m_editor.SetZoom( 1.1 );
}

void CDLGEditorSView::On120perc() 
{
	m_editor.SetZoom( 1.2 );
}

void CDLGEditorSView::On130perc() 
{
	m_editor.SetZoom( 1.3 );
}

void CDLGEditorSView::On140perc() 
{
	m_editor.SetZoom( 1.4 );
}

void CDLGEditorSView::On150perc() 
{
	m_editor.SetZoom( 1.5 );
}

void CDLGEditorSView::On200perc() 
{
	m_editor.SetZoom( 2.0 );
}

void CDLGEditorSView::On25perc() 
{
	m_editor.SetZoom( .25 );
}

void CDLGEditorSView::On50perc() 
{
	m_editor.SetZoom( .5 );
}

void CDLGEditorSView::On60perc() 
{
	m_editor.SetZoom( .6 );
}

void CDLGEditorSView::On70perc() 
{
	m_editor.SetZoom( .7 );
}

void CDLGEditorSView::On80perc() 
{
	m_editor.SetZoom( .8 );
}

void CDLGEditorSView::On90perc() 
{
	m_editor.SetZoom( .9 );
}

// Zoom Update
// leon add
void CDLGEditorSView::OnUpdateZoomIn(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(FALSE);
}

void CDLGEditorSView::OnUpdateZoomOut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

///////////////////////////////////////////////////////////
// Grid, margin, multidraw and restraints handling
//

void CDLGEditorSView::OnShowGrid() 
{
	m_editor.ShowGrid( !m_editor.IsGridVisible() );
}

void CDLGEditorSView::OnUpdateShowGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsGridVisible() );	
}

void CDLGEditorSView::OnSnapToGrid() 
{
	m_editor.SetSnapToGrid( !m_editor.GetSnapToGrid() );	
}

void CDLGEditorSView::OnUpdateSnapToGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.GetSnapToGrid() );	
}

void CDLGEditorSView::OnMargins() 
{
	m_editor.ShowMargin( !m_editor.IsMarginVisible() );
}

void CDLGEditorSView::OnUpdateMargins(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsMarginVisible() );
}

void CDLGEditorSView::OnRestraints() 
{
	if( m_editor.GetRestraints() == RESTRAINT_MARGIN )
		m_editor.SetRestraints( RESTRAINT_NONE );
	else
		m_editor.SetRestraints( RESTRAINT_MARGIN );
}

void CDLGEditorSView::OnUpdateRestraints(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.GetRestraints() == RESTRAINT_MARGIN );
	
}

void CDLGEditorSView::OnButtonMultidraw() 
{
	m_editor.SetMultidraw( !m_editor.GetMultidraw() );
	m_drawObject = DRAW_OBJECT_NONE;
}

void CDLGEditorSView::OnUpdateButtonMultidraw(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.GetMultidraw() );
}

///////////////////////////////////////////////////////////
// Align menu
//

void CDLGEditorSView::OnLeftAlign() 
{
	m_editor.LeftAlignSelected();
}
void CDLGEditorSView::OnTopAlign() 
{
	m_editor.TopAlignSelected();
}
void CDLGEditorSView::OnRightAlign() 
{
	m_editor.RightAlignSelected();
}
void CDLGEditorSView::OnBottomAlign() 
{
	m_editor.BottomAlignSelected();
}

void CDLGEditorSView::OnUpdateLeftAlign(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}
void CDLGEditorSView::OnUpdateTopAlign(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}
void CDLGEditorSView::OnUpdateRightAlign(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}
void CDLGEditorSView::OnUpdateBottomAlign(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}

void CDLGEditorSView::OnMakeSameSize() 
{
	m_editor.MakeSameSizeSelected();
}
void CDLGEditorSView::OnUpdateMakeSameSize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}

void CDLGEditorSView::OnUp() 
{
	m_editor.Up();
}
void CDLGEditorSView::OnDown() 
{
	m_editor.Down();
}
void CDLGEditorSView::OnFront() 
{
	m_editor.Front();
}
void CDLGEditorSView::OnBottom() 
{
	m_editor.Bottom();
}

void CDLGEditorSView::OnUpdateUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() == 1 );
}
void CDLGEditorSView::OnUpdateDown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() == 1 );
}
void CDLGEditorSView::OnUpdateFront(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() == 1 );
}
void CDLGEditorSView::OnUpdateBottom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() == 1 );
}

///////////////////////////////////////////////////////////
// Exporting
//

void CDLGEditorSView::OnExport() 
{
	CDLGEditorSDoc* pDoc = GetDocument();
	pDoc->Export();	
}

void CDLGEditorSView::OnUpdateExport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetObjectCount() );
}

void CDLGEditorSView::OnDlgProperty()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_editor.m_DLGPropertyDlg.ShowProperties(this);
}

void CDLGEditorSView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnMouseMove(nFlags, point);
}

//void CDLGEditorSView::OnViewMagnetMode()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//}

void CDLGEditorSView::OnUpdateViewMagnetMode(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck( g_bMagnetMode );
}

void CDLGEditorSView::OnViewMagnetMode()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	g_bMagnetMode = ( g_bMagnetMode==TRUE?FALSE:TRUE );
}
