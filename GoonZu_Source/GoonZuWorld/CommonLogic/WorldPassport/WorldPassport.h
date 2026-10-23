#pragma once

#include <Directives.h>

const SI32 Const_Max_WorldPassport_Name = 20;

// DB패킷용 Class
// DB에서 데이터를 받아올 때만 사용한다
class cltWorldPassportForDB
{
public:
	// 여권이름
	TCHAR	m_szPassportName[Const_Max_WorldPassport_Name];
	// 발행일자
	SI32	m_siIssueDateVary;
	// 만료일
	SI32	m_siExpireDateVary;

	cltWorldPassportForDB()
	{
		ZeroMemory( this, sizeof(cltWorldPassportForDB) );
	}
	~cltWorldPassportForDB()
	{
	}
};


// 무역관련 여권
class cltWorldPassport
{
//== Variable Area Start ==
private:
	// 여권이름
	TCHAR	m_szPassportName[Const_Max_WorldPassport_Name];
	// 발행일자
	SI32	m_siIssueDateVary;
	// 만료일
	SI32	m_siExpireDateVary;

public:
//== Variable Area End ==

//== Function Area Start ==
private:
public:
	cltWorldPassport()
	{
		Init();
	}

	~cltWorldPassport()
	{
	}

	void	Init( void )
	{
		ZeroMemory( this, sizeof(cltWorldPassport) );
	}

	void	Set( cltWorldPassport* pclWorldPassport );
	void	Set( cltWorldPassportForDB* pclWorldPassportForDB );
	void	Set( TCHAR* pszPassportName, SI32 siIssueDateVary, SI32 siExpireDateVary );

	TCHAR*	GetPassportName( void );
	void	SetPassportName( TCHAR* pszPassportName );

	SI32	GetIssueDateVary( void );
	void	SetIssueDateVary( SI32 siIssueDateVary );

	SI32	GetExpireDateVary( void );
	void	SetExpireDateVary( SI32 siExpireDateVary );

	// X년 X월 X일 형식의 문자열을 넘겨준다
	void	GetIssueRealDateText( TCHAR* pszBuffer, SI32 siSize );
	// X년 X월 X일 형식의 문자열을 넘겨준다
	void	GetExpireRealDateText( TCHAR* pszBuffer, SI32 siSize );

	// 여권 이름이 유효한 이름인지 체크(영문인지 아닌지)
	// 내부에 있는 것을 체크하는 것이 아니고 인수로 들어온 것을 체크하는 것이다
	bool	IsCorrectPassportName( TCHAR* pszPassportName );
	bool	IsHavePassport( void );


//== Function Area End ==

};

