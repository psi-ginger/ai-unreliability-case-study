#ifndef _ASTAROPENEDLIST_H_
#define _ASTAROPENEDLIST_H_

#include "List.h"

class CAStarNode;

class CAStarOpenedList : public CAStarList
{
public:
	CAStarOpenedList();										// 생 성 자
	~CAStarOpenedList();									// 소 멸 자

	void Initialize();										// 초 기 화
	void Destroy();											// 종료 처리

	void AddNode( CAStarNode *pNode );				// OpendList 에 추가 ( overloading 했음 )

	CAStarNode *Pop();										// 하나 끄집어 냄

private:

};

#endif