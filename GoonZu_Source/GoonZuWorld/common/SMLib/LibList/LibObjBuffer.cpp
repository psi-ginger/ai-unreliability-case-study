#include "LibObjBuffer.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CLibObjBuffer::CLibObjBuffer( UI32 uiMaxBufferSize )
//	Desc : LibBuffer 생성자
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------
CLibObjBuffer::CLibObjBuffer( UI32 uiMaxBufferSize )
{
	m_uiDataSize = 0;

	m_pData = new BYTE[ uiMaxBufferSize ];

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CLibObjBuffer::~CLibObjBuffer()
//	Desc : LibBuffer 소멸자
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------
CLibObjBuffer::~CLibObjBuffer()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibObjBuffer::Initialize()
//	Desc : LibBuffer 초기화
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibObjBuffer::Initialize()
{
	if( m_pData )
	{
		memset( m_pData, 0, m_uiDataSize );
		m_uiDataSize = 0;
	}

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibObjBuffer::Initialize()
//	Desc : LibBuffer 종료처리
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibObjBuffer::Destroy()
{
	if ( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;

		m_uiDataSize = 0;
	}

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibObjBuffer::SetData( BYTE *pData, UI32 ui32DataSize )
//	Desc : LibBuffer 데이터를 버퍼에 삽입
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibObjBuffer::SetData( BYTE *pData, UI32 ui32DataSize )
{
	memcpy( m_pData, pData, ui32DataSize );

	m_uiDataSize = ui32DataSize;
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibObjBuffer::GetData( BYTE *pData, UI32* pDataSize )
//	Desc : LibBuffer 버퍼로부터 데이터를 읽어서 파라미터로 입력한 pData 에 넣어줌
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------	
void CLibObjBuffer::GetData( BYTE *pData, UI32* pDataSize )
{
#ifdef _DEBUG
	if ( ! IsDataExist() )
	{
		MessageBox( NULL, TEXT("읽을 데이터가 없는데, 읽으라고 합니다."), 
			TEXT("void CLibObjBuffer::ReadData( BYTE *pData, SI32 *pDataSize )"), MB_OK|MB_TOPMOST );
		return;
	}
#endif

	memcpy( pData, m_pData, m_uiDataSize );
	*pDataSize = m_uiDataSize;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - BYTE *CLibObjBuffer::GetDataPtr()
//	Desc : LibBuffer 버퍼의 데이터 포인터를 반환
//					 포인터로 데이터 접근 정도로 사용할 것이면 
//					 이게 훨씬 빠르므로..., 
//					 단 주의할 점은 읽는 동안에는 데이터가 절대 변해서는 안 된다는 것이다.
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------	
BYTE *CLibObjBuffer::GetDataPtr()
{
#ifdef _DEBUG
	if ( ! IsDataExist() )
	{
		MessageBox( NULL, TEXT("읽을 데이터가 없는데, 읽으라고 합니다."), 
			TEXT("void CLibObjBuffer::ReadData( BYTE *pData, SI32 *pDataSize )"), MB_OK|MB_TOPMOST );
		return NULL;
	}
#endif*
	
	return m_pData;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CLibObjBuffer::IsDataExist()
//	Desc : LibBuffer 데이터 사이즈를 받아오는 것보다 데이터 있느냐 없느냐만 체크하는게 주된 것이므로 데이터가 있느냐만 체크 ( 이게 성능이 더 좋음 )	
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------	
bool CLibObjBuffer::IsDataExist()
{
	if ( m_uiDataSize > 0 )
	{
		return true;
	}
	
	return false;	
}