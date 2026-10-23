#pragma once 

#include "../../../Lib/Directives.h"
#include "../../../../NLib/NArray.h"

#define MAX_BOOK_NUM	30

enum
{
	BOOKTYPE_BGN = 0,
	BOOKTYPE_INT,
	BOOKTYPE_ADV,
	BOOKTYPE_NUM
};

class cltBookBox
{
public:
	cltBookBox();
	~cltBookBox();

	void Init();
	void Destroy();

private:
	SI32	m_siDropRate;
	SI32	m_siPercentBGN;						// 초급책이 나올 확률
	SI32	m_siPercentINT;						// 중급책이 나올 확률
	SI32	m_siPercentADV;						// 고급책이 나올 확률
#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<SI32, MAX_BOOK_NUM>, BOOKTYPE_NUM>		m_siUnique;
	NSafeTArray< NSafeTArray<SI32, MAX_BOOK_NUM>, BOOKTYPE_NUM>		m_siPercent;
#else
	SI32	m_siUnique[BOOKTYPE_NUM][MAX_BOOK_NUM];
	SI32	m_siPercent[BOOKTYPE_NUM][MAX_BOOK_NUM];
#endif

public:
	void SetFromFile();
	SI32 GetDropRate();
	SI32 GetRandBookUnique();
};