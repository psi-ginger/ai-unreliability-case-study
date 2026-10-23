#ifndef _ITZQLIST_H
#define _ITZQLIST_H

#include "itzbase.h"

class ITZObject;

// push, pop은 동시에 서로 다른 스레드에서 동시에 처리가 가능하다 
// getFirst, getLast, remove 등은 단일 스레드에서만 사용하여야 한다

class ITZQList
{
public:
	ITZQList();
	~ITZQList();

public:

	ITZObject*			getFirst() { return m_pFirst; };
	ITZObject*			getLast() { return m_pLast; };

	void				clear();

	void				push( ITZObject *pElem );
	ITZObject*			pop();

	void				remove( ITZObject *pElem );
	
	SI32				size() { return m_siNumberOfElements; };

private:
	
	ITZObject*			m_pFirst;
	ITZObject*			m_pLast;

	SI32				m_siNumberOfElements;

	CRITICAL_SECTION	m_cs;

};

#endif