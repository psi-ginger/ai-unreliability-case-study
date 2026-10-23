#include ".\RewardItemByProbMgr.h"

CRewardTypeItemByProbMgr::CRewardTypeItemByProbMgr()
{
	m_uiRewardType = 0;
	m_uiItemCount  = 0;
	m_pVecRewardItem = NULL;
}

CRewardTypeItemByProbMgr::~CRewardTypeItemByProbMgr(void)
{
	Destroy();
}

//	데이터 생성 - 파일 읽고 메모리에 담기
BOOL	CRewardTypeItemByProbMgr::Create(UI32 uiRewardType, UI32 uiCount)
{
	m_uiRewardType = 0;
	m_uiItemCount  = 0;
	m_pVecRewardItem = NULL;

	// 보상아이템리스트 생성
	m_pVecRewardItem = NkNew NkVector<CRewardItemByProb>(uiCount,uiCount);
	
	// 보상 타입
	m_uiRewardType = uiRewardType;

	return TRUE;
}
// 제거
BOOL	CRewardTypeItemByProbMgr::Destroy()
{
	if( NULL != m_pVecRewardItem)
	{
		NkDelete m_pVecRewardItem;
		m_pVecRewardItem = NULL;
	}

	return TRUE;
}


// 아이템 정보를 분석한다. 
BOOL	CRewardTypeItemByProbMgr::AnalyzeItemInfo(TCHAR* pszData)
{
	TCHAR szTemp;

	// 아이템 
	TCHAR szItemName[MAX_PATH];
	SI32 siItemNameIndex = 0;
	bool bItemNameSwitch = false;

	// 아이템 최대수
	TCHAR szItemNum[MAX_PATH];
	SI32 siItemNumIndex	= 0;
	bool bItemNumSwitch = true;

	// 아이템 최소수
	TCHAR szItemMinNum[MAX_PATH];
	SI32 siItemMinNumIndex	= 0;
	bool bItemMinNumSwitch = true;

	// 아이템 확율
	TCHAR szItemPbt[128];
	SI32 siItemPbtIndex	= 0;
	bool bItemPbtSwitch = false;

	SI32 siTextIndex	= 0;

	bool bUnLimitLoop = true;

	
	while( bUnLimitLoop )
	{
		szTemp = pszData[siTextIndex];
		if(szTemp == '\0')break;

		siTextIndex++;

		// Space는 무시한다. 
		if( szTemp == ' ')continue;
		if( szTemp == '+')
		{
			bItemNameSwitch	= false;
			bItemNumSwitch	= true;
			bItemMinNumSwitch = true;

			continue;
		}
		if( szTemp == '(')
		{
			szItemName[siItemNameIndex] = '\0';	// 이름을 마무리한다. 
			siItemNameIndex	= 0;

			bItemNameSwitch = true;	// 이름은 얻어왔다. 다음은 아이템확률 순서다. 
			continue;
		}
		if( szTemp == '-')
		{
			bItemMinNumSwitch = false;	// 아이템 최소수를 구하는 모드이다. 
		}
		if( szTemp == ':')
		{
			bItemMinNumSwitch = true ;
			bItemNumSwitch = false;	// 아이템 최대수를 구하는 모드이다. 
		}
		if( szTemp == ')')
		{

			szItemMinNum[siItemMinNumIndex] = '\0';	// 아이템 최소수를 마무리한다. 
			siItemMinNumIndex	= 0;	

			szItemNum[siItemNumIndex] = '\0';		// 아이템 최대수를 마무리한다. 
			siItemNumIndex	= 0;	

			szItemPbt[siItemPbtIndex] = '\0';		// 아이템 확률을 마무리한다. 
			siItemPbtIndex	= 0;

			//-------------------------------------------
			// 이름과 확률을 모두 얻어왔으므로 분석한다. 
			//-------------------------------------------

			// NULL이면 리턴한다. 
			if(_tcscmp(szItemName, TEXT("NULL")) == 0)
			{
				return FALSE;
			}

			SI32 siUnique = _tstoi(szItemName);
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(siUnique);
			//if(unique)
			if ( ref > 0 )
			{
				SI32 itempbt	= _tstoi(szItemPbt);

				SI32 itemnum;
				SI32 minnum;

				if(bItemNumSwitch == false )
				{
					itemnum = _tstoi(szItemNum);
			
					if(itemnum <= 0)	itemnum	= 1;

					minnum = _tstoi(szItemMinNum);
					if(minnum <= 0)	minnum	= 1;

					if ( minnum > itemnum )
						minnum = 1 ;
				}
				else
				{
					itemnum = 1;
					minnum = 1;
				}
				
				// 보상 아이템 정보 입력
		
				CRewardItemByProb& clRewardItemByProb = m_pVecRewardItem->GetData(m_uiItemCount);
				clRewardItemByProb.SetMinNum(minnum);
				clRewardItemByProb.SetNum(itemnum);
				clRewardItemByProb.SetProb(itempbt);
				clRewardItemByProb.SetUnique(siUnique);

				++m_uiItemCount;
			}
			else
			{
				MsgBox(TEXT("NotProperItem"), TEXT("text:[%s] itemname:[%s]"), pszData, szItemName);
			}
			continue;
		}

		// 아이템의 이름이 아직 결정되지 않았으면 이름을 입력받는다. 
		if(bItemNameSwitch  == false)
		{
			szItemName[siItemNameIndex ] = szTemp;
			siItemNameIndex ++;
			if(siItemNameIndex  > 100)return false;
		}
		else if(bItemMinNumSwitch  == false)
		{
			// 아이템 최소수를 구한다. 
			if(szTemp != '-')
			{
				szItemMinNum[siItemMinNumIndex] = szTemp;
				siItemMinNumIndex++;
				if(siItemMinNumIndex > 100)return false;
			}
		}
		else if(bItemNumSwitch   == false)
		{
			// 아이템 최대수를 구한다. 
			if(szTemp != ':')
			{
				szItemNum[siItemNumIndex] = szTemp;
				siItemNumIndex++;
				if(siItemNumIndex > 100)return false;
			}
		}
		else
		{
			szItemPbt[siItemPbtIndex] = szTemp;
			siItemPbtIndex++;
		}

	}
	
	return TRUE;
}
// 아이템 보상
BOOL	CRewardTypeItemByProbMgr::GetRandItemUnique(UI32* puiUnique,UI32* puiNum)
{
	srand((unsigned)time(NULL));
	UI32 uiRandValue = rand() % MAX_RANDVALUE;


	UI32 uiTemp = 0;
	
	for(UI32 i = 0 ; i < m_uiItemCount; i++)
	{
		CRewardItemByProb& pclRewardItemByProb = m_pVecRewardItem->GetData(i);
		
		// 임의의 값에 확률 수치를 더한다.
		uiTemp = uiTemp + pclRewardItemByProb.GetProb();

		if(  uiRandValue <= uiTemp)
		{	
			
			if(pclRewardItemByProb.GetNum() != 0)
				*puiNum = (rand() % pclRewardItemByProb.GetNum()) + 1;
			*puiNum = 0;
			*puiUnique = pclRewardItemByProb.GetUnique();

			return TRUE;
		}
	}
	
	return FALSE;
}
UI32	CRewardTypeItemByProbMgr::GetType()
{
	return m_uiRewardType;
}

//========================================================================================================

CRewardItemByProbMgr::CRewardItemByProbMgr()
{

}
CRewardItemByProbMgr::~CRewardItemByProbMgr()
{
	Destroy();
}
// 생성
BOOL	CRewardItemByProbMgr::Create()
{	
	for( SI32 siCount = REWARDTYPE::BATTLEROYAL_WINNER ; siCount <= REWARDTYPE::COUNT ; siCount++ )
	{
		CRewardTypeItemByProbMgr pclRewardTypeItemByProbMgr;
		m_pVecRewardItemMgr.PushBack(pclRewardTypeItemByProbMgr);
		if( FALSE == m_pVecRewardItemMgr.GetBackData().Create(  siCount , CRewardTypeItemByProbMgr::INIT_ITEMCOUNT ))
		{
			return FALSE;
		}
	}
		
	// 파일 로드
	if( FALSE == LoadFileData())
		return FALSE;
	return TRUE;
}
// 제거
BOOL	CRewardItemByProbMgr::Destroy()
{
	SI32 siCount = m_pVecRewardItemMgr.GetCount();

	for(SI32 i = 0; i < siCount; i++)
	{
		m_pVecRewardItemMgr.GetData(i).Destroy();
	}

	return TRUE;
}

// 파일 데이터 로드
BOOL	CRewardItemByProbMgr::LoadFileData()
{
	NDataLoader	dataloader;

	const SI32	MAX_TEXT = 1024;

	bool bResult = FALSE;

	// 암호화 여부
	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption))
		bResult = dataloader.LoadDataFromCryptFile(TEXT("Data/RewardTypeItemInfo.txt"));
	else
		bResult = dataloader.LoadDataFromFile(TEXT("Data/RewardTypeItemInfo.txt"));

	if (FALSE == bResult)
		return FALSE;

	SI32	siType = 0;
	TCHAR	szItemDataInfo[MAX_TEXT] = { TEXT('\0') };

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siType,	4,
		NDATA_MBSTRING,	&szItemDataInfo,	MAX_TEXT,
			0,0,0
	};

	// 데이터가 담길 포인터 넘김
	dataloader.RegDataType(pDataTypeInfo);

	// 데이터 읽어 오기 
	while(dataloader.IsEndOfData() != true) 
	{
		if (dataloader.ReadData() == true)	
		{
			CRewardTypeItemByProbMgr& pclRewardTypeItemByProbMgr = m_pVecRewardItemMgr.GetData(siType - 1);
			pclRewardTypeItemByProbMgr.AnalyzeItemInfo(szItemDataInfo);
		}
		else
			break;

	}
	return TRUE;
}

CRewardTypeItemByProbMgr*	CRewardItemByProbMgr::GetRewardTypeItemByProbMgr(UI32 uiRewardType)
{
	if( REWARDTYPE::COUNT < uiRewardType)
	{
		return NULL;
	}
	return (CRewardTypeItemByProbMgr*)&m_pVecRewardItemMgr.GetData(uiRewardType - 1);
}
