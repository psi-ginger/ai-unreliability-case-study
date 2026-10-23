#pragma once

#include <windows.h>
#include <time.h>
#include <stdio.h>

#define	GMA_V1_VERSION			100			//	0.01
#define	GMA_V2_VERSION			200			//	0.02

#define	MAX_GMA_DATANUM_V1		6000
#define	MAX_GMA_DATANUM_V2		6000

enum	eGMAActionType{ GMA_ACTION_TYPE_NONE, GMA_ACTION_TYPE_MOUSEMOVE };

//==============================
// Version 1
struct	strGMAHeader_V1
{
	LONG		lVersion;
	DWORD		starttime;		
	CHAR		szCharName[ 32 ];
	LONG		lLevel;
	LONG		lTotalDataNum;
};

struct	strGMAData_V1
{
	DWORD		dwtime;

	LONG		lAction;
	LONG		lParam1;
};
//==============================


//==============================
// Version 2
struct	strGMAHeader_V2
{
	LONG		lVersion;	

	DWORD		starttime;		
	CHAR		szCharName[ 32 ];
	LONG		lLevel;	
	
	time_t		t_recoredtime;
	CHAR		szIP[ 32 ];
	CHAR		szServerName[ 16 ];
	LONG		lPersonID;
	CHAR		szAccountID[ 32 ];
	CHAR		szMapName[ 32 ];
	LONG		lmapx;
	LONG		lmapy;

	LONG		lTotalDataNum;
};

typedef	strGMAData_V1	strGMAData_V2;
//==============================

// GMA -> Goonzu Macro Analysis
class	cltGMA		
{
private:

	strGMAHeader_V2		m_header;
	strGMAData_V2		m_gmadata[ MAX_GMA_DATANUM_V1 ];


protected:

	BOOL	LoadGMAFile_V1( FILE *fp );
	BOOL	WriteGMAFile_V1( FILE *fp );

	BOOL	LoadGMAFile_V2( FILE *fp );
	BOOL	WriteGMAFile_V2( FILE *fp );

public:
	cltGMA();
	~cltGMA();

	VOID	Initialize();									// 쓰기용
	BOOL	Initialize( CHAR *pszFileName );				// 읽기용
		
	VOID	SetHeader_V1( CHAR *pszCharName, LONG level );
	VOID	SetHeader_V2( CHAR *charname, LONG level, time_t recordtime, CHAR *ip, CHAR *servername, LONG personid, CHAR *accountid, CHAR *mapname, LONG map_x, LONG map_y );

	VOID	Free();		
	
    BOOL	Add( LONG lActionType, 	LONG lParam1 );	

	BOOL	Write( CHAR *pszFileName );
	

	strGMAHeader_V2*	GetHeader();
	strGMAData_V2*		GetGMADataByIndex( LONG lIndex );
	strGMAData_V2*		GetGMADataByTime( DWORD dwtime );
};


