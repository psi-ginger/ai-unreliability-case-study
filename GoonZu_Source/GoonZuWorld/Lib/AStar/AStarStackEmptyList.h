#ifndef _ASTARSTACKEMPTYLIST_H_
#define _ASTARSTACKEMPTYLIST_H_

class CAStarNode;
struct stAStarStack;

class CAStarStackEmptyList  
{
public:
	CAStarStackEmptyList( int Max_Node_Num );
	~CAStarStackEmptyList();

	void Initialize();
	void Destroy();

	void Push( stAStarStack *pStack );
	stAStarStack *Pop();

	int GetNodeNum();

private:	

	stAStarStack *m_pFirst;
	

};

#endif