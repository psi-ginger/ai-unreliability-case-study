#include "Item-BookBox.h"
#include "../../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;


cltBookBox::cltBookBox()
{
	Init();
}

cltBookBox::~cltBookBox()
{
	Destroy();
}

void cltBookBox::Init()
{
	m_siDropRate = 0;
	m_siPercentBGN = 0;
	m_siPercentINT = 0;
	m_siPercentADV = 0;
#ifdef _SAFE_MEMORY
	SI32 i=0;
	for(i=0; i<BOOKTYPE_NUM; i++)		m_siUnique[i].ZeroMem();
	for(i=0; i<BOOKTYPE_NUM; i++)		m_siPercent[i].ZeroMem();
#else
	ZeroMemory( m_siUnique, sizeof(SI32)*BOOKTYPE_NUM*MAX_BOOK_NUM );
	ZeroMemory( m_siPercent, sizeof(SI32)*BOOKTYPE_NUM*MAX_BOOK_NUM );
#endif
}

void cltBookBox::Destroy()
{

}

void cltBookBox::SetFromFile()
{
	if ( pclClient->siServiceArea != ConstServiceArea_China )
		return;

	//KHY - 0810 - 텍스트 리소스 암호화. - 혹시나 암호화 되는것을 피하기 위해.
	FILE *fp = _tfopen( TEXT("Data\\BookBox\\BookBox.txt"), TEXT("rt") );

	if ( fp == NULL )
	{
		MessageBox( NULL, TEXT("Load BookBox.txt Error"), TEXT("error"), MB_OK|MB_TOPMOST );
		return;
	}

	TCHAR buffer[256] = TEXT("");
	TCHAR temp[256] = TEXT("");

	for ( SI16 i=0; i<4; i++ )
	{
		_fgetts(buffer, 256, fp);			// 제목, 공백 건너뛰기
	}

	// 책상자 드롭 확률
	_fgetts(buffer, 256, fp);
	_stscanf(buffer, TEXT("%s %d"), temp, &m_siDropRate);

	// 공백
	_fgetts(buffer, 256, fp);

	SI32	dropRate = 0;

	// 초급 드롭 확률
	_fgetts(buffer, 256, fp);
	_stscanf(buffer, TEXT("%s %d"), temp, &dropRate);
	m_siPercentBGN = dropRate;

	//중급
	_fgetts(buffer, 256, fp);
	_stscanf(buffer, TEXT("%s %d"), temp, &dropRate);
	m_siPercentINT = dropRate + m_siPercentBGN;

	//고급
	_fgetts(buffer, 256, fp);
	_stscanf(buffer, TEXT("%s %d"), temp, &dropRate);
	m_siPercentADV = dropRate + m_siPercentINT;


	for ( SI16 i=0; i<3; i++ )
	{
		_fgetts(buffer, 256, fp);			// 제목, 공백 건너뛰기
	}

	SI32 index = 0;
	SI32 DropPercent[BOOKTYPE_NUM] = {0,};

	while ( _fgetts(buffer, 256, fp) )
	{
		_stscanf( buffer, TEXT("%d %d %d %d %d %d"), 
					&m_siUnique[BOOKTYPE_BGN][index], &DropPercent[BOOKTYPE_BGN],
					&m_siUnique[BOOKTYPE_INT][index], &DropPercent[BOOKTYPE_INT],
					&m_siUnique[BOOKTYPE_ADV][index], &DropPercent[BOOKTYPE_ADV]);

		if ( index == 0 )
		{
			m_siPercent[BOOKTYPE_BGN][index] = DropPercent[BOOKTYPE_BGN];
			m_siPercent[BOOKTYPE_INT][index] = DropPercent[BOOKTYPE_INT];
			m_siPercent[BOOKTYPE_ADV][index] = DropPercent[BOOKTYPE_ADV];
		}
		else
		{
			m_siPercent[BOOKTYPE_BGN][index] = m_siPercent[BOOKTYPE_BGN][index-1] + DropPercent[BOOKTYPE_BGN];
			m_siPercent[BOOKTYPE_INT][index] = m_siPercent[BOOKTYPE_INT][index-1] + DropPercent[BOOKTYPE_INT];
			m_siPercent[BOOKTYPE_ADV][index] = m_siPercent[BOOKTYPE_ADV][index-1] + DropPercent[BOOKTYPE_ADV];
		}
		++index;
	}
	fclose( fp );
}

SI32 cltBookBox::GetRandBookUnique()
{
	SI32 gradeRand = rand() % 100;
	SI32 grade = BOOKTYPE_BGN;

	if ( gradeRand < m_siPercentBGN )		grade = BOOKTYPE_BGN;
	else if ( gradeRand < m_siPercentINT )	grade = BOOKTYPE_INT;
	else	grade = BOOKTYPE_ADV;

	SI32 uniqueRand = rand() % 100;
	for( SI32 i=0; i<MAX_BOOK_NUM; i++ )
	{
		if( uniqueRand < m_siPercent[grade][i] )
		{
			//pclClient->pclLog->FilePrint(TEXT("Config\\BookBox.log"), TEXT("grade\t%d\trand\t%d\tunique\t%d" ), grade, uniqueRand, m_siUnique[grade][i] );
				
			return m_siUnique[grade][i];
		}
	}

	return 0;
}

SI32 cltBookBox::GetDropRate()
{
	return m_siDropRate;
}