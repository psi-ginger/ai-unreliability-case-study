#include "stdafx.h"
#include "DiagramEntityContainer.h"
#include "DiagramEntity.h"
#include "Tokenizer.h"
#include "GroupFactory.h"
#include "../DLGDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4706 )

//##############################################################################
// 전역 변수 선언.. 고유한 Dialog ID 생성에 사용
int sDLGUniqueNum = 1;
CString strDLGUniqueID;
//##############################################################################

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer construction/destruction/initialization

CDiagramEntityContainer::CDiagramEntityContainer( CDiagramClipboardHandler* clip )
/* ============================================================
	Function :		CDiagramEntityContainer::CDiagramEntityContainer
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	m_clip = clip;

	// DIALOG DEFAULT 셋팅
	SetDLGDefault(true);

	SetUndoStackSize( 0 );
	//Clear();
	//SetVirtualSize( CSize( 0, 0 ) );

	if(m_DLG_Property != NULL)
		SetVirtualSize( CSize( _ttoi(m_DLG_Property[DLG_WIDTH]) , _ttoi(m_DLG_Property[DLG_HEIGHT]) ) );
	else
        SetVirtualSize( CSize( 0, 0 ) );

	m_defaultImagePath = GOONZU_DEFAULT_ROOT_PATH;
	m_defaultTxtPath = GOONZU_DEFAULT_TEXTMGR_PATH;
}

CDiagramEntityContainer::~CDiagramEntityContainer()
/* ============================================================
	Function :		CDiagramEntityContainer::~CDiagramEntityContainer
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	Clear();

}


void CDiagramEntityContainer::SetDLGDefault(bool mustBe)
{
	if(m_DLG_Property != NULL)
	{
		//for(int i=0; i<DLG_PROPERTY_COUNT; i++)
		//	m_DLG_Property[i].Empty();

		if( mustBe || m_DLG_Property[DLG_KIND_NAME].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_KIND_NAME] = DLG_PAPER_STR;
		// 고유한 리소스 id 생성
		if( mustBe || m_DLG_Property[DLG_RESOURCE_ID].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		{
			m_DLG_Property[DLG_RESOURCE_ID].Format(BASIC_ID_FORMAT, DLG_PAPER_STR, sDLGUniqueNum);
			strDLGUniqueID = m_DLG_Property[DLG_RESOURCE_ID];
		}

		if( mustBe || m_DLG_Property[DLG_X_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_X_COORD] = STR_ZERO;
		if( mustBe || m_DLG_Property[DLG_Y_COORD].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_Y_COORD] = STR_ZERO;
		if( mustBe || m_DLG_Property[DLG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_WIDTH].Format("%d", DLG_SIZE_X);
		if( mustBe || m_DLG_Property[DLG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_HEIGHT].Format("%d", DLG_SIZE_Y);
			
		if( mustBe || m_DLG_Property[DLG_BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_BG_IMAGE_FULLPATH].Empty();
		if( mustBe || m_DLG_Property[DLG_BG_WIDTH].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_BG_WIDTH].Format("%d", DLG_SIZE_X);
		if( mustBe || m_DLG_Property[DLG_BG_HEIGHT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_BG_HEIGHT].Format("%d", DLG_SIZE_Y);
		if( mustBe || m_DLG_Property[DLG_BG_TILE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_BG_TILE] = STR_ZERO;

		if( mustBe || m_DLG_Property[DLG_BG_TILE_IMAGE_NUM].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_BG_TILE_IMAGE_NUM] = STR_ZERO;
		if( mustBe || m_DLG_Property[DLG_BG_START_FONT].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_BG_START_FONT] = STR_ZERO;
		if( mustBe || m_DLG_Property[DLG_TEXT_COLOR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_TEXT_COLOR].Format("%d", DEFAULT_FONT_COLOR);
		if( mustBe || m_DLG_Property[DLG_TEXT_STYLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_TEXT_STYLE] = DEFAULT_FONT_STYLE;
		if( mustBe || m_DLG_Property[DLG_TEXT_SIZE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_TEXT_SIZE].Format("%d", DEFAULT_FONT_SIZE);

		//if( mustBe || m_DLG_Property[DLG_CALLBACK_POINTER].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		//	m_DLG_Property[DLG_CALLBACK_POINTER].Empty();
		if( mustBe || m_DLG_Property[DLG_TITLE_STR].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_TITLE_STR] = DLG_DEFAULT_TITLE;
		if( mustBe || m_DLG_Property[DLG_TITLE_VISIBLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_TITLE_VISIBLE] = STR_ONE;
		if( mustBe || m_DLG_Property[DLG_MOVABLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_MOVABLE] = STR_ONE;
		if( mustBe || m_DLG_Property[DLG_CLOSABLE].Trim(WHITESPACE_N_TAB) == NULL_STRING)
			m_DLG_Property[DLG_CLOSABLE] = STR_ONE;

		//// 자식 창 파일명 추가 (10개까지..)
		//if( mustBe || m_DLG_Property[DLG_CHILD_FILENAMES].Trim(WHITESPACE_N_TAB) == NULL_STRING)
		//	m_DLG_Property[DLG_CHILD_FILENAMES] = DLG_DEFAULT_CHILD_FILENAMES;
	}
}

/**
leon add
* Base Dialog Window의 크기는 
* 초기화 하지 않는다..
*/
void CDiagramEntityContainer::Clear()
/* ============================================================
	Function :		CDiagramEntityContainer::Clear
	Description :	Removes all data from the data and undo.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to remove data from the container. The 
					Paste-array will be kept.

   ============================================================*/
{
	RemoveAll();
	ClearUndo();
	SetModified( FALSE );

	m_spr.ClearImageData();
	m_BGFullPath.Empty();

	SetDLGDefault(true);

	// 정적 고유 번호 증가
	sDLGUniqueNum++;
}

// Default Root File Path를 멤버변수에 저장
void CDiagramEntityContainer::SetDefaultFilePath(CString rootpath)
{
	rootpath.Trim(WHITESPACE_N_TAB);

	int length = rootpath.GetLength();

	// 무결성 검사
	// 디폴트 루트 패스 무결성 검사!!
	if(rootpath != STR_EMPTY)
	{
		if(
			(rootpath[length-1] != FILE_PATH_SPLITTER_BACKSLASH && rootpath[length-1] != FILE_PATH_SPLITTER_SLASH) 
			&& (rootpath[length-2] != FILE_PATH_SPLITTER_BACKSLASH && rootpath[length-2] != FILE_PATH_SPLITTER_SLASH) 
			&& (rootpath.Right(0) != FILE_PATH_SPLITTER_BACKSLASH && rootpath.Right(0) != FILE_PATH_SPLITTER_SLASH) 
			&& (rootpath.Right(1) != FILE_PATH_SPLITTER_BACKSLASH && rootpath.Right(1) != FILE_PATH_SPLITTER_SLASH)
			&& (rootpath.Right(2) != FILE_PATH_SPLITTER_BACKSLASH && rootpath.Right(2) != FILE_PATH_SPLITTER_SLASH) )
		{
			if(rootpath.Find(FILE_PATH_SPLITTER_BACKSLASH) > -1)
				rootpath += FILE_PATH_SPLITTER_BACKSLASH;
			else
                rootpath += FILE_PATH_SPLITTER_SLASH;
		}
	}

    m_defaultImagePath = rootpath;	
}

// Default Txt Path를 멤버변수에 저장
void CDiagramEntityContainer::SetDefaultTxtFilePath(CString rootpath)
{
	rootpath.Trim(WHITESPACE_N_TAB);

	//// 무결성 검사
	//// 디폴트 루트 패스 무결성 검사!!
	//if(rootpath != STR_EMPTY)
	//{
	//	if(rootpath.Right(1) != FILE_PATH_SPLITTER_BACKSLASH && rootpath.Right(1) != FILE_PATH_SPLITTER_SLASH)
	//	{
	//		if(rootpath.Find(FILE_PATH_SPLITTER_BACKSLASH) > -1)
	//			rootpath += FILE_PATH_SPLITTER_BACKSLASH;
	//		else
 //               rootpath += FILE_PATH_SPLITTER_SLASH;
	//	}
	//}

    m_defaultTxtPath = rootpath;	
}

// Default Root File Path를 멤버변수에서 얻어옴
CString CDiagramEntityContainer::GetDefaultFilePath()
{
    return m_defaultImagePath;
}

// Default Txt Path
CString CDiagramEntityContainer::GetDefaultTxtFilePath()
{
    return m_defaultTxtPath;
}

// fullPath에서 Default Root File Path를 제거하고 남은 스트링만 리턴함
// 각 컨트롤 배경 이미지 상대경로 만드는 곳은
// CDiagramEntity에 GetFormatString 함수에서 처리함
CString CDiagramEntityContainer::DeleteDefaultFilePath(CString fullPath)
{
	CString tmpStr, tmpStr2;
	int length = 0;

    m_defaultImagePath.Trim();
	length = m_defaultImagePath.GetLength();

	if(length < 1)
		return tmpStr;

    if(m_defaultImagePath[length-1] != FILE_PATH_SPLITTER_SLASH && m_defaultImagePath[length-1] != FILE_PATH_SPLITTER_BACKSLASH)
		m_defaultImagePath = m_defaultImagePath + FILE_PATH_SPLITTER_SLASH;

	tmpStr = fullPath.MakeLower();
	tmpStr2 = m_defaultImagePath.MakeLower();

	tmpStr.Replace( tmpStr2, STR_EMPTY);

	return tmpStr;
}

/**
leon alter
* paper property 추가
* 파일로 저장시 호출됨
*/
CString CDiagramEntityContainer::GetString() //const
/* ============================================================
	Function :		CDiagramEntityContainer::GetString
	Description :	Returns a string representation of the 
					virtual paper size
	Access :		Public

	Return :		CString	-	Resulting string
	Parameters :	none

	Usage :			Call to get a string representing the paper 
					size of the container. The format is 
					"paper:x,y;" where "x" and "y" are the 
					horisontal and vertical sizes.

   ============================================================*/
{

	CString str;
	CString headerSplitter;

	headerSplitter = DLG_PAPER_STR;
	headerSplitter += DLG_HEADER_SPLITTER_STR;
	//headerSplitter += "%i,%i,%s;";

	//headerSplitter += "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s";
	//headerSplitter += DLG_ENDLINE_STR;

	//str.Format( _T( "paper:%i,%i;" ), GetVirtualSize().cx, GetVirtualSize().cy );
	//str.Format( _T( headerSplitter ), GetVirtualSize().cx, GetVirtualSize().cy, GetBGPicFullPath() );

	// 배경 이미지 경로에서 백슬래쉬 => 슬래쉬로 바꿈
	m_DLG_Property[DLG_BG_IMAGE_FULLPATH].Replace( FILE_PATH_SPLITTER_BACKSLASH, FILE_PATH_SPLITTER_SLASH);
	/**
	leon add 
	* 전체 경로에서 default root path 제거
	*/
	for(int i=0; i<DLG_PROPERTY_COUNT; i++)
	{
		if( i != DLG_BG_IMAGE_FULLPATH)
			headerSplitter += m_DLG_Property[i];
		else
			headerSplitter += DeleteDefaultFilePath(m_DLG_Property[DLG_BG_IMAGE_FULLPATH]);

		if(i < DLG_PROPERTY_COUNT -1)
			headerSplitter += DLG_ITEM_SPLITTER_STR;
	}

	headerSplitter += DLG_ENDLINE_STR;

	str = headerSplitter;

	//str.Format( _T( headerSplitter ), m_DLG_Property[DLG_KIND_NAME], m_DLG_Property[DLG_RESOURCE_ID], m_DLG_Property[DLG_X_COORD], m_DLG_Property[DLG_Y_COORD], m_DLG_Property[DLG_WIDTH],
	//	m_DLG_Property[DLG_HEIGHT], DeleteDefaultFilePath(m_DLG_Property[DLG_BG_IMAGE_FULLPATH]), m_DLG_Property[DLG_BG_WIDTH], m_DLG_Property[DLG_BG_HEIGHT], m_DLG_Property[DLG_BG_TILE], m_DLG_Property[DLG_BG_START_FONT], m_DLG_Property[DLG_CALLBACK_POINTER],
	//	m_DLG_Property[DLG_TITLE_STR], m_DLG_Property[DLG_TITLE_VISIBLE], m_DLG_Property[DLG_MOVABLE], m_DLG_Property[DLG_CLOSABLE]);

	return str;
}

/**
leon add
* Dialog Header 정보를 읽어서 처리함
*/
BOOL CDiagramEntityContainer::FromString( const CString& str )
/* ============================================================
	Function :		CDiagramEntityContainer::FromString
	Description :	Sets the virtual paper size from a string.
	Access :		Public

	Return :		BOOL				-	"TRUE" if the string 
											represented a 
											paper.
	Parameters :	const CString& str	-	The string 
											representation.
					
	Usage :			Call to set the paper size of the container 
					from a string. The format is "paper:x,y;" 
					where "x" and "y" are the horisontal and 
					vertical sizes.

   ============================================================*/
{

	BOOL result = FALSE;

	CTokenizer main( str, _T( DLG_HEADER_SPLITTER_STR ) );
	

	CString header;
	CString data;
	CString paperBKImage;
	CString tmpStr;

	if( main.GetSize() == 2 )
	{
		main.GetAt( 0, header );
		main.GetAt( 1, data );
		header.TrimLeft();
		header.TrimRight();
		data.TrimLeft();
		data.TrimRight();

		// 파일 로드 후 header 처리
		if( header == _T( DLG_PAPER_STR ) )
		{
			CTokenizer tok( data.Left( data.GetLength() - 1 ) );
			int size = tok.GetSize();

			// 파일에 자식 창 파일명 10개가 추가되므로..
			if( size == DLG_FILE_PROPERTY_COUNT/*DLG_PROPERTY_COUNT*/ )
			{
				// 자식창 파일 이름 저장(총 10개)
				for(int i=0; i<DLG_FILE_PROPERTY_COUNT; i++)
				{
					if(i < DLG_CHILD_FILENAMES || i >= DLG_CHILD_FILENAMES + DLG_CHILD_FILENUM)
						tok.GetAt(i, m_DLG_Property[i]);
					else
					{
						// 생성자에서 초기화한 m_DLG_Property[DLG_CHILD_FILENAMES] 변수의 값에 덧붙여 쓰지 않도록 초기화함
						if(i == DLG_CHILD_FILENAMES && m_DLG_Property[DLG_CHILD_FILENAMES].Trim(WHITESPACE_N_TAB) != STR_EMPTY)
							m_DLG_Property[DLG_CHILD_FILENAMES].Empty();

						tok.GetAt(i, tmpStr);
                        m_DLG_Property[DLG_CHILD_FILENAMES] += tmpStr;

						if( i < DLG_CHILD_FILENAMES + DLG_CHILD_FILENUM -1)
							m_DLG_Property[DLG_CHILD_FILENAMES] += DLG_CHILD_FILENAME_SPLITTER_CHAR;
					}                        
				}

				// 파일에서 읽어온 경로와 Default Path를 붙여서 전체 경로를 만듦
				// 파일명이 없는 null path는 건너뜀
				if(m_DLG_Property[BG_IMAGE_FULLPATH].Trim(WHITESPACE_N_TAB) != STR_EMPTY)
					m_DLG_Property[BG_IMAGE_FULLPATH] = GetDefaultFilePath() + m_DLG_Property[BG_IMAGE_FULLPATH];
			
				SetVirtualSize( CSize( _ttoi(m_DLG_Property[DLG_WIDTH]), _ttoi(m_DLG_Property[DLG_HEIGHT])), true );

				// 다이얼로그 에디터에서 쓸 배경 이미지 데이터를 읽음
				if(ReadSprBG(m_DLG_Property[DLG_BG_IMAGE_FULLPATH]))
				{
					// 파일에 배경 이미지 폭과 높이가 셋팅 안되어있을 경우 만듦
					if(m_DLG_Property[DLG_BG_WIDTH].Trim(DLG_FILE_TRIM_LETTER) == STR_EMPTY)
						m_DLG_Property[DLG_BG_WIDTH].Format("%d", m_spr.clHeader.siXsize);

					if(m_DLG_Property[DLG_BG_HEIGHT].Trim(DLG_FILE_TRIM_LETTER) == STR_EMPTY)
						m_DLG_Property[DLG_BG_HEIGHT].Format("%d", m_spr.clHeader.siYsize);

					result = TRUE;
				}
				else
					result = FALSE;

				// 파일에서 읽은 후 없는 값을 디폴트 값으로 셋팅함
				SetDLGDefault();
			}
		}
	}

	return result;

}

void CDiagramEntityContainer::Export( CStringArray& stra, UINT format ) const
/* ============================================================
	Function :		CDiagramEntityContainer::Export
	Description :	Exports all objects to format format.
	Access :		Public

	Return :		void
	Parameters :	CStringArray& stra	-	"CStingArray" that 
											will be filled with 
											data on return. 
					UINT format			-	Format to save to.
					
	Usage :			Call to export the contents of the container 
					to a "CStringArray". "Export" will - of course - 
					have to be defined for the derived objects.

   ============================================================*/
{

	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CDiagramEntity* obj = GetAt( t );
		stra.Add( obj->Export( format ) );
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer data access

int CDiagramEntityContainer::GetSize() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetSize
	Description :	Returns the number of objects in the data
					container.
	Access :		Public

	Return :		int		-	The number of objects.
	Parameters :	none

	Usage :			Call to get the number of objects currently 
					in the data array of the container.

   ============================================================*/
{

	return m_objs.GetSize();

}

void CDiagramEntityContainer::Add( CDiagramEntity* obj, BOOL fromFile, int insertAt )
/* ============================================================
	Function :		CDiagramEntityContainer::Add
	Description :	Add an object to the data.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to add.
					
	Usage :			Call to add a new object to the container.

   ============================================================*/
{

	obj->SetParent( this );

	// 새객체 생성시 레이어 위치 지정이 필요하다면 중간에 삽입하여 뒤로 밀기
	if( insertAt < 0 )
		m_objs.Add( obj );
	else
		m_objs.InsertAt( insertAt, obj );

	/**
	leon add
	* 각 객체 unique id num 증가
	*/
	if(!fromFile)
		obj->IncreaseUniqueIDNum();

	SetModified( TRUE );

}

CDiagramEntity* CDiagramEntityContainer::GetAt( int index ) const
/* ============================================================
	Function :		CDiagramEntityContainer::GetAt
	Description :	Gets the object at position "index".
	Access :		Public

	Return :		CDiagramEntity*	-	The object or "NULL" if 
										out of range.
	Parameters :	int index		-	The index to get data 
										from
					
	Usage :			Call to get a specific object from the 
					container.

   ============================================================*/
{

	CDiagramEntity* result = NULL;
	if( index < m_objs.GetSize() && index >= 0 )
		result = static_cast< CDiagramEntity* >( m_objs.GetAt( index ) );
	return result;

}

void CDiagramEntityContainer::SetAt( int index, CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::SetAt
	Description :	Sets an object at position "index".
	Access :		Public

	Return :		void
	Parameters :	int index			-	Index to set data 
											at.
					CDiagramEntity* obj	-	Object to set.
					
	Usage :			Internal function. Used by "Swap".

   ============================================================*/
{

	m_objs.SetAt( index, obj );
	SetModified( TRUE );

}

void CDiagramEntityContainer::RemoveAt( int index )
/* ============================================================
	Function :		CDiagramEntityContainer::RemoveAt
	Description :	Removes the object at index.
	Access :		Public

	Return :		void
	Parameters :	int index	-	The index of the object 
									to remove.
					
	Usage :			Call to remove a specific object. Memory is 
					freed.

   ============================================================*/
{

	CDiagramEntity* obj = GetAt( index );
	if( obj )
	{
		delete obj;
		m_objs.RemoveAt( index );
		SetModified( TRUE );
	}

}

void CDiagramEntityContainer::RemoveAll()
/* ============================================================
	Function :		CDiagramEntityContainer::RemoveAll
	Description :	Removes all data objects
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to remove all data objects in the 
					container. Undo- and paste arrays are not 
					emptied.
					Allocated memory is released. Undo and 
					paste not deleted.

   ============================================================*/
{

	int max = m_objs.GetSize();
	if( max )
	{

		for( int t = 0 ; t < max ; t++ )
		{
			CDiagramEntity* obj = static_cast< CDiagramEntity* >( m_objs.GetAt( t ) );
			delete obj;
		}

		m_objs.RemoveAll();
		SetModified( TRUE );

	}

}

void CDiagramEntityContainer::RemoveAllSelected()
/* ============================================================
	Function :		CDiagramEntityContainer::RemoveAllSelected
	Description :	Removes all selected objects
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to remove all selected objects from the 
					container. Releases allocated data

   ============================================================*/
{

	int max = m_objs.GetSize() - 1;
	for( int t = max ; t >= 0 ; t-- )
		if( GetAt( t )->IsSelected() )
			RemoveAt( t );

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer property access

/**
leon add
* 객체이미지가 있을 경우 그것의 가로 세로 혹은 세로 폭에 맞춰 최소 공배수 크기로 맞출 때 사용!!
* original_val	: 변환할 값
* ref_multiple	: 배수 값
* return 값		: 최대 공배수 값
*/
int CDiagramEntityContainer::SetToMultipleSize(double origin_val, double ref_multiple)
{
	div_t result;

    if(origin_val < 1 || ref_multiple < 1)
		return origin_val;

	if(origin_val <= ref_multiple)
		return origin_val;

	result = div(origin_val, ref_multiple);

	return result.quot * ref_multiple;
}

void CDiagramEntityContainer::SetVirtualSize( CSize size, bool bIsLoading, int nModeDetail )
/* ============================================================
	Function :		CDiagramEntityContainer::SetVirtualSize
	Description :	Sets the current virtual paper size.
	Access :		Public

	Return :		void
	Parameters :	CSize size	-	The size to set
					
	Usage :			Call to set the paper size. Note that 
					"SetModified( TRUE )" might have to be called 
					as well.

   ============================================================*/
{
	if(m_DLG_Property != NULL)
	{
		//m_DLG_Property[DLG_WIDTH].Format("%i", size.cx);
		//m_DLG_Property[DLG_HEIGHT].Format("%i", size.cy);

		int mul_width = size.cx, mul_height = size.cy;
		/**
		leon add
		* 크기를 이미지 가로 혹은 세로 폭에 맞춰서 계산함
		*/
	if( nModeDetail == 5 || nModeDetail == 4 ) // Shift Key를 누르고 있을 때만 아래 적용
	{
		if(m_DLG_Property != NULL && m_DLG_Property[BG_IMAGE_FULLPATH].Trim(DLG_FILE_TRIM_LETTER) != STR_EMPTY
			&& m_DLG_Property[BG_WIDTH].Trim(DLG_FILE_TRIM_LETTER) != STR_EMPTY && m_DLG_Property[BG_WIDTH].Trim(DLG_FILE_TRIM_LETTER) != STR_ZERO)
				mul_width = SetToMultipleSize(size.cx, m_spr.clHeader.siXsize/*_tstof(m_DLG_Property[BG_WIDTH])*/);

		if(m_DLG_Property != NULL && m_DLG_Property[BG_IMAGE_FULLPATH].Trim(DLG_FILE_TRIM_LETTER) != STR_EMPTY
			&& m_DLG_Property[BG_HEIGHT].Trim(DLG_FILE_TRIM_LETTER) != STR_EMPTY && m_DLG_Property[BG_HEIGHT].Trim(DLG_FILE_TRIM_LETTER) != STR_ZERO)
				mul_height = SetToMultipleSize(size.cy, m_spr.clHeader.siYsize/*_tstof(m_DLG_Property[BG_HEIGHT])*/);
	}

		m_DLG_Property[DLG_WIDTH].Format("%i", mul_width);
		m_DLG_Property[DLG_HEIGHT].Format("%i", mul_height);

		if(!bIsLoading)
		{
			//if(mul_width > _ttoi(m_DLG_Property[DLG_BG_WIDTH]))
			{
				m_DLG_Property[DLG_BG_WIDTH].Format("%i", (int)mul_width);
			}
			
			//if(mul_height > _ttoi(m_DLG_Property[DLG_BG_HEIGHT]))
			{
				m_DLG_Property[DLG_BG_HEIGHT].Format("%i", (int)mul_height);
			}
		}
	}
	//else
	//{
	//	m_virtualSize.cx = 0;
	//	m_virtualSize.cy = 0;
	//}

	m_virtualSize = size;

}

CSize CDiagramEntityContainer::GetVirtualSize() //const
/* ============================================================
	Function :		CDiagramEntityContainer::GetVirtualSize
	Description :	Gets the virtual paper size.
	Access :		Public

	Return :		CSize	-	The current size
	Parameters :	none

	Usage :			Call to get the current paper size.

   ============================================================*/
{
	if(m_DLG_Property != NULL)
	{
        m_virtualSize.cx = _ttoi(m_DLG_Property[DLG_WIDTH]);
		m_virtualSize.cy = _ttoi(m_DLG_Property[DLG_HEIGHT]);
	}
	//else
	//{
 //       m_virtualSize.cx = 0;
	//	m_virtualSize.cy = 0;
	//}

	return m_virtualSize;
}

BOOL CDiagramEntityContainer::IsModified() const
/* ============================================================
	Function :		CDiagramEntityContainer::IsModified
	Description :	Returns the state of the modified-flag.
	Access :		Public

	Return :		BOOL	-	"TRUE" if data is changed
	Parameters :	none

	Usage :			Call to see if data is modified.

   ============================================================*/
{

	return m_dirty;

}

void CDiagramEntityContainer::SetModified( BOOL dirty )
/* ============================================================
	Function :		CDiagramEntityContainer::SetModified
	Description :	Sets the state of the modified flag
	Access :		Public

	Return :		void
	Parameters :	BOOL dirty	-	"TRUE" if data is changed.
					
	Usage :			Call to mark the data as modified.

   ============================================================*/
{

	m_dirty = dirty;

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer single object handlers

void CDiagramEntityContainer::Remove( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Remove
	Description :	Removes the object.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to 
											remove.
					
	Usage :			Call to remove "obj" - if it exists - from the 
					container. Allocated memory is released.

   ============================================================*/
{

	int index = Find( obj );
	if( index != -1 )
		RemoveAt( index );

}

void CDiagramEntityContainer::Duplicate( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Duplicate
	Description :	Duplicates the object and adds the new 
					one 10 pixels offset down and right.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to 
											duplicate.	
					
	Usage :			Call to create a copy of the selected 
					element.

   ============================================================*/
{

	int index = Find( obj );
	if( index != -1 )
	{
		CDiagramEntity* newobj = obj->Clone();
		newobj->SetRect( newobj->GetLeft() + 10, newobj->GetTop() + 10, newobj->GetRight() + 10, newobj->GetBottom() + 10 );
		Add( newobj );
	}

}

void CDiagramEntityContainer::Cut( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Cut
	Description :	Cuts out the object and puts it into the 
					'clipboard'
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to cut.
					
	Usage :			Call in response to a Cut-command. See also 
					the functions for copy/paste below.

   ============================================================*/
{

	Copy( obj );
	Remove( obj );

}

void CDiagramEntityContainer::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Copy
	Description :	Copies the object to the 'clipboard'.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to copy.	
					
	Usage :			Call in response to a Copy-command. Note 
					that obj will only be copied to the 
					clipboard, not the screen. See also the 
					functions for copy/paste below.

   ============================================================*/
{

	ASSERT( obj );

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	if( obj )
		m_clip->Copy( obj );

}

void CDiagramEntityContainer::Up( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Up
	Description :	Moves the object one step up in the z-
					order.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.	
					
	Usage :			Call to move "obj" in the z-order.

   ============================================================*/
{

	int index = Find( obj );
	Swap( index, index + 1);

}

void CDiagramEntityContainer::Down( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Down
	Description :	Moves the object one step down in the z-
					order.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.	
					
	Usage :			Call to move "obj" in the z-order.

   ============================================================*/
{

	int index = Find( obj );
	Swap( index, index - 1);

}

void CDiagramEntityContainer::Front( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Front
	Description :	Moves "obj" to the top of the z-order.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.	
					
	Usage :			Call to move "obj" in the z-order.

   ============================================================*/
{

	int index = Find( obj );
	m_objs.RemoveAt( index );
	m_objs.Add( obj );
	SetModified( TRUE );

}

void CDiagramEntityContainer::Bottom( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Bottom
	Description :	Moves "obj" to the bottom of the z-order.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.
					
	Usage :			Call to move "obj" in the z-order.

   ============================================================*/
{

	int index = Find( obj );
	m_objs.RemoveAt( index );
	m_objs.InsertAt( 0, obj );
	SetModified( TRUE );

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer copy/paste is implemented as separate class.

void CDiagramEntityContainer::SetClipboardHandler( CDiagramClipboardHandler* clip )
/* ============================================================
	Function :		CDiagramEntityContainer::SetClipboardHandler
	Description :	Sets the container clipboard class.
	Access :		Public

	Return :		void
	Parameters :	CDiagramClipboardHandler* clip	-	A pointer
														to the
														class
					
	Usage :			Call to set the clipboard handler for this 
					container. The same clipboard handler 
					instance can be used for several containers 
					to allow several editors (in an MDI-
					application) to share the same clipboard.

   ============================================================*/
{

	m_clip = clip;

}

CDiagramClipboardHandler* CDiagramEntityContainer::GetClipboardHandler()
/* ============================================================
	Function :		CDiagramEntityContainer::GetClipboardHandler
	Description :	Returns a pointer to the current clipboard 
					handler.
	Access :		Public

	Return :		CDiagramClipboardHandler*	-	Current handler.
	Parameters :	none

	Usage :			Call to get a pointer to the current handler.

   ============================================================*/
{

	return m_clip;

}

void CDiagramEntityContainer::CopyAllSelected()
/* ============================================================
	Function :		CDiagramEntityContainer::CopyAllSelected
	Description :	Clones all selected object to the paste 
					array.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to copy all selected objects to the 
					clipboard. "Paste" will put them on screen.

   ============================================================*/
{

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	m_clip->CopyAllSelected( this );

}

int CDiagramEntityContainer::ObjectsInPaste()
/* ============================================================
	Function :		CDiagramEntityContainer::ObjectsInPaste
	Description :	Returns the number of objects in the paste 
					array.
	Access :		Public

	Return :		int		-	The number of objects.
	Parameters :	none

	Usage :			Call to get the number of objects in the 
					clipboard.

   ============================================================*/
{

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	return m_clip->ObjectsInPaste();

}

void CDiagramEntityContainer::ClearPaste()
/* ============================================================
	Function :		CDiagramEntityContainer::ClearPaste
	Description :	Clears the paste-array.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to clear the clipboard. All memory is 
					released.

   ============================================================*/
{

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	m_clip->ClearPaste();

}

void CDiagramEntityContainer::Paste()
/* ============================================================
	Function :		CDiagramEntityContainer::Paste
	Description :	Clones the contents of the paste array 
					into the container data array.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to paste the contents of the clipboard 
					to screen.

   ============================================================*/
{

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	m_clip->Paste( this );

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer message handling

void CDiagramEntityContainer::SendMessageToObjects( int command, BOOL selected, CDiagramEntity* sender, CWnd* from )
/* ============================================================
	Function :		CDiagramEntityContainer::SendMessageToObjects
	Description :	Sends "command" to objects. 
	Access :		Public

	Return :		void
	Parameters :	int command				-	The command to send.
					BOOL selected			-	If "TRUE", the command 
												will only be sent to 
												selected objects, 
												otherwise, it will be 
												sent to all objects.
					CDiagramEntity* sender	-	Original sender 
												or "NULL" if not 
												an object.

	Usage :			Call this member to send messages to 
					(selected) objects in the range "CMD_START" 
					to "CMD_END" inclusively (defined in 
					DiagramEntity.h). Calls the object "DoCommand".

   ============================================================*/
{

	BOOL stop = FALSE;
	int max = m_objs.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CDiagramEntity* obj = GetAt( t );
		if( !stop && ( !selected || obj->IsSelected() ) )
		{
			stop = obj->DoMessage( command, sender, from );
			SetModified( TRUE );
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer private helpers

int CDiagramEntityContainer::Find( CDiagramEntity* testobj )
/* ============================================================
	Function :		CDiagramEntityContainer::Find
	Description :	Finds the index of object "testobj" in the 
					data array.
	Access :		Protected

	Return :		int						-	Index of the 
												object or -1 
												if not found.
	Parameters :	CDiagramEntity* testobj	-	Object to find.
					
	Usage :			Internal function. 

   ============================================================*/
{

	int index = -1;
	CDiagramEntity* obj;
	int count = 0;
	while( ( obj = GetAt( count ) ) )
	{
		if( obj == testobj )
			index = count;
		count++;
	}

	return index;

}

void CDiagramEntityContainer::Swap( int index1, int index2 )
/* ============================================================
	Function :		CDiagramEntityContainer::Swap
	Description :	Swaps the elements at "index1" and "index2".
	Access :		Private

	Return :		void
	Parameters :	int index1	-	First object to swap
					int index2	-	Second object to swap
					
	Usage :			Internal function. Used to move objects up 
					or down in the z-order.

   ============================================================*/
{

	int max = m_objs.GetSize();
	if( index1 >= 0 && index1 < max && index2 >= 0 && index2 < max )
	{
		CDiagramEntity* obj1 = GetAt( index1 );
		CDiagramEntity* obj2 = GetAt( index2 );
		SetAt( index1, obj2 );
		SetAt( index2, obj1 );
	}

}

void CDiagramEntityContainer::Undo()
/* ============================================================
	Function :		CDiagramEntityContainer::Undo
	Description :	Sets the container data to the last entry 
					in the undo stack.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to undo the last operation

   ============================================================*/
{

	if( m_undo.GetSize() )
	{
		// We remove all current data
		RemoveAll();

		// We get the last entry from the undo-stack
		// and clone it into the container data
		CUndoItem* undo = static_cast< CUndoItem* >( m_undo.GetAt( m_undo.GetUpperBound() ) );
		int count = ( undo->arr ).GetSize();
		for( int t = 0 ; t < count ; t++ )
		{

			CDiagramEntity* obj = static_cast< CDiagramEntity* >( ( undo->arr ).GetAt( t ) );
			Add( obj->Clone() );

		}

		// Set the saved virtual size as well
		SetVirtualSize( undo->pt );

		// We remove the entry from the undo-stack
		delete undo;

		m_undo.RemoveAt( m_undo.GetUpperBound() );

	}

}

void CDiagramEntityContainer::Snapshot()
/* ============================================================
	Function :		CDiagramEntityContainer::Snapshot
	Description :	Copies the current state of the data to 
					the undo-stack.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to add the current state to the undo-stack. 
					If the undo stack has a maximum size and 
					the stack will grow above the stack limit, 
					the first undo array will be removed.

   ============================================================*/
{

	if( m_maxstacksize > 0 && m_undo.GetSize() == m_maxstacksize )
	{
		delete m_undo.GetAt( 0 );
		m_undo.RemoveAt( 0 );
	}

	CUndoItem* undo = new CUndoItem;

	while( !undo && m_undo.GetSize() )
	{

		// We seem - however unlikely -
		// to be out of memory.
		// Remove first element in
		// undo-stack and try again
		delete m_undo.GetAt( 0 );
		m_undo.RemoveAt( 0 );
		undo = new CUndoItem;

	}

	if( undo )
	{

		// Save current virtual size
		undo->pt = GetVirtualSize();

		// Save all objects
		int count = m_objs.GetSize();
		for( int t = 0 ; t < count ; t++ )
			( undo->arr ).Add( GetAt( t )->Clone() );

		// Add to undo stack
		m_undo.Add( undo );

	}

}

void CDiagramEntityContainer::ClearUndo()
/* ============================================================
	Function :		CDiagramEntityContainer::ClearUndo
	Description :	Remove all undo arrays from the undo stack
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to clear the undo-stack. All memory will 
					be deleted.

   ============================================================*/
{

	int count = m_undo.GetSize() - 1;
	for( int t = count ; t >= 0 ; t-- )
	{
		CUndoItem* undo = static_cast< CUndoItem* >( m_undo.GetAt( t ) );
		// Remove the stack entry itself.
		delete undo;
	}

	m_undo.RemoveAll();

}

BOOL CDiagramEntityContainer::IsUndoPossible()
/* ============================================================
	Function :		CDiagramEntityContainer::IsUndoPossible
	Description :	Check if it is possible to undo.
	Access :		Public

	Return :		BOOL	-	"TRUE" if undo is possible.
	Parameters :	none

	Usage :			Use this call for command enabling

   ============================================================*/
{

	return m_undo.GetSize();

}


void CDiagramEntityContainer::SetUndoStackSize( int maxstacksize )
/* ============================================================
	Function :		CDiagramEntityContainer::SetUndoStackSize
	Description :	Sets the size of the undo stack.
	Access :		Public

	Return :		void
	Parameters :	int maxstacksize	-	New size. -1 means 
											no limit, 0 no undo.
					
	Usage :			Call to set the max undo stack size.

   ============================================================*/
{

	m_maxstacksize = maxstacksize;

}

int CDiagramEntityContainer::GetUndoStackSize() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetUndoStackSize
	Description :	Returns the size of the undo-stack
	Access :		Public

	Return :		int	-	Current size
	Parameters :	none

	Usage :			Call to get the max undo stack size.

   ============================================================*/
{

	return m_maxstacksize;

}

CObArray* CDiagramEntityContainer::GetData() 
/* ============================================================
	Function :		CDiagramEntityContainer::GetData
	Description :	Accessor for the internal data array
	Access :		Public

	Return :		CObArray*	-	A pointer to the internal 
									data array.
	Parameters :	none

	Usage :			Call to access the internal data array. To 
					be used in derived classes.

   ============================================================*/
{ 

	return &m_objs; 

}

CObArray* CDiagramEntityContainer::GetPaste()	
/* ============================================================
	Function :		CDiagramEntityContainer::GetPaste
	Description :	Accessor for the internal paste array
	Access :		Protected

	Return :		CObArray*	-	A pointer to the paste 
									array
	Parameters :	none

	Usage :			Call to access the internal paste array. To 
					be used in derived classes.

   ============================================================*/
{ 

	CObArray* arr = NULL;
	if( m_clip )
		arr = m_clip->GetData();

	return arr;

}

CObArray* CDiagramEntityContainer::GetUndo()
/* ============================================================
	Function :		CDiagramEntityContainer::GetUndo
	Description :	Accessor for the internal undo array
	Access :		Protected

	Return :		CObArray*	-	A pointer to the undo 
									array
	Parameters :	none

	Usage :			Call to access the internal undo array. To 
					be used in derived classes.

   ============================================================*/
{ 

	return &m_undo;

}

void CDiagramEntityContainer::Group()
/* ============================================================
	Function :		CDiagramEntityContainer::Group
	Description :	Groups the currently selected objects into 
					the same group.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to group all selected items into the 
					same group.
					Grouped objects can be moved as a 
					single entity. Technically, when one object 
					in a group is selected, all other objects 
					are also selected automatically.

   ============================================================*/
{

	CDiagramEntity* obj;
	int count = 0;
	int group = CGroupFactory::GetNewGroup();
	while( ( obj = GetAt( count ) ) )
	{
		if( obj->IsSelected() )
			obj->SetGroup( group );
		count++;
	}

}

void CDiagramEntityContainer::Ungroup()
/* ============================================================
	Function :		CDiagramEntityContainer::Ungroup
	Description :	Ungroups the currently selected objects.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to ungroup all selected items. 
					Grouped objects can be moved as a 
					single entity. Technically, when one object 
					in a group is selected, all other objects 
					are also selected automatically.

   ============================================================*/
{

	CDiagramEntity* obj;
	int count = 0;
	while( ( obj = GetAt( count ) ) )
	{
		if( obj->IsSelected() )
			obj->SetGroup( 0 );
		count++;
	}

}

CSize CDiagramEntityContainer::GetTotalSize()
/* ============================================================
	Function :		CDiagramEntityContainer::GetTotalSize
	Description :	Gets the minimum bounding size for the 
					objects in the container.
	Access :		

	Return :		CSize	-	Minimum bounding size
	Parameters :	none

	Usage :			Call to get the screen size of the objects 
					in the container.

   ============================================================*/
{
	CPoint start = GetStartPoint();
	double width = 0;
	double height = 0;

	CDiagramEntity* obj;
	int count = 0;
	while( ( obj = GetAt( count ) ) )
	{

		width = max( width, obj->GetLeft() );
		width = max( width, obj->GetRight() );
		height = max( height, obj->GetTop() );
		height = max( height, obj->GetBottom() );

		count++;

	}

	return CSize( round( width - start.x ), round( height - start.y ) );

}

CPoint CDiagramEntityContainer::GetStartPoint()
/* ============================================================
	Function :		CDiagramEntityContainer::GetStartPoint
	Description :	Gets the starting screen position of the 
					objects in the container (normally the 
					top-left corner of the top-left object).
	Access :		

	Return :		CPoint	-	Top-left position of the 
								objects.
	Parameters :	none

	Usage :			Call to get the starting point on screen of 
					the objects.

   ============================================================*/
{

	double startx = 2000.0;
	double starty = 2000.0;

	CDiagramEntity* obj;
	int count = 0;

	while( ( obj = GetAt( count ) ) )
	{

		startx = min( startx, obj->GetLeft() );
		startx = min( startx, obj->GetRight() );
		starty = min( starty, obj->GetTop() );
		starty = min( starty, obj->GetBottom() );

		count++;

	}

	return CPoint( round( startx ), round( starty ) );

}

int	CDiagramEntityContainer::GetSelectCount() const
/* ============================================================
	Function :		int	CDiagramEntityContainer::GetSelectCount
	Description :	Gets the number of currently selected 
					objects in the container.
	Access :		

	Return :		int		-	Currently selected objects.
	Parameters :	none

	Usage :			Call to get the number of selected objects.

   ============================================================*/
{

	int max = GetSize();
	int count = 0;
	for( int t = 0 ; t < max ; t++ )
		if( GetAt( t )->IsSelected() )
			count++;

	return count;

}

void CDiagramEntityContainer::SelectAll()
{
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
		GetAt( t )->Select( TRUE );
}

void CDiagramEntityContainer::UnselectAll()
{
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
		GetAt( t )->Select( FALSE );
}

/**
leon add
* 이미지 로딩 및 메모리DC 처리
*/
BOOL CDiagramEntityContainer::ReadSprBG(CString picFullPath)
{
	// 배경 이미지 전체 경로 및 파일명까지 저장
	m_BGFullPath = picFullPath;
	if(m_spr.LoadSpr((char*)(LPCTSTR)picFullPath.Trim(STR_TRIM_LETTER)))
		return TRUE;
	else
		return FALSE;
}

CString CDiagramEntityContainer::GetBGPicFullPath()
{
	return m_BGFullPath;
}

#pragma warning( default : 4706 )
