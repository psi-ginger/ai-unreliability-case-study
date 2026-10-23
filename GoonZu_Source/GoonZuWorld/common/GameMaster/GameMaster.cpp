//---------------------------------
// 2004/5/18 김태곤
//---------------------------------
#include "CommonLogic.h"
#include "GameMaster.h"
#include "..\Char\CharCommon\Char-Common.h"
#include "NDataLoader.h"
#include "NFile.h"
#include "NUtil.h"

#include "Char\CharManager\CharManager.h"
#include "..\DBManager\GameDBManager_World\DBMsg-Item.h"

#include "..\server\Server.h"

extern cltCommonLogic *pclClient;

cltGameMasterInfo::cltGameMasterInfo(SI32 unique, SI32 gmlevel, TCHAR* porder, TCHAR* pexplain, TCHAR* psampleuse, SI32 aAtb)
{

	siUnique		= unique;
	siNeedGMLevel	= gmlevel;
	MStrCpy( szOrder, porder, MAX_GM_ORDER_NAME_LENGTH );
	
	SI32 size  = lstrlen(pexplain);
	if(size >= 1024)
	{
		MsgBox(TEXT("fdf2"), TEXT("fdsfd:%s"), porder);
	}

	pExplain = new TCHAR[size + 1];
	StringCchCopy(pExplain, size + 1, pexplain);


	size  = lstrlen(psampleuse);
	if(size >= 1024)
	{
		MsgBox(TEXT("fdf2"), TEXT("fdsfdss:%s"), porder);
	}

	pSampleUse = new TCHAR[size + 1];
	StringCchCopy(pSampleUse, size + 1, psampleuse);

	mGMatb = aAtb;
}

cltGameMasterInfo::~cltGameMasterInfo()
{
	if(pExplain)
	{
		delete[] pExplain;
		pExplain = NULL;
	}
	
	if( pSampleUse )
	{
		delete [] pSampleUse;
		pSampleUse = NULL;
	}
}

cltGameMasterManager::cltGameMasterManager()
{
	m_clSupplyItem.Init();
	m_szSupplyMsg[0] = 0;
	m_siGMInfoCount = 1;
	
	kUpdateTimer.Init(1* 60 * 1000);		// 1분 마다 한번
	kUpdateTimer.SetActive(true, GetTickCount());

	SI32 i;

	for(i = 0;i < MAX_GMINFO_NUMBER;i++)
	{
		pclGMInfo[i] = NULL;
	}

	NDataAtbParser  atbparser;

	// GM_ATB 등록
	atbparser.AddAtbInfo( TEXT("GM_ATB_NONE"), GM_ATB_NONE);
	atbparser.AddAtbInfo( TEXT("GM_ATB_CHARUNIQUE"), GM_ATB_CHARUNIQUE);
	atbparser.AddAtbInfo( TEXT("GM_ATB_MONSTER"), GM_ATB_MONSTER);
	atbparser.AddAtbInfo( TEXT("GM_ATB_ITEM"), GM_ATB_ITEM);
	atbparser.AddAtbInfo( TEXT("GM_ATB_SKILL"), GM_ATB_SKILL);
	atbparser.AddAtbInfo( TEXT("GM_ATB_VILLAGE"), GM_ATB_VILLAGE);
	atbparser.AddAtbInfo( TEXT("GM_ATB_HUNTMAP"), GM_ATB_HUNTMAP);
	atbparser.AddAtbInfo( TEXT("GM_ATB_ETC"), GM_ATB_ETC);

	BOOL bLoadFile = FALSE;

	NDataLoader	dataloader;

	NFile file;

	file.LoadFile( TEXT("Data/GMInfo.dat") );
	if( NULL == file.GetBuffer() )
	{
		MessageBox(NULL, "Data/GMInfo.dat", "file open fail", 0);
		return;
	}

	NCryptHeader *pHeader = (NCryptHeader *)file.GetBuffer();
	BYTE *pData = (BYTE*)pHeader + sizeof( NCryptHeader );

	int size = pHeader->dwDataSize;
	BYTE key = pHeader->key;

	BYTE *pBuf = new BYTE[ file.m_dwFileSize ];

	NByteDecrypt( pData, pBuf, size, key );

	dataloader.LoadDataFromMem( pBuf, size );

	NDelete_Array( pBuf );

	bLoadFile = true;

	if (!bLoadFile)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("GameMasterInfo Load Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
	}
	else
	{
		SI32 gmlevel = -1, iAtb = -1;
		TCHAR order[MAX_GM_ORDER_NAME_LENGTH], explain[1024], sampleuse[1024], atb[256];
	    
		NDataTypeInfo pDataTypeInfo[] =
		{
				NDATA_INT32,	&gmlevel,		4,
				NDATA_MBSTRING,	order,		MAX_GM_ORDER_NAME_LENGTH,
				NDATA_MBSTRING,	explain,		1024,
				NDATA_MBSTRING,	sampleuse,		256,
				NDATA_MBSTRING,	atb,		64,
				0,0,0
		};

		dataloader.RegDataType(pDataTypeInfo);

		while( !dataloader.IsEndOfData() ) 
		{
			iAtb = -1;
			if (dataloader.ReadData())
			{
				iAtb = atbparser.GetValueByString( atb );

				if ( order[ 0 ] != NULL && iAtb != -1)
				{
					Add(gmlevel, order, explain, sampleuse, iAtb);
				}
			}
		}
	}
}

cltGameMasterManager::~cltGameMasterManager()
{
	SI32 i;

	for(i = 0;i < MAX_GMINFO_NUMBER;i++)
	{
		if(pclGMInfo[i])
		{
			delete pclGMInfo[i];
			pclGMInfo[i] = NULL;
		}
	}

}

//KHY -0702- 접속중인 모든 유저들에게, 예약한 시간에 아이템을 제공한다. 
void cltGameMasterManager::Action( )
{
	if(timeSwitch != TRUE)
		return;

	if(kUpdateTimer.IsActivated() == false)
		return;

	//_SYSTEMTIME startTime;
	//ZeroMemory(&startTime, sizeof(_SYSTEMTIME));
	cltServer* pclserver = (cltServer*)pclClient;

	SI16 hour = (SI16)pclserver->sTime.wHour;
	SI16 minute = (SI16)pclserver->sTime.wMinute;


	if( (hour== time_hour) &&(minute == time_min))
	{
		timeSwitch = FALSE;

		pclClient->pclCM->GiveItem(0, itemunique, num, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_MASTER, usedate);

	 	time_hour = 0;
		time_min = 0;
		itemunique = 0;
		num = 0;
		usedate = 0;
	}
}

void cltGameMasterManager:: giveItemTime(UI32 item_hour,UI32 item_min,SI32 item_unique,SI32 item_num,UI16 item_usedate )
{
 	time_hour = item_hour;
	time_min = item_min;
	itemunique = item_unique;
	num = item_num;
	usedate = item_usedate;
}
//////////////////////////////////////////////////////////////////////

// 운영자 권한을 추가한다. 
void cltGameMasterManager::Add(SI32 gmlevel, TCHAR* porder, TCHAR* pexplain, TCHAR* psampleuse, SI32 aAtb)
{
	/*cyj 운영자 권한 추가 명령 수정
	for(i = 1;i < MAX_GMINFO_NUMBER;i++)
	{
		if(pclGMInfo[i])continue;

		pclGMInfo[i] = new cltGameMasterInfo(i, gmlevel, porder, pexplain, psampleuse, aAtb);
		return ;
	}
	*/

	while (m_siGMInfoCount < MAX_GMINFO_NUMBER && pclGMInfo[m_siGMInfoCount] != NULL)
	{
		m_siGMInfoCount++;
	}

	if (m_siGMInfoCount < MAX_GMINFO_NUMBER && pclGMInfo[m_siGMInfoCount] == NULL)
	{
		pclGMInfo[m_siGMInfoCount] = new cltGameMasterInfo(m_siGMInfoCount, gmlevel, porder, pexplain, psampleuse, aAtb);
		m_siGMInfoCount++;
	}
	else
	{
#ifdef _DEBUG
		MsgBox(TEXT("asf134d"),TEXT("pclGMInfo = %d"),m_siGMInfoCount);
#endif
	}
}

// 명령을 근거로 유니크를 찾는다. 
SI32 cltGameMasterManager::FindUniqueFromOrder(TCHAR* porder)
{
	SI32 i;

	for(i = 1;i < MAX_GMINFO_NUMBER;i++)
	{
		if(pclGMInfo[i] == NULL)continue;

		if(_tcscmp(porder, pclGMInfo[i]->szOrder) == 0)
		{
			return i;
		}
	}

	return 0;
}

// 운영자 명령에 대한 전제 설명을 얻어온다 (2K의 텍스트 사이즈를 확보해서 넘겨야 한다) 
bool cltGameMasterManager::GetText(TCHAR* porder, TCHAR* ptext, SI16 txtSize)
{
	SI32 gmunique = FindUniqueFromOrder(porder);
	if(gmunique <= 0)return false;

	TCHAR *ptxt = GetTxtFromMgr(6337);

	StringCchPrintf(ptext, txtSize, ptxt, 
	pclGMInfo[gmunique]->szOrder,
	pclGMInfo[gmunique]->siNeedGMLevel,
	pclGMInfo[gmunique]->pExplain,
	pclGMInfo[gmunique]->pSampleUse);


	return true;

}

bool cltGameMasterManager::SetSupplyItem(IN const cltItem * pclitem)
{
	if( pclitem == NULL ) return false;
	
	m_clSupplyItem.Set(pclitem);

	return true;
}

bool cltGameMasterManager::GetSupplyItem(OUT cltItem * pclitem)
{	
	if( pclitem == NULL ) return false;
	
	if( m_clSupplyItem.siUnique <= 0) return false;

	pclitem->Set(&m_clSupplyItem);
	
	return true;
}

void cltGameMasterManager::SetSupplyMsg(const TCHAR * msg)
{
	MStrCpy(m_szSupplyMsg ,  msg , 256);
}

TCHAR * cltGameMasterManager::GetSupplyMsg()
{
	return m_szSupplyMsg;		
}
