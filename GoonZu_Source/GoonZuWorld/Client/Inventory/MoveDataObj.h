#ifndef _MOVEDATAOBJ_H_
#define _MOVEDATAOBJ_H_

#include "ItemInventoryBaseDefinition.h"

class CMoveDataObj
{
public:
	CMoveDataObj( stMoveData * );
	CMoveDataObj( BYTE *pData, SI32 siSize );
	~CMoveDataObj();

	CMoveDataObj*	GetPre();
	void			SetPre( CMoveDataObj * );
	
	CMoveDataObj*	GetNext();
	void			SetNext( CMoveDataObj * );

	stMoveData*		GetMoveData();

	stMoveData		m_stMoveData;

	void			GetData( BYTE *pData );
	void			SetData( BYTE *pData, SI32 siSize );

private:
	CMoveDataObj	*m_pPre;
	CMoveDataObj	*m_pNext;

	TCHAR			m_pData[ MAX_BUFFER_INVENTORY_SIZE ];
	SI32			m_siSize;

	
};

#endif