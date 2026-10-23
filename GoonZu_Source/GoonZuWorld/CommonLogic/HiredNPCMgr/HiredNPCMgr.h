#ifndef _HIREDNPCMGR_H
#define _HIREDNPCMGR_H

#include <Directives.h>

#define MAX_HIREDNPC_KIND_NUM							80

class cltHiredNPCPayInfo
{
public:
	cltHiredNPCPayInfo();
	~cltHiredNPCPayInfo();

	void					Init();
	
public:
	SI32					siNPCKind;						// NPC kind
	SI32					siTotalHiredNum;				// 해당 NPC가 전체 마을에서 고용된 총 숫자 
	GMONEY					siPayment;						// 급여
};

class cltHiredNPCPayManager
{
public:
	cltHiredNPCPayManager();
	~cltHiredNPCPayManager();

public:
	void					Init();
	void					CalcPayment( int i );
	
	void					AddNPC( int npckind );
	void					DelNPC( int npckind );

	GMONEY					GetPayment( int npckind );
	
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltHiredNPCPayInfo, MAX_HIREDNPC_KIND_NUM>		clHiredNPCPayInfo;
#else
	cltHiredNPCPayInfo		clHiredNPCPayInfo[ MAX_HIREDNPC_KIND_NUM ];		// NPC 종류에 따른 정보
#endif
	SI32					siTotalNPCNum;									// 마을 전체 NPC숫자 
	SI32					siNPCKindNum;									// NPC종류 

};

#endif
