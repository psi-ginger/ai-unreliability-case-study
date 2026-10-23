// GoonzuUserlogDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CVillageManager 대화 상자
class CVillageManager
{

// 생성
public:
	CVillageManager();
	~CVillageManager();

	char village_name[64];
	void FindVillage(char *itemUnique);
	bool IsNumber(LPCTSTR pszText);
	char* Set(char *VillageUnique);

};
