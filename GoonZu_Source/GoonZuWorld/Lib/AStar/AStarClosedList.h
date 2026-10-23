#ifndef _ASTARCLOSEDLIST_H_
#define _ASTARCLOSEDLIST_H_

#include "List.h"

class CAStarNode;

class CAStarClosedList : public CAStarList
{
public:
	CAStarClosedList();	
	~CAStarClosedList();

	void Initialize();
	void Destroy();

	CAStarNode *Pop();

private:
	
};

#endif
