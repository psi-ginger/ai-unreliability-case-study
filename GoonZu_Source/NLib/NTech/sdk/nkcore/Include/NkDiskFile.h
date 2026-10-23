#ifndef _NKDISKFILE_H
#define _NKDISKFILE_H

class NkDisk;
class NkDiskNode_File;

class NkDiskFile
{
public:
	NkDiskFile( NkDisk& kDisk );

	BOOL OpenFile( const char* szFileName );
	void CloseFile();
	BOOL ReadFile( LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead );
	DWORD SetFilePointer( LONG lDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN );
	DWORD GetFileSize();
	
protected:
	NkDisk*	m_pkDisk;
	NkDiskNode_File* m_pkFileNode;
	DWORD m_dwCurrentPos;
};


#endif