#include "stdafx.h"
#include "DiagramCombobox.h"
#include "DiagramEditor/DiagramEntityContainer.h"

#include "../MyDefine/MyDefine.h"
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
// CDiagramCombobox
//##############################################################################
// 전역 변수 선언.. 고유한 ComboBox ID 생성에 사용
int sCBXUniqueNum = 1;
//##############################################################################

CDiagramCombobox::CDiagramCombobox()
{

	/**
	leon add
	* COMBOBOX Property Creation
	*/
	m_pProperty = NULL;
	m_pProperty = new CString[CBX_PROPERTY_COUNT];

	for(int i=0; i<CBX_PROPERTY_COUNT; i++)
	{
        m_pProperty[i].Empty();
	}

	SetPropertyDefault();

	// Note the constraints
	//SetConstraints( CSize( 20, 20 ), CSize( -1, 20 ) );
	SetConstraints( CSize( 20, CBX_ROW_HT ), CSize( -1, CBX_ROW_HT ) );

	SetTitle( CBX_STR );

	SetType( CBX_STR );
	SetName( CBX_STR );

	/**
	leon add
	* todo - 초기화
	*/
	SetPicFullPath("");
	SetOldPicFullPath("");

	//SetPropertyDialog( &m_dlg, CEditPropertyDlg::IDD );
	SetPropertyDialog( &m_dlg, CComboBoxPropertyDlg::IDD );

}

void CDiagramCombobox::IncreaseUniqueIDNum()
{
	CString tmpStr;

	if(m_pProperty != NULL)
	{
		do
		{
			//if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
			//	tmpStr.Format(EX_ID_FORMAT, strDLGUniqueID, CBX_STR, sCBXUniqueNum++);
			//else
				tmpStr.Format(BASIC_ID_FORMAT, CBX_STR, sCBXUniqueNum++);
			
			// 생성시 값 수정!!
			m_pProperty[CBX_RESOURCE_ID] = tmpStr;

		}while(IsOverLapping(this, tmpStr));

	}

	//// 정적 컨트롤 개수 증가
	//sCBXUniqueNum++;
}

void CDiagramCombobox::SetPropertyDefault()
{
	if(m_pProperty != NULL)
	{
		//for(int i=0; i<CBX_PROPERTY_COUNT; i++)
		//	m_pProperty[i].Empty();

		if(m_pProperty[CBX_KIND_NAME].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_KIND_NAME] = CBX_STR;
		// 자동 생성 로직 필요!!
		if(m_pProperty[CBX_RESOURCE_ID].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		{
			if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
				m_pProperty[CBX_RESOURCE_ID].Format(EX_ID_FORMAT, strDLGUniqueID, CBX_STR, sCBXUniqueNum);
			else
				m_pProperty[CBX_RESOURCE_ID].Format(BASIC_ID_FORMAT, CBX_STR, sCBXUniqueNum);
		}

		if(m_pProperty[CBX_X_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_X_COORD] = STR_ZERO;
		if(m_pProperty[CBX_Y_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_Y_COORD] = STR_ZERO;
		if(m_pProperty[CBX_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_WIDTH].Format("%d", CBX_SIZE_X);
		if(m_pProperty[CBX_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_HEIGHT].Format("%d", CBX_SIZE_Y);
		// Diable 속성으로 할 것!!
		if(m_pProperty[CBX_BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_BG_IMAGE_FULLPATH].Empty();
		if(m_pProperty[CBX_BG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_BG_WIDTH] = STR_ZERO;
		if(m_pProperty[CBX_BG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_BG_HEIGHT] = STR_ZERO;
		if(m_pProperty[CBX_BG_TILE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_BG_TILE] = STR_ZERO;

		if(m_pProperty[CBX_BG_TILE_IMAGE_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_BG_TILE_IMAGE_NUM] = STR_ZERO;
		if(m_pProperty[CBX_BG_START_FONT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_BG_START_FONT] = STR_ZERO;
		if(m_pProperty[CBX_TEXT_COLOR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_TEXT_COLOR].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CBX_TEXT_COLOR_R].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_TEXT_COLOR_R].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CBX_TEXT_COLOR_G].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_TEXT_COLOR_G].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CBX_TEXT_COLOR_B].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_TEXT_COLOR_B].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CBX_WEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_WEIGHT].Format("%d", DEFAULT_FONT_WEIGHT);

		if(m_pProperty[CBX_TEXT_STYLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_TEXT_STYLE] = DEFAULT_FONT_STYLE;
		if(m_pProperty[CBX_TEXT_SIZE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_TEXT_SIZE].Format("%d", DEFAULT_FONT_SIZE);
		if(m_pProperty[CBX_GROUP_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_GROUP_NUM] = STR_ZERO;

		if(m_pProperty[CBX_POPUP_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_POPUP_HEIGHT].Format("%d", CBX_POPUP_HT);
		if(m_pProperty[CBX_LINE_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CBX_LINE_HEIGHT].Format("%d", CBX_LINE_HT);
	}
}

CDiagramCombobox::~CDiagramCombobox()
{
	if(m_pProperty)
	{
		delete [] m_pProperty;
		m_pProperty = NULL;
	}
}

CDiagramEntity* CDiagramCombobox::Clone()
/* ============================================================
	Function :		CDiagramCombobox::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramCombobox* obj = new CDiagramCombobox;
	obj->Copy( this );
	return obj;
}

void CDiagramCombobox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramCombobox::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	// COMBOBOX 그림!!
	//CStdGrfx::drawsunkenframed3dWindow( dc, rect );
   
	//LOGFONT lf;
	//GetFont( lf );
	//dc->SetBkMode( TRANSPARENT );
	//dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

	//CRect rectArrow( rect );
	//rectArrow.left = rectArrow.right - ( int ) ( ( double ) GetMinimumSize().cy * GetZoom() );
	//rectArrow.InflateRect( -2, -2 );

	//CStdGrfx::drawframed3dBox( dc, rectArrow );

	//CFont chk;
	//chk.CreateFont( lf.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	//dc->SelectObject( &chk );
	//dc->DrawText( "u", rectArrow, DT_VCENTER | DT_SINGLELINE | DT_CENTER );

	//dc->SelectStockObject( DEFAULT_GUI_FONT );

	// 테두리만 그림
	CStdGrfx::draw3dFrame( dc, rect );

	/**
	leon add
	* todo - 메모리DC 그리기
	*/
	CString strTemp;

	CRect tmp_rect(0, 0, rect.Width(), rect.Height());
	
	GetPropertyWithIndex(strTemp, CBX_BG_IMAGE_FULLPATH);

	//if( GetPicFullPath() != "" && !IsEqualPicPath() )
	if(m_pProperty[CBX_BG_IMAGE_FULLPATH] != "" && m_pProperty[CBX_BG_IMAGE_FULLPATH] != m_oldPicFullPath)
	{
		if(m_pProperty[CBX_BG_TILE] != STR_ZERO)
			MakeMemDC( dc, atoi(m_pProperty[CBX_BG_START_FONT]), atoi(m_pProperty[CBX_BG_TILE_IMAGE_NUM]));
		else
			MakeMemDC( dc, atoi(m_pProperty[CBX_BG_START_FONT]), WHOLE_IMAGE_STYLE_FONTNUM);

		SetOldPicFullPath(m_pProperty[CBX_BG_IMAGE_FULLPATH]);
	}

	if( m_pProperty[CBX_BG_IMAGE_FULLPATH] != "" )
	{
		// Window Style Control
		//CStdGrfx::drawframed3dBox( GetBufferDC(0), tmp_rect );
		//BitBltDS( GetBufferDC(0), GetBufferDC(2), tmp_rect.Width(), tmp_rect.Height(), SRCAND);//SRCAND);
		//BitBltDS( GetBufferDC(0), GetBufferDC(1), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT);

		//// Game Style Control
		//BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
		//BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		if(m_pProperty[CBX_BG_TILE] != STR_ZERO)
		{
			DrawTileImg(dc, SPR_PUTSPRT, 0, rect.left, rect.top, rect.Width(), rect.Height(), atoi(m_pProperty[CBX_BG_TILE_IMAGE_NUM]), true);
		}
		else
		{
			BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
			BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		}

	}
	//else
	LOGFONT lf;
	GetFont( lf );
	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

	CRect rectArrow( rect );
	rectArrow.left = rectArrow.right - ( int ) ( ( double ) GetMinimumSize().cy * GetZoom() );
	rectArrow.InflateRect( -2, -2 );

	CStdGrfx::drawframed3dBox( dc, rectArrow );

	CFont chk;
	chk.CreateFont( lf.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SelectObject( &chk );

	//int ret = atoi((char*)(LPCTSTR)m_pProperty[TEXT_STR]);

	//if( ret > 0 ) 
	//	dc->DrawText( GetTxtFromMgr( ret ), rect, DT_SINGLELINE );
	//else
		dc->DrawText( "u", rectArrow, DT_VCENTER | DT_SINGLELINE | DT_CENTER );

	dc->SelectStockObject( DEFAULT_GUI_FONT );
}

CDiagramEntity* CDiagramCombobox::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramCombobox::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramCombobox* obj = new CDiagramCombobox;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramCombobox::Export( UINT /*format*/ ) const
/* ============================================================
	Function :		CDiagramCombobox::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	CString input( "\t<select class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;' onchange='JavaScript:comboboxHandler(this)' name='%s'></select>" );
	CString str;
	CRect rect = GetRect();
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName() );

	return str;
}
