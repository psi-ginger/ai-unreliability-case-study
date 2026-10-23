//---------------------------------
// 2004/11/5 김태곤
//---------------------------------

//----------------------------------
// Common
//----------------------------------
#include "../Common/CommonHeader.h"
#include "Char\CharManager\CharManager.h"

//---------------------------------
// Server
//---------------------------------
#include "../Server/Server.h"

#include "../Server/Minister/Minister-Goonzu.h"

//---------------------------------
// CommonLogic
//---------------------------------
#include "MsgType-System.h"

//---------------------------------
// DBManager
//---------------------------------
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"


// 제조 보너스 타임 
void cltServer::MakeBonusTimeAction()
{
	static DWORD dwLastClock = 0;

	TCHAR buffer[256] = TEXT("");
	TCHAR szservicetimetxt[128] = TEXT("");

	bool bevent = false;

	if(bGMBonusTimeSwitch == 0)
	{
		if (GetFrame() % 3600)return ;
	}

	SI16 goonzuMakeBonusTime = -1;

	CMinisterGoonzu *pclministerGoonzu = (CMinisterGoonzu *)pclMinisterMgr->GetMinister(MINISTER_GOONZU);
	if ( pclministerGoonzu && 
		_tcslen(pclministerGoonzu->m_szAccountID) > 0 &&
		_tcslen(pclministerGoonzu->m_szPersonName) > 0 ) 
	{
		goonzuMakeBonusTime = pclministerGoonzu->m_siMakeBonusTime;
	}


	// 보너스 시간. 
	if (pclClient->siServiceArea == ConstServiceArea_English)
	{
		if( bDoNotAutoBonus == false)
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if(sTime.wHour == 0|| sTime.wHour == 10 || sTime.wHour == 16 || sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;

					if (sTime.wHour == 16)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 16:00~17:00)"));
					}
					else if (sTime.wHour == 0)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 00:00~01:00)"));
					}
					else if (sTime.wHour == 10)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 10:00~11:00)"));
					}
					else if ( sTime.wHour == goonzuMakeBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6927);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}
		}
		else
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if( sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;


					TCHAR * pText = GetTxtFromMgr(6927);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);

				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}

		}

	}
	else if (pclClient->siServiceArea == ConstServiceArea_USA)
	{
		if( bDoNotAutoBonus == false)
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if ( sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6927);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
				}


				if( sTime.wDayOfWeek == 0 )
				{
					SYSTEMTIME	start, end;
					ZeroMemory(&start, sizeof(start));
					ZeroMemory(&end, sizeof(end));

					start.wYear = 2008;		start.wMonth = 7;		start.wDay = 16;
					end.wYear	= 3008;		end.wMonth   = 9;		end.wDay   = 17;	end.wHour = 23;

					// 토요일 하루 종일 이벤트
					if( TimeInBounds(&sTime, &start, &end) == true)
					{
						// 하루종일 이벤트.
						bAlldayBonus = true;

						bevent = true;

						TCHAR * pText = GetTxtFromMgr(8414);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
					}
				}


				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);
					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);
				}
				else
				{
					// 하루종일 이벤트.
					bAlldayBonus = false;

					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);
					}
				}
			}
		}
		else
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if( sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6927);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
				}

				
				//[진성] 일요일은 하루종일 제조 경험치 보너스 적용.
				if( sTime.wDayOfWeek == 0 )
				{
					SYSTEMTIME	start, end;
					ZeroMemory(&start, sizeof(start));
					ZeroMemory(&end, sizeof(end));

					start.wYear = 2008;		start.wMonth = 7;		start.wDay = 16;
					end.wYear	= 2008;		end.wMonth   = 9;		end.wDay   = 17;	end.wHour = 23;

					// 토요일 하루 종일 이벤트
					if( TimeInBounds(&sTime, &start, &end) == true)
					{
						// 하루종일 이벤트.
						bAlldayBonus = true;

						bevent = true;

						TCHAR * pText = GetTxtFromMgr(8414);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);
					}
					
					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					bAlldayBonus = false;
					
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
	}
	else if (pclClient->siServiceArea == ConstServiceArea_Japan)
	{
		if( bDoNotAutoBonus == false)
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if(sTime.wHour == 15 || sTime.wHour == 22 || sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;

					if (sTime.wHour == 15)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(15:00~16:00)"));
					}
					else if (sTime.wHour == 22)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(22:00~23:00)"));
					}
					else if ( sTime.wHour == goonzuMakeBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6927);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}
		}
		else
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if( sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;


					TCHAR * pText = GetTxtFromMgr(6927);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);

				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}
		}
	}
	//KHY - 0620 - 군주S 제조 경험치 보너스시간 조정. 다음에 이벤트시간 수정할때는 시간값을 디파인 해놓자.
	else if (pclClient->siServiceArea == ConstServiceArea_Korea)
	{
		if( bDoNotAutoBonus == false)
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				// 이벤트 시작 2007.08.10 - by LEEKH	//KHY -0814 -  20시 부터 두시간(sTime.wHour >= 20 && sTime.wHour <= 22)
				if ( ((sTime.wHour >= 20 && sTime.wHour < 22) || sTime.wHour == goonzuMakeBonusTime 
					|| pclClient->pclEventTimeManager->InEventTime( TEXT("NewYear_ManufactureBonusTime"), &sTime ))
					// 설날 이벤트 기간동안은 다른 보너스 타임은 발동하지 않고 오로지 설날 보너스 타임만 적용되도록 변경
					&& (pclClient->pclEventTimeManager->InEventTime( TEXT("NewYear_AttackBonusTime"), &sTime ) == false)
				)
				//if(sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;

					if ( pclClient->pclEventTimeManager->InEventTime( "NewYear_ManufactureBonusTime", &sTime ) == true )
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(00:00~23:59)"));
					}
					else if (sTime.wHour >= 20 && sTime.wHour < 22)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(20:00~22:00)"));
					}
					else if ( sTime.wHour == goonzuMakeBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6927);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}
		}
		else
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if( sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6927);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);

				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}

		}

	}

	// 대만 제조 경험치 보너스 타임 추가 (PCK - 07.12.14)
	else if (pclClient->siServiceArea == ConstServiceArea_Taiwan)
	{
		if( bDoNotAutoBonus == false)
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				// [설날 이벤트 2008.01.10 - 김영훈 추가 : 설날 이벤트 타임 설정]
				if ( pclClient->pclEventTimeManager->InEventTime( "NewYear_ManufactureBonusTime", &sTime ) == true )
				{
					bevent = true;

					StringCchCopy(szservicetimetxt, 128, TEXT("(00:00~23:59)"));
				}

				if( sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;

					if ( sTime.wHour == goonzuMakeBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6927);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}
		}
		else
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if( sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;


					TCHAR * pText = GetTxtFromMgr(6927);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);

				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}
		}
	}

	// NHN CHINA 경험치 보너스 타임 추가 
	else if (pclClient->siServiceArea == ConstServiceArea_NHNChina)
	{
		if( bDoNotAutoBonus == false)
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if(sTime.wHour == 0|| sTime.wHour == 10 || sTime.wHour == 16 || sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;

					if (sTime.wHour == 16)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 16:00~17:00)"));
					}
					else if (sTime.wHour == 0)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 00:00~01:00)"));
					}
					else if (sTime.wHour == 10)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 10:00~11:00)"));
					}
					else if ( sTime.wHour == goonzuMakeBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6927);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}
		}
		else
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if( sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;


					TCHAR * pText = GetTxtFromMgr(6927);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);

				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}

		}

	}
	else if (pclClient->siServiceArea == ConstServiceArea_EUROPE)
	{
		if( bDoNotAutoBonus == false)
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if(sTime.wHour == 0|| sTime.wHour == 10 || sTime.wHour == 16 || sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;

					if (sTime.wHour == 16)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 16:00~17:00)"));
					}
					else if (sTime.wHour == 0)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 00:00~01:00)"));
					}
					else if (sTime.wHour == 10)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 10:00~11:00)"));
					}
					else if ( sTime.wHour == goonzuMakeBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6927);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}
		}
		else
		{
			if( bIsGMBonusTime2 == false && bGMBonusTimeSwitch == 0 )
			{
				if( sTime.wHour == goonzuMakeBonusTime )
				{
					bevent = true;


					TCHAR * pText = GetTxtFromMgr(6927);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuMakeBonusTime);

				}

				if(bevent)
				{
					if(bBonus2TimeSwitch == false)
					{
						bBonus2TimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR * pText = GetTxtFromMgr(5384);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);

				}
				else
				{
					if(bBonus2TimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR * pText = GetTxtFromMgr(5385);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonus2TimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}

			}

		}

	}

	if (bGMBonusTimeSwitch == 2)
	{
		//제조 경험치
		if(bBonus2TimeSwitch)
		{
			cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
			cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

			/*
			// 전체 유저에게 알림 메시지를 보낸다. 
			if( siGMBonusTime2Percent > 0 )
			{
			TCHAR * pText = GetTxtFromMgr(6972);
			TCHAR szBuffer[256];
			szBuffer[0] = 0;

			StringCchPrintf( szBuffer , 256 , pText , siGMBonusTime2Percent + 100 );

			SendNoticeMsg( -1, 0, 0, szBuffer );

			}
			else
			{
			TCHAR * pText = GetTxtFromMgr(5384);

			SendNoticeMsg( -1, 0, 0, pText );
			}*/

			bIsGMBonusTime2 = true;
		}
		else
		{
			// 전체 유저에게 알림 메시지를 보낸다. 
			TCHAR* pText = GetTxtFromMgr(5385);
			sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
			SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
			SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

			cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
			cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);
			bIsGMBonusTime2 = false;
		}

		bGMBonusTimeSwitch = 0;
	}


	if( bIsGMBonusTime2 ) // GM보너스 타임임을 유저들에게 메시지로 광고한다
	{

		if( siGMBonusTime2Percent > 0 )
		{
			TCHAR * pText = GetTxtFromMgr(6972);
			TCHAR szBuffer[256];
			szBuffer[0] = 0;

			StringCchPrintf( szBuffer , 256 , pText , siGMBonusTime2Percent + 100 );

			SendNoticeMsg( -1, 0, 0, szBuffer );

		}
		else
		{
			TCHAR * pText = GetTxtFromMgr(5384);
			sprintf(buffer,pText);

			SendNoticeMsg( -1, 0, 0, buffer );
		}

	}

	/*
	// 보너스 시간. 
	if(sTime.wHour == 19)
	{
	bevent = true;
	}

	// 군주오픈기념일에는 
	if(bDoubleBonusDaySwitch == true)
	{
	if(sTime.wHour == 20)
	{
	bevent	= true;
	}
	}

	// 서비스 타임 문장 확보. 
	if(bDoubleBonusDaySwitch)
	{
	if ( siServiceArea == ConstServiceArea_Korea )
	strcpy(szservicetimetxt, TEXT("(오후 7시~ 9시)"));
	else if(siServiceArea == ConstServiceArea_China)
	strcpy(szservicetimetxt, TEXT("(PM 7Hour~ 9Hour)"));
	else if(siServiceArea == ConstServiceArea_English)
	strcpy(szservicetimetxt, TEXT("(PM 7Hour~ 9Hour)"));
	}
	else
	{
	if ( siServiceArea == ConstServiceArea_Korea )
	strcpy(szservicetimetxt, TEXT("(오후 7시~ 8시)"));
	else if(siServiceArea == ConstServiceArea_China)
	strcpy(szservicetimetxt, TEXT("(PM 7Hour~ 8Hour)"));
	else if(siServiceArea == ConstServiceArea_English)
	strcpy(szservicetimetxt, TEXT("(PM 7Hour~ 8Hour)"));
	}

	if(bevent)
	{
	if(bBonus2TimeSwitch == false)
	{
	bBonus2TimeSwitch = true;

	cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
	cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

	}

	// 전체 유저에게 알림 메시지를 보낸다. 
	TCHAR * pText = GetTxtFromMgr(5384);
	SendNoticeMsg(-1, 0, 0, pText);
	SendNoticeMsg(-1, 0, 0, szservicetimetxt);

	}
	else
	{
	if(bBonus2TimeSwitch == true)
	{
	// 전체 유저에게 알림 메시지를 보낸다. 
	TCHAR * pText = GetTxtFromMgr(5385);
	SendNoticeMsg(-1, 0, 0, pText);
	SendNoticeMsg(-1, 0, 0, szservicetimetxt);

	bBonus2TimeSwitch = false;

	cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_MAKE,  bBonus2TimeSwitch);
	cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

	}
	}
	*/	
}


// 전투 보너스 타임 액션.
void cltServer::BattleBonusTimeAction()
{
	static DWORD dwLastClock = 0;

	if(bGMBonusTimeSwitch == 0)
	{
		if (GetFrame() % 3600)return ;
	}

	TCHAR buffer[256];
	TCHAR szservicetimetxt[128] = TEXT("");


	bool bevent = false;


	SI16 goonzuBonusTime = -1;

	CMinisterGoonzu *pclministerGoonzu = (CMinisterGoonzu *)pclMinisterMgr->GetMinister(MINISTER_GOONZU);
	if ( pclministerGoonzu && 
		_tcslen(pclministerGoonzu->m_szAccountID) > 0 &&
		_tcslen(pclministerGoonzu->m_szPersonName) > 0 ) 
	{
		goonzuBonusTime = pclministerGoonzu->m_siBonusTime;
	}


	// 보너스 시간. 
	if (pclClient->siServiceArea == ConstServiceArea_English )
	{
		if( bDoNotAutoBonus == false)
		{

			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{
				if(sTime.wHour == 9|| sTime.wHour == 15 || sTime.wHour == 23 || sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					if (sTime.wHour == 15)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 15:00~16:00)"));
					}
					else if (sTime.wHour == 23)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 23:00~00:00)"));
					}
					else if (sTime.wHour == 9)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 09:00~10:00)"));
					}
					else if(sTime.wHour == goonzuBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6925);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
		else
		{
			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{
				if( sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6925);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);

				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
	}
	else if (pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		if( bDoNotAutoBonus == false)
		{

			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{ 
				if(sTime.wHour == 14|| sTime.wHour == 21 || sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					if (sTime.wHour == 14)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(14:00~15:00)"));
					}
					else if (sTime.wHour == 21)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(21:00~22:00)"));
					}
					else if ( sTime.wHour == goonzuBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6925);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);
					}

				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]


						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
		else
		{

			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{ 
				if( sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6925);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);
				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]


						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
	}
	//KHY - 0620 - 군주S 보너스 시간 조정.
	else if (pclClient->siServiceArea == ConstServiceArea_Korea )
	{
		if( bDoNotAutoBonus == false)
		{
			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{ 
				// 이벤트 시작 - 2007-08-10 by LEEKH
				if ( ((sTime.wHour >= 20 && sTime.wHour < 22)||sTime.wHour == goonzuBonusTime 
					|| pclClient->pclEventTimeManager->InEventTime( TEXT("NewYear_AttackBonusTime"), &sTime ))
					// 설날 이벤트 기간동안은 다른 보너스 타임은 발동하지 않고 오로지 설날 보너스 타임만 적용되도록 변경
					&& (pclClient->pclEventTimeManager->InEventTime( TEXT("NewYear_ManufactureBonusTime"), &sTime ) == false)
				)
				//if(sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					if ( pclClient->pclEventTimeManager->InEventTime( TEXT("NewYear_AttackBonusTime"), &sTime ) == true )
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(00:00~23:59)"));
					}
					else if (sTime.wHour >= 20 && sTime.wHour < 22)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(20:00~22:00)"));
					}
					else if ( sTime.wHour == goonzuBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6925);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);
					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);
				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다.
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]


						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);
					}
				}
			}
		}
		else
		{
			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{ 
				if( sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6925);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);
				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);
				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]


						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
	}
	// 미국용 이벤트 타임
	else if (pclClient->siServiceArea == ConstServiceArea_USA )
	{
		if( bDoNotAutoBonus == false)
		{
			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{
				SYSTEMTIME	start, end;
				ZeroMemory(&start, sizeof(start));
				ZeroMemory(&end, sizeof(end));

				start.wYear = 2008;		start.wMonth = 7;		start.wDay = 16;
				end.wYear	= 3008;		end.wMonth   = 9;		end.wDay   = 17;	end.wHour = 23;

				// 토요일 하루 종일 이벤트
				if( TimeInBounds(&sTime, &start, &end) == true && sTime.wDayOfWeek == 6 )
				{
					bevent = true;

					StringCchCopy(szservicetimetxt, 128, TEXT("(For a day)"));

					// 전체 유저에게 알림 메시지를 보낸다. 
					//[진성] 중복 2회 출력으로 아래에서 출력하므로 여기는 삭제~ => 2008-5-7
					/*
						TCHAR* pText = GetTxtFromMgr(7971);
						sprintf(buffer, pText);
						SendNoticeMsg(-1, 0, 0, buffer);
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);
					*/
				}
				else if ( sTime.wHour == goonzuBonusTime )
				{
					bevent = true;
					bBonusTimeGoonzu = true;
				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);
					}
					
					if( sTime.wDayOfWeek == 6 )
					{
						// 하루종일 이벤트.
						bAlldayBonus = true;

						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(7971);
						sprintf(buffer,pText);					//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);		//%가 나오지 않도록 추가. [2007.07.05 성웅]
					}
					else if(true == bBonusTimeGoonzu)
					{
						TCHAR * pText = GetTxtFromMgr(6925);
						StringCchPrintf(buffer, 128 , pText , goonzuBonusTime);
						SendNoticeMsg(-1, 0, 0, buffer);
					}
				}
				else
				{
					//if( sTime.wDayOfWeek != 6 )
					//{
					//	// 전체 유저에게 알림 메시지를 보낸다. 
					//	TCHAR* pText = GetTxtFromMgr(7972);
					//	sprintf(buffer,pText);					//%가 나오지 않도록 추가. [2007.07.05 성웅]
					//	SendNoticeMsg(-1, 0, 0, buffer);		//%가 나오지 않도록 추가. [2007.07.05 성웅]
					//}

					// 군주 보너스 타임인데 종료를 하면 메시지를 보낸다.
					if(true == bBonusTimeGoonzu)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer, pText);				//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);	//%가 나오지 않도록 추가. [2007.07.05 성웅]
					}

					
					if(bBonusTimeSwitch == true)
					{
						// 하루종일 이벤트.
						bAlldayBonus = false;

						bBonusTimeSwitch = false;
						bBonusTimeGoonzu = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
		else
		{
			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{
				if( sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6925);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);

				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonusTimeSwitch = false;
						bBonusTimeGoonzu = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
	}
	// 대만 전투보너스 타임 시간 (PCK - 07.12.14)
	else if (pclClient->siServiceArea == ConstServiceArea_Taiwan )
	{
		if( bDoNotAutoBonus == false)
		{

			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{ 
				// [설날 이벤트 2008.01.10 - 김영훈 추가 : 설날 이벤트 타임 설정]
				if ( pclClient->pclEventTimeManager->InEventTime( TEXT("NewYear_AttackBonusTime"), &sTime ) == true )
				{
					bevent = true;

					StringCchCopy(szservicetimetxt, 128, TEXT("(00:00~23:59)"));
				}

				if( sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					if ( sTime.wHour == goonzuBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6925);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]


						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
		else
		{

			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{ 
				if( sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6925);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);
				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]


						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
	}

	// NHNCHINA 전투 보너스 타임 시간.
	else if (pclClient->siServiceArea == ConstServiceArea_NHNChina )
	{
		if( bDoNotAutoBonus == false)
		{

			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{
				if(sTime.wHour == 9|| sTime.wHour == 15 || sTime.wHour == 23 || sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					if (sTime.wHour == 15)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 15:00~16:00)"));
					}
					else if (sTime.wHour == 23)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 23:00~00:00)"));
					}
					else if (sTime.wHour == 9)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 09:00~10:00)"));
					}
					else if(sTime.wHour == goonzuBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6925);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
		else
		{
			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{
				if( sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6925);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);

				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
	}
	else if (pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		if( bDoNotAutoBonus == false)
		{

			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{
				if(sTime.wHour == 9|| sTime.wHour == 15 || sTime.wHour == 23 || sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					if (sTime.wHour == 15)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 15:00~16:00)"));
					}
					else if (sTime.wHour == 23)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 23:00~00:00)"));
					}
					else if (sTime.wHour == 9)
					{
						StringCchCopy(szservicetimetxt, 128, TEXT("(PST 09:00~10:00)"));
					}
					else if(sTime.wHour == goonzuBonusTime )
					{
						TCHAR * pText = GetTxtFromMgr(6925);
						StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);
					}
				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
		else
		{
			if ( bIsGMBonusTime == false && bGMBonusTimeSwitch == 0)
			{
				if( sTime.wHour == goonzuBonusTime )
				{
					bevent = true;

					TCHAR * pText = GetTxtFromMgr(6925);
					StringCchPrintf(szservicetimetxt , 128 , pText , goonzuBonusTime);

				}

				if(bevent)
				{
					if(bBonusTimeSwitch == false)
					{
						bBonusTimeSwitch = true;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}

					// 전체 유저에게 알림 메시지를 보낸다. 
					TCHAR* pText = GetTxtFromMgr(3783);
					sprintf(buffer,pText);
					SendNoticeMsg(-1, 0, 0, buffer);
					SendNoticeMsg(-1, 0, 0, szservicetimetxt);


				}
				else
				{
					if(bBonusTimeSwitch == true)
					{
						// 전체 유저에게 알림 메시지를 보낸다. 
						TCHAR* pText = GetTxtFromMgr(3784);
						sprintf(buffer,pText);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, buffer);//%가 나오지 않도록 추가. [2007.07.05 성웅]
						SendNoticeMsg(-1, 0, 0, szservicetimetxt);//%가 나오지 않도록 추가. [2007.07.05 성웅]

						bBonusTimeSwitch = false;

						cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
						cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

					}
				}
			}
		}
	}

	if (bGMBonusTimeSwitch == 1)
	{
		if(bBonusTimeSwitch)
		{
			cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
			cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);


			// 전체 유저에게 알림 메시지를 보낸다. 
			/*
			if( siGMBonusTimePercent > 0 )
			{
			TCHAR * pText = GetTxtFromMgr(6971);
			TCHAR szBuffer[256];
			szBuffer[0] = 0;

			StringCchPrintf( szBuffer , 256 , pText , siGMBonusTimePercent + 100 );  // 30% 증가일경우 130% 로 표시해준다

			SendNoticeMsg( -1, 0, 0, szBuffer );

			}
			else
			{
			TCHAR * pText = GetTxtFromMgr(3783);

			SendNoticeMsg( -1, 0, 0, pText );
			}
			*/

			bIsGMBonusTime = true;
		}
		else
		{
			// 전체 유저에게 알림 메시지를 보낸다. 
			TCHAR* pText = GetTxtFromMgr(3784);
			SendNoticeMsg(-1, 0, 0, pText);
			//SendNoticeMsg(-1, 0, 0, szservicetimetxt);

			cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
			cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

			bIsGMBonusTime = false;
		}

		bGMBonusTimeSwitch = 0;
	}


	if( bIsGMBonusTime ) // GM보너스 타임임을 유저들에게 메시지로 광고한다
	{

		if( siGMBonusTimePercent > 0 )
		{
			TCHAR * pText = GetTxtFromMgr(6971);
			TCHAR szBuffer[256];
			szBuffer[0] = 0;

			StringCchPrintf( szBuffer , 256 , pText , siGMBonusTimePercent + 100 );  // 30% 증가일경우 130% 로 표시해준다

			SendNoticeMsg( -1, 0, 0, szBuffer );

		}
		else
		{
			TCHAR * pText = GetTxtFromMgr(3783);
			sprintf(buffer, pText);

			SendNoticeMsg( -1, 0, 0, buffer );
		}

	}

	/*
	// 보너스 시간. 
	if(sTime.wHour == 18 || sTime.wHour == 22 )//|| sTime.wHour == 1)
	{
	bevent = true;
	}

	// 군주오픈기념일이랑 어린이날에는
	if(bDoubleBonusDaySwitch || bSpecailDay8switch )
	{
	if(sTime.wHour == 17 || sTime.wHour == 21 )//|| sTime.wHour == 2 )
	{
	bevent	= true;
	}
	}

	// 서비스 타임 문장 확보. 
	if(bDoubleBonusDaySwitch || bSpecailDay8switch )
	{
	if ( siServiceArea == ConstServiceArea_Korea ) 
	strcpy(szservicetimetxt, TEXT("(오후 5시~ 7시, 밤 9시 ~ 11시)"));//, 새벽 1시 ~ 3시)");
	else if ( siServiceArea == ConstServiceArea_China)
	strcpy(szservicetimetxt, TEXT("(PM 5Hour~ 7Hour, PM 9Hour ~ 11Hour)"));//, AM 1Hour ~ 3Hour)");
	else if ( siServiceArea == ConstServiceArea_English)
	strcpy(szservicetimetxt, TEXT("(PM 5Hour~ 7Hour, PM 9Hour ~ 11Hour)"));//, AM 1Hour ~ 3Hour)");
	}
	else
	{
	if ( siServiceArea == ConstServiceArea_Korea )
	strcpy(szservicetimetxt, TEXT("(오후 6시~ 7시, 밤 10시 ~ 11시)"));//, 새벽 1시 ~ 2시)");
	else if( siServiceArea == ConstServiceArea_China)
	strcpy(szservicetimetxt, TEXT("(PM 6Hour~ 7Hour, PM 10Hour ~ 11Hour)"));//, AM 1Hour ~ 2Hour)");
	else if( siServiceArea == ConstServiceArea_English)
	strcpy(szservicetimetxt, TEXT("(PM 6Hour~ 7Hour, PM 10Hour ~ 11Hour)"));//, AM 1Hour ~ 2Hour)");
	}

	if(bevent)
	{
	if(bBonusTimeSwitch == false)
	{
	bBonusTimeSwitch = true;

	cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
	cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

	}

	// 전체 유저에게 알림 메시지를 보낸다. 
	TCHAR* pText = GetTxtFromMgr(3783);
	sprintf(buffer,pText);
	SendNoticeMsg(-1, 0, 0, buffer);
	SendNoticeMsg(-1, 0, 0, szservicetimetxt);


	}
	else
	{
	if(bBonusTimeSwitch == true)
	{
	// 전체 유저에게 알림 메시지를 보낸다. 
	TCHAR* pText = GetTxtFromMgr(3784);
	SendNoticeMsg(-1, 0, 0, pText);
	SendNoticeMsg(-1, 0, 0, szservicetimetxt);

	bBonusTimeSwitch = false;

	cltGameMsgResponse_BonusTime clinfo(BONUSTIMEMODE_NORMAL,  bBonusTimeSwitch);
	cltMsg clMsg(GAMEMSG_RESPONSE_BONUSTIME, 	sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->SendAllPersonMsg((sPacketHeader *)&clMsg);

	}
	}
	*/	
}

//[추가 : 황진성 2007. 11. 1 이전에 있던 요술램프 코드를 참고하여 확장가능하게 수정함]
void cltServer::SpecialQuestAction()
{
	class MagicLamp
	{	
		private:
		public:
			bool	m_bGiveSwitch;			// 주냐 마냐.
			WORD	m_wDay;					// 일.
			WORD	m_wHour;				// 시.
			WORD	m_wMinute;				// 분.
			WORD	m_wSecond;				// 초.
			WORD	m_wUserLevel;			// 유저 제한 레벨.
		
			MagicLamp(BYTE _Day = -1, BYTE _Hour = -1, BYTE _Minute = -1, BYTE	_Second = -1, BYTE _UserLevel = -1, bool _GiveSwitch = FALSE)
			{
				m_bGiveSwitch = _GiveSwitch;
				m_wDay		  = _Day;
				m_wHour		  = _Hour;
				m_wMinute	  = _Minute;
				m_wSecond	  = _Second;
				m_wUserLevel  = _UserLevel;
			}

			void Set(BYTE _Day, BYTE _Hour, BYTE _Minute,  BYTE	_Second, BYTE _UserLevel, bool _GiveSwitch = TRUE)
			{
				m_bGiveSwitch = _GiveSwitch;
				m_wDay		  = _Day;
				m_wHour		  = _Hour;
				m_wMinute	  = _Minute;
				m_wSecond	  = _Second;
				m_wUserLevel  = _UserLevel;
			}	
	};

	static const SI32   LampSize = 10;
	static MagicLamp	Lamp[LampSize] = {MagicLamp(), };
	
	// 초기화 날짜.
	static WORD	Day = 0;

	static _SYSTEMTIME	giveTime;
	
	DiffTimeSecond( &sTime, &giveTime );
	
	//--------------------------------------------------------------------------------
	// 해당 나라에 시간만 정해주면 요술램프가 지급됩니다.
	// 사용하지 않는 나라는 해당 국가 시간넣는 부분에 return을 넣어주세요. 
	// 시간은 개수는 10개 까지 가능합니다.
	//--------------------------------------------------------------------------------

	if(Day != sTime.wDay)
	{
		Day = sTime.wDay;

		if ( IsWhereServiceArea( ConstServiceArea_China ) )
		{
			Lamp[0].Set(0, 20, 30, 0, 30);
		}
		else if ( IsWhereServiceArea( ConstServiceArea_Korea ) )
		{
			Lamp[0].Set(0, 19, 0, 0, 30);//19시 30렙 이상 요술램프 준다.[08.01.15]
		}
		else if ( IsWhereServiceArea( ConstServiceArea_NHNChina ) )
		{
			Lamp[0].Set(0, 19, 0, 0, 30);//19시 30렙 이상 요술램프 준다
		}
		else if ( IsWhereServiceArea( ConstServiceArea_English ) )
		{
			Lamp[0].Set(0,  3, 0, 0, 30);
			Lamp[1].Set(0,  6, 0, 0, 30);
			Lamp[2].Set(0, 15, 0, 0, 30);
			Lamp[3].Set(0, 18, 0, 0, 30);
		}
		else if ( IsWhereServiceArea( ConstServiceArea_USA ) )
		{
			Lamp[0].Set(0,  0, 0, 0, 30);
			Lamp[1].Set(0,  3, 0, 0, 30);
			Lamp[2].Set(0,  6, 0, 0, 30);
			Lamp[3].Set(0,  9, 0, 0, 30);
			Lamp[4].Set(0, 12, 0, 0, 30);
			Lamp[5].Set(0, 15, 0, 0, 30);
			Lamp[6].Set(0, 18, 0, 0, 30);
			Lamp[7].Set(0, 21, 0, 0, 30);
		}
		// 대만 요술램프 지급 기간 설정 (매일저녁 8시에 20레벨부터)
		else if ( IsWhereServiceArea( ConstServiceArea_Taiwan ) )
		{
			Lamp[0].Set(0,  20, 0, 0, 20);
		}
		// 일본 매일 한번 저녁 6시에 배포한다
		else if ( IsWhereServiceArea( ConstServiceArea_Japan ) )
		{
			// 서버 이벤트 시간에만 배포하도록한다
			if ( IsCountrySwitch(Switch_Server_Event) && pclEventTimeManager->InEventTime(TEXT("GoonzuDay_Week"), &sTime) )
			{
				Lamp[0].Set(0,  18, 0, 0, 30);
			}

			// 요술램프 이벤트 기간에만 21시마다배부. 갯수는 아래.
			if( pclClient->pclEventTimeManager->InEventTime("MagicLamp_event", &sTime) )
			{
				Lamp[0].Set(0,  21, 0, 0, 30);
			}
		}
		else if ( IsWhereServiceArea( ConstServiceArea_EUROPE ) )
		{
			Lamp[0].Set(0,  6, 0, 0, 30);
			Lamp[1].Set(0, 18, 0, 0, 30);
		}
		else
		{
			return;
		}
	}

	
	static clock_t	sendLampMsgClock = 0;

	for(int i = 0; i < LampSize; ++i)
	{
		if (Lamp[i].m_bGiveSwitch)
		{
			// 같은 날에는 같은 시간의 이벤트가 발동 안하게.(하루 한번)
			if( sTime.wDay == Lamp[i].m_wDay	) 
			{
				continue;
			}

			giveTime.wHour   = Lamp[i].m_wHour;
			giveTime.wMinute = Lamp[i].m_wMinute;
			giveTime.wSecond = Lamp[i].m_wSecond;

			SI32 diffTime = DiffTimeSecond( &sTime, &giveTime );

			if ( diffTime >= -600 && diffTime < 60 )
			{
				if ( TABS( CurrentClock - sendLampMsgClock) > 1000 * 60*5 ) // 60초에 한번씩 ->5분에 한번.
				{
						TCHAR* pnotice = GetTxtFromMgr( 3787 );
						TCHAR buf[128] = {0,};
						StringCchPrintf( buf, 128, pnotice, TABS(diffTime/60) );
						SendNoticeMsg(-1, 0, 0, buf);
						pnotice = GetTxtFromMgr( 3788 );
						SendNoticeMsg(-1, 0, 0, pnotice);

						sendLampMsgClock = CurrentClock;
				}
			}

			if (sTime.wHour   == Lamp[i].m_wHour	&& 
				sTime.wMinute >= Lamp[i].m_wMinute	&& 	
				sTime.wMinute <= Lamp[i].m_wMinute + 3 )
			{
				// 시간이 되었음에도 아직 흥부박이 지급되지 않았다면, 
				SI16 itemunique = ITEMUNIQUE(13025);
				SI16 sLampNum	= 1; 				// 지급하는 램프 갯수.

				if ( IsWhereServiceArea( ConstServiceArea_Japan ) )
				{
					sLampNum = 1;		// 일본은 5개에 1개로 수정.
				}
					
				pclCM->GiveItem(0, itemunique, sLampNum, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER,0,0, Lamp[i].m_wUserLevel);
				
				// 오늘날짜를 넣으므로 위의 조건문을 수행한다.(같은날 발동 안함.)
				Lamp[i].m_wDay = sTime.wDay;					

				TCHAR* pnotice = GetTxtFromMgr( 3786 );
				SendNoticeMsg(-1, 0, 0, pnotice);
			}
		}
		else
		{
			break;
		}
	}
}

//KHY - 0115 발렌타인데이 이벤트.
void cltServer::ValentineEventGiveItemAction()
{
	class ValentineItemTime
	{	
		private:
		public:
			bool	m_bGiveSwitch;			// 주냐 마냐.
			WORD	m_wDay;					// 일.
			WORD	m_wHour;				// 시.
			WORD	m_wMinute;				// 분.
			WORD	m_wSecond;				// 초.
			WORD	m_wUserLevel;			// 유저 제한 레벨.
		
			ValentineItemTime(BYTE _Day = 0, BYTE _Hour = 0, BYTE _Minute = 0, BYTE	_Second = 0, BYTE _UserLevel = 0, bool _GiveSwitch = FALSE)
			{
				m_bGiveSwitch = _GiveSwitch;
				m_wDay		  = _Day;
				m_wHour		  = _Hour;
				m_wMinute	  = _Minute;
				m_wSecond	  = _Second;
				m_wUserLevel  = _UserLevel;
			}

			void Set(BYTE _Day, BYTE _Hour, BYTE _Minute,  BYTE	_Second, BYTE _UserLevel, bool _GiveSwitch = TRUE)
			{
				m_bGiveSwitch = _GiveSwitch;
				m_wDay		  = _Day;
				m_wHour		  = _Hour;
				m_wMinute	  = _Minute;
				m_wSecond	  = _Second;
				m_wUserLevel  = _UserLevel;
			}	
	};

	static const SI32   TimeSize = 24;
	static ValentineItemTime	vItem[TimeSize] = {ValentineItemTime(), };
	
	// 초기화 날짜.
	static WORD	Day = 0;

	static _SYSTEMTIME	giveTime;
	
	DiffTimeSecond( &sTime, &giveTime );
	
	//--------------------------------------------------------------------------------
	// 해당 나라에 시간만 정해주면 아이템이 지급됩니다.
	// 사용하지 않는 나라는 해당 국가 시간넣는 부분에 return을 넣어주세요. 
	// 시간은 개수는 24개 까지 가능합니다.
	//  리소스에 정의된 기간에만 발동합니다.
	//--------------------------------------------------------------------------------
	NDate Today;
	Today.SetDate(sTime.wYear, sTime.wMonth, sTime.wDay );

	if ( Today.GetDateVary() >= clValentineEventStart.GetDateVary() &&
		Today.GetDateVary() <= clValentineEventEnd.GetDateVary() )
	{
		if(Day != sTime.wDay)
		{
			Day = sTime.wDay;

			for(int i = 0; i < TimeSize; ++i)
			{
				vItem[i].Set(0,i, 0, 0, 20); // 매시 30렙이상 준다.
			}
		}
	}
	else
	{
		return;
	}
	
	static clock_t	sendLampMsgClock = 0;

	for(int i = 0; i < TimeSize; ++i)
	{
		if (vItem[i].m_bGiveSwitch)
		{
			// 같은 날에는 같은 시간의 이벤트가 발동 안하게.(하루 한번)
			if(sTime.wDay == vItem[i].m_wDay) 
			{
				continue;
			}

			giveTime.wHour   = vItem[i].m_wHour;
			giveTime.wMinute = vItem[i].m_wMinute;
			giveTime.wSecond = vItem[i].m_wSecond;

			SI32 diffTime = DiffTimeSecond( &sTime, &giveTime );

			if ( diffTime >= -600 && diffTime < 60 )
			{
				if ( TABS( CurrentClock - sendLampMsgClock) > 1000 * 60 ) // 60초에 한번씩
				{
					TCHAR* pnotice = GetTxtFromMgr( 10306 );
					TCHAR buf[128] = {0,};
					StringCchPrintf( buf, 128, pnotice, TABS(diffTime/60) );
					SendNoticeMsg(-1, 0, 0, buf);
					pnotice = GetTxtFromMgr( 10307 );
					SendNoticeMsg(-1, 0, 0, pnotice);

					sendLampMsgClock = CurrentClock;
				}
			}

			if (sTime.wHour   == vItem[i].m_wHour	&& 
				sTime.wMinute >= vItem[i].m_wMinute	&& 	
				sTime.wMinute <= vItem[i].m_wMinute + 3 )
			{
				// 시간이 되었음에도 아직 아이템이  지급되지 않았다면, 
				SI16 itemunique = ITEMUNIQUE(13011);  //T카드
				pclCM->GiveItem(0, itemunique, 1, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER,0,0, vItem[i].m_wUserLevel);
				
				// 오늘날짜를 넣으므로 위의 조건문을 수행한다.(같은날 발동 안함.)
				vItem[i].m_wDay = sTime.wDay;					

				TCHAR* pnotice = GetTxtFromMgr( 10308 );
				SendNoticeMsg(-1, 0, 0, pnotice);
			}
		}
		else
		{
			break;
		}
	}
}

//KHY - 0218 화이트데이 이벤트.
void cltServer::WhiteEventGiveItemAction()
{
	class WhiteItemTime
	{	
		private:
		public:
			bool	m_bGiveSwitch;			// 주냐 마냐.
			WORD	m_wDay;					// 일.
			WORD	m_wHour;				// 시.
			WORD	m_wMinute;				// 분.
			WORD	m_wSecond;				// 초.
			WORD	m_wUserLevel;			// 유저 제한 레벨.
		
			WhiteItemTime(BYTE _Day = 0, BYTE _Hour = 0, BYTE _Minute = 0, BYTE	_Second = 0, BYTE _UserLevel = 0, bool _GiveSwitch = FALSE)
			{
				m_bGiveSwitch = _GiveSwitch;
				m_wDay		  = _Day;
				m_wHour		  = _Hour;
				m_wMinute	  = _Minute;
				m_wSecond	  = _Second;
				m_wUserLevel  = _UserLevel;
			}

			void Set(BYTE _Day, BYTE _Hour, BYTE _Minute,  BYTE	_Second, BYTE _UserLevel, bool _GiveSwitch = TRUE)
			{
				m_bGiveSwitch = _GiveSwitch;
				m_wDay		  = _Day;
				m_wHour		  = _Hour;
				m_wMinute	  = _Minute;
				m_wSecond	  = _Second;
				m_wUserLevel  = _UserLevel;
			}	
	};

	static const SI32   TimeSize = 24;
	static WhiteItemTime	wItem[TimeSize] = {WhiteItemTime(), };
	
	// 초기화 날짜.
	static WORD	Day = 0;

	static _SYSTEMTIME	giveTime;
	
	DiffTimeSecond( &sTime, &giveTime );
	
	//--------------------------------------------------------------------------------
	// 해당 나라에 시간만 정해주면 아이템이 지급됩니다.
	// 사용하지 않는 나라는 해당 국가 시간넣는 부분에 return을 넣어주세요. 
	// 시간은 개수는 24개 까지 가능합니다.
	//  리소스에 정의된 기간에만 발동합니다.
	//--------------------------------------------------------------------------------
	NDate Today;
	Today.SetDate(sTime.wYear, sTime.wMonth, sTime.wDay );

	if ( Today.GetDateVary() >= clWhiteEventStart.GetDateVary() &&
		Today.GetDateVary() <= clWhiteEventEnd.GetDateVary() )
	{
		if(Day != sTime.wDay)
		{
			Day = sTime.wDay;

			for(int i = 0; i < TimeSize; ++i)
			{
				wItem[i].Set(0,i, 0, 0, 20); // 매시 30렙이상 준다.
			}
		}
	}
	else
	{
		return;
	}
	
	static clock_t	sendLampMsgClock = 0;

	for(int i = 0; i < TimeSize; ++i)
	{
		if (wItem[i].m_bGiveSwitch)
		{
			// 같은 날에는 같은 시간의 이벤트가 발동 안하게.(하루 한번)
			if(sTime.wDay == wItem[i].m_wDay) 
			{
				continue;
			}

			giveTime.wHour   = wItem[i].m_wHour;
			giveTime.wMinute = wItem[i].m_wMinute;
			giveTime.wSecond = wItem[i].m_wSecond;

			SI32 diffTime = DiffTimeSecond( &sTime, &giveTime );

			if ( diffTime >= -600 && diffTime < 60 )
			{
				if ( TABS( CurrentClock - sendLampMsgClock) > 1000 * 60 ) // 60초에 한번씩
				{
					TCHAR* pnotice = GetTxtFromMgr( 7956 );
					TCHAR buf[128] = {0,};

					StringCchPrintf( buf, 128, pnotice, TABS(diffTime/60) );
					SendNoticeMsg(-1, 0, 0, buf);

					sendLampMsgClock = CurrentClock;
				}
			}

			if (sTime.wHour   == wItem[i].m_wHour	&& 
				sTime.wMinute >= wItem[i].m_wMinute	&& 	
				sTime.wMinute <= wItem[i].m_wMinute + 3 )
			{
				// 시간이 되었음에도 아직 아이템이  지급되지 않았다면, 
				SI16 itemunique = ITEMUNIQUE(13020);  //E카드
				pclCM->GiveItem(0, itemunique, 1, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER,0,0, wItem[i].m_wUserLevel);
				
				// 오늘날짜를 넣으므로 위의 조건문을 수행한다.(같은날 발동 안함.)
				wItem[i].m_wDay = sTime.wDay;					

				TCHAR* pnotice = GetTxtFromMgr( 10309 );
				SendNoticeMsg(-1, 0, 0, pnotice);
			}
		}
		else
		{
			break;
		}
	}
}

/*
// 흥부박 이벤트 처리.
void cltServer::SpecialQuestAction()
{
	static WORD giveday_1st		= 0;
	static WORD giveday_2nd		= 0;


	WORD givehour_1st		= 0;
	WORD giveminute_1st		= 0;
	WORD givehour_2nd		= 0;
	WORD giveminute_2nd		= 0;

	bool giveswitch_1st		= false;
	bool giveswitch_2nd		= false;

	if ( IsWhereServiceArea( ConstServiceArea_China ) )
	{
		giveswitch_1st		= true;
		givehour_1st		= 20;
		giveminute_1st		= 30;
	}
	else if ( IsWhereServiceArea( ConstServiceArea_Korea ) )
	{
		giveswitch_1st		= true;
		givehour_1st		= 19;
		giveminute_1st		= 0;
	}
	else if ( IsWhereServiceArea( ConstServiceArea_English ) )
	{
		giveswitch_1st		= true;
		givehour_1st		= 6;
		giveminute_1st		= 0;

		giveswitch_2nd		= true;
		givehour_2nd		= 18;
		giveminute_2nd		= 0;
	}
	else
	{
		return;
	}

	_SYSTEMTIME giveTime;
	static clock_t sendLampMsgClock = 0;

	//----------------------------------------------------------------
	// 흥부박 지급하기. 
	//----------------------------------------------------------------
	if ( giveswitch_1st )
	{
		giveTime.wHour = givehour_1st;
		giveTime.wMinute = giveminute_1st;
		giveTime.wSecond = 0;

		SI32 diffTime = DiffTimeSecond( &sTime, &giveTime );
		if ( diffTime >= -600 && diffTime < 60 )
		{
			if ( TABS( CurrentClock - sendLampMsgClock) > 1000 * 60 ) // 60초에 한번씩
			{
				TCHAR* pnotice = GetTxtFromMgr( 3787 );
				TCHAR buf[128] = {0,};
				StringCchPrintf( buf, 128, pnotice, TABS(diffTime/60) );
				SendNoticeMsg(-1, 0, 0, buf);
				pnotice = GetTxtFromMgr( 3788 );
				SendNoticeMsg(-1, 0, 0, pnotice);

				sendLampMsgClock = CurrentClock;
			}
		}

		if ( giveday_1st != sTime.wDay && 
			sTime.wHour == givehour_1st && 
			sTime.wMinute >= giveminute_1st && sTime.wMinute <= giveminute_1st + 3 )
		{
			// 시간이 되었음에도 아직 흥부박이 지급되지 않았다면, 
			SI16 itemunique = ITEMUNIQUE(8035);
			pclCM->GiveItem(0, itemunique, 1, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER,0,0,30);
			giveday_1st = sTime.wDay;

			TCHAR* pnotice = GetTxtFromMgr( 3786 );
			SendNoticeMsg(-1, 0, 0, pnotice);
		}
	}

	if ( giveswitch_2nd )
	{
		giveTime.wHour = givehour_1st;
		giveTime.wMinute = giveminute_1st;
		giveTime.wSecond = 0;

		SI32 diffTime = DiffTimeSecond( &sTime, &giveTime );
		if ( diffTime >= -600 && diffTime < 60 )
		{
			if ( TABS( CurrentClock - sendLampMsgClock) > 1000 * 60 ) // 30초에 한번씩
			{
				TCHAR* pnotice = GetTxtFromMgr( 3787 );
				TCHAR buf[128] = {0,};
				StringCchPrintf( buf, 128, pnotice, TABS(diffTime/60) );
				SendNoticeMsg(-1, 0, 0, buf);
				pnotice = GetTxtFromMgr( 3788 );
				SendNoticeMsg(-1, 0, 0, pnotice);

				sendLampMsgClock = CurrentClock;
			}
		}

		if ( giveday_2nd != sTime.wDay && 
			sTime.wHour == givehour_2nd && 
			sTime.wMinute >= giveminute_2nd && sTime.wMinute <= giveminute_2nd + 3 )
		{
			// 시간이 되었음에도 아직 흥부박이 지급되지 않았다면, 
			SI16 itemunique = ITEMUNIQUE(8035);
			pclCM->GiveItem(0, itemunique, 1, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER,0,0,30);
			giveday_2nd = sTime.wDay;

			TCHAR* pnotice = GetTxtFromMgr( 3786 );
			SendNoticeMsg(-1, 0, 0, pnotice);
		}
	}
}*/



void cltServer::PCRoomEventAction()
{
	static BOOL bPCRoomGiveItem = FALSE;

	return;

	//if( sTime.wHour == 17 &&
	//	sTime.wMinute >= 30 &&
	//	sTime.wMinute < 35 ) 
	//{

	//	if( bPCRoomGiveItem == TRUE ) return;

	//	//----------------------------------------------------------------
	//	// PC방 유저에게 흥부박 지급하기. 
	//	//----------------------------------------------------------------		
	//	TCHAR* pText = GetTxtFromMgr(3791);
	//	SendNoticeMsg(-1, 0, 0, pText);

	//	//흥부박 
	//	SI16 itemunique = 8200;
	//	pclCM->GiveItem(0, itemunique, 1, GIVEITEM_MODE_PCROOM, INPUTITEMPERSON_OPTION_MASTER,0);

	//	bPCRoomGiveItem = TRUE;
	//	
	//} else {

	//	bPCRoomGiveItem = FALSE;
	//}
}
