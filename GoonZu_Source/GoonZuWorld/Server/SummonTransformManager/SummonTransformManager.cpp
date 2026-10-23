#include "SummonTransformManager.h"

/************************************************************************
*                                                                      
* 소환수 변신 관련.
*
************************************************************************/
bool cltSummonTransformManager::Init( void )
{
#if defined(_DBMANAGER)
	return false;
#else
	if( false == LoadFile() ) return false;

	return true;
#endif
}

bool cltSummonTransformManager::LoadFile( void )
{
#if defined(_DBMANAGER)
	return false;
#else
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//=========================================================================================================
	// SummonTransform.txt
	//=========================================================================================================
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/SummonTransform.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/SummonTransform.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("cltSummonTransformManager::LoadFile()"), TEXT("LoadError:[%s]"), "Data/SummonTransform.txt" );
		return false;
	}

	TCHAR	m_szSummonKind[128]			 = TEXT("");
	TCHAR	m_szTransformSummonKind[128] = TEXT("");
	SI32	m_siAbility					 = 0;

	NDataTypeInfo pSummonTransformInfo[] =
	{
		NDATA_MBSTRING,	&m_szSummonKind, 			sizeof(m_szSummonKind),
		NDATA_MBSTRING,	&m_szTransformSummonKind,	sizeof(m_szTransformSummonKind),
		NDATA_INT32,	&m_siAbility,				4,
		0,				0,							0
	};

	dataloader.RegDataType( pSummonTransformInfo );

	SI32 siCount = 0;
	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData( true ) )
		{
			m_clSummonTransformInfo[siCount].SetSummonKind( pclClient->GetUniqueFromHash( m_szSummonKind ) );	
			m_clSummonTransformInfo[siCount].SetTransformSummonKind( pclClient->GetUniqueFromHash( m_szTransformSummonKind ) );	
			m_clSummonTransformInfo[siCount].SetAbility( m_siAbility );
			++siCount;
		}
	}

	return true;
#endif
}
bool cltSummonTransformManager::IsTransformSummonInfo( IN SI32 _siSummonKind )
{
#if defined(_DBMANAGER)
	return false;
#else
	
	for( SI32 siCount = 0; siCount < MAX_KINDINFO_NUMBER; ++siCount )
	{
		if( 0 == m_clSummonTransformInfo[siCount].GetSummonKind() )	break;

		if( m_clSummonTransformInfo[siCount].GetSummonKind() == _siSummonKind )
		{
			return true;
		}
	}
#endif
	return false;
}

void cltSummonTransformManager::GetTransformSummonInfo( IN SI32 _siSummonKind, OUT SI32& _siTransformSummonKind, OUT SI32& _siTransformSummonAbiliy )
{
#if defined(_DBMANAGER)
	return;
#else
	_siTransformSummonKind		= 0;
	_siTransformSummonAbiliy   = 0;

	for( SI32 siCount = 0; siCount < MAX_KINDINFO_NUMBER; ++siCount )
	{
		if( 0 == m_clSummonTransformInfo[siCount].GetSummonKind() )	break;

		if( m_clSummonTransformInfo[siCount].GetSummonKind() == _siSummonKind )
		{
			_siTransformSummonKind	   = m_clSummonTransformInfo[siCount].GetTransformSummonKind();
			_siTransformSummonAbiliy   = m_clSummonTransformInfo[siCount].GetAbility(); 
			break;
		}
	}
#endif
}


