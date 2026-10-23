
#include "BoxItemMgr.h"

extern cltCommonLogic* pclClient;

/************************************************************************************************************************
* 클래스 이름 : CBoxItemUnit
* 목적 : 상자 아이템에 들어갈 개별 아이템 데이터
************************************************************************************************************************/
CBoxItemUnit::CBoxItemUnit( void )
	: m_uiItemUnique(0)
	, m_uiMinNum(0)
	, m_uiMaxNum(0)
	, m_uiItemUseDate(0)
	, m_uiProbability(0)
	, m_uiCanNotice(0)
{
	
}

CBoxItemUnit::CBoxItemUnit( UI16 uiItemUnique, UI16 uiMinNum, UI16 uiMaxNum, UI16 uiItemUseDate, UI16 uiProbability, UI16 uiCanNotice )
	: m_uiItemUnique(uiItemUnique)
	, m_uiMinNum(uiMinNum)
	, m_uiMaxNum(uiMaxNum)
	, m_uiItemUseDate(uiItemUseDate)
	, m_uiProbability(uiProbability)
	, m_uiCanNotice(uiCanNotice)
{

}

CBoxItemUnit::CBoxItemUnit( const CBoxItemUnit* pclBoxItemUnit )
{
	if ( pclBoxItemUnit )
	{
		memcpy( this, pclBoxItemUnit, sizeof(CBoxItemUnit) );
	}
}

CBoxItemUnit::~CBoxItemUnit( void )
{

}

void CBoxItemUnit::Init( void )
{
	ZeroMemory( this, sizeof(CBoxItemUnit) );
}

void CBoxItemUnit::SetBoxItemUnit( UI16 uiItemUnique, UI16 uiMinNum, UI16 uiMaxNum, UI16 uiItemUseDate, UI16 uiProbability, UI16 uiCanNotice )
{
	m_uiItemUnique	= uiItemUnique;
	m_uiMinNum		= uiMinNum;
	m_uiMaxNum		= uiMaxNum;
	m_uiItemUseDate	= uiItemUseDate;
	m_uiProbability	= uiProbability;
	m_uiCanNotice	= uiCanNotice;
}

void CBoxItemUnit::SetBoxItemUnit( const CBoxItemUnit* pclBoxItemUnit )
{
	if ( pclBoxItemUnit )
	{
		memcpy( this, pclBoxItemUnit, sizeof(CBoxItemUnit) );
	}
}

UI16 CBoxItemUnit::GetItemNum( void ) const
{
	// min > max 경우(이 경우는 리소스가 잘못된것이라고 생각)
	// min == max 경우
	// min < max 경우
	// 위의 세가지 상황을 고려해야 한다

	if ( m_uiMinNum < m_uiMaxNum )
	{
		UI16 uiRange		= m_uiMaxNum - m_uiMinNum;
		UI16 uiReturnValue	= (rand() % uiRange) + m_uiMinNum;

		return uiReturnValue;
	}
	else if ( m_uiMinNum == m_uiMaxNum )
	{
		return m_uiMinNum; // min이나 max 둘중 어떤값을 리턴해도 됨
	}

	return 0;
}

/************************************************************************************************************************
* 클래스 이름 : CBoxItemSet
* 목적 : 하나의 상자 아이템을 위한 클래스
************************************************************************************************************************/
CBoxItemSet::CBoxItemSet( void )
	: m_uiBoxItemUnique(0)
	, m_uiBoxItemType(0)
	, m_pclVecBoxItemUnit(NULL)
{
}

CBoxItemSet::CBoxItemSet( UI16 uiBoxItemUnique, UI16 uiBoxItemType, NkVec_BoxItemUnit* pclVecBoxItemUnit/* =NULL  */)
	: m_uiBoxItemUnique(uiBoxItemUnique)
	, m_uiBoxItemType(uiBoxItemType)
	, m_pclVecBoxItemUnit(pclVecBoxItemUnit)
{
}

CBoxItemSet::CBoxItemSet( const CBoxItemSet& rclBoxItemSet )
	: m_uiBoxItemUnique(rclBoxItemSet.m_uiBoxItemUnique)
	, m_uiBoxItemType(rclBoxItemSet.m_uiBoxItemType)
	, m_pclVecBoxItemUnit(rclBoxItemSet.m_pclVecBoxItemUnit)
{

}

CBoxItemSet::~CBoxItemSet( void )
{
	ClearBoxItemUnit();

	if ( m_pclVecBoxItemUnit )
	{
		// 해제
		NkDelete m_pclVecBoxItemUnit;
		m_pclVecBoxItemUnit = NULL;
	}
}

/************************************************************************************************************************
* 함수명 : Init
* 목  적 : 초기화
* 인  수 :
* 리턴값 : 
************************************************************************************************************************/
void CBoxItemSet::Init( void )
{
	m_uiBoxItemUnique	= 0;
	m_uiBoxItemType		= 0;

	ClearBoxItemUnit();
}

/************************************************************************************************************************
* 함수명 : ClearBoxItemUnit
* 목  적 : 아이템 목록의 모든 메모리를 해제한다
* 인  수 :
* 리턴값 : 
************************************************************************************************************************/
void CBoxItemSet::ClearBoxItemUnit( void )
{
	if ( m_pclVecBoxItemUnit )
	{
		// 내부에 있는 데이터를 먼저 해재한다
		UI32 uiMaxCount = m_pclVecBoxItemUnit->GetCount();
		for ( UI32 uiIndex=0; uiIndex<uiMaxCount; uiIndex++ )
		{
			delete m_pclVecBoxItemUnit->GetData(uiIndex);
		}
	}
}

/************************************************************************************************************************
* 함수명 : GetRandItem
* 목  적 : 아이템 목록 중에서 아이템유닛 구조체를 랜덤하게 하나 가져온다
* 인  수 :
* 리턴값 : 랜덤하게 가져오는 CBoxItemUnit포인터
************************************************************************************************************************/
CBoxItemUnit* CBoxItemSet::GetRandItem( void )
{
	if ( NULL == m_pclVecBoxItemUnit )
	{
		return NULL;
	}

	SI32 siGetItemRandValue		= rand() % Const_Max_BoxItemMgr_Probability;
	SI32 siNowValue				= 0;
	SI32 siProbility			= 0;

	UI32 uiMaxCount				= m_pclVecBoxItemUnit->GetCount();
	for ( UI32 uiIndex=0; uiIndex<uiMaxCount; uiIndex++ )
	{
		siProbility = m_pclVecBoxItemUnit->GetData(uiIndex)->GetProbability();

		if ( (siNowValue <= siGetItemRandValue) && (siGetItemRandValue < siNowValue+siProbility) )
		{
			return m_pclVecBoxItemUnit->GetData(uiIndex);
		}

		siNowValue += siProbility;
	}

	return NULL;
}

/************************************************************************************************************************
* 함수명 : LoadBoxItemSet
* 목  적 : 상자 아이템 리소스를 읽어온다
*			서버에서만 쓰임
* 인  수 :	TCHAR* szFileName	- 읽어올 리소스 파일 이름 ("ServerData/Box/'유니크'.txt" 의 형식임)
* 리턴값 : 성공적으로 리소스를 읽어왔다면 TRUE
************************************************************************************************************************/
BOOL CBoxItemSet::LoadBoxItemSet( TCHAR* const szFileName )
{
	NDataLoader	dataloader;
	BOOL		bLoadFile	= FALSE;

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
		MsgBox(TEXT("Error"), TEXT("LoadError:[%s]"), szFileName);
		return FALSE;
	}

	if ( NULL == m_pclVecBoxItemUnit )
	{
		m_pclVecBoxItemUnit = NkNew NkVec_BoxItemUnit;
	}

	SI32 siItemUnique	= 0;
	SI32 siMinNum		= 0;
	SI32 siMaxNum		= 0;
	SI32 siUseDate		= 0;
	SI32 siProbability	= 0;
	SI32 siCanNotice	= 0;

	//확인 변수
	SI32 siTotalProb	= 0; // 각각의 상자안에 들어갈 물품의 확률 합은Const_Max_BoxItemMgr_Probability 이어야 한다

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siItemUnique,	4,
		NDATA_INT32,	&siMinNum,		4,
		NDATA_INT32,	&siMaxNum,		4,
		NDATA_INT32,	&siUseDate,		4,
		NDATA_INT32,	&siProbability,	4,
		NDATA_INT32,	&siCanNotice,	4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			CBoxItemUnit* pclBoxItemUnit = new CBoxItemUnit( siItemUnique, siMinNum, siMaxNum, siUseDate, siProbability, siCanNotice );

			m_pclVecBoxItemUnit->PushBack( pclBoxItemUnit );

			// 확률 확인용
			siTotalProb += siProbability;
		}
	}

	// 더이상 넣을 것이 없음
	m_pclVecBoxItemUnit->Resize();

	if ( Const_Max_BoxItemMgr_Probability != siTotalProb )
	{
		MsgBox(TEXT("Error"), TEXT("Probility Over Max[%d]/Now[%d], File[%s]"), Const_Max_BoxItemMgr_Probability, siTotalProb, szFileName);
		return FALSE;
	}

	return TRUE;
}

/************************************************************************************************************************
* 함수명 : GetInItemList
* 목  적 : 안에 들어있는 아이템의 리스트를 얻어온다
* 인  수 :	SI32* psiInBoxItemList	- 아이템 리스트를 저장할 배열
*			UI32 uiListCount		- 아이템 리스트를 저장할 배열의 갯수
* 리턴값 : 아이템 리스트를 복사한 갯수
************************************************************************************************************************/
UI32 CBoxItemSet::GetInItemList( OUT SI32* psiInBoxItemList, IN const UI32 uiListCount )
{
	if ( (NULL == psiInBoxItemList) || (0 == uiListCount) )
	{
		return 0;
	}

	if ( NULL == m_pclVecBoxItemUnit )
	{
		return 0;
	}

	UI32 uiMaxCount	= min(m_pclVecBoxItemUnit->GetCount(), uiListCount);

	for ( UI32 uiIndex=0; uiIndex<uiMaxCount; uiIndex++ )
	{
		psiInBoxItemList[uiIndex] = m_pclVecBoxItemUnit->GetData(uiIndex)->GetItemUnique();
	}

	return uiMaxCount;
}

/************************************************************************************************************************
* 함수명 : IsItemCanNotice
* 목  적 : 인수로 들어온 아이템이 대박 물품인지 검사
* 인  수 :	SI32 siInItemUnique		- 상자 아이템 안에 들어있는 아이템 유니크
* 리턴값 : 대박 물품이 맞다면 TRUE
************************************************************************************************************************/
BOOL CBoxItemSet::IsItemCanNotice( const SI32 siInItemUnique )
{
	if ( NULL == m_pclVecBoxItemUnit )
	{
		return FALSE;
	}

	UI32 uiMaxCount	= m_pclVecBoxItemUnit->GetCount();
	for ( UI32 uiIndex=0; uiIndex<uiMaxCount; uiIndex++ )
	{
		if ( m_pclVecBoxItemUnit->GetData(uiIndex)->GetItemUnique() == siInItemUnique )
		{
			return DATA_TO_TYPE_BOOL( m_pclVecBoxItemUnit->GetData(uiIndex)->GetCanNotice() );
		}
	}

	return FALSE;
}

/************************************************************************************************************************
* 클래스 이름 : CBoxItemMgr
* 목적 : 모든 상자 아이템을 관리함
************************************************************************************************************************/
CBoxItemMgr::CBoxItemMgr( void )
{

}

CBoxItemMgr::~CBoxItemMgr( void )
{

}

/************************************************************************************************************************
* 함수명 : LoadBoxItemInfo
* 목  적 : 상자 아이템의 기본이 되는 정보 리소스를 읽어온다
* 인  수 :
* 리턴값 : 성공적으로 리소스를 읽어왔다면 TRUE
************************************************************************************************************************/
BOOL CBoxItemMgr::LoadBoxItemInfo( void )
{
	NDataLoader	dataloader;
	BOOL		bLoadFile	= FALSE;
	TCHAR*		szFileName	= TEXT("Data/BoxInfo.txt");

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
		MsgBox(TEXT("Error"), TEXT("LoadError:[%s]"), szFileName);
		return FALSE;
	}

	SI32 siItemUnique	= 0;
	SI32 siItemMode		= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siItemUnique,	4,
		NDATA_INT32,	&siItemMode,	4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			CBoxItemSet clBoxItemSet( siItemUnique, siItemMode );

			m_clMapBoxItemSet.Insert( siItemUnique, clBoxItemSet );
		}
	}

	return TRUE;
}

/************************************************************************************************************************
* 함수명 : LoadAllBoxItemSet
* 목  적 : 모든 상자 유니크 리소스를 읽어온다
*			서버에서만 쓰임
* 인  수 :
* 리턴값 :
************************************************************************************************************************/
void CBoxItemMgr::LoadAllBoxItemSet( void )
{
	TCHAR			szFileName[MAX_PATH]	= {0,};
	SI32			siItemUnique			= 0;

	UI32			uiMaxCount				= m_clMapBoxItemSet.GetCount();

	NkRbTreeMapPos	kPos					= m_clMapBoxItemSet.GetBeginPos();

	for ( UI32 uiCount=0; uiCount<uiMaxCount; uiCount++ )
	{
		siItemUnique = m_clMapBoxItemSet.GetKey( kPos );
		if ( 0 < siItemUnique )
		{
			StringCchPrintf( szFileName, MAX_PATH, TEXT("ServerData/Box/%d.txt"), siItemUnique );

			m_clMapBoxItemSet.GetData( kPos ).LoadBoxItemSet( szFileName );
		}

		// 다음 포지션
		kPos = m_clMapBoxItemSet.GetNextPos( kPos );
	}
	/*
	for ( NkRbTreeMapPos kPos=m_clMapBoxItemSet.GetBeginPos(); kPos!=m_clMapBoxItemSet.GetEndPos(); kPos=m_clMapBoxItemSet.GetNextPos(kPos) )
	{
		siItemUnique = m_clMapBoxItemSet.GetKey( kPos );
		if ( 0 < siItemUnique )
		{
			StringCchPrintf( szFileName, MAX_PATH, TEXT("ServerData/Box/%d.txt"), siItemUnique );

			m_clMapBoxItemSet.GetData( kPos ).LoadBoxItemSet( szFileName );
		}
	}
	*/

}

/************************************************************************************************************************
* 함수명 : GetRandItemFromUnique
* 목  적 : 특정 상자 아이템안에 들어있는 아이템유닛 구조체를 랜덤하게 하나 가져온다
* 인  수 :	SI32 siBoxItemUnique	- 상자 아이템 유니크
* 리턴값 : 랜덤하게 가져오는 CBoxItemUnit포인터
************************************************************************************************************************/
CBoxItemUnit* CBoxItemMgr::GetRandItemFromUnique( const SI32 siBoxItemUnique )
{
	CBoxItemSet* pclBoxItemSet = m_clMapBoxItemSet.FindDataPtr(siBoxItemUnique);

	if ( NULL == pclBoxItemSet )
	{
		return NULL;
	}

	return pclBoxItemSet->GetRandItem();
}

/************************************************************************************************************************
* 함수명 : IsSameBoxItemType
* 목  적 : 특정 상자 아이템의 타입이 인수로 들어온 타입이 맞는지 검사
* 인  수 :	SI32 siBoxItemUnique	- 상자 아이템 유니크
*			SI32 siBoxItemType		- 검사하고자 하는 타입
* 리턴값 : 해당 타입의 상자 아이템이 맞다면 TRUE
************************************************************************************************************************/
BOOL CBoxItemMgr::IsSameBoxItemType( const SI32 siBoxItemUnique, const SI32 siBoxItemType )
{
	// 상자 유니크 확인
	CBoxItemSet* pclBoxItemSet = m_clMapBoxItemSet.FindDataPtr(siBoxItemUnique);
	if ( NULL == pclBoxItemSet )
	{
		return FALSE;
	}

	// 모드확인
	if ( pclBoxItemSet->GetBoxITemType() != siBoxItemType )
	{
		return FALSE;
	}

	return TRUE;
}
/************************************************************************************************************************
* 함수명 : GetInBoxItemList
* 목  적 : 특정 상자 아이템 안에서 나오는 아이템의 리스트를 얻어온다
* 인  수 :	SI32 siBoxItemUnique	- 상자 아이템 유니크
*			SI32* psiInBoxItemList	- 아이템 리스트를 저장할 배열
*			UI32 uiListCount		- 아이템 리스트를 저장할 배열의 갯수
* 리턴값 : 아이템 리스트를 복사한 갯수
************************************************************************************************************************/
UI32 CBoxItemMgr::GetInBoxItemList( IN const SI32 siBoxItemUnique, OUT SI32* psiInBoxItemList, IN const UI32 uiListCount )
{
	CBoxItemSet* pclBoxItemSet = m_clMapBoxItemSet.FindDataPtr(siBoxItemUnique);
	if ( NULL == pclBoxItemSet )
	{
		return 0;
	}

	return pclBoxItemSet->GetInItemList( psiInBoxItemList, uiListCount );
}

/************************************************************************************************************************
* 함수명 : IsInBoxItemCanNotice
* 목  적 : 특정 상자 아이템 안에서 나오는 아이템이 대박 물품인지를 검사한다
* 인  수 :	SI32 siBoxItemUnique	- 상자 아이템 유니크
*			SI32 siInItemUnique		- 상자 아이템 안에 들어있는 아이템 유니크
* 리턴값 : 대박물품일경우 TRUE
************************************************************************************************************************/
BOOL CBoxItemMgr::IsInBoxItemCanNotice( const SI32 siBoxItemUnique, const SI32 siInItemUnique )
{
	CBoxItemSet* pclBoxItemSet = m_clMapBoxItemSet.FindDataPtr(siBoxItemUnique);
	if ( NULL == pclBoxItemSet )
	{
		return FALSE;
	}

	return pclBoxItemSet->IsItemCanNotice( siInItemUnique );
}
