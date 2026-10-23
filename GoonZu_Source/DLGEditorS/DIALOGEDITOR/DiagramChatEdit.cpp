#include "../stdafx.h"
#include "DiagramChatEdit.h"
#include "StdGrfx.h"
#include "DiagramEditor/DiagramEntityContainer.h"

#include "../TextMgr/TextMgr.h"
#include "../../NLib/NTextManager.h"

//cyj extern cltTextMgr g_pTextMgr;
extern NTextManager g_pTextManager;


/////////////////////////////////////////////////////////////////////////////
// CDiagramChatEdit
//##############################################################################
// 전역 변수 선언.. 고유한 EditBox ID 생성에 사용
int sCHTETBUniqueNum = 1;
//##############################################################################

CDiagramChatEdit::CDiagramChatEdit()
/* ============================================================
	Function :		CDiagramChatEdit::CDiagramChatEdit
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	/**
	leon add
	* EDITBOX Property Creation
	*/
	m_pProperty = NULL;
	m_pProperty = new CString[CHTETB_PROPERTY_COUNT];

	for(int i=0; i<CHTETB_PROPERTY_COUNT; i++)
	{
        m_pProperty[i].Empty();
	}

	SetPropertyDefault();

	SetMinimumSize( CSize( 1, 1 ) );

	SetTitle(CHTETB_DEFAULT_TITLE);
	
	// 파일 로드시는 안 나타나고, 새로 객체 생성시만 셋팅!!
	m_pProperty[CHTETB_TEXT_STR] = CHTETB_DEFAULT_TITLE;

	SetType(CHTETB_STR);
	SetName( CHTETB_STR );

	SetPropertyDialog( &m_dlg, CChatEditPropertyDlg::IDD );

}

CDiagramChatEdit::~CDiagramChatEdit()
{
	if(m_pProperty)
	{
		delete [] m_pProperty;
		m_pProperty = NULL;
	}
}

void CDiagramChatEdit::IncreaseUniqueIDNum()
{
	CString tmpStr;

	if(m_pProperty != NULL)
	{
		do
		{
			//if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
			//	tmpStr.Format(EX_ID_FORMAT, strDLGUniqueID, CHTETB_STR, sCHTETBUniqueNum++);
			//else
				tmpStr.Format(BASIC_ID_FORMAT, CHTETB_STR, sCHTETBUniqueNum++);
			
			// 생성시 값 수정!!
			m_pProperty[CHTETB_RESOURCE_ID] = tmpStr;

		}while(IsOverLapping(this, tmpStr));

	}
}

void CDiagramChatEdit::SetPropertyDefault()
{
	if(m_pProperty != NULL)
	{
		//for(int i=0; i<CHTETB_PROPERTY_COUNT; i++)
		//	m_pProperty[i].Empty();

		if(m_pProperty[CHTETB_KIND_NAME].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_KIND_NAME] = CHTETB_STR;
		// 자동 생성 로직 필요!!
		if(m_pProperty[CHTETB_RESOURCE_ID].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		{
			if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
				m_pProperty[CHTETB_RESOURCE_ID].Format(EX_ID_FORMAT, strDLGUniqueID, CHTETB_STR, sCHTETBUniqueNum);
			else
				m_pProperty[CHTETB_RESOURCE_ID].Format(BASIC_ID_FORMAT, CHTETB_STR, sCHTETBUniqueNum);
		}
		if(m_pProperty[CHTETB_X_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_X_COORD] = STR_ZERO;
		if(m_pProperty[CHTETB_Y_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_Y_COORD] = STR_ZERO;
		if(m_pProperty[CHTETB_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_WIDTH].Format("%d", CHTETB_SIZE_X);
		if(m_pProperty[CHTETB_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_HEIGHT].Format("%d", CHTETB_SIZE_Y);
		// Diable 속성으로 할 것!!
		if(m_pProperty[CHTETB_BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_BG_IMAGE_FULLPATH].Empty();
		if(m_pProperty[CHTETB_BG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_BG_WIDTH] = STR_ZERO;
		if(m_pProperty[CHTETB_BG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_BG_HEIGHT] = STR_ZERO;
		if(m_pProperty[CHTETB_BG_TILE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_BG_TILE] = STR_ZERO;

		if(m_pProperty[CHTETB_BG_TILE_IMAGE_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_BG_TILE_IMAGE_NUM] = STR_ZERO;
		if(m_pProperty[CHTETB_BG_START_FONT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_BG_START_FONT] = STR_ZERO;
		if(m_pProperty[CHTETB_TEXT_COLOR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_TEXT_COLOR].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CHTETB_TEXT_COLOR_R].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_TEXT_COLOR_R].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CHTETB_TEXT_COLOR_G].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_TEXT_COLOR_G].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CHTETB_TEXT_COLOR_B].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_TEXT_COLOR_B].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[CHTETB_WEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_WEIGHT].Format("%d", DEFAULT_FONT_WEIGHT);

		if(m_pProperty[CHTETB_TEXT_STYLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_TEXT_STYLE] = DEFAULT_FONT_STYLE;
		if(m_pProperty[CHTETB_TEXT_SIZE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_TEXT_SIZE].Format("%d", DEFAULT_FONT_SIZE);
		if(m_pProperty[CHTETB_GROUP_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_GROUP_NUM] = STR_ZERO;

		if(m_pProperty[CHTETB_LINE_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_LINE_HEIGHT].Format("%d", CHTETB_LINE_HT);

		if(m_pProperty[CHTETB_AUTOSCROLL].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_AUTOSCROLL] = STR_ONE;

		if(m_pProperty[CHTETB_OFFSET_X].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_OFFSET_X].Format("%d", CHTETB_OFFSET_X_VALUE);
		
		if(m_pProperty[CHTETB_OFFSET_Y].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[CHTETB_OFFSET_Y].Format("%d", CHTETB_OFFSET_Y_VALUE);
	}
}

CDiagramEntity* CDiagramChatEdit::Clone()
/* ============================================================
	Function :		CDiagramChatEdit::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramChatEdit* obj = new CDiagramChatEdit;
	obj->Copy( this );
	return obj;
}

void CDiagramChatEdit::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramChatEdit::Draw
	Description :	
					
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	 //EditBox 그림
	//CStdGrfx::drawsunkenframed3dWindow( dc, rect );
	CStdGrfx::drawdoublesunken3dFrame( dc, rect );

	// 테두리만 그림
	//CStdGrfx::draw3dFrame( dc, rect );

	/**
	leon add
	* todo - 메모리DC 그리기
	*/
	CString strTemp;

	CRect tmp_rect(0, 0, rect.Width(), rect.Height());
	
	GetPropertyWithIndex(strTemp, CHTETB_BG_IMAGE_FULLPATH);

	//if( GetPicFullPath() != "" && !IsEqualPicPath() )
	if(m_pProperty[CHTETB_BG_IMAGE_FULLPATH] != "" && m_pProperty[CHTETB_BG_IMAGE_FULLPATH] != m_oldPicFullPath)
	{
		if(m_pProperty[CHTETB_BG_TILE] != STR_ZERO)
			MakeMemDC( dc, atoi(m_pProperty[CHTETB_BG_START_FONT]), atoi(m_pProperty[CHTETB_BG_TILE_IMAGE_NUM]));
		else
			MakeMemDC( dc, atoi(m_pProperty[CHTETB_BG_START_FONT]), WHOLE_IMAGE_STYLE_FONTNUM);

		SetOldPicFullPath(m_pProperty[CHTETB_BG_IMAGE_FULLPATH]);
	}

	if( m_pProperty[CHTETB_BG_IMAGE_FULLPATH] != "" )
	{
		// Window Style Control
		//CStdGrfx::drawframed3dBox( GetBufferDC(0), tmp_rect );
		//BitBltDS( GetBufferDC(0), GetBufferDC(2), tmp_rect.Width(), tmp_rect.Height(), SRCAND);//SRCAND);
		//BitBltDS( GetBufferDC(0), GetBufferDC(1), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT);

		// Game Style Control
		//BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
		//BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		if(m_pProperty[CHTETB_BG_TILE] != STR_ZERO)
		{
			DrawTileImg(dc, SPR_PUTSPRT, 0, rect.left, rect.top, rect.Width(), rect.Height(), atoi(m_pProperty[CHTETB_BG_TILE_IMAGE_NUM]), true);
		}
		else
		{
			BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
			BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		}

	}
	//else
	//	CStdGrfx::draw3dFrame( dc, rect );

	LOGFONT lf;
	CFont font;

	GetFont(lf);

	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T("Arial"));

	font.CreateFontIndirect(&lf);

	dc->SelectObject(&font);

	dc->SetBkMode(TRANSPARENT);

	int ret = atoi((char*)(LPCTSTR)m_pProperty[CHTETB_TEXT_STR]);

	if( ret > 0 ) 
		dc->DrawText( GetTxtFromMgr( ret ), rect, DT_LEFT | DT_TOP | DT_SINGLELINE );
	else
	    dc->DrawText( m_pProperty[CHTETB_TEXT_STR], rect, DT_LEFT | DT_TOP | DT_SINGLELINE );

	dc->SelectStockObject( ANSI_VAR_FONT );

}

CDiagramEntity* CDiagramChatEdit::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramChatEdit::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramChatEdit* obj = new CDiagramChatEdit;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramChatEdit::Export( UINT /*format*/ ) const
/* ============================================================
	Function :		CDiagramChatEdit::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	CString input( "\t<input class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;' type=text name='%s'></input>" );
	CString str;
	CRect rect = GetRect();
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName() );
	return str;
}

