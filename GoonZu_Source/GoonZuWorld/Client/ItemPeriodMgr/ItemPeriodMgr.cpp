#include "ItemPeriodMgr.h"
#include "CommonHeader.h"
#include "NDataLoader.h"
#include "CommonLogic.h"
#include "Client.h"
#include "ninterface/nPeriodItemListDlg/NPeriodItemListDlg.h"


CItemPeriodMgr::CItemPeriodMgr(void)
{
	m_SItemPeriodObj.reserve(0);
}

CItemPeriodMgr::~CItemPeriodMgr(void)
{
}

class CNPeriodItemListDlg;

bool CItemPeriodMgr::Initialize()
{
	NDataLoader	dataloader;

	bool bResult;

	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption))
		bResult = dataloader.LoadDataFromCryptFile(TEXT("Data/ItemPeriod_Info.txt"));
	else
		bResult = dataloader.LoadDataFromFile(TEXT("Data/ItemPeriod_Info.txt"));

	if (bResult == false)
		return false;

	TCHAR	szBuffName[MAX_PATH] = TEXT("");
	SI32	siTabIndex = 0;
	SI32	siIconIndex = 0;
	SI32	siTextindex = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szBuffName,		MAX_PATH,
		NDATA_INT32,	&siTabIndex,	4,
		NDATA_INT32,	&siIconIndex,	4,
		NDATA_INT32,	&siTextindex,	4,
		0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo);

	SI32 siCount = 0;


	// 
	while(dataloader.IsEndOfData() != true) 
	{
		if (dataloader.ReadData() == true)	
		{
			if(siCount >=  CNPeriodItemListDlg::BuffIndex_End)
				break;

			sItemPeriodObj sItemPeriodData;
			ZeroMemory(&sItemPeriodData, sizeof(sItemPeriodObj));

			sItemPeriodData.m_siTabIndex	= siTabIndex;
			sItemPeriodData.m_siIconIndex	= siIconIndex;
			sItemPeriodData.m_siTextindex	= siTextindex;
			StringCchCopy(sItemPeriodData.m_szBuffName, MAX_PATH, szBuffName);

#ifdef _DEBUG
			if(sItemPeriodData.m_siTabIndex < 0 || sItemPeriodData.m_siTabIndex > 2)
				MsgBox("Error", "[ItemPeriod_Info.txt]::TabIndex is WrongData");
#endif
			

			m_SItemPeriodObj.push_back(sItemPeriodData);

			siCount++;
		}
		else
			return true;

	}
	return false;
}


sItemPeriodObj* CItemPeriodMgr::GetBuffData(SI32 siBuffIndex)
{
	sItemPeriodObj* pData = NULL;

	if(siBuffIndex < 0 || siBuffIndex >= CNPeriodItemListDlg::BuffIndex_End)
	{
		assert(0);
		return NULL;
	}

	return &m_SItemPeriodObj.at(siBuffIndex);
}
