#ifndef _SYSTEMNPCOBJ_H_
#define _SYSTEMNPCOBJ_H_

#include "SystemNPCBaseInfo.h"

class CSystemNPCObj : public CObj
{
public:
	CSystemNPCObj();
	~CSystemNPCObj();

	void	Initialize();
	void	Destroy();

	void	SetText( TCHAR *strText );
	void	Action();
	void	Draw();

private:

	SI32	m_si32NPCID;
	TCHAR	m_strNPCName[ MAX_TEXT_NUM ][ MAX_TEXT_LENGTH ];
	

};

#endif