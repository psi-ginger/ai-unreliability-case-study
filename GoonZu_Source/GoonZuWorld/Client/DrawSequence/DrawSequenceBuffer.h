#ifndef _DRAWSEQUENCEBUFFER_H_
#define _DRAWSEQUENCEBUFFER_H_

#include "DrawSequenceBaseInfo.h"

class CDrawSequenceDataObj;

class CDrawSequenceBuffer
{
public:
	CDrawSequenceBuffer();
	~CDrawSequenceBuffer();

	void	Initialize();
	void	Destroy();
	
	void	Push( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize, char *pData );
	bool	Pop( char *pData, SI32 si32Size );

private:

	CDrawSequenceDataObj	*m_pFirst;
	CDrawSequenceDataObj	*m_pLast;

	SI32				m_siDrawSequenceDataObjCount;	

};


#endif