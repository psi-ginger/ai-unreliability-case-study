#ifndef _DRAWSEQUENCEDATAOBJ_H_
#define _DRAWSEQUENCEDATAOBJ_H_

#include "DrawSequenceBaseInfo.h"

class CDrawSequenceDataObj
{
public:
	CDrawSequenceDataObj( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize, char *pData );
	~CDrawSequenceDataObj();

	CDrawSequenceDataObj*	GetPre();
	void					SetPre( CDrawSequenceDataObj *pDrawSequenceData );
	
	CDrawSequenceDataObj*	GetNext();
	void					SetNext( CDrawSequenceDataObj *pDrawSequenceData );

	bool					IsDrawingLater( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize );

	char*					GetDrawSequenceData();



private:
	SI32					m_si32XPos;
	SI32					m_si32YPos;

	SI32					m_si32XSize;
	SI32					m_si32YSize;
	
	char					m_DrawData[ MAX_DRAW_DATA_SIZE ];

	CDrawSequenceDataObj	*m_pPre;
	CDrawSequenceDataObj	*m_pNext;

};


#endif