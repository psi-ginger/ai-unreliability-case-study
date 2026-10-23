#pragma once

#include <Directives.h>
#include <stdio.h>
#include "../Client/InterfaceMgr/DialogResource.h"

class CDlgPositionSave
{
public:

	CDlgPositionSave();
	~CDlgPositionSave();

	inline void SetPosition(SI32 index, SI32 x, SI32 y)
	{
		DLG_Position[index].x = x;
		DLG_Position[index].y = y;
		siSaveState[index] = 1;
	}

	inline SI32 GetPositionX(SI32 index)
	{
		return DLG_Position[index].x;
	}

	inline SI32 GetPositionY(SI32 index)
	{
		return DLG_Position[index].y;
	}

	inline bool GetSaveState(SI32 index)
	{
		return siSaveState[index] ? true : false ;
	}

	void LoadPosition();
	void SavePosition();

private:

	POINT	DLG_Position[DIALOG_END];
	SI16	siSaveState[DIALOG_END];
};

extern CDlgPositionSave g_DlgPositionSave;