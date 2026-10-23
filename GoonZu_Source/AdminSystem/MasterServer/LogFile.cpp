// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "LogFile.h"
#include "ServerInfo.h"
#include "MasterServer.h"

CLogFile*	logFile;

extern ServerInfo	g_serverinfo;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile()
{
	InitializeCriticalSection( &m_cs );

	m_iLastNum = 0 ;
	m_iOldMin = 0 ;

	Clear();

	m_bWrite = FALSE ;
}

CLogFile::~CLogFile()
{
	DeleteCriticalSection( &m_cs );
}

void CLogFile::Clear()
{
	for ( int i = 0 ; i < 288 ; i ++ )
		m_iServerTotalUseNum[i] = 0 ;;

	for ( i = 0 ; i < 100 ; i ++ )
		for ( int j = 0 ; j < 288 ; j ++ )
		m_iServer[i][j] = 0 ;

	for ( i = 0 ; i < 288 ; i ++ )
		for ( int j = 0 ; j < 24 ; j ++)
		m_chDate[i][j] = NULL ;
}

void CLogFile::Init()
{
	ZeroMemory(&m_sTime, sizeof(_SYSTEMTIME));
	InitializeCriticalSection( &m_cs );
	GetLocalTime(&m_sTime);
	CriTicalWait();
	ReadLogFile();
	CriTicalRelease();

	if ( ParallelFile() )
	{
		m_iOldMin = m_sTime.wMinute ;
		m_bWrite = TRUE ;
	}
}

void CLogFile::Run()
{
	GetLocalTime(&m_sTime);

	if ( m_sTime.wMinute % 5 == 0 && !m_bWrite )
	{
		CriTicalWait();
		WriteLogFile();
		m_iOldMin = m_sTime.wMinute ;
		m_bWrite = TRUE ;
		CriTicalRelease();
	}

	if ( m_bWrite && m_iOldMin < m_sTime.wMinute )
		m_bWrite = FALSE ;
}

void CLogFile::ReadLogFile()
{
	char buf[24] = "";
	FILE * fp = NULL ;
	char fileBuf[2];
	char month[4],day[4];

	Clear();

	if ( m_sTime.wMonth < 10 )
		wsprintf(month,"0%d",m_sTime.wMonth);
	else
		wsprintf(month,"%d",m_sTime.wMonth);
	if ( m_sTime.wDay < 10 )
		wsprintf(day,"0%d",m_sTime.wDay);
	else
		wsprintf(day,"%d",m_sTime.wDay);

	wsprintf(buf,"%d%s%s.txt",m_sTime.wYear,month,day);
	fp = fopen(buf,"a+t");
	if ( fp != NULL )
	{
		for ( int i = 0 ; i < 288 ; i ++ ) {

			for ( int j = 0 ; j < g_serverinfo.m_usWorldCount ; j ++ )
			{
				fread(fileBuf,sizeof(char),1,fp);
				if ( fileBuf[0] == 10 || feof(fp) )
				{
					m_iLastNum = i - 1 ;
					if ( m_iLastNum < 0 )
						m_iLastNum = 0 ;
					fclose(fp);
					return ;
				}
				fseek(fp,-1,SEEK_CUR);

				if ( j == 0 )
				{
					char dateall[24];
					char backdate[24];
					int h , m ;
					char * date;
					fscanf(fp,"%s",dateall);
					strcpy(backdate,dateall);
					date = strtok(dateall,"-");
					date = strtok(NULL,"-");
					date = strtok(NULL,"-");
					date = strtok(NULL,"-");
					h = atoi(date);
					date = strtok(NULL,"-");
					m = atoi(date);
					i = ( h ) * 12 + ( m/5 ) ;
					strcpy(m_chDate[i],backdate);
				}

				int read = fscanf(fp,"%d",&m_iServer[j][i]);
				m_iServerTotalUseNum[i] += m_iServer[j][i] ;

				fread(fileBuf,sizeof(char),1,fp);
				if ( fileBuf[0] == 10 )		// 줄바꿈 문자를 만난경우 처리 - 파일의 월드수가 현재 월드수보다 적은 경우
					j = g_serverinfo.m_usWorldCount;
			}

			// 나머지 줄바꿈 문자까지 처리한다. - 파일의 월드수가 현재 월드수보다 많은 경우
			while( fileBuf[0] != 10 )	{		fread(fileBuf,sizeof(char),1,fp);		}

			if( g_maxUser < m_iServerTotalUseNum[i] ) InterlockedExchange( (LPLONG)&g_maxUser, m_iServerTotalUseNum[i] );
		}
	}
}

BOOL CLogFile::ReadLogFileEX(int year,int month,int day)
{
	char buf[24] = "";
	FILE * fp = NULL ;
	char fileBuf[2];
	char Month[4],Day[4];

	Clear();

	if ( month < 10 )
		wsprintf(Month,"0%d",month);
	else
		wsprintf(Month,"%d",month);
	if ( day < 10 )
		wsprintf(Day,"0%d",day);
	else
		wsprintf(Day,"%d",day);

	wsprintf(buf,"%d%s%s.txt",year,Month,Day);
	fp = fopen(buf,"a+t");
	if ( fp != NULL )
	{
		for ( int i = 0 ; i < 288 ; i ++ )
		{
			for ( int j = 0 ; j < g_serverinfo.m_usWorldCount ; j ++ )
			{
				fread(fileBuf,sizeof(char),1,fp);
				if ( fileBuf[0] == 10 || feof(fp) )
				{
					fclose(fp);
					return TRUE;
				}
				fseek(fp,-1,SEEK_CUR);

				if ( j == 0 )
				{
					char dateall[24];
					char backdate[24];
					int h , m ;
					char * date;
					fscanf(fp,"%s",dateall);
					strcpy(backdate,dateall);
					date = strtok(dateall,"-");
					date = strtok(NULL,"-");
					date = strtok(NULL,"-");
					date = strtok(NULL,"-");
					h = atoi(date);
					date = strtok(NULL,"-");
					m = atoi(date);
					i = ( h ) * 12 + ( m/5 ) ;
					strcpy(m_chDate[i],backdate);
				}

				fscanf(fp,"%d",&m_iServer[j][i]);
				m_iServerTotalUseNum[i] += m_iServer[j][i] ;
				
				fread(fileBuf,sizeof(char),1,fp);
				if ( fileBuf[0] == 10 ) break;

			}
		} 

	} else {
		return FALSE;
	}

	return TRUE;
}

void CLogFile::WriteLogFile()
{
	char buf[24] = "";
	FILE	* fp = NULL ;
	char fileBuf[32] = "";
	char month[4],day[4],hour[4],minute[4];

	if ( m_sTime.wMonth < 10 )
		wsprintf(month,"0%d",m_sTime.wMonth);
	else
		wsprintf(month,"%d",m_sTime.wMonth);
	if ( m_sTime.wDay < 10 )
		wsprintf(day,"0%d",m_sTime.wDay);
	else
		wsprintf(day,"%d",m_sTime.wDay);
	if ( m_sTime.wHour < 10 )
		wsprintf(hour,"0%d",m_sTime.wHour);
	else
		wsprintf(hour,"%d",m_sTime.wHour);
	if ( m_sTime.wMinute < 10 )
		wsprintf(minute,"0%d",m_sTime.wMinute);
	else
		wsprintf(minute,"%d",m_sTime.wMinute);

	wsprintf(buf,"%d%s%s.txt",m_sTime.wYear,month,day);
	fp = fopen(buf,"a+t");
	fseek(fp,0,SEEK_END);
	for ( int i = 0 ; i < g_serverinfo.m_usWorldCount ; i ++ )
	{
		if ( i == 0 )
		{
			wsprintf(fileBuf,"%d-%s-%s-%s-%s\t",m_sTime.wYear,month,day,hour,minute);
			fwrite(fileBuf,strlen(fileBuf),1,fp);
		}

		if ( i == g_serverinfo.m_usWorldCount - 1 )
		{
			wsprintf(fileBuf,"%d\n",g_serverinfo.m_gameRefresh[i].usCurrentUserNum);
			fwrite(fileBuf,strlen(fileBuf),1,fp);
		}
		else
		{
			wsprintf(fileBuf,"%d\t",g_serverinfo.m_gameRefresh[i].usCurrentUserNum);
			fwrite(fileBuf,strlen(fileBuf),1,fp);
		}
	}
	fclose(fp);
}

BOOL CLogFile::ParallelFile()
{
	char fileBuf[24] = "" ;
	char month[4],day[4],hour[4],minute[4];

	if ( m_sTime.wMonth < 10 )
		wsprintf(month,"0%d",m_sTime.wMonth);
	else
		wsprintf(month,"%d",m_sTime.wMonth);
	if ( m_sTime.wDay < 10 )
		wsprintf(day,"0%d",m_sTime.wDay);
	else
		wsprintf(day,"%d",m_sTime.wDay);
	if ( m_sTime.wHour < 10 )
		wsprintf(hour,"0%d",m_sTime.wHour);
	else
		wsprintf(hour,"%d",m_sTime.wHour);
	if ( m_sTime.wMinute < 10 )
		wsprintf(minute,"0%d",m_sTime.wMinute);
	else
		wsprintf(minute,"%d",m_sTime.wMinute);

	wsprintf(fileBuf,"%d-%s-%s-%s-%s",m_sTime.wYear,month,day,hour,minute);
	if ( !strcmp(fileBuf , m_chDate[m_iLastNum]))
		return TRUE ;
	else return FALSE ;
}