//---------------------------------
// 2003/9/27 김태곤
//---------------------------------
#include "CommonLogic.h"
#include "Health.h"

#include "Char/CharManager/CharManager.h"
#include "Char/CharCommon/Char-Common.h"
#include "Skill/Skill-Manager.h"

extern cltCommonLogic* pclClient;

//---------------------------------------------------------------
// 건강 
//----------------------------------------------------------------
cltHealth::cltHealth()
{
	pclInfo = NULL;
}

SI32 cltHealth::GetBasic(SI32 level)
{
	return  min(100, max(30, level));
}

SI32 cltHealth::GetHungryMaxLimit(SI32 level)
{
	SI32 basic = GetBasic(level);

	return basic * 2;
}

// 배고픔 상수를 찾는다. 
SI32 cltHealth::GetHungryStatus(SI32 level)
{
	SI32 basic	= GetBasic(level);
	SI32 status = 0;

	SI32 interval1 = (300 - basic) / 3;

	
	if(pclInfo->siHungry  > basic  + (interval1 * 2))	
	{
		status |= HEALTH_STATUS_HUNGRY_1;
	}
	else if(pclInfo->siHungry  > basic  + (interval1 * 1))	
	{
		status |= HEALTH_STATUS_HUNGRY_2;
	}
	else if(pclInfo->siHungry  > basic  + (interval1 * 0))	
	{
		status |= HEALTH_STATUS_HUNGRY_3;
	}
	else if(pclInfo->siHungry  >= -basic)
	{
		status |= HEALTH_STATUS_HUNGRY_NORMAL;

		SI32 interval2 = (basic * 2) / 6;
		if(pclInfo->siHungry >  interval2)
		{
			status |= HEALTH_STATUS_HUNGRY_4;
		}
		else if(pclInfo->siHungry >  -interval2)
		{
			status |= HEALTH_STATUS_HUNGRY_5;
		}
		else
		{
			status |= HEALTH_STATUS_HUNGRY_6;
		}

	}
	else if(pclInfo->siHungry  >= -basic - interval1)
	{
		status |= HEALTH_STATUS_HUNGRY_7;
	}
	else if(pclInfo->siHungry  >= -basic - (interval1 * 2))
	{
		status |= HEALTH_STATUS_HUNGRY_8;
	}
	else 
	{
		status |= HEALTH_STATUS_HUNGRY_9;
	}


	return status;

}


bool cltHealth::Action(SI32 level, DWORD currentclock)
{

	SI32 penalty = 0;
	bool rtnval = false;
	cltDiseaseInfo* pcldisease ;

	if(TABS(currentclock - ActionClock) < HEALTH_ACTION_DELAY)return false;
	ActionClock	= currentclock;

	SI32 diff = 0;


	//---------------------------------------------------
	// 내장 기관 증감. 
	//---------------------------------------------------
	
	// 소화 기관. 

	// 배가 너무 부른 상태 
	diff = pclInfo->siHungry;

	// 배고픈 상태에 문제가 많다면, 
	if(level >= FATHER_LEVEL && GetHungryStatus(level) != HEALTH_STATUS_HUNGRY_NORMAL && TABS(diff) > (rand() % GetHungryMaxLimit(level)) )
	{
		// 소화계에 이상 신호를 보낸다. 
		SI32 max = MAXORG(level);
		if(pclInfo->siInnerOrg1 > -max)
		{
			pclInfo->siInnerOrg1--;
		}
		rtnval	= true;
		
	}
	else
	{
		// 질병은 없는 상태여야 소화계가 정상을 찾을 수 있다. 
		if(pclInfo->siInnerOrg1 < 0 && pclInfo->uiOrg1Disease == 0)
		{
			pclInfo->siInnerOrg1+=2;		// 소화계가 정상을 찾는다. 
			if(pclInfo->siInnerOrg1 > 0)pclInfo->siInnerOrg1 = 0;

			rtnval	= true;
		}
	}
	
	// 순환 기관 
	// 질병은 없는 상태여야 소화계가 정상을 찾을 수 있다. 
	if(pclInfo->siInnerOrg2 < 0 && pclInfo->uiOrg2Disease == 0)
	{
		pclInfo->siInnerOrg2++;		// 순환계가 정상을 찾는다. 
		rtnval	= true;
	}

	// 호흡 기관 
	// 질병은 없는 상태여야 호흡계가 정상을 찾을 수 있다. 
	if(pclInfo->siInnerOrg3 < 0 && pclInfo->uiOrg3Disease == 0)
	{
		pclInfo->siInnerOrg3++;	
		rtnval	= true;
	}

	// 신경 기관 
	// 질병은 없는 상태여야 신경계가 정상을 찾을 수 있다. 
	if(pclInfo->siInnerOrg4 < 0 && pclInfo->uiOrg4Disease == 0)
	{
		pclInfo->siInnerOrg4++;		// 순환계가 정상을 찾는다. 
		rtnval	= true;
	}

	//----------------------------------------------------
	// 낮은 레벨의 사용자들은 건강의 영향을 받지 않는다. 
	//---------------------------------------------------
	if( pclClient->IsCountrySwitch( Switch_Health ) )
	{
		if(level < INIT_LEVEL_SECOND )
			return rtnval;
	}
	else
	{
		if(level < FATHER_LEVEL )
			return rtnval;
	}


	//----------------------------------------------
	// 질병에 걸리게 한다. 
	//----------------------------------------------
	// 소화계의 상태를 점검한다. 
	// 질병에 걸린다. 
	if(pclInfo->siInnerOrg1 < -30)
	{
		if(rand() % (1000 + pclInfo->uiOrg1Disease * 30)  < (-pclInfo->siInnerOrg1))
		{
			if(pclInfo->uiOrg1Disease < min(level/2, 50))
			{
				pclInfo->uiOrg1Disease++;
				rtnval	= true;
			}
		}
	}

	// 순환계의 상태를 점검한다. 
	// 질병에 걸린다. 
	if(pclInfo->siInnerOrg2 < -30)
	{
		if(rand() % (500 + pclInfo->uiOrg2Disease * 20)  < (-pclInfo->siInnerOrg2))
		{
			if(pclInfo->uiOrg2Disease < min(level/2, 50))
			{
				pclInfo->uiOrg2Disease++;
				rtnval	= true;
			}
		}
	}

	// 호흡계의 상태를 점검한다. 
	// 질병에 걸린다. 
	if(pclInfo->siInnerOrg3 < -30)
	{
		if(rand() % (500 + pclInfo->uiOrg3Disease * 20)  < (-pclInfo->siInnerOrg3))
		{
			if(pclInfo->uiOrg3Disease < min(level/2, 50))
			{
				pclInfo->uiOrg3Disease++;
				rtnval	= true;
			}
		}
	}

	// 신경계의 상태를 점검한다. 
	// 질병에 걸린다. 
	if(pclInfo->siInnerOrg4 < -30)
	{
		if(rand() % (500 + pclInfo->uiOrg4Disease * 20)  < (-pclInfo->siInnerOrg4))
		{
			if(pclInfo->uiOrg4Disease < min(level/2, 50))
			{
				pclInfo->uiOrg4Disease++;
				rtnval	= true;
			}
		}
	}



	//------------------------------------------------
	// 질병시의 처치. 
	//------------------------------------------------

	// 소화계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_1, pclInfo->uiOrg1Disease);
	if(pcldisease)
	{
		penalty += pcldisease->siPenalty;
	}

	// 순환계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_2, pclInfo->uiOrg2Disease);
	if(pcldisease)
	{
		penalty += pcldisease->siPenalty;
	}

	// 호흡계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_3, pclInfo->uiOrg3Disease);
	if(pcldisease)
	{
		penalty += pcldisease->siPenalty;
	}

	// 신경계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_4, pclInfo->uiOrg4Disease);
	if(pcldisease)
	{
		penalty += pcldisease->siPenalty;
	}
	
	// 페널티의 최대 범위를 설정한다. 
	if(penalty)
	{
		penalty = min(50, penalty);
	}
	


	//  질병이 있으면, 
	if(penalty)
	{
		// 건강을 질병의 페널티만큼 줄여준다. 
		SI32 limit = 100 - penalty;
		if(pclInfo->uiHealthPoint > limit)
		{
			pclInfo->uiHealthPoint--;
			rtnval = true;
		}
	}
	// 질병이 없는데 건강이 떨어져 있으면 건강을 100으로 올려준다. 
	else if(pclInfo->uiHealthPoint < 100)
	{
		pclInfo->uiHealthPoint++;
		rtnval = true;
	}
	// 질병도 없고 건강도 정상 이상이면, 
	else
	{
		// 간혹 건강을 높여준다. 
		if(rand() % 10 == 0)
		{
			if(pclInfo->siInnerOrg1 >= 0 && pclInfo->siInnerOrg2 >= 0
			&& pclInfo->siInnerOrg3 >= 0 && pclInfo->siInnerOrg4 >= 0)	// 내장 기관이 정상이어야 한다. 		
			{
				if(pclInfo->uiHealthPoint < 110)	// 최대 건강 수치를 넘길 수는 없다. 
				{
					pclInfo->uiHealthPoint ++;
					rtnval = true;
				}
			}
		}
	}



	return rtnval;

}

// 건강 상태를 분석해서 그 결과를 돌려준다. 
SI32 cltHealth::AnalyzeHealth(SI32 level)
{
	SI32 status = 0;
	SI32 diff = 0 ;

	// 배가 고픈 정도를 구한다. 
	status = GetHungryStatus(level);

	// 소화계의 이상 유무를 구한다.
	if(pclInfo->siInnerOrg1 < -80)
	{
		status |= HEALTH_STATUS_ORG1_SERIOUS;
	}
	else if(pclInfo->siInnerOrg1 < -50)
	{
		status |= HEALTH_STATUS_ORG1_BAD;
	}
	else if(pclInfo->siInnerOrg1 < -20)
	{
		status |= HEALTH_STATUS_ORG1_NORMAL;
	}
	else 
	{
		status |= HEALTH_STATUS_ORG1_GOOD;
	}

	// 순환계의  이상 유무를 구한다.
	if(pclInfo->siInnerOrg2 < -80)
	{
		status |= HEALTH_STATUS_ORG2_SERIOUS;
	}
	else if(pclInfo->siInnerOrg2 < -50)
	{
		status |= HEALTH_STATUS_ORG2_BAD;
	}
	else if(pclInfo->siInnerOrg2 < -20)
	{
		status |= HEALTH_STATUS_ORG2_NORMAL;
	}
	else 
	{
		status |= HEALTH_STATUS_ORG2_GOOD;
	}

	// 호흡계의  이상 유무를 구한다.
	if(pclInfo->siInnerOrg3 < -80)
	{
		status |= HEALTH_STATUS_ORG3_SERIOUS;
	}
	else if(pclInfo->siInnerOrg3 < -50)
	{
		status |= HEALTH_STATUS_ORG3_BAD;
	}
	else if(pclInfo->siInnerOrg3 < -20)
	{
		status |= HEALTH_STATUS_ORG3_NORMAL;
	}
	else 
	{
		status |= HEALTH_STATUS_ORG3_GOOD;
	}

	// 신경계의  이상 유무를 구한다.
	if(pclInfo->siInnerOrg2 < -80)
	{
		status |= HEALTH_STATUS_ORG4_SERIOUS;
	}
	else if(pclInfo->siInnerOrg4 < -50)
	{
		status |= HEALTH_STATUS_ORG4_BAD;
	}
	else if(pclInfo->siInnerOrg4 < -20)
	{
		status |= HEALTH_STATUS_ORG4_NORMAL;
	}
	else 
	{
		status |= HEALTH_STATUS_ORG4_GOOD;
	}

	return status;
}

// 현재의 건강 상태에 대한 Text를 구한다.
bool cltHealth::GetHealthText(SI32 level, TCHAR* text, SI16 txtSize)
{
	cltCharCommon* pclchar = pclClient->pclCM->GetChar( 1 );
	if ( NULL == pclchar )
	{
		return false;
	}

	TCHAR buffer[2048] =TEXT("");
	TCHAR temp[1024];

	SI32 status = AnalyzeHealth(level);
	SI32 max	= min(100, MAXORG(level));

	// 총평. 
	SI32 healthpoint = pclInfo->uiHealthPoint;


	bool bhungryshowswitch = false;
	if(status & (HEALTH_STATUS_HUNGRY_1 | HEALTH_STATUS_HUNGRY_2))
	{
		TCHAR* pText = GetTxtFromMgr(532);
		StringCchPrintf(temp, 1024, pText);
		StringCchCat(temp, 1024, "\r\n");
		StringCchCat(buffer, 2048, temp);
		bhungryshowswitch = true;
	}
	if(status & HEALTH_STATUS_HUNGRY_3)
	{
		TCHAR* pText = GetTxtFromMgr(533);
		StringCchPrintf(temp, 1024, pText);
		StringCchCat(temp, 1024, "\r\n");
		StringCchCat(buffer, 2048, temp);
		bhungryshowswitch = true;
	}
	if(status & HEALTH_STATUS_HUNGRY_NORMAL)
	{
		TCHAR* pText = GetTxtFromMgr(534);
		StringCchPrintf(temp, 1024, pText);
		StringCchCat(temp, 1024, "\r\n");
		StringCchCat(buffer, 2048, temp);
		bhungryshowswitch = true;
	}
	if(status & HEALTH_STATUS_HUNGRY_7)
	{
		TCHAR* pText = GetTxtFromMgr(535);
		StringCchPrintf(temp, 1024, pText);
		StringCchCat(temp, 1024, "\r\n");
		StringCchCat(buffer, 2048, temp);
		bhungryshowswitch = true;
	}
	if(status & (HEALTH_STATUS_HUNGRY_8 | HEALTH_STATUS_HUNGRY_9))
	{
		TCHAR* pText = GetTxtFromMgr(536);
		StringCchPrintf(temp, 1024, pText);
		StringCchCat(temp, 1024, "\r\n");
		StringCchCat(buffer, 2048, temp);
		bhungryshowswitch = true;
	}

	if(bhungryshowswitch)
	{
		TCHAR* pText = GetTxtFromMgr(537);
		StringCchPrintf(temp, 1024, pText, pclInfo->siHungry, -GetBasic(level), GetBasic(level));
		StringCchCat(buffer, 2048, temp);
	}

	// 소화계 질병을 조사한다. 
	cltDiseaseInfo* pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_1, pclInfo->uiOrg1Disease);
	if(pcldisease)
	{
		TCHAR* pText = GetTxtFromMgr(538);
		StringCchPrintf(temp, 1024, pText, pcldisease->szName, pclInfo->uiOrg1Disease - pcldisease->siLevel + 1);
		StringCchCat(buffer, 2048, temp);
	}
	// 소화계의 전반적인 상태를 알려준다. 
	else
	{
		if(status & HEALTH_STATUS_ORG1_SERIOUS)
		{
			TCHAR* pText = GetTxtFromMgr(539);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg1, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG1_BAD)
		{
			TCHAR* pText = GetTxtFromMgr(540);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg1, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG1_NORMAL)
		{
			TCHAR* pText = GetTxtFromMgr(541);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg1, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG1_GOOD)
		{
			TCHAR* pText = GetTxtFromMgr(542);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg1, max);
			StringCchCat(buffer, 2048, temp);
		}
	}

	// 순환계 질병을 조사한다. 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_2, pclInfo->uiOrg2Disease);
	if(pcldisease)
	{
		TCHAR* pText = GetTxtFromMgr(538);
		StringCchPrintf(temp, 1024, pText, pcldisease->szName, pclInfo->uiOrg2Disease - pcldisease->siLevel + 1);
		StringCchCat(buffer, 2048, temp);
	}
	// 전반적인 상태를 알려준다. 
	else
	{
		if(status & HEALTH_STATUS_ORG2_SERIOUS)
		{
			TCHAR* pText = GetTxtFromMgr(543);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg2, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG2_BAD)
		{
			TCHAR* pText = GetTxtFromMgr(544);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg2, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG2_NORMAL)
		{
			TCHAR* pText = GetTxtFromMgr(545);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg2, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG2_GOOD)
		{
			TCHAR* pText = GetTxtFromMgr(546);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg2, max);
			StringCchCat(buffer, 2048, temp);
		}

	}

	// 호흡계 질병을 조사한다. 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_3, pclInfo->uiOrg3Disease);
	if(pcldisease)
	{
		TCHAR* pText = GetTxtFromMgr(538);
		StringCchPrintf(temp, 1024, pText, pcldisease->szName, pclInfo->uiOrg3Disease - pcldisease->siLevel + 1);
		StringCchCat(buffer, 2048, temp);
	}
	// 전반적인 상태를 알려준다. 
	else
	{
		if(status & HEALTH_STATUS_ORG3_SERIOUS)
		{
			TCHAR* pText = GetTxtFromMgr(547);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg3, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG3_BAD)
		{
			TCHAR* pText = GetTxtFromMgr(548);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg3, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG3_NORMAL)
		{
			TCHAR* pText = GetTxtFromMgr(549);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg3, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG3_GOOD)
		{
			TCHAR* pText = GetTxtFromMgr(550);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg3, max);
			StringCchCat(buffer, 2048, temp);
		}

	}

	// 신경계 질병을 조사한다. 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_4, pclInfo->uiOrg4Disease);
	if(pcldisease)
	{
		TCHAR* pText = GetTxtFromMgr(538);
		StringCchPrintf(temp, 1024, pText, pcldisease->szName, pclInfo->uiOrg4Disease - pcldisease->siLevel + 1);
		StringCchCat(buffer, 2048, temp);
	}
	// 전반적인 상태를 알려준다. 
	else
	{
		if(status & HEALTH_STATUS_ORG4_SERIOUS)
		{
			TCHAR* pText = GetTxtFromMgr(551);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg4, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG4_BAD)
		{
			TCHAR* pText = GetTxtFromMgr(552);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg4, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG4_NORMAL)
		{
			TCHAR* pText = GetTxtFromMgr(553);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg4, max);
			StringCchCat(buffer, 2048, temp);
		}
		if(status & HEALTH_STATUS_ORG4_GOOD)
		{
			TCHAR* pText = GetTxtFromMgr(554);
			StringCchPrintf(temp, 1024, pText, pclInfo->siInnerOrg4, max);
			StringCchCat(buffer, 2048, temp);
		}

	}

												
	if(pclClient->IsCountrySwitch(Switch_PeriodItem) == false)				//	이하의 코드는 Switch_PeriodItem가 false일 경우에만  적용되어야 한다.
	{											//	Switch_PeriodItem가 true일 경우는 perioditemlistdlg클래스에서 밑의 코드구현이 포함 되어있다.
		TCHAR warpbuf[2048] = TEXT("") ;
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp )
		{
			//무한 워프 기간 표시.
			
			cltDate MaxWarpDate;//무한 티켓 날자 최대 셋팅
			MaxWarpDate.uiYear = 255;
			MaxWarpDate.uiMonth = 12;
			MaxWarpDate.uiDay = 30;
			SI32 MAX_GAME_DATE = MaxWarpDate.GetDateVary();
			SI32 warpdate = pclClient->pclCM->CR[1]->pclCI->clBI.siNoLimitTicketWarpDateVary;//현제 워프가능 기한 
			if (warpdate >=MAX_GAME_DATE) {
				TCHAR* timebuf= GetTxtFromMgr(7866);
				TCHAR* pText = GetTxtFromMgr(3187);
				StringCchPrintf(warpbuf, 2048, pText,timebuf);
				StringCchCat(buffer, 2048, warpbuf);


			}
			else{
				TCHAR timebuf[512] = TEXT("") ;
				cltDate cldate ;
				cldate.MakeFullDateFromDateVary(pclClient->pclCM->CR[1]->pclCI->clBI.siNoLimitTicketWarpDateVary);
				if ( cldate.GetDateText(timebuf) )
				{
					TCHAR* pText = GetTxtFromMgr(3187);
					StringCchPrintf(warpbuf, 2048, pText,timebuf);
				}
				StringCchCat(buffer, 2048, warpbuf);
				StringCchCopy(text, txtSize, buffer);
				StringCchCopy(buffer, txtSize, text);
			}
		}
		//경험치 150%포션 기간표시
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_PREMIUMEXPUP)){
			
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_PREMIUMEXPUP);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			/*cldate.MakeFullDateFromDateVary( BuffMinute );*/
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(7937);
				StringCchPrintf(warpbuf, 2048, pText,timebuf);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else{
			StringCchCopy(text, txtSize, buffer);
		}
		//경험치 200%포션 기간표시
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUPEREXPUP))
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_SUPEREXPUP);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(7938);
				StringCchPrintf(warpbuf, 2048, pText,timebuf);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		//경험치 300%포션 기간표시(스페셜 엠플)
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SPECIALAMPUL_EXPUP300))
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_SPECIALAMPUL_EXPUP300);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(9888);
				StringCchPrintf(warpbuf, 2048, pText, timebuf);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		// 마을 EXP 캡슐
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_RESIDENTS_EXPUP_10 ) )
		{
			TCHAR timebuf[512] = TEXT("");
			cltDate cldate;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime( BUF_RESIDENTS_EXPUP_10 );
			cldate.MakeFullHourFromHourVary( BuffMinute );
			if (cldate.GetDateText( timebuf ) )
			{
				TCHAR* pText = GetTxtFromMgr( 40827 );
				StringCchPrintf( warpbuf, 2048, pText, timebuf );
			}
			StringCchCat( buffer, 2048, warpbuf );
			StringCchCopy( text, txtSize, buffer );
			StringCchCopy( buffer, txtSize, text );
		}
		else
		{
			StringCchCopy( text, txtSize, buffer );
		}

		// 마을 강화제
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_RESIDENTS_STRONG ) )
		{
			TCHAR timebuf[512] = TEXT("");
			cltDate cldate;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime( BUF_RESIDENTS_STRONG );
			cldate.MakeFullHourFromHourVary( BuffMinute );
			if (cldate.GetDateText( timebuf ) )
			{
				TCHAR* pText = GetTxtFromMgr( 40828 );
				StringCchPrintf( warpbuf, 2048, pText, timebuf );
			}
			StringCchCat( buffer, 2048, warpbuf );
			StringCchCopy( text, txtSize, buffer );
			StringCchCopy( buffer, txtSize, text );
		}
		else
		{
			StringCchCopy( text, txtSize, buffer );
		}

		//채광 농경용 아이템
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FARMINGNMININGFOREVER)){

			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_FARMINGNMININGFOREVER);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(7962);
				StringCchPrintf(warpbuf, 2048, pText,timebuf);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}
		//공간상자.
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bSpaceBoxPremiumService)
		{ 
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			cldate.MakeFullDateFromDateVary(pclClient->pclCM->CR[1]->pclCI->clBI.siSpaceBoxPremiumServiceDate);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(7913);
				StringCchPrintf(warpbuf, 2048, pText,timebuf);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}
		
		//[진성] 스텟 적용 버프의 남은 시간표시 => 2008-6-19 
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_STR))
		{ 
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_STR);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(8468);
				StringCchPrintf(warpbuf, 2048, pText,
								timebuf, 
								pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_STR)->siPercent);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		//[진성] 스텟 적용 버프의 남은 시간표시 => 2008-6-19 
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_DEX))
		{ 
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_DEX);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(8469);
				StringCchPrintf(warpbuf, 2048, pText,
								timebuf, 
								pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_DEX)->siPercent);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}


		//[진성] 스텟 적용 버프의 남은 시간표시 => 2008-6-19 
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_MAG))
		{ 
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_MAG);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(8470);
				StringCchPrintf(warpbuf, 2048, pText,
								timebuf, 
								pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_MAG)->siPercent);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		//[지연] 스텟 적용 버프의 남은 시간표시 => 2009-10-23 
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_WIS))
		{ 
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_WIS);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(40327);
				StringCchPrintf(warpbuf, 2048, pText,
					timebuf, 
					pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_WIS)->siPercent);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}


		//[진성] 스텟 적용 버프의 남은 시간표시 => 2008-6-19 
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_VIT))
		{ 
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_VIT);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(8471);
				StringCchPrintf(warpbuf, 2048, pText,
								timebuf, 
								pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_VIT)->siPercent);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}


		//[진성] 스텟 적용 버프의 남은 시간표시 => 2008-6-19 
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_HAND))
		{ 
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_HAND);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(8472);
				StringCchPrintf(warpbuf, 2048, pText,
								timebuf, 
								pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_HAND)->siPercent);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}
		//[진성] 스텟 적용 버프의 남은 시간표시 => 2008-6-19 
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_EXP_UP))
		{ 
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_EXP_UP);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(8542);  
				StringCchPrintf(warpbuf, 2048, pText,
					timebuf, 
					pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_EXP_UP)->siPercent+100);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}


		TCHAR premiumbuf[2048] = TEXT("") ;

		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bPremiumService )
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			cldate.MakeFullDateFromDateVary(pclClient->pclCM->CR[1]->pclCI->clBI.siPremiumServiceDate);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(5606);
				StringCchPrintf(premiumbuf, 2048, pText,timebuf);
			}

		
			StringCchCat(buffer, 2048, premiumbuf);
			StringCchCopy(text,txtSize, buffer);
			StringCchCopy(buffer, 2048, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		TCHAR combatbuf[2048] = TEXT("") ;	
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bCombatService )
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			cldate.MakeFullDateFromDateVary(pclClient->pclCM->CR[1]->pclCI->clBI.siCombatServiceDate);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(5620);
				StringCchPrintf(combatbuf, 2048, pText,timebuf);
			}       
			StringCchCat(buffer, 2048, combatbuf);
			StringCchCopy(text, txtSize, buffer);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		TCHAR MakeItembuf[2048] = TEXT("") ;	
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bMakeItemService )
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			cldate.MakeFullDateFromDateVary(pclClient->pclCM->CR[1]->pclCI->clBI.siMakeItemServiceDate);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(6130);
				StringCchPrintf(MakeItembuf, 2048, pText,timebuf);
			}

			StringCchCat(buffer, 2048, MakeItembuf);
			StringCchCopy(text,txtSize, buffer);
			StringCchCopy(buffer, 2048, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		TCHAR Bulletbuf[2048] = TEXT("") ;	
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bBulletService )
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			cldate.MakeFullDateFromDateVary(pclClient->pclCM->CR[1]->pclCI->clBI.siBulletServiceDate);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(6275);
				StringCchPrintf(Bulletbuf, 2048, pText,timebuf);
			}

			StringCchCat(buffer, 2048, Bulletbuf);
			StringCchCopy(text,txtSize, buffer);
			StringCchCopy(buffer, 2048, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		TCHAR SummonStaminabuf[2048] = TEXT("") ;	
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bSummonStaminaService )
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			cldate.MakeFullDateFromDateVary(pclClient->pclCM->CR[1]->pclCI->clBI.siSummonStaminaServiceDate);
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(6440);
				StringCchPrintf(SummonStaminabuf, 2048, pText,timebuf);
			}

			StringCchCat(buffer, 2048, SummonStaminabuf);
			StringCchCopy(text,txtSize, buffer);
			StringCchCopy(buffer, 2048, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		TCHAR marketPremiumBuf[2048] = TEXT("") ;	
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bMarKetPremiumService )
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			cldate.MakeFullDateFromDateVary( pclClient->pclCM->CR[1]->pclCI->clBI.siMarKetPremiumServiceDate );
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(10169);
				StringCchPrintf(marketPremiumBuf, 2048, pText,timebuf);
			}

			StringCchCat(buffer, 2048, marketPremiumBuf);
			StringCchCopy(text,txtSize, buffer);
			StringCchCopy(buffer, 2048, text);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		//[진성] 통합시장 프리미엄 정보 보이기.
		TCHAR NewMarketPremiumBuf[2048] = TEXT("") ;	
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bNewMarketPremiumService )
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			cldate.MakeFullDateFromDateVary( pclClient->pclCM->CR[1]->pclCI->clBI.siNewMarketPremiumServiceDate );
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = "[%s]까지  통합시장 프리미엄을 사용 중입니다.";//GetTxtFromMgr(10169);
				StringCchPrintf(NewMarketPremiumBuf, 2048, pText,timebuf);
			}

			StringCchCat(  buffer, sizeof(buffer),	NewMarketPremiumBuf	);
			StringCchCopy( text,   txtSize,			buffer					);
			StringCchCopy( buffer, sizeof(buffer),	text					);
		}
		else
		{
			StringCchCopy(text, txtSize, buffer);
		}

		//	메달 시간표시
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_OLYMPICMEDAL)){

			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_OLYMPICMEDAL);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			if ( cldate.GetDateText(timebuf) )
			{
				SI16 bufpersent = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent( BUF_OLYMPICMEDAL ) ;
				if (bufpersent == 20) {
					TCHAR* pText = GetTxtFromMgr(8648); // 금메달
					StringCchPrintf(warpbuf, 2048, pText,timebuf);
				}
				else if (bufpersent == 10) {
					TCHAR* pText = GetTxtFromMgr(8649); //은메달
					StringCchPrintf(warpbuf, 2048, pText,timebuf);
				}
				else if (bufpersent == 5) {
					TCHAR* pText = GetTxtFromMgr(8650); //동메달
					StringCchPrintf(warpbuf, 2048, pText,timebuf);
				}
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else{
			StringCchCopy(text, txtSize, buffer);
		}

		// PCK : 제조 프리미엄 기간 표시
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_PREMIUMMAKEEXPUP)){

			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			SI32 BuffMinute = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_PREMIUMMAKEEXPUP);
			cldate.MakeFullHourFromHourVary(BuffMinute);
			/*cldate.MakeFullDateFromDateVary( BuffMinute );*/
			if ( cldate.GetDateText(timebuf) )
			{
				TCHAR* pText = GetTxtFromMgr(9554);
				StringCchPrintf(warpbuf, 2048, pText,timebuf);
			}
			StringCchCat(buffer, 2048, warpbuf);
			StringCchCopy(text, txtSize, buffer);
			StringCchCopy(buffer, txtSize, text);
		}
		else{
			StringCchCopy(text, txtSize, buffer);
		}
	}
	else
	{
		StringCchCopy(text, txtSize, buffer);
	}


	// 무기술의 달인 - 엠블렘 표시
	if ( (PERSONITEM_WEAPON1 == pclchar->ActiveWeapon) || (PERSONITEM_WEAPON2 == pclchar->ActiveWeapon) )
	{
		TCHAR szEmblemText[512] = TEXT("");

		SI32 siItemType		= pclchar->pclCI->clCharItem.GetWeaponType( pclClient->pclItemManager, pclchar->ActiveWeapon );
		SI32 siSkillUnique	= pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel = pclchar->pclCI->clCharSkill.GetSkillExpLevel( siSkillUnique );

			if ( pclClient->pclSkillManager->m_clMasterManager.GetEmblemExplain(siItemType, siSkillLevel, szEmblemText, sizeof(szEmblemText)) )
			{
				StringCchCat(buffer, 2048, szEmblemText);
				StringCchCopy(text, txtSize, buffer);
				StringCchCopy(buffer, txtSize, text);
			}
		}
	}
	else
	{
		StringCchCopy(text, txtSize, buffer);
	}


	return true;
}


// 현재 가지고 있는 질병의 정보를 알려준다. 
bool cltHealth::GetCurDisease(cltCurDisease* pclcurdisease)
{

	bool rtnval = false;
	cltDiseaseInfo* pcldisease ;

	// 소화계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_1, pclInfo->uiOrg1Disease);
	if(pcldisease)
	{
		pclcurdisease->siOrg1Unique = pcldisease->siUnique;
		rtnval = true;
	}

	// 순환계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_2, pclInfo->uiOrg2Disease);
	if(pcldisease)
	{
		pclcurdisease->siOrg2Unique = pcldisease->siUnique;
		rtnval = true;
	}

	// 호흡계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_3, pclInfo->uiOrg3Disease);
	if(pcldisease)
	{
		pclcurdisease->siOrg3Unique = pcldisease->siUnique;
		rtnval = true;
	}

	// 신경계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_4, pclInfo->uiOrg4Disease);
	if(pcldisease)
	{
		pclcurdisease->siOrg4Unique = pcldisease->siUnique;
		rtnval = true;
	}

	return rtnval;

}

// 현재 질병의 이름을 구한다. 
bool cltHealth::GetCurDiseaseName(TCHAR* pname, SI16 txtSize)
{
	cltCurDisease cldisease;

	// 내가 갖고 있는 질병의 상태를 구한다. 
	if(GetCurDisease(&cldisease) == false)
	{
		return false;
	}

	SI32 diseaseunique = 0;

	if(cldisease.siOrg1Unique)
	{
		diseaseunique = cldisease.siOrg1Unique;
	}
	else if(cldisease.siOrg2Unique)
	{
		diseaseunique = cldisease.siOrg2Unique;
	}
	else if(cldisease.siOrg3Unique)
	{
		diseaseunique = cldisease.siOrg3Unique;
	}
	else if(cldisease.siOrg4Unique)
	{
		diseaseunique = cldisease.siOrg4Unique;
	}

	if(diseaseunique == 0)return false;

	if(pclClient->pclDiseaseManager->GetNameFromdUnique(diseaseunique, pname, txtSize) == true)
	{
		return true;
	}

	return false;
}



// 질병을 치료한다. 
bool cltHealth::CureDisease(SI32 diseaseunique)
{

	cltDiseaseInfo* pcldisease ;

	// 소화계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_1, pclInfo->uiOrg1Disease);
	if(pcldisease)
	{
		if(pcldisease->siUnique == diseaseunique)
		{
			pclInfo->uiOrg1Disease--;

			// 소화기관도 다소 회복시킨다. 
			if(pclInfo->siInnerOrg1 < 10)
			{
				pclInfo->siInnerOrg1 += 10;
			}

			// 치료 결과 질병이 없어졌다면 Disease를 0으로 설정해야 한다. 
			pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_1, pclInfo->uiOrg1Disease);
			if(pcldisease == NULL)
			{
				pclInfo->uiOrg1Disease = 0;
			}

			return true;
		}
	}

	// 순환계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_2, pclInfo->uiOrg2Disease);
	if(pcldisease)
	{
		if(pcldisease->siUnique == diseaseunique)
		{
			pclInfo->uiOrg2Disease--;

			// 기관도 다소 회복시킨다. 
			if(pclInfo->siInnerOrg2 < 10)
			{
				pclInfo->siInnerOrg2 += 10;
			}


			// 치료 결과 질병이 없어졌다면 Disease를 0으로 설정해야 한다. 
			pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_2, pclInfo->uiOrg2Disease);
			if(pcldisease == NULL)
			{
				pclInfo->uiOrg2Disease = 0;
			}

			return true;
		}
	}

	// 호흡계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_3, pclInfo->uiOrg3Disease);
	if(pcldisease)
	{
		if(pcldisease->siUnique == diseaseunique)
		{
			pclInfo->uiOrg3Disease--;

			// 기관도 다소 회복시킨다. 
			if(pclInfo->siInnerOrg2 < 10)
			{
				pclInfo->siInnerOrg2 += 10;
			}

			// 치료 결과 질병이 없어졌다면 Disease를 0으로 설정해야 한다. 
			pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_3, pclInfo->uiOrg3Disease);
			if(pcldisease == NULL)
			{
				pclInfo->uiOrg3Disease = 0;
			}

			return true;
		}
	}

	// 신경계 질병, 
	pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_4, pclInfo->uiOrg4Disease);
	if(pcldisease)
	{
		if(pcldisease->siUnique == diseaseunique)
		{
			pclInfo->uiOrg4Disease--;

			// 기관도 다소 회복시킨다. 
			if(pclInfo->siInnerOrg2 < 10)
			{
				pclInfo->siInnerOrg2 += 10;
			}

			// 치료 결과 질병이 없어졌다면 Disease를 0으로 설정해야 한다. 
			pcldisease = pclClient->pclDiseaseManager->FindDisease(ORG_4, pclInfo->uiOrg4Disease);
			if(pcldisease == NULL)
			{
				pclInfo->uiOrg4Disease = 0;
			}

			return true;
		}
	}

	return false;

}



