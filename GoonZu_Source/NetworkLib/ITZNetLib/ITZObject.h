#ifndef _ITZOBJECT_H
#define _ITZOBJECT_H

#include "itzbase.h"

class ITZObject
{
friend class ITZQList;

public:
	ITZObject() : m_pPrev(0), m_pNext(0), m_bActive(0) {};
	virtual ~ITZObject() {};

	virtual void		Init() {};
	virtual void		Close() {};

	ITZObject*			GetNext() { return m_pNext; };
	ITZObject*			GetPrev() { return m_pPrev; };

	void				SetActive( BOOL bActive ) { InterlockedExchange( (LPLONG)&m_bActive, (LONG)bActive ); };
	BOOL				IsActive() { return m_bActive; };

public:
	UI16				m_usIndex;

private:
	ITZObject*			m_pPrev;
	ITZObject*			m_pNext;

	BOOL				m_bActive;
};

class ITZObjectPtr : public ITZObject
{
public:
	ITZObjectPtr() {};
	virtual ~ITZObjectPtr() {};

	void				SetObject( ITZObject *pObject ) { m_pObject = pObject; };
	ITZObject*			GetObject() { return m_pObject; };

private:
	ITZObject*			m_pObject;
};

#endif
