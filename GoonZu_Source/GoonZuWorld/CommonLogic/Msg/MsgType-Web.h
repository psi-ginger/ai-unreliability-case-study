#ifndef _MSGTYPEWEB_H
#define _MSGTYPEWEB_H

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Web.h"

#define MAX_PUCHIKOMI_URL 516		// 아이디( 256 )+ 암호( 256 )+ 아이디와 암호를 구분하는 문자( 4 )


//-------------------------------------------
// 뉴스 정보 통보. 
//-------------------------------------------
class cltGameMsgResponse_News{
public:
//	bool	bBroadcastSwitch;						// 방송이 진행중인지 여부. 
	
	TCHAR	szNewsTitle[MAX_NEWS_TITLE_LENGTH];		// 뉴스 머릿기사. 
	TCHAR	szNewspaperName[MAX_NEWSPAPER_NAME_LENGTH];	// 신문사 이름. 
	TCHAR	szNewURL[MAX_PATH];							// 뉴스 링크되는 URL

	SI32	siNewsKind;	// 뉴스의 종류

	cltGameMsgResponse_News(TCHAR* ptitle, TCHAR* pnewspaper, TCHAR* pnewsurl ,SI32 newskind )
	{
		MStrCpy( szNewsTitle, ptitle, MAX_NEWS_TITLE_LENGTH );
		MStrCpy( szNewspaperName, pnewspaper, MAX_NEWSPAPER_NAME_LENGTH );
		MStrCpy( szNewURL , pnewsurl, MAX_PATH );

//		bBroadcastSwitch	= bbroadcast;

		siNewsKind = newskind ;
	}
};


//-------------------------------------------
// 푸치코미에 로그인된 주소를 얻어온다. (자신)
//-------------------------------------------
class cltGameMsgRequest_MyPuchikomiURL{
public:
	
	TCHAR				szAccountID[MAX_PLAYER_NAME] ; // 자신의 계정 ID
	TCHAR				szPassWord[FILE_NAME_SIZE] ;	// 자신의 계정 PASSWORD

	cltGameMsgRequest_MyPuchikomiURL(TCHAR* account, TCHAR* password)
	{
		MStrCpy(szAccountID,account,MAX_PLAYER_NAME);
		MStrCpy(szPassWord,password,FILE_NAME_SIZE);
	}
};

class cltGameMsgResponse_MyPuchikomiURL{
public:

	TCHAR	szMyPuchikomiURL[MAX_PUCHIKOMI_URL];
	
    cltGameMsgResponse_MyPuchikomiURL(TCHAR * myPuchikomiURL)
	{
		MStrCpy(szMyPuchikomiURL,myPuchikomiURL,MAX_PUCHIKOMI_URL);
	}

};



//-------------------------------------------
// 다른 유저의 푸치코미 주소를 알아온다
//-------------------------------------------
class cltGameMsgRequest_UserPuchikomiURL{
public:
	SI32 siToPersonCharUnique;
	TCHAR szToPersonName[ MAX_PLAYER_NAME ];

	cltGameMsgRequest_UserPuchikomiURL( SI32 ToPersonCharUnique )
	{
		siToPersonCharUnique = ToPersonCharUnique;
		szToPersonName[ 0 ] = NULL;
	}

	cltGameMsgRequest_UserPuchikomiURL( TCHAR *szname )
	{
		siToPersonCharUnique = 0;
		MStrCpy( szToPersonName, szname, MAX_PLAYER_NAME );
		szToPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
	}
};


class cltGameMsgResponse_UserPuchikomiURL{
public:

	TCHAR	szUserPuchikomiURL[MAX_PUCHIKOMI_URL];

	cltGameMsgResponse_UserPuchikomiURL(TCHAR * userPuchikomiURL)
	{
		MStrCpy(szUserPuchikomiURL,userPuchikomiURL,MAX_PUCHIKOMI_URL);
	}

};


#endif
