#include "../../common/CommonHeader.h"
#include "../../common/JWLib/IsCorrectID/IsCorrectID.h"

#include "WorldPassport.h"

void cltWorldPassport::Set( cltWorldPassport* pclWorldPassport )
{
	if ( NULL == pclWorldPassport )
	{
		return;
	}

	memcpy( this, pclWorldPassport, sizeof(cltWorldPassport) );

}

void cltWorldPassport::Set( cltWorldPassportForDB* pclWorldPassportForDB )
{
	if ( NULL == pclWorldPassportForDB )
	{
		return;
	}

	if ( pclWorldPassportForDB->m_szPassportName )
	{
		StringCchCopy( m_szPassportName, sizeof(m_szPassportName), pclWorldPassportForDB->m_szPassportName );
	}

	m_siIssueDateVary	= pclWorldPassportForDB->m_siIssueDateVary;
	m_siExpireDateVary	= pclWorldPassportForDB->m_siExpireDateVary;
}

void cltWorldPassport::Set( TCHAR* pszPassportName, SI32 siIssueDateVary, SI32 siExpireDateVary )
{
	if ( pszPassportName )
	{
		StringCchCopy( m_szPassportName, sizeof(m_szPassportName), pszPassportName );
	}

	m_siIssueDateVary	= siIssueDateVary;
	m_siExpireDateVary	= siExpireDateVary;
}

TCHAR* cltWorldPassport::GetPassportName( void )
{
	return m_szPassportName;
}

void cltWorldPassport::SetPassportName( TCHAR* pszPassportName )
{
	if ( pszPassportName )
	{
		StringCchCopy( m_szPassportName, sizeof(m_szPassportName), pszPassportName );
	}
}

SI32 cltWorldPassport::GetIssueDateVary( void )
{
	return m_siIssueDateVary;
}

void cltWorldPassport::SetIssueDateVary( SI32 siIssueDateVary )
{
	m_siIssueDateVary = siIssueDateVary;
}

SI32 cltWorldPassport::GetExpireDateVary( void )
{
	return m_siExpireDateVary;
}

void cltWorldPassport::SetExpireDateVary( SI32 siExpireDateVary )
{
	m_siExpireDateVary = siExpireDateVary;
}

void cltWorldPassport::GetIssueRealDateText( TCHAR* pszBuffer, SI32 siSize )
{
	if ( (NULL == pszBuffer) || (0 >= siSize) )
	{
		return;
	}

	cltDate clIssueDate;
	clIssueDate.MakeFullDateFromDateVary( m_siIssueDateVary );

	clIssueDate.GetRealDateText( pszBuffer );
}

void cltWorldPassport::GetExpireRealDateText( TCHAR* pszBuffer, SI32 siSize )
{
	if ( (NULL == pszBuffer) || (0 >= siSize) )
	{
		return;
	}

	cltDate clExpireDate;
	clExpireDate.MakeFullDateFromDateVary( m_siExpireDateVary );

	clExpireDate.GetRealDateText( pszBuffer );
}

bool cltWorldPassport::IsCorrectPassportName( TCHAR* pszPassportName )
{
	if ( NULL == pszPassportName )
	{
		return false;
	}

	WCHAR	wszID[ Const_Max_WorldPassport_Name ]	= { 0, };
#ifdef _UNICODE
	StringCchCopy( wszID, Const_Max_WorldPassport_Name, pszPassportName );
#else
	MultiByteToWideChar( CP_ACP, MB_COMPOSITE, pszPassportName, -1, wszID, 32 );
#endif

	SI32 siLength = (SI32)wcslen( wszID );
	if ( 0 >= siLength )
	{
		return false;
	}


	for( SI32 siCount=0; siCount<siLength; ++siCount )
	{
		if ( TRUE == IsAlphabet(wszID[siCount]) )
		{
			continue;
		}
		else if ( TRUE == IsNumber(wszID[siCount]) )
		{
			continue;
		}

		return false;
	}

	return true;
}

bool cltWorldPassport::IsHavePassport( void )
{
	// 결과적으로는 이름,발행일,만료일이 설정이 되어있어야 여권을 갖고 있는 것이다
	// 이름 검사
	if ( _tcscmp(m_szPassportName, TEXT("")) == 0 )
	{
		return false;
	}

	// 발행일
	if ( 0 >= m_siIssueDateVary )
	{
		return false;
	}

	// 만료기간
	if ( 0 >= m_siExpireDateVary )
	{
		return false;
	}

	return true;
}