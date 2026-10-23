#include "StockDistributionMgr.h"

#include "StockDistributionObj.h"

#include "StockDistributionBaseInfo.h"

#include "..\SMLib\LibList\LibList.h"

#include "..\CommonLogic\CommonLogic.h"
#include "..\Server\Server.h"
extern cltCommonLogic *pclClient;

#include "../../../DBManager/GameDBManager_World/DBMsg-Stock.h"

#define MAX_STOCKDISTRIBUTIONMGR_OBJ_NUM		128

CStockDistributionMgr::CStockDistributionMgr() : CLibListMgr()
{
	CreateMaxEmptyObj();
	Initialize();
}

CStockDistributionMgr::~CStockDistributionMgr()
{
	Destroy();
}

void CStockDistributionMgr::Initialize()
{
	return;
}

void CStockDistributionMgr::Destroy()
{
	return;
}

void CStockDistributionMgr::CreateMaxEmptyObj()
{
	CStockDistributionObj *pTemp = NULL;

	for ( int i = 0; i < MAX_STOCKDISTRIBUTIONMGR_OBJ_NUM; ++i )
	{
		pTemp = new CStockDistributionObj( i + 1 );		// 인덱스
		PushBackToEmptyList( pTemp );
	}

	return;
}

SI32 CStockDistributionMgr::Set( SI32 VillageUnique, SI32 MaxDistributionStockNum, SI32 DistributionStockUnit, GMONEY DistributionStockPrice, bool VillageWarStockDistribution, SI32 VillageWarWinVillageUnique )
{
	CStockDistributionObj *pTemp = (CStockDistributionObj *)GetExistList()->GetFirst();
		
	while( pTemp )
	{
		if ( pTemp->GetCurrentStatus() == STOCK_DISTRIBUTION_PLAY_STATUS )
		{
			if ( pTemp->GetVillageUnique() == VillageUnique && pTemp->IsVillageWarStockDistribution() == VillageWarStockDistribution )
			{
				return -2;		// 현재 그 마을의 주식 공모가 진행중
			}
		}

		pTemp = (CStockDistributionObj *)pTemp->GetNext();
	}


	pTemp = (CStockDistributionObj *)PopFrontToEmptyListAndPushBackToExistList();

	if ( pTemp == NULL )		// 개수 초과
	{
		return -1;
	}

	pTemp->Initialize();		// 기존에 쓰레기 값들을 완전 초기화 시킴.

	return pTemp->Set( VillageUnique, MaxDistributionStockNum, DistributionStockUnit, DistributionStockPrice, VillageWarStockDistribution, VillageWarWinVillageUnique );
}

void CStockDistributionMgr::Action()
{
	CStockDistributionObj *pTemp = (CStockDistributionObj *)GetExistList()->GetFirst();
	CStockDistributionObj *pTemp1 = NULL;
	
	while( pTemp )
	{
		if ( pTemp->GetCurrentStatus() == STOCK_DISTRIBUTION_PLAY_STATUS )
		{
			if ( pTemp->Action() )		// 시간 다 되서 종료
			{
				pTemp1 = pTemp;
				pTemp = (CStockDistributionObj *)pTemp->GetNext();

				pTemp1->Initialize();
				GetExistList()->Pop( pTemp1 );
				PushBackToEmptyList( pTemp1 );
			}
			else
			{
				pTemp = (CStockDistributionObj *)pTemp->GetNext();
			}
		}
		else
		{
			pTemp = (CStockDistributionObj *)pTemp->GetNext();
		}
	}
	
	return;
}

CStockDistributionObj *CStockDistributionMgr::GetStockDistributionObj( SI32 VillageUnique, bool VillageWarStockDistribution )
{
	CStockDistributionObj *pTemp = (CStockDistributionObj *)GetExistList()->GetFirst();
	
	while( pTemp )
	{
		if ( pTemp->GetVillageUnique() == VillageUnique && pTemp->IsVillageWarStockDistribution() == VillageWarStockDistribution )
		{
			return pTemp;
		}

		pTemp = (CStockDistributionObj *)pTemp->GetNext();
	}

	return NULL;
}

void CStockDistributionMgr::IncreaseDistributionStockNum( SI32 VillageUnique, SI32 IncreaseDistributionStockNum )
{
	CStockDistributionObj *pTemp = (CStockDistributionObj *)GetExistList()->GetFirst();
	CStockDistributionObj *pTemp1 = NULL;
	
	while( pTemp )
	{
		if ( pTemp->GetCurrentStatus() == STOCK_DISTRIBUTION_PLAY_STATUS )
		{
			if ( pTemp->GetVillageUnique() == VillageUnique )
			{
				if ( pTemp->IncreaseDistributionStockNum( IncreaseDistributionStockNum ) == -1 )
				{
					pTemp1 = pTemp;
					pTemp = (CStockDistributionObj *)pTemp->GetNext();

					pTemp1->Initialize();
					GetExistList()->Pop( pTemp1 );
					PushBackToEmptyList( pTemp1 );
				}
				break;
			}
		}
		
		pTemp = (CStockDistributionObj *)pTemp->GetNext();
	}
	
	return;
}

bool CStockDistributionMgr::IsAvailable( SI32 VillageUnique )
{
	CStockDistributionObj *pTemp = (CStockDistributionObj *)GetExistList()->GetFirst();	
	
	while( pTemp )
	{
		if ( pTemp->GetCurrentStatus() == STOCK_DISTRIBUTION_PLAY_STATUS )
		{
			if ( pTemp->GetVillageUnique() == VillageUnique )
			{
				return false;
			}
		}
		
		pTemp = (CStockDistributionObj *)pTemp->GetNext();
	}
	
	return true;
}

bool CStockDistributionMgr::NotVillageWarStockDistributionExist()
{
	CStockDistributionObj *pTemp = (CStockDistributionObj *)GetExistList()->GetFirst();	
	
	while( pTemp )
	{
		if ( pTemp->GetCurrentStatus() == STOCK_DISTRIBUTION_PLAY_STATUS && pTemp->IsVillageWarStockDistribution() == false )
		{
			return true;
		}
		
		pTemp = (CStockDistributionObj *)pTemp->GetNext();
	}
	
	return false;
}

void CStockDistributionMgr::SendStockDistributionNoticeMsg( SI32 CurrentFrame, _SYSTEMTIME sTime )
{
	cltServer *pclServer = (cltServer *)pclClient;
	// 금요일 , 일요일
	if ( sTime.wDayOfWeek == 5 ||sTime.wDayOfWeek == 0 )
	{
		if ( ( sTime.wHour == 17 || sTime.wHour == 20 ) && sTime.wMinute == 32 )
		{
			if ( sTime.wHour == 17 )
			{
				SI32 VillageUnique = pclClient->pclVillageManager->FindNoStockVillage();
				if ( VillageUnique > 0 )
				{
					if( GetStockDistributionObj( VillageUnique, false ) == NULL && NotVillageWarStockDistributionExist() == false )
					{
						sDBRequest_MaxStockDistributionNum clMsg( VillageUnique, 100000, 20, 5000, false, 1, 0 );
						
						pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

					}
				}
			}
			else if ( pclServer->clServerStartInfo.siStockDistributionVillageUnique > 0 )
			{
				TCHAR *VillageName = pclClient->pclVillageManager->GetName( pclServer->clServerStartInfo.siStockDistributionVillageUnique );
				
				if ( VillageName != NULL )
				{
					if( GetStockDistributionObj( pclServer->clServerStartInfo.siStockDistributionVillageUnique, false ) == NULL )
					{
						sDBRequest_MaxStockDistributionNum clMsg( pclServer->clServerStartInfo.siStockDistributionVillageUnique, 100000, 20, 5000, false, 1, 0 );
						pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}
			}
		}				
		else if ( sTime.wHour < 20 || ( sTime.wHour == 20 && sTime.wMinute < 32 ) )
		{
			SI32 VillageUnique = 0;

			if ( sTime.wHour < 17 || ( sTime.wHour == 17 && sTime.wMinute < 32 ) )
			{
				VillageUnique = pclClient->pclVillageManager->FindNoStockVillage();
			}
			else
			{
				VillageUnique = pclServer->clServerStartInfo.siStockDistributionVillageUnique;
			}
			
			if ( VillageUnique > 0 )
			{
				TCHAR *VillageName = pclClient->pclVillageManager->GetName( VillageUnique );
				
				if ( VillageName != NULL )
				{
					if ( sTime.wHour == 20 || sTime.wHour == 17 )
					{
						if( sTime.wMinute >= 30 && sTime.wMinute < 32 &&( CurrentFrame % 20 == 0 ) )
						{
							SendStockDistributionExplainMsg( CurrentFrame, sTime );
						}						
						else if( CurrentFrame % ( 1200 * 2 ) == 0 )		// 2분 간격
						{
							if ( sTime.wHour == 17 && sTime.wMinute < 30 )
							{
								TCHAR NoticeText[ 1024 ] = TEXT("");
								TCHAR* pText = GetTxtFromMgr(3613);
								StringCchPrintf( NoticeText, 1024, pText, VillageName );
								pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
							}
							else
							{
								TCHAR NoticeText[ 1024 ] = TEXT("");
								TCHAR* pText = GetTxtFromMgr(3614);
								StringCchPrintf( NoticeText, 1024, pText, VillageName );
								pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
							}
						}
					}
					else if( CurrentFrame % ( 1200 * 5 ) == 0 )		// 5분 간격
					{
						if ( sTime.wHour < 17 )
						{
							TCHAR NoticeText[ 1024 ] = TEXT("");
							TCHAR* pText = GetTxtFromMgr(3615);
							StringCchPrintf( NoticeText, 1024, pText, VillageName );
							pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
						}
						else
						{
							TCHAR NoticeText[ 1024 ] = TEXT("");
							TCHAR* pText = GetTxtFromMgr(3616);
							StringCchPrintf( NoticeText, 1024, pText, VillageName );
							pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
						}
					}
				}
			}
		}
/*		else if ( sTime.wHour > 21 || ( sTime.wHour == 21 && sTime.wMinute > 32 ) )
		{
			if ( pclServer->clServerStartInfo.siStockDistributionVillageUnique > 0 )
			{
				TCHAR *VillageName = pclClient->pclVillageManager->GetName( pclServer->clServerStartInfo.siStockDistributionVillageUnique );
				
				if ( VillageName != NULL )
				{
					if( CurrentFrame % ( 1200 * 5 ) == 0 )		// 5분 간격
					{
						TCHAR NoticeText[ 1024 ] = TEXT("");
						TCHAR* pText = GetTxtFromMgr(3617);
						sprintf( NoticeText, pText, VillageName );
						pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
					}
				}
			}
		}*/
	}
/*	// 화요일, 목요일, 일요일
	else if ( sTime.wDayOfWeek == 2 || sTime.wDayOfWeek == 4 || sTime.wDayOfWeek == 0 )
	{
		if ( pclServer->clServerStartInfo.siStockDistributionVillageUnique > 0 )
		{
			TCHAR *VillageName = pclClient->pclVillageManager->GetName( pclServer->clServerStartInfo.siStockDistributionVillageUnique );
			
			if ( VillageName != NULL )
			{
				if ( ( sTime.wHour == 18 || sTime.wHour == 21 ) && sTime.wMinute == 32 )
				{
					if ( sTime.wHour == 18 )
					{
						if( GetStockDistributionObj( pclServer->clServerStartInfo.siStockDistributionVillageUnique, false ) == NULL )
						{
							sDBRequest_MaxStockDistributionNum clMsg( pclServer->clServerStartInfo.siStockDistributionVillageUnique, 100000, 10, 5000, false, 1, 0 );
							pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
						}
					}
					else
					{
						if( GetStockDistributionObj( pclServer->clServerStartInfo.siStockDistributionVillageUnique, false ) == NULL )
						{
							sDBRequest_MaxStockDistributionNum clMsg( pclServer->clServerStartInfo.siStockDistributionVillageUnique, 100000, 10, 5000, false, 0, 0 );
							pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
						}
					}
				}
				else if ( sTime.wHour < 21 || ( sTime.wHour == 21 && sTime.wMinute < 32 ) )
				{
					if ( sTime.wHour == 21 || sTime.wHour == 18 )
					{
						if( sTime.wMinute >= 30 && sTime.wMinute < 32 && ( CurrentFrame % 20 == 0 ) )
						{
							SendStockDistributionExplainMsg( CurrentFrame, sTime );
						}
						else if( CurrentFrame % ( 1200 * 2 ) == 0 )		// 2분 간격
						{
							if ( sTime.wHour == 18 && sTime.wMinute < 30 )
							{
								TCHAR NoticeText[ 1024 ] = TEXT("");
								TCHAR* pText = GetTxtFromMgr(3618);
								sprintf( NoticeText, pText, VillageName );
								pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
							}
							else
							{
								TCHAR NoticeText[ 1024 ] = TEXT("");
								TCHAR* pText = GetTxtFromMgr(3619);
								sprintf( NoticeText, pText, VillageName );
								pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
							}
						}
					}
					else if( CurrentFrame % ( 1200 * 5 ) == 0 )		// 5분 간격
					{
						if ( sTime.wHour < 18 )
						{
							TCHAR NoticeText[ 1024 ] = TEXT("");
							TCHAR* pText = GetTxtFromMgr(3620);
							sprintf( NoticeText, pText, VillageName );
							pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
						}
						else
						{
							TCHAR NoticeText[ 1024 ] = TEXT("");
							TCHAR* pText = GetTxtFromMgr(3621);
							sprintf( NoticeText, pText, VillageName );
							pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
						}
					}
				}
			}
		}
	}*/
	
	return;
}

void CStockDistributionMgr::SendStockDistributionExplainMsg( SI32 CurrentFrame, _SYSTEMTIME sTime )
{
	cltServer *pclServer = (cltServer *)pclClient;
	TCHAR* pText = GetTxtFromMgr(3622);

	if ( sTime.wMinute == 30 )
	{
		TCHAR NoticeText[ 1024 ] = TEXT("");
								
		switch ( sTime.wSecond )
		{
		case 0:
			StringCchPrintf( NoticeText, 1024, pText );
			pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
			break;
		case 15:
			pText = GetTxtFromMgr(3623);
			StringCchPrintf( NoticeText, 1024, pText );
			pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
			break;
		case 30:
			pText = GetTxtFromMgr(3624);
			StringCchPrintf( NoticeText, 1024, pText );
			pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
			break;
		case 45:
			pText = GetTxtFromMgr(3625);
			StringCchPrintf( NoticeText, 1024, pText );
			pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
			break;
		}
	}
	else if ( sTime.wMinute == 31 )
	{
		TCHAR NoticeText[ 1024 ] = TEXT("");
		
		switch ( sTime.wSecond )
		{
		case 0:
			pText = GetTxtFromMgr(3626);
			StringCchPrintf( NoticeText, 1024, pText );
			pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
			break;
		case 15:
			pText = GetTxtFromMgr(3627);
			StringCchPrintf( NoticeText, 1024, pText );
			pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
			break;
		case 30:
			pText = GetTxtFromMgr(3628);
			StringCchPrintf( NoticeText, 1024, pText );
			pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
			break;
		case 45:
			pText = GetTxtFromMgr(3629);
			StringCchPrintf( NoticeText, 1024, pText );
			pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
			pText = GetTxtFromMgr(3630);
			StringCchPrintf( NoticeText, 1024, pText );
			pclServer->SendNoticeMsg(-1, 0, 0, NoticeText );
			break;
		}
	}
	
	return;
}