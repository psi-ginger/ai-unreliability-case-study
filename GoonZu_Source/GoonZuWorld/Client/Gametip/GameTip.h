#pragma once

#include "CommonLogic.h"

#define GAME_TIP_NUM	64

class CGameTip
{
public:
	CGameTip(void);
	~CGameTip(void);

public:
	TCHAR*	RandTipText(void);
	void	LoadGameTipFile(void);

private:
	SI32	m_siTipText[GAME_TIP_NUM];
	SI32	m_siTipTextNum;
};
