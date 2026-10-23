#pragma once

#include "NDataTypes.h"
#include "TSpr.h"
#include <time.h>


#define MAX_EMOTICON_ANISTEP	7
#define MAX_EMOTICON_NUMBER		50

class CEmoticonInfo
{
public:
	CEmoticonInfo();
	~CEmoticonInfo();

	void	Initialize();
	void	Destroy();

	void	LoadEmoticonInfo();

	SI16	GetStartFrame( const SI16 index ) const;
	SI16	GetAniStepNumber( const SI16 index ) const;
	SI16	GetLastAniDelay( const SI16 index) const;
	SI16	GetEmoticonCount();
	TCHAR*	GetCode( const SI16 index );
	TCHAR*	GetInfo( const SI16 index );

private:
	TCHAR	m_szInfo[MAX_EMOTICON_NUMBER][15];
	TCHAR	m_szCode[MAX_EMOTICON_NUMBER][4];
	SI16	m_siStartFrame[MAX_EMOTICON_NUMBER];
	SI16	m_siAniStepNumber[MAX_EMOTICON_NUMBER];			// 프레임 갯수
	SI16	m_siLastAniDelay[MAX_EMOTICON_NUMBER];			// 마지막 프레임의 딜레이여부
};

class CEmoticonEffect
{
public:
	CEmoticonEffect( TSpr* pEmoticonSpr );
	~CEmoticonEffect();

	void	Initialize();
	void	Destroy();

	void	Set( SI16 emoticonUnique );
	void	Action();
	void	Draw( SI32 siCenterXPos, SI32 siCenterYPos );

private:
	TSpr	*m_pEmoticonSPR;

	SI32	m_siCurrentDrawFrame;
	SI32	m_siLastFrame;

	SI32	m_siDelayCount;

	bool	m_bActive;

	clock_t	m_EmoticonClock;
};
