#ifndef _DRAWSEQUENCEMGR_H_
#define _DRAWSEQUENCEMGR_H_

#include "..\..\Common\SMLib\LibList\LibListMgr.h"
#include "DrawSequenceBaseInfo.h"

class	CDrawSequenceMgr : public CLibListMgr
{
public:
	CDrawSequenceMgr();
	~CDrawSequenceMgr();

	void	Initialize();
	void	Destroy();

	void	CreateMaxEmptyObj( int si32MaxObjNum );

	void	Push( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize, cltDrawObjData *pclData );
	cltDrawObjData *Pop();

private:

	CLibList	*m_pVerticalList;
	
	int			m_si32VerticalObjNum;

};

#endif