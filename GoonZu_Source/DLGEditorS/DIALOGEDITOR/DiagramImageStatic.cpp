#include "StdAfx.h"
#include ".\diagramimagestatic.h"
#include "StdGrfx.h"
#include "DiagramEditor/DiagramEntityContainer.h"

#include "../MyDefine/MyDefine.h"
#include "./DiagramEditor/Tokenizer.h"

#include "../TextMgr/TextMgr.h"
#include "../../NLib/NTextManager.h"

//cyj extern cltTextMgr g_pTextMgr;
extern NTextManager g_pTextManager;

//##############################################################################
// 전역 변수 선언.. 고유한 Button ID 생성에 사용
int sIMGSTTUniqueNum = 1;
//##############################################################################

CDiagramImageStatic::CDiagramImageStatic(void)
{
	m_pProperty = NULL;
	m_pProperty = new CString[IMGSTT_PROPERTY_COUNT];

	for(int i=0; i<IMGSTT_PROPERTY_COUNT; i++)
	{
        m_pProperty[i].Empty();
	}

	SetPropertyDefault();

	SetMinimumSize( CSize( 1, 1 ) );
	
	//SetTitle( IMGSTT_DEFAULT_TITLE );

	//SetType( IMGSTT_STR );
	//SetName( IMGSTT_STR );
	
	/**
	leon add
	* todo - 초기화
	*/
	SetPicFullPath("");
	SetOldPicFullPath("");

	SetPropertyDialog( &m_dlg, CImageStaticPropertyDlg::IDD );

}

CDiagramImageStatic::~CDiagramImageStatic(void)
{
	if(m_pProperty)
	{
		delete [] m_pProperty;
		m_pProperty = NULL;
	}
}

void CDiagramImageStatic::IncreaseUniqueIDNum()
{
	CString tmpStr;

	if(m_pProperty != NULL)
	{
		do
		{
			//if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
			//	tmpStr.Format(EX_ID_FORMAT, strDLGUniqueID, IMGSTT_STR, sIMGSTTUniqueNum++);
			//else
				tmpStr.Format(BASIC_ID_FORMAT, IMGSTT_STR, sIMGSTTUniqueNum++);
			
			// 생성시 값 수정!!
			m_pProperty[IMGSTT_RESOURCE_ID] = tmpStr;

		}while(IsOverLapping(this, tmpStr));

	}

	//// 정적 컨트롤 개수 증가
	//sIMGSTTUniqueNum++;
}

void CDiagramImageStatic::SetPropertyDefault()
{
	CString str;

	if(m_pProperty != NULL)
	{
		//for(int i=0; i<IMGSTT_PROPERTY_COUNT; i++)
		//	m_pProperty[i].Empty();

		if(m_pProperty[IMGSTT_KIND_NAME].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_KIND_NAME] = IMGSTT_STR;
		// 자동 생성 로직 필요!!
		if(m_pProperty[IMGSTT_RESOURCE_ID].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		{
			if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
				m_pProperty[IMGSTT_RESOURCE_ID].Format(EX_ID_FORMAT, strDLGUniqueID, IMGSTT_STR, sIMGSTTUniqueNum);
			else
				m_pProperty[IMGSTT_RESOURCE_ID].Format(BASIC_ID_FORMAT, IMGSTT_STR, sIMGSTTUniqueNum);
		}
		if(m_pProperty[IMGSTT_X_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_X_COORD] = STR_ZERO;
		if(m_pProperty[IMGSTT_Y_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_Y_COORD] = STR_ZERO;
		if(m_pProperty[IMGSTT_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_WIDTH].Format("%d", IMGSTT_SIZE_X);
		if(m_pProperty[IMGSTT_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_HEIGHT].Format("%d", IMGSTT_SIZE_Y);
		// Diable 속성으로 할 것!!
		if(m_pProperty[IMGSTT_BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_BG_IMAGE_FULLPATH].Empty();
		if(m_pProperty[IMGSTT_BG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_BG_WIDTH] = STR_ZERO;
		if(m_pProperty[IMGSTT_BG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_BG_HEIGHT] = STR_ZERO;
		if(m_pProperty[IMGSTT_BG_TILE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_BG_TILE] = STR_ZERO;

		if(m_pProperty[IMGSTT_BG_TILE_IMAGE_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_BG_TILE_IMAGE_NUM] = STR_ZERO;
		if(m_pProperty[IMGSTT_BG_START_FONT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_BG_START_FONT] = STR_ZERO;
		if(m_pProperty[IMGSTT_TEXT_COLOR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_TEXT_COLOR].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[IMGSTT_TEXT_COLOR_R].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_TEXT_COLOR_R].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[IMGSTT_TEXT_COLOR_G].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_TEXT_COLOR_G].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[IMGSTT_TEXT_COLOR_B].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_TEXT_COLOR_B].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[IMGSTT_WEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_WEIGHT].Format("%d", DEFAULT_FONT_WEIGHT);

		if(m_pProperty[IMGSTT_TEXT_STYLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_TEXT_STYLE] = DEFAULT_FONT_STYLE;
		if(m_pProperty[IMGSTT_TEXT_SIZE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_TEXT_SIZE].Format("%d", DEFAULT_FONT_SIZE);
		if(m_pProperty[IMGSTT_GROUP_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[IMGSTT_GROUP_NUM] = STR_ZERO;

		//if(m_pProperty[IMGSTT_TEXT_STR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		//	m_pProperty[IMGSTT_TEXT_STR] = IMGSTT_DEFAULT_TITLE;
	}
}

CDiagramEntity* CDiagramImageStatic::Clone()
/* ============================================================
	Function :		CDiagramImageStatic::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{

	CDiagramImageStatic* obj = new CDiagramImageStatic;
	obj->Copy( this );
	return obj;

}

void CDiagramImageStatic::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramImageStatic::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	// IMAGE_STATIC 그림
	//CStdGrfx::drawframed3dBox( dc, rect );

	// 테두리만 그림
	CStdGrfx::draw3dFrame( dc, rect );

	/**
	leon add
	* todo - 메모리DC 그리기
	*/
	CString strTemp;

	CRect tmp_rect(0, 0, rect.Width(), rect.Height());
	
	GetPropertyWithIndex(strTemp, IMGSTT_BG_IMAGE_FULLPATH);

	//if( GetPicFullPath() != "" && !IsEqualPicPath() )
	if(m_pProperty[IMGSTT_BG_IMAGE_FULLPATH] != "" && m_pProperty[IMGSTT_BG_IMAGE_FULLPATH] != m_oldPicFullPath)
	{
		if(m_pProperty[IMGSTT_BG_TILE] != STR_ZERO)
			MakeMemDC( dc, atoi(m_pProperty[IMGSTT_BG_START_FONT]), atoi(m_pProperty[IMGSTT_BG_TILE_IMAGE_NUM]));
		else
			MakeMemDC( dc, atoi(m_pProperty[IMGSTT_BG_START_FONT]), WHOLE_IMAGE_STYLE_FONTNUM);

		SetOldPicFullPath(m_pProperty[IMGSTT_BG_IMAGE_FULLPATH]);
	}

	if( m_pProperty[IMGSTT_BG_IMAGE_FULLPATH] != "" )
	{
		// Window Style Control
		//CStdGrfx::drawframed3dBox( GetBufferDC(0), tmp_rect );
		//BitBltDS( GetBufferDC(0), GetBufferDC(2), tmp_rect.Width(), tmp_rect.Height(), SRCAND);//SRCAND);
		//BitBltDS( GetBufferDC(0), GetBufferDC(1), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT);

		// Game Style Control
		//BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
		//BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		if(m_pProperty[IMGSTT_BG_TILE] != STR_ZERO)
		{
			DrawTileImg(dc, SPR_PUTSPRT, 0, rect.left, rect.top, rect.Width(), rect.Height(), atoi(m_pProperty[IMGSTT_BG_TILE_IMAGE_NUM]), true);
		}
		else
		{
			BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
			BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		}

	}
	//else
	//	CStdGrfx::draw3dFrame( dc, rect );
    
	//LOGFONT lf;
	//CFont font;

	//GetFont( lf );
	//// MS Sans Serif will not scale below 8 pts.
	//if( GetZoom() < 1 )
	//	lstrcpy( lf.lfFaceName, _T( "Arial" ) );

	//font.CreateFontIndirect( &lf );

	//dc->SelectObject( &font );

	//dc->SetBkMode( TRANSPARENT );
	//dc->DrawText( m_pProperty[IMGSTT_TEXT_STR], /*GetTitle()*/ rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	//dc->SelectStockObject( ANSI_VAR_FONT );

}

CDiagramEntity* CDiagramImageStatic::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramImageStatic::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramImageStatic* obj = new CDiagramImageStatic;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramImageStatic::Export( UINT /*format*/ ) const
/* ============================================================
	Function :		CDiagramImageStatic::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	CString input( "\t<input class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;' type=IMAGE_STATIC onclick='JavaScript:IMAGE_STATICHandler(this)' name='%s' value='%s'></input>" );
	CString str;
	CRect rect = GetRect();
	CString title = GetTitle();
	title.Replace( " ", "&nbsp;" );
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName(), title );

	return str;
}
