
#include "stdafx.h"
#include "DiagramListbox.h"
#include "StdGrfx.h"
#include "DiagramEditor/DiagramEntityContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../TextMgr/TextMgr.h"
#include "../../NLib/NTextManager.h"

//cyj extern cltTextMgr g_pTextMgr;
extern NTextManager g_pTextManager;

/////////////////////////////////////////////////////////////////////////////
// CDiagramListbox
//##############################################################################
// 전역 변수 선언.. 고유한 ListBox ID 생성에 사용
int sLSTBUniqueNum = 1;
//##############################################################################

CDiagramListbox::CDiagramListbox()
/* ============================================================
	Function :		CDiagramListbox::CDiagramListbox
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	/**
	leon add
	* ListBox Property Creation
	*/
	m_pProperty = NULL;
	m_pProperty = new CString[LSTB_PROPERTY_COUNT];

	for(int i=0; i<LSTB_PROPERTY_COUNT; i++)
	{
        m_pProperty[i].Empty();
	}

	SetPropertyDefault();

	SetMinimumSize( CSize( 20, 20 ) );

	SetTitle( LSTB_DEFAULT_TITLE );

	SetType( LSTB_STR );
	SetName( LSTB_STR );

	//SetPropertyDialog( &m_dlg, CEditPropertyDlg::IDD );
	SetPropertyDialog( &m_dlg, CListBoxPropertyDlg::IDD );

}

CDiagramListbox::~CDiagramListbox()
{
	if(m_pProperty)
	{
		delete [] m_pProperty;
		m_pProperty = NULL;
	}
}

void CDiagramListbox::IncreaseUniqueIDNum()
{

	CString tmpStr;

	if(m_pProperty != NULL)
	{
		do
		{
			//if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
			//	tmpStr.Format(EX_ID_FORMAT, strDLGUniqueID, LSTB_STR, sLSTBUniqueNum++);
			//else
				tmpStr.Format(BASIC_ID_FORMAT, LSTB_STR, sLSTBUniqueNum++);
			
			// 생성시 값 수정!!
			m_pProperty[LSTB_RESOURCE_ID] = tmpStr;

		}while(IsOverLapping(this, tmpStr));

	}

	//// 정적 컨트롤 개수 증가
	//sLSTBUniqueNum++;
}

void CDiagramListbox::SetPropertyDefault()
{
	if(m_pProperty != NULL)
	{
		//for(int i=0; i<LSTB_PROPERTY_COUNT; i++)
		//	m_pProperty[i].Empty();

		if(m_pProperty[LSTB_KIND_NAME].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_KIND_NAME] = LSTB_STR;
		// 자동 생성 로직 필요!!
		if(m_pProperty[LSTB_RESOURCE_ID].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		{
			if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
				m_pProperty[LSTB_RESOURCE_ID].Format(EX_ID_FORMAT, strDLGUniqueID, LSTB_STR, sLSTBUniqueNum);
			else
				m_pProperty[LSTB_RESOURCE_ID].Format(BASIC_ID_FORMAT, LSTB_STR, sLSTBUniqueNum);
		}
		if(m_pProperty[LSTB_X_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_X_COORD] = STR_ZERO;
		if(m_pProperty[LSTB_Y_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_Y_COORD] = STR_ZERO;
		if(m_pProperty[LSTB_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_WIDTH].Format("%d", LSTB_SIZE_X);
		if(m_pProperty[LSTB_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_HEIGHT].Format("%d", LSTB_SIZE_Y);
		// Diable 속성으로 할 것!!
		if(m_pProperty[LSTB_BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_BG_IMAGE_FULLPATH].Empty();
		if(m_pProperty[LSTB_BG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_BG_WIDTH] = STR_ZERO;
		if(m_pProperty[LSTB_BG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_BG_HEIGHT] = STR_ZERO;
		if(m_pProperty[LSTB_BG_TILE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_BG_TILE] = STR_ZERO;

		if(m_pProperty[LSTB_BG_TILE_IMAGE_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_BG_TILE_IMAGE_NUM] = STR_ZERO;
		if(m_pProperty[LSTB_BG_START_FONT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_BG_START_FONT] = STR_ZERO;
		if(m_pProperty[LSTB_TEXT_COLOR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_TEXT_COLOR].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[LSTB_TEXT_COLOR_R].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_TEXT_COLOR_R].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[LSTB_TEXT_COLOR_G].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_TEXT_COLOR_G].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[LSTB_TEXT_COLOR_B].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_TEXT_COLOR_B].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[LSTB_WEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_WEIGHT].Format("%d", DEFAULT_FONT_WEIGHT);



		if(m_pProperty[LSTB_TEXT_STYLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_TEXT_STYLE] = DEFAULT_FONT_STYLE;
		if(m_pProperty[LSTB_TEXT_SIZE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_TEXT_SIZE].Format("%d", DEFAULT_FONT_SIZE);
		if(m_pProperty[LSTB_GROUP_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_GROUP_NUM] = STR_ZERO;

		if(m_pProperty[LSTB_MULTI_COLUMN].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_MULTI_COLUMN] = STR_ONE;

		if(m_pProperty[LSTB_MULTI_SELECTION].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_MULTI_SELECTION] = STR_ZERO;

		if(m_pProperty[LSTB_LINE_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_LINE_HEIGHT].Format("%d", LSTB_LINE_HT);

		if(m_pProperty[LSTB_FIXED_ROW_VISIBLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTB_FIXED_ROW_VISIBLE] = STR_ONE;
	}
}

CDiagramEntity* CDiagramListbox::Clone()
/* ============================================================
	Function :		CDiagramListbox::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramListbox* obj = new CDiagramListbox;
	obj->Copy( this );
	return obj;
}

void CDiagramListbox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramListbox::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{
	/**
	leon add
	* todo - 메모리DC 그리기
	*/
	CString strTemp;

	CRect tmp_rect(0, 0, rect.Width(), rect.Height());
	
	GetPropertyWithIndex(strTemp, LSTB_BG_IMAGE_FULLPATH);

	//if( GetPicFullPath() != "" && !IsEqualPicPath() )
	if(m_pProperty[LSTB_BG_IMAGE_FULLPATH] != "" && m_pProperty[LSTB_BG_IMAGE_FULLPATH] != m_oldPicFullPath)
	{
		if(m_pProperty[LSTB_BG_TILE] != STR_ZERO)
			MakeMemDC( dc, atoi(m_pProperty[LSTB_BG_START_FONT]), atoi(m_pProperty[LSTB_BG_TILE_IMAGE_NUM]));
		else
			MakeMemDC( dc, atoi(m_pProperty[LSTB_BG_START_FONT]), WHOLE_IMAGE_STYLE_FONTNUM);

		//SetOldPicFullPath(GetPicFullPath());
		SetOldPicFullPath(m_pProperty[LSTB_BG_IMAGE_FULLPATH]);
	}

	if( m_pProperty[LSTB_BG_IMAGE_FULLPATH] != "" )
	{
		// Window Style Control
		//CStdGrfx::drawframed3dBox( GetBufferDC(0), tmp_rect );
		//BitBltDS( GetBufferDC(0), GetBufferDC(2), tmp_rect.Width(), tmp_rect.Height(), SRCAND);//SRCAND);
		//BitBltDS( GetBufferDC(0), GetBufferDC(1), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT);

		// Game Style Control
		//BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
		//BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		if(m_pProperty[LSTB_BG_TILE] != STR_ZERO)
		{
			DrawTileImg(dc, SPR_PUTSPRT, 0, rect.left, rect.top, rect.Width(), rect.Height(), atoi(m_pProperty[LSTB_BG_TILE_IMAGE_NUM]), true);
		}
		else
		{
			BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
			BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		}

	}
	//else
	//	CStdGrfx::draw3dFrame( dc, rect );
	CRect clientRect( rect );

	// 테두리만 그림
	CStdGrfx::draw3dFrame( dc, rect );

	//CStdGrfx::drawsunkenframed3dWindow( dc, rect );

	LOGFONT lf;
	GetFont( lf );
	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

	// Sidebar
	dc->SelectObject( CStdGrfx::scrollBrush() );
	dc->SelectObject( CStdGrfx::scrollPen() );
	int side = ( int ) ( ( double ) GetMinimumSize().cx * GetZoom() );

	rect.left = rect.right - side;
	rect.InflateRect( -2, -2 );
	dc->Rectangle( rect );

	CFont chk;
	chk.CreateFont( lf.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SelectObject( &chk );

	CRect rectTop( rect );
	CRect rectBottom( rect );
	rectTop.bottom = rectTop.top + rect.Width();
	rectBottom.top = rectBottom.bottom - rect.Width();

	int clientHeight = clientRect.Height() - 4;
	if( clientHeight < rectTop.Height() + rectBottom.Height() )
	{
		rectTop.bottom = rectTop.top + clientHeight / 2;
		rectBottom.top = rectTop.bottom;
	}


	CStdGrfx::drawframed3dBox( dc, rectTop );
	CStdGrfx::drawframed3dBox( dc, rectBottom );
	CRect tmpRect( rectTop.left, rectTop.bottom, rectBottom.right, rectBottom.top);
	CStdGrfx::drawframed3dBox( dc, tmpRect );

	dc->DrawText( "u", rectTop, DT_VCENTER | DT_SINGLELINE | DT_CENTER );
	dc->DrawText( "u", rectBottom, DT_VCENTER | DT_SINGLELINE | DT_CENTER );

	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );



}

CDiagramEntity* CDiagramListbox::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramListbox::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramListbox* obj = new CDiagramListbox;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramListbox::Export( UINT /*format*/ ) const
/* ============================================================
	Function :		CDiagramListbox::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	CString input( "\t<select size=2 class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;' onchange='JavaScript:listboxHandler(this)' name='%s'></select>" );
	CString str;
	CRect rect = GetRect();
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName() );

	return str;
}