//-----------------------------------------------------------------------------
// 2005/09/13 이제완
//-----------------------------------------------------------------------------

#ifndef _NDATALOADER_H
#define _NDATALOADER_H

#include "NDataTypes.h"

struct NDataTypeInfo
{
	int		datatype;
	void*	pVar;
	int		size;
};

enum NDATATYPE
{
	NDATA_NONE = 0,
	NDATA_INT32,
	NDATA_INT64,
	NDATA_FLOAT,	
	NDATA_DOUBLE,
	NDATA_MBSTRING,
	NDATA_WCSTRING,
	NDATA_END
};

#define NDATA_MAX_COLUMN		64

class NDataLoader
{
public:
	NDataLoader();
	~NDataLoader();

	bool			LoadDataFromFile( TCHAR *filename );
	bool			LoadDataFromCryptFile( TCHAR *filename );
	bool			LoadDataFromMem( unsigned char *pBuffer, int size );
	void			RemoveData();
	
	void			RegDataType( NDataTypeInfo *pDataTypeInfo );

	bool			IsEndOfData();

	void			SkipRow( int rowcount );
	void			ReadyToRead();

	bool			ReadData(bool bCheckColumnCount = false);

	NDataTypeInfo	m_pDataTypeInfo[ NDATA_MAX_COLUMN ];
	int				m_iColumnNum;

	BOOL			m_bDataSection;

	WCHAR*			m_pBuffer;

	WCHAR*			m_pStartPos;
	WCHAR*			m_pCurPos;
	WCHAR*			m_pEndPos;

	TCHAR			m_szFileName[ MAX_PATH ];

	// 변환 함수 포인터
	class CFuncPtr
	{
	public:
		void	(NDataLoader::*Func)( WCHAR *wstr, void *pVar, int size );
	};

	CFuncPtr		m_pFuncPtr[ NDATA_END ];

	void			ConvertToInt32( WCHAR *str, void *pVar, int size );
	void			ConvertToInt64( WCHAR *str, void *pVar, int size );
	void			ConvertToFloat( WCHAR *str, void *pVar, int size );
	void			ConvertToDouble( WCHAR *str, void *pVar, int size );
	void			ConvertToMBString( WCHAR *str, void *pVar, int size );
	void			ConvertToWCString( WCHAR *str, void *pVar, int size );
};

class NDataAtbParser
{
public:
	NDataAtbParser();
	~NDataAtbParser();

	void AddAtbInfo( TCHAR *szString, DWORD dwValue );
	DWORD GetValueByString( TCHAR *szString );

	TCHAR		m_szString[ 32 ][ 64 ];
	DWORD		m_dwValue[ 32 ];

	int			m_iNumber;
};
/*
class NDataPointParser 
{
public:
	NDataPointParser();
	~NDataPointParser();


};
*/
#endif