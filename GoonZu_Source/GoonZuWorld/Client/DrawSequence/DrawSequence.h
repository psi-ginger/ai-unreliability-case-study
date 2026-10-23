#ifndef _DRAWSEQUENCE_H_
#define _DRAWSEQUENCE_H_

#include "DrawSequenceBaseInfo.h"

//class	CDrawSequenceBuffer;
class	CDrawSequenceMgr;

class CDrawSequence
{
public:
	CDrawSequence( SI16 DrawSequenceObjNum = MAX_DRAW_SEQUENCE_DATA_BUFFER_NUM );
	~CDrawSequence();

	void	Initialize();
	void	Destroy();

	void	Push( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize, cltDrawObjData *pclData );
	cltDrawObjData *Pop();

private:

	//CDrawSequenceBuffer		*m_pDrawSequenceBuffer;
	CDrawSequenceMgr		*m_pDrawSequenceMgr;




};


#endif