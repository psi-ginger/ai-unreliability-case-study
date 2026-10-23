#ifndef _ASTARLISTMGR_H_
#define _ASTARLISTMGR_H_

#include "../../common/commonheader.h"


class CAStarOpenedList;
class CAStarClosedList;
class CAStarNotUsingNodeList;
class CAStarNode;

class CAStarStackListMgr;

//typedef bool (__stdcall *IsBlocking)( int, int );

#define MAX_NODE_NUM	120								// 최대 찾기 가능한 횟수는 800/8 = 100(최대 루프 도는 횟수) 번이 됨.
//SI32	MAX_NODE_NUM = 240;


class CAStarListMgr
{
public:
	CAStarListMgr( void *pParent );	// 생 성 자
	~CAStarListMgr();									// 소 멸 자

	void Initialize();									// 초 기 화
	void Destroy();										// 종료처리

	CAStarOpenedList *GetOpenedList();					// OpendList 포인터 얻어옮
	CAStarClosedList *GetClosedList();					// ClosedList 포인터 얻어옮

	//IsBlocking isblockfunc;
	bool (*IsBlocking)(int, int, int, int, void*);

	CAStarNode *GetBest();								// m_pOpendList 에서 제일 앞에 있는 노드를 리턴하고, 그 노드를 m_pClosedList 제일 앞에 넣음.

	CAStarNode *GetBestPtr();							// m_pBestPtr의 포인터 리턴해줌

	bool FindNextPath( int siStartXIndex, int siStartYIndex, int siEndXIndex, int siEndYIndex, int *siNextXIndex, int *siNextYIndex, bool (*IsBlocked)( int, int, void * ) );	// 다음 움직일 좌표를 리턴해줌
	bool FindNextPath( bool bNotUseAStar, int siStartXIndex, int siStartYIndex, int siEndXIndex, int siEndYIndex, cltPos *pNextPos, int *psiRetNum );		// 다음 움직일 좌표를 리턴해줌

	void GetNextPath( CAStarNode* pNode, int siStartXIndex, int siStartYIndex, int *siNextXIndex, int *siNextYIndex );					// 다음 갈 인덱스 검색 하는 곳
	void GetNextPath( CAStarNode* pNode, int siStartXIndex, int siStartYIndex, cltPos *pNextPos, int *psiRetNum );	// 다음 갈 곳들 갯수만큼 인덱스 검색 하는 곳
	//void GetNextPathNotFoundEndPos( int siStartXIndex, int siStartYIndex, int *siNextXIndex, int *siNextYIndex );	// 다음 갈 인덱스 검색 하는 곳 ( 완전 경로를 찾지 못했을 시에도 어딘가로는 근접하게 다가가야 하므로...)	
	void GetNextPathNotFoundEndPos( int siStartXIndex, int siStartYIndex, cltPos *pNextPos, int *psiRetNum);		// 최저값의 heuristic 을 조사한다. 그런데 약간의 문제가 있다. 최저값과 같은 것이 있는 경우에 대해서는 처리 하지 않기 때문에 도착지점은 같더라도 돌아가는 경우가 발생할 수 있다. 이것은 같은 최저값을 체크해서 더 최적의 경로를 찾아 갈 수 있지만 거의 없는 일이라 가정이 되는 관계로, 맨 먼저 발견되는 최저값만 체크한다.
	bool CreateChildren( CAStarNode *pParent );			// 차일드 생성( 생성이라기 보다는 m_pNotUsingNodeList 에서 노드 하나 꺼내와서 포인터 이동이 더 정확한 뜻 )
	void UpdateParents( CAStarNode *pNode );			// 부모 변경

	int Step();											// 스텝

	void SetMode( bool Mode )
	{
		m_bMode = Mode;
	}

private:
	void *m_pParentPtr;									// 이 객체를 생성한 객체(부모)의 포인터

	CAStarOpenedList *m_pOpenedList;					// OpenedList
	CAStarClosedList *m_pClosedList;					// ClosedList

	int m_siStartXIndex;
	int m_siStartYIndex;

	int m_siEndXIndex;									// 도착 Tile X Index
	int m_siEndYIndex;									// 도착 Tile Y Index 

	CAStarNotUsingNodeList *m_pNotUsingNodeList;		// 현재 메모리에서 쓰여지고 있지 않는 노드들의 리스트 ( 서버에서 쓸 것이므로, new 와 delete 를 하지 않게 하기 위해서임. )

	CAStarStackListMgr *m_pStackListMgr;				// Stack 의 메모리 관리를 위해서...

	CAStarNode *m_pBest;

	//CAStarNode *m_pPathList[ MAX_NODE_NUM ];			// 길 찾아 가는 것의 경로를 가지고 있음.
	int			m_siPathListNum;						// 길 찾아 가는 것의 경로가 몇개 있는지...

	int			m_siMaxFindPathNum;						// 최대 길 찾는 개수.

	bool		m_bMode;								// true : 8방향, false : 4 방향 

};

#endif
