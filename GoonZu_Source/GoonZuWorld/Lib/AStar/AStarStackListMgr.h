#ifndef _ASTARSTACKLISTMGR_H_
#define _ASTARSTACKLISTMGR_H_

class CAStarNode;

class CAStarStackList;
class CAStarStackEmptyList;

class CAStarStackListMgr  
{
public:
	CAStarStackListMgr( int Max_Node_Num );
	~CAStarStackListMgr();

	void Initialize();
	void Destroy();

	void Push( CAStarNode * );
	CAStarNode *Pop();

	CAStarStackList *GetStackList();
	CAStarStackEmptyList *GetStackEmptyList();



	int GetStackNum();
	
private:

	CAStarStackList *m_pStackList;
	CAStarStackEmptyList *m_pStackEmptyList;

};

#endif
