/*
//////////////////////////////////////////////////////////////////////////

//	NPathString.h

//	2006-11-10 11:06
//	Lee Seunghun <tsoi@ndoors.net> / <tsoi@naver.com>
*/

#pragma once

#include "NDataTypes.h"
#include "NString.h"

#ifndef MAX_PATH
#define MAX_PATH		512
#endif

//	Minimum DLL Version :		shlwapi.dll version 4.71 or later 
//	Custom :					Implementation No 
//	Header :					shlwapi.h 
//	Import library :			shlwapi.lib 
//	Minimum operating systems :	Windows 2000, Windows NT 4.0 with Internet Explorer 4.0, Windows 98, 
//								Windows 95 with Internet Explorer 4.0 

#pragma comment(lib, "shlwapi.lib")


class NPathString : public NString< MAX_PATH >
{
public:
	static int	MAX_PATH_SIZE; /* = MAX_PATH */

public:
	void		AddBackslash();
	BOOL		AddExtension( LPCTSTR pszExtension );
	BOOL		Append( LPCTSTR pszMore );
	BOOL		AppendFormat( LPCTSTR format, ... );
	void		Combine( LPCTSTR pszDir, LPCTSTR pszFile );

	void		Canonicalize();

	BOOL		IsDirectory() const;
	BOOL		IsRelative() const;
	BOOL		IsRoot() const;
	BOOL		IsUNC() const; // Universal Naming Convention (UNC) for a server and share path. 
	BOOL		IsSameRoot( LPCTSTR pszOther ) const;
	BOOL		IsPrefix( LPCTSTR pszPrefix ) const;

	BOOL		FileExists() const;
	LPCTSTR		FindExtension() const;
	LPCTSTR		FindFileName() const;

	void		RemoveArgs();
	void		RemoveBackslash();
	void		RemoveBlanks();
	void		RemoveFileSpec();
	BOOL		RemoveExceptForRoot();
	void		RemoveExtension();
	void		StripPath();

	BOOL		RenameFileName( LPCTSTR pszFileName );
	BOOL		RenameExtension( LPCTSTR pszExtension );

	LPCTSTR		SetRelativePath( LPCTSTR pszRelative );

	void		GetExtension( OUT LPTSTR pszOutput ) const;
	void		GetFileName( OUT LPTSTR pszOutput ) const;
	void		GetDrive( OUT LPTSTR pszOutput ) const;
	void		GetRelativePath( LPCTSTR pszRelative, OUT LPSTR pszOutput ) const;
	void		GetCommonPrefix( LPCTSTR pszOther, OUT LPSTR pszOutput ) const;
	int			GetDriveNumber() const;


	void		QuoteSpaces();
	void		UnquoteSpaces();

public:
	NPathString();
	NPathString( LPCTSTR str );
};
// 