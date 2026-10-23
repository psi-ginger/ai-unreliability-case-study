#include "stdafx.h"
#include "DiagramButton.h"
#include "StdGrfx.h"
#include "DiagramEditor/DiagramEntityContainer.h"

#include "../MyDefine/MyDefine.h"
#include "./DiagramEditor/Tokenizer.h"

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
// CDiagramButton

//##############################################################################
// 전역 변수 선언.. 고유한 Button ID 생성에 사용
int sBTNUniqueNum = 1;
//##############################################################################

CDiagramButton::CDiagramButton()
{

	m_pProperty = NULL;
	m_pProperty = new CString[BTN_PROPERTY_COUNT];

	for(int i=0; i<BTN_PROPERTY_COUNT; i++)
	{
        m_pProperty[i].Empty();
	}

	SetPicFullPath("");
	SetOldPicFullPath("");
	SetPropertyDefault();

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( BTN_DEFAULT_TITLE );
	SetType( BTN_STR );
	SetName( BTN_STR );

	// 파일 로드시는 안 나타나고, 새로 객체 생성시만 셋팅!!
	m_pProperty[BTN_TEXT_STR] = BTN_DEFAULT_TITLE;
	
	SetPropertyDialog( &m_dlg, CButtonPropertyDlg::IDD );
}

CDiagramButton::~CDiagramButton()
{
	if(m_pProperty)
	{
		delete [] m_pProperty;
		m_pProperty = NULL;
	}

}

// sBTNUniqueNum : 결국 다음 ID와 연결될 번호를 뜻함
void CDiagramButton::IncreaseUniqueIDNum()
{
	CString tmpStr;

	if(m_pProperty != NULL)
	{
		do
		{
			tmpStr.Format(BASIC_ID_FORMAT, BTN_STR, sBTNUniqueNum++);
			
			// 생성시 값 수정!!
			m_pProperty[BTN_RESOURCE_ID] = tmpStr;

		}while(IsOverLapping(this, tmpStr));

	}
}
// 1. 파일로드시.. 없는 값 세팅을 위해서 사용됨
// 2. 새로 생성시.. 디폴트 값 세팅을 위해서 사용됨
void CDiagramButton::SetPropertyDefault()
{
	CString str;

	if(m_pProperty != NULL)
	{
		if(m_pProperty[BTN_KIND_NAME].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_KIND_NAME] = BTN_STR;
		// 자동 생성 로직 필요!!
		if(m_pProperty[BTN_RESOURCE_ID].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		{
			if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
				m_pProperty[BTN_RESOURCE_ID].Format(EX_ID_FORMAT, strDLGUniqueID, BTN_STR, sBTNUniqueNum);
			else
				m_pProperty[BTN_RESOURCE_ID].Format(BASIC_ID_FORMAT, BTN_STR, sBTNUniqueNum);
		}

		if(m_pProperty[BTN_X_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_X_COORD] = STR_ZERO;
		if(m_pProperty[BTN_Y_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_Y_COORD] = STR_ZERO;
		if(m_pProperty[BTN_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_WIDTH].Format("%d", BTN_SIZE_X);
		if(m_pProperty[BTN_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_HEIGHT].Format("%d", BTN_SIZE_Y);
		// Disable 속성으로 할 것!!
		if(m_pProperty[BTN_BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_BG_IMAGE_FULLPATH].Empty();
		if(m_pProperty[BTN_BG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_BG_WIDTH] = STR_ZERO;
		if(m_pProperty[BTN_BG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_BG_HEIGHT] = STR_ZERO;
		if(m_pProperty[BTN_BG_TILE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_BG_TILE] = STR_ZERO;

		if(m_pProperty[BTN_BG_TILE_IMAGE_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_BG_TILE_IMAGE_NUM] = STR_ZERO;
		if(m_pProperty[BTN_BG_START_FONT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_BG_START_FONT] = STR_ZERO;
		if(m_pProperty[BTN_TEXT_COLOR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_TEXT_COLOR].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[BTN_TEXT_COLOR_R].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_TEXT_COLOR_R].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[BTN_TEXT_COLOR_G].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_TEXT_COLOR_G].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[BTN_TEXT_COLOR_B].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_TEXT_COLOR_B].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[BTN_WEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_WEIGHT].Format("%d", DEFAULT_FONT_WEIGHT);

		if(m_pProperty[BTN_TEXT_STYLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_TEXT_STYLE] = DEFAULT_FONT_STYLE;
		if(m_pProperty[BTN_TEXT_SIZE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_TEXT_SIZE].Format("%d", DEFAULT_FONT_SIZE);
		if(m_pProperty[BTN_GROUP_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[BTN_GROUP_NUM] = STR_ZERO;

		if(m_pProperty[BTN_ANIMATION].Trim(WHITESPACE_N_TAB) == NULL_STRING)		// 애니메이션 속성 : 디폴트 0 <= Disable 애니메이션
			m_pProperty[BTN_ANIMATION] = STR_ZERO;

		//if(m_pProperty[BTN_TEXT_STR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		//	m_pProperty[BTN_TEXT_STR] = BTN_DEFAULT_TITLE;
	}
}

CDiagramEntity* CDiagramButton::Clone()
/* ============================================================
	Function :		CDiagramButton::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{

	CDiagramButton* obj = new CDiagramButton;
	obj->Copy( this );

	return obj;

}

void CDiagramButton::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramButton::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	// BUTTON 그림
	//CStdGrfx::drawframed3dBox( dc, rect );

	// 테두리만 그림
	CStdGrfx::draw3dFrame( dc, rect );

	// 메모리DC 그리기
	CString strTemp;

	CRect tmp_rect(0, 0, rect.Width(), rect.Height());
	
	GetPropertyWithIndex(strTemp, BTN_BG_IMAGE_FULLPATH);

	//if( GetPicFullPath() != "" && !IsEqualPicPath() )
	if(m_pProperty[BTN_BG_IMAGE_FULLPATH].Trim() != STR_EMPTY && m_pProperty[BTN_BG_IMAGE_FULLPATH] != m_oldPicFullPath)
	{
		if(m_pProperty[BTN_BG_TILE] != STR_ZERO)
			MakeMemDC( dc, atoi(m_pProperty[BTN_BG_START_FONT]), atoi(m_pProperty[BTN_BG_TILE_IMAGE_NUM]));
		else
			MakeMemDC( dc, atoi(m_pProperty[BTN_BG_START_FONT]), WHOLE_IMAGE_STYLE_FONTNUM);

		SetOldPicFullPath(m_pProperty[BTN_BG_IMAGE_FULLPATH]);
	}

	if( m_pProperty[BTN_BG_IMAGE_FULLPATH] != "" )
	{
		// Window Style Control
		//CStdGrfx::drawframed3dBox( GetBufferDC(0), tmp_rect );
		//BitBltDS( GetBufferDC(0), GetBufferDC(2), tmp_rect.Width(), tmp_rect.Height(), SRCAND);//SRCAND);
		//BitBltDS( GetBufferDC(0), GetBufferDC(1), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT);

		// Game Style Control
		// Non-Tile Style Control
		//BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
		//BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);

		if(m_pProperty[BTN_BG_TILE] != STR_ZERO)
		{
			DrawTileImg(dc, SPR_PUTSPRT, 0, rect.left, rect.top, rect.Width(), rect.Height(), atoi(m_pProperty[BTN_BG_TILE_IMAGE_NUM]), true);
			//BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
			//BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);

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
	//// MS Sans Serif will not scale below 8 pts.
	//if( GetZoom() < 1 )
	//	lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	lstrcpy(lf.lfFaceName, m_pProperty[BTN_TEXT_STYLE]);
	lf.lfHeight = atoi(m_pProperty[BTN_TEXT_SIZE]);

	font.CreateFontIndirect( &lf );
	dc->SelectObject( &font );

	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor(atoi(m_pProperty[BTN_TEXT_COLOR]));
	
	int ret = atoi((char*)(LPCTSTR)m_pProperty[BTN_TEXT_STR]);

	if( ret > 0 ) 
		dc->DrawText( GetTxtFromMgr( ret ), /*GetTitle()*/ rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	else
		dc->DrawText( m_pProperty[BTN_TEXT_STR], /*GetTitle()*/ rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	dc->SelectStockObject( ANSI_VAR_FONT );

}

CDiagramEntity* CDiagramButton::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramButton::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramButton* obj = new CDiagramButton;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;
}

CString CDiagramButton::Export( UINT /*format*/ ) const
/* ============================================================
	Function :		CDiagramButton::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	CString input( "\t<input class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;' type=button onclick='JavaScript:buttonHandler(this)' name='%s' value='%s'></input>" );
	CString str;
	CRect rect = GetRect();
	CString title = GetTitle();
	title.Replace( " ", "&nbsp;" );
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName(), title );

	return str;
}
