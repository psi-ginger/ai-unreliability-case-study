//-----------------------------------------------------------------------------
// 2008/01/09 ÀÌ°­Çü
//-----------------------------------------------------------------------------

#ifndef _INSTANCEMAPGATEMANAGER_H
#define _INSTANCEMAPGATEMANAGER_H

#include "ndatatypes.h"

#define INSTANCEMAPGATE_START					10000
#define MAX_INSTANCEMAPGATE_NUMBER				800
#define MAX_INSTANCEMAPGATE_SETNUMBER			5
#define MAX_INSTANCEMAPGATE_MAXCLONE			5

#define MAX_TOTAL_INSTANCEMAPGATE_NUMBER		MAX_INSTANCEMAPGATE_NUMBER * (MAX_INSTANCEMAPGATE_MAXCLONE + 1)

class cltInstanceMapGate
{
public:
	cltInstanceMapGate();
	cltInstanceMapGate( SI32 unique, TCHAR* name, SI32 founique, 
						SI32 srcmapindex, SI32 srcx, SI32 srcy, 
						SI32 dstmapindex, SI32 dstx, SI32 dsty );

	void		Init();
	void		Set( cltInstanceMapGate* pclgate );


public:
	SI32		siUnique;					// Gate Unique

	TCHAR		szName[128];					
	SI32		FOUnique;					// Field Object Unique

	SI32		SrcMapIndex;
	SI32		SrcX;
	SI32		SrcY;

	SI32		DstMapIndex;
	SI32		DstX;
	SI32		DstY;
};

class cltInstanceMapGateInfo{
public:
	SI32 siUnique;
	//TCHAR szName[128];
	SI32 siTxtNum;
	SI32 siFOUnique;
	
	SI32 siSrcMapIndex;
	SI32 siSrcMapX;
	SI32 siSrcMapY;

	SI32 siDstMapIndex;
	SI32 siDstMapX;
	SI32 siDstMapY;
};

class cltInstanceMapGateManager
{
public:
	cltInstanceMapGateManager();
	~cltInstanceMapGateManager();

	bool		Add(SI32 siSetNumber, cltInstanceMapGate* pclgate);
public:
	SI32					m_siMaxSetCount;

	void		ReadInstanceMapGateData(SI32 ServiceArea);
	SI32		GetInstanceMapGates( SI32 siMapIndex, SI32 siGateSetNumber, cltInstanceMapGate* pclGates, SI32 siMaxCount );
	
	cltInstanceMapGate* GetGatebyUnique(SI32 siUnique, SI32 siSetNumber);
	cltInstanceMapGate* GetGatebyIndex(SI32 siIndex, SI32 siSetNumber);

private:
#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<cltInstanceMapGate, MAX_INSTANCEMAPGATE_SETNUMBER>, MAX_TOTAL_INSTANCEMAPGATE_NUMBER>	clInstanceMapGate;
#else
	cltInstanceMapGate		clInstanceMapGate[ MAX_TOTAL_INSTANCEMAPGATE_NUMBER ][ MAX_INSTANCEMAPGATE_SETNUMBER ];
#endif
};

#endif