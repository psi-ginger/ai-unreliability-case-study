#include "NkDisk.h"

NkImplementRootRTTI(NkDiskNode_Base);
NkImplementRTTI(NkDiskNode_Folder,NkDiskNode_Base);
NkImplementRTTI(NkDiskNode_File,NkDiskNode_Base);

NkDisk::NkDisk()
{
	m_hDiskFile = INVALID_HANDLE_VALUE;

	m_dwCurrentStringPos = 0;

	m_bReadOnly = FALSE;
	m_bModified = FALSE;
}

NkDisk::~NkDisk()
{
	CloseDisk();
}

NkDiskNode_Folder* NkDisk::NewNode_Folder()
{
	return NkNew NkDiskNode_Folder;
}

NkDiskNode_File* NkDisk::NewNode_File()
{
	return NkNew NkDiskNode_File;
}

void NkDisk::DeleteNode( NkDiskNode_Base* pkNode )
{
	delete pkNode;
}

//-----------------------------------------------------------------------------
// 디스크 파일을 생성한다
//-----------------------------------------------------------------------------
BOOL NkDisk::CreateDisk( const char* szDiskName )
{
	if( m_pkRootNode ) return FALSE;

	m_hDiskFile = CreateFile( szDiskName, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0 );

	if( m_hDiskFile == INVALID_HANDLE_VALUE ) return FALSE;

	m_llEndOfFileDataOffset = 128;

	// 헤더 생성
	m_pkFileHeader = NkNew NkDiskHeader;

	// 헤더 정보 초기화
	InitHeader();

	// Root Node 생성
	m_pkRootNode = NewNode_Folder();
	m_pkRootNode->SetFileName("");

	// 해쉬 맵 생성 
	m_pkFileMap = NkNew NkDiskFileMap( 10000 );

	m_bReadOnly = FALSE;
	m_bModified = TRUE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// 디스크 파일을 오픈한다
//-----------------------------------------------------------------------------
BOOL NkDisk::OpenDisk( const char* szDiskName, BOOL bReadOnly )
{
	if( m_pkRootNode ) return FALSE;

	m_hDiskFile = CreateFile( szDiskName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0 );

	if( m_hDiskFile == INVALID_HANDLE_VALUE ) return FALSE;

	m_bReadOnly = bReadOnly;
	m_bModified = FALSE;

	// 헤더 및 디스크 정보 로드
	LoadDiskInfos();

	return TRUE;
}

//-----------------------------------------------------------------------------
// 디스크 파일을 닫는다
//-----------------------------------------------------------------------------
void NkDisk::CloseDisk()
{
	// 수정 되었다면, 헤더 및 디스크 정보를 기록한다
	if( m_bModified )
	{
		SaveDiskInfos();
	}

	if( m_hDiskFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hDiskFile );
		m_hDiskFile = INVALID_HANDLE_VALUE;
	}

	// 노드 삭제
	if( m_pkRootNode )
	{
		RemoveAllChildNodes( m_pkRootNode );
		m_pkRootNode.Delete();
	}

	// 파일 맵 삭제
	m_pkFileMap.Delete();

	m_bReadOnly = FALSE;
	m_bModified = FALSE;
}

//-----------------------------------------------------------------------------
// 헤더 초기화
//-----------------------------------------------------------------------------
void NkDisk::InitHeader()
{
	if( m_pkFileHeader )
	{
		ZeroMemory( m_pkFileHeader, sizeof(NkDiskHeader) );

		m_pkFileHeader->m_dwUnique = 0x00000036;
		m_pkFileHeader->m_dwVersion = 0x00000101;
		m_pkFileHeader->m_llFileDataOffset = 128;
		m_pkFileHeader->m_llFileListOffset = 128;
		m_pkFileHeader->m_llStringTableOffset = 128;
		m_pkFileHeader->m_dwFileListSize = 0;
		m_pkFileHeader->m_dwStringTableSize = 0;
		m_pkFileHeader->m_dwRootFolderCount = 0;
		m_pkFileHeader->m_dwRootFileCount = 0;
		m_pkFileHeader->m_dwTotalFolderCount = 0;
		m_pkFileHeader->m_dwTotalFileCount = 0;
	}
}


//-----------------------------------------------------------------------------
// 주어진 폴더 노드에 새로운 폴더 노드를 추가한다.
//-----------------------------------------------------------------------------
NkDiskNode_Folder* NkDisk::AddNode_Folder( const char* szFolderName, NkDiskNode_Folder* pkParentNode )
{
	// 노드 생성
	NkDiskNode_Folder* pkNewNode = NewNode_Folder();

	// 부모 설정
	pkNewNode->m_pkParent = pkParentNode;

	// 폴더명 설정
	pkNewNode->SetFileName( szFolderName );

	// 해쉬멥에 추가
	if( m_pkFileMap->AddAt( pkNewNode->m_pcFileName, pkNewNode ) == false )
	{
		DeleteNode( pkNewNode );
		return NULL;
	}

	// 주어진 노드(폴더)의 자식 노드(폴더)에 추가
	pkNewNode->m_pkNext = pkParentNode->m_pkFirstFolder;
	pkParentNode->m_pkFirstFolder = pkNewNode;

	++pkParentNode->m_dwFolderCount;
	++m_pkFileHeader->m_dwTotalFolderCount;

	return pkNewNode;
}

//-----------------------------------------------------------------------------
// 주어진 폴더 노드에 새로운 파일 노드를 추가한다.
//-----------------------------------------------------------------------------
NkDiskNode_File* NkDisk::AddNode_File( const char* szFileName, NkDiskNode_Folder* pkParentNode )
{
	// 노드 생성
	NkDiskNode_File* pkNewNode = NewNode_File();

	// 부모 설정
	pkNewNode->m_pkParent = pkParentNode;

	// 파일명 설정
	pkNewNode->SetFileName( szFileName );

	// 해쉬맵에 추가
	if( m_pkFileMap->AddAt( pkNewNode->m_pcFileName, pkNewNode ) == false )
	{
		DeleteNode( pkNewNode );
		return NULL;
	}

	// 주어진 노드(폴더)의 자식 노드(파일)에 추가
	pkNewNode->m_pkNext = pkParentNode->m_pkFirstFile;
	pkParentNode->m_pkFirstFile = pkNewNode;

	++pkParentNode->m_dwFileCount;
	++m_pkFileHeader->m_dwTotalFileCount;

	return pkNewNode;
};

//-----------------------------------------------------------------------------
// 폴더 노드를 제거 한다.
//-----------------------------------------------------------------------------
BOOL NkDisk::RemoveNode_Folder( NkDiskNode_Folder* pkFolderNode )
{

	NkDiskNode_Folder* pkParent = pkFolderNode->m_pkParent;
	NkDiskNode_Folder* pkCurrNode = pkParent->m_pkFirstFolder;
	NkDiskNode_Folder* pkPrevNode = 0;

	while( pkCurrNode )
	{
		if( pkCurrNode == pkFolderNode )
		{
			if( pkPrevNode )
			{
				pkPrevNode->m_pkNext = pkCurrNode->m_pkNext;
			}
			else
			{
				pkParent->m_pkFirstFolder = (NkDiskNode_Folder*)pkCurrNode->m_pkNext;
			}

			DeleteNode( pkCurrNode );

			--pkParent->m_dwFolderCount;
			--m_pkFileHeader->m_dwTotalFolderCount;

			// 해쉬맵에서 제거
			m_pkFileMap->RemoveAt( pkFolderNode->m_pcFileName, (NkDiskNode_Base**)&pkFolderNode );

			return TRUE;
		}

		pkPrevNode = pkCurrNode;
		pkCurrNode = (NkDiskNode_Folder*)pkCurrNode->m_pkNext;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// 파일 노드를 제거 한다.
//-----------------------------------------------------------------------------
BOOL NkDisk::RemoveNode_File( NkDiskNode_File* pkFileNode )
{

	NkDiskNode_Folder* pkParent = pkFileNode->m_pkParent;
	NkDiskNode_File* pkCurrNode = pkParent->m_pkFirstFile;
	NkDiskNode_File* pkPrevNode = 0;

	while( pkCurrNode )
	{
		if( pkCurrNode == pkFileNode )
		{
			if( pkPrevNode )
			{
				pkPrevNode->m_pkNext = pkCurrNode->m_pkNext;
			}
			else
			{
				pkParent->m_pkFirstFile = (NkDiskNode_File*)pkCurrNode->m_pkNext;
			}

			DeleteNode( pkCurrNode );

			--pkParent->m_dwFileCount;
			--m_pkFileHeader->m_dwTotalFileCount;

			// 해쉬맵에서 제거
			m_pkFileMap->RemoveAt( pkFileNode->m_pcFileName, (NkDiskNode_Base**)&pkFileNode );

			return TRUE;
		}

		pkPrevNode = pkCurrNode;
		pkCurrNode = (NkDiskNode_File*)pkCurrNode->m_pkNext;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// 자식 폴더 및 파일 노드를 모두 제거 한다
//-----------------------------------------------------------------------------
void NkDisk::RemoveAllChildNodes( NkDiskNode_Folder* pkFolderNode )
{
	// 자식 폴더 제거
	NkDiskNode_Folder* pkCurrFolderNode = pkFolderNode->m_pkFirstFolder;

	while( pkCurrFolderNode )
	{
		NkDiskNode_Folder* pkDelNode = pkCurrFolderNode;	

		pkCurrFolderNode = (NkDiskNode_Folder*)pkCurrFolderNode->m_pkNext;

		RemoveAllChildNodes( pkDelNode );

		DeleteNode( pkDelNode );

		--m_pkFileHeader->m_dwTotalFolderCount;
	}

	pkFolderNode->m_dwFolderCount = 0;
	pkFolderNode->m_pkFirstFolder = 0;

	// 자식 파일 제거
	NkDiskNode_File* pkCurrFileNode = pkFolderNode->m_pkFirstFile;

	while( pkCurrFileNode )
	{
		NkDiskNode_File* pkDelNode = pkCurrFileNode;	

		pkCurrFileNode = (NkDiskNode_File*)pkCurrFileNode->m_pkNext;

		DeleteNode( pkDelNode );

		--m_pkFileHeader->m_dwTotalFolderCount;
	}

	pkFolderNode->m_dwFileCount = 0;
	pkFolderNode->m_pkFirstFile = 0;
}

//-----------------------------------------------------------------------------
// 폴더 트리를 구성한다
//-----------------------------------------------------------------------------
void NkDisk::MakeFoldersTree( const char* szFolderName, NkDiskNode_Folder* pkFolderNode )
{
	BOOL bResult;

	NkCharsT< 256 > kPath;

	WIN32_FIND_DATA wfd;
	HANDLE hFindFile;

	kPath = szFolderName;	
	kPath += "\\*.*";

	hFindFile = ::FindFirstFile( kPath.GetString(), &wfd );

	if( hFindFile != INVALID_HANDLE_VALUE )
	{
		bResult = TRUE;

		while( bResult )
		{
			if( !(wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) )
			{
				// 폴더이면,
				if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					if( wfd.cFileName[ 0 ] != _T('.') )
					{
						AddNode_Folder( wfd.cFileName, pkFolderNode );						
					}
				}
				// 파일이면,
				else
				{
					AddNode_File( wfd.cFileName, pkFolderNode );
				}
			}

			bResult = ::FindNextFile( hFindFile, &wfd );
		}
		::FindClose( hFindFile );

		// 하위 폴더 탐색
		pkFolderNode = pkFolderNode->m_pkFirstFolder;

		while( pkFolderNode )
		{
			kPath.Format( "%s\\%s", szFolderName, pkFolderNode->m_pcFileName );

			MakeFoldersTree( kPath.GetString(), pkFolderNode );

			pkFolderNode = (NkDiskNode_Folder*)pkFolderNode->m_pkNext;
		}
	}

};

//-----------------------------------------------------------------------------
// 트리를 탐색하여 파일을 어태치 한다
//-----------------------------------------------------------------------------
void NkDisk::AttachFilesOnTree( const char* szFolderName, NkDiskNode_Folder* pkFolderNode )
{
	NkCharsT< 256 > kPath;

	// 폴더내 파일 탐색 및 패키징 파일에 붙이기
	NkDiskNode_File* pkFileNode = pkFolderNode->m_pkFirstFile;
	while( pkFileNode )
	{
		kPath.Format( "%s\\%s", szFolderName, pkFileNode->m_pcFileName );

		AttachFile( kPath, pkFileNode );

		pkFileNode = (NkDiskNode_File*)pkFileNode->m_pkNext;
	}

	// 하위 폴더 탐색
	pkFolderNode = pkFolderNode->m_pkFirstFolder;
	while( pkFolderNode )
	{
		kPath.Format( "%s\\%s", szFolderName, pkFolderNode->m_pcFileName );

		AttachFilesOnTree( kPath.GetString(), pkFolderNode );

		pkFolderNode = (NkDiskNode_Folder*)pkFolderNode->m_pkNext;
	}
}

//-----------------------------------------------------------------------------
// 파일을 삽입 한다
//-----------------------------------------------------------------------------
BOOL NkDisk::AttachFile( const char* szFileName, NkDiskNode_File* pkFileNode )
{
	HANDLE hFile = CreateFile( szFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	BOOL bRet = AttachFile( hFile, pkFileNode );

	CloseHandle( hFile );

	return bRet;
}


//-----------------------------------------------------------------------------
// 파일을 삽입 한다
//-----------------------------------------------------------------------------
BOOL NkDisk::AttachFile( HANDLE hFile, NkDiskNode_File* pkFileNode )
{

	// 파일 크기 구하기
	DWORD dwFileSize, dwFileSizeHigh;
	dwFileSize = GetFileSize( hFile, &dwFileSizeHigh );

	// 파일이 너무 크면 FALSE 반환
	if( dwFileSizeHigh != 0 || dwFileSize > (DWORD)0x7FFFFFFF ) 
	{
		return FALSE;
	}

	// 빈블럭을 찾는다
	LONGLONG llFileDataOffset;
	if( !GetEmptyBlock( dwFileSize + sizeof( NkDiskFileDesc ), &llFileDataOffset ) )
	{
		llFileDataOffset = m_llEndOfFileDataOffset;
	}

	pkFileNode->m_llFileDataOffset = llFileDataOffset;

	HANDLE hFileMapping = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
	if( hFileMapping == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}

	void* pFilePtr = (BYTE*)MapViewOfFile( hFileMapping, FILE_MAP_READ, 0, 0, 0 );
	if( pFilePtr == NULL )
	{
		CloseHandle( hFileMapping );
		return FALSE;
	}

	// 파일 정보 쓰기
	NkDiskFileDesc kFileDesc;
	DWORD dwWritten;

	GetFileTime( hFile, &kFileDesc.m_ftCreation, NULL, &kFileDesc.m_ftLastWrite );

	LARGE_INTEGER liOffset;
	liOffset.QuadPart = llFileDataOffset;
	SetFilePointerEx( m_hDiskFile, liOffset, 0, FILE_BEGIN );

	WriteFile( m_hDiskFile, &kFileDesc, sizeof(kFileDesc), &dwWritten, 0 );
	llFileDataOffset += dwWritten;

	WriteFile( m_hDiskFile, pFilePtr, dwFileSize, &dwWritten, 0 );
	llFileDataOffset += dwWritten;

	if( llFileDataOffset > m_llEndOfFileDataOffset ) m_llEndOfFileDataOffset = llFileDataOffset;

	UnmapViewOfFile( pFilePtr );

	CloseHandle( hFileMapping );

	pkFileNode->m_dwFileSize = dwFileSize;

	return TRUE;
}


//-----------------------------------------------------------------------------
// 트리를 탐색하여 파일을 뽑아낸다
//-----------------------------------------------------------------------------
void NkDisk::ExtractFilesFromTree( const char* /*szFolderName*/, NkDiskNode_Folder* /*pkFolderNode*/ )
{
	/*
	NkChars< 256 > kPath;

	// 폴더내 파일 탐색 및 패키징 파일에 붙이기
	NkDiskNode_File* pkFileNode = pkFolderNode->m_pkFirstFile;
	while( pkFileNode )
	{
		kPath.Format( "%s\\%s", szFolderName, pkFileNode->m_pcFileName );

		SetCurrentDirectory( szFolderName )

		ExtractFile( kpkFileNode );
		pkFileNode->m_qwFileDataOffset = m_llCurrentFilePos;

		pkFileNode = (NkDiskNode_File*)pkFileNode->m_pkNext;
	}

	// 하위 폴더 탐색
	pkFolderNode = pkFolderNode->m_pkFirstFolder;
	while( pkFolderNode )
	{
		kPath.Format( "%s\\%s", szFolderName, pkFolderNode->m_pcFileName );

		m_pkCurrentNode = pkFolderNode;
		AttachFilesOnTree( kPath.GetString(), pkFolderNode );

		pkFolderNode = (NkDiskNode_Folder*)pkFolderNode->m_pkNext;
	}
	*/
}

//-----------------------------------------------------------------------------
// 파일을 뽑아 낸다
//-----------------------------------------------------------------------------
BOOL NkDisk::ExtractFile( NkDiskNode_File* pkFileNode )
{
	HANDLE hFile = CreateFile( pkFileNode->m_pcFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0 );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	DWORD dwFileSize = pkFileNode->m_dwFileSize;

	HANDLE hFileMapping = CreateFileMapping( hFile, NULL, PAGE_WRITECOPY, 0, dwFileSize, NULL );
	if( hFileMapping == INVALID_HANDLE_VALUE )
	{
		CloseHandle( hFile );
		return FALSE;
	}

	void* pFilePtr = (BYTE*)MapViewOfFile( hFileMapping, FILE_MAP_COPY, 0, 0, 0 );
	if( pFilePtr == NULL )
	{
		CloseHandle( hFileMapping );
		CloseHandle( hFile );
		return FALSE;
	}

	LARGE_INTEGER liOffset;
	liOffset.QuadPart = pkFileNode->m_llFileDataOffset;
	SetFilePointerEx( m_hDiskFile, liOffset, 0, FILE_BEGIN );
	
	NkDiskFileDesc kFileDesc;
	DWORD dwRead;
	ReadFile( m_hDiskFile, &kFileDesc, sizeof(kFileDesc), &dwRead, 0 );

	ReadFile( m_hDiskFile, pFilePtr, pkFileNode->m_dwFileSize, &dwRead, 0 );
	
	SetFileTime( hFile, &kFileDesc.m_ftCreation, NULL, &kFileDesc.m_ftLastWrite );

	UnmapViewOfFile( pFilePtr );

	CloseHandle( hFileMapping );
	CloseHandle( hFile );

	return TRUE;
}


//-----------------------------------------------------------------------------
// 파일 리스트( 폴더, 파일 )를 읽어온다.
//-----------------------------------------------------------------------------
BOOL NkDisk::ReadFileList( NkDiskNode_Folder* pkNode, DWORD dwFolderCount, DWORD dwFileCount )
{
	DWORD i;
	DWORD dwRead;
	char* pcFileName;

	// 폴더 정보 읽기
	NkDiskFolderInfo kFolderInfo;

	for( i = 0; i < dwFolderCount; ++i )
	{
		ReadFile( m_hDiskFile, &kFolderInfo, sizeof(kFolderInfo), &dwRead, 0 );

		pcFileName = &m_pStringTable[ kFolderInfo.m_dwFolderNameOffset ];

		NkDiskNode_Folder* pkNewFolderNode = AddNode_Folder( pcFileName, pkNode );

		ReadFileList( pkNewFolderNode, kFolderInfo.m_dwFolderCount, kFolderInfo.m_dwFileCount );
	}

	// 파일 정보 읽기
	NkDiskFileInfo kFileInfo;

	for( i = 0; i < dwFileCount; ++i )
	{
		ReadFile( m_hDiskFile, &kFileInfo, sizeof(kFileInfo), &dwRead, 0 );

		pcFileName = &m_pStringTable[ kFileInfo.m_dwFileNameOffset ];

		NkDiskNode_File* pkNewFileNode = AddNode_File( pcFileName, pkNode );
		pkNewFileNode->m_llFileDataOffset = kFileInfo.m_llFileDataOffset;
		pkNewFileNode->m_dwFileSize = kFileInfo.m_dwFileSize;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 파일 리스트( 폴더, 파일 )를 기록한다.
//-----------------------------------------------------------------------------
BOOL NkDisk::WriteFileList( NkDiskNode_Folder* pkNode )
{
	NkDiskFolderInfo kFolderInfo;
	DWORD dwWritten;

	// 폴더 정보 기록하기
	NkDiskNode_Folder* pkFolderNode = pkNode->m_pkFirstFolder;
	while( pkFolderNode )
	{
		kFolderInfo.m_dwFolderCount = pkFolderNode->m_dwFolderCount;
		kFolderInfo.m_dwFileCount = pkFolderNode->m_dwFileCount;
		kFolderInfo.m_dwFolderNameOffset = m_dwCurrentStringPos;

		m_dwCurrentStringPos += (DWORD)strlen( pkFolderNode->m_pcFileName ) + 1;

		WriteFile( m_hDiskFile, &kFolderInfo, sizeof(kFolderInfo), &dwWritten, 0 );

		// 파일 쓰기에 실패하면, FALSE 반환
		if( sizeof(kFolderInfo) != dwWritten ) return FALSE;

		// 파일 리스트 크기 증가
		m_pkFileHeader->m_dwFileListSize += dwWritten;

		if( WriteFileList( pkFolderNode ) == FALSE ) return FALSE;

		pkFolderNode = (NkDiskNode_Folder*)pkFolderNode->m_pkNext;
	}

	// 파일 정보 기록하기
	NkDiskFileInfo kFileInfo;

	NkDiskNode_File* pkFileNode = pkNode->m_pkFirstFile;
	while( pkFileNode )
	{
		kFileInfo.m_llFileDataOffset = pkFileNode->m_llFileDataOffset;
		kFileInfo.m_dwFileSize = pkFileNode->m_dwFileSize;
		kFileInfo.m_dwFileNameOffset = m_dwCurrentStringPos;

		m_dwCurrentStringPos += (DWORD)strlen( pkFileNode->m_pcFileName ) + 1;

		WriteFile( m_hDiskFile, &kFileInfo, sizeof(kFileInfo), &dwWritten, 0 );

		// 파일 쓰기에 실패하면, FALSE 반환
		if( sizeof(kFileInfo) != dwWritten ) return FALSE;

		// 파일 리스트 크기 증가
		m_pkFileHeader->m_dwFileListSize += dwWritten;

		pkFileNode = (NkDiskNode_File*)pkFileNode->m_pkNext;
	}

	return TRUE;
}

BOOL NkDisk::ReadEmptyBlockList()
{

	DWORD i, dwRead;
	for( i = 0; i < m_pkFileHeader->m_dwEmptyBlockCount; ++i )
	{
		NkDiskEmptyBlockInfo* pkEmptyBlockInfo = m_pkEmptyList->NewBack();

		ReadFile( m_hDiskFile, pkEmptyBlockInfo, sizeof(NkDiskEmptyBlockInfo), &dwRead, 0 );
	}

	return TRUE;
}

BOOL NkDisk::WriteEmptyBlockList()
{
	if( !m_pkEmptyList )
	{
		m_pkFileHeader->m_dwEmptyBlockCount = 0;
		return TRUE;
	}

	NkListPos kPos = m_pkEmptyList->GetFrontPos();

	NkDiskEmptyBlockInfo* pkEmptyBlockInfo;
	while( NULL != (pkEmptyBlockInfo = m_pkEmptyList->GetDataPtr( kPos )) )
	{
		DWORD dwWritten;
		WriteFile( m_hDiskFile, pkEmptyBlockInfo, sizeof(NkDiskEmptyBlockInfo), &dwWritten, 0 );

		m_pkEmptyList->MoveNext( kPos );
	}
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// 스트링 테이블( 폴더명, 파일명 )을 기록한다.
//-----------------------------------------------------------------------------
BOOL NkDisk::WriteStringTable( NkDiskNode_Folder* pkNode )
{
	DWORD dwWritten;
	DWORD iSize;

	// 폴더명 기록하기
	NkDiskNode_Folder* pkFolderNode = pkNode->m_pkFirstFolder;
	while( pkFolderNode )
	{
		iSize = (int)strlen( pkFolderNode->m_pcFileName ) + 1;
		WriteFile( m_hDiskFile, pkFolderNode->m_pcFileName, iSize, &dwWritten, 0 );

		// 파일 쓰기에 실패하면, FALSE 반환
		if( iSize != dwWritten ) return FALSE;

		if( WriteStringTable( pkFolderNode ) == FALSE ) return FALSE;

		pkFolderNode = (NkDiskNode_Folder*)pkFolderNode->m_pkNext;
	}

	// 파일명 기록하기
	NkDiskNode_File* pkFileNode = pkNode->m_pkFirstFile;
	while( pkFileNode )
	{
		iSize = (int)strlen( pkFileNode->m_pcFileName ) + 1;
		WriteFile( m_hDiskFile, pkFileNode->m_pcFileName, iSize, &dwWritten, 0 );

		// 파일 쓰기에 실패하면, FALSE 반환
		if( iSize != dwWritten ) return FALSE;

		pkFileNode = (NkDiskNode_File*)pkFileNode->m_pkNext;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 특정 폴더와 하위 폴더를 모두 패킹한다
//-----------------------------------------------------------------------------
void NkDisk::PackFiles( LPCSTR szWorkFolderName )
{
	// 작업 폴더 설정
	if( szWorkFolderName )
	{
		SetWorkFolder( szWorkFolderName );
	}

	// 트리 및 해쉬 테이블 구성
	MakeFoldersTree( m_kWorkFolderName, m_pkRootNode );

	// 파일 쓰기 
	AttachFilesOnTree( m_kWorkFolderName, m_pkRootNode );
}


//-----------------------------------------------------------------------------
// 파일을 추가 한다
//-----------------------------------------------------------------------------
BOOL NkDisk::AddFile( const char* szSourceFileName, const char* szTargetFileName )
{

	HANDLE hFile = CreateFile( szSourceFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	// 폴더 찾기, 없으면 만들기
	NkCharsA< 256 > kStr;
	kStr = szTargetFileName;

	char* pStr1 = kStr;
	char* pStr2 = kStr;

	NkDiskNode_Base** ppkNode = (NkDiskNode_Base**)&m_pkRootNode;

	NkDiskNode_Folder* pkFolderNode;

	while( *pStr2 )
	{
		if( *pStr2 == '\\' || *pStr2 == '/' )
		{
			*pStr2 = '\0';

			if( NkIsExactKindOf( NkDiskNode_Folder, (*ppkNode) ) )
			{
				pkFolderNode = (NkDiskNode_Folder*)(*ppkNode);
				ppkNode = m_pkFileMap->GetAt( pStr1, (NkDiskNode_Base**)&pkFolderNode->m_pkFirstFolder );

				// 폴더가 없으면 생성한다
				if( !ppkNode ) 
				{
					pkFolderNode = AddNode_Folder( pStr1, pkFolderNode );
					if( !pkFolderNode )
					{
						CloseHandle( hFile );
						return FALSE;
					}

					ppkNode = (NkDiskNode_Base**)&pkFolderNode;
				}
			}
			else
			{
				CloseHandle( hFile );
				return FALSE;
			}

			pStr1 = ++pStr2;
			continue;
		}
		++pStr2;
	}

	// 파일 노드 추가
	if( NkIsExactKindOf( NkDiskNode_Folder, (*ppkNode) ) )
	{
		pkFolderNode = (NkDiskNode_Folder*)*ppkNode;

		NkDiskNode_File* pkFileNode = AddNode_File( pStr1, pkFolderNode );

		if( !pkFileNode )
		{
			CloseHandle( hFile );
			return FALSE;
		}

		// 파일 붙이기
		if( AttachFile( hFile, pkFileNode ) )
		{
			CloseHandle( hFile );
			return TRUE;
		}

		CloseHandle( hFile );
		return FALSE;
	}

	CloseHandle( hFile );
	return FALSE;
}



//-----------------------------------------------------------------------------
// 파일을 삭제 한다
//-----------------------------------------------------------------------------
BOOL NkDisk::DeleteFile( const char* szFileName )
{
	NkDiskNode_File* pkFileNode = FindFile( szFileName );

	if( !pkFileNode ) return FALSE;

	NkDiskEmptyBlockInfo* pkEmptyBlockInfo;

	pkEmptyBlockInfo = m_pkEmptyList->NewFront();

	pkEmptyBlockInfo->m_llEmptyBlockOffset = pkFileNode->m_llFileDataOffset;
	pkEmptyBlockInfo->m_llEmptyBlockSize = pkFileNode->m_dwFileSize;

	return RemoveNode_File( pkFileNode );
}


//-----------------------------------------------------------------------------
// 작업 폴더를 설정한다
//-----------------------------------------------------------------------------
BOOL NkDisk::SetWorkFolder( const char* szFolderName )
{
	m_kWorkFolderName = szFolderName;
	return TRUE;
}


//-----------------------------------------------------------------------------
// 디스크(패키징 파일) 정보를 로드한다 (헤더, 스트링 테이블, 파일 리스트)
//-----------------------------------------------------------------------------
BOOL NkDisk::LoadDiskInfos()
{
	DWORD dwRead;
	LARGE_INTEGER liOffset;

	// Root Node 생성
	m_pkRootNode = NewNode_Folder();
	m_pkRootNode->SetFileName("");

	// 헤더 생성
	m_pkFileHeader = NkNew NkDiskHeader;

	// 헤더 읽기
	SetFilePointer( m_hDiskFile, 0, 0, FILE_BEGIN );
	ReadFile( m_hDiskFile, m_pkFileHeader, sizeof(NkDiskHeader), &dwRead, 0 );

	m_llEndOfFileDataOffset = m_pkFileHeader->m_llFileDataOffset + m_pkFileHeader->m_llFileDataSize;

	// 읽기 전용이 아닌 경우, 빈 블럭 리스트 읽기
	if( !m_bReadOnly )
	{
		liOffset.QuadPart = m_pkFileHeader->m_llEmptyBlockListOffset;
		SetFilePointerEx( m_hDiskFile, liOffset, 0, FILE_BEGIN );
		ReadEmptyBlockList();
	}

	// 해쉬 맵 생성 
	DWORD size = m_pkFileHeader->m_dwTotalFileCount + m_pkFileHeader->m_dwTotalFolderCount;
	size /= 3;

	m_pkFileMap = NkNew NkDiskFileMap( size );

	// 스트링 테이블 생성
	m_pStringTable = new char[ m_pkFileHeader->m_dwStringTableSize ];

	// 스트링 테이블 읽기
	liOffset.QuadPart = m_pkFileHeader->m_llStringTableOffset;
	SetFilePointerEx( m_hDiskFile, liOffset, 0, FILE_BEGIN );
	ReadFile( m_hDiskFile, m_pStringTable, m_pkFileHeader->m_dwStringTableSize, &dwRead, 0 );

	// 파일 리스트 읽기
	liOffset.QuadPart = m_pkFileHeader->m_llFileListOffset;
	SetFilePointerEx( m_hDiskFile, liOffset, 0, FILE_BEGIN );
	ReadFileList( m_pkRootNode, m_pkFileHeader->m_dwRootFolderCount, m_pkFileHeader->m_dwRootFileCount );

	m_pStringTable.Delete();

	return TRUE;
}

//-----------------------------------------------------------------------------
// 디스크(패키징 파일) 정보를 저장한다 (파일 리스트, 스트링 테이블, 헤더)
//-----------------------------------------------------------------------------
BOOL NkDisk::SaveDiskInfos()
{
	m_dwCurrentStringPos = 0;

	// 파일 리스트 기록
	LARGE_INTEGER liOffset;
	liOffset.QuadPart = m_llEndOfFileDataOffset;
	SetFilePointerEx( m_hDiskFile, liOffset, 0, FILE_BEGIN );

	WriteFileList( m_pkRootNode );

	// 스트링 테이블 기록
	WriteStringTable( m_pkRootNode );

	// 빈 블럭 리스트 기록
	WriteEmptyBlockList();

	// 헤더 기록
	m_pkFileHeader->m_dwRootFolderCount = m_pkRootNode->m_dwFolderCount;
	m_pkFileHeader->m_dwRootFileCount = m_pkRootNode->m_dwFileCount;

	m_pkFileHeader->m_llFileDataSize = m_llEndOfFileDataOffset - m_pkFileHeader->m_llFileDataOffset;

	m_pkFileHeader->m_llFileListOffset = m_llEndOfFileDataOffset;
	m_pkFileHeader->m_llStringTableOffset = m_pkFileHeader->m_llFileListOffset + m_pkFileHeader->m_dwFileListSize;
	m_pkFileHeader->m_dwStringTableSize = m_dwCurrentStringPos;

	m_pkFileHeader->m_llEmptyBlockListOffset = m_pkFileHeader->m_llStringTableOffset + m_pkFileHeader->m_dwStringTableSize;

	SetFilePointer( m_hDiskFile, 0, 0, FILE_BEGIN );
	
	DWORD dwWritten;
	WriteFile( m_hDiskFile, m_pkFileHeader, sizeof(NkDiskHeader), &dwWritten, 0 );

	return TRUE;
}

//-----------------------------------------------------------------------------
// 파일을 찾는다
//-----------------------------------------------------------------------------
NkDiskNode_File* NkDisk::FindFile( const char* szPath )
{
	NkCharsA< 256 > kStr;
	kStr = szPath;

	char* pStr1 = kStr;
	char* pStr2 = kStr;

	NkDiskNode_Base** ppkNode = (NkDiskNode_Base**)&m_pkRootNode;

	NkDiskNode_Folder* pkFolderNode;

	while( *pStr2 )
	{
		if( *pStr2 == '\\' || *pStr2 == '/' )
		{
			*pStr2 = '\0';

			if( NkIsExactKindOf( NkDiskNode_Folder, (*ppkNode) ) )
			{
				pkFolderNode = (NkDiskNode_Folder*)(*ppkNode);
				ppkNode = m_pkFileMap->GetAt( pStr1, (NkDiskNode_Base**)&pkFolderNode->m_pkFirstFolder );
				if( !ppkNode ) return NULL;
			}
			else
			{
				return NULL;
			}

			pStr1 = ++pStr2;
			continue;
		}
		++pStr2;
	}

	if( NkIsExactKindOf( NkDiskNode_Folder, (*ppkNode) ) )
	{
		pkFolderNode = (NkDiskNode_Folder*)*ppkNode;
		ppkNode = m_pkFileMap->GetAt( pStr1, (NkDiskNode_Base**)&pkFolderNode->m_pkFirstFile );

		if( ppkNode )
		{
			if( NkIsExactKindOf( NkDiskNode_File, (*ppkNode) ) )
			{
				return (NkDiskNode_File*)(*ppkNode);
			}
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// 폴더를 찾는다
//-----------------------------------------------------------------------------
NkDiskNode_Folder* NkDisk::FindFolder( const char* szPath )
{
	NkCharsA< 256 > kStr;
	kStr = szPath;

	char* pStr1 = kStr;
	char* pStr2 = kStr;

	NkDiskNode_Base** ppkNode = (NkDiskNode_Base**)&m_pkRootNode;

	NkDiskNode_Folder* pkFolderNode;

	while( *pStr2 )
	{
		if( *pStr2 == '\\' || *pStr2 == '/' )
		{
			*pStr2 = '\0';

			if( NkIsExactKindOf( NkDiskNode_Folder, (*ppkNode) ) )
			{
				pkFolderNode = (NkDiskNode_Folder*)(*ppkNode);
				ppkNode = m_pkFileMap->GetAt( pStr1, (NkDiskNode_Base**)&pkFolderNode->m_pkFirstFolder );
				if( !ppkNode ) return NULL;
			}
			else
			{
				return NULL;
			}

			pStr1 = ++pStr2;
			continue;
		}
		++pStr2;
	}

	if( NkIsExactKindOf( NkDiskNode_Folder, (*ppkNode) ) )
	{
		pkFolderNode = (NkDiskNode_Folder*)*ppkNode;
		ppkNode = m_pkFileMap->GetAt( pStr1, (NkDiskNode_Base**)&pkFolderNode->m_pkFirstFolder );

		if( ppkNode )
		{
			if( NkIsExactKindOf( NkDiskNode_Folder, (*ppkNode) ) )
			{
				return (NkDiskNode_Folder*)(*ppkNode);
			}
		}
	}

	return NULL;
}


BOOL NkDisk::ReadFileData( NkDiskNode_File* pkFileNode, DWORD dwPosition, LPVOID lpBuffer, DWORD dwBytesToRead, LPDWORD lpBytesRead )
{
	LARGE_INTEGER liOffset;
	liOffset.QuadPart = pkFileNode->m_llFileDataOffset;

	liOffset.QuadPart += sizeof(NkDiskFileDesc) + dwPosition;

	SetFilePointerEx( m_hDiskFile, liOffset, 0, FILE_BEGIN );

	ReadFile( m_hDiskFile, lpBuffer, dwBytesToRead, lpBytesRead, 0 );

	return TRUE;
}


BOOL NkDisk::GetEmptyBlock( DWORD dwSize, LONGLONG* pllOffset )
{
	if( !m_pkEmptyList ) 
	{
		*pllOffset = 0;
		return FALSE;
	}

	NkDiskEmptyBlockInfo* pkEmptyBlock;
	
	NkListPos kPos = m_pkEmptyList->GetFrontPos();

	while( NULL != (pkEmptyBlock = m_pkEmptyList->GetDataPtr( kPos )) )
	{
		if( pkEmptyBlock->m_llEmptyBlockSize >= dwSize ) 
		{
			*pllOffset = pkEmptyBlock->m_llEmptyBlockOffset;

			if( pkEmptyBlock->m_llEmptyBlockSize == dwSize )
			{
				m_pkEmptyList->Remove( kPos );
			}
			else
			{
				pkEmptyBlock->m_llEmptyBlockOffset += dwSize;
				pkEmptyBlock->m_llEmptyBlockSize -= dwSize;
			}

			return TRUE;
		}

		m_pkEmptyList->MoveNext( kPos );
	}
	
	*pllOffset = 0;
	return NULL;
}

void NkDisk::GetTotalEmptyBlockSize( LONGLONG* pllSize )
{
	if( !m_pkEmptyList )
	{
		*pllSize = 0;
		return;
	}

	LONGLONG llSize = 0;

	NkDiskEmptyBlockInfo* pkEmptyBlock;

	NkListPos kPos = m_pkEmptyList->GetFrontPos();

	while( NULL != (pkEmptyBlock = m_pkEmptyList->GetDataPtr( kPos )) )
	{
		llSize += pkEmptyBlock->m_llEmptyBlockSize;
		m_pkEmptyList->MoveNext( kPos );
	}

	*pllSize = llSize;
}

