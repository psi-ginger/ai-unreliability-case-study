
#include "stdafx.h"
#include "DiagramEditor\DiagramEntity.h"
#include "DiagramCheckbox.h"
#include "StdGrfx.h"

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
// CDiagramCheckbox

//##############################################################################
// 전역 변수 선언.. 고유한 ID 생성에 사용
int sCHKBUniqueNum = 1;
//##############################################################################

CDiagramCheckbox::CDiagramCheckbox()
{

	m_pProperty = NULL;
	m_pProperty = new CString[CHKB_PROPERTY_COUNT];

	for(int i=0; i<CHKB_PROPERTY_COUNT; i++)
	{
        m_pProperty[i].Empty();
	}

	SetPicFullPath("");
	SetOldPicFullPath("");
	SetPropertyDefault();

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( CHKB_DEFAULT_TITLE );
	SetType( CHKB_STR );
	SetName( CHKB_STR );

	// 파일 로드시는 안 나타나고, 새로 객체 생성시만 셋팅!!
	m_pProperty[CHKB_TEXT_STR] = CHKB_DEFAULT_TITLE;

	SetPropertyDialog( &m_dlg, CCheckBoxPropertyDlg::IDD );
}

CDiagramCheckbox::~CDiagramCheckbox()
{
	if(m_pProperty)
	{
		delete [] m_pProperty;
		m_pProperty = NULL;
	}

}

// sCHKBUniqueNum : 결국 다음 ID와 연결될 번호를 뜻함
void CDiagramCheckbox::IncreaseUniqueIDNum()
{
	CString tmpStr;

	if(m_pProperty != NULL)
	{
		do
		{
			tmpStr.Format(BASIC_ID_FORMAT, CHKB_STR, sCHKBUniqueNum++);
			
			// 생성시 값 수정!!
			m_pProperty[CHKB_RESOURCE_ID] = tmpStr;

		}while(IsOverLapping(this, tmpStr));

	}
}

// 1. 파일로드시.. 없는 값 세팅을 위해서 사용됨
// 2. 새로 생성시.. 디폴트 값 세팅을 위해서 사용됨
void CDiagramCheckbox::SetPropertyDefault()
{
	CString str;

	if(m_pProperty != NULL)
	{
		if(m_pProperty[CHKB_KIND_NAME].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_KIND_NAME] = CHKB_STR;
		// 자동 생성 로직 필요!!
		if(m_pProperty[CHKB_RESOURCE_ID].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		{
			if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
				m_pProperty[CHKB_RESOURCE_ID].Format(EX_ID_FORMAT, strDLGUniqueID, CHKB_STR, sCHKBUniqueNum);
			else
				m_pProperty[CHKB_RESOURCE_ID].Format(BASIC_ID_FORMAT, CHKB_STR, sCHKBUniqueNum);
		}

		if(m_pProperty[CHKB_X_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_X_COORD] = STR_ZERO;
		if(m_pProperty[CHKB_Y_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_Y_COORD] = STR_ZERO;
		if(m_pProperty[CHKB_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_WIDTH].Format("%d", CHKB_SIZE_X);
		if(m_pProperty[CHKB_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_HEIGHT].Format("%d", CHKB_SIZE_Y);
		// Disable 속성으로 할 것!!
		if(m_pProperty[CHKB_BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_BG_IMAGE_FULLPATH].Empty();
		if(m_pProperty[CHKB_BG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_BG_WIDTH] = STR_ZERO;
		if(m_pProperty[CHKB_BG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_BG_HEIGHT] = STR_ZERO;
		if(m_pProperty[CHKB_BG_TILE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_BG_TILE] = STR_ZERO;

		if(m_pProperty[CHKB_BG_TILE_IMAGE_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_BG_TILE_IMAGE_NUM] = STR_ZERO;
		if(m_pProperty[CHKB_BG_START_FONT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_BG_START_FONT] = STR_ZERO;
		if(m_pProperty[CHKB_TEXT_COLOR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_TEXT_COLOR].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CHKB_TEXT_COLOR_R].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_TEXT_COLOR_R].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CHKB_TEXT_COLOR_G].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_TEXT_COLOR_G].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CHKB_TEXT_COLOR_B].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_TEXT_COLOR_B].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CHKB_WEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_WEIGHT].Format("%d", DEFAULT_FONT_WEIGHT);

		if(m_pProperty[CHKB_TEXT_STYLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_TEXT_STYLE] = DEFAULT_FONT_STYLE;
		if(m_pProperty[CHKB_TEXT_SIZE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_TEXT_SIZE].Format("%d", DEFAULT_FONT_SIZE);
		if(m_pProperty[CHKB_GROUP_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHKB_GROUP_NUM] = STR_ZERO;

		//if(m_pProperty[CHKB_TEXT_STR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		//	m_pProperty[CHKB_TEXT_STR] = CHKB_DEFAULT_TITLE;
	}
}

CDiagramEntity* CDiagramCheckbox::Clone()
/* ============================================================
	Function :		CDiagramCheckbox::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramCheckbox* obj = new CDiagramCheckbox;
	obj->Copy( this );
	return obj;
}

void CDiagramCheckbox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramCheckbox::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	dc->SelectObject( CStdGrfx::dialogBrush() );
	dc->SelectObject( CStdGrfx::dialogPen() );

	// 테두리만 그림
	CStdGrfx::draw3dFrame( dc, rect );

	// 메모리DC 그리기
	CString strTemp;

	CRect tmp_rect(0, 0, rect.Width(), rect.Height());
	
	GetPropertyWithIndex(strTemp, CHKB_BG_IMAGE_FULLPATH);

	if(m_pProperty[CHKB_BG_IMAGE_FULLPATH].Trim() != STR_EMPTY && m_pProperty[CHKB_BG_IMAGE_FULLPATH] != m_oldPicFullPath)
	{
		if(m_pProperty[CHKB_BG_TILE] != STR_ZERO)
			MakeMemDC( dc, atoi(m_pProperty[CHKB_BG_START_FONT]), atoi(m_pProperty[CHKB_BG_TILE_IMAGE_NUM]));
		else
			MakeMemDC( dc, atoi(m_pProperty[CHKB_BG_START_FONT]), WHOLE_IMAGE_STYLE_FONTNUM);

		SetOldPicFullPath(m_pProperty[CHKB_BG_IMAGE_FULLPATH]);
	}

	if( m_pProperty[CHKB_BG_IMAGE_FULLPATH] != "" )
	{

		if(m_pProperty[CHKB_BG_TILE] != STR_ZERO)
		{
			DrawTileImg(dc, SPR_PUTSPRT, 0, rect.left, rect.top, rect.Width(), rect.Height(), atoi(m_pProperty[CHKB_BG_TILE_IMAGE_NUM]), true);
		}
		else
		{
			BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
			BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		}
	}

	LOGFONT lf;
	CFont chk;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	lf.lfHeight = atoi(m_pProperty[CHKB_TEXT_SIZE]);
	font.CreateFontIndirect( &lf );
	
	// Marlett is used for the checkbox
	chk.CreateFont( ( int ) ( ( double ) lf.lfHeight * 1.25 ), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SelectObject( &chk );

	//dc->Rectangle( rect );

	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor( ::GetSysColor( COLOR_WINDOW ) );
	dc->TextOut( rect.left, rect.top, "g" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DSHADOW ) );
	dc->TextOut( rect.left, rect.top, "d" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DHIGHLIGHT ) );
	dc->TextOut( rect.left, rect.top, "d" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DDKSHADOW ) );
	dc->TextOut( rect.left, rect.top, "e" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DLIGHT ) );
	dc->TextOut( rect.left, rect.top, "f" );

	dc->SelectObject( &font );
	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );
	rect.left += ( int ) ( ( double ) abs( lf.lfHeight ) * 1.5 );

	int ret = atoi((char*)(LPCTSTR)m_pProperty[CHKB_TEXT_STR]);

	if( ret > 0 ) 
		dc->DrawText( GetTxtFromMgr( ret ), rect, DT_SINGLELINE );
	else
		dc->DrawText( m_pProperty[CHKB_TEXT_STR], rect, DT_SINGLELINE );

	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SelectStockObject( NULL_PEN );
	dc->SelectStockObject( LTGRAY_BRUSH );

}

CDiagramEntity* CDiagramCheckbox::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramCheckbox::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramCheckbox* obj = new CDiagramCheckbox;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramCheckbox::Export( UINT /*format*/ ) const
/* ============================================================
	Function :		CDiagramCheckbox::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	CString input( "\t<div class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;'><input type=checkbox onfocus='blur()' onclick='JavaScript:checkboxHandler(this)' name='%s' >%s</input></div>" );
	CString str;
	CRect rect = GetRect();
	CString title = GetTitle();
	title.Replace( " ", "&nbsp;" );
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName(), title );
	return str;
}