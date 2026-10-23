//-----------------------------------------------------------------------------
// 2005/09/26 이제완
//-----------------------------------------------------------------------------

#ifndef _GATEMANAGER_H
#define _GATEMANAGER_H

#include "ndatatypes.h"

#define MAX_GATE_NUMBER				800

class cltGate
{
public:
	cltGate();
	cltGate( SI32 unique, TCHAR* name, SI32 founique, 
			 SI32 itemunique, SI32 itemnum,
		     SI32 srcmapindex, SI32 srcx, SI32 srcy, 
			 SI32 dstmapindex, SI32 dstx, SI32 dsty, bool PkAreaGate );

	void		Init();
	void		Set( cltGate* pclgate );


public:
	SI32		siUnique;					// Gate Unique

	TCHAR		szName[128];					
	SI32		FOUnique;					// Field Object Unique

	SI32		ItemUnique;
	SI32		ItemNum;

	SI32		SrcMapIndex;
	SI32		SrcX;
	SI32		SrcY;

	SI32		DstMapIndex;
	SI32		DstX;
	SI32		DstY;

	bool		bPkAreaGate;
};

class cltGateInfo{
public:
	SI32 siUnique;
	//TCHAR szName[128];
	SI32 siTxtNum;
	SI32 siFOUnique;
	
	SI32 siItemUnique;
	SI32 siItemNum;

	SI32 siSrcMapIndex;
	SI32 siSrcMapX;
	SI32 siSrcMapY;

	SI32 siDstMapIndex;
	SI32 siDstMapX;
	SI32 siDstMapY;

	SI32 siPkAreaGate;

};

class cltGateManager
{
public:
	cltGateManager();
	~cltGateManager();

	bool		Add(cltGate* pclgate);
    bool		FindGate(SI32 index, cltGate* pclgate);
	bool		FindGate(SI32 srcMapIndex, SI32 dstMapIndex, cltGate* pclgate );
	bool		FindImportantPos(SI32 mapindex, SI32* px, SI32* py);			// 특정 지도의 대표 좌표를 구한다. 	
	bool		FindMainFieldPos(SI32 mapindex, SI32* px, SI32* py);			// 특정 지도의 야외지역 좌표를 구한다. 
	bool		IsPkAreaGate(SI32 GateUnique);
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltGate, MAX_GATE_NUMBER>		clGate;
#else
	cltGate		clGate[ MAX_GATE_NUMBER ];
#endif

	void		ReadGateData(SI32 ServiceArea);
};

#endif