#include "Obj.h"
#include "LibObjBuffer.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj::CObj( UI32 uiMaxBufferSize )
//	Desc : Obj 생성자
//		   Obj 에 기본 정보를 두기 위한 버퍼가 필요한 경우가 많아서 여기에서 기본 버퍼도 준비해 둠.
//		   만약, 기본 버퍼가 필요없다면 파라미터 값을 0으로 두면 됨.
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj::CObj( UI32 uiMaxBufferSize )
{
	m_pPre = NULL;
	m_pNext = NULL;

	if ( uiMaxBufferSize == 0 )
	{
		m_pObjBuffer = NULL;
	}
	else
	{
		m_pObjBuffer = new CLibObjBuffer( uiMaxBufferSize );
	}
	
	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj::~CObj()
//	Desc : Obj 소멸자
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj::~CObj()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CObj::Initialize()
//	Desc : Obj 초기화 ( 포인터 초기화 및 데이터가 있다면 데이터까지 초기화 )
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CObj::Initialize()
{
	m_pPre = NULL;
	m_pNext = NULL;

	if ( m_pObjBuffer )
	{
		m_pObjBuffer->Initialize();
	}

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CObj::ObjPtrInitialize()
//	Desc : Obj 초기화 ( 포인터만 초기화 )
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CObj::ObjPtrInitialize()
{
	m_pPre= NULL;
	m_pNext = NULL;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CObj::Destroy()
//	Desc : Obj 종료 처리
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CObj::Destroy()
{
	m_pPre = NULL;
	m_pNext = NULL;

	if ( m_pObjBuffer )
	{
		delete m_pObjBuffer;
		m_pObjBuffer = NULL;
	}

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CObj::GetPre()
//	Desc : 앞의 Obj 포인터 반환
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CObj::GetPre()
{
	return m_pPre;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CObj::SetPre( CObj *pPre )
//	Desc : 앞의 Obj 포인터 설정
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CObj::SetPre( CObj *pPre )
{
	m_pPre = pPre;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CObj::GetNext()
//	Desc : 뒤의 Obj 포인터 반환
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CObj::GetNext()
{
	return m_pNext;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CObj::SetNext( CObj *pNext )
//	Desc : 뒤의 Obj 포인터 설정
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CObj::SetNext( CObj *pNext )
{
	m_pNext = pNext;
	return;
}


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CObj::SetData( BYTE *pData, UI32 ui32DataSize )
//	Desc : Obj에 기본 데이터가 구조체로 되어 있을 경우, 그런 구조체들을 바이트 단위로 담을 수 있게 하기 위해서...
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------
void CObj::SetData( BYTE *pData, UI32 ui32DataSize )
{
	m_pObjBuffer->SetData( pData, ui32DataSize );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CObj::GetData( BYTE *pData, UI32 *pDataSize )
//	Desc : Obj에 기본 데이터를 읽어서 복사해서 넣어줌.
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------
void CObj::GetData( BYTE *pData, UI32 *pDataSize )
{
	m_pObjBuffer->GetData( pData, pDataSize );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - BYTE* CObj::GetDataPtr()
//	Desc : Obj에 기본 데이터의 포인터만 넘겨줌.
//		   쓸때, 주의해야 함. 단, 이것을 쓰면 위험한 반면 성능을 훨씬 좋음.
//		   이 함수를 쓴다는 것은, 이 함수가 불려서 넘겨진 데이터를 쓰는 동안에는 절대 데이터가 변경되지 않을 경우에만 사용 가능 함.
//	Date : 2003. 11. 14
//	Update : 
//--------------------------------------------------------------------------------------------
BYTE* CObj::GetDataPtr()
{
	return m_pObjBuffer->GetDataPtr();
}