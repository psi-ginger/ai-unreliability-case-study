#ifndef _WEATHER_H_
#define _WEATHER_H_

#include <Directives.h>


#define CHANGE_WEATHER_DELAY_TIME		60000			// 날씨
#define CHANGE_WIND_DELAY_TIME			50000			// 풍향
#define CHANGE_WIND_SPEED_DELAY_TIME	30000			// 풍속

#define	WEAHTER_RAIN_PERCENTAGE			30
#define	THUNDER_PERCENTAGE				50

#define MAX_WEATHER_WIND_SPEED			10

//enum { SEASON_SPRING, SEASON_SUMMER, SEASON_FALL, SEASON_WINTER };				// 계절은 빠짐.
enum { WEATHER_FINE, WEATHER_RAIN };
enum { WIND_SOUTH, WIND_SOUTHWEST, WIND_WEST, WIND_NORTHWEST, WIND_NORTH, WIND_NORTHEAST, WIND_EAST, WIND_SOUTHEAST };

struct stWeatherData
{
	SI08 m_si08WeatherStatus;
	SI08 m_si08WindDirection;
	SI08 m_si08WindSpeed;

	void Set( stWeatherData *pWeatherData )
	{
		memcpy( this, pWeatherData , sizeof( stWeatherData ) );
	}
};

class CWeather
{
public:
	CWeather();
	~CWeather();

	void	ChangeWeatherStatus();
	void	ChangeWindStatus();
	void	ChangeWindSpeed();
	bool	Action( DWORD dwCurrentTime );			// 이 함수는 시간 간격을 좀 두고 호출되어야 할 듯... 계속 호출될 필요는 없는데... 
	
	//-------------------------------------------------------------------------------------------------
	//-------------- 현재 날씨 정보를 얻어오는 함수
	bool	GetWeatherData( stWeatherData *pWeatherData );
	//-------------------------------------------------------------------------------------------------

private:

	stWeatherData m_stWeatherData;
	//bool	m_bThunderStatus;
	SI08	m_si08WeatherStatus;
	SI08	m_si08WindStatus;
	SI08	m_si08WindSpeed;
	//SI08	m_si08SeasonStatus;

	DWORD	m_dwWeatherStartTime;
	DWORD	m_dwWindStartTime;
	DWORD	m_dwWindSpeedStartTime;
};

#endif