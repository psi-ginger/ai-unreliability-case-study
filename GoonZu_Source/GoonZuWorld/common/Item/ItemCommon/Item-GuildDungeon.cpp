#include "Item-GuildDungeon.h"
#include "NDataLoader.h"

#include "../../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;

cltGuildDungeonItem::cltGuildDungeonItem()
{
	Init();
}

cltGuildDungeonItem::~cltGuildDungeonItem()
{
	Destroy();
}

void cltGuildDungeonItem::Init()
{
	for( SI32 i=0; i<MAX_GUILDDUNGEON; i++ )
	{
		m_pclItem[i] = NULL;
	}

	LoadFromFile();
}

void cltGuildDungeonItem::Destroy()
{
	for ( SI32 i=0; i<MAX_GUILDDUNGEON; i++ )
	{
		if ( m_pclItem[i] )
		{
			delete m_pclItem[i];
			m_pclItem[i] = NULL;
		}
	}
}

void cltGuildDungeonItem::LoadFromFile()
{
	BOOL bLoadFile = FALSE;
	NDataLoader	dataloader;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/GuildDungeonItem.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/GuildDungeonItem.txt") );
	}

	if (!bLoadFile)
	{
		return;
	}

	SI32 siMapIndex = 0;
	SI32 siItemUnique = 0;
	SI32 siCondition[5] = {0,};
	SI32 siItemNum[5] = {0,};

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siMapIndex,		4,
		NDATA_INT32,	&siItemUnique,		4,
		NDATA_INT32,	&siCondition[0],	4,
		NDATA_INT32,	&siItemNum[0],		4,
		NDATA_INT32,	&siCondition[1],	4,
		NDATA_INT32,	&siItemNum[1],		4,
		NDATA_INT32,	&siCondition[2],	4,
		NDATA_INT32,	&siItemNum[2],		4,
		NDATA_INT32,	&siCondition[3],	4,
		NDATA_INT32,	&siItemNum[3],		4,
		NDATA_INT32,	&siCondition[4],	4,
		NDATA_INT32,	&siItemNum[4],		4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI16 index = 0;

	while( !dataloader.IsEndOfData() ) 
	{
		if ( index >= MAX_GUILDDUNGEON ) break;

		if (dataloader.ReadData())	
		{
			m_pclItem[index] = new cltGuildDungeonItemUnit( siMapIndex, siItemUnique, siCondition, siItemNum );
			++index;
		}
	}
}

SI32 cltGuildDungeonItem::GetItem(IN SI32 mapindex, IN SI32 gp, OUT SI32 *itemunique )
{
	SI32 zeronum = 0 ;

	for( SI32 i = 0 ; i < MAX_GUILDDUNGEON; i++ )
	{
		if( m_pclItem[i] )
		{
			if ( m_pclItem[i]->m_siMapIndex == mapindex )
			{
				*itemunique = m_pclItem[i]->m_siItemUnique;

				if ( gp > m_pclItem[i]->m_siCondition[4] )
					return m_pclItem[i]->m_siItemNum[4];
				else if ( gp > m_pclItem[i]->m_siCondition[3] )
					return m_pclItem[i]->m_siItemNum[3];	
				else if ( gp > m_pclItem[i]->m_siCondition[2] )
					return m_pclItem[i]->m_siItemNum[2];
				else if ( gp > m_pclItem[i]->m_siCondition[1] )
					return m_pclItem[i]->m_siItemNum[1];
				else if ( gp > m_pclItem[i]->m_siCondition[0] )
					return m_pclItem[i]->m_siItemNum[0];
				else if ( gp == 0 )// gp가 0일때 는 갯수는 0개.
					return zeronum;
			}
		}
	}

	return 0;
}
