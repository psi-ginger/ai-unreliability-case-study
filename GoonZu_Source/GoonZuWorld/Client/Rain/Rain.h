#ifndef _RAIN_H_
#define _RAIN_H_

#include <Directives.h>
#include "TSpr.h"

enum { RAIN_STATUS, NOT_RAIN_STATUS };

class CRain
{
public:
	CRain( TSpr *pXSPR, SI32 siWidth, SI32 siHeight );
	~CRain();

	void	Initialize();
	void	Destroy();

	void	SetAction( DWORD dwDelayTime );
	void	Action( DWORD dwCurrentTime );
	void	Draw();

private:
	TSpr	*m_pRainSPR;

	DWORD	m_dwStartTime;
	DWORD	m_dwDelayTime;

	bool	m_bRainStatus;

	SI32	m_siWidth;
	SI32	m_siHeight;
};

#endif