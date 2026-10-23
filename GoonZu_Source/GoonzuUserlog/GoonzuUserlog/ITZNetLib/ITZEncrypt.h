#ifndef _ITZENCRYPT_H
#define _ITZENCRYPT_H

#include "itzbase.h"


class ITZEncrypt
{
public:
	ITZEncrypt();
	~ITZEncrypt();

public:
	void			SetKey( UI08 cKey1, UI08 cKey2 );

	void			Encode( UI08 *pSrc, UI08 *pDest );
	BOOL			Decode( UI08 *pSrc, UI08 *pDest );


private:
	UI08			m_ucKey1;
	UI08			m_ucKey2;

	UI16			local_usSize;

	UI08			local_cProcessKey1;
	UI08			local_cProcessKey2;

	UI08			*local_pCur;
	UI08			*local_pEnd;
};

#endif

