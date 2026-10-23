#pragma once  

#include <Directives.h>
#include <Graphic_Capsule.h>

#include "CommonLogic.h"


#define MARK_FILE_NUM	3

extern cltCommonLogic*		pclClient;

const SI32 siImageCount[MARK_FILE_NUM] = { 256, 256, 95 };

class cltMarkManager
{
private:
	TSpr*	m_pMarkSpr[MARK_FILE_NUM];
	TCHAR	m_szFileName[MARK_FILE_NUM][MAX_PATH];
	

public:
	cltMarkManager();
	~cltMarkManager();

	void LoadMarkFromFile();

	TCHAR* GetMarkSprAndFontIndex( IN SI32 index, OUT TSpr** pSpr, OUT SI32* font );
	SI32 GetTotalCount();
};