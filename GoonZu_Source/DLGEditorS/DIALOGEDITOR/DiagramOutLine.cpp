#include "stdafx.h"
#include "DiagramEditor\DiagramEntity.h"
#include "DiagramOutLine.h"
#include "StdGrfx.h"

#include "DiagramEditor/DiagramEntityContainer.h"

#include "../TextMgr/TextMgr.h"
#include "../../NLib/NTextManager.h"

//cyj extern cltTextMgr g_pTextMgr;
extern NTextManager g_pTextManager;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramOutLine
//##############################################################################
// 전역 변수 선언.. 고유한 OutLine ID 생성에 사용
int sOUTLNEUniqueNum = 1;
//##############################################################################

CDiagramOutLine::CDiagramOutLine()
/* ============================================================
	Function :		CDiagramOutLine::CDiagramOutLine
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	/**
	leon add
	* OutLine Property Creation
	*/
	m_pProperty = NULL;
	m_pProperty = new CString[OUTLNE_PROPERTY_COUNT];

	for(int i=0; i<OUTLNE_PROPERTY_COUNT; i++)
	{
        m_pProperty[i].Empty();
	}

	SetPropertyDefault();

	SetMinimumSize( CSize( 1, 1 ) );

	SetTitle( OUTLNE_STR );

	// 파일 로드시는 안 나타나고, 새로 객체 생성시만 셋팅!!
	m_pProperty[OUTLNE_TEXT_STR] = OUTLNE_DEFAULT_TITLE;

	SetType( OUTLNE_STR );
	SetName( OUTLNE_STR );

	//SetPropertyDialog( &m_dlg, CButtonPropertyDlg::IDD );
	SetPropertyDialog( &m_dlg, COutLinePropertyDlg::IDD );
}

CDiagramOutLine::~CDiagramOutLine()
{
	if(m_pProperty)
	{
		delete [] m_pProperty;
		m_pProperty = NULL;
	}
}

void CDiagramOutLine::IncreaseUniqueIDNum()
{
	CString tmpStr;

	if(m_pProperty != NULL)
	{
		do
		{
			tmpStr.Format(BASIC_ID_FORMAT, OUTLNE_STR, sOUTLNEUniqueNum++);
			
			// 생성시 값 수정!!
			m_pProperty[OUTLNE_RESOURCE_ID] = tmpStr;

		}while(IsOverLapping(this, tmpStr));

	}

}

void CDiagramOutLine::SetPropertyDefault()
{
	if(m_pProperty != NULL)
	{
		//for(int i=0; i<OUTLNE_PROPERTY_COUNT; i++)
		//	m_pProperty[i].Empty();

		if(m_pProperty[OUTLNE_KIND_NAME].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_KIND_NAME] = OUTLNE_STR;
		// 자동 생성 로직 필요!!
		if(m_pProperty[OUTLNE_RESOURCE_ID].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		{
			if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
				m_pProperty[OUTLNE_RESOURCE_ID].Format(EX_ID_FORMAT, strDLGUniqueID, OUTLNE_STR, sOUTLNEUniqueNum);
			else
				m_pProperty[OUTLNE_RESOURCE_ID].Format(BASIC_ID_FORMAT, OUTLNE_STR, sOUTLNEUniqueNum);
		}
		if(m_pProperty[OUTLNE_X_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_X_COORD] = STR_ZERO;
		if(m_pProperty[OUTLNE_Y_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_Y_COORD] = STR_ZERO;
		if(m_pProperty[OUTLNE_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_WIDTH].Format("%d", OUTLNE_SIZE_X);
		if(m_pProperty[OUTLNE_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_HEIGHT].Format("%d", OUTLNE_SIZE_Y);
		// Diable 속성으로 할 것!!
		if(m_pProperty[OUTLNE_BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_BG_IMAGE_FULLPATH].Empty();
		if(m_pProperty[OUTLNE_BG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_BG_WIDTH] = STR_ZERO;
		if(m_pProperty[OUTLNE_BG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_BG_HEIGHT] = STR_ZERO;
		if(m_pProperty[OUTLNE_BG_TILE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_BG_TILE] = STR_ZERO;
		
		if(m_pProperty[OUTLNE_BG_TILE_IMAGE_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_BG_TILE_IMAGE_NUM] = STR_ZERO;
		if(m_pProperty[OUTLNE_BG_START_FONT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_BG_START_FONT] = STR_ZERO;
		if(m_pProperty[OUTLNE_TEXT_COLOR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_TEXT_COLOR].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[OUTLNE_TEXT_COLOR_R].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_TEXT_COLOR_R].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[OUTLNE_TEXT_COLOR_G].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_TEXT_COLOR_G].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[OUTLNE_TEXT_COLOR_B].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_TEXT_COLOR_B].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[OUTLNE_WEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_WEIGHT].Format("%d", DEFAULT_FONT_WEIGHT);

		if(m_pProperty[OUTLNE_TEXT_STYLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_TEXT_STYLE] = DEFAULT_FONT_STYLE;
		if(m_pProperty[OUTLNE_TEXT_SIZE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_TEXT_SIZE].Format("%d", DEFAULT_FONT_SIZE);
		if(m_pProperty[OUTLNE_GROUP_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[OUTLNE_GROUP_NUM] = STR_ZERO;

		//m_pProperty[OUTLNE_TEXT_STR] = OUTLNE_DEFAULT_TITLE;
	}
}

CDiagramEntity* CDiagramOutLine::Clone()
/* ============================================================
	Function :		CDiagramOutLine::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramOutLine* obj = new CDiagramOutLine;
	obj->Copy( this );
	return obj;
}

void CDiagramOutLine::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramOutLine::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	// OutLine 그림
	//dc->SelectObject( CStdGrfx::dialogBrush() );
	//dc->SelectObject( CStdGrfx::dialogPen() );

	//dc->Rectangle( rect );

	//LOGFONT lf;
	//CFont font;
	//GetFont( lf );
	//// MS Sans Serif will not scale below 8 pts
	//if( GetZoom() < 1 )
	//	lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	//font.CreateFontIndirect( &lf );

	//dc->SelectObject( &font );

	//dc->SetBkMode( TRANSPARENT );
	//dc->DrawText( GetTitle(), rect, DT_SINGLELINE );

	//dc->SelectStockObject( ANSI_VAR_FONT );
	//dc->SelectStockObject( BLACK_PEN );
	//dc->SelectStockObject( WHITE_BRUSH );

	// 테두리만 그림
	CStdGrfx::drawFrame( dc, rect );

	/**
	leon add
	* todo - 메모리DC 그리기
	*/
	CString strTemp;

	CRect tmp_rect(0, 0, rect.Width(), rect.Height());
	
	GetPropertyWithIndex(strTemp, OUTLNE_BG_IMAGE_FULLPATH);

	//if( GetPicFullPath() != "" && !IsEqualPicPath() )
	if(m_pProperty[OUTLNE_BG_IMAGE_FULLPATH] != "" && m_pProperty[OUTLNE_BG_IMAGE_FULLPATH] != m_oldPicFullPath)
	{
		if(m_pProperty[OUTLNE_BG_TILE] != STR_ZERO)
			MakeMemDC( dc, atoi(m_pProperty[OUTLNE_BG_START_FONT]), atoi(m_pProperty[OUTLNE_BG_TILE_IMAGE_NUM]));
		else
			MakeMemDC( dc, atoi(m_pProperty[OUTLNE_BG_START_FONT]), WHOLE_IMAGE_STYLE_FONTNUM);

		//SetOldPicFullPath(GetPicFullPath());
		SetOldPicFullPath(m_pProperty[OUTLNE_BG_IMAGE_FULLPATH]);
	}

	if( m_pProperty[OUTLNE_BG_IMAGE_FULLPATH] != "" )
	{
		// Window Style Control
		//CStdGrfx::drawframed3dBox( GetBufferDC(0), tmp_rect );
		//BitBltDS( GetBufferDC(0), GetBufferDC(2), tmp_rect.Width(), tmp_rect.Height(), SRCAND);//SRCAND);
		//BitBltDS( GetBufferDC(0), GetBufferDC(1), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT);

		// Game Style Control
		//BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
		//BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);

		if(m_pProperty[OUTLNE_BG_TILE] != STR_ZERO)
		{
			DrawTileImg(dc, SPR_PUTSPRT, 0, rect.left, rect.top, rect.Width(), rect.Height(), atoi(m_pProperty[OUTLNE_BG_TILE_IMAGE_NUM]), true);
		}
		else
		{
			BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
			BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		}

	}

	LOGFONT lf;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	font.CreateFontIndirect( &lf );

	dc->SelectObject( &font );

	dc->SetBkMode( TRANSPARENT );

	int ret = atoi((char*)(LPCTSTR)m_pProperty[OUTLNE_TEXT_STR]);

	if( ret > 0 ) 
		dc->DrawText( GetTxtFromMgr( ret ), rect, DT_SINGLELINE );
	else
        dc->DrawText( m_pProperty[OUTLNE_TEXT_STR], rect, DT_SINGLELINE );

	dc->SelectStockObject( ANSI_VAR_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDiagramEntity* CDiagramOutLine::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramOutLine::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramOutLine* obj = new CDiagramOutLine;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramOutLine::Export( UINT /*format*/ )
/* ============================================================
	Function :		CDiagramOutLine::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{

	CString input( "\t<div class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;'>%s</div>" );
	CString str;
	CRect rect = GetRect();
	CString title = GetTitle();
	title.Replace( " ", "&nbsp;" );
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), title );
	return str;
}