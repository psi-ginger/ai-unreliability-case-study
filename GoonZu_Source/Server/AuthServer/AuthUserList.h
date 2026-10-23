#ifndef _AUTHUSERLIST_H
#define _AUTHUSERLIST_H

#include "AuthServer.h"
#include "ITZList.h"

class AuthUserList
{
public:
	AuthUserList();
	~AuthUserList();

	void	CreateList( int maxSize );

	UI16	RegUser( cUserInfo &userinfo );
	BOOL	ChkUser( cUserInfo &userinfo );

public:
	ITZList< cUserInfo >	m_authUserList;

	CRITICAL_SECTION		m_cs;
};


#endif