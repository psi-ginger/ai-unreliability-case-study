#include "StdAfx.h"
#include ".\diagramlistview.h"
#include "DiagramEditor/DiagramEntityContainer.h"

#include "../TextMgr/TextMgr.h"
#include "../../NLib/NTextManager.h"

//cyj extern cltTextMgr g_pTextMgr;
extern NTextManager g_pTextManager;

//##############################################################################
// 전역 변수 선언.. 고유한 ListView ID 생성에 사용
int sLSTVUniqueNum = 1;
//##############################################################################

CDiagramListView::CDiagramListView(void)
{
	m_pProperty = NULL;
	m_pProperty = new CString[LSTV_PROPERTY_COUNT];

	for(int i=0; i<LSTV_PROPERTY_COUNT; i++)
	{
        m_pProperty[i].Empty();
	}

	SetPropertyDefault();

	SetMinimumSize( CSize( 1, 1 ) );
	
	SetType( LSTV_STR );
	SetName( LSTV_STR );
	
	/**
	leon add
	* todo - 초기화
	*/
	SetPicFullPath("");
	SetOldPicFullPath("");

	SetPropertyDialog( &m_dlg, CListViewPropertyDlg::IDD );

}

CDiagramListView::~CDiagramListView(void)
{
	if(m_pProperty)
	{
		delete [] m_pProperty;
		m_pProperty = NULL;
	}

}

void CDiagramListView::IncreaseUniqueIDNum()
{
	CString tmpStr;

	if(m_pProperty != NULL)
	{
		do
		{
			//if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
			//	tmpStr.Format(EX_ID_FORMAT, strDLGUniqueID, LSTV_STR, sLSTVUniqueNum++);
			//else
				tmpStr.Format(BASIC_ID_FORMAT, LSTV_STR, sLSTVUniqueNum++);
			
			// 생성시 값 수정!!
			m_pProperty[LSTV_RESOURCE_ID] = tmpStr;

		}while(IsOverLapping(this, tmpStr));

	}
	//// 정적 컨트롤 개수 증가
	//sLSTVUniqueNum++;
}

void CDiagramListView::SetPropertyDefault()
{
	CString str;

	if(m_pProperty != NULL)
	{
		//for(int i=0; i<BTN_PROPERTY_COUNT; i++)
		//	m_pProperty[i].Empty();

		//############################   공통 속성   ###############################################

		if(m_pProperty[LSTV_KIND_NAME].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_KIND_NAME] = LSTV_STR;
		// 자동 생성 로직 필요!!
		if(m_pProperty[LSTV_RESOURCE_ID].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		{
			if(strDLGUniqueID.Trim(WHITESPACE_N_TAB) != STR_EMPTY)
				m_pProperty[LSTV_RESOURCE_ID].Format(EX_ID_FORMAT, strDLGUniqueID, LSTV_STR, sLSTVUniqueNum);
			else
				m_pProperty[LSTV_RESOURCE_ID].Format(BASIC_ID_FORMAT, LSTV_STR, sLSTVUniqueNum);
		}
		if(m_pProperty[LSTV_X_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_X_COORD] = STR_ZERO;
		if(m_pProperty[LSTV_Y_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_Y_COORD] = STR_ZERO;
		if(m_pProperty[LSTV_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_WIDTH].Format("%d", LSTV_SIZE_X);
		if(m_pProperty[LSTV_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_HEIGHT].Format("%d", LSTV_SIZE_Y);
		// Diable 속성으로 할 것!!
		if(m_pProperty[LSTV_BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_BG_IMAGE_FULLPATH].Empty();
		if(m_pProperty[LSTV_BG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_BG_WIDTH] = STR_ZERO;
		if(m_pProperty[LSTV_BG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_BG_HEIGHT] = STR_ZERO;
		if(m_pProperty[LSTV_BG_TILE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_BG_TILE] = STR_ZERO;

		if(m_pProperty[LSTV_BG_TILE_IMAGE_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_BG_TILE_IMAGE_NUM] = STR_ZERO;
		if(m_pProperty[LSTV_BG_START_FONT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_BG_START_FONT] = STR_ZERO;
		if(m_pProperty[LSTV_TEXT_COLOR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_TEXT_COLOR].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[LSTV_TEXT_COLOR_R].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_TEXT_COLOR_R].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[LSTV_TEXT_COLOR_G].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_TEXT_COLOR_G].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[LSTV_TEXT_COLOR_B].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_TEXT_COLOR_B].Format("%d", DEFAULT_FONT_COLOR);
		if(m_pProperty[LSTV_WEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_WEIGHT].Format("%d", DEFAULT_FONT_WEIGHT);


		if(m_pProperty[LSTV_TEXT_STYLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_TEXT_STYLE] = DEFAULT_FONT_STYLE;
		if(m_pProperty[LSTV_TEXT_SIZE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_TEXT_SIZE].Format("%d", DEFAULT_FONT_SIZE);
		if(m_pProperty[LSTV_GROUP_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_GROUP_NUM] = STR_ZERO;
		//#################################################################################
		//if(m_pProperty[LSTV_TEXT_STR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		//	m_pProperty[LSTV_TEXT_STR] = LSTV_DEFAULT_TITLE;

		if(m_pProperty[LSTV_ITEM_EMPTY].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_EMPTY] = STR_ONE;

		if(m_pProperty[LSTV_ITEM_COUNT_VISIBLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_COUNT_VISIBLE] = STR_ONE;

		if(m_pProperty[LSTV_ITEM_TOOLTIP_USABLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_TOOLTIP_USABLE] = STR_ONE;

		if(m_pProperty[LSTV_ITEM_TOOLTIP_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_TOOLTIP_WIDTH].Format("%d", LSTV_TOOLTIP_SIZE_WIDTH);

		if(m_pProperty[LSTV_ITEM_TOOLTIP_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_TOOLTIP_HEIGHT].Format("%d", LSTV_TOOLTIP_SIZE_HEIGHT);

		if(m_pProperty[LSTV_ITEM_START_X].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_START_X].Format("%d", LSTV_ITEM_START_POS_X);

		if(m_pProperty[LSTV_ITEM_START_Y].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_START_Y].Format("%d", LSTV_ITEM_START_POS_Y);

		if(m_pProperty[LSTV_ITEM_OFFSET_X].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_OFFSET_X].Format("%d", LSTV_ITEM_OFFSET_POS_X);

		if(m_pProperty[LSTV_ITEM_OFFSET_Y].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_OFFSET_Y].Format("%d", LSTV_ITEM_OFFSET_POS_Y);

		//####### 2005-02-21 속성 2개 추가 ##########################
		if(m_pProperty[LSTV_ITEM_SIZE_X].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_SIZE_X].Format("%d", LSTV_ITEM_DEFAULT_SIZE_X);

		if(m_pProperty[LSTV_ITEM_SIZE_Y].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_pProperty[LSTV_ITEM_SIZE_Y].Format("%d", LSTV_ITEM_DEFAULT_SIZE_Y);

	}
}

CDiagramEntity* CDiagramListView::Clone()
/* ============================================================
	Function :		CDiagramListView::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{

	CDiagramListView* obj = new CDiagramListView;
	obj->Copy( this );
	return obj;

}

void CDiagramListView::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramListView::Draw
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

	/**
	leon add
	* todo - 메모리DC 그리기
	*/
	CString strTemp;

	CRect tmp_rect(0, 0, rect.Width(), rect.Height());
	
	GetPropertyWithIndex(strTemp, LSTV_BG_IMAGE_FULLPATH);

	//if( GetPicFullPath() != "" && !IsEqualPicPath() )
	if(m_pProperty[LSTV_BG_IMAGE_FULLPATH] != "" && m_pProperty[LSTV_BG_IMAGE_FULLPATH] != m_oldPicFullPath)
	{
		if(m_pProperty[LSTV_BG_TILE] != STR_ZERO)
			MakeMemDC( dc, atoi(m_pProperty[LSTV_BG_START_FONT]), atoi(m_pProperty[LSTV_BG_TILE_IMAGE_NUM]));
		else
			MakeMemDC( dc, atoi(m_pProperty[LSTV_BG_START_FONT]), WHOLE_IMAGE_STYLE_FONTNUM);

		//SetOldPicFullPath(GetPicFullPath());
		SetOldPicFullPath(m_pProperty[LSTV_BG_IMAGE_FULLPATH]);
	}

	if( m_pProperty[LSTV_BG_IMAGE_FULLPATH] != "" )
	{
		// Window Style Control
		//CStdGrfx::drawframed3dBox( GetBufferDC(0), tmp_rect );
		//BitBltDS( GetBufferDC(0), GetBufferDC(2), tmp_rect.Width(), tmp_rect.Height(), SRCAND);//SRCAND);
		//BitBltDS( GetBufferDC(0), GetBufferDC(1), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT);

		// Game Style Control
		//BitBltDS( dc, GetBufferDC(MASK_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCAND, 0, 0, rect.left, rect.top);
		//BitBltDS( dc, GetBufferDC(REAL_BACKGROUND_NUM), tmp_rect.Width(), tmp_rect.Height(), SRCPAINT, 0, 0, rect.left, rect.top);
		if(m_pProperty[LSTV_BG_TILE] != STR_ZERO)
		{
			DrawTileImg(dc, SPR_PUTSPRT, 0, rect.left, rect.top, rect.Width(), rect.Height(), atoi(m_pProperty[LSTV_BG_TILE_IMAGE_NUM]), true);
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
	// MS Sans Serif will not scale below 8 pts.
	//if( GetZoom() < 1 )
	//	lstrcpy( lf.lfFaceName, _T( "Arial" ) );

	//font.CreateFontIndirect( &lf );

	//dc->SelectObject( &font );

	//dc->SetBkMode( TRANSPARENT );
	//dc->DrawText( m_pProperty[LSTV_TEXT_STR], /*GetTitle()*/ rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	//dc->SelectStockObject( ANSI_VAR_FONT );

}

CDiagramEntity* CDiagramListView::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramListView::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramListView* obj = new CDiagramListView;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramListView::Export( UINT /*format*/ ) const
/* ============================================================
	Function :		CDiagramListView::Export
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
