#include "LevelUpText.h"
#include "../../../NLib/NDataLoader.h"
#include "../../../NLib/NFile.h"
#include "../../../NLib/NUtil.h"
#include "../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;

CLevelUpText::CLevelUpText()
{
	ZeroMemory( this, sizeof(CLevelUpText) );
	LoadLevelUpText();
}

CLevelUpText::~CLevelUpText()
{
	
}

void CLevelUpText::LoadLevelUpText()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	TCHAR fileName[MAX_PATH] = TEXT("TextMgr\\LevelUpText.txt");

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( fileName );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( fileName );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadBeginnerText()"), TEXT("LoadError:[%s]"), fileName );
		return;
	}

	SI32 level = 0;
	TCHAR text[256] = { 0, };

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&level,	4,		
			NDATA_MBSTRING,	text,	256,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 count = 0;
	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData() )
		{
			StringCchCopy( szLevelUpText[level], 256, text );
			bSetText[level] = true;
		}
	}
}

bool CLevelUpText::GetLevelUpText( SI32 level, TCHAR* pText, SI32 txtSize )
{
	if ( level < 1 && level > MAX_CHARACTER_LEVEL ) return false;

	if ( bSetText[level] )
	{
		StringCchCopy( pText, txtSize, szLevelUpText[level] );
		return true;
	}
	return false;
}
