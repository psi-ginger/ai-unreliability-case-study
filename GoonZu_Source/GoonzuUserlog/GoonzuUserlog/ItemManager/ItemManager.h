// GoonzuUserlogDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CItemManager 대화 상자
class CItemManager
{

// 생성
public:
	CItemManager();
	~CItemManager();

	bool LoadFile(char* itemUnique);
	void FindUnique(char* FileName,char *itemUnique);
	void Set(char * BinaryData, char* ItemUnique);

};
