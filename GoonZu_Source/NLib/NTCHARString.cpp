//-----------------------------------------------------------------------------
// 2007/12/14 ÀÌ°­Çü
//-----------------------------------------------------------------------------

#include "NTCHARString.h"

NTCHARString64 SI64ToString( SI64 siVal, SI32 siRadix /* = 10 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	_i64tot(siVal, szTmp, siRadix);
	Ret = szTmp;

	return Ret;
}

NTCHARString64 UI64ToString( UI64 uiVal, SI32 siRadix /* = 10 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	_ui64tot(uiVal, szTmp, siRadix);
	Ret = szTmp;

	return Ret;
}

NTCHARString64 SI32ToString( SI32 siVal, SI32 siRadix /* = 10 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	_itot(siVal, szTmp, siRadix);
	Ret = szTmp;

	return Ret;
}

NTCHARString64 UI32ToString( UI32 uiVal, SI32 siRadix /* = 10 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	_itot(uiVal, szTmp, siRadix);
	Ret = szTmp;

	return Ret;
}

NTCHARString64 SI16ToString( SI16 siVal, SI32 siRadix /* = 10 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	_itot(siVal, szTmp, siRadix);
	Ret = szTmp;

	return Ret;
}

NTCHARString64 UI16ToString( UI16 uiVal, SI32 siRadix /* = 10 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	_itot(uiVal, szTmp, siRadix);
	Ret = szTmp;

	return Ret;
}

NTCHARString64 SI08ToString( SI08 siVal, SI32 siRadix /* = 10 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	_itot(siVal, szTmp, siRadix);
	Ret = szTmp;

	return Ret;
}

NTCHARString64 UI08ToString( UI08 uiVal, SI32 siRadix /* = 10 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	_itot(uiVal, szTmp, siRadix);
	Ret = szTmp;

	return Ret;
}

NTCHARString64 REAL32ToString( REAL32 fVal, SI32 siNumber /* = -1 */, SI32 siDecimal /* = 2 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	TCHAR			szFormat[64] = { '\0', };

	if(siNumber > 0)
		StringCchPrintf(szFormat, sizeof(szFormat)-1, TEXT("%%.%df"), siDecimal);
	else
		StringCchPrintf(szFormat, sizeof(szFormat)-1, TEXT("%%%d.%df"), siNumber, siDecimal);

	StringCchPrintf(szTmp, sizeof(szTmp)-1, szFormat, fVal);
	szTmp[sizeof(szTmp) - 1] = '\0';

	Ret = szTmp;

	return Ret;
}

NTCHARString64 REAL64ToString( REAL64 fVal, SI32 siNumber /* = -1 */, SI32 siDecimal /* = 2 */ )
{
	NTCHARString64	Ret;
	TCHAR			szTmp[64] = { '\0', };
	TCHAR			szFormat[64] = { '\0', };

	if(siNumber > 0)
		StringCchPrintf(szFormat, sizeof(szFormat)-1, TEXT("%%.%df"), siDecimal);
	else
		StringCchPrintf(szFormat, sizeof(szFormat)-1, TEXT("%%%d.%df"), siNumber, siDecimal);

	StringCchPrintf(szTmp, sizeof(szTmp)-1, szFormat, fVal);
	szTmp[sizeof(szTmp) - 1] = '\0';

	Ret = szTmp;

	return Ret;
}


NTCHARStringBuilder& NTCHARStringBuilder::SetFormat(const TCHAR* pszFormat)
{
	if(pszFormat == NULL)		return *this;
	m_siCurParam = 0;

	m_kStr = pszFormat;

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (const TCHAR* pszParam)
{
	if(pszParam == NULL)		return *this;
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, pszParam);

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (TCHAR* pszParam)
{
	if(pszParam == NULL)		return *this;
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, pszParam);

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (SI32 siVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, SI32ToString(siVal));

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (UI32 uiVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, UI32ToString(uiVal));

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (SI16 siVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, SI16ToString(siVal));

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (UI16 uiVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, UI16ToString(uiVal));

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (SI08 siVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, SI08ToString(siVal));

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (UI08 uiVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, UI08ToString(uiVal));

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (SI64 siVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, SI64ToString(siVal));

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (UI64 uiVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, UI64ToString(uiVal));

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (REAL32 fVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, REAL32ToString(fVal));

	return *this;
}

NTCHARStringBuilder& NTCHARStringBuilder::operator<< (REAL64 fVal)
{
	m_siCurParam++;

	NTCHARString32	kTmp;
	kTmp += "{";
	kTmp += SI32ToString(m_siCurParam);
	kTmp += "}";

	m_kStr.Replace(kTmp, REAL64ToString(fVal));

	return *this;
}
