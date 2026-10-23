#ifndef _OBJ_H_
#define _OBJ_H_

#include "LibListBaseInfo.h"

class	CLibObjBuffer;

class	CObj
{
public:
	CObj( UI32 uiMaxBufferSize = 0 );		// 기본 데이터를 가질 버퍼가 필요없는 경우에는 파라미터 값으로 아무것도 넣지 않으면 됨.
	virtual	~CObj();

	void	Initialize();
	void	ObjPtrInitialize();
	void	Destroy();

	CObj	*GetPre();
	void	SetPre( CObj *pPre );

	CObj	*GetNext();
	void	SetNext( CObj *pNext );

	void	SetData( BYTE *pData, UI32 ui32DataSize );
	void	GetData( BYTE *pData, UI32 *pDataSize );
	BYTE	*GetDataPtr();

private:
	CObj			*m_pPre;
	CObj			*m_pNext;

	CLibObjBuffer	*m_pObjBuffer;

};

#endif