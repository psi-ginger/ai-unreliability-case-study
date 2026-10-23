#ifndef _DRAWSEQUENCEOBJ_H_
#define _DRAWSEQUENCEOBJ_H_

#include "..\..\Common\SMLib\LibList\Obj.h"

#include "DrawSequenceBaseInfo.h"

class	CDrawSequenceObj : public CObj
{
public:
	CDrawSequenceObj();
	~CDrawSequenceObj();

	void	Initialize();
	void	Destroy();

	void	SetData( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize, cltDrawObjData *pclData );

	int		IsDrawingLater( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize );	
	cltDrawObjData*	GetDrawSequenceData();

	/*
	CDrawSequenceObj	*GetVerticalNext();
	void				SetVerticalNext( CDrawSequenceObj	*pObj );

	CDrawSequenceObj	*GetVerticalPre();
	void				SetVerticalPre( CDrawSequenceObj	*pObj );
	*/

private:

	SI32				m_si32XPos;
	SI32				m_si32YPos;

	SI32				m_si32XSize;
	SI32				m_si32YSize;
	
	cltDrawObjData		m_clDrawData;

	//char				m_DrawData[ MAX_DRAW_DATA_SIZE ];

	/*
	CDrawSequenceObj	*m_pVerticalPre;
	CDrawSequenceObj	*m_pVerticalNext;
	*/
	
};

#endif