#include "stdafx.h"
#include "DiagramEntity.h"
#include "DiagramEntityContainer.h"
#include "Tokenizer.h"
#include "../DLGDefine.h"

#include "../StdGrfx.h"
#include "../../MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../../MyDefine/MyDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntity
/**
leon add
* 각 컨트롤 속성 개수
*/
CTRL_PROPERTY_COUNT_INFO ctrlnum[CTRL_COUNT] = 
{
	BTN_STR, BTN_PROPERTY_COUNT,
	CBX_STR, CBX_PROPERTY_COUNT,
	ETB_STR, ETB_PROPERTY_COUNT,
	STATT_STR, STATT_PROPERTY_COUNT,
	IMGSTT_STR, IMGSTT_PROPERTY_COUNT, 
	LSTB_STR, LSTB_PROPERTY_COUNT, 
	LSTV_STR, LSTV_PROPERTY_COUNT,
	CHKB_STR, CHKB_PROPERTY_COUNT,
	RADB_STR, RADB_PROPERTY_COUNT,
	CHTETB_STR, CHTETB_PROPERTY_COUNT,
	OUTLNE_STR, OUTLNE_PROPERTY_COUNT,
};

CDiagramEntity::CDiagramEntity()
/* ============================================================
	Function :		CDiagramEntity::CDiagramEntity
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	m_pProperty = NULL;

	m_pFrame = NULL;

	for(int i=0; i<MAX_CONTROL_PROPERTY_NUM; i++)
	{
		SetBufferDC(NULL, i);
		SetBmp(NULL, i);
	}

	SetParent( NULL );
	SetPropertyDialog( NULL, 0 );
	Clear();
	SetType( _T( "basic" ) );

	SetGroup( 0 );
	SetPicFullPath(NULL);
	SetOldPicFullPath(NULL);
}

CDiagramEntity::CDiagramEntity(int count)
{
	if(count > 0)
		m_pProperty = new CString[count];
	else
		m_pProperty = NULL;

	for(int i=0; i<MAX_CONTROL_PROPERTY_NUM; i++)
	{
		SetBufferDC(NULL, i);
		SetBmp(NULL, i);
	}

	SetParent( NULL );
	SetPropertyDialog( NULL, 0 );
	Clear();
	SetType( _T( "basic" ) );

	SetGroup( 0 );
	SetPicFullPath(NULL);
	SetOldPicFullPath(NULL);
}


CDiagramEntity::~CDiagramEntity()
/* ============================================================
	Function :		CDiagramEntity::~CDiagramEntity
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	/**
	leon add
	* bufferDC 초기화
	*/
	DeleteAllBmp();
	DeleteAllBufferDC();

	if(m_pProperty)
	{
		delete [] m_pProperty;
		m_pProperty = NULL;
	}
}


// 입력 받은 id 번호가 이미 존재하는가? true : false
BOOL CDiagramEntity::IsOverLapping(CDiagramEntity* tmpObj, CString tmpIDNum)
{
	if(m_parent)
	{
		if( !m_parent->m_objs.IsEmpty())
		{
			int i = 0;
			CDiagramEntity* obj;

			for(i=0; i<m_parent->m_objs.GetCount(); i++)
			{
				obj = (CDiagramEntity*)m_parent->m_objs.GetAt( i );

				if(obj->m_pProperty[RESOURCE_ID] == tmpIDNum && tmpObj != obj)
					return TRUE;
			}
		}
	}

	return FALSE;
}

// 각 객체의 고유 id 번호를 증가시키는 가상 함수
void CDiagramEntity::IncreaseUniqueIDNum()
{
	return;
}
/**
leon add
* virtual function Property Set/Get
* Not Used!!
*/
BOOL CDiagramEntity::SetPropertyWithIndex(CString strVal, int index)
{
	int Count=0;

	Count = GetPropertyCount();

	if(Count == ERROR_CODE || m_pProperty == NULL)
	{
		AfxMessageBox(__FUNCTION__);
		return false;
	}

	if(index < 0 || index >= Count)
		return false;

    m_pProperty[index] = strVal;

	return true;
}

BOOL CDiagramEntity::GetPropertyWithIndex(CString& strVal, int index)
{
	int Count=0;

	Count = GetPropertyCount();

	if(Count == ERROR_CODE || m_pProperty == NULL)
	{
		AfxMessageBox(__FUNCTION__);
		return false;
	}

	if(index < 0 || index >= Count)
		return false;

    strVal = m_pProperty[index];

	return true;
}

int CDiagramEntity::GetPropertyCount()
{
	int i=0;

	if(m_pProperty != NULL)
	{
		for(i=0; i<CTRL_COUNT;	i++)
		{
			if(strcmp(ctrlnum[i].type, m_pProperty[KIND_NAME]) == 0)
				return ctrlnum[i].num;
		}
	}

	return ERROR_CODE;
}

void CDiagramEntity::SetPropertyDefault()
{
	int Count = 0;

	Count = GetPropertyCount();
	
    if(m_pProperty != NULL && Count != ERROR_CODE)
	{
        for(int i=0; i<Count; i++)
			m_pProperty[i].Empty();
	}
}

/**
leon add
* All Control Managing
* 파일로 저장을 위해 포맷을 만듦
*/
CString CDiagramEntity::GetFormatString()
{
        
	int i=0, Count=0;
	CString retStr;//, ctrlFormat;
	CString tmpStr, tmpStr2;

	Count = GetPropertyCount();

	if(Count == ERROR_CODE || m_pProperty == NULL)
	{
        AfxMessageBox(__FUNCTION__);
		return "";
	}

	//if(m_pProperty != NULL)
	{
        retStr = GetType();
		retStr += DLG_HEADER_SPLITTER_STR;
		//retStr += GetType();

		for(i=0; i<Count; i++)
		{
			// Dialog 자체 이미지 배경 상대경로로 만드는 곳은
			// CDiagramEntityContainer.cpp에 CString CDiagramEntityContainer::DeleteDefaultFilePath(CString fullPath)
			// 에서 처리함
			if(i == BG_IMAGE_FULLPATH)
			{
				// 배경 이미지 경로를 저장하기 위해 디폴트 루트 경로를 제거하여 상대 경로로 만듦
				tmpStr = m_pProperty[BG_IMAGE_FULLPATH];

				tmpStr = tmpStr.MakeLower();
				tmpStr2 = m_parent->GetDefaultFilePath();
				tmpStr2 = tmpStr2.MakeLower();

				if(m_parent != NULL)
					tmpStr.Replace( tmpStr2, STR_EMPTY);

				retStr += tmpStr;
			}
			else
                retStr += m_pProperty[i];
			
			if(i < Count-1) 
				retStr += DLG_ITEM_SPLITTER_STR;
		}

		return retStr;
	}

	AfxMessageBox(__FUNCTION__);

	return retStr;

	//if(m_pProperty != NULL)
	//{
	//	// Button Control
	//	if(strcmp(m_pProperty[KIND_NAME], BTN_STR) == 0)
	//	{
 //           ctrlFormat = "%s";
	//		ctrlFormat += DLG_HEADER_SPLITTER_STR;
 //           ctrlFormat += "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s";

	//		retStr.Format( _T( ctrlFormat ), GetType(), m_pProperty[BTN_KIND_NAME], m_pProperty[BTN_RESOURCE_ID], m_pProperty[BTN_X_COORD], m_pProperty[BTN_Y_COORD], m_pProperty[BTN_WIDTH], m_pProperty[BTN_HEIGHT],
	//			m_pProperty[BTN_BG_IMAGE_FULLPATH], m_pProperty[BTN_BG_WIDTH], m_pProperty[BTN_BG_HEIGHT], m_pProperty[BTN_BG_TILE], m_pProperty[BTN_BG_START_FONT], m_pProperty[BTN_GROUP_NUM], m_pProperty[BTN_TEXT_STR]);

	//		return retStr;
	//	}
}

void CDiagramEntity::Clear()
/* ============================================================
	Function :		CDiagramEntity::Clear
	Description :	Zero all properties of this object.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Call to initialize the object.

   ============================================================*/
{

	int i=0, count=0;

	count=GetPropertyCount();

	//for(i=0; i<count; i++)
	//	m_pProperty[i].Empty();
	SetPropertyDefault();

	SetRect( 0.0, 0.0, 0.0, 0.0 );
	SetMarkerSize( CSize( MARKERSIZE_X, MARKERSIZE_Y ) );
	SetConstraints( CSize( 1, 1 ), CSize( -1, -1 ) );
	Select( FALSE );
	SetParent( NULL );
	SetName( _T( "" ) );

	/**
	leon add
	* bufferDC 초기화
	*/
	ClearAllOldBmp();
	DeleteAllBmp();
	DeleteAllBufferDC();
	RestoreAllMemBitmap();
	SetAllBufferDC( NULL );
    
	m_spr.ClearImageData();

	// 컨트롤 이미지 전체 경로인 m_oldPicFullPath는 초기화하지 않음
	// SetOldPicFullPath( NULL );

}

//// 객체(엔터티) 복사 안됨.. 잠정 결정 ㅠㅠ..
CDiagramEntity* CDiagramEntity::Clone()
/* ============================================================
	Function :		CDiagramEntity::Clone
	Description :	Clone this object to a new object.
	Access :		Public

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{
	// 사용 안함
	ASSERT( 1 == 0);
	return NULL;

	//CDiagramEntity* obj = new CDiagramEntity;
	//obj->Copy( this );
	//return obj;
}

//// 객체(엔터티) 복사 안됨.. 잠정 결정 ㅠㅠ..
void CDiagramEntity::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntity::Copy
	Description :	Copy the information in "obj" to this object.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to copy 
											from.
					
	Usage :			Copies basic information. from "obj" to this.
					"GetType" can be used to check for the correct 
					object type in overridden versions.
   ============================================================*/
{
	//// 사용 안함
	//ASSERT( 1 == 0);
	//return ;

	Clear();

	SetMarkerSize( obj->GetMarkerSize() );
	SetConstraints( obj->GetMinimumSize(), obj->GetMaximumSize() );

	Select( obj->IsSelected() );
	SetParent( obj->GetParent() );
	SetType( obj->GetType() );
	SetTitle( obj->GetTitle() );
	SetName( obj->GetName() );
	SetRect( obj->GetLeft(), obj->GetTop(), obj->GetRight(), obj->GetBottom() );

	/**
	leon add
	* todo - 초기화
	*/
	SetPicFullPath( obj->GetPicFullPath() );
	SetOldPicFullPath( obj->GetOldPicFullPath() );

	// 객체(엔터티) 복사 안됨.. 잠정 결정 ㅠㅠ..
	//for(int i=0; i<MAX_CONTROL_PROPERTY_NUM; i++)
	//{

	//	obj->CreateBufferDC(..)
	//	SetBmp(obj->GetBmp(i), i);
	//}
}

/**
leon add
* 모든 컨트롤 객체가 CDiagramEntity에서 상속 받아
* FromString(..) 함수를 호출하여 파일로 부터 읽은
* 데이터를 처리한다.
*/
BOOL CDiagramEntity::FromString( const CString& str )
/* ============================================================
	Function :		CDiagramEntity::FromString
	Description :	Sets the values for an object from "str". 
	Access :		Public

	Return :		BOOL				-	"TRUE" if "str" 
											represents an 
											object of this 
											type.
	Parameters :	const CString& str	-	Possible text 
											format 
											representation.
					
	Usage :			Can be called to fill an existing object 
					with information from a string created with 
					"GetString".

   ============================================================*/
{

	BOOL result = FALSE;
	CString data( str );
	CString header = GetHeaderFromString( data );
	if( header == GetType() )
		if( GetDefaultFromString( data ) )
			result = TRUE;

	return result;

}

/**
leon add
* 각 컨트롤에서 넘어온 파일 정보에서
* 헤더 정보와 데이터(속성) 정보를 분리하여
* 헤더 정보는 리턴값으로 보내고 속성 정보 스트링을
* 참조로 빼낸다.
*/
CString CDiagramEntity::GetHeaderFromString( CString& str )
/* ============================================================
	Function :		CDiagramEntity::GetHeaderFromString
	Description :	Gets the header from "str".
	Access :		Protected

	Return :		CString			-	The type of "str".
	Parameters :	CString& str	-	"CString" to get type from.
					
	Usage :			Call as a part of loading the object. "str" 
					will have the type removed after the call.

   ============================================================*/
{

	CTokenizer main( str, _T( DLG_HEADER_SPLITTER_STR ) );

	CString header;
	CString data;
	if( main.GetSize() == 2 )
	{

		main.GetAt( 0, header );
		main.GetAt( 1, data );
		header.TrimLeft();
		header.TrimRight();
		data.TrimLeft();
		data.TrimRight();

		str = data;
	}

	return header;

}

/**
leon add
* 각 컨트롤에서 넘어온 파일 정보에서
* 데이터(속성) 정보를 처리한다.
* 파일에서 데이터를 가져올 때 사용함
*/
BOOL CDiagramEntity::GetDefaultFromString( CString& str )
/* ============================================================
	Function :		CDiagramEntity::GetDefaultFromString
	Description :	Gets the default properties from "str"
	Access :		Protected

	Return :		BOOL			-	"TRUE" if the default 
										properties could be loaded ok.
	Parameters :	CString& str	-	"CString" to get the 
										default properties from.
					
	Usage :			Call as a part of loading the object from 
					disk. The default object properties will 
					be stripped from "str" and the object 
					properties set from the data.

   ============================================================*/
{
	//// 사용 안함
	//ASSERT(1 == 0);
	//return false;
	BOOL result = FALSE;
	CString data( str );

	if( data[ data.GetLength() -1 ] == DLG_ENDLINE_CHAR )
	{
		data = data.Left( data.GetLength() - 1 ); // Strip the ';'
	}

	CTokenizer tok( data ); 
	int size = tok.GetSize();

	/**
	 leon alter
	 * todo - property 개수 변경
	 */
	int Count = GetPropertyCount();

	if(Count == ERROR_CODE || m_pProperty == NULL)
	{
        AfxMessageBox(__FUNCTION__);
		return false;
	}

	//if( m_pProperty != NULL)
	{
		if( size <= Count )					// 각 컨트롤의 속성 값이 추가 됨에 따라 버젼 관리상 카운트가 작을 수 있음
		{
			for(int i=0; i<size/*Count*/; i++)
			{
				tok.GetAt(i, m_pProperty[i]);

				m_pProperty[i].Replace( _T( DLG_REPLACE_HEADER_SPLITTER_STR ), _T( DLG_HEADER_SPLITTER_STR ) );
				m_pProperty[i].Replace( _T( DLG_REPLACE_ENDLINE_STR ), _T( DLG_ENDLINE_STR ) );
				m_pProperty[i].Replace( _T( DLG_REPLACE_ITEM_SPLITTER_STR ), _T( DLG_ITEM_SPLITTER_STR ) );
				m_pProperty[i].Replace( _T( DLG_REPLACE_NEWLINE ), _T( "\r\n" ) );

				//// 배경 이미지 전체 경로를 얻기 위해 디폴트 루트 경로를 상대 경로 앞에 붙여 만듦
				//if(i == BG_IMAGE_FULLPATH && m_parent != NULL)
				//	m_pProperty[BG_IMAGE_FULLPATH] = m_parent->GetDefaultFilePath() + m_pProperty[BG_IMAGE_FULLPATH];
			}
		
			//m_pProperty[BG_WIDTH].Format("%d", m_spr

			SetRect( _ttoi(m_pProperty[X_COORD]), _ttoi(m_pProperty[Y_COORD]), _ttoi(m_pProperty[X_COORD]) + _ttoi(m_pProperty[WIDTH]), _ttoi(m_pProperty[Y_COORD]) + _ttoi(m_pProperty[HEIGHT]), true);

			//SetTitle( m_pProperty[BTN_TEXT_STR] );
			//SetName( m_pProperty[BTN_RESOURCE_ID] );
			//SetGroup( _ttoi(m_pProperty[BTN_GROUP_NUM]) );
			
			/**
			leon add
			* todo - new property member function
			*/
			//m_spr.ClearImageData();

			//if(m_parent != NULL)
			//{
			//	SetPicFullPath( m_pProperty[BG_IMAGE_FULLPATH] );
			//}

			//if(!m_spr.LoadSpr((char *)(LPCTSTR) m_pProperty[BG_IMAGE_FULLPATH].Trim(STR_TRIM_LETTER)))
			//	return FALSE;

			// Rebuild rest of string
			str = _T( "" );
			for( int t = Count ; t < size ; t++ )
			{
				tok.GetAt( t, data );

				str += data;
				if( t < size - 1 )
					str += _T( DLG_ITEM_SPLITTER_STR );
			}

			result = TRUE;

			// 파일에서 읽은 후 없는 값을 디폴트 값으로 셋팅함
			SetPropertyDefault();
		}
	}

	return result;

}

BOOL CDiagramEntity::LoadFromString( CString& data )
/* ============================================================
	Function :		CDiagramEntity::LoadFromString
	Description :	Loads the object from "data".
	Access :		Public

	Return :		BOOL			-	"TRUE" if "str" is a 
										well-formed object prefix.
	Parameters :	CString& data	-	String to load from
					
	Usage :			Call to load the first part of an object 
					from string.

   ============================================================*/
{

	BOOL result = FALSE;
	CString header = GetHeaderFromString( data );
	if( header == GetType() )
		if( GetDefaultFromString( data ) )
			result = TRUE;

	return result;

}

CDiagramEntity* CDiagramEntity::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramEntity::CreateFromString
	Description :	Static factory function that creates and 
					returns an instance of this class if "str" 
					is a valid representation.
	Access :		Public

	Return :		CDiagramEntity*		-	The object, or "NULL" 
											if "str" is not a 
											representation of 
											this type.
	Parameters :	const CString& str	-	The string to create 
											from.
					
	Usage :			Can be used as a factory for text file loads. 
					Each object type should have its own 
					version - the default one is a model 
					implementation.

   ============================================================*/
{

	CDiagramEntity* obj = new CDiagramEntity;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramEntity::GetString()// const
/* ============================================================
	Function :		CDiagramEntity::GetString
	Description :	Creates a string representing the object.
	Access :		Public

	Return :		CString	-	The resulting string
	Parameters :	none

	Usage :			Used to save this object to a text file.

   ============================================================*/
{

	CString str = GetDefaultGetString();

	str += _T( DLG_ENDLINE_STR );

	return str;

}

/**
leon alter
* control property 추가
* 각 컨트롤 객체를 파일로 저장시 호출됨
*/
CString CDiagramEntity::GetDefaultGetString()// const
/* ============================================================
	Function :		CDiagramEntity::GetDefaultString
	Description :	Gets the default properties of the object 
					as a string.
	Access :		Protected

	Return :		CString	-	Resulting string
	Parameters :	none

	Usage :			Call as a part of the saving of objects 
					to disk.

   ============================================================*/
{
	//// 사용 안함
	//ASSERT(1 == 0);
	//return "";

	int i=0, j=0;
	int Count=0;
	
	Count = GetPropertyCount();

	if(Count == ERROR_CODE || m_pProperty == NULL)
	{
		AfxMessageBox(__FUNCTION__);
		return "";
	}

	CString strTmp;

	for(i=0; i<Count; i++)
	{
		m_pProperty[i].Replace( _T( DLG_HEADER_SPLITTER_STR ), _T( DLG_REPLACE_HEADER_SPLITTER_STR ) );
		m_pProperty[i].Replace( _T( DLG_ENDLINE_STR ), _T( DLG_REPLACE_ENDLINE_STR ) );
		m_pProperty[i].Replace( _T( DLG_ITEM_SPLITTER_STR ), _T( DLG_REPLACE_ITEM_SPLITTER_STR ) );
		
		//if(i == BG_IMAGE_FULLPATH)
		//{
			// 배경 이미지 경로에서 백슬래쉬 => 슬래쉬로 바꿈
			m_pProperty[i].Replace( FILE_PATH_SPLITTER_BACKSLASH, FILE_PATH_SPLITTER_SLASH);
		//	// 배경 이미지 경로를 저장하기 위해 디폴트 루트 경로를 제거하여 상대 경로로 만듦
		//	m_pProperty[i].Replace( m_parent->GetDefaultFilePath(), STR_EMPTY);
		//}

		m_pProperty[i].Replace( _T( "\r\n" ), _T( DLG_REPLACE_NEWLINE ) );
	}

//	BUTTON PROPERTY	KIND_NAME	RESOURCE_ID	X_COORD	Y_COORD	WIDTH	HEIGHT	BG_IMAGE_FULLPATH	BG_WIDTH	BG_HEIGHT	BG_TILE	BG_START_FONT	GROUP_NUM	TEXT_STR
//	예)												
//					button		1000		200		200		100		100		전체경로...			0				0				300			200				0		처음보이는 텍스트
//					문자열		숫자		숫자	숫자	숫자	숫자	문자열				숫자			숫자			숫자		숫자			숫자	문자열

	return 	GetFormatString();
}

CRect CDiagramEntity::GetRect() const
/* ============================================================
	Function :		CDiagramEntity::GetRect
	Description :	Returns the object rectangle.
	Access :		Public

	Return :		CRect	-	The object rectangle.
	Parameters :	none

	Usage :			Call to get the object position and size. 
					Will round of fractions.

   ============================================================*/
{
	CRect rect( static_cast< int >( GetLeft() ), 
				static_cast< int >( GetTop() ), 
				static_cast< int >( GetRight() ), 
				static_cast< int >( GetBottom() ) );

	return rect;
}

void CDiagramEntity::SetRect( CRect rect )
/* ============================================================
	Function :		CDiagramEntity::SetRect
	Description :	Sets the object rectangle, normalized.
	Access :		Public

	Return :		void
	Parameters :	CRect rect	-	The rectangle to set.
					
	Usage :			Call to place the object.

   ============================================================*/
{

	rect.NormalizeRect();
	SetRect( static_cast< double >( rect.left ), 
				static_cast< double >( rect.top ), 
				static_cast< double >( rect.right ), 
				static_cast< double >( rect.bottom ) );

}

/**
leon add
* 객체이미지가 있을 경우 그것의 가로 세로 혹은 세로 폭에 맞춰 최소 공배수 크기로 맞출 때 사용!!
*/
int CDiagramEntity::SetToMultipleSize(double origin_val, double ref_multiple)
{
	div_t result;

    if(origin_val < 1 || ref_multiple < 1)
		return origin_val;

	if(origin_val <= ref_multiple)
		return origin_val;

	result = div(origin_val, ref_multiple);

	return result.quot * ref_multiple;
}

void CDiagramEntity::SetRect( double left, double top, double right, double bottom, bool bIsLoading, int nModeDetail )
/* ============================================================
	Function :		CDiagramEntity::SetRect
	Description :	Sets the object rectangle.
	Access :		Public

	Return :		void
	Parameters :	double left		-	Left edge
					double top		-	Top edge
					double right	-	Right edge
					double bottom	-	Bottom edge
					
	Usage :			Call to place the object.

   ============================================================*/
{

	int mul_width = right-left, mul_height = bottom-top;

	SetLeft( left );
	SetTop( top );

	//SetRight( right );
	//SetBottom( bottom );
	/**
	leon add
	* 크기를 이미지 가로 혹은 세로 폭에 맞춰서 계산함
	*/
	if( nModeDetail == 5 || nModeDetail == 4 ) // Shift Key를 누르고 있을 때만 아래 적용
	{
		if(m_pProperty != NULL && m_pProperty[BG_IMAGE_FULLPATH].Trim(DLG_FILE_TRIM_LETTER) != STR_EMPTY
			&& m_pProperty[BG_WIDTH].Trim(DLG_FILE_TRIM_LETTER) != STR_EMPTY && m_pProperty[BG_WIDTH].Trim(DLG_FILE_TRIM_LETTER) != STR_ZERO)
		{
				//mul_width = SetToMultipleSize(right - left, _tstof(m_pProperty[BG_WIDTH]));
				mul_width = SetToMultipleSize(right - left, m_spr.clHeader.siXsize);
		}

		if(m_pProperty != NULL && m_pProperty[BG_IMAGE_FULLPATH].Trim(DLG_FILE_TRIM_LETTER) != STR_EMPTY
			&& m_pProperty[BG_HEIGHT].Trim(DLG_FILE_TRIM_LETTER) != STR_EMPTY && m_pProperty[BG_HEIGHT].Trim(DLG_FILE_TRIM_LETTER) != STR_ZERO)
		{
				//mul_height = SetToMultipleSize(bottom - top, _tstof(m_pProperty[BG_HEIGHT]));
				mul_height = SetToMultipleSize(bottom - top, m_spr.clHeader.siYsize);
		}
	}

	SetRight( left + mul_width );
	SetBottom( top +  mul_height);

	if( GetMinimumSize().cx != -1 )
		if( GetRect().Width() < GetMinimumSize().cx )
			SetRight( GetLeft() + GetMinimumSize().cx );

	if( GetMinimumSize().cy != -1 )
		if( GetRect().Height() < GetMinimumSize().cy )
			SetBottom( GetTop() + GetMinimumSize().cy );

	if( GetMaximumSize().cx != -1 )
		if( GetRect().Width() > GetMaximumSize().cx )
			SetRight( GetLeft() + GetMaximumSize().cx );

	if( GetMaximumSize().cy != -1 )
		if( GetRect().Height() > GetMaximumSize().cy )
			SetBottom( GetTop() + GetMaximumSize().cy );

	if(!bIsLoading)
	{
		if(m_pProperty)
		{
			//// surface 크기 > 컨트롤 크기일때만 실행되게 할려면
			//if(m_pProperty[KIND_NAME].MakeLower() != CBX_STR) //if(mul_width > _ttoi(m_pProperty[BG_WIDTH]))
			{
				m_pProperty[BG_WIDTH].Format("%d", (int)mul_width );
			}
			//mul_width = SetToMultipleSize( _ttoi(m_pProperty[BG_WIDTH]), m_spr.clHeader.siXsize);
			//m_pProperty[BG_WIDTH].Format("%d", (int)mul_width );
		}

		if(m_pProperty)
		{
			// surface 크기 > 컨트롤 크기일때만 실행되게 할려면
			if(m_pProperty[KIND_NAME].MakeLower() != CBX_STR) //if(mul_height > _ttoi(m_pProperty[BG_HEIGHT]))
			{
				m_pProperty[BG_HEIGHT].Format("%d", (int)mul_height);
			}
			//mul_height = SetToMultipleSize( _ttoi(m_pProperty[BG_HEIGHT]), m_spr.clHeader.siYsize);
			//m_pProperty[BG_HEIGHT].Format("%d", (int)mul_height);
		}
	}

	// leon add
	// Dialog Editor 크기 조작 중..
	if( GetPropertyDialog() )
			GetPropertyDialog()->SetValues();

}

void CDiagramEntity::MoveRect( double x, double y )
/* ============================================================
	Function :		CDiagramEntity::MoveRect
	Description :	Moves the object rectangle.
	Access :		Public

	Return :		void
	Parameters :	double x	-	Move x steps horizontally.
					double y	-	Move y steps vertically.
					
	Usage :			Call to move the object on screen.

   ============================================================*/
{

	SetRect( GetLeft() + x, GetTop() + y, GetRight() + x, GetBottom() + y );

}

void CDiagramEntity::Select( BOOL selected )
/* ============================================================
	Function :		CDiagramEntity::Select
	Description :	Sets the object select state.
	Access :		Public

	Return :		void
	Parameters :	BOOL selected	-	"TRUE" to select, "FALSE" 
										to unselect.
					
	Usage :			Call to select/deselect the object.

   ============================================================*/
{

	m_selected = selected;

	if( selected && GetGroup() )
	{
		CDiagramEntityContainer* parent = GetParent();
		if( parent )
			parent->SendMessageToObjects( CMD_SELECT_GROUP, FALSE, this );
	}

}

BOOL CDiagramEntity::IsSelected() const
/* ============================================================
	Function :		CDiagramEntity::IsSelected
	Description :	Checks if the object is selected.
	Access :		Public

	Return :		BOOL	-	"TRUE" if the object is selected.
	Parameters :	none

	Usage :			Call to see if the object is selected.

   ============================================================*/
{

	return m_selected;

}

BOOL CDiagramEntity::BodyInRect( CRect rect ) const
/* ============================================================
	Function :		CDiagramEntity::BodyInRect
	Description :	Used to see if any part of the object lies 
					in "rect".
	Access :		Public

	Return :		BOOL		-	"TRUE" if any part of the 
									object lies inside rect.
	Parameters :	CRect rect	-	The rectangle to check.
					
	Usage :			Call to see if the object overlaps - for 
					example - a selection rubberband.

   ============================================================*/
{

	BOOL result = FALSE;
	CRect rectEntity = GetRect();
	CRect rectIntersect;

	rect.NormalizeRect();
	rectEntity.NormalizeRect();

	rectIntersect.IntersectRect( rect, rectEntity );
	if( !rectIntersect.IsRectEmpty() )
		result = TRUE;

	return result;

}

int CDiagramEntity::GetHitCode( CPoint point ) const
/* ============================================================
	Function :		CDiagramEntity::GetHitCode
	Description :	Returns the hit point constant for "point".
	Access :		Public

	Return :		int				-	The hit point, 
										"DEHT_NONE" if none.
	Parameters :	CPoint point	-	The point to check
					
	Usage :			Call to see in what part of the object point 
					lies. The hit point can be one of the following:
						"DEHT_NONE" No hit-point
						"DEHT_BODY" Inside object body
						"DEHT_TOPLEFT" Top-left corner
						"DEHT_TOPMIDDLE" Middle top-side
						"DEHT_TOPRIGHT" Top-right corner
						"DEHT_BOTTOMLEFT" Bottom-left corner
						"DEHT_BOTTOMMIDDLE" Middle bottom-side
						"DEHT_BOTTOMRIGHT" Bottom-right corner
						"DEHT_LEFTMIDDLE" Middle left-side
						"DEHT_RIGHTMIDDLE" Middle right-side

   ============================================================*/
{

	CRect rect = GetRect();
	return GetHitCode( point, rect );

}

BOOL CDiagramEntity::DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from )
/* ============================================================
	Function :		CDiagramEntity::DoMessage
	Description :	Message handler for the object.
	Access :		Public

	Return :		BOOL					-	"TRUE" to stop 
												further processing.
	Parameters :	UINT msg				-	The message.
					CDiagramEntity* sender	-	Original sender of 
												this message, or 
												"NULL" if not an object.

	Usage :			The container can send messages to all 
					objects. The messages should lie in the 
					range "CMD_START" to "CMD_STOP" inclusively - 
					a few are already predefined in 
					DiagramEntity.h. This function will be 
					called as response to those messages. This 
					mechanism is already used for sending back 
					messages from "CDiagramEditor" to the 
					relevant object when a object popup menu 
					alternative is selected.

   ============================================================*/
{

	BOOL stop = FALSE;
	switch( msg )
	{
		case CMD_CUT:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Cut( this );
			}
		break;

		case CMD_COPY:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Copy( this );
			}
		break;

		case CMD_UP:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Up( this );
			}
		break;

		case CMD_DOWN:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Down( this );
			}
		break;

		case CMD_FRONT:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Front( this );
			}
		break;

		case CMD_BOTTOM:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Bottom( this );
			}
		break;

		case CMD_DUPLICATE:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Duplicate( this );
				Select( FALSE );
			}
		break;

		case CMD_PROPERTIES:
			if( IsSelected() )
			{
				ShowProperties( from );
				stop = TRUE;
			}
		break;

		case CMD_SELECT_GROUP:
			if( sender != this )
				if( sender->GetGroup() == GetGroup() )
					m_selected = TRUE;
		break;

	}

	return stop;

}

void CDiagramEntity::ShowPopup( CPoint point, CWnd* parent )
/* ============================================================
	Function :		CDiagramEntity::ShowPopup
	Description :	Shows the popup menu for the object.
	Access :		Public

	Return :		void
	Parameters :	CPoint point	-	The point to track.
					CWnd* parent	-	The parent "CWnd" of the 
										menu (should be the 
										"CDiagramEditor")

	Usage :			The function uses hardcoded strings to 
					avoid having to include resource file 
					fragments. Derived classes needing a non-
					standard or localized menu should load 
					menues from resources instead.

   ============================================================*/
{

	CMenu menu;
	if( menu.CreatePopupMenu() )
	{

		menu.AppendMenu( MF_STRING, CMD_CUT, _T( "Cut" ) );
		menu.AppendMenu( MF_STRING, CMD_COPY, _T( "Copy" ) );
		menu.AppendMenu( MF_STRING, CMD_DUPLICATE, _T( "Duplicate" ) );

		/**
		leon add
		* Edit 기능 사용 안함
		*/
		menu.EnableMenuItem( CMD_CUT, MF_BYCOMMAND | MF_GRAYED );
		menu.EnableMenuItem( CMD_COPY, MF_BYCOMMAND | MF_GRAYED );
		menu.EnableMenuItem( CMD_DUPLICATE, MF_BYCOMMAND | MF_GRAYED );

		menu.AppendMenu( MF_SEPARATOR );
		menu.AppendMenu( MF_STRING, CMD_UP, _T( "Up" ) );
		menu.AppendMenu( MF_STRING, CMD_DOWN, _T( "Down" ) );
		menu.AppendMenu( MF_STRING, CMD_FRONT, _T( "To front" ) );
		menu.AppendMenu( MF_STRING, CMD_BOTTOM, _T( "To back" ) );
		menu.AppendMenu( MF_SEPARATOR );
		menu.AppendMenu( MF_STRING, CMD_PROPERTIES, _T( "Properties" ) );
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, parent );

	}

}

void CDiagramEntity::ShowProperties( CWnd* parent, BOOL show )
/* ============================================================
	Function :		CDiagramEntity::ShowProperties
	Description :	Shows the property dialog for the object.
	Access :		Public

	Return :		void
	Parameters :	CWnd* parent	-	Parent of the dialog
					BOOL show		-	"TRUE" to show, "FALSE" 
										to hide.

	Usage :			Call to show the property dialog for this 
					object.

   ============================================================*/
{

	if( m_propertydlg )
	{
		if( show )
		{
			if( !m_propertydlg->m_hWnd )
				m_propertydlg->Create( ( UINT ) m_propertydlgresid, parent );

			m_propertydlg->ShowWindow( SW_SHOW );

			m_propertydlg->SetValues();
			m_propertydlg->SetFocus();
		}
		else
			if( m_propertydlg->m_hWnd )
				m_propertydlg->ShowWindow( SW_HIDE );
	}

}

void CDiagramEntity::DrawObject( CDC* dc, double zoom )
/* ============================================================
	Function :		CDiagramEntity::DrawObject
	Description :	Top-level drawing function for the object.
	Access :		Public

	Return :		void
	Parameters :	CDC* dc		-	"CDC" to draw to.
					double zoom	-	Zoom level to use
					
	Usage :			Even though virtual, this function should 
					normally not be overridden (use "Draw" 
					instead). The function stores the zoom and 
					calculates the true drawing rectangle.

   ============================================================*/
{

	SetZoom( zoom );
	CRect rect( round( GetLeft() * zoom ), 
				round( GetTop() * zoom ), 
				round( GetRight() * zoom ), 
				round( GetBottom() * zoom ) );

	Draw( dc, rect );

	if( IsSelected() )
		DrawSelectionMarkers( dc, rect );

}

void CDiagramEntity::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramEntity::Draw
	Description :	Draws the object.
	Access :		Public

	Return :		void
	Parameters :	CDC* dc		-	The "CDC" to draw to. 
					CRect rect	-	The real rectangle of the 
									object.

	Usage :			The function should clean up all selected 
					objects. Note that the "CDC" is a memory "CDC", 
					so creating a memory "CDC" in this function 
					will probably not speed up the function.

   ============================================================*/
{

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

	dc->Rectangle( rect );


}

HCURSOR CDiagramEntity::GetCursor( int hit ) const
/* ============================================================
	Function :		CDiagramEntity::GetCursor
	Description :	Returns the cursor for the given hit point.
	Access :		Public

	Return :		HCURSOR	-	The cursor to show
	Parameters :	int hit	-	The hit point constant ("DEHT_") 
								to get the cursor for.

	Usage :			Call to get the cursor for a specific hit 
					point constant.
					"hit" can be one of the following:
						"DEHT_NONE" No hit-point
						"DEHT_BODY" Inside object body
						"DEHT_TOPLEFT" Top-left corner
						"DEHT_TOPMIDDLE" Middle top-side
						"DEHT_TOPRIGHT" Top-right corner
						"DEHT_BOTTOMLEFT" Bottom-left corner
						"DEHT_BOTTOMMIDDLE" Middle bottom-side
						"DEHT_BOTTOMRIGHT" Bottom-right corner
						"DEHT_LEFTMIDDLE" Middle left-side
						"DEHT_RIGHTMIDDLE" Middle right-side

   ============================================================*/
{
	HCURSOR cursor = NULL;
	switch( hit )
	{
		case DEHT_BODY:
			cursor = LoadCursor( NULL, IDC_SIZEALL );
		break;
		case DEHT_TOPLEFT:
			cursor = LoadCursor( NULL, IDC_SIZENWSE );
		break;
		case DEHT_TOPMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZENS );
		break;
		case DEHT_TOPRIGHT:
			cursor = LoadCursor( NULL, IDC_SIZENESW );
		break;
		case DEHT_BOTTOMLEFT:
			cursor = LoadCursor( NULL, IDC_SIZENESW );
		break;
		case DEHT_BOTTOMMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZENS );
		break;
		case DEHT_BOTTOMRIGHT:
			cursor = LoadCursor( NULL, IDC_SIZENWSE );
		break;
		case DEHT_LEFTMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZEWE );
		break;
		case DEHT_RIGHTMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZEWE );
		break;
	}
	return cursor;
}

void CDiagramEntity::DrawSelectionMarkers( CDC* dc, CRect rect ) const
/* ============================================================
	Function :		CDiagramEntity::DrawSelectionMarkers
	Description :	Draws the selection markers for the 
					object.
	Access :		Protected

					
	Return :		void
	Parameters :	CDC* dc		-	The "CDC" to draw to
					CRect rect	-	The real object rectangle.
					
	Usage :			"rect" is the true rectangle (zoomed) of the 
					object.

   ============================================================*/
{

	// 선택시 보일 외곽선이 있다면
	//CStdGrfx::draw3dFrame( dc, rect );

	// Draw selection markers
	CRect rectSelect;

	if(m_pFrame != NULL && m_pProperty != NULL)
	{
		CString strText;

		strText.Format( "현재 타입 : %s , ID : %s", GetType(), m_pProperty[RESOURCE_ID] );

		m_pFrame->m_wndStatusBar.SetPaneText( 1, strText );
	}

	dc->SelectStockObject( BLACK_BRUSH );
	rectSelect = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPMIDDLE, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMMIDDLE, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_RIGHTMIDDLE, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_LEFTMIDDLE, rect );
	dc->Rectangle( rectSelect );

}

CRect CDiagramEntity::GetSelectionMarkerRect( UINT marker, CRect rect ) const
/* ============================================================
	Function :		CDiagramEntity::GetSelectionMarkerRect
	Description :	Gets the selection marker rectangle for 
					marker, given the true object rectangle 
					"rect".
	Access :		Protected

					
	Return :		CRect		-	The marker rectangle
	Parameters :	UINT marker	-	The marker type ("DEHT_"-
									constants defined in 
									DiargramEntity.h)
					CRect rect	-	The object rectangle
					
	Usage :			"marker" can be one of the following:
						"DEHT_NONE" No hit-point
						"DEHT_BODY" Inside object body
						"DEHT_TOPLEFT" Top-left corner
						"DEHT_TOPMIDDLE" Middle top-side
						"DEHT_TOPRIGHT" Top-right corner
						"DEHT_BOTTOMLEFT" Bottom-left corner
						"DEHT_BOTTOMMIDDLE" Middle bottom-side
						"DEHT_BOTTOMRIGHT" Bottom-right corner
						"DEHT_LEFTMIDDLE" Middle left-side
						"DEHT_RIGHTMIDDLE" Middle right-side

   ============================================================*/
{

	CRect rectMarker;
	int horz = m_markerSize.cx / 2;
	int vert = m_markerSize.cy / 2;

	switch( marker )
	{
		case DEHT_TOPLEFT:
			rectMarker.SetRect( rect.left - horz,
								rect.top - vert,
								rect.left + horz,
								rect.top + vert );
		break;

		case DEHT_TOPMIDDLE:
			rectMarker.SetRect( rect.left + ( rect.Width() / 2 ) - horz,
								rect.top - vert,
								rect.left + ( rect.Width() / 2 ) + horz,
								rect.top + vert );
		break;

		case DEHT_TOPRIGHT:
			rectMarker.SetRect( rect.right - horz,
								rect.top - vert,
								rect.right + horz,
								rect.top + vert );
		break;

		case DEHT_BOTTOMLEFT:
			rectMarker.SetRect( rect.left - horz,
								rect.bottom - vert,
								rect.left + horz,
								rect.bottom + vert );
		break;

		case DEHT_BOTTOMMIDDLE:
			rectMarker.SetRect( rect.left + ( rect.Width() / 2 ) - horz,
								rect.bottom - vert,
								rect.left + ( rect.Width() / 2 ) + horz,
								rect.bottom + vert );
		break;

		case DEHT_BOTTOMRIGHT:
			rectMarker.SetRect( rect.right - horz,
								rect.bottom - vert,
								rect.right + horz,
								rect.bottom + vert );
		break;

		case DEHT_LEFTMIDDLE:
			rectMarker.SetRect( rect.left - horz,
								rect.top + ( rect.Height() / 2 ) - vert,
								rect.left + horz,
								rect.top + ( rect.Height() / 2 ) + vert );
		break;

		case DEHT_RIGHTMIDDLE:
			rectMarker.SetRect( rect.right - horz,
								rect.top + ( rect.Height() / 2 ) - vert,
								rect.right + horz,
								rect.top + ( rect.Height() / 2 ) + vert );
		break;
	}

	return rectMarker;

}

// CDiagramEntityContainer를 설정하면서 동시에
// 읽어온 path 경로를 전체 경로로 만듦
void CDiagramEntity::SetParent( CDiagramEntityContainer * parent )
/* ============================================================
	Function :		CDiagramEntity::SetParent
	Description :	Set the container owning the object.
	Access :		Protected

	Return :		void
	Parameters :	CDiagramEntityContainer * parent	-	the 
															parent.
					
	Usage :			Call to set the parent of the object. 
					Objects must know their parent, to allow 
					copying etc. 

   ============================================================*/
{

	m_parent = parent;

	/**
	leon add
	* todo - 차후 분리 필요!!
	*/
	if(m_parent != NULL && m_pProperty != NULL && m_pProperty[BG_IMAGE_FULLPATH].Trim() != STR_EMPTY)
	{
		CString tmpStr, tmpStr2;

		tmpStr = m_pProperty[BG_IMAGE_FULLPATH];
		tmpStr = tmpStr.MakeLower();

		tmpStr2 = m_parent->GetDefaultFilePath();
		tmpStr2 = tmpStr.MakeLower();

		if(tmpStr.Find( tmpStr2 )< 0)
			AfxMessageBox( m_pProperty[BG_IMAGE_FULLPATH] + " 파일안에 이미지 경로가 잘못 되었습니다. 상대경로가 필요합니다. " );
		
		// 파일에서 읽어온 경로와 Default Path를 붙여서 전체 경로를 만듦
		m_pProperty[BG_IMAGE_FULLPATH] = m_parent->GetDefaultFilePath() + m_pProperty[BG_IMAGE_FULLPATH];
		m_pProperty[BG_IMAGE_FULLPATH] =  m_pProperty[BG_IMAGE_FULLPATH];

		m_spr.ClearImageData();

		if(m_parent != NULL)
		{
			SetPicFullPath( m_pProperty[BG_IMAGE_FULLPATH] );
		}

		m_spr.LoadSpr((char *)(LPCTSTR) m_pProperty[BG_IMAGE_FULLPATH].Trim(STR_TRIM_LETTER));

		/**
		leon add
		* 잘못된 이미지 크기 조정
		* => 이미지 크기가 아니라 surface 크기로 설정해야함
		*/
		//m_pProperty[BG_WIDTH].Format("%d", m_spr.clHeader.siXsize);
		//m_pProperty[BG_HEIGHT].Format("%d", m_spr.clHeader.siYsize);
	}
}

void CDiagramEntity::GetFont( LOGFONT& lf ) const
/* ============================================================
	Function :		CDiagramEntity::GetFont
	Description :	Returns the system GUI font in a "LOGFONT" 
					scaled to the zoom level of the object.
	Access :		Protected

					
	Return :		void
	Parameters :	LOGFONT& lf	-	The "LOGFONT" for the system
									GUI font.
					
	Usage :			Call to get the system font. Note that MS 
					Sans Serif will not scale below 8 points.

   ============================================================*/
{

	HFONT hfont = ( HFONT ) ::GetStockObject( DEFAULT_GUI_FONT );
	CFont* font = CFont::FromHandle( hfont );
	font->GetLogFont( &lf );
	lf.lfHeight = round( static_cast< double >( lf.lfHeight ) * m_zoom );

}

CString CDiagramEntity::GetType() const
/* ============================================================
	Function :		CDiagramEntity::GetType
	Description :	Returns the object type.
	Access :		Public

	Return :		CString	-	The type of the object.
	Parameters :	none

	Usage :			Call to get the type of the object. The type 
					is used when saving and loading objects 
					to/from a text file.

   ============================================================*/
{
	//return m_type;

	if(m_pProperty)
		return m_pProperty[KIND_NAME];

//	ASSERT(1 == 0);

	return "";
}

void CDiagramEntity::SetType( CString type )
/* ============================================================
	Function :		CDiagramEntity::SetType
	Description :	Set the object type.
	Access :		Public

	Return :		void
	Parameters :	CString type	-	The type to set
					
	Usage :			Call to set the object type - normally in 
					the "ctor" of this object. The type is used 
					when saving and loading objects to/from a 
					text file.

   ============================================================*/
{
	if(m_pProperty)
		m_pProperty[KIND_NAME] = type;
	//else
 //       ASSERT(1==0);
	//m_type = type;
}

CString CDiagramEntity::GetTitle() const
/* ============================================================
	Function :		CDiagramEntity::GetTitle
	Description :	Gets the Title property
	Access :		Public

	Return :		CString	-	The current title
	Parameters :	none

	Usage :			Call to get the title of the object. Title 
					is a property that the object can use in
					whatever way it wants.

   ============================================================*/
{

	return m_title;
}

void CDiagramEntity::SetTitle( CString title )
/* ============================================================
	Function :		CDiagramEntity::SetTitle
	Description :	Sets the Title property
	Access :		Public

	Return :		void
	Parameters :	CString title	-	The new title
					
	Usage :			Call to set the title of the object. Title 
					is a property that the object can use in
					whatever way it wants.

   ============================================================*/
{

	m_title = title;

}

CString CDiagramEntity::GetName() const
/* ============================================================
	Function :		CDiagramEntity::GetName
	Description :	Gets the Name property
	Access :		Public

	Return :		CString	-	The current name
	Parameters :	none

	Usage :			Call to get the name of the object. Name is 
					a property that the object can use in
					whatever way it wants.


   ============================================================*/
{

	return m_name;

}

void CDiagramEntity::SetName( CString name )
/* ============================================================
	Function :		CDiagramEntity::SetName
	Description :	Sets the Name property
	Access :		Public

	Return :		void
	Parameters :	CString name	-	The new name
					
	Usage :			Call to set the name of the object. Name is 
					a property that the object can use in
					whatever way it wants.

   ============================================================*/
{

	m_name = name;

}

CString CDiagramEntity::GetPicFullPath() const
{
/* ============================================================
	Function :		CDiagramEntity::GetPicFullPath
	Description :	Gets the Picture FullPath property
	Access :		Public

	Return :		CString	-	The current Picture FullPath
	Parameters :	none

	Usage :			
   ============================================================*/

	//return m_picFullPath;

	if(m_pProperty)
		return m_pProperty[BG_IMAGE_FULLPATH];

	ASSERT(1 == 0);
	return "";
}

void CDiagramEntity::SetPicFullPath( CString picFullPath )
{
/* ============================================================
	Function :		CDiagramEntity::SetPicFullPath
	Description :	Sets the Picture FullPath property
	Access :		Public

	Return :		CString	-	The current Picture FullPath
	Parameters :	none

	Usage :			
   ============================================================*/

    //m_picFullPath = picFullPath;
	if(m_pProperty)
		m_pProperty[BG_IMAGE_FULLPATH] = picFullPath;

//	ASSERT(1 == 0);
}

CString CDiagramEntity::GetOldPicFullPath() const
{
	return m_oldPicFullPath;
}

void CDiagramEntity::SetOldPicFullPath( CString oldPicFullPath )
{
	m_oldPicFullPath = oldPicFullPath;
}
/**
//*****************************************************************
leon add
* Get Buffer DC
*/
void CDiagramEntity::CreateBmp(CDC* dc, int index)
{
    m_pmemBmp[index] = new CBitmap;
	m_pmemBmp[index]->CreateCompatibleBitmap(dc, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize);
}

void CDiagramEntity::CreateBufferDC(CDC* dc, int index)
{
	m_pbufferDC[index] = new CDC;
	// ? dc 선정이 오류???
	CreateBmp(dc, index);
	m_pbufferDC[index]->BitBlt(0, 0, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, dc, 0, 0, SRCCOPY);
}

CBitmap* CDiagramEntity::GetOldBmp( int index )
{
    return m_pOldBmp[index];
}

void CDiagramEntity::SetOldBmp( CBitmap* pmemBmp, int index )
{
    m_pOldBmp[index] = pmemBmp;
}

void CDiagramEntity::SetAllOldBmp( CBitmap* pmemBmp )
{
	for(int i=0; i<MAX_CONTROL_BUFFER_LENGTH; i++)
	{
	   m_pOldBmp[i] = pmemBmp;
	}
}

CBitmap* CDiagramEntity::GetBmp( int index )
{
    return m_pmemBmp[index];
}

void CDiagramEntity::SetBmp( CBitmap* pmemBmp, int index )
{
    m_pmemBmp[index] = pmemBmp;
}

void CDiagramEntity::SetAllBmp( CBitmap* pmemBmp )
{
	for(int i=0; i<MAX_CONTROL_BUFFER_LENGTH; i++)
	{
	   m_pmemBmp[i] = pmemBmp;
	}
}

void CDiagramEntity::DeleteBmp(int index)
{
	if(m_pmemBmp[index])
	{
		m_pmemBmp[index]->DeleteObject();
		delete m_pmemBmp[index];
		SetBmp(NULL, index);
	}
}

void CDiagramEntity::DeleteAllBmp()
{
	for(int i=0; i<MAX_CONTROL_BUFFER_LENGTH; i++)
        DeleteBmp(i);
}

CDC* CDiagramEntity::GetBufferDC(int index)
{
	return m_pbufferDC[index];
}

void CDiagramEntity::SetBufferDC( CDC* pbufferDC, int index )
{
    m_pbufferDC[index] = pbufferDC;
}

void CDiagramEntity::SetAllBufferDC( CDC* pbufferDC)
{
	for(int i=0; i<MAX_CONTROL_BUFFER_LENGTH; i++)
	{
	   m_pbufferDC[i] = pbufferDC;
	}
}

void CDiagramEntity::DeleteAllBufferDC()
{
	for(int i=0; i<MAX_CONTROL_BUFFER_LENGTH; i++)
	{
	    if(m_pbufferDC[i])
		{
			//m_pbufferDC[i]->DeleteDC();
			//m_pbufferDC[i]->ReleaseOutputDC();
			m_pbufferDC[i]->FillSolidRect(0, 0, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, TRANSPARENT_COLOR);
			delete m_pbufferDC[i];
			SetBufferDC( NULL, i );
		}
	}
}

void CDiagramEntity::ClearOldBmp(int index)
{
    m_pOldBmp[index] = NULL;
}

void CDiagramEntity::ClearAllOldBmp()
{
	for(int i=0; i<MAX_CONTROL_BUFFER_LENGTH; i++)
		ClearOldBmp(i);
}

void CDiagramEntity::RestoreMemBitmap(int index)
{
	if(m_pOldBmp[index])
		m_pbufferDC[index]->SelectObject(m_pOldBmp[index]);
}

void CDiagramEntity::RestoreAllMemBitmap()
{
	for(int i=0; i<MAX_CONTROL_BUFFER_LENGTH; i++)
		RestoreMemBitmap(i);
}

void CDiagramEntity::MakeMemDC( CDC* dc, int tileImageStartFont, int tileImageNum )
{
	// bufferDC에 내용이 이미 존재하면 만들지 않음
	cltTSpr *pSpr = &m_spr;
	
	// mask buffer, real buffer ...
	ASSERT(MAX_CONTROL_BUFFER_LENGTH >= 2);

	int i=0;

	if(tileImageNum < 1 || tileImageStartFont< 0)
		return;
	/**
	* m_pbufferDC : 메모리DC에 이미지 로딩함
	*/
	for(i=0; i<MAX_CONTROL_BUFFER_LENGTH; i++)
	{
		if(!m_pbufferDC[i])
		{
			m_pbufferDC[i] = new CDC;
			m_pbufferDC[i]->CreateCompatibleDC( dc );
		}

		// memory bmp
		// tile image num 개수를 전체로 붙인 것 만큼 생성!!
		if(!m_pmemBmp[i])
		{
			m_pmemBmp[i] = new CBitmap;
			m_pmemBmp[i]->CreateCompatibleBitmap( dc, pSpr->clHeader.siXsize * tileImageNum, pSpr->clHeader.siYsize);
		}

		if(!m_pOldBmp[i])
		{
			// m_pOldBmp는 포인터만 사용할 것이므로 new 할당 필요없음
			m_pOldBmp[i] = (CBitmap*) m_pbufferDC[i]->SelectObject(m_pmemBmp[i]);
		}
	}

	// 파일로부터 읽어들인 실제 이미지 버퍼 ( tile image num 개수 만큼 한번에 읽음)
	DrawImageOnMemDC( m_pbufferDC[REAL_BACKGROUND_NUM], tileImageStartFont, tileImageNum);

	// 파일로부터 읽어서 마스크 비트맵을 만듦
	// 투명부분 : 흰색
	// 색깔부분 : 검은색
	 //( tile image num 개수 만큼 한번에 읽음)
	DrawImageOnMemDC(m_pbufferDC[MASK_BACKGROUND_NUM], tileImageStartFont, tileImageNum, WHITE_COLOR, TRANSPARENT_COLOR, TRANSPARENT_COLOR, true);
}

void CDiagramEntity::BitBltDS(CDC* dest, CDC* src, int dest_width, int dest_height, DWORD mode, int src_x, int src_y, int dest_x, int dest_y)
{
	double zoom_width=0, zoom_height=0;

	zoom_width = dest_width/GetZoom();
	zoom_height = dest_height/GetZoom();

	//if(	zoom_width > m_spr.clHeader.siXsize )
	//	zoom_width = m_spr.clHeader.siXsize;
	//
	//if(	zoom_height > m_spr.clHeader.siYsize )
	//	zoom_height = m_spr.clHeader.siYsize;

	if(dest !=NULL && src !=NULL)
	{
		dest->StretchBlt(dest_x, dest_y, dest_width, dest_height, src, src_x, src_y, zoom_width, zoom_height, mode);
	}
}

void CDiagramEntity::DrawImageOnMemDC(CDC* dc, int tileImageStartFont, int tileImageNum, DWORD col_dest, DWORD col_src, DWORD col_exclusive_src, BOOL bexclusive )
{
	int i, j, k, m;
	DWORD color;
	unsigned char *psrc;

	// 이미지 데이터 로딩이 안되었거나 총 폰트 수 보다 타일 이미지 수가 더 크면 오류
	if(m_spr.Image == NULL || m_spr.clHeader.siFontNum < tileImageNum || m_spr.clHeader.siFontNum < tileImageStartFont + tileImageNum)
		return;

	cltTSpr *pSpr = &m_spr;
	CDC *pDC = dc;

	for(m = tileImageStartFont; m< tileImageStartFont+tileImageNum; m++)
	{
		psrc = &m_spr.Image[ m_spr.clHeader.clImgInfo[ m ].siStartPos ];

		for( i = 0; i < pSpr->clHeader.siYsize; ++i ) {

			j = 0;

			while( j < pSpr->clHeader.siXsize ) {

				if( *psrc == 254 ) {
				
					// pixel skip하기
					++psrc;

					for(k=0; k<*psrc; k++)
					{
						pDC->SetPixel( j + (m-tileImageStartFont)*(pSpr->clHeader.siXsize), i, col_dest );
						j++;
					}

					//j += *psrc;
					++psrc;

				} else {

					int cnt = 0;

					// DrawMpr이라면
					if(pSpr->clHeader.siSprType !=0)
					{
						cnt = *psrc;
						++psrc;
					}
					else // DrawSpr이라면
					{
						cnt = 1;
					}

					for( k = 0; k < cnt; ++k ) 
					{
						// 16팔레트에서 컬러 읽어오기
						WORD c, r, g, b; 

						c = pSpr->pal[ *psrc ];

						// r, g, b 값 추출  (565팔레트 기준 : LoadSpr에서 565로 읽었으므로)
						r = (c & RGB16_565_RMASK) >> 11;
						g = (c & RGB16_565_GMASK) >> 5;
						b = (c & RGB16_565_BMASK);

						// r(0~31), g(0~63), b(0~31) 값을 0~255범위의 값으로 변환

						r = r << 3;		// 5bit -> 8bit
						g = g << 2;		// 6bit -> 8bit
						b = b << 3;		// 5bit -> 8bit

						color = RGB( r, g, b );
						
						if( color == col_src )
							pDC->SetPixel( j + (m-tileImageStartFont)*(pSpr->clHeader.siXsize), i, col_dest );
						else
						{
							if(bexclusive)
								pDC->SetPixel( j + (m-tileImageStartFont)*(pSpr->clHeader.siXsize), i, col_exclusive_src);
							else
								pDC->SetPixel( j + (m-tileImageStartFont)*(pSpr->clHeader.siXsize), i, color);
						}

						++psrc;
						++j;
					}//for(k..)
				}
			} //while(j..)
		}// for(i..)
	}// for( m...)
}

// 타일 이미지로 어떤 컨트롤을 덮어 버린다. 단, 반드시 타일로 완전히 덮을 수 있어야 한다.
// 다이얼로그의 배경을 덮을 때도 유용하게 사용됨
// 1pixel 도 차이가 나지 않게 하기 위한것
// PutSprParam1 은 이미지의 Font Index 를 가리키는 것임
// PutSprParam2 는 PutSprOption이 PutSprAlphaT인 경우에 Alpha 값을 넣어줘야 함으로 그때 사용
// TileImgNum 은 1, 3, 9 셋중에 하나만 가능하다.
// 1개의 이미지로 덮을것이냐, 3개의 이미지로 덮을것이냐, 9개의 이미지로 덮을것이냐 차이지..
// IsRowTile 이것은 세로로만 타일을 덮을때 사용한다. 3개의 이미지로 기본적으로 가로로 덮겠지만,
// 스크롤 같은 경우는 세로로 타일 이미지를 덮음므로...
//enum
//{
//	SPR_PUTSPRT,
//	SPR_PUTSPRALPHAT,
//};

bool CDiagramEntity::DrawTileImg( CDC* dest, SI16 PutSprOption, /*SI16 PutSprParam1,*/ SI16 PutSprParam2, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 TileImgNum, bool IsRowTile)
{
	bool LeftTileXSize = false;
	bool LeftTileYSize = false;

	SI16 TileXSize = m_spr.clHeader.siXsize;
	SI16 TileYSize = m_spr.clHeader.siYsize;

	if( TileXSize < 1 || TileYSize < 1 ) return false;
	// 타일이 완전히 덮을 수 있는지 체크 ( 덜 덮거나, 초과해 버리면 안됨으로 )
	// 아래 3줄 주석 처리해 버리면, 아래 코드에 해당하는 경우에는 
	// 배경을 찍기는 하겠지만, 끝 부분을 안 찍게 됨.
	/*
	if ( ( Width % TileXSize ) != 0 || ( Height % TileYSize ) != 0 )
	{
	return false;
	}
	*/

	SI16 RowNum = Height / TileYSize;

	if ( Height % TileYSize != 0 )
	{
		LeftTileXSize = true;
	}

	SI16 ColumnNum = Width / TileXSize;
	if ( Width % TileXSize != 0 )
	{
		LeftTileYSize = true;
	}

	/*
	SI16 i = 0;
	SI16 j = 0;
	*/

	SI16 CurrentDrawXPos = XPos;
	SI16 CurrentDrawYPos = YPos;

	// 타일 3개로 이루어진 이미지
	if ( TileImgNum == 3 )
	{	
		if ( IsRowTile )
		{
			// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
			// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
			switch( PutSprOption )
			{
			case SPR_PUTSPRT:
				{
					for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
					{
						for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
						{
							if ( CurrentDrawXPos + TileXSize >= XPos + Width )
							{
								CurrentDrawXPos = XPos + Width - TileXSize;
							}
							else if ( CurrentDrawXPos + TileXSize > XPos + Width - TileXSize )
							{
								CurrentDrawXPos = XPos + Width - TileXSize * 2;
							}

							if ( CurrentDrawYPos + TileYSize > YPos + Height )
							{
								CurrentDrawYPos = YPos + Height - TileYSize;
							}

                            // 최소 사이즈 보다 작을 때 그리지 않는다.
							if( CurrentDrawXPos < XPos || CurrentDrawYPos < YPos ) return false;

							if ( CurrentDrawXPos < XPos + 1 )
							{
								//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else if ( CurrentDrawXPos >= XPos + Width - TileXSize )
							{	
								//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else
							{	
								//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 1 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}							
						}
					}
				}
				break;
			case SPR_PUTSPRALPHAT:
				{
					for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
					{
						for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
						{
							if ( CurrentDrawXPos + TileXSize >= XPos + Width )
							{
								CurrentDrawXPos = XPos + Width - TileXSize;
							}
							else if ( CurrentDrawXPos + TileXSize > XPos + Width - TileXSize )
							{
								CurrentDrawXPos = XPos + Width - TileXSize * 2;
							}

							if ( CurrentDrawYPos + TileYSize > YPos + Height )
							{
								CurrentDrawYPos = YPos + Height - TileYSize;
							}

                            // 최소 사이즈 보다 작을 때 그리지 않는다.
							if( CurrentDrawXPos < XPos || CurrentDrawYPos < YPos ) return false;

							if ( CurrentDrawXPos < XPos + 1 )
							{	
								//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1, PutSprParam2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else if ( CurrentDrawXPos >= XPos + Width - TileXSize )
							{	
								//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 2, PutSprParam2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else
							{	
								//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 1, PutSprParam2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}							
						}
					}
				}
				break;
			default:
				return false;
			}
		}
		else
		{
			// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
			// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
			switch( PutSprOption )
			{
			case SPR_PUTSPRT:
				{
					for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
					{
						for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
						{
							if ( CurrentDrawXPos + TileXSize > XPos + Width )
							{
								CurrentDrawXPos = XPos + Width - TileXSize;
							}

							if ( CurrentDrawYPos + TileYSize >= YPos + Height )
							{
								CurrentDrawYPos = YPos + Height - TileYSize;
							}
							else if ( CurrentDrawYPos + TileYSize > YPos + Height - TileYSize )
							{
								CurrentDrawYPos = YPos + Height - TileYSize * 2;
							}

                            // 최소 사이즈 보다 작을 때 그리지 않는다.
							if( CurrentDrawXPos < XPos || CurrentDrawYPos < YPos ) return false;

							if ( CurrentDrawYPos < YPos + 1 )
							{
								//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
							{	
								//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else
							{	
								//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 1 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}							
						}
					}
				}
				break;
			case SPR_PUTSPRALPHAT:
				{
					for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
					{
						for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
						{
							if ( CurrentDrawXPos + TileXSize > XPos + Width )
							{
								CurrentDrawXPos = XPos + Width - TileXSize;
							}

							if ( CurrentDrawYPos + TileYSize >= YPos + Height )
							{
								CurrentDrawYPos = YPos + Height - TileYSize;
							}
							else if ( CurrentDrawYPos + TileYSize > YPos + Height - TileYSize )
							{
								CurrentDrawYPos = YPos + Height - TileYSize * 2;
							}

                            // 최소 사이즈 보다 작을 때 그리지 않는다.
							if( CurrentDrawXPos < XPos || CurrentDrawYPos < YPos ) return false;

							if ( CurrentDrawYPos < YPos + 1 )
							{
								//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 0, PutSprParam2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
							{	
								//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 2, PutSprParam2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else
							{	
								//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 1, PutSprParam2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}							
						}
					}
				}
				break;
			default:
				return false;
			}
		}
	}
	else if ( TileImgNum == 9 )		// 타일 9개로 이루어진 이미지
	{
		// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
		// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
		switch( PutSprOption )
		{
		case SPR_PUTSPRT:
			{
				for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
				{
					for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
					{
						if ( CurrentDrawXPos + TileXSize > XPos + Width )
						{
							CurrentDrawXPos = XPos + Width - TileXSize;
						}

						if ( CurrentDrawYPos + TileYSize > YPos + Height )
						{
							CurrentDrawYPos = YPos + Height - TileYSize;
						}

                        // 최소 사이즈 보다 작을 때 그리지 않는다.
						if( CurrentDrawXPos < XPos || CurrentDrawYPos < YPos ) return false;

						if ( CurrentDrawXPos > XPos && CurrentDrawYPos > YPos && 
							CurrentDrawXPos < XPos + Width - TileXSize && CurrentDrawYPos < YPos + Height - TileYSize )
						{
							//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 4 );
							BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 4*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 4*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
						}
						else
						{
							if ( CurrentDrawXPos < XPos + 1 )
							{
								if ( CurrentDrawYPos < YPos + 1 )
								{
									//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
								else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
								{
									//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 6 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 6*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 6*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
								else
								{
									//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 3 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 3*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 3*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
							}
							else if ( CurrentDrawXPos >= XPos + Width - TileXSize )
							{
								if ( CurrentDrawYPos < YPos + 1 )
								{
									//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 2 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
								else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
								{
									//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 8 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 8*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 8*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
								else
								{
									//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 5 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 5*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 5*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
							}
							else if ( CurrentDrawYPos < YPos + 1 )
							{
								//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 1 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
							{
								//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 7 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 7*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 7*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
						}
					}
				}
			}
			break;
		case SPR_PUTSPRALPHAT:
			{
				for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
				{
					for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
					{
						if ( CurrentDrawXPos + TileXSize > XPos + Width )
						{
							CurrentDrawXPos = XPos + Width - TileXSize;
						}

						if ( CurrentDrawYPos + TileYSize > YPos + Height )
						{
							CurrentDrawYPos = YPos + Height - TileYSize;
						}

                        // 최소 사이즈 보다 작을 때 그리지 않는다.
						if( CurrentDrawXPos < XPos || CurrentDrawYPos < YPos ) return false;

						if ( CurrentDrawXPos > XPos && CurrentDrawYPos > YPos && 
							CurrentDrawXPos < XPos + Width - TileXSize && CurrentDrawYPos < YPos + Height - TileYSize )
						{
							//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 4, PutSprParam2 );
							BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 4*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 4*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
						}
						else
						{
							if ( CurrentDrawXPos < XPos + 1 )
							{
								if ( CurrentDrawYPos < YPos + 1 )
								{
									//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1, PutSprParam2 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
								else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
								{
									//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 6, PutSprParam2 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 6*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 6*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
								else
								{
									//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 3, PutSprParam2 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 3*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 3*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
							}
							else if ( CurrentDrawXPos >= XPos + Width - TileXSize )
							{
								if ( CurrentDrawYPos < YPos + 1 )
								{
									//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 2, PutSprParam2 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
								else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
								{
									//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 8, PutSprParam2 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 8*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 8*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
								else
								{
									//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 5, PutSprParam2 );
									BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 5*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
									BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 5*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								}
							}
							else if ( CurrentDrawYPos < YPos + 1 )
							{
								//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 1, PutSprParam2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
							else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
							{
								//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 7, PutSprParam2 );
								BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 7*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
								BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 7*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
							}
						}
					}
				}
			}
			break;
		default:
			return false;
		}
	}
	else if ( TileImgNum == 1 )		// 타일 1개로 이루어진 이미지 ( 제일 사용안 될것 같기에, if 문 최하위로 뒀음 )
	{
		// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
		// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
		switch( PutSprOption )
		{
		case SPR_PUTSPRT:
			{
				for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
				{
					for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
					{
						if ( CurrentDrawXPos + TileXSize > XPos + Width )
						{
							CurrentDrawXPos = XPos + Width - TileXSize;
						}

						if ( CurrentDrawYPos + TileYSize > YPos + Height )
						{
							CurrentDrawYPos = YPos + Height - TileYSize;
						}

                        // 최소 사이즈 보다 작을 때 그리지 않는다.
						if( CurrentDrawXPos < XPos || CurrentDrawYPos < YPos ) return false;

						//pTileImg->PutSprT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 );
						BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
						BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
					}
				}
			}
			break;
		case SPR_PUTSPRALPHAT:
			{
				for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
				{
					for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
					{
						if ( CurrentDrawXPos + TileXSize > XPos + Width )
						{
							CurrentDrawXPos = XPos + Width - TileXSize;
						}

						if ( CurrentDrawYPos + TileYSize > YPos + Height )
						{
							CurrentDrawYPos = YPos + Height - TileYSize;
						}

                        // 최소 사이즈 보다 작을 때 그리지 않는다.
						if( CurrentDrawXPos < XPos || CurrentDrawYPos < YPos ) return false;

						//pTileImg->PutSprAlphaT( CurrentDrawXPos, CurrentDrawYPos, PutSprParam1, PutSprParam2 );
						BitBltDS( dest, m_pbufferDC[MASK_BACKGROUND_NUM], TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
						BitBltDS( dest, m_pbufferDC[REAL_BACKGROUND_NUM], TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, CurrentDrawXPos, CurrentDrawYPos);
					}
				}
			}
			break;
		default:
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
	//SI16 TileXSize = m_spr.clHeader.siXsize;
	//SI16 TileYSize = m_spr.clHeader.siYsize;

	//// 타일이 완전히 덮을 수 있는지 체크 ( 덜 덮거나, 초과해 버리면 안됨으로 )
	//// 아래 3줄 주석 처리해 버리면, 아래 코드에 해당하는 경우에는 
	//// 배경을 찍기는 하겠지만, 끝 부분을 안 찍게 됨.
	///*
	//if ( ( Width % TileXSize ) != 0 || ( Height % TileYSize ) != 0 )
	//{
	//	return false;
	//}
	//*/
	//if(TileXSize < 1 || TileYSize < 1)
	//	return FALSE;

	//SI16 RowNum = Height / TileYSize;
	//SI16 ColumnNum = Width / TileXSize;

	//SI16 i = 0;
	//SI16 j = 0;

	//if(RowNum == 0 || ColumnNum == 0)
	//{
	//	BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), Width, Height, SRCAND, 0*TileXSize, 0, XPos + 0 * TileXSize, YPos + 0 * TileYSize);
	//	BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), Width, Height, SRCPAINT, 0*TileXSize, 0, XPos + 0 * TileXSize, YPos + 0 * TileYSize);
	//}

	//// 타일 3개로 이루어진 이미지
	//if ( TileImgNum == 3 )
	//{	
	//	if ( IsRowTile )
	//	{
	//		// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
	//		// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
	//		switch( PutSprOption )
	//		{
	//		case SPR_PUTSPRT:
	//			{
	//				for ( i = 0; i < RowNum; ++i )
	//				{
	//					for ( j = 0; j < ColumnNum; ++j )
	//					{
	//						if ( j < 1 )
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 0 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else if ( j >= ColumnNum - 1 )
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 2 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 1 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//					}
	//				}
	//			}
	//			break;
	//		case SPR_PUTSPRALPHAT:
	//			{
	//				for ( i = 0; i < RowNum; ++i )
	//				{
	//					for ( j = 0; j < ColumnNum; ++j )
	//					{
	//						//if ( j < 1 )
	//						//{
	//						//	pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 0, PutSprParam2 );

	//						//}
	//						//else if ( j >= ColumnNum - 1 )
	//						//{
	//						//	pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 2, PutSprParam2 );
	//						//}
	//						//else
	//						//{
	//						//	pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 1, PutSprParam2 );
	//						//}
	//						if ( j < 1 )
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 0 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else if ( j >= ColumnNum - 1 )
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 2 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 1 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//					}
	//				}
	//			}
	//			break;
	//		default:
	//			return false;
	//		}
	//	}
	//	else
	//	{
	//		// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
	//		// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
	//		switch( PutSprOption )
	//		{
	//		case SPR_PUTSPRT:
	//			{
	//				for ( i = 0; i < RowNum; ++i )
	//				{
	//					for ( j = 0; j < ColumnNum; ++j )
	//					{
	//						if ( i < 1 )
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 0 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else if ( i >= RowNum - 1 )
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 2 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 1 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//					}
	//				}
	//			}
	//			break;
	//		case SPR_PUTSPRALPHAT:
	//			{
	//				for ( i = 0; i < RowNum; ++i )
	//				{
	//					for ( j = 0; j < ColumnNum; ++j )
	//					{
	//						//if ( i < 1 )
	//						//{
	//						//	pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 0, PutSprParam2 );
	//						//}
	//						//else if ( i >= RowNum - 1 )
	//						//{
	//						//	pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 2, PutSprParam2 );
	//						//}
	//						//else
	//						//{
	//						//	pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 1, PutSprParam2 );
	//						//}
	//						if ( i < 1 )
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 0 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else if ( i >= RowNum - 1 )
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 2 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else
	//						{
	//							//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 1 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//					}
	//				}
	//			}
	//			break;
	//		default:
	//			return false;
	//		}
	//	}
	//}
	//else if ( TileImgNum == 9 )		// 타일 9개로 이루어진 이미지
	//{
	//	// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
	//	// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
	//	switch( PutSprOption )
	//	{
	//	case SPR_PUTSPRT:
	//		{
	//			for ( i = 0; i < RowNum; ++i )
	//			{
	//				for ( j = 0; j < ColumnNum; ++j )
	//				{
	//					// 외곽이 아닌 내부 영역
	//					if ( i > 0 && j > 0 && i < RowNum - 1 && j < ColumnNum - 1 )
	//					{
	//						//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 4 );
	//						BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 4*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 4*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);

	//					}
	//					// 외곽 영역
	//					else
	//					{
	//						if ( i < 1 )
	//						{
	//							if ( j < 1 )
	//							{
	//								//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 0 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND,  0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//							else if ( j >= ColumnNum - 1 )
	//							{
	//								//pTileImg->PutSprT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 2 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//							else
	//							{
	//								//pTileImg->PutSprT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 1 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//						}
	//						else if ( i >= RowNum - 1 )
	//						{
	//							if ( j < 1 )
	//							{
	//								//pTileImg->PutSprT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 6 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 6*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 6*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//							else if ( j >= ColumnNum - 1 )
	//							{
	//								//pTileImg->PutSprT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 8 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 8*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 8*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//							else
	//							{
	//								//pTileImg->PutSprT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 7 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 7*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 7*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//						}
	//						else if ( j < 1 )
	//						{
	//							//pTileImg->PutSprT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 3 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 3*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 3*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else if ( j >= ColumnNum - 1 )
	//						{
	//							//pTileImg->PutSprT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 5 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 5*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 5*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//					}
	//				}
	//			}
	//		}
	//		break;
	//	case SPR_PUTSPRALPHAT:
	//		{
	//			for ( i = 0; i < RowNum; ++i )
	//			{
	//				for ( j = 0; j < ColumnNum; ++j )
	//				{
	//					// 외곽이 아닌 내부 영역
	//					if ( i > 0 && j > 0 && i < RowNum - 1 && j < ColumnNum - 1 )
	//					{
	//						//pTileImg->PutSprAlphaT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 4, PutSprParam2 );
	//						BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 4*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 4*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);

	//					}
	//					// 외곽 영역
	//					else
	//					{
	//						if ( i < 1 )
	//						{
	//							if ( j < 1 )
	//							{
	//								//pTileImg->PutSprAlphaT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 0, PutSprParam2 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);

	//							}
	//							else if ( j >= ColumnNum - 1 )
	//							{
	//								//pTileImg->PutSprAlphaT( XPos +j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 2, PutSprParam2 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 2*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//							else
	//							{
	//								//pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 1, PutSprParam2 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 1*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//						}
	//						else if ( i >= RowNum - 1 )
	//						{
	//							if ( j < 1 )
	//							{
	//								//pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 6, PutSprParam2 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 6*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 6*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//							else if ( j >= ColumnNum - 1 )
	//							{
	//								//pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 8, PutSprParam2 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 8*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 8*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//							else
	//							{
	//								//pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 7, PutSprParam2 );
	//								BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 7*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//								BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 7*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							}
	//						}
	//						else if ( j < 1 )
	//						{
	//							//pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 3, PutSprParam2 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 3*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 3*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//						else if ( j >= ColumnNum - 1 )
	//						{
	//							//pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 5, PutSprParam2 );
	//							BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 5*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//							BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 5*TileXSize,0,  XPos + j * TileXSize, YPos + i * TileYSize);
	//						}
	//					}
	//				}
	//			}
	//		}
	//		break;
	//	default:
	//		return false;
	//	}
	//}
	//else if ( TileImgNum == 1 )		// 타일 1개로 이루어진 이미지 ( 제일 사용안 될것 같기에, if 문 최하위로 뒀음 )
	//{
	//	// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
	//	// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
	//	switch( PutSprOption )
	//	{
	//	case SPR_PUTSPRT:
	//		{
	//			for ( i = 0; i < RowNum; ++i )
	//			{
	//				for ( j = 0; j < ColumnNum; ++j )
	//				{
	//					//pTileImg->PutSprT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 );
	//					BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//					BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//				}
	//			}
	//		}
	//		break;
	//	case SPR_PUTSPRALPHAT:
	//		{
	//			for ( i = 0; i < RowNum; ++i )
	//			{
	//				for ( j = 0; j < ColumnNum; ++j )
	//				{
	//					//pTileImg->PutSprAlphaT( XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1, PutSprParam2 );
	//					BitBltDS( dest, GetBufferDC(MASK_BACKGROUND_NUM), TileXSize, TileYSize, SRCAND, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//					BitBltDS( dest, GetBufferDC(REAL_BACKGROUND_NUM), TileXSize, TileYSize, SRCPAINT, 0*TileXSize, 0, XPos + j * TileXSize, YPos + i * TileYSize);
	//				}
	//			}
	//		}
	//		break;
	//	default:
	//		return false;
	//	}
	//}
	//else
	//{
	//	return false;
	//}

	//return true;
}

/**
leon add
* m_picFullPath : 현재 컨트롤(엔티티) 이미지 경로
* m_oldPicFullPath : 이전 이미지(엔티티) 이미지 경로
* 가 서로 같은지 비교함
* return true : 서로 경로가 같을때 ( ""일때도 포함
* return false : 서로 경로가 다를 때
*/
BOOL CDiagramEntity::IsEqualPicPath()
{

	if(m_pProperty)
	{
		if(m_pProperty[BG_IMAGE_FULLPATH] == m_oldPicFullPath)
			return true;
		else
			return false;
	}

	//사용안함
	ASSERT( 1 == 0);
	return FALSE;

	//if(m_picFullPath != "" && m_picFullPath == OldPicFullPath)
	//if(m_picFullPath == m_oldPicFullPath)
	//	return true;
	//else
	//	return false;
}

//*****************************************************************


double CDiagramEntity::GetLeft() const
/* ============================================================
	Function :		CDiagramEntity::GetLeft
	Description :	Gets the left edge of the object rectangle
	Access :		Public

	Return :		double	-	Left position
	Parameters :	none

	Usage :			Call to get the left edge of the object. 
					Note that if minimum sizes are not set for 
					the object, the left edge might be bigger 
					than the right.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{
	
	if(m_pProperty)
		return _tstof(m_pProperty[X_COORD]);
		//return _ttoi(m_pProperty[X_COORD]);


//	ASSERT(1==0);
	return 0;
	//return m_left;

}

double CDiagramEntity::GetRight() const
/* ============================================================
	Function :		CDiagramEntity::GetRight
	Description :	Gets the right edge of the object 
					rectangle
	Access :		Public

	Return :		double	-	Right position
	Parameters :	none

	Usage :			Call to get the right edge of the object.
					Note that if minimum sizes are not set for 
					the object, the left edge might be bigger 
					than the right.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	if(m_pProperty)
		return _tstof(m_pProperty[X_COORD]) + _tstof(m_pProperty[WIDTH]);
		//return _ttoi(m_pProperty[X_COORD]) + _ttoi(m_pProperty[WIDTH]);

//	ASSERT(1==0);
	return 0;
	//return m_right;
}

double CDiagramEntity::GetTop() const
/* ============================================================
	Function :		CDiagramEntity::GetTop
	Description :	Gets the top edge of the object rectangle
	Access :		Public

	Return :		double	-	Top position
	Parameters :	none

	Usage :			Call to get the top edge of the object.
					Note that if minimum sizes are not set for 
					the object, the top edge might be bigger 
					than the bottom.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	if(m_pProperty)
		return _tstof(m_pProperty[Y_COORD]);
		//return _ttoi(m_pProperty[Y_COORD]);

//	ASSERT(1==0);
	return 0;

	//return m_top;
}

double CDiagramEntity::GetBottom() const
/* ============================================================
	Function :		CDiagramEntity::GetBottom
	Description :	Gets the bottom edge of the object 
					rectangle
	Access :		Public

	Return :		double	-	Bottom postion
	Parameters :	none

	Usage :			Call to get the bottom edge of the object.
					Note that if minimum sizes are not set for 
					the object, the top edge might be bigger 
					than the bottom.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	if(m_pProperty)
		return _tstof(m_pProperty[Y_COORD]) + _tstof(m_pProperty[HEIGHT]);
		//return _ttoi(m_pProperty[Y_COORD]) + _ttoi(m_pProperty[HEIGHT]);

//	ASSERT(1==0);
	return 0;

	//return m_bottom;
}

void CDiagramEntity::SetLeft( double left )
/* ============================================================
	Function :		CDiagramEntity::SetLeft
	Description :	Sets the left edge of the object rectangle
	Access :		Public

	Return :		void
	Parameters :	double left	-	New left position
					
	Usage :			Call to set the left edge of the object.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	if(m_pProperty)
	{
		//m_pProperty[X_COORD].Format("%f", left);
		m_pProperty[X_COORD].Format("%d", (int)left);
	}
	//else
 //       ASSERT(1==0);
	//m_left = left;
}

void CDiagramEntity::SetRight( double right )
/* ============================================================
	Function :		CDiagramEntity::SetRight
	Description :	Sets the right edge of the object 
					rectangle
	Access :		Public

	Return :		void
	Parameters :	double right	-	New right position
					
	Usage :			Call to set the right edge of the object.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	if(m_pProperty)
	{
		if(right > _ttoi(m_pProperty[X_COORD]))
			m_pProperty[WIDTH].Format("%d", (int)(right - _ttoi(m_pProperty[X_COORD])));
		else
			m_pProperty[WIDTH].Format("%d", (int)(_ttoi(m_pProperty[X_COORD]) - right));

		//if(right > _tstof(m_pProperty[X_COORD]))
		//	m_pProperty[WIDTH].Format("%f", right - _tstof(m_pProperty[X_COORD]));
		//else
		//	m_pProperty[WIDTH].Format("%f", _tstof(m_pProperty[X_COORD]) - right);
	}
	//else
 //       ASSERT(1==0);

	//m_right = right;

}

void CDiagramEntity::SetTop( double top )
/* ============================================================
	Function :		CDiagramEntity::SetTop
	Description :	Sets the top edge of the object rectangle
	Access :		Public

	Return :		void
	Parameters :	double top	-	New top position
					
	Usage :			Call to set the top edge of the object.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

//if( top == m_bottom )
//	top = top;
//	m_top = top;
	if(m_pProperty)
		//m_pProperty[Y_COORD].Format("%f", top);
		m_pProperty[Y_COORD].Format("%d", (int)top);
	//else
 //       ASSERT(1==0);

}

void CDiagramEntity::SetBottom( double bottom )
/* ============================================================
	Function :		CDiagramEntity::SetBottom
	Description :	Sets the bottom edge of the object 
					rectangle
	Access :		Public

	Return :		void
	Parameters :	double bottom	-	New bottom position
					
	Usage :			Call to set the bottom edge of the object.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	//m_bottom = bottom;
	if(m_pProperty)
	{
		if(bottom > _ttoi(m_pProperty[Y_COORD]))
			m_pProperty[HEIGHT].Format("%d", (int)(bottom - _ttoi(m_pProperty[Y_COORD])));
		else
			m_pProperty[HEIGHT].Format("%d", (int)(_ttoi(m_pProperty[Y_COORD]) - bottom));

		//if(bottom > _tstof(m_pProperty[Y_COORD]))
		//	m_pProperty[HEIGHT].Format("%f", bottom - _tstof(m_pProperty[Y_COORD]));
		//else
		//	m_pProperty[HEIGHT].Format("%f", _tstof(m_pProperty[Y_COORD]) - bottom);
	}
	//else
 //       ASSERT(1==0);

}

void CDiagramEntity::SetMarkerSize( CSize markerSize )
/* ============================================================
	Function :		CDiagramEntity::SetMarkerSize
	Description :	Gets the size of selection markers
	Access :		Protected
	Access :		Public

	Return :		void
	Parameters :	CSize markerSize	-	The new size of a 
											selection marker
					
	Usage :			Call to set a new selection marker size for 
					the object.

   ============================================================*/
{

	m_markerSize = markerSize;

}

CSize CDiagramEntity::GetMarkerSize() const
/* ============================================================
	Function :		CDiagramEntity::GetMarkerSize
	Description :	Gets the size of selection marker
	Access :		Protected
	Access :		Public

	Return :		CSize	-	The current size of a 
								selection marker
	Parameters :	none

	Usage :			Call to get the selection marker size for
					the object.

   ============================================================*/
{

	return m_markerSize;

}

void CDiagramEntity::SetMinimumSize( CSize minimumSize )
/* ============================================================
	Function :		CDiagramEntity::SetMinimumSize
	Description :	Sets the minimum size for instances of 
					this object.
	Access :		Public

	Return :		void
	Parameters :	CSize minimumSize	-	The minimum allowed 
											size
					
	Usage :			Call to set the minimum size of the object.
					It is not possible to resize an object to a 
					size smaller than the minimum allowed size.

   ============================================================*/
{

	m_minimumSize = minimumSize;

}

CSize CDiagramEntity::GetMinimumSize() const
/* ============================================================
	Function :		CDiagramEntity::GetMinimumSize
	Description :	Gets the minimum size for instances of 
					this object.
	Access :		Public

	Return :		CSize	-	The minimum allowed size
	Parameters :	none

	Usage :			Call to get the minimum size of the object.
					It is not possible to resize an object to a 
					size smaller than the minimum allowed size.

   ============================================================*/
{

	return m_minimumSize;

}

void CDiagramEntity::SetMaximumSize( CSize maximumSize )
/* ============================================================
	Function :		CDiagramEntity::SetMaximumSize
	Description :	Sets the maximum size for instances of 
					this object.
	Access :		Public

	Return :		void
	Parameters :	CSize maximumSize	-	The maximum allowed 
											size.
					
	Usage :			Call to set the maximum size of the object.
					It is not possible to resize an object to a 
					size larger than the maximum allowed size.

   ============================================================*/
{

	m_maximumSize = maximumSize;

}

CSize CDiagramEntity::GetMaximumSize() const
/* ============================================================
	Function :		CDiagramEntity::GetMaximumSize
	Description :	Returns the maximum size for instances of 
					this object.
	Access :		Public

	Return :		CSize	-	The maximum allowed size.
	Parameters :	none

	Usage :			Call to get the maximum size of the object.
					It is not possible to resize an object to a 
					size larger than the maximum allowed size.

   ============================================================*/
{

	return m_maximumSize;

}

void CDiagramEntity::SetConstraints( CSize min, CSize max )
/* ============================================================
	Function :		CDiagramEntity::SetConstraints
	Description :	Sets the minimum and maximum sizes for 
					instances of this object. -1 means no 
					constraints.
	Access :		Public

	Return :		void
	Parameters :	CSize min	-	Minimum size
					CSize max	-	Maximum size
					
	Usage :			Call to set the minimum and maximum sizes 
					of the object.
					It is not possible to resize an object to 
					smaller or bigger than the min- and max 
					size.

   ============================================================*/
{

	m_minimumSize = min;
	m_maximumSize = max;

}

CDiagramEntityContainer* CDiagramEntity::GetParent() const
/* ============================================================
	Function :		CDiagramEntity::GetParent
	Description :	Returns a pointer to the parent container.
	Access :		Protected

	Return :		CDiagramEntityContainer*	-	Parent
													container.
	Parameters :	none

	Usage :			Call to get the parent of the object.

   ============================================================*/
{

	return m_parent;

}

void CDiagramEntity::SetPropertyDialog( CDiagramPropertyDlg* dlg, UINT dlgresid )
/* ============================================================
	Function :		CDiagramEntity::SetPropertyDialog
	Description :	Sets the property dialog pointer.
	Access :		Protected

	Return :		void
	Parameters :	CDiagramPropertyDlg* dlg	-	a pointer 
													to a dialog 
													instance. 
					UINT dlgresid				-	The resource 
													id of the 
													dialog template.
					
	Usage :			Call to set a property dialog for the object 
					(normally in the "ctor"). 

   ============================================================*/
{

	m_propertydlg = dlg;
	m_propertydlgresid = dlgresid;

	if( dlg )
		m_propertydlg->SetEntity( this );

}

CDiagramPropertyDlg* CDiagramEntity::GetPropertyDialog() const
/* ============================================================
	Function :		CDiagramEntity::GetPropertyDialog
	Description :	Returns a pointer to the class property 
					dialog.
	Access :		Protected

	Return :		CDiagramPropertyDlg*	-	The dialog 
												pointer. "NULL" 
												if none.
	Parameters :	none

	Usage :			Call to get a pointer to the object property 
					dialog.

   ============================================================*/
{

	return m_propertydlg;

}

double CDiagramEntity::GetZoom() const
/* ============================================================
	Function :		CDiagramEntity::GetZoom
	Description :	Returns the zoom level for the object.
	Access :		Public

	Return :		double	-	
	Parameters :	none

	Usage :			Internal function. Can be called by derived 
					classes to get the zoom level. The zoom 
					level is set by the owning editor when 
					drawing the object, is read-only and this 
					function should only be called from "Draw".

   ============================================================*/
{

	return m_zoom;

}

void CDiagramEntity::SetZoom( double zoom )
/* ============================================================
	Function :		CDiagramEntity::SetZoom
	Description :	Sets the zoom level
	Access :		Protected

	Return :		nothing
	Parameters :	double zoom	-	The new zoom level
					
	Usage :			Internal call.

   ============================================================*/
{
	m_zoom = zoom;
}

int CDiagramEntity::GetGroup() const
/* ============================================================
	Function :		CDiagramEntity::GetGroup
	Description :	Gets the object group.
	Access :		Public

	Return :		int	-	Group of object
	Parameters :	none

	Usage :			Call to get the group for the object. All 
					objects in a group are selected together.

   ============================================================*/
{

	if(m_pProperty)
	{
		return _ttoi(m_pProperty[GROUP_NUM]);
	}
	else
        ASSERT(1==0);
//	return m_group;

}

void CDiagramEntity::SetGroup( int group )
/* ============================================================
	Function :		CDiagramEntity::SetGroup
	Description :	Sets the object group to "group".
	Access :		Public

	Return :		void
	Parameters :	int group	-	New group to set
					
	Usage :			Call to set a group for the object. All 
					objects in a group are selected together.

   ============================================================*/
{

	if(m_pProperty)
	{
		m_pProperty[GROUP_NUM].Format("%d", group);
	}
	//else
 //       ASSERT(1==0);
///	m_group = group;

}

CString CDiagramEntity::Export( UINT /*format*/ ) const
/* ============================================================
	Function :		CDiagramEntity::Export
	Description :	Exports the object to format
	Access :		Public

	Return :		CString		-	The object representation 
									in format.
	Parameters :	UINT format	-	The format to export to.
					
	Usage :			Virtual function to allow easy exporting of 
					the objects to different text based formats.

   ============================================================*/
{

	return _T( "" );

}

int CDiagramEntity::GetHitCode( const CPoint& point, const CRect& rect ) const
/* ============================================================
	Function :		CDiagramEntity::GetHitCode
	Description :	Returns the hit point constant for "point" 
					assuming the object rectangle "rect".
	Access :		Public

	Return :		int				-	The hit point, 
										"DEHT_NONE" if none.
	Parameters :	CPoint point	-	The point to check
					
	Usage :			Call to see in what part of the object point 
					lies. The hit point can be one of the following:
						"DEHT_NONE" No hit-point
						"DEHT_BODY" Inside object body
						"DEHT_TOPLEFT" Top-left corner
						"DEHT_TOPMIDDLE" Middle top-side
						"DEHT_TOPRIGHT" Top-right corner
						"DEHT_BOTTOMLEFT" Bottom-left corner
						"DEHT_BOTTOMMIDDLE" Middle bottom-side
						"DEHT_BOTTOMRIGHT" Bottom-right corner
						"DEHT_LEFTMIDDLE" Middle left-side
						"DEHT_RIGHTMIDDLE" Middle right-side

   ============================================================*/
{

	int result = DEHT_NONE;

	if( rect.PtInRect( point ) )
		result = DEHT_BODY;

	CRect rectTest;

	rectTest = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_TOPMIDDLE, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPMIDDLE;

	rectTest = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPRIGHT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMMIDDLE, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMMIDDLE;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMRIGHT;

	rectTest = GetSelectionMarkerRect( DEHT_LEFTMIDDLE, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_LEFTMIDDLE;

	rectTest = GetSelectionMarkerRect( DEHT_RIGHTMIDDLE, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_RIGHTMIDDLE;

	return result;

}

