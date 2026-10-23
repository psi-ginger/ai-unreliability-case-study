#ifndef _GMSECTIONDATA_H
#define _GMSECTIONDATA_H

#include <stdarg.h>

class GMSectionData
{
public:
	GMSectionData( int iBufferSize );
	~GMSectionData();

	void	ClearData();
	bool	BeginData( char* name );
	bool	AddDataInt( char* name, int value );
	bool	AddDataInt( char* name, __int64 value );
	bool	AddDatafloat( char* name, float value );
	bool	AddDataStr( char* name, char* value );
	bool	AddDataFormat( char* name, const char* format, ... );
	bool	EndData( char* name );
	
	char*	m_pBuffer;
	int		m_iBufferSize;
	int		m_iCurPos;

	bool	AddSectionData(GMSectionData* pSectionData);
};




#endif