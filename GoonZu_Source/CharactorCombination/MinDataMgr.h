#ifndef _MINDATAMGR_H_
#define _MINDATAMGR_H_

#include "BaseDefinition.h"

class CMinDataObj;

class CMinDataMgr  
{
public:
	CMinDataMgr();
	~CMinDataMgr();

	void Initialize();
	void Destroy();

	void FindMinFiles();
	
	CMinDataObj *GetFirst();
	void SetFirst( CMinDataObj *pFirst );

	CMinDataObj *GetLast();
	void SetLast( CMinDataObj *pLast );

	void PushBack( CMinDataObj *pNode );

	CMinDataObj *GetSameMinDataObjPtr( char *strFileName );

	char *GetFileName( int siIndex );

			
private:
	CMinDataObj *m_pFirst;
	CMinDataObj *m_pLast;

	bool m_bIsFindBefore;
	int m_siMinFileNum;
};

#endif