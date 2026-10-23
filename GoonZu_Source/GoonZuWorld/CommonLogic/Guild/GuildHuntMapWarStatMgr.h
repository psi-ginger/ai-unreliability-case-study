#ifndef __GUILDHUNTMAPWARSTATMGR_H__
#define __GUILDHUNTMAPWARSTATMGR_H__

#include "guild.h"
#include "CommonHeader.h"
#include "../../Client/client.h"


class cltGuildHuntMapWarStatMgr
{
public:
	
	//	Home/Away 길드 이름
	enum GuildHomeAway
	{
		GUILDTEAM_HOME,		//	홈팀
		GUILDTEAM_AWAY,		//	어웨이
		GUILDTEAM_END,		
	};

	enum GuildHuntMapDrawFlag
	{
		DRAWFLAG_NONE,	
		DRAWFLAG_START,	
		DRAWFLAG_WIN,	
		DRAWFLAG_LOSE,	
		DRAWFLAG_DRAW,	
		DRAWFLAG_END,	
	};

	TCHAR		m_szGuildName[GUILDTEAM_END][MAX_GUILD_NAME];				//	길드 이름

	SI32		m_siBGP[GUILDTEAM_END];										//	BGP (battle guild point)
	SI32		m_siGuildPoint[GUILDTEAM_END];								//	길드 포인트
	SI16		m_siGuildMarkIndex[GUILDTEAM_END];							//	길드 마크
   
	TCHAR		m_siKillUserName[MAX_PLAYER_NAME];							//	죽인 유저
	TCHAR		m_siDeathUserName[MAX_PLAYER_NAME];							//	죽은 유저

	bool		m_bGuildWarSwitch;											//	현재 사냥터획득 길드전인가

	SI32		m_siDrawFlag;												//	Draw상태
	clock_t		m_noticeClock;												//	시간관련 체크 변수
	clock_t		m_endClock;													//	길드전 종료 시간
	SI32		m_siResult;													//	승패?



public:
	cltGuildHuntMapWarStatMgr();
	~cltGuildHuntMapWarStatMgr();

	void	Init();		

	void	StartGuildWar(TCHAR* homename, TCHAR* awayname, SI16 homemark, SI16 awaymark);	//	사냥터획득 길드전 시작
	void	EndGuildWar(SI32 recvGP, SI32 siTeamIndex, TCHAR* szPlayerName, SI32  siKillNum, SI32  siDeathNum);														//	길드전 종료
	
	
	void	SetGuildName(SI32 siTeamIndex, TCHAR* szGuildName);								//	길드 이름 설정
	TCHAR*	GetGuildName(SI32 siTeamIndex){return m_szGuildName[siTeamIndex];}				


	void	SetGuildMarkIndex(SI32 siTeamIndex, SI16 siGuildMarkIndex);						//	길드 마크 인덱스 설정
	SI16	GetGuildMarkIndex(SI32 siTeamIndex){return m_siGuildMarkIndex[siTeamIndex];}						


	void	SetBGP(SI32 siTeamIndex, SI32 siBGP);											//	BGP 설정
	SI32	GetBGP(SI32 siTeamIndex){return m_siBGP[siTeamIndex];}							


	void	SetPersonKillInfo(TCHAR* szKillUserName, TCHAR* szDeathUserName, SI32 siTeamIndex);		//	개개인의 킬데스 정보
	
	void	SetGuildPoint(SI32 siTeamIndex);												//	길드 포인트 설정
	SI32	GetGuildPoint(SI32 siTeamIndex){return m_siGuildPoint[siTeamIndex];}			

	void	SetDrawFlag(SI32 siDrawFlag);													// 드로우플래그 설정
	SI32	GetDrawFlag(){return m_siDrawFlag;}		

	bool	IsGuildHuntMapWar(){return m_bGuildWarSwitch;}									//	지금 길드전 중?

	SI32	GetResult(){return m_siResult;}													//	승 무 패 결과
    
	void	DrawStatus(LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry);

};


#endif