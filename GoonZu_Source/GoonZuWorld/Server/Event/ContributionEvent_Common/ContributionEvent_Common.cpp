#include "../../../common/Event/event.h"
#include "../Server/Server.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Structure.h"
#include "dxutil.h"


#include "ContributionEvent_Common.h"

bool					cltContributionEvent_Common::m_bCreate																			= false;
cltContributionInfo*	cltContributionEvent_Common::m_pclContributionInfo[CONTRIBUTION_INFO_NUM]										= {NULL, };
void*					cltContributionEvent_Common::m_pClass[MAX_KIND_NUMBER]															= {NULL, };
bool					(*cltContributionEvent_Common::m_pMakeNpcFunc[MAX_KIND_NUMBER])( void* _pClass, cltCharServer* _pclChar )		= {NULL, };
bool					(*cltContributionEvent_Common::m_pCompleteMakeNpcFunc[MAX_KIND_NUMBER])( void* _pClass, cltCharServer* _pclChar )		= {NULL, };
bool					(*cltContributionEvent_Common::m_pContributionFunc[MAX_KIND_NUMBER])( void* _pClass, cltCharServer* _pclChar )	= {NULL, };

cltContributionEvent_Common::cltContributionEvent_Common( void )
{
	if( true == m_bCreate ) return;

	Clear();

	m_bCreate = true;
}

cltContributionEvent_Common::~cltContributionEvent_Common( void )
{
	for(SI32 siCount = 0; CONTRIBUTION_INFO_NUM > siCount; ++siCount )
	{
		SAFE_DELETE( m_pclContributionInfo[siCount] );
	}
}


void	cltContributionEvent_Common::Clear( void )
{
	for(SI32 siCount = 0; CONTRIBUTION_INFO_NUM > siCount; ++siCount )
	{
       m_pclContributionInfo[siCount] = NULL;
	}
}

bool	cltContributionEvent_Common::FileLoad( void )
{
	bool		bLoadFile = false;
	NDataLoader	dataloader;
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( "Data/ContributionEvent_Common.txt" );		
	}
	else
	{
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
		{
			bLoadFile = dataloader.LoadDataFromCryptFile( "Data/ContributionEvent_Common.txt" );
		}
		else
		{
			bLoadFile = dataloader.LoadDataFromFile( "Data/ContributionEvent_Common.txt" );
		}
	}
	
	TCHAR	szNPCKind[128]		= "";
	TCHAR	szMakeNPCKind[128]	= "";
	TCHAR	szItemInfo[2048]	= "";
	SI32	siMyVillage			= 0;
		
	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szNPCKind,		sizeof(szNPCKind),
		NDATA_MBSTRING,	szMakeNPCKind,	sizeof(szMakeNPCKind),
		NDATA_MBSTRING,	szItemInfo,		sizeof(szItemInfo),
		NDATA_INT32,	&siMyVillage,	sizeof(siMyVillage),
		0,0,0
	};

	SI32 siCount = 0;

	dataloader.RegDataType( pDataTypeInfo );
	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData() )
		{
			m_pclContributionInfo[siCount] = new cltContributionInfo;
			if( NULL == m_pclContributionInfo[siCount] )
			{
				MessageBox( NULL, TEXT("Error"), TEXT("new cltContributionInfo Fail"), 0 );
				return false;
			}
			ZeroMemory( m_pclContributionInfo[siCount], sizeof(cltContributionInfo) );

			m_pclContributionInfo[siCount]->m_siNPCKind		= pclClient->GetUniqueFromHash( szNPCKind );
			m_pclContributionInfo[siCount]->m_siMakeNPCKind	= pclClient->GetUniqueFromHash( szMakeNPCKind );
			

			SI32 siItemCount = 0;
			TCHAR* pszData = _tcstok( szItemInfo, "(" );
			while( pszData )
			{
				m_pclContributionInfo[siCount]->m_siItemUnique[siItemCount]	= _ttoi(pszData);
				pszData = _tcstok(NULL, ")");
				m_pclContributionInfo[siCount]->m_siItemNum[siItemCount]	= (SI16)_ttoi(pszData);

				pszData = _tcstok(NULL, "(");
				pszData ? pszData++ : 0;

				++siItemCount;
			}
			
			m_pclContributionInfo[siCount]->m_bMyVillage = siMyVillage ? true : false;

			++siCount;
			if( CONTRIBUTION_INFO_NUM <= siCount ) 
			{
				MessageBox(NULL, "기부 아이템 정보 수용개수가 초과 했습니다.", "초기화 실패", 0 );
				return false;
			}
		}
	}

	return bLoadFile;
}

cltContributionInfo*	cltContributionEvent_Common::FindContributionInfo( SI32 siNPCKind )
{
	for(SI32 siCount = 0; CONTRIBUTION_INFO_NUM > siCount; ++siCount )
	{
		if( NULL == m_pclContributionInfo[siCount] ) break;
		if( siNPCKind == m_pclContributionInfo[siCount]->m_siNPCKind )
			return m_pclContributionInfo[siCount];
	}

	return NULL;
}

SI32	cltContributionEvent_Common::GetMakeNPC( SI32 siNPCKind )
{
	cltContributionInfo* pclContributionInfo = FindContributionInfo( siNPCKind );
	if( NULL == pclContributionInfo ) return 0;

	return pclContributionInfo->m_siMakeNPCKind;
}

bool cltContributionEvent_Common::Insert( SI32 _siNPCKind, 
										  void* _pClass, 
										  bool (*_pMakeNpcFunc)( void* _pClass, cltCharServer* _pclChar ), 
										  bool (*_pCompleteMakeNpcFunc)( void* _pClass, cltCharServer* _pclChar ),
										  bool (*_pContributionFunc)( void* _pClass, cltCharServer* _pclChar ) )
{
	if( MAX_KIND_NUMBER <= _siNPCKind || NULL == _pClass || m_pClass[_siNPCKind] )  
	{
		MessageBox(NULL, "cltContributionEvent_Common::Insert() Fail", "Fail", 0 );
		return false;
	}

	m_pClass[_siNPCKind]				= _pClass;
	m_pMakeNpcFunc[_siNPCKind]			= _pMakeNpcFunc;
	m_pCompleteMakeNpcFunc[_siNPCKind]	= _pCompleteMakeNpcFunc;
	m_pContributionFunc[_siNPCKind]		= _pContributionFunc;

	return true;	
}

bool cltContributionEvent_Common::IsMakeNpc( SI32 _siNPCKind, cltCharServer* _pclChar )
{
	if( NULL == m_pMakeNpcFunc[_siNPCKind] ) return true;

	return m_pMakeNpcFunc[_siNPCKind]( m_pClass[_siNPCKind], _pclChar );
}

bool cltContributionEvent_Common::CompleteMakeNpc( SI32 _siNPCKind, cltCharServer* _pclChar )
{
	if( NULL == m_pCompleteMakeNpcFunc[_siNPCKind] ) return true;

	return m_pCompleteMakeNpcFunc[_siNPCKind]( m_pClass[_siNPCKind], _pclChar );
}

bool cltContributionEvent_Common::IsContribution( SI32 _siNPCKind, cltCharServer* _pclChar )
{
	if( NULL == m_pContributionFunc[_siNPCKind] ) return true;

	return m_pContributionFunc[_siNPCKind]( m_pClass[_siNPCKind], _pclChar );
}

void	cltContributionEvent_Common::ContributionItemListDelete( SI32 _siVillageUnique, SI32 _siNPCKind )
{
	if( 0 >= _siVillageUnique || 0 >= _siNPCKind ) return;

	sDBRequest_Contribution_Item_List_Delete clInfo( _siVillageUnique, _siNPCKind );
	((cltServer*)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clInfo );	
}
