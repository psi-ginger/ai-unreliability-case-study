#pragma once

#include <Directives.h>
#include "../../common/Char/ParametaBox/parametabox.h"


class CLevelUpText
{
private:
	TCHAR szLevelUpText[MAX_CHARACTER_LEVEL + 1][256];
	bool bSetText[MAX_CHARACTER_LEVEL + 1];

public:
	CLevelUpText();
	~CLevelUpText();

	void LoadLevelUpText();
	bool GetLevelUpText( SI32 level, TCHAR* pText, SI32 txtSize );
};