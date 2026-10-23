#ifndef _ASTATNOTUSINGNODELIST_H_
#define _ASTATNOTUSINGNODELIST_H_

class CAStarNode;

class CAStarNotUsingNodeList
{
public:

	CAStarNotUsingNodeList( int Max_Node_Num );
	~CAStarNotUsingNodeList();

	void Initialize();
	void Destroy();

	void AddNode( CAStarNode * );
	void DelNode( CAStarNode * );

	CAStarNode *GetLastNodeAndMoveLastPtr();

	int GetNodeNum();

private:

	CAStarNode *m_pFirst;
	CAStarNode *m_pLast;

};

#endif