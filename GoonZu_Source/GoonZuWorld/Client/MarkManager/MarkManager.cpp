#include "MarkManager.h"

cltMarkManager::cltMarkManager()
{
	for( SI32 i=0; i< MARK_FILE_NUM; i++ )
	{
		m_pMarkSpr[i] = NULL;
	}
}

cltMarkManager::~cltMarkManager()
{
	SI32 i;

	for(i = 0;i < MARK_FILE_NUM;i++)
	{
		if( m_pMarkSpr[i] )
		{
			if(m_pMarkSpr[i]->pImage)
			{
				GP.FreeSpr( *(m_pMarkSpr[i]) );
			}
			delete m_pMarkSpr[i];
			m_pMarkSpr[i] = NULL;
		}
	}
}

void cltMarkManager::LoadMarkFromFile()
{
	TCHAR fileName[MAX_PATH] = {0,};

	for ( SI32 i = 0; i < MARK_FILE_NUM; ++i )
	{
		StringCchPrintf( fileName, MAX_PATH, TEXT("GImg\\VillageMark%02d.Spr"), i+1 );

		StringCchCopy( m_szFileName[i], MAX_PATH, fileName );

		m_pMarkSpr[i] = new TSpr;
		GP.LoadSprFromFile( fileName, m_pMarkSpr[i] );
	}

}

TCHAR* cltMarkManager::GetMarkSprAndFontIndex(IN SI32 index, OUT TSpr** pSpr, OUT SI32* font )
{
	SI32 totalCount = 0;
	SI32 calcCount = 0;
	*font = 0;

	for ( SI32 i = 0; i<MARK_FILE_NUM; ++i )
	{
		totalCount += siImageCount[i];

		if ( index <= totalCount )
		{
			*pSpr = m_pMarkSpr[i];
			*font = index - calcCount - 1;
			return m_szFileName[i];
		}

		calcCount += siImageCount[i];
	}

	return FALSE;
}

SI32 cltMarkManager::GetTotalCount()
{
	SI32 totalCount = 0;

	for ( SI32 i=0; i<MARK_FILE_NUM; ++i )
	{
		if ( m_pMarkSpr[i] )
			totalCount += siImageCount[i];
	}

	return totalCount;
}