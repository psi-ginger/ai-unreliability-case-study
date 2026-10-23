#include "Weather.h"

CWeather::CWeather()
{	
	m_si08WeatherStatus = -1;
	m_si08WindStatus = -1;
	m_si08WindSpeed = -1;

	memset( &m_stWeatherData, 0, sizeof ( stWeatherData ) );

	Action( timeGetTime() );
}

CWeather::~CWeather()
{

}

void CWeather::ChangeWeatherStatus()
{
	if ( rand() % WEAHTER_RAIN_PERCENTAGE == 0 )
	{
		m_si08WeatherStatus = WEATHER_RAIN;
		
		/*
		if ( rand() % THUNDER_PERCENTAGE == 0 )
		{
			m_bThunderStatus = true;
		}
		else
		{
			m_bThunderStatus = false;
		}
		*/
	}
	else
	{
		m_si08WeatherStatus = WEATHER_FINE;
		//m_bThunderStatus = false;
	}
	
	return;
}

void CWeather::ChangeWindStatus()
{
	m_si08WindStatus = rand() % ( WIND_SOUTHEAST + 1 );
	return;
}

void CWeather::ChangeWindSpeed()
{
	m_si08WindSpeed = rand() % MAX_WEATHER_WIND_SPEED;
	return;
}

bool CWeather::Action( DWORD dwCurrentTime )
{
	//-------------------------------------------------------------------------------------------------
	//-------------날씨 변경 상태 확인부
	if ( m_si08WeatherStatus < 0 || m_si08WindStatus < 0 || m_si08WindSpeed < 0 )
	{
		m_dwWeatherStartTime = dwCurrentTime;
		m_dwWindStartTime = dwCurrentTime;
		m_dwWindSpeedStartTime = dwCurrentTime;

		ChangeWeatherStatus();
		ChangeWindStatus();
		ChangeWindSpeed();
		return true;
	}
	else if ( dwCurrentTime > m_dwWeatherStartTime + CHANGE_WEATHER_DELAY_TIME )
	{
		m_dwWeatherStartTime = dwCurrentTime;
		ChangeWeatherStatus();
		return true;
	}
	else if ( dwCurrentTime > m_dwWindStartTime + CHANGE_WIND_DELAY_TIME )
	{
		m_dwWindStartTime = dwCurrentTime;		
		ChangeWindStatus();		
		return true;
	}
	else if ( dwCurrentTime > m_dwWindSpeedStartTime + CHANGE_WIND_SPEED_DELAY_TIME )
	{
		m_dwWindSpeedStartTime = dwCurrentTime;
		ChangeWindSpeed();
		return true;
	}
	
	return false;
}

bool CWeather::GetWeatherData( stWeatherData *pWeatherData )
{
	if ( m_si08WeatherStatus < 0 || m_si08WindStatus < 0 || m_si08WindSpeed < 0 )
	{
		return false;
	}

	m_stWeatherData.m_si08WeatherStatus = m_si08WeatherStatus;
	m_stWeatherData.m_si08WindDirection = m_si08WindStatus;
	m_stWeatherData.m_si08WindSpeed = m_si08WindSpeed;

	memcpy( pWeatherData, &m_stWeatherData, sizeof( stWeatherData ) );
	
	pWeatherData = &m_stWeatherData;

	return true;
}