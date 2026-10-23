#ifndef _ASTARLIST_H_
#define _ASTARLIST_H_

class CAStarNode;

class CAStarList  
{
public:
	CAStarList();													// 생 성 자
	~CAStarList();													// 소 멸 자

	void			Initialize();								// 초 기 화
	void			Destroy();									// 종료 처리

	void			ListClear();								// 리스트 내의 노드들 완전 삭제
	
	//void	AddNode( CAStarNode *pNode );				// 리스트에 노드 포인터 추가
		
	CAStarNode		*IsExist( int siTileX, int siTileY );		// 리스트에 노드가 존재하는 지를 체크

	CAStarNode		*GetFirstNode();							// 리스트 제일 앞의 노드의 포인터를 리턴함.
	void			SetFirstNode( CAStarNode * );				// 리스트 제일 앞의 노드의 포인터를 설정함

	CAStarNode		*GetLastNode();								// 리스트 제일 뒤의 노드의 포인터를 리턴함.
	void			SetLastNode( CAStarNode * pNode );			// 리스트 제일 뒤의 노드의 포인터를 설정함
	
	int				GetNodeNum();								// 리스트에 객체가 총 몇 개인지...

private:
	CAStarNode *m_pFirst;									// 리스트 맨 앞의 노드
	CAStarNode *m_pLast;									// 리스트 맨 끝의 노드

	
};

#endif