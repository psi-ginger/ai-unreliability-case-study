#ifndef _NPCTRADEBUFFER_H_
#define _NPCTRADEBUFFER_H_

#include "NPCTradeBaseInfo.h"

class CNPCTradeData;

class CNPCTradeBuffer
{
public:
	CNPCTradeBuffer();
	~CNPCTradeBuffer();

	void	Initialize();
	void	Destroy();

	void	Push( SI16 Msg, SI32 lParam, SI32 NPCCharUnique, cltItem clItem );	
	bool	Pop( stNPCTradeData *pNPCTradeData );

private:

	CNPCTradeData		*m_pFirst;
	CNPCTradeData		*m_pLast;

	SI32				m_siNPCTradeDataCount;	

};


#endif