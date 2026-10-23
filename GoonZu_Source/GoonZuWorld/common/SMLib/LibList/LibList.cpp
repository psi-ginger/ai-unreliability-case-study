#include "LibList.h"
#include "Obj.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CLibList::CLibList()
//	Desc : List 라이브러리 생성자
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CLibList::CLibList()
{
	m_pFirst = NULL;
	m_pLast = NULL;

	m_si32ObjNum = 0;

	InitializeCriticalSection( &m_LibListCrit );

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CLibList::~CLibList()
//	Desc : List 라이브러리 소멸자
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CLibList::~CLibList()
{
	Destroy();
	DeleteCriticalSection( &m_LibListCrit );
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::Initialize()
//	Desc : List 라이브러리 초기화
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::Initialize()
{
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::Destroy()
//	Desc : List 라이브러리 종료처리
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::Destroy()
{
	CObj *pTemp = NULL;

	while( m_pFirst )
	{
		pTemp = m_pFirst;
		m_pFirst = m_pFirst->GetNext();
		
		delete pTemp;
		pTemp = NULL;
	}

	m_pFirst = NULL;
	m_pLast = NULL;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::Create( int	si32MakeNodeNum )
//	Desc : List 노드 생성( 기본적으로 Max개의 노드를 생성하도록 하게 하기 위해서. - EmptyList, ExistList 두개가 있는데, 모든 노드를 EmptyList에 두게 하기 위해서임 )
//		   굳이 CObj를 상속 받을 필요 없이, CObj의 버퍼만으로도 사용이 가능하다면, 그냥 CObj 객체를 생성해서 사용하면 더 편리하므로...
//		   si32BufferSize 는 CObj의 버퍼 크기이다.
//	Date : 2003. 11. 20
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::Create( int si32MakeNodeNum, int si32BufferSize )
{
	CObj *pTemp = NULL;

	for ( int i = 0; i < si32MakeNodeNum; ++i )
	{
		pTemp = new CObj( si32BufferSize );
		PushBack( pTemp );
	}
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::Lock()
//	Desc : List 자료 처리 하기 전에 Lock 걸기
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::Lock()
{
	EnterCriticalSection( &m_LibListCrit );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::UnLock()
//	Desc : List 자료 처리 후에 UnLock 하기
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::UnLock()
{
	LeaveCriticalSection( &m_LibListCrit );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::PushFront( CObj *pObj )
//	Desc : List 앞에 추가
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::PushFront( CObj *pObj )
{
	pObj->ObjPtrInitialize();

	if ( m_pFirst )
	{
		m_pFirst->SetPre( pObj );
		pObj->SetNext( m_pFirst );
		m_pFirst = pObj;
	}
	else
	{
		m_pFirst = pObj;
		m_pLast = pObj;
	}

	++m_si32ObjNum;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::PushBack( CObj *pObj )
//	Desc : List 뒤에 추가
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::PushBack( CObj *pObj )
{
	pObj->ObjPtrInitialize();

	if ( m_pFirst )
	{
		m_pLast->SetNext( pObj );
		pObj->SetPre( m_pLast );
		m_pLast = pObj;
	}
	else
	{
		m_pFirst = pObj;
		m_pLast = pObj;
	}

	++m_si32ObjNum;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::PushIndex( int si32Index, CObj *pObj )
//	Desc : List si32Index번째에 추가 ( 약간의 테스트가 필요함 )
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::PushIndex( int si32Index, CObj *pObj )
{
#ifdef _DEBUG
	if ( si32Index > m_si32ObjNum )
	{
		MessageBox( NULL, TEXT("adfhlh3"), TEXT("23ui5y"), MB_OK|MB_TOPMOST );
		return;
	}
#endif

	if ( si32Index == 0 || si32Index == m_si32ObjNum )
	{
		if ( si32Index == 0 )
		{
			PushFront( pObj );
		}
		else
		{
			PushBack( pObj );
		}
	}
	else
	{
		CObj *pTemp = m_pFirst;

		for ( int i = 0; i < si32Index - 1; ++i )
		{
			pTemp = pTemp->GetNext();
		}

		pObj->SetNext( pTemp->GetNext() );
		pObj->SetPre( pTemp );

		pTemp->GetNext()->SetPre( pObj );
		pTemp->SetNext( pObj );
	}
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibList::PopFront()
//	Desc : List 앞에꺼 꺼냄 ( 꺼낸 후에 처리도 모두 함 )
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibList::PopFront()
{
	if ( m_pFirst )
	{
		CObj *pTemp = m_pFirst;

		if ( m_pFirst->GetNext() )
		{
			m_pFirst->GetNext()->SetPre( NULL );
			m_pFirst = m_pFirst->GetNext();
		}
		else
		{
			m_pFirst = NULL;
			m_pLast = NULL;
		}

		--m_si32ObjNum;

		pTemp->ObjPtrInitialize();
		return pTemp;
	}
	else
	{
		return NULL;
	}
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibList::PopBack()
//	Desc : List 뒤에꺼 꺼냄 ( 꺼낸 후에 처리도 모두 함 )
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibList::PopBack()
{
	if ( m_pFirst )
	{
		CObj *pTemp = m_pLast;

		if ( m_pLast->GetPre() )
		{
			m_pLast->GetPre()->SetNext( NULL );
			m_pLast = m_pLast->GetPre();
		}
		else
		{
			m_pFirst = NULL;
			m_pLast = NULL;
		}

		--m_si32ObjNum;

		pTemp->ObjPtrInitialize();
		return pTemp;
	}
	else
	{
		return NULL;
	}
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::Pop( CObj *pObj )
//	Desc : 특정 노드( 파라미터로 입력한 CObj *pObj) 만 리스트에서 빼기
//		   파라미터 값이 빼는 노드이기 때문에 굳이 리턴 값으로 객체 포인터 할 필요 없음... 당연한 것이지용...
//	Date : 2003. 10. 01
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::Pop( CObj *pObj )
{
	/*

#ifdef _DEBUG
	if ( m_si32ObjNum == 0 )
	{
		MessageBox( NULL, TEXT("Pop 할게 없는데... 어찌된일인지"), TEXT("Pop 할게 없다."), MB_OK|MB_TOPMOST );
		return;
	}
#endif

	// 후에 최적화 시킬때에는 이 부분의 주석을 해제 시켜서 더 빠른 속도를 기대해 봄
	if ( m_si32ObjNum == 1 )
	{
		m_pFirst = NULL;
		m_pLast = NULL;

		--m_si32ObjNum;
		pObj->ObjPtrInitialize();

		return;
	}
	*/

	if ( pObj->GetPre() )
	{
		pObj->GetPre()->SetNext( pObj->GetNext() );
	}
	else				// pObj 가 m_pFirst 인 경우이지...
	{
		if ( pObj->GetNext() )
		{
			m_pFirst = pObj->GetNext();
			m_pFirst->SetPre( NULL );
		}
		else
		{
			m_pFirst = NULL;
			m_pLast = NULL;
		}
	}

	if ( pObj->GetNext() )
	{
		pObj->GetNext()->SetPre( pObj->GetPre() );
	}
	else				// pObj 가 m_pLast 인 경우이지...
	{
		if ( pObj->GetPre() )
		{
			m_pLast = pObj->GetPre();
			m_pLast->SetNext( NULL );
		}
		else
		{
			m_pFirst = NULL;
			m_pLast = NULL;
		}
	}

	--m_si32ObjNum;

	pObj->ObjPtrInitialize();

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - int CLibList::GetObjNum()
//	Desc : List 에 있는 객체의 숫자를 리턴해 줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
int CLibList::GetObjNum()
{
	return m_si32ObjNum;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibList::GetFirst()
//	Desc : List 에 있는 맨 처음의 노드를 리턴해줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibList::GetFirst()
{
	return m_pFirst;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibList::GetLast()
//	Desc : List 에 있는 맨 끝의 노드를 리턴해줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibList::GetLast()
{
	return m_pLast;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::SetFirst( CObj	*pObj )
//	Desc : List 제일 앞의 노드 설정 ( 주의 설정과 관계된 부분들은 이 함수 호출 전에 처리 되어야 함. 노드 옮긴다는지 하는 것 )
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::SetFirst( CObj	*pObj )
{
	m_pFirst = pObj;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibList::SetFirst( CObj	*pObj )
//	Desc : List 제일 뒤의 노드 설정 ( 주의 설정과 관계된 부분들은 이 함수 호출 전에 처리 되어야 함. 노드 옮긴다는지 하는 것 )
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibList::SetLast( CObj	*pObj )
{
	m_pLast = pObj;
	return;
}
