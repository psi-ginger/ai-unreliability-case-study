#include "..\..\Client\Client.h"
#include "..\..\Server\Server.h"

#include "Char\CharManager\CharManager.h"

#include "mytime.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;

/*cyj
//------------------------------------
// cltDate
//------------------------------------
void cltDate::Set(SI32 year, SI32 month, SI32 day, SI32 hour, SI32 minute)
{
	uiYear		= year;
	uiMonth		= month;
	uiDay		= day;
	uiHour		= hour;
	uiMinute	= minute;
	uiReserved	= 0;
}

void cltDate::Set(const cltDate* pdate)
{
	memcpy(this, pdate, sizeof(cltDate));
}

SI32 cltDate::GetDateVary()
{
	SI32 vary ;
	vary = (uiDay-1) + (uiMonth-1)*30 + (uiYear*12)*30;

	return vary;
}


// 시간 변수를 구한다. 
SI32 cltDate::GetHourVary()
{
	return  GetDateVary() * 24 + uiHour;
}

// DateVary를 가지고 완전 시간 정보를 조합한다. 
void cltDate::MakeFullDateFromDateVary(SI32 datevary)
{
	uiYear  = datevary / (12* 30);
	datevary %= (12* 30);

	uiMonth = datevary / 30 + 1;
	datevary %= 30;

	uiDay		= datevary + 1;
	uiHour		= 0;
	uiMinute	= 0;
}

SI32 cltDate::GetDay()
{
	return min(30, max(1, uiDay));
}

BOOL cltDate::IsLaterDayThan( cltDate* pclDate )
{

	UI32 a, b;
	a = uiYear * 10000 + uiMonth * 100 + uiDay;
	b = pclDate->uiYear * 10000 + pclDate->uiMonth * 100 + pclDate->uiDay;

	return ( a > b ); 
}

BOOL cltDate::IsLaterMonthThan( cltDate* pclDate )
{

	UI32 a, b;
	a = uiYear * 100 + uiMonth;
	b = pclDate->uiYear * 100 + pclDate->uiMonth;

	return ( a > b );
}


// 달을 더한다. 
void cltDate::AddMonth(SI32 addmonth)
{
	SI32 month = uiMonth;
	month += addmonth;

	SI32 addyear = ( month - 1 ) / 12;
	if(addyear)
	{
		uiYear += addyear;
		month  = (( month - 1 ) % 12) + 1;
	}

	uiMonth = (UI32)month;
}
	
BOOL cltDate::IncreaseTime(bool* pbdaychangedswitch, bool bforcetimeswitch)
{
	// 날짜가 변경되지 않았음을 설정한다. (디폴트. ) 
	*pbdaychangedswitch	= false;

	BOOL updateswitch = FALSE;

	uiMinute ++;

	if(bforcetimeswitch)
	{
		if(uiMinute >= 1)
		{
			uiMinute = 0;
			uiHour++;
			
			// 일정 시간 간간격으로 TRUE 리턴.
			if(uiHour % 2 ==0)
			{
				updateswitch = TRUE;
			}
			
			if(uiHour >= 1)
			{
				uiHour=0;
				uiDay++;
				
				// 날짜가 변경되었음을 설정한다. 
				*pbdaychangedswitch	= true;
				
				// 날짜가 변경되면 반드시 DB에 저장한다. 
				updateswitch = TRUE;
				
				if(uiDay > 30)
				{
					uiDay = 1;
					uiMonth++;
					
					if(uiMonth > 12)
					{
						uiMonth = 1;
						uiYear++;
					}
				}
			}
		}
	}
	else
	{
		
		if(uiMinute >= 60)
		{
			uiMinute = 0;
			uiHour++;
			
			// 일정 시간 간간격으로 TRUE 리턴.
			if(uiHour % 2 ==0)
			{
				updateswitch = TRUE;
			}
			
			if(uiHour >= 24)
			{
				uiHour=0;
				uiDay++;
				
				// 날짜가 변경되었음을 설정한다. 
				*pbdaychangedswitch	= true;
				
				// 날짜가 변경되면 반드시 DB에 저장한다. 
				updateswitch = TRUE;
				
				if(uiDay > 30)
				{
					uiDay = 1;
					uiMonth++;
					
					if(uiMonth > 12)
					{
						uiMonth = 1;
						uiYear++;
					}
				}
			}
		}
	}

	return updateswitch;
}

// 시간을 문자열로 만든다. 
BOOL cltDate::GetDateText(TCHAR* buffer)
{
	if(buffer == NULL)return FALSE;

	TCHAR* pText = GetTxtFromMgr(3808);
	wsprintf(buffer, pText, uiYear, uiMonth, uiDay, uiHour);

	return TRUE;
}

void cltDate::DiffDate( cltDate *pCurrentDate, cltDate *pOldDate )
{
	int temp;

	ZeroMemory( this, sizeof( cltDate ) );

	// minute
	temp = pCurrentDate->uiMinute - pOldDate->uiMinute;
	if( temp < 0 ) { uiMinute = 60 + temp; temp = -1; }
	else { uiMinute = temp; temp = 0; }

	// Hour
	temp += ( pCurrentDate->uiHour - pOldDate->uiHour );
	if( temp < 0 ) { uiHour = 24 + temp; temp = -1; }
	else { uiHour = temp; temp = 0; }

	// Day
	temp += ( pCurrentDate->uiDay - pOldDate->uiDay );
	if( temp < 0 ) { uiDay = 30 + temp; temp = -1; }
	else { uiDay = temp; temp = 0; }

	// Month
	temp += ( pCurrentDate->uiMonth - pOldDate->uiMonth );
	if( temp < 0 ) { uiMonth = 12 + temp; temp = -1; }
	else { uiMonth = temp; temp = 0; }

	// Year
	temp += ( pCurrentDate->uiYear - pOldDate->uiYear );
	uiYear = temp;
}

*/

//------------------------------------------
// cltTime
//------------------------------------------
cltTime::cltTime()
{
	bDayChangedSwitch	= false;
}


BOOL cltTime::Action(bool bforcetimeswitch)
{
	SI32 id;
	TCHAR buffer[256];
	BOOL rtnval = FALSE;

	// 날짜가 변경되었음을 초기화한다. 
	bDayChangedSwitch	= false;

	if(pclClient->GetFrame() % 40 != 0)return FALSE;

	if(clDate.IncreaseTime(&bDayChangedSwitch, bforcetimeswitch) == TRUE)
	{
		if(clDate.GetDateText(buffer) == TRUE)
		{
			pclClient->SetStatusBarText(buffer, 1);

			// 클라이언트로 현재 시간을 보낸다. 
			SI32 index = 0;
			while(id = pclClient->pclCM->GetCharOrder(index))
			{
				index++;

				SI32 charunique	=	pclClient->pclCM->CR[id]->GetCharUnique();
				
				// 사용자인 컴에게만 전달한다. 
				if(pclClient->pclCM->CR[id]->IsValidConnection() == true)
				{
					cltMsg clMsg(GAMEMSG_RESPONSE_DATE, sizeof(cltDate), (BYTE*)(BYTE*)&clDate);
					
					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					
				}

			}
		}

		rtnval = TRUE;
	}


	// 시간을 DB에 저장한다. 
	((cltServer*)pclClient)->UpdateDBTime(&clDate);


	return rtnval;
}
