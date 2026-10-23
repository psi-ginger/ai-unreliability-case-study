#include <memory.h>
#include "MissionGameData.h"

#include "../../../NLib/NUtil.h"
#include "../../../NLib/NDataLoader.h"

#include "../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;

cltMissionGameDataManager::cltMissionGameDataManager()
{
	memset(this, 0, sizeof(*this));
}

cltMissionGameDataManager::~cltMissionGameDataManager()
{
	for( SI32 i=0; i<m_clMissionGameData.GetSize(); i++ )
	{
		if( m_clMissionGameData[i] )
		{
			delete m_clMissionGameData[i];
		}
	}
}

bool cltMissionGameDataManager::Init()
{
	bool bRet = false;
	
	bRet = LoadDataFromFile();

	return bRet;
}

bool cltMissionGameDataManager::LoadDataFromFile()
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//=========================================================================================================
	// Mission.txt
	//=========================================================================================================
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( "./Data/Mission.txt" );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( "./Data/Mission.txt" );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("cltMissionManager::LoadDataFromFile()"), TEXT("LoadError:[%s]"), "./Data/Mission.txt" );
		return false;
	}

	SI32	siUnique;
	TCHAR	szMissionUniqueString[MAX_MISSON_UNIQUESTRING_LENGTH];
	SI32	siTime;
	SI32	siFee;

	NDataTypeInfo pDataTypeInfo[] =
	{	
		NDATA_INT32,	&siUnique,					4,
		NDATA_MBSTRING,	szMissionUniqueString,		MAX_MISSON_UNIQUESTRING_LENGTH,
		NDATA_INT32,	&siTime,					4,
		NDATA_INT32,	&siFee,						4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData(true))
		{
			if( InBounds(siUnique, (SI32)1, m_clMissionGameData.GetSize()-1) == false )
			{
#ifdef _DEBUG
				NTCHARString<128>		kTmp;
				kTmp += "MissionUnique가 허용범위를 넘었습니다. 최대는(";
				kTmp += SI32ToString(m_clMissionGameData.GetSize() - 1);
				kTmp += ")입니다.";

				MessageBox(NULL, kTmp, "ERROR", MB_OK);
#endif
			}

			m_clMissionGameData[siUnique] = new cltMissionGameData();
			m_clMissionGameData[siUnique]->m_siMissionUnique		= siUnique;
			m_clMissionGameData[siUnique]->m_kMissionUniqueString	= szMissionUniqueString;
			m_clMissionGameData[siUnique]->m_siTime					= siTime;
			m_clMissionGameData[siUnique]->m_siFee					= siFee;
		}
	}

	//=========================================================================================================
	// MissionClearCondition.txt
	//=========================================================================================================
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( "./Data/MissionClearCondition.txt" );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( "./Data/MissionClearCondition.txt" );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("cltMissionManager::LoadDataFromFile()"), TEXT("LoadError:[%s]"), "./Data/MissionClearCondition.txt" );
		return false;
	}

	SI32	siCommand;
	SI32	siPara1;
	SI32	siPara2;
	SI32	siPara3;

	NDataTypeInfo pDataTypeInfo_ClearCond[] =
	{	
		NDATA_MBSTRING,	szMissionUniqueString,		MAX_MISSON_UNIQUESTRING_LENGTH,
		NDATA_INT32,	&siCommand,					4,
		NDATA_INT32,	&siPara1,					4,
		NDATA_INT32,	&siPara2,					4,
		NDATA_INT32,	&siPara3,					4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo_ClearCond );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData(true))
		{
			cltMissionGameData* pclMissionGameData = GetMissionDataFromUniqueString((const TCHAR*)szMissionUniqueString);
			if( pclMissionGameData == NULL )
			{
#ifdef _DEBUG
				NTCHARString<128>		kTmp;
				kTmp += "잘못된 미션 유니크문자열입니다. [";
				kTmp += szMissionUniqueString;
				kTmp += "]";

				MessageBox(NULL, kTmp, "ERROR", MB_OK);
#endif
				continue;
			}
			
			pclMissionGameData->m_clClearCondition.m_siClearCommand = siCommand;
			pclMissionGameData->m_clClearCondition.m_siPara1	 	= siPara1;
			pclMissionGameData->m_clClearCondition.m_siPara2	 	= siPara2;
			pclMissionGameData->m_clClearCondition.m_siPara3	 	= siPara3;
		}
	}

	//=========================================================================================================
	// MissionReward.txt
	//=========================================================================================================
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( "./Data/MissionReward.txt" );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( "./Data/MissionReward.txt" );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("cltMissionManager::LoadDataFromFile()"), TEXT("LoadError:[%s]"), "./Data/MissionReward.txt" );
		return false;
	}

	SI32	siRewardType;
	SI32	siKindUnique;
	SI32	siAmount;

	NDataTypeInfo pDataTypeInfo_Reward[] =
	{	
		NDATA_MBSTRING,	szMissionUniqueString,		MAX_MISSON_UNIQUESTRING_LENGTH,
		NDATA_INT32,	&siRewardType,				4,
		NDATA_INT32,	&siKindUnique,				4,
		NDATA_INT32,	&siAmount,					4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo_ClearCond );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData(true))
		{
			cltMissionGameData* pclMissionGameData = GetMissionDataFromUniqueString((const TCHAR*)szMissionUniqueString);
			if( pclMissionGameData == NULL )
			{
#ifdef _DEBUG
				NTCHARString<128>		kTmp;
				kTmp += "잘못된 미션 유니크문자열입니다. [";
				kTmp += szMissionUniqueString;
				kTmp += "]";

				MessageBox(NULL, kTmp, "ERROR", MB_OK);
#endif
				continue;
			}

			if( pclMissionGameData->AddReward( siRewardType, siKindUnique, siAmount ) == false )
			{
#ifdef _DEBUG
				NTCHARString<128>		kTmp;
				kTmp += "보상개수가 너무 많습니다. [";
				kTmp += szMissionUniqueString;
				kTmp += "]";

				MessageBox(NULL, kTmp, "ERROR", MB_OK);
#endif
			}
		}
	}

	return true;
}

cltMissionGameData* cltMissionGameDataManager::GetMissionDataFromUniqueString( const TCHAR* pszUniqueString )
{
	for( SI32 i=0; i<m_clMissionGameData.GetSize(); i++ )
	{
		if( m_clMissionGameData[i] == NULL )			continue;

		if( m_clMissionGameData[i]->GetUniqueString() == pszUniqueString )
		{
			return m_clMissionGameData[i];
		}
	}

	return NULL;
}

cltMissionGameData* cltMissionGameDataManager::GetMissionDataFromUnique( SI16 siUnique )
{
	if( InBounds(siUnique, (SI16)1, (SI16)(m_clMissionGameData.GetSize()-1) ) == false )
		return NULL;

	return m_clMissionGameData[siUnique];
}

