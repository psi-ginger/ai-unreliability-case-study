#ifndef _ASTARSTACKLIST_H_
#define _ASTARSTACKLIST_H_

class CAStarNode;
struct stAStarStack;

class CAStarStackList
{
public:
	CAStarStackList();
	~CAStarStackList();

	void Initialize();
	void Destroy();

	void Push( stAStarStack *pStack );
	stAStarStack *Pop();

	stAStarStack *GetFirst();

	int GetNodeNum();

private:
	stAStarStack *m_pFirst;	
};

#endif
