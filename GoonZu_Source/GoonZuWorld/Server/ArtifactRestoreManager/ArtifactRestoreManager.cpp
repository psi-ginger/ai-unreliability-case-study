#include "ArtifactRestoreManager.h"

extern cltCommonLogic* pclClient;

CArtifactRestoreManager::CArtifactRestoreManager()
{
	ZeroMemory( this, sizeof( CArtifactRestoreManager ) );
	Init();
}

CArtifactRestoreManager::~CArtifactRestoreManager()
{

}

void CArtifactRestoreManager::Init( void )
{
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		LoadElementRestoreDataFromTxt();
		LoadPowderRestoreDataFromTxt();
	}
}


bool CArtifactRestoreManager::LoadElementRestoreDataFromTxt()
{
	if( pclClient->GameMode == GAMEMODE_CLIENT ) 
		return false;

	// 메모리 초기화.
#ifdef _SAFE_MEMORY
	m_clElementRestore.ZeroMem();
#else
	ZeroMemory( m_clElementRestore, sizeof( m_clElementRestore ) );
#endif

	NDataLoader	dataloader;
	BOOL	bLoadFile=false;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("ServerData/ElementRestoreListK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/ElementRestoreListC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/ElementRestoreListJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/ElementRestoreListE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/ElementRestoreListT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/ElementRestoreListU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/ElementRestoreListC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/ElementRestoreListG.txt") );
		break;

	}

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename);		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(datfilename);							
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadElementRestoreDataFromTxt()"), TEXT("LoadError:[%s]"), datfilename);
		return false;
	}

	SI16 Counter = 0;

	SI32	siLevel									= 0;								
	SI32	siItemUnique[ITEM_BY_ELEMENT_KIND_NUM]	= { 0, };
	SI32	siMinNum[ITEM_BY_ELEMENT_KIND_NUM]		= { 0, };
	SI32	siMaxNum[ITEM_BY_ELEMENT_KIND_NUM]		= { 0, };
	SI32    siRate[ITEM_BY_ELEMENT_KIND_NUM]		= { 0, };
	
	
	NDataTypeInfo pDataTypeInfo[] =
	{	
		NDATA_INT32,	&siLevel,			4,		
		NDATA_INT32,	&siItemUnique[0],	4,		
		NDATA_INT32,	&siMinNum[0],		4,
		NDATA_INT32,	&siMaxNum[0],		4,		
		NDATA_INT32,	&siRate[0],			4,		
		NDATA_INT32,	&siItemUnique[1],	4,		
		NDATA_INT32,	&siMinNum[1],		4,
		NDATA_INT32,	&siMaxNum[1],		4,		
		NDATA_INT32,	&siRate[1],			4,		
		NDATA_INT32,	&siItemUnique[2],	4,		
		NDATA_INT32,	&siMinNum[2],		4,
		NDATA_INT32,	&siMaxNum[2],		4,		
		NDATA_INT32,	&siRate[2],			4,		
		0,				0,					0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			m_clElementRestore[Counter].m_siLevel = siLevel;
			
			for( SI32 ArrayCount = 0; ArrayCount < ITEM_BY_ELEMENT_KIND_NUM; ++ArrayCount )
			{
				m_clElementRestore[Counter].m_siItemUnique[ArrayCount]	= siItemUnique[ArrayCount];	
				m_clElementRestore[Counter].m_siMinNum[ArrayCount]		= siMinNum[ArrayCount];		
				m_clElementRestore[Counter].m_siMaxNum[ArrayCount]		= siMaxNum[ArrayCount];		
				m_clElementRestore[Counter].m_siRate[ArrayCount]		= siRate[ArrayCount];		
			}
						
			if ( ++Counter >= ELEMENT_RESTORE_MAX_LIST )
			{
				break;
			}				
		}
	}

	return true;
}

bool CArtifactRestoreManager::LoadPowderRestoreDataFromTxt( void )
{
	if( pclClient->GameMode == GAMEMODE_CLIENT ) 
		return false;

	// 메모리 초기화.
#ifdef _SAFE_MEMORY
	m_clPowderRestore.ZeroMem();
#else
	ZeroMemory( m_clPowderRestore, sizeof( m_clPowderRestore ) );
#endif

	NDataLoader	dataloader;
	BOOL bLoadFile=false;
	TCHAR datfilename[128] = TEXT("");

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea :
		StringCchPrintf(datfilename, 128, TEXT("ServerData/PowderRestoreListK.txt") );
		break;
	case ConstServiceArea_China :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/PowderRestoreListC.txt") );
		break;
	case ConstServiceArea_Japan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/PowderRestoreListJ.txt") );
		break;
	case ConstServiceArea_English :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/PowderRestoreListE.txt") );
		break;
	case ConstServiceArea_Taiwan :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/PowderRestoreListT.txt") );
		break;
	case ConstServiceArea_USA  :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/PowderRestoreListU.txt") );
		break;
	case ConstServiceArea_NHNChina :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/PowderRestoreListC.txt") );
		break;
	case ConstServiceArea_EUROPE :
		StringCchPrintf(datfilename,  128, TEXT("ServerData/PowderRestoreListG.txt") );
		break;

	}

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile(datfilename);		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(datfilename);							
	}


	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadElementRestoreDataFromTxt()"), TEXT("LoadError:[%s]"), datfilename);
		return false;
	}

	SI16 Counter = 0;

	SI32	siLevel									= 0;								
	SI32	siItemUnique[ITEM_BY_POWDER_KIND_NUM]	= { 0, };
	SI32	siMinNum[ITEM_BY_POWDER_KIND_NUM]		= { 0, };
	SI32	siMaxNum[ITEM_BY_POWDER_KIND_NUM]		= { 0, };
	SI32    siRate[ITEM_BY_POWDER_KIND_NUM]			= { 0, };


	NDataTypeInfo pDataTypeInfo[] =
	{	
			NDATA_INT32,	&siLevel,			4,		
			NDATA_INT32,	&siItemUnique[0],	4,		
			NDATA_INT32,	&siMinNum[0],		4,
			NDATA_INT32,	&siMaxNum[0],		4,		
			NDATA_INT32,	&siRate[0],			4,		
			NDATA_INT32,	&siItemUnique[1],	4,		
			NDATA_INT32,	&siMinNum[1],		4,
			NDATA_INT32,	&siMaxNum[1],		4,		
			NDATA_INT32,	&siRate[1],			4,		
			NDATA_INT32,	&siItemUnique[2],	4,		
			NDATA_INT32,	&siMinNum[2],		4,
			NDATA_INT32,	&siMaxNum[2],		4,		
			NDATA_INT32,	&siRate[2],			4,	
			NDATA_INT32,	&siItemUnique[3],	4,		
			NDATA_INT32,	&siMinNum[3],		4,
			NDATA_INT32,	&siMaxNum[3],		4,		
			NDATA_INT32,	&siRate[3],			4,		
			NDATA_INT32,	&siItemUnique[4],	4,		
			NDATA_INT32,	&siMinNum[4],		4,
			NDATA_INT32,	&siMaxNum[4],		4,		
			NDATA_INT32,	&siRate[4],			4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			m_clPowderRestore[Counter].m_siLevel = siLevel;

			for( SI32 ArrayCount = 0; ArrayCount < ITEM_BY_POWDER_KIND_NUM; ++ArrayCount )
			{
				m_clPowderRestore[Counter].m_siItemUnique[ArrayCount]	= siItemUnique[ArrayCount];	
				m_clPowderRestore[Counter].m_siMinNum[ArrayCount]		= siMinNum[ArrayCount];		
				m_clPowderRestore[Counter].m_siMaxNum[ArrayCount]		= siMaxNum[ArrayCount];		
				m_clPowderRestore[Counter].m_siRate[ArrayCount]			= siRate[ArrayCount];		
			}

			if ( ++Counter >= POWDER_RESTORE_MAX_LIST )
			{
				break;
			}				
		}
	}

	return true;
}


