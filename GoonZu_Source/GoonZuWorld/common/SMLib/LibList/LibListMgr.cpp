#include "LibListMgr.h"
#include "LibList.h"
#include "Obj.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CLibListMgr::CLibListMgr( bool bCircleLinkedListMode )
//	Desc : List Manager 라이브러리 생성자
//		   bCircleLinkedListMode 는 노드가 다 찼을때, 새로운 노드를 추가하라고 하면 제일 처음에 썼던 노드를 초기화 하고, 그 노드를 제일 끝으로 재배치 시킴...
//		   쉽게 말하면 emptylist 에 노드가 없을때, existlist 에 제일 처음에 입력된 노드를 초기화 한 다음에, 그 것을 existlist 제일 끝에 놓는 것이지...
//		   서버에서 쓸때는 조심해야지....
//		   그래서 default 값으로 false 로 해 놓았음
//		   bCircleLinkedListMode 를 false 로 할 경우에는 m_bCircleLinkedListMode 도 필요 없기 때문에, 거기에 관계된 if 문도 모두 삭제해서 더 빠른 속도를 기대할 수 있게 된다.
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CLibListMgr::CLibListMgr( bool bCircleLinkedListMode )
{
	m_pExistList = new CLibList();
	m_pEmptyList = new CLibList();

	m_bCircleLinkedListMode = bCircleLinkedListMode;

	InitializeCriticalSection( &m_LibListMgrCrit );

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CLibListMgr::~CLibListMgr()
//	Desc : List Manager 라이브러리 소멸자
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CLibListMgr::~CLibListMgr()
{
	Destroy();

	DeleteCriticalSection( &m_LibListMgrCrit );
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibListMgr::Initialize()
//	Desc : List Manager 라이브러리 초기화
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibListMgr::Initialize()
{
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibListMgr::Destroy()
//	Desc : List Manager 라이브러리 종료 처리
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibListMgr::Destroy()
{
	if ( m_pExistList )
	{
		delete	m_pExistList;
		m_pExistList = NULL;
	}

	if ( m_pEmptyList )
	{
		delete m_pEmptyList;
		m_pEmptyList = NULL;
	}

	return;
}


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibListMgr::CreateMaxEmptyObj( int si32MaxObjNum, int si32BufferSize )
//	Desc : Empty List 에 최대 갯수를 생성해서 넣어주기 위해서
//		   굳이 CObj를 상속 받을 필요 없이, CObj의 버퍼만으로도 사용이 가능하다면, 그냥 CObj 객체를 생성해서 사용하면 더 편리하므로...
//		   유의할 점은 꼭 CObj 상속 받을 필요가 없는 경우에만 사용해야 한다는 것이다.
//		   두번째 파라미터는 CObj 의 버퍼 크기이다.
//	Date : 2003. 11. 20
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibListMgr::CreateMaxEmptyObj( int si32MaxObjNum, int si32BufferSize )
{
	m_pEmptyList->Create( si32MaxObjNum, si32BufferSize );
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibListMgr::Clear()
//	Desc : Exist 리스트에 있는걸 전부 Empty 리스트 에 넣어줌.
//	Date : 2004. 5. 27
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibListMgr::Clear()
{
	Lock();

	bool unlimitloop = true;
	while ( unlimitloop )
	{
		CObj *pTemp = m_pExistList->PopFront();
		if ( pTemp == NULL )
		{
			break;
		}

		m_pEmptyList->PushBack( pTemp );
	}

	UnLock();
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibListMgr::Lock()
//	Desc : List Manager Lock
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibListMgr::Lock()
{
	EnterCriticalSection( &m_LibListMgrCrit );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CLibListMgr::UnLock()
//	Desc : List Manager UnLock
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
void CLibListMgr::UnLock()
{
	LeaveCriticalSection( &m_LibListMgrCrit );
	return;
}

void CLibListMgr::PushFrontToExistList( CObj *pObj )
{
	Lock();
	m_pExistList->PushFront( pObj );
	UnLock();
	return;
}

void CLibListMgr::PushBackToExistList( CObj *pObj )
{
	Lock();
	m_pExistList->PushBack( pObj );
	UnLock();
	return;
}

void CLibListMgr::PushFrontToEmptyList( CObj *pObj )
{
	Lock();
	m_pEmptyList->PushFront( pObj );
	UnLock();
	return;
}

void CLibListMgr::PushBackToEmptyList( CObj *pObj )
{
	Lock();
	m_pEmptyList->PushBack( pObj );
	UnLock();
	return;
}


CObj *CLibListMgr::PopFrontToExistList()
{
	return m_pExistList->PopFront();
}

CObj *CLibListMgr::PopBackToExistList()
{
	return m_pExistList->PopBack();
}

CObj *CLibListMgr::PopFrontToEmptyList()
{
	return m_pEmptyList->PopFront();
}

CObj *CLibListMgr::PopBackToEmptyList()
{
	return m_pEmptyList->PopBack();
}


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibListMgr::PopFrontToExistListAndPushFrontToEmptyList()
//	Desc : Exist 리스트의 앞에서 꺼내와서 Empty 리스트 앞에 넣어줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibListMgr::PopFrontToExistListAndPushFrontToEmptyList()
{
	Lock();
	CObj *pTemp = m_pExistList->PopFront();

	if ( pTemp == NULL )
	{
		if ( m_bCircleLinkedListMode )
		{
			pTemp = m_pEmptyList->PopBack();
#ifdef _DEBUG
			if ( pTemp == NULL )
			{
				MessageBox( NULL, TEXT("객체 생성 확인"), TEXT("객체 생성 확인"), MB_OK|MB_TOPMOST );
			}
#endif
			m_pEmptyList->PushFront( pTemp );
			UnLock();
			return pTemp;
		}

		UnLock();
		return NULL;
	}

	m_pEmptyList->PushFront( pTemp );
	UnLock();
	return pTemp;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibListMgr::PopFrontToExistListAndPushBackToEmptyList()
//	Desc : Exist 리스트의 앞에서 꺼내와서 Empty 리스트 뒤에 넣어줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibListMgr::PopFrontToExistListAndPushBackToEmptyList()
{
	Lock();
	CObj *pTemp = m_pExistList->PopFront();

	if ( pTemp == NULL )
	{
		if ( m_bCircleLinkedListMode )
		{
			pTemp = m_pEmptyList->PopFront();
#ifdef _DEBUG
			if ( pTemp == NULL )
			{
				MessageBox( NULL, TEXT("객체 생성 확인"), TEXT("객체 생성 확인"), MB_OK|MB_TOPMOST );
			}
#endif
			m_pEmptyList->PushBack( pTemp );
			UnLock();
			return pTemp;
		}

		UnLock();
		return NULL;
	}

	m_pEmptyList->PushBack( pTemp );
	UnLock();
	return pTemp;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibListMgr::PopBackToExistListAndPushFrontToEmptyList()
//	Desc : Exist 리스트의 뒤에서 꺼내와서 Empty 리스트 앞에 넣어줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibListMgr::PopBackToExistListAndPushFrontToEmptyList()
{
	Lock();
	CObj *pTemp = m_pExistList->PopBack();

	if ( pTemp == NULL )
	{
		if ( m_bCircleLinkedListMode )
		{
			pTemp = m_pEmptyList->PopBack();
#ifdef _DEBUG
			if ( pTemp == NULL )
			{
				MessageBox( NULL, TEXT("객체 생성 확인"), TEXT("객체 생성 확인"), MB_OK|MB_TOPMOST );
			}
#endif
			m_pEmptyList->PushFront( pTemp );
			UnLock();
			return pTemp;
		}

		UnLock();
		return NULL;
	}


	m_pEmptyList->PushFront( pTemp );
	UnLock();
	return pTemp;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibListMgr::PopBackToExistListAndPushBackToEmptyList()
//	Desc : Exist 리스트의 뒤에서 꺼내와서 Empty 리스트 뒤에 넣어줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibListMgr::PopBackToExistListAndPushBackToEmptyList()
{
	Lock();
	CObj *pTemp = m_pExistList->PopBack();

	if ( pTemp == NULL )
	{
		if ( m_bCircleLinkedListMode )
		{
			pTemp = m_pEmptyList->PopFront();
#ifdef _DEBUG
			if ( pTemp == NULL )
			{
				MessageBox( NULL, TEXT("객체 생성 확인"), TEXT("객체 생성 확인"), MB_OK|MB_TOPMOST );
			}
#endif
			m_pEmptyList->PushBack( pTemp );
			UnLock();
			return pTemp;
		}

		UnLock();
		return NULL;
	}

	m_pEmptyList->PushBack( pTemp );
	UnLock();
	return pTemp;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibListMgr::PopFrontToEmptyListAndPushFrontToExistList()
//	Desc : Empty 리스트의 앞에서 꺼내와서 Exist 리스트 앞에 넣어줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibListMgr::PopFrontToEmptyListAndPushFrontToExistList()
{
	Lock();
	CObj *pTemp = m_pEmptyList->PopFront();

	if ( pTemp == NULL )
	{
		if ( m_bCircleLinkedListMode )
		{
			pTemp = m_pExistList->PopBack();
#ifdef _DEBUG
			if ( pTemp == NULL )
			{
				MessageBox( NULL, TEXT("객체 생성 확인"), TEXT("객체 생성 확인"), MB_OK|MB_TOPMOST );
			}
#endif
			m_pExistList->PushFront( pTemp );
			UnLock();
			return pTemp;
		}

		UnLock();
		return NULL;
	}

	m_pExistList->PushFront( pTemp );
	UnLock();
	return pTemp;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibListMgr::PopFrontToEmptyListAndPushBackToExistList()
//	Desc : Empty 리스트의 앞에서 꺼내와서 Exist 리스트 뒤에 넣어줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibListMgr::PopFrontToEmptyListAndPushBackToExistList()
{
	Lock();
	CObj *pTemp = m_pEmptyList->PopFront();

	if ( pTemp == NULL )
	{
		if ( m_bCircleLinkedListMode )
		{
			pTemp = m_pExistList->PopFront();
#ifdef _DEBUG
			if ( pTemp == NULL )
			{
				MessageBox( NULL, TEXT("객체 생성 확인"), TEXT("객체 생성 확인"), MB_OK|MB_TOPMOST );
			}
#endif
			m_pExistList->PushBack( pTemp );
			UnLock();
			return pTemp;
		}

		UnLock();
		return NULL;
	}

	m_pExistList->PushBack( pTemp );
	UnLock();
	return pTemp;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibListMgr::PopBackToEmptyListAndPushFrontToExistList()
//	Desc : Empty 리스트의 뒤에서 꺼내와서 Exist 리스트 앞에 넣어줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibListMgr::PopBackToEmptyListAndPushFrontToExistList()
{
	Lock();
	CObj *pTemp = m_pEmptyList->PopBack();

	if ( pTemp == NULL )
	{
		if ( m_bCircleLinkedListMode )
		{
			pTemp = m_pExistList->PopBack();
#ifdef _DEBUG
			if ( pTemp == NULL )
			{
				MessageBox( NULL, TEXT("객체 생성 확인"), TEXT("객체 생성 확인"), MB_OK|MB_TOPMOST );
			}
#endif
			m_pExistList->PushFront( pTemp );
			UnLock();
			return pTemp;
		}
		
		UnLock();
		return NULL;
	}

	m_pExistList->PushFront( pTemp );
	UnLock();
	return pTemp;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj *CLibListMgr::PopBackToEmptyListAndPushBackToExistList()
//	Desc : Empty 리스트의 뒤에서 꺼내와서 Exist 리스트 뒤에 넣어줌
//	Date : 2003. 09. 22
//	Update : 
//--------------------------------------------------------------------------------------------
CObj *CLibListMgr::PopBackToEmptyListAndPushBackToExistList()
{
	Lock();
	CObj *pTemp = m_pEmptyList->PopBack();

	if ( pTemp == NULL )
	{
		if ( m_bCircleLinkedListMode )
		{
			pTemp = m_pExistList->PopFront();
#ifdef _DEBUG
			if ( pTemp == NULL )
			{
				MessageBox( NULL, TEXT("객체 생성 확인"), TEXT("객체 생성 확인"), MB_OK|MB_TOPMOST );
			}
#endif
			m_pExistList->PushBack( pTemp );
			UnLock();
			return pTemp;
		}

		UnLock();
		return NULL;
	}

	m_pExistList->PushBack( pTemp );
	UnLock();
	return pTemp;
}

int	CLibListMgr::GetExistListObjNum()
{
	return m_pExistList->GetObjNum();
}

int CLibListMgr::GetEmptyListObjNum()
{
	return m_pEmptyList->GetObjNum();
}

CLibList *CLibListMgr::GetExistList()
{
	return m_pExistList;
}

CLibList *CLibListMgr::GetEmptyList()
{
	return m_pEmptyList;
}