#include "CRC32.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Name		: CCRC32
//
// Day		: 2007.12.27
//
// Comment	: CRC32코드를 리턴하고 인수로 넣은 변수에 파일 용량(총 읽은 바이트)을 보낸다.
///////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned long CCRC32::m_ulCRCTable[MAX_CRC_TABLE_NUM] = {0x00000000,
	0x02000000, 0x04000000, 0x06000000, 0x08000000, 0x0A000000,
	0x0C000000, 0x0E000000, 0x10000000, 0x12000000, 0x14000000,
	0x16000000, 0x18000000, 0x1A000000, 0x1C000000, 0x1E000000,
	0x20000000, 0x22000000, 0x24000000, 0x26000000, 0x28000000,
	0x2A000000, 0x2C000000, 0x2E000000, 0x30000000, 0x32000000,
	0x34000000, 0x36000000, 0x38000000, 0x3A000000, 0x3C000000,
	0x3E000000, 0x40000000, 0x42000000, 0x44000000, 0x46000000,
	0x48000000, 0x4A000000, 0x4C000000, 0x4E000000, 0x50000000,
	0x52000000, 0x54000000, 0x56000000, 0x58000000, 0x5A000000,
	0x5C000000, 0x5E000000, 0x60000000, 0x62000000, 0x64000000,
	0x66000000, 0x68000000, 0x6A000000, 0x6C000000, 0x6E000000,
	0x70000000, 0x72000000, 0x74000000, 0x76000000, 0x78000000,
	0x7A000000, 0x7C000000, 0x7E000000, 0x80000000, 0x82000000,
	0x84000000, 0x86000000, 0x88000000, 0x8A000000, 0x8C000000,
	0x8E000000, 0x90000000, 0x92000000, 0x94000000, 0x96000000,
	0x98000000, 0x9A000000, 0x9C000000, 0x9E000000, 0xA0000000,
	0xA2000000, 0xA4000000, 0xA6000000, 0xA8000000, 0xAA000000,
	0xAC000000, 0xAE000000, 0xB0000000, 0xB2000000, 0xB4000000,
	0xB6000000, 0xB8000000, 0xBA000000, 0xBC000000, 0xBE000000,
	0xC0000000, 0xC2000000, 0xC4000000, 0xC6000000, 0xC8000000,
	0xCA000000, 0xCC000000, 0xCE000000, 0xD0000000, 0xD2000000,
	0xD4000000, 0xD6000000, 0xD8000000, 0xDA000000, 0xDC000000,
	0xDE000000, 0xE0000000, 0xE2000000, 0xE4000000, 0xE6000000,
	0xE8000000, 0xEA000000, 0xEC000000, 0xEE000000, 0xF0000000,
	0xF2000000, 0xF4000000, 0xF6000000, 0xF8000000, 0xFA000000,
	0xFC000000, 0xFE000000, 0xFFFFFFFF, 0xFDFFFFFF, 0xFBFFFFFF,
	0xF9FFFFFF, 0xF7FFFFFF, 0xF5FFFFFF, 0xF3FFFFFF, 0xF1FFFFFF,
	0xEFFFFFFF, 0xEDFFFFFF, 0xEBFFFFFF, 0xE9FFFFFF, 0xE7FFFFFF,
	0xE5FFFFFF, 0xE3FFFFFF, 0xE1FFFFFF, 0xDFFFFFFF, 0xDDFFFFFF,
	0xDBFFFFFF, 0xD9FFFFFF, 0xD7FFFFFF, 0xD5FFFFFF, 0xD3FFFFFF,
	0xD1FFFFFF, 0xCFFFFFFF, 0xCDFFFFFF, 0xCBFFFFFF, 0xC9FFFFFF,
	0xC7FFFFFF, 0xC5FFFFFF, 0xC3FFFFFF, 0xC1FFFFFF, 0xBFFFFFFF,
	0xBDFFFFFF, 0xBBFFFFFF, 0xB9FFFFFF, 0xB7FFFFFF, 0xB5FFFFFF,
	0xB3FFFFFF, 0xB1FFFFFF, 0xAFFFFFFF, 0xADFFFFFF, 0xABFFFFFF,
	0xA9FFFFFF, 0xA7FFFFFF, 0xA5FFFFFF, 0xA3FFFFFF, 0xA1FFFFFF,
	0x9FFFFFFF, 0x9DFFFFFF, 0x9BFFFFFF, 0x99FFFFFF, 0x97FFFFFF,
	0x95FFFFFF, 0x93FFFFFF, 0x91FFFFFF, 0x8FFFFFFF, 0x8DFFFFFF,
	0x8BFFFFFF, 0x89FFFFFF, 0x87FFFFFF, 0x85FFFFFF, 0x83FFFFFF,
	0x81FFFFFF, 0x7FFFFFFF, 0x7DFFFFFF, 0x7BFFFFFF, 0x79FFFFFF,
	0x77FFFFFF, 0x75FFFFFF, 0x73FFFFFF, 0x71FFFFFF, 0x6FFFFFFF,
	0x6DFFFFFF, 0x6BFFFFFF, 0x69FFFFFF, 0x67FFFFFF, 0x65FFFFFF,
	0x63FFFFFF, 0x61FFFFFF, 0x5FFFFFFF, 0x5DFFFFFF, 0x5BFFFFFF,
	0x59FFFFFF, 0x57FFFFFF, 0x55FFFFFF, 0x53FFFFFF, 0x51FFFFFF,
	0x4FFFFFFF, 0x4DFFFFFF, 0x4BFFFFFF, 0x49FFFFFF, 0x47FFFFFF,
	0x45FFFFFF, 0x43FFFFFF, 0x41FFFFFF, 0x3FFFFFFF, 0x3DFFFFFF,
	0x3BFFFFFF, 0x39FFFFFF, 0x37FFFFFF, 0x35FFFFFF, 0x33FFFFFF,
	0x31FFFFFF, 0x2FFFFFFF, 0x2DFFFFFF, 0x2BFFFFFF, 0x29FFFFFF,
	0x27FFFFFF, 0x25FFFFFF, 0x23FFFFFF, 0x21FFFFFF, 0x1FFFFFFF,
	0x1DFFFFFF, 0x1BFFFFFF, 0x19FFFFFF, 0x17FFFFFF, 0x15FFFFFF,
	0x13FFFFFF, 0x11FFFFFF, 0x0FFFFFFF, 0x0DFFFFFF, 0x0BFFFFFF,
	0x09FFFFFF, 0x07FFFFFF, 0x05FFFFFF, 0x03FFFFFF, 0x01FFFFFF
};


CCRC32::CCRC32(void)
{

}

CCRC32::CCRC32( const CCRC32& inDuplicate )
{
}

CCRC32::~CCRC32(void)
{
}

// 파일 이름으로 CRC 코드 리턴하기
unsigned long CCRC32::CalculateCRCCode( TCHAR* ptstrFileName, unsigned long *pGetSize/* =NULL  */)
{
	unsigned long	ulReturnCRC		= 0;
	FILE			*pfSourceFile	= NULL;

	
	if ( (pfSourceFile = _tfopen( ptstrFileName,  _T("rb") )) == NULL )
	{
		return ERROR_RETURN_VALUE;
	}

	ulReturnCRC = CalculateCRCCode( pfSourceFile, pGetSize );

	fclose( pfSourceFile );

	return ulReturnCRC;
}

// 파일 포인터로 CRC 코드 리턴하기
unsigned long CCRC32::CalculateCRCCode( FILE* pFile, unsigned long *pGetSize/* =NULL  */)
{
	unsigned long	ulReturnCRC		= 0;	//실제 리턴될 CRC값
	unsigned long	ulTableValue	= 0;	//중간 계산용
	unsigned char	ucBuf[MAX_READ_BUFFER_SIZE] = {0,}; //읽기 버퍼
	long			lOldFilePoint	= ftell( pFile ); //현재의 파일 포인터를 저장
	size_t			stReadByte		= 0;	//현재 읽어온 바이트 수
	size_t			stIndex			= 0;	//반복문에 사용
	size_t			stBufSize		= sizeof(ucBuf); //반복적인 sizeof호출을 막기위해 미리 저장


	ulReturnCRC		= (unsigned long)XOR_VALUE;
	if (pGetSize != NULL) (*pGetSize) = 0;

	rewind( pFile ); //파일포인터를 맨 처음으로 옮김
	
	while ( (stReadByte = fread( ucBuf, 1, stBufSize, pFile )) > 0 )
	{
		if (pGetSize != NULL) (*pGetSize) += (unsigned long)stReadByte;

		for (stIndex=0; stIndex<stReadByte; stIndex++)
		{
			ulTableValue = m_ulCRCTable[ (ulReturnCRC^ucBuf[stIndex]) & 0xFF ];
			ulReturnCRC = (ulReturnCRC>>8) ^ ulTableValue;
		}
		ZeroMemory(ucBuf, stBufSize);
	}

	fseek( pFile, lOldFilePoint, SEEK_SET ); //함수를 호출하기 전 파일 포인터로 되돌림

	return ( ulReturnCRC^XOR_VALUE );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Name		: CCRC32List
//
// Day		: 2008.01.09
//
// Comment	: CCRC32 클래스를 상속해서 crc코드와 파일이름도 함께 저장될 수 있도록 함
///////////////////////////////////////////////////////////////////////////////////////////////////////
CCRC32List::CCRC32List() : m_ulCRC32Code(0), m_ulFileSize(0)
{
	ZeroMemory( m_tszFileName, MAX_PATH );
}

CCRC32List::CCRC32List( const TCHAR* ptsFileName )
{
	ZeroMemory( m_tszFileName, MAX_PATH );

	SetList( ptsFileName );
}

CCRC32List::CCRC32List( const CCRC32List& inDuplicate )
{
	ZeroMemory( m_tszFileName, MAX_PATH );

	StringCchCopy( m_tszFileName, MAX_PATH, inDuplicate.GetFileName() );
	m_ulCRC32Code	= inDuplicate.GetCRC32Code();
	m_ulFileSize	= inDuplicate.GetFileSize();

}

BOOL CCRC32List::SetList( const TCHAR* ptsFileName )
{
	unsigned long	ulReturnValue	= 0;
	FILE*			pFile			= NULL;


	// 인수의 파일 이름으로 파일을 연다
	if ( (pFile = _tfopen( ptsFileName,  _T("rb") )) == NULL )
	{
		return FALSE;
	}

	// 해당 파일의 CRC32 코드를 얻어낸다(파일 사이즈도 같이 얻어 온다)
	ulReturnValue = CalculateCRCCode( pFile, &m_ulFileSize );
	
	if ( (ulReturnValue == 0) || (m_ulFileSize == 0) )
	{
		m_ulFileSize = 0;
		return FALSE;
	}

	// 멤버 변수에 값 대입
	StringCchCopy( m_tszFileName, MAX_PATH, ptsFileName );
	m_ulCRC32Code	= ulReturnValue;


	fclose( pFile );

	return TRUE;
}

SI08 CCRC32List::Compare( const CCRC32List& inCompare )
{
	// 파일 이름 비교
	if ( _tcscmp( m_tszFileName, inCompare.GetFileName() ) != 0 ) return -1;

	// 파일 사이즈 비교
	if ( m_ulFileSize != inCompare.GetFileSize() ) return -2;
	
	// CRC32코드 비교
	if ( m_ulCRC32Code != inCompare.GetCRC32Code() ) return -3;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Name		: CCRC32Controller
//
// Day		: 2008.01.09
//
// Comment	: CCRC32List 클래스를 이용하여 효율적으로 리스트 설정 및 비교를 하기위한 컨트롤 클래스
///////////////////////////////////////////////////////////////////////////////////////////////////////
CCRC32Controller::CCRC32Controller() : m_pCRC32List(NULL), m_uiElements(0) 
{
}

CCRC32Controller::~CCRC32Controller()
{
	if ( m_pCRC32List )
	{
		delete [] m_pCRC32List;
		m_pCRC32List = NULL;
	}
}

BOOL CCRC32Controller::Set( const TCHAR* ptsFileNames, const UI08 uiElements )
{
	if ( uiElements == 0) return FALSE;

	
	m_pCRC32List = new CCRC32List[uiElements];

	if ( m_pCRC32List == NULL ) return FALSE;


	for (UI08 i=0; i<uiElements; i++)
	{	// 2차원 배열을 포인터로 받았기 때문에 각 요소에 접근하기 위해서 MAX_PATH가 필요함
		m_pCRC32List[i].SetList( &ptsFileNames[i*MAX_PATH] );
	}

	m_uiElements = uiElements;

	return TRUE;
}

CCRC32List* CCRC32Controller::GetCRC32List( const UI08 uiElement )
{
	if ( uiElement >= m_uiElements ) return NULL;


	return &m_pCRC32List[uiElement];
}

BOOL CCRC32Controller::ListCompare( const CCRC32List* pCRC32List, const UI08 uiElements )
{
	if ( m_uiElements != uiElements ) return FALSE;

	for (UI08 i=0; i<uiElements; i++)
	{
		if ( m_pCRC32List[i].Compare( pCRC32List[i] ) < 0 ) return FALSE;
	}

	return TRUE;
}