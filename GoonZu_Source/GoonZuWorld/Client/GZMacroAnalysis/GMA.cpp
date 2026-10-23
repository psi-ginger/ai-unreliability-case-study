#include "GMA.h"

cltGMA::cltGMA()
{
	ZeroMemory( &m_header, sizeof( m_header ) );
	ZeroMemory( m_gmadata, sizeof( m_gmadata ) );
	
}

cltGMA::~cltGMA()
{
}


BOOL	cltGMA::LoadGMAFile_V1( FILE *fp )
{
	strGMAHeader_V1		header;
	strGMAData_V1		*data;

	// header
	if( ( fread( &header, sizeof( header ), 1, fp ) ) == 0 )														return	FALSE;	

	m_header.lVersion		=	header.lVersion;
	m_header.starttime		=	header.starttime;
	strcpy( m_header.szCharName, header.szCharName );
	m_header.lLevel			=	header.lLevel;
	m_header.lTotalDataNum	=	header.lTotalDataNum;

	data	=	new strGMAData_V1[ MAX_GMA_DATANUM_V1 ];

	if( ( fread( data, sizeof( strGMAData_V1 ), m_header.lTotalDataNum, fp ) ) != m_header.lTotalDataNum )
	{	
		delete [] data;
		data	=	NULL;

		return	FALSE;
	}

	memcpy( m_gmadata, data, sizeof( strGMAData_V1 ) * MAX_GMA_DATANUM_V1 );

	if( data )
	{
		delete [] data;
		data	=	NULL;
	}

	return	TRUE;
}


BOOL	cltGMA::LoadGMAFile_V2( FILE *fp )
{
	strGMAHeader_V2		header;
	strGMAData_V2		*data;

	// header
	if( ( fread( &header, sizeof( header ), 1, fp ) ) == 0 )														return	FALSE;	

	memcpy( &m_header, &header, sizeof( strGMAHeader_V2	) );

	data	=	new strGMAData_V2[ MAX_GMA_DATANUM_V2 ];

	if( ( fread( data, sizeof( strGMAData_V2 ), m_header.lTotalDataNum, fp ) ) != m_header.lTotalDataNum )
	{
		delete [] data;
		data	=	NULL;

		return	FALSE;
	}

	memcpy( m_gmadata, data, sizeof( strGMAData_V2 ) * MAX_GMA_DATANUM_V2 );

	if( data )
	{
		delete [] data;
		data	=	NULL;
	}

	return	TRUE;
}


VOID	cltGMA::Initialize()
{
	ZeroMemory( &m_header, sizeof( m_header ) );
	ZeroMemory( m_gmadata, sizeof( m_gmadata ) );	
}

VOID	cltGMA::SetHeader_V1( CHAR *pszCharName, LONG level )
{
	strcpy( m_header.szCharName, pszCharName );
	m_header.starttime		=	timeGetTime();
	m_header.lVersion		=	GMA_V1_VERSION;
	m_header.lLevel			=	level;
	m_header.lTotalDataNum	=	0;
}


VOID	cltGMA::SetHeader_V2( CHAR *charname, LONG level, time_t recordtime, CHAR *ip, CHAR *servername, LONG personid, CHAR *accountid, CHAR *mapname, LONG map_x, LONG map_y )
{
	ZeroMemory( &m_header, sizeof( m_header ) );

	m_header.lVersion		=	GMA_V2_VERSION;	

	m_header.starttime		=	timeGetTime();

	strcpy( m_header.szCharName, charname );

	m_header.lLevel			=	level;

	m_header.t_recoredtime	=	recordtime;

	strcpy( m_header.szIP, ip );

	strcpy( m_header.szServerName, servername );

	m_header.lPersonID		=	personid;

	strcpy( m_header.szAccountID, accountid );

	strcpy( m_header.szMapName, mapname );

	m_header.lmapx			=	map_x;
	m_header.lmapy			=	map_y;

	m_header.lTotalDataNum	=	0;
}

BOOL	cltGMA::Initialize( CHAR *pszFileName )
{
	FILE	*fp;
	BOOL	bSuccess = FALSE;
	LONG	lversion;

	if( ( fp = fopen( pszFileName, "rb" ) ) == NULL )				return	FALSE;

	if( ( fread( &lversion, sizeof( lversion ), 1, fp ) ) == 0 )	goto	END;

	fseek( fp, SEEK_SET , 0 );

	switch( lversion  )
	{
	case GMA_V1_VERSION:

		if( LoadGMAFile_V1( fp ) == FALSE )							goto	END;

		break;

	case GMA_V2_VERSION:

		if( LoadGMAFile_V2( fp ) == FALSE )							goto	END;

		break;

	default:

		goto	END;

		break;
	}

	bSuccess	=	TRUE;

END:

	if( fp )
	{
		fclose( fp );
		fp	=	NULL;
	}


	return	bSuccess;
}

VOID	cltGMA::Free()
{
	ZeroMemory( &m_header, sizeof( m_header ) );
	ZeroMemory( m_gmadata, sizeof( m_gmadata ) );
}
	

BOOL	cltGMA::Add( LONG lActionType, 	LONG lParam1 )
{
	if( m_header.lTotalDataNum >= MAX_GMA_DATANUM_V1 )							return	FALSE;

	m_gmadata[ m_header.lTotalDataNum ].dwtime	=	timeGetTime();
	m_gmadata[ m_header.lTotalDataNum ].lAction	=	lActionType;
	m_gmadata[ m_header.lTotalDataNum ].lParam1	=	lParam1;

	m_header.lTotalDataNum++;

	return	TRUE;
}

strGMAHeader_V2*	cltGMA::GetHeader()
{
	return	&m_header;
}

strGMAData_V1*		cltGMA::GetGMADataByIndex( LONG lIndex )
{
	if( lIndex < 0 || lIndex >= m_header.lTotalDataNum )			return	NULL;

	return	&m_gmadata[ lIndex ];
}

strGMAData_V1*		cltGMA::GetGMADataByTime( DWORD dwtime )
{
	LONG	i;
	DWORD	dwtimegap;
	LONG	index;

	for( i = 0; i < m_header.lTotalDataNum; i++ )
	{
		dwtimegap	=	m_gmadata[ i ].dwtime - m_header.starttime ;
		
		if( dwtimegap < dwtime )								continue;		

		break;
	}

	index	=	max( i - 1, 0 );

	return	GetGMADataByIndex( index );
}


BOOL	cltGMA::WriteGMAFile_V1( FILE *fp )
{
	strGMAHeader_V1		header;
	strGMAData_V1		*data;

	header.lVersion			=	m_header.lVersion;
	header.starttime		=	m_header.starttime;
	strcpy( header.szCharName, m_header.szCharName );
	header.lLevel			=	m_header.lLevel;
	header.lTotalDataNum	=	m_header.lTotalDataNum;

	if( fwrite( &header, sizeof( header ), 1, fp ) == 0 )										return	FALSE;

	data	=	new	strGMAData_V1[ header.lTotalDataNum ];
	
	memcpy( data, m_gmadata, sizeof( strGMAData_V1 ) * header.lTotalDataNum );

	if( fwrite( data, sizeof( strGMAData_V1 ), m_header.lTotalDataNum, fp ) == 0 )
	{
		delete [] data;
		data	=	NULL;

		return	FALSE;
	}

	if( data )
	{
		delete [] data;
		data	=	NULL;
	}

	return	TRUE;
}

BOOL	cltGMA::WriteGMAFile_V2( FILE *fp )
{
	strGMAHeader_V2		header;
	strGMAData_V2		*data;

	memcpy( &header, &m_header, sizeof( strGMAHeader_V2 ) );

	if( fwrite( &header, sizeof( header ), 1, fp ) == 0 )										return	FALSE;

	data	=	new	strGMAData_V2[ header.lTotalDataNum ];
	
	memcpy( data, m_gmadata, sizeof( strGMAData_V2 ) * header.lTotalDataNum );

	if( fwrite( data, sizeof( strGMAData_V2 ), m_header.lTotalDataNum, fp ) == 0 )
	{
		delete [] data;
		data	=	NULL;

		return	FALSE;
	}

	if( data )
	{
		delete [] data;
		data	=	NULL;
	}

	return	TRUE;
}

BOOL	cltGMA::Write( CHAR *pszFileName )
{
	FILE	*fp;
	BOOL	bSuccess = FALSE;

	if( ( fp = fopen( pszFileName, "wb" ) ) == NULL )				return	FALSE;

	switch( m_header.lVersion )	
	{
	case GMA_V1_VERSION:

		if( WriteGMAFile_V1( fp ) == FALSE )						goto	END;

		break;

	case GMA_V2_VERSION:

		if( WriteGMAFile_V2( fp ) == FALSE )						goto	END;

	break;
	}

	bSuccess	=	TRUE;

END:

	if( fp )
	{
		fclose( fp );
		fp	=	NULL;
	}

	return	bSuccess;
}