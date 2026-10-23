/*//---------------------------------
// 2005/3/25 ¼ºÁØ¿±
//---------------------------------
#ifndef _INSTANCEMAPMANAGER_H
#define _INSTANCEMAPMANAGER_H

#include <Windows.h>
#include <Directives.h>

#define MAX_INSTANCEMAP_NUM		10

class cltInstanceMapManager{
public:
	cltInstanceMapManager();
	~cltInstanceMapManager();

	void Init();

	void Action();

	void Start();
	void End();

	bool WarpGate();

	siPartyIndex[MAX_INSTANCEMAP_NUM] ;




};

#endif*/