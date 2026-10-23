#ifndef _LIBLIST_H_
#define _LIBLIST_H_

#include "LibListBaseInfo.h"

class CObj;

class CLibList
{
public:
	CLibList();
	virtual	~CLibList();

	void	Initialize();
	void	Destroy();

	void	Create( int	si32MakeNodeNum, int si32BufferSize );

	void	Lock();
	void	UnLock();
	
	void	PushFront( CObj *pObj );
	void	PushBack( CObj *pObj );	
	void	PushIndex( int si32Index, CObj *pObj );

	CObj	*PopFront();
	CObj	*PopBack();
	
	CObj	*PopFrontOK();
	CObj	*PopBackOK();

	void	Pop( CObj *pObj );

	CObj	*GetFirst();
	CObj	*GetLast();

	void	SetFirst( CObj	*pObj );
	void	SetLast( CObj	*pObj );

	int		GetObjNum();

private:

	CObj				*m_pFirst;
	CObj				*m_pLast;

	int					m_si32ObjNum;

	CRITICAL_SECTION	m_LibListCrit;
};

#endif