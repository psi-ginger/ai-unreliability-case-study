#include "ServerBoxItem.h"
#include "../Server.h"
#include "../../CommonLogic/CommonLogic.h"

#include "../../Lib/dxutil.h"


extern cltCommonLogic* pclClient;

cltNewSelectBoxMgr::cltNewSelectBoxMgr()
{
	ZeroMemory( this, sizeof(cltNewSelectBoxMgr) );
}

cltNewSelectBoxMgr::~cltNewSelectBoxMgr()
{
	for ( SI32 siListIndex=0; siListIndex<MAX_NEWSELECTBOX_SELECTINDEX; ++siListIndex )
	{
		for ( SI32 siItemCount=0; siItemCount<MAX_NEWSELECTBOX_ITEM_LIST; ++ siItemCount )
		{
			SAFE_DELETE( m_pclBoxItemList[siListIndex][siItemCount] );
		}
	}
}

bool cltNewSelectBoxMgr::LoadNewSelectBoxItemList( void )
{
	TCHAR szFileName[MAX_PATH] = { '\0', };
	if( pclClient->siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\NewSelectBoxItemListK.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\NewSelectBoxItemListJ.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\NewSelectBoxItemListC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )  // 영어
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\NewSelectBoxItemListE.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\NewSelectBoxItemListT.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\NewSelectBoxItemListU.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )  //NHN중국
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\NewSelectBoxItemListC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		StringCchCopy(szFileName, MAX_PATH, TEXT("ServerData\\NewSelectBoxItemListG.txt"));
	}

	NDataLoader	dataloader;
	BOOL		bLoadFile				= FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( szFileName );		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( szFileName );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("cltNewSelectBoxMgr::LoadNewSelectBoxItemList()"), TEXT("LoadError:[%s]"), szFileName);
		return false;
	}


	TCHAR	szSelectIndex[64]	= { '\0', };
	SI32	siItemUnique		= 0;
	SI32	siItemNum			= 0;
	SI32	siRate				= 0;
	SI32	siUseDate			= 0;
	SI32	siCanNotice			= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szSelectIndex,	64,
		NDATA_INT32,	&siItemUnique,	4,		
		NDATA_INT32,	&siItemNum,		4,		
		NDATA_INT32,	&siRate,		4,		
		NDATA_INT32,	&siUseDate,		4,		
		NDATA_INT32,	&siCanNotice,	4,		
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			SI32 siSelectIndex = SelectStringToInteger( szSelectIndex );
			if ( SELECTINDEX_NONE == siSelectIndex )
			{
				continue;
			}

			cltBoxItem clBoxItem( siItemUnique, siItemNum, siRate, siUseDate, siCanNotice );

			AddBoxItem( siSelectIndex, & clBoxItem );
		}
	}

	return true;
}

SI32 cltNewSelectBoxMgr::SelectStringToInteger( TCHAR* pszSelectString )
{
	if ( NULL == pszSelectString )
	{
		return SELECTINDEX_NONE;
	}

	if     ( _tcscmp(pszSelectString, "SELECTINDEX_00") == 0 )	return SELECTINDEX_00;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_01") == 0 )	return SELECTINDEX_01;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_02") == 0 )	return SELECTINDEX_02;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_03") == 0 )	return SELECTINDEX_03;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_04") == 0 )	return SELECTINDEX_04;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_05") == 0 )	return SELECTINDEX_05;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_06") == 0 )	return SELECTINDEX_06;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_07") == 0 )	return SELECTINDEX_07;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_08") == 0 )	return SELECTINDEX_08;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_09") == 0 )	return SELECTINDEX_09;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_10") == 0 )	return SELECTINDEX_10;
	else if( _tcscmp(pszSelectString, "SELECTINDEX_11") == 0 )	return SELECTINDEX_11;
	// 굳이 해줄 필요는 없지만 디버깅시에 오류 메시지를 안나오게 하기 위해서
	else if( _tcscmp(pszSelectString, "SELECTINDEX_SEPARATE") == 0 )	return SELECTINDEX_NONE;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 새로운 타입은 이 위로 추가해 주세요
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	else 
	{
		NTCHARString256 strText( pszSelectString );
		NTCHARString64	strCaption( _T("Error NewSelectBoxItemList.txt") );

		strText += _T(" Type Not Found");
		MessageBox(NULL, strText, strCaption, MB_ICONWARNING | MB_OK );
	}
#endif

	return SELECTINDEX_NONE;

}

void cltNewSelectBoxMgr::AddBoxItem( SI32 siIndex, IN cltBoxItem* pclBoxItem )
{
	if ( NULL == pclBoxItem )
	{
		return;
	}

	if ( (0 > siIndex) || (MAX_NEWSELECTBOX_SELECTINDEX <= siIndex) )
	{
		return;
	}

	SI32 siItemCount = m_siBoxItemListCount[siIndex];
	if ( (0 > siItemCount) || (MAX_NEWSELECTBOX_ITEM_LIST <= siItemCount) )
	{
		return;
	}
	
	if ( NULL != m_pclBoxItemList[siIndex][siItemCount] )
	{
		return;
	}

	m_pclBoxItemList[siIndex][siItemCount] = new cltBoxItem;

	cltBoxItem* pclSourceBoxItem = m_pclBoxItemList[siIndex][siItemCount];
	if ( NULL ==  pclSourceBoxItem )
	{
		return;
	}

	pclSourceBoxItem->Set( pclBoxItem );

	m_siBoxItemListCount[siIndex]++;
}

bool cltNewSelectBoxMgr::GetItemListFromIndex( SI32 siIndex, OUT SI16* psiItemListArray )
{
	if ( NULL == psiItemListArray )
	{
		return false;
	}

	if ( (0 > siIndex) || (MAX_NEWSELECTBOX_SELECTINDEX <= siIndex) )
	{
		return false;
	}

	cltBoxItem*	pclBoxItem	= NULL;
	SI32		siMaxCount	= min( m_siBoxItemListCount[siIndex], MAX_NEWSELECTBOX_ITEM_LIST );

	for ( SI32 siCount=0; siCount<siMaxCount; ++siCount )
	{
		pclBoxItem = m_pclBoxItemList[siIndex][siCount];
		if ( NULL == pclBoxItem )
		{
			continue;
		}

		psiItemListArray[siCount] = pclBoxItem->GetItemUnique();
	}

	return true;
}

bool cltNewSelectBoxMgr::GetRandItemFromIndex( SI32 siIndex, OUT cltBoxItem* pclBoxItem )
{
	if ( NULL == pclBoxItem )
	{
		return false;
	}

	if ( (0 > siIndex) || (MAX_NEWSELECTBOX_SELECTINDEX <= siIndex) )
	{
		return false;
	}

	cltBoxItem*	pclGetBoxItem	= NULL;
	SI32		siRandRate		= rand() % Const_Max_BoxItem_TotalRate;
	SI32		siTotalRate		= 0;
	SI32		siMaxCount		= min( m_siBoxItemListCount[siIndex], MAX_NEWSELECTBOX_ITEM_LIST );

	for ( SI32 siCount=0; siCount<siMaxCount; ++siCount )
	{
		pclGetBoxItem = m_pclBoxItemList[siIndex][siCount];
		if ( NULL == pclGetBoxItem )
		{
			continue;
		}

		if ( 0 >= pclGetBoxItem->GetItemUnique() )
		{
			continue;
		}

		if ( (siTotalRate <= siRandRate) && ( siRandRate < (siTotalRate+pclGetBoxItem->GetRate()) ) )
		{
			pclBoxItem->Set( pclGetBoxItem );
			return true;
		}

		siTotalRate += pclGetBoxItem->GetRate();
	}

	return false;
}