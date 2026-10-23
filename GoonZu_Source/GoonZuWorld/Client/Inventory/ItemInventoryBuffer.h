#ifndef _ITEMINVENTORYBUFFER_H_
#define _ITEMINVENTORYBUFFER_H_

#include "ItemInventoryBaseDefinition.h"

class	CMoveDataObj;

class CItemInventoryBuffer  
{
public:
	CItemInventoryBuffer();
	~CItemInventoryBuffer();

	void Initialize();
	void Destroy();

	//void Push( SI32 siStartPos, SI32 siEndPos, cltItem clItem );
	void Push( SI16 Msg, SI32 lParam, cltItem *pclItem );
	void Push( SI16 Msg, SI32 lParam, SI32 lParam1 );
	void Push( SI16 Msg, SI32 lParam, SI32 lParam1, SI16 lParam2, cltItem *pclItem );
	void Push( BYTE *pData, SI32 siSize );
	//stMoveData *Pop();
	bool Pop( stMoveData *pMoveData );
	bool Pop( BYTE *pData );
	

	CMoveDataObj	*GetFirst();
	void			SetFirst( CMoveDataObj *pMoveDataObj );
	
	CMoveDataObj	*GetLast();
	void			SetLast( CMoveDataObj	*pMoveDataObj );

	SI32			GetDataCount();

	
private:

	CMoveDataObj		*m_pFirst;
	CMoveDataObj		*m_pLast;

	SI32				m_siMoveDataCount;	

};

#endif