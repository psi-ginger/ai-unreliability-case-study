//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"


//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\Common\Order\Order.h"
#include "..\Common\Smoke\Smoke.h"
#include "..\Common\Map\Map.h"
#include "Char\CharClient\Char-Client.h"
#include "Char\CharManager\CharManager.h"

// Client
#include "Client.h"
#include "Music\Music.h"
#include "Effect\Regeneration\Regeneration.h"

// CommonLogic
#include "MsgType-System.h"

// Server
#include "..\Server\Candidate\Candidate.h"


#include "..\resource.h"




extern cltCommonLogic* pclClient;


//------------------------------------------
// SetChar
//-------------------------------------------

// 캐릭터를 안전할 때 만들 수 있도록 버퍼에 넣는다. 
BOOL cltClient::PushChar(cltMakeCharInfo* pclinfo)
{
	if(siSetCharBufferIndex >= MAX_SETCHAR_RESERVE_NUMBER)
	{
		if (pclClient->GameMode == GAMEMODE_SERVER)
			pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("PushCharBufferFull"));
		return FALSE;
	}

	memcpy(&cltSetCharBuffer[siSetCharBufferIndex],  pclinfo, sizeof(cltMakeCharInfo));

	siSetCharBufferIndex++;

	return TRUE;
}

BOOL cltClient::PopChar(cltMakeCharInfo* pclinfo)
{
	SI32 i;

	if(siSetCharBufferIndex == 0)return FALSE;
	// 마을 건물 정보가 서버로부터 오기를 기다리는 중이라면 캐릭터를 생성해서는 안된다. 
	// 캐릭터가 먼저 만들어지면 나중에 지어지는 건물에 의해 가려질 수 있기 때문이다. 
	if(bWaitVillageStructureInfoSwitch == true)return FALSE;

#ifdef _SAFE_MEMORY
	memcpy(pclinfo, &cltSetCharBuffer[0], sizeof(cltMakeCharInfo));
#else
	memcpy(pclinfo, cltSetCharBuffer, sizeof(cltMakeCharInfo));
#endif
	for(i = 1; i < siSetCharBufferIndex;i++)
	{
		memcpy(&cltSetCharBuffer[i - 1], &cltSetCharBuffer[i], sizeof(cltMakeCharInfo));
	}

	siSetCharBufferIndex --;

	return TRUE;
}

// 생산 예약된 캐릭터에서 특정 캐릭터를 삭제한다. 
bool cltClient::DeleteReservedChar(SI32 charunique)
{

	SI32 i;

	for(i = 0; i < siSetCharBufferIndex;i++)
	{
		if((SI32)cltSetCharBuffer[i].uiMakeCharUnique == charunique)
		{
			memcpy(&cltSetCharBuffer[i], &cltSetCharBuffer[siSetCharBufferIndex-1], sizeof(cltMakeCharInfo));
			--siSetCharBufferIndex;
			return true;
		}
	}

	return false;
}

// 만들도록 예약되어 있는 캐릭터를 만든다. 
BOOL cltClient::SetCharReservedChar()
{
	SI32 id;
	cltMakeCharInfo clinfo;
	TCHAR buffer[256];

	// 서버 정보를 받지 못했다면 캐릭터를 생성하지 않는다. 
	if(bReceiveServerInfoSwitch == false)return FALSE;

	while(PopChar(&clinfo) == TRUE)
	{
		// 만들어야 할 캐릭터가 중복되어 있는 것은 아닌지 확인한다. 
		id = cltCharCommon::CharUniqueInfo[ clinfo.uiMakeCharUnique] ;
		if(id)
		{
			// 셀 내에서의 정보를 삭제한다. 
			pclCM->CR[id]->DeleteInCell();
			pclCM->DeleteChar(id);
		}

		// 한개의 메인 지도 이외의 캐릭터는 만들지 않는다. 
		if((SI32)clinfo.uiMapIndex != pclMapManager->CurrentMapIndex)
		{
			continue;
		}

		if(MyCharUnique == (SI32)clinfo.uiMakeCharUnique)
		{
			id = 1;
		}
		else
		{
			id = pclCM->pclIDManager->GetID(0);
		}

		id = SetChar(id, &clinfo);

		cltCharClient* pclchar = (cltCharClient*)pclCM->CR[id];
		if(pclchar == NULL)
		return false;

		if ( id != 1 )
		{
			pclchar->MakeRankMarkEncBuffer();
		}

		if(id)
		{
			//---------------------------------
			// 제조 에니메이션
			//---------------------------------

			pclchar->siPersonalMakeItemType = clinfo.siMakeAniType;
			
			pclchar->SetEnchantEffect(clinfo.siMakeAniType);

			//---------------------------------
			// 신분 
			//---------------------------------
			pclchar->pclCI->clCharRank.Set(&clinfo.clRank);
			pclchar->pclBoss->Set(clinfo.clRank.GetRankType());
			
			//---------------------------------
			// 리젠 효과 
			//---------------------------------
			if(id == 1)
			{
				pclchar->SetCharRegeneration();
			}

			pclchar->pclCI->clBI.clGuildRank.Set(&clinfo.clGuildRank);
			
			// 클라이언트 사용자의 운영자 권한 등급을 설정한다. 
			pclchar->SetGameMasterLevel(clinfo.siGameMasterLevel);
			pclchar->SetRareItemRate(0,0,0,0);
			pclchar->SetWennyOldMan(false,false);
		}


		// 서버로부터 받은 정보를 근거로 캐릭터를 만든다. 
		if((SI32)clinfo.uiMakeCharUnique == MyCharUnique )
		{

			// 주인공 캐릭터의 정보를 받았던 상태면, 
			if(bReceiveMyCharAllInfoSwitch == true)
			{
				// 백업된 정보가 있다면, 
				if(bBackUpSwitch == true)
				{
					//-------------------------------------------------------------------------------------------------
					pclchar->pclCI->Set(&pclHeroDataBackup->clMyCharPersonInfo);
					pclchar->siWarEnemyKillNumber				= pclHeroDataBackup->siWarEnemyKillNumber;
					pclchar->siWarKilledNumber					= pclHeroDataBackup->siWarKilledNumber;
					pclchar->siBlackArmyWarMapIndex				= pclHeroDataBackup->siBlackArmyWarMapIndex;

					//-------------------------------------------------------------------------------------------------
					pclchar->siBlackWarEnemyKillNumber			= pclHeroDataBackup->siBlackWarEnemyKillNumber;
					pclchar->siBlackWarKilledNumber				= pclHeroDataBackup->siBlackWarKilledNumber;

					//-------------------------------------------------------------------------------------------------
					//[추가 : 황진성 2008. 1. 28 => 흑의 군단 개편 정보 백업]
					pclchar->siBlackWarEnemyKillScore			= pclHeroDataBackup->siBlackWarEnemyKillScore;	       
					pclchar->siBlackWarEnemyKillVillageScore	= pclHeroDataBackup->siBlackWarEnemyKillVillageScore;

					//-------------------------------------------------------------------------------------------------
					pclchar->bStartMakeItemSwitch				= pclHeroDataBackup->bStartMakeItemSwitch;
					pclchar->dwStartMakeItemClock				= pclHeroDataBackup->dwStartMakeItemClock;
					pclchar->dwDelayMakeItemClock				= pclHeroDataBackup->dwDelayMakeItemClock;
					
					//-------------------------------------------------------------------------------------------------
					pclchar->ActiveWeapon						= pclHeroDataBackup->siOldActiveWeapon;
				
					//-------------------------------------------------------------------------------------------------
					pclchar->ActiveClothes						= pclHeroDataBackup->siOldActiveClothes;
					pclchar->ActiveHat							= pclHeroDataBackup->siOldActiveHat;
					pclchar->siCurrentMagic						= pclHeroDataBackup->siOldCurrnetMagic;
					pclchar->siCurrentMagicCastType				= pclHeroDataBackup->siOldCurrnetMagicCastType;
					
					//-------------------------------------------------------------------------------------------------
					pclchar->clPB.BasicMoveSpeed				= pclHeroDataBackup->siOldBasicMoveSpeed;
					pclchar->clPB.ShoesMoveSpeed				= pclHeroDataBackup->siOldShoesMoveSpeed;
					pclchar->clPB.TotalMoveSpeed				= pclHeroDataBackup->siOldTotalMoveSpeed;
					pclchar->clPB.GivenMoveSpeed				= pclHeroDataBackup->siOldGivenMoveSpeed;
/*
					pclchar->clPB.siGuildStr = pclHeroDataBackup->siGuildOldStr;
					pclchar->clPB.siGuildDex = pclHeroDataBackup->siGuildOldDex;
					pclchar->clPB.siGuildMag = pclHeroDataBackup->siGuildOldMag;
					pclchar->clPB.siGuildVit = pclHeroDataBackup->siGuildOldVit;
					pclchar->clPB.siGuildHnd = pclHeroDataBackup->siGuildOldHnd;
					pclchar->clPB.siGuildLuck = pclHeroDataBackup->siGuildOldLuck;
*/
					//-------------------------------------------------------------------------------------------------
					pclchar->clPB.clGuildBA.Set(&pclHeroDataBackup->clGuildOldBA);
					
					//-------------------------------------------------------------------------------------------------
					//이정보는 저장한후 복구 하는 정보가 아니다.
					pclchar->clAddInfo.Set(&pclHeroDataBackup->clOldAddInfo);
					pclchar->pclCI->clItemMall.Set(&pclHeroDataBackup->clOldItemMall); // 아이템 몰 정보 재저장
					//pclchar->pclCI->clLimitDateItem.Set(&pclHeroDataBackup->clOldLimitDateItemInfo);
					
					//-------------------------------------------------------------------------------------------------
					//[진성] 공간상자.  워프등의 캐릭터 초기화가 이루어질 때 백업된 정보를 쉐팅. => 2008-4-3.
					if(pclchar->pclSpaceBoxItem)	memcpy(pclchar->pclSpaceBoxItem,    &pclHeroDataBackup->clSpaceBoxItem,    sizeof(pclchar->pclSpaceBoxItem));
					if(pclchar->pclTreasureBoxItem)	memcpy(pclchar->pclTreasureBoxItem, &pclHeroDataBackup->clTreasureBoxItem, sizeof(pclchar->pclTreasureBoxItem));

					pclchar->m_clMagicBufMgr.Set( &pclHeroDataBackup->clOldMagicBufMgr );

					// 채팅 금지 제한시간 복사
					memcpy(&pclchar->m_stMuteTime, &pclHeroDataBackup->stMuteTime, sizeof(pclchar->m_stMuteTime));
					// 발렌타인데이 전쟁 
					pclchar->pclValentineWar->Set( &pclHeroDataBackup->clValentinewarinfo);

					pclchar->pclPVP_LeagueInfo->Set( &pclHeroDataBackup->clPVP_LeagueInfo);
				}
			}
			else
			{
				// 주인공 캐릭터의 모든 정보를 서버로 요청한다. 
				// Importantparameta, Skill등의 정보가 해당된다.
				cltMsg clMsg(GAMEMSG_REQUEST_ALLCHARINFO, 0, NULL);
				SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		else if(pclchar->clIdentity.siIdentity == IDENTITY_VILLAGEGOD)
		{
			TCHAR* pText = GetTxtFromMgr(847);
			

			StringCchPrintf(buffer, 256, TEXT("%s %s"),  pText, pclchar->pclKI->GetName());
			pclchar->SetName(buffer);
		}
		else if(pclchar->clIdentity.siIdentity == IDENTITY_VILLAGEGOD_SUMMON)
		{
			pclchar->TransMode = TRANSPARENT_MODE_SCREEN;
		}
		else if(pclchar->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
		{
			SI32 siparentid = pclchar->GetSummonParentID();
			if(pclCM->IsValidID(siparentid) == TRUE)
			{
				cltCharClient* pclparentchar = (cltCharClient*)pclCM->CR[siparentid];
				cltSummon *pclSummon = pclparentchar->pclCI->clSummonInfo.GetSummon(pclchar->GetSummonIndex());
				if(pclSummon)	StringCchPrintf(buffer, 256, TEXT("%s"),  pclSummon->szName);
				else 
				{
					TCHAR* pText = GetTxtFromMgr(449);
					

					StringCchPrintf(buffer, 256, TEXT("%s"),  pText);
				}

				cltKindInfo * pclsummoninfo = pclKindInfoSet->pclKI[pclSummon->siKind];
				if ( pclsummoninfo == NULL ) return false ;
			}
			else
			{
				StringCchPrintf(buffer, 256, TEXT("%s"),  pclchar->pclKI->GetName());
			}
			pclchar->SetName(buffer);
			
			// 소환수 이면 IP를 보내줄 것을 요청한다 
			cltMsg clMsg(GAMEMSG_REQUEST_PERSON_IP, pclchar->GetCharUnique());
			SendMsgToServer((sPacketHeader*)&clMsg);
		}
		// [영훈] 수호정령_090525 : 수호정령의 이름으로 설정
		else if ( IDENTITY_SOULGUARD == pclchar->clIdentity.siIdentity )
		{

		}



		//---------------------------------
		// 이용권을 사용한 워프 효과. 
		//---------------------------------
		if(clinfo.uiTicketWarpSwitch)
		{
			SI32 centerx, centery;
			if(pclchar->pclMap->GetXYDotFromXYCord(pclchar->GetX(), pclchar->GetY(),  centerx, centery) == TRUE)
			{
				pclClient->pclSmokeManager->SetSmoke(SMOKE_ENDWARP, centerx, centery - 115, pclchar->pclMap);
			}
			
		}


	}

	return TRUE;
}


void cltClient::NPCMgr( )
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	TCHAR datfilename[MAX_PATH] = {0,};


	if( pclClient->siServiceArea ==  ConstServiceArea_Korea )	// 한국
	{
		StringCchPrintf(datfilename,  MAX_PATH, TEXT("TextMgr\\NPCMgrK.txt") );
	}
	else if( pclClient->siServiceArea ==  ConstServiceArea_English )
	{
		StringCchPrintf(datfilename,  MAX_PATH, TEXT("TextMgr\\NPCMgrE.txt") );
	}
	else if(pclClient->siServiceArea ==  ConstServiceArea_China )
	{
		StringCchPrintf(datfilename,  MAX_PATH, TEXT("TextMgr\\NPCMgrC.txt") );
	}
	else if(pclClient->siServiceArea ==  ConstServiceArea_Japan )
	{
		StringCchPrintf(datfilename,  MAX_PATH, TEXT("TextMgr\\NPCMgrJ.txt") );
	}
	else if(pclClient->siServiceArea ==  ConstServiceArea_Taiwan )
	{
		StringCchPrintf(datfilename,  MAX_PATH, TEXT("TextMgr\\NPCMgrT.txt") );
	}
	else if(pclClient->siServiceArea ==  ConstServiceArea_USA  )
	{
		StringCchPrintf(datfilename,  MAX_PATH, TEXT("TextMgr\\NPCMgrU.txt") );
	}
	else if( pclClient->siServiceArea ==  ConstServiceArea_NHNChina )
	{
		StringCchPrintf(datfilename,  MAX_PATH, TEXT("TextMgr\\NPCMgrC.txt") );
	}
	else if( pclClient->siServiceArea ==  ConstServiceArea_EUROPE )
	{
		StringCchPrintf(datfilename,  MAX_PATH, TEXT("TextMgr\\NPCMgrG.txt") );
	}

	//KHY - 0809 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		
	}
	else
	{
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
		{
			bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );
		}
		else
		{
			bLoadFile = dataloader.LoadDataFromFile( datfilename );
		}
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadNpcMgr()"), TEXT("LoadError:[%s]"), datfilename);
		return;
	}

	for ( SI32 i = 0 ; i < MAX_NPCKINDFORRANKMARK ; i ++ )
	{
		StringCchCopy(m_NPCName[i], 64, TEXT(""));
		m_NPCImgIndex[i]=0;		
	}

	SI32	siindex = 0;
	TCHAR	szname[128] = {0,};
	SI32	sikind =0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siindex,	4,		
			NDATA_MBSTRING,	szname,	64,
			NDATA_INT32,	&sikind, 4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 index = 0;

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			StringCchCopy( m_NPCName[index], 64, szname );
			m_NPCImgIndex[index] = sikind;
			index++;
		}
	}



	//TCHAR buffer[1024] = TEXT("");
	//FILE* fp= NULL;

	//if( pclClient->siServiceArea ==  ConstServiceArea_Korea )	// 한국
	//{
	//	fp = _tfopen(TEXT("TextMgr\\NPCMgrK.txt"), TEXT("rt"));
	//}
	//else if( pclClient->siServiceArea ==  ConstServiceArea_English )
	//{
	//	fp = _tfopen(TEXT("TextMgr\\NPCMgrE.txt"), TEXT("rt"));
	//}
	//else if(pclClient->siServiceArea ==  ConstServiceArea_China )
	//{
	//	fp = _tfopen(TEXT("TextMgr\\NPCMgrC.txt"), TEXT("rt"));
	//}
	//else if(pclClient->siServiceArea ==  ConstServiceArea_Japan )
	//{
	//	fp = _tfopen(TEXT("TextMgr\\NPCMgrJ.txt"), TEXT("rt"));
	//}


	//if(fp == NULL)
	//{
	//	MsgBox(TEXT("fileError"), TEXT("not NPCMgrtextFile"));
	//	return ;
	//}

	//// 엔피씨 텍스트의 4줄은 주석으로 무시한다.
	//for(SI32 i = 0;i < 4;i++)
	//	_fgetts(buffer, 1024, fp);

	//for ( i = 0 ; i < MAX_NPCKINDFORRANKMARK ; i ++ )
	//{
	//	StringCchCopy(m_NPCName[i], 64, TEXT(""));
	//	m_NPCImgIndex[i]=0;		
	//}


	//i = 0 ;

	//while( _fgetts(buffer, 1024, fp) != NULL)
	//{
	//	if ( i >= MAX_NPCKINDFORRANKMARK ) break ;

	//	TCHAR * szindex;
	//	TCHAR * szname;
	//	TCHAR * szkind;

	//	szname = _tcstok(buffer, TEXT("\t"));
	//	szkind = _tcstok(NULL,TEXT("\r\n"));

	//	if ( !szname || !szkind ) break ;
	//	{

	//		szindex= _tcstok(szname,TEXT(":"));
	//		szname =_tcstok(NULL,TEXT(" ") );
	//		StringCchCopy(m_NPCName[i], 64, szname);
	//	}
	//	m_NPCImgIndex[i] = _tstoi(szkind);
	//	i ++ ;
	//}

	//fclose(fp);	
}
