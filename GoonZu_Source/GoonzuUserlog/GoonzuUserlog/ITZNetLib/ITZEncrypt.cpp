#include "ITZEncrypt.h"


ITZEncrypt::ITZEncrypt()
{

}

ITZEncrypt::~ITZEncrypt()
{
	
}

void ITZEncrypt::SetKey( UI08 ucKey1, UI08 ucKey2 )
{
	m_ucKey1 = ucKey1;
	m_ucKey2 = ucKey2;
}


void ITZEncrypt::Encode( IN UI08 *pSrc, OUT UI08 *pDest )
{
 
	m_ucKey1++; 
	m_ucKey2++;

	local_usSize = *(UI16 *)pSrc;

	if( local_usSize <= 3 ) {

		memcpy( pDest, pSrc, local_usSize );
		return;
	}

	pDest[ 2 ] = pSrc[ local_usSize - 1 ] + m_ucKey1;

	local_cProcessKey1 = m_ucKey1;
	local_cProcessKey2 = m_ucKey2;

	local_pEnd = (UI08*)( pSrc + local_usSize );

	*(UI16 *)pDest = local_usSize;

	pDest += 2;

	for( pSrc += 3;	pSrc < local_pEnd; ++pSrc ) {

		++pDest;
 
		local_cProcessKey1 = local_cProcessKey1 ^ *pSrc;

		*pDest = local_cProcessKey1 + local_cProcessKey2;

		local_cProcessKey2 = local_cProcessKey2 + *pSrc;

	}

}

BOOL ITZEncrypt::Decode( IN UI08 *pSrc, OUT UI08 *pDest )
{

	m_ucKey1++;
	m_ucKey2++;

	local_usSize = *(UI16 *)pSrc;

	if( local_usSize > PACKET_MAX_SIZE ) return FALSE;

	if( local_usSize <= 3 ) {

		memcpy( pDest, pSrc, local_usSize );
		return TRUE;
	}

	pSrc[ 2 ] = pSrc[ 2 ] - m_ucKey1;

	local_cProcessKey1 = m_ucKey1;
	local_cProcessKey2 = m_ucKey2;

	local_pEnd = (UI08 *)( pSrc + local_usSize );

	local_pCur = pSrc;

	*(UI16 *)pDest = local_usSize;
	
	pDest += 2;

	for( pSrc += 3; pSrc < local_pEnd; ++pSrc ) {

		++pDest;

		*pDest = *pSrc - local_cProcessKey2;

		*pDest = *pDest ^ local_cProcessKey1;

		local_cProcessKey1 = local_cProcessKey1 ^ *pDest;

		local_cProcessKey2 = local_cProcessKey2 + *pDest;
	}

	if( local_pCur[ 2 ] == *pDest ) return TRUE;

	return FALSE;
}
