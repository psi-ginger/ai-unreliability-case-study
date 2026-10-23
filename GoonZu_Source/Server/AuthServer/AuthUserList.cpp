#include "AuthUserList.h"

AuthUserList::AuthUserList()
{
	InitializeCriticalSection( &m_cs );
}

AuthUserList::~AuthUserList()
{
	DeleteCriticalSection( &m_cs );
}

void AuthUserList::CreateList( int maxSize )
{
	
	m_authUserList.CreateList( maxSize, TRUE );
}

UI16 AuthUserList::RegUser( cUserInfo &userinfo )
{

	EnterCriticalSection( &m_cs );

	BOOL	  bRegUser = TRUE;

	cUserInfo *pUserInfo;

	LIST_LOOP( &m_authUserList, pUserInfo ) 
	{
		if( userinfo.idnum == pUserInfo->idnum ) {
			bRegUser = FALSE;
			break;
		}
	}

	if( bRegUser ) pUserInfo = m_authUserList.push();

	if( pUserInfo == NULL ) {

		LeaveCriticalSection( &m_cs );

		return 0;
	}

	memcpy( pUserInfo->userid, userinfo.userid, 20 );
	pUserInfo->userid[ 19 ] = NULL;

	memcpy( pUserInfo->userpw, userinfo.userpw, 20 );
	pUserInfo->userpw[ 19 ] = NULL;

	pUserInfo->idnum = userinfo.idnum;

	pUserInfo->siGameMasterLevel = userinfo.siGameMasterLevel;

	pUserInfo->usAuthKey = pUserInfo->m_usIndex;

	pUserInfo->dwLastTick = GetTickCount();
	
	LeaveCriticalSection( &m_cs );

	// AuthKey¸¦ ¹ÝÈ¯
	return pUserInfo->usAuthKey;
}

BOOL AuthUserList::ChkUser( cUserInfo &userinfo )
{
	EnterCriticalSection( &m_cs );

	BOOL bRet = TRUE;
	cUserInfo *pSrc;

	int idnum = 0;

	pSrc = m_authUserList.get( userinfo.usAuthKey );

	printf( "Chk: authkey:%d\n", userinfo.usAuthKey );

	if( pSrc == NULL ) bRet = FALSE;
	else if( strcmp( pSrc->userid, userinfo.userid ) != 0 ) bRet = FALSE;
	else if( strcmp( pSrc->userpw, userinfo.userpw ) != 0 ) bRet = FALSE;

	if( bRet ) {

		pSrc->userid[ 0 ] = NULL;
		pSrc->userpw[ 0 ] = NULL;

		userinfo.siGameMasterLevel = pSrc->siGameMasterLevel;
		
		idnum = pSrc->idnum;

		m_authUserList.remove( pSrc );
	}

	LeaveCriticalSection( &m_cs );

	return idnum;
}

