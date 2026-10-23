#pragma once 

#include <Directives.h>
#include "..\..\Common\CommonHeader.h"


#define MAX_KOINOBORYFAMILY_MAKE_POS	42

enum KOINOBORYFAMILY_KIND
{
	KOINOBORYFAMILY_KIND_NONE	= -1,

	KOINOBORYFAMILY_KIND_YELLOW	= 0,
	KOINOBORYFAMILY_KIND_RED,
	KOINOBORYFAMILY_KIND_BLUE,

	KOINOBORYFAMILY_KIND_MAX
};

class cltKoinoboryFamilyPosition
{
private:
	// 코이노보리 출몰 위치
	cltCharPos	m_clPos[MAX_KOINOBORYFAMILY_MAKE_POS];

	// 코이노보리 출몰 위치를 렌덤하게 갖고 오게 하기위한 변수
	// 위에 포지션객체를 랜덤하게 하는것보다 속도의 이득을 보기위함
	SI32		m_siRandIndex[MAX_KOINOBORYFAMILY_MAKE_POS];

public:
	cltKoinoboryFamilyPosition( void );
	~cltKoinoboryFamilyPosition( void );

	void		Init( void );

public:
	void		SetPosition( void );
	void		RandomMixUpIndex( void );
	
	cltCharPos*	GetRandomPosition( const SI32 siIndex );

};

class cltKoinoboryFamilyManager
{
public:
	cltKoinoboryFamilyManager();
	~cltKoinoboryFamilyManager();

	void Action();

	void StartEvent();
	void EndEvent();

public:
	//	175	KIND_KOINOBORYYELLOW	코이노보리 - 노랑
	//	176	KIND_KOINOBORYRED		코이노보리 - 빨강
	//	177	KIND_KOINOBORYBLUE		코이노보리 - 파랑
	SI32 GetKoinoboryYellowKind();
	SI32 GetKoinoboryRedKind();
	SI32 GetKoinoboryBlueKind();

private:
	void MakeKoinoboryFamily( void );
	void NoticeKoinoboryFamily( void );

	bool CreateKoinobory( const SI32 siIndex );
	bool DeleteKoinobory( const SI32 siIndex );

	SI32 GetKoinoboryKind( const SI32 siIndex );


private:
	SI16						m_siStartHour;

	SI32						m_siKonoboryIndex[KOINOBORYFAMILY_KIND_MAX];

	cltKoinoboryFamilyPosition	m_clPos;

};

