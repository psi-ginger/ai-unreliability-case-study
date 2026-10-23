#pragma once

#include "NDataTypes.h"

class CCRC32
{

enum{
	XOR_VALUE				= 0xFFFFFFFFL,	//반전 비트(이값 대신 ~연산자를 써도 가능)
	POLYNOMIAL				= 0xEDB88320L,	//최대한 중복값이 없는 제수값
	MAX_READ_BUFFER_SIZE	= 65536,		//최대 읽기 버퍼 사이즈
	MAX_CRC_TABLE_NUM		= 256,			//최대 CRC 테이블 값	
	ERROR_RETURN_VALUE		= 0				//에러시 리턴되는 값
};


// 맴버 변수 영역
protected:
	//속도향상 및 메모리 낭비(객체는 달라도 테이블은 같은것을 이용함)를 막기위해 static으로 테이블 값 설정
	const static unsigned long m_ulCRCTable[MAX_CRC_TABLE_NUM]; 

// 멤버 함수 영역
public:
	CCRC32(void);
	// 복사 생성자
	CCRC32( const CCRC32& inDuplicate );
	~CCRC32(void);

	unsigned long CalculateCRCCode( TCHAR* ptstrFileName, unsigned long *pGetSize=NULL );
	unsigned long CalculateCRCCode( FILE* pFile, unsigned long *pGetSize=NULL );
		
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
class CCRC32List : public CCRC32
{
// 멤버 변수 영역
protected:
	TCHAR			m_tszFileName[MAX_PATH];	// 파일 이름
	unsigned long	m_ulCRC32Code;				// CRC32 코드
	unsigned long	m_ulFileSize;				// 파일 사이즈


// 멤버 함수 영역
public:
	CCRC32List();
	CCRC32List( const TCHAR* ptsFileName );
	// 복사 생성자
	CCRC32List( const CCRC32List& inDuplicate );

	~CCRC32List() {};

public:
	BOOL SetList( const TCHAR* ptsFileName );	// 파일 이름을 넣으면 모든 멤버 변수를 셋트한다.

	const TCHAR* GetFileName(void) const { return m_tszFileName; }
	unsigned long GetCRC32Code(void) const { return m_ulCRC32Code; }
	unsigned long GetFileSize(void) const { return m_ulFileSize; }

public:
	SI08 Compare( const CCRC32List& inCompare);


};


///////////////////////////////////////////////////////////////////////////////////////////////////////
class CCRC32Controller
{
// 멤버 변수 영역
protected:
	CCRC32List*		m_pCRC32List; // 동적 할당을 위한 변수
	unsigned int	m_uiElements; // 현재 CCRC32List의 갯수


// 멤버 함수 영역
public:
	CCRC32Controller();
	~CCRC32Controller();

public:
	BOOL Set( const TCHAR* ptsFileNames, const UI08 uiElements );
	CCRC32List* GetCRC32List( const UI08 uiElement );

public:
	BOOL ListCompare( const CCRC32List* pCRC32List, const UI08 uiElements );

};
