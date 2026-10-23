
#ifdef __SUN_NNPCMENU__


#ifdef __SUN_NNPCTRADE1__



#ifndef _NPCTRADEDATA_H_
#define _NPCTRADEDATA_H_

#include "NPCTradeBaseInfo.h"

class CNPCTradeData
{
public:
	CNPCTradeData( stNPCTradeData *pNPCTradeData );
	~CNPCTradeData();

	CNPCTradeData*	GetPre();
	void			SetPre( CNPCTradeData *pTradeData );
	
	CNPCTradeData*	GetNext();
	void			SetNext( CNPCTradeData *pTradeData );

	stNPCTradeData*	GetTradeData();

private:
	stNPCTradeData	m_stNPCTradeData;

	CNPCTradeData	*m_pPre;
	CNPCTradeData	*m_pNext;

};


#endif
#endif
#endif