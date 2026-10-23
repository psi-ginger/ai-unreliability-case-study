#ifndef _NBYTECRYPTO_H
#define _NBYTECRYPTO_H

class NByteCrypto
{
public:
	NByteCrypto();
	~NByteCrypto();

	void			OpenFile( TCHAR *filename );
	void			SaveFile( TCHAR *filename );

	unsigned char*	m_pBuffer;

	int				m_iFileSize;
};

#endif