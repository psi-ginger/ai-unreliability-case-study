//-----------------------------------------------------------------------------
// 2005/09/13 ������
//-----------------------------------------------------------------------------

#include "NDataTypes.h"
#include "NTextManager.h"
#include "NHashTableString.h"
#include "NFile.h"
#include "NUtil.h"
#include "NTCHARString.h"

NTextManager::NTextManager()
{
	//m_pBuffer = NULL;
	m_pHashTable = NULL;

	m_iCurBufPos = 0;
}

NTextManager::~NTextManager()
{
	NDelete(m_pHashTable);
	//NDelete_Array(m_pBuffer);
}


void NTextManager::CreateTextManager()
{

	m_iCurBufPos = 0;
	//m_pBuffer = new TCHAR[ 500 * 1024 ];
	m_pBuffer.NewMem( 500 * 1024 );

	m_pHashTable = new NHashTableString< TCHAR* >;

	// TextMgr_Text_ENG.txt alone carries 15,185 keys, so the old 12,000/15,000
	// sizing overflowed the table.
	m_pHashTable->CreateHashTableString( 40000, 60000, MAX_TEXT_KEY );

}


bool NTextManager::LoadTextFromFile( TCHAR *filename, int setcolumn )
{
	TCHAR strKey[ MAX_TEXT_KEY ];

	HANDLE hFile = NULL;
	hFile = CreateFile(	filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		0,
		0 );

	if( hFile == INVALID_HANDLE_VALUE ) 
	{
		return false;
	}

	// ���� ũ�⸦ ���´�
	DWORD dwFileSize;
	DWORD dwFileSizeHigh;
	dwFileSize = GetFileSize( hFile, &dwFileSizeHigh );

	DWORD dwFileWLen;
	dwFileWLen = dwFileSize >> 1;

	// Size the storage from the file being read. The previous fixed 500*1024
	// buffer overflowed on TextMgr_Text_ENG.txt (775,341 chars of text), and every
	// out-of-range write raised EXCEPTION_ARRAY_BOUNDS_EXCEEDED, which the
	// top-level SEH handler swallowed - an endless exception loop at 100% CPU.
	m_pBuffer.ReNewMem( (int)dwFileWLen + ( (int)dwFileWLen >> 1 ) + 1024 );

	// ���� ����
	WCHAR *pBuf = NULL;
	pBuf = new WCHAR[ dwFileWLen + 1 ];

	// ���Ͽ��� ������ �о����
	if( hFile ) 
	{
		DWORD dwBytesRead;
		ReadFile( hFile, (void*)pBuf, dwFileSize, &dwBytesRead, NULL );

		CloseHandle( hFile );

		hFile = NULL;

		if( dwBytesRead != dwFileSize ) {
			delete [] pBuf;
			return false;
		}
	}

	pBuf[ dwFileWLen ] = NULL;

	int	iCurCol = 0;

	WCHAR *pCurPos = pBuf;					// ���� ��ġ
	WCHAR *pStartPos = pBuf;				// ���ڿ� ���� ��ġ
	WCHAR *pEndPos = pBuf + dwFileWLen;

	int len, i;
	while( pCurPos < pEndPos ) 
	{
		// �� ������ �̸�
		if( *pCurPos == L'\t' )	
		{
			*pCurPos = L'\0';

			if( pCurPos != pStartPos ) {

				if( iCurCol == 0 ) {

					// �����ڵ带 �о� ��Ƽ����Ʈ�� �ٲٰ� ���ۿ� ���� 
#ifdef _UNICODE
					StringCchCopy( strKey, MAX_TEXT_KEY, pStartPos );
					len = wcslen( pStartPos );
#else
					len = WideCharToMultiByte( CP_ACP, 0, pStartPos, -1, strKey, MAX_TEXT_KEY, NULL, NULL );
#endif

				} else if( iCurCol == setcolumn ) {

					// �����ڵ带 �о� ��Ƽ����Ʈ�� �ٲٰ� ���ۿ� ���� 
#ifdef _UNICODE
					wcscpy( &m_pBuffer[ m_iCurBufPos ], pStartPos );
					len = wcslen( pStartPos );
#else
					len = WideCharToMultiByte( CP_ACP, 0, pStartPos, -1, &m_pBuffer[ m_iCurBufPos ], 1024, NULL, NULL );
#endif

					// ���ʿ� " �� �ִٸ� �����Ѵ�.
					NTCHARString1024 kText( &m_pBuffer[ m_iCurBufPos ] );
					kText.RemoveDoubleQuotation();
					StringCchCopy( &m_pBuffer[ m_iCurBufPos ], 1024, kText );

					for( i = 0; i < len; ++i )
					{
						if( m_pBuffer[ m_iCurBufPos + i ] == TEXT('\\') )
						{
							if( m_pBuffer[ m_iCurBufPos + i + 1 ] == TEXT('r') )
							{
								m_pBuffer[ m_iCurBufPos + i ] = TEXT('\r');
								m_pBuffer[ m_iCurBufPos + i + 1 ] = TEXT('\n');

								++i;
								continue;
							}
						}
					}

					m_pHashTable->Insert( strKey, &m_pBuffer[ m_iCurBufPos ] );

					m_iCurBufPos += len;
				}
			}

			++iCurCol;
			++pCurPos;

			pStartPos = pCurPos;

			continue;

			// �ٹٲ� ������ �̸� 
		} else if( *pCurPos == L'\r' ) {
			*pCurPos = L'\0';


			if( pCurPos != pStartPos && iCurCol == setcolumn ) {

				// �����ڵ带 �о� ��Ƽ����Ʈ�� �ٲٰ� ���ۿ� ���� 
#ifdef _UNICODE
				wcscpy( &m_pBuffer[ m_iCurBufPos ], pStartPos );
				len = wcslen( pStartPos );
#else
				len = WideCharToMultiByte( CP_ACP, 0, pStartPos, -1, &m_pBuffer[ m_iCurBufPos ], 1024, NULL, NULL );
#endif

				// ���ʿ� " �� �ִٸ� �����Ѵ�.
				NTCHARString1024 kText( &m_pBuffer[ m_iCurBufPos ] );
				kText.RemoveDoubleQuotation();
				StringCchCopy( &m_pBuffer[ m_iCurBufPos ], 1024, kText );

				for( i = 0; i < len; ++i )
				{
					if( m_pBuffer[ m_iCurBufPos + i ] == TEXT('\\') )
					{
						if( m_pBuffer[ m_iCurBufPos + i + 1 ] == TEXT('r') )
						{
							m_pBuffer[ m_iCurBufPos + i ] = TEXT('\r');
							m_pBuffer[ m_iCurBufPos + i + 1 ] = TEXT('\n');

							++i;
							continue;
						}
					}
				}

				if( strKey ) m_pHashTable->Insert( strKey, &m_pBuffer[ m_iCurBufPos ] );

				m_iCurBufPos += len;
			}

			iCurCol = 0;
			pCurPos += 2;
			pStartPos = pCurPos;

			strKey[ 0 ] = NULL;

			continue;

		}

		++pCurPos;
	}

	if( pBuf ) delete [] pBuf;

	return true;
}

// ���� �Լ� �����ص� �ǳ�.. ����ó�� ���Ͽ� ���� ����..
bool NTextManager::LoadTextFromCryptFile( TCHAR *filename, int setcolumn )
{
	TCHAR strKey[ MAX_TEXT_KEY ];

	WCHAR *pBuf = NULL;

	NFile file;
	DWORD dwFileWLen;

	if ( file.LoadFile( filename ) == false )
	{
#ifdef _DEBUG
		NTCHARString64 kMsg;
		kMsg  = filename;
		kMsg += " ������ �����ϴ�.";
		MessageBox(NULL, kMsg, "ERROR", MB_OK);
#endif
		return false;
	}

	NCryptHeader *pHeader = (NCryptHeader *)file.GetBuffer();
	if( pHeader->id[ 0 ] == 'N' &&
		pHeader->id[ 1 ] == 'C' &&
		pHeader->id[ 2 ] == 'R' ) 
	{
		// ��ȣȭ �� ���
		BYTE *pData = (BYTE*)pHeader + sizeof( NCryptHeader );
		int size = pHeader->dwDataSize;
		BYTE key = pHeader->key;

		dwFileWLen = size >> 1;
		pBuf = new WCHAR[ dwFileWLen + 1 ];

		// See LoadTextFromFile: storage is sized from the payload length.
		m_pBuffer.ReNewMem( (int)dwFileWLen + ( (int)dwFileWLen >> 1 ) + 1024 );

		NByteDecrypt( pData, (BYTE*)pBuf, size, key );

		pBuf[ dwFileWLen ] = NULL;
	}
	else
	{
#ifdef _DEBUG
		// ��ȣȭ ���� ���� ���
		HANDLE hFile = NULL;
		hFile = CreateFile(	filename,
			GENERIC_READ,
			FILE_SHARE_READ,
			0,
			OPEN_EXISTING,
			0,
			0 );

		if( hFile == INVALID_HANDLE_VALUE ) 
		{
			return false;
		}

		// ���� ũ�⸦ ���´�
		DWORD dwFileSize;
		DWORD dwFileSizeHigh;
		dwFileSize = GetFileSize( hFile, &dwFileSizeHigh );

		dwFileWLen = dwFileSize >> 1;

		// ���� ����
		pBuf = new WCHAR[ dwFileWLen + 1 ];

		// ���Ͽ��� ������ �о����
		if( hFile ) 
		{
			DWORD dwBytesRead;
			ReadFile( hFile, (void*)pBuf, dwFileSize, &dwBytesRead, NULL );

			CloseHandle( hFile );

			hFile = NULL;

			if( dwBytesRead != dwFileSize ) 
			{
				delete [] pBuf;
				return false;
			}
		}
		pBuf[ dwFileWLen ] = NULL;
#else
		return false;
#endif
	}

	int	iCurCol = 0;

	WCHAR *pCurPos = pBuf;					// ���� ��ġ
	WCHAR *pStartPos = pBuf;				// ���ڿ� ���� ��ġ
	WCHAR *pEndPos = pBuf + dwFileWLen;

	int len, i;
	while( pCurPos < pEndPos ) 
	{

		// �� ������ �̸�
		if( *pCurPos == L'\t' )	
		{
			*pCurPos = L'\0';

			if( pCurPos != pStartPos ) {

				if( iCurCol == 0 ) {

					// �����ڵ带 �о� ��Ƽ����Ʈ�� �ٲٰ� ���ۿ� ���� 
#ifdef _UNICODE
					StringCchCopy( strKey, MAX_TEXT_KEY, pStartPos );
					len = wcslen( pStartPos );
#else
					len = WideCharToMultiByte( CP_ACP, 0, pStartPos, -1, strKey, MAX_TEXT_KEY, NULL, NULL );
#endif

				} else if( iCurCol == setcolumn ) {

					// �����ڵ带 �о� ��Ƽ����Ʈ�� �ٲٰ� ���ۿ� ���� 
#ifdef _UNICODE
					wcscpy( &m_pBuffer[ m_iCurBufPos ], pStartPos );
					len = wcslen( pStartPos );
#else
					len = WideCharToMultiByte( CP_ACP, 0, pStartPos, -1, &m_pBuffer[ m_iCurBufPos ], 1024, NULL, NULL );
#endif

					// ���ʿ� " �� �ִٸ� �����Ѵ�.
					NTCHARString1024 kText( &m_pBuffer[ m_iCurBufPos ] );
					kText.RemoveDoubleQuotation();
					StringCchCopy( &m_pBuffer[ m_iCurBufPos ], 1024, kText );

					for( i = 0; i < len; ++i )
					{
						if( m_pBuffer[ m_iCurBufPos + i ] == TEXT('\\') )
						{
							if( m_pBuffer[ m_iCurBufPos + i + 1 ] == TEXT('r') )
							{
								m_pBuffer[ m_iCurBufPos + i ] = TEXT('\r');
								m_pBuffer[ m_iCurBufPos + i + 1 ] = TEXT('\n');

								++i;
								continue;
							}
						}
					}

					m_pHashTable->Insert( strKey, &m_pBuffer[ m_iCurBufPos ] );

					m_iCurBufPos += len;
				}
			}

			++iCurCol;
			++pCurPos;

			pStartPos = pCurPos;

			continue;

			// �ٹٲ� ������ �̸� 
		} else if( *pCurPos == L'\r' ) {
			*pCurPos = L'\0';


			if( pCurPos != pStartPos && iCurCol == setcolumn ) {

				// �����ڵ带 �о� ��Ƽ����Ʈ�� �ٲٰ� ���ۿ� ���� 
#ifdef _UNICODE
				wcscpy( &m_pBuffer[ m_iCurBufPos ], pStartPos );
				len = wcslen( pStartPos );
#else
				len = WideCharToMultiByte( CP_ACP, 0, pStartPos, -1, &m_pBuffer[ m_iCurBufPos ], 1024, NULL, NULL );
#endif

				// ���ʿ� " �� �ִٸ� �����Ѵ�.
				NTCHARString1024 kText( &m_pBuffer[ m_iCurBufPos ] );
				kText.RemoveDoubleQuotation();
				StringCchCopy( &m_pBuffer[ m_iCurBufPos ], 1024, kText );

				for( i = 0; i < len; ++i )
				{
					if( m_pBuffer[ m_iCurBufPos + i ] == TEXT('\\') )
					{
						if( m_pBuffer[ m_iCurBufPos + i + 1 ] == TEXT('r') )
						{
							m_pBuffer[ m_iCurBufPos + i ] = TEXT('\r');
							m_pBuffer[ m_iCurBufPos + i + 1 ] = TEXT('\n');

							++i;
							continue;
						}
					}
				}

				if( strKey ) m_pHashTable->Insert( strKey, &m_pBuffer[ m_iCurBufPos ] );

				m_iCurBufPos += len;
			}

			iCurCol = 0;
			pCurPos += 2;
			pStartPos = pCurPos;

			strKey[ 0 ] = NULL;

			continue;

		}

		++pCurPos;
	}

	if( pBuf ) delete [] pBuf;

	return true;
}

TCHAR* NTextManager::GetText( TCHAR *strKey )
{
	TCHAR *text;
	
	if( m_pHashTable->Find( strKey, text ) ) return text;

	return NULL;
}

TCHAR* NTextManager::GetSafeText( TCHAR *strKey )
{
	TCHAR *text;
	
	if( m_pHashTable->Find( strKey, text ) ) return text;

	return TEXT("");
}

