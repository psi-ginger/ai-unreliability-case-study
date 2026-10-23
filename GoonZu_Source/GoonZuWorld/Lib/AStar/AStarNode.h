#ifndef _ASTARNODE_H_
#define _ASTARNODE_H_

#include <windows.h>

class CAStarNode
{
public:
	CAStarNode();								// 생 성 자
	~CAStarNode();								// 소 멸 자

	void Initialize();							// 초 기 화
	void Destroy();								// 종료 처리

	inline unsigned int GetTileXIndex()	{ return m_uiTileXIndex; };			// 타일 X 좌표 얻어 옮	
	void SetTileXIndex( int siTileXIndex );		// 타일 X 좌표 설정
	
	inline unsigned int GetTileYIndex()	{ return m_uiTileYIndex; };			// 타일 X 좌표 얻어 옮		
	void SetTileYIndex( int siTileYIndex );		// 타일 Y 좌표 설정

	float GetFitness();							// 적합도 값 얻어옮
	void SetFitness( float fFitness );			// 적합도 값 설정
	void SetFitness( int siStartXIndex, int siStartYIndex, int siEndXIndex, int siEndYIndex, int siGoal );	// 한번에 값들 설정하게 하기 위해서
	

	int GetGoal();								// 목표 값 얻어옮
	void SetGoal( int siGoal );					// 목표 값 설정

	
	float GetHeuristic();						// 휴리스틱 값 얻어옮
	//void SetHeuristic( float fHeuristic );		// 휴리스틱 값 설정
	

	CAStarNode *GetParentNode();				// Parent Node 포인터 얻어옮
	void SetParentNode( CAStarNode *pNode );	// Parent Node 포인터 설정

	inline CAStarNode *GetNextNode() { return m_pNext; };					// Next Node 포인터 얻어옮
	void SetNextNode( CAStarNode *pNode );		// Next Node 포인터 설정


	CAStarNode *GetEmptyPre();					// m_pEmptyPre 포인터 얻어옮
	void SetEmptyPre( CAStarNode *pNode );		// m_pEmptyPre 포인터 설정

	CAStarNode *GetEmptyNext();					// m_pEmptyNext 포인터 얻어옮
	void SetEmptyNext( CAStarNode *pNode );		// m_pEmptyNext 포인터 설정

	CAStarNode *GetChild( char cIndex);			// 차일드 얻어옮
	void SetChild( CAStarNode *pNode );			// 차일드 배정	
	char GetChildrenNum();						// 차일드 수 리턴


	
private:
	CAStarNode *m_pParent;						// Parent Node
	CAStarNode *m_pNext;						// OpenedList 또는 ClosedList 둘 중 하나는 반드시 추가되어야 하므로...

	CAStarNode *m_pEmptyPre;					// NotUsingList 에 있을 경우에는 양방향 링크드를 쓰므로...
	CAStarNode *m_pEmptyNext;					// NotUsingList 에 있을 경우에는 양방향 링크드를 쓰므로...

	float m_fFitness;							// f : 적 합 도
	int m_siGoal;								// g : 목    표
	float m_fHeuristic;							// h : 휴리스틱
	
	unsigned int m_uiTileXIndex;				// 어디에 있는 타일에 있는지 타일 X 좌표
	unsigned int m_uiTileYIndex;				// 어디에 있는 타일에 있는지 타일 Y 좌표

	CAStarNode *m_pChildren[8];					// 차일드가 될수 있는 방향은 8방향 ( 좌, 우, 상, 하, 좌상, 좌하, 우상, 우하 )
	char m_cChildrenNum;						// 차일드의 수
};

struct stAStarStack
{
	CAStarNode *m_pNode;
	stAStarStack *m_pNext;
};

#endif