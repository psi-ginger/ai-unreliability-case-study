#ifndef _LIBLiSTMGR_H_
#define _LIBLiSTMGR_H_

#include "LibListBaseInfo.h"

class	CLibList;
class	CObj;

class CLibListMgr
{
public:
	CLibListMgr( bool bCircleLinkedListMode = false );					// bCircleLinkedListMode 는 노드가 다 찼을때, 새로운 노드를 추가하라고 하면 제일 처음에 썼던 노드를 초기화 하고, 그 노드를 제일 끝으로 재배치 시킴...
																		// 쉽게 말하면 emptylist 에 노드가 없을때, existlist 에 제일 처음에 입력된 노드를 초기화 한 다음에, 그 것을 existlist 제일 끝에 놓는 것이지...
																		// 이것을 true로 사용하는 때는 이미지 로딩이 많을때, 노드가 부족하면 exist List 의 가장 옛날노드를 초기화 시켜서 대체 시키기 위한 것이지... 주로 클라이언트에서 사용이 되었음.
																		// 서버에서 쓸때는 조심해야지....
																		// 그래서 default 값으로 false 로 해 놓았음
																		// bCircleLinkedListMode 를 false 로 할 경우에는 CircalMode에 관계된 if 문도 모두 삭제해서 더 빠른 속도를 기대할 수 있게 된다.
	virtual	~CLibListMgr();

	void	Initialize();
	void	Destroy();

	void	CreateMaxEmptyObj( int si32MaxObjNum, int si32BufferSize );	// 생성시에 Empty List 에 최대 갯수를 생성해서 넣어주기 위해서.
	void	Clear();

	void	Lock();
	void	UnLock();

	void	PushFrontToExistList( CObj *pObj );
	void	PushBackToExistList( CObj *pObj );

	void	PushFrontToEmptyList( CObj *pObj );
	void	PushBackToEmptyList( CObj *pObj );


	CObj	*PopFrontToExistList();
	CObj	*PopBackToExistList();

	CObj	*PopFrontToEmptyList();
	CObj	*PopBackToEmptyList();

	// 실제로 위의 8개 함수들 보다는 아래의 8개 함수가 쓰일 것임. ( 아래 함수들이 위의 함수를 조합한 기능을 포함하므로, 더 편리하고 성능도 좋음. )
	CObj	*PopFrontToExistListAndPushFrontToEmptyList();
	CObj	*PopFrontToExistListAndPushBackToEmptyList();
	CObj	*PopBackToExistListAndPushFrontToEmptyList();
	CObj	*PopBackToExistListAndPushBackToEmptyList();

	CObj	*PopFrontToEmptyListAndPushFrontToExistList();
	CObj	*PopFrontToEmptyListAndPushBackToExistList();
	CObj	*PopBackToEmptyListAndPushFrontToExistList();
	CObj	*PopBackToEmptyListAndPushBackToExistList();

	int		GetExistListObjNum();
	int		GetEmptyListObjNum();

	CLibList	*GetExistList();
	CLibList	*GetEmptyList();

private:

	bool				m_bCircleLinkedListMode;

	CLibList			*m_pExistList;
	CLibList			*m_pEmptyList;

	CRITICAL_SECTION	m_LibListMgrCrit;
};

#endif