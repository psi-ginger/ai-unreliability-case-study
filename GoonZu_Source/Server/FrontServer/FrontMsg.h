#ifndef _FRONTMSG_H
#define _FRONTMSG_H

#include "itzbase.h"

struct GAMESERVER_BASEINFO
{
	UI16	worldId;						// 월드 ID
	TCHAR	worldname[ 20 ];				// 월드명
	SI32	siServerID;						// 서버 ID
	TCHAR	gameserverip[ 16 ];				// 서버 IP
	UI16	gameserverport;					// 서버 PORT
	TCHAR	opendate[ 15 ];					// 서버 오픈 시간
	TCHAR	shortmessage[ 64 ];				// 짧은 안내글
};


struct GAMESERVER_REFRESHINFO
{
	UI16	worldId;						// 월드 ID
	UI16	state;							// 0:접속가능
	// 1:접속허용인원초과  2:점검중  3:오픈예정
	UI16	currentConnections;				// 동접유저숫자
};

struct GAMESERVER_GAMESERVERINFO
{
	TCHAR	goonzuname[ 20 ];				// 군주 캐릭터 이름 
	TCHAR	goonzutext[ 256 ];				// 서버 인사말 
	SI32	population;						// 백성수
	SI32	currentPrice;					// 시세
	SI32	npcrate;						//
	SI32	fixationmoney;					// 
};

struct WORLDINFO
{
	GAMESERVER_BASEINFO			baseinfo;		// 기본적인 정보
	GAMESERVER_REFRESHINFO		refreshinfo;	// 주기적으로 업데이트 되는 정보( 5초 정도 )
	GAMESERVER_GAMESERVERINFO	gameserverinfo;	// 게임서버 정보	
};

enum {
	FRONTMSG_REQUEST_LOGIN = 1, 
	FRONTMSG_REQUEST_NOTICE,
	FRONTMSG_REQUEST_WORLDLIST,
	FRONTMSG_REQUEST_PATCHINFO,
	FRONTMSG_REQUEST_PATCHTEXT,
	FRONTMSG_REQUEST_PATCHTEXT2,
	FRONTMSG_REQUEST_JOINGAME,

	FRONTMSG_HEARTBEAT,

	FRONTMSG_REQUEST_NETMARBLEJP_LOGIN,

	FRONTMSG_REQUEST_PINGTEST,
	FRONTMSG_RESPONSE_PINGTEST,
	
	FRONTMSG_REQUEST_HANAUTH_LOGIN,

	//FRONTMSG_REQUEST_HANGAMEJP_LOGIN,

	FRONTMSG_REQUEST_DIFFHELLMAN_KEY,

	FRONTMSG_REQUEST_TAIWAN_LOGIN,

	//---------------------------------------------------------
	//KHY - 0204 -  중국 PurpleAuth 인증 모듈
	FRONTMSG_REQUEST_PORTEAUTH_LOGIN,

	//---------------------------------------------------------
	//KHY - 0617 - 일본 채널링 추가.	
	FRONTMSG_REQUEST_HANGAMEJP_LOGIN,
	FRONTMSG_REQUEST_GAMANIAJP_LOGIN,
	FRONTMSG_REQUEST_GAMEPOTJP_LOGIN,
	FRONTMSG_REQUEST_NDEJP_LOGIN,
	FRONTMSG_REQUEST_NEXONJP_LOGIN,
	//---------------------------------------------------------

	// PCK : IOVATION 인증 추가 (2009.02.03)
	FRONTMSG_REQUEST_ENGLISH_LOGIN,

	// [진성] USA 새로운 인증.
	FRONTMSG_REQUEST_USA_LOGIN,

	// [영훈] EUROPE 인증 추가
	FRONTMSG_REQUEST_EUROPE_LOGIN,

	//	[종호_NHN->NDOORS] 인증 추가
	FRONTMSG_REQUEST_KOREA_LOGIN,

	//---------------------------------------------------------
	FRONTMSG_RESPONSE_LOGIN = 1000,
	FRONTMSG_RESPONSE_NOTICE,
	FRONTMSG_RESPONSE_WORLDLIST,
	FRONTMSG_RESPONSE_PATCHINFO,
	FRONTMSG_RESPONSE_PATCHTEXT,
	FRONTMSG_RESPONSE_PATCHTEXT2,
	FRONTMSG_RESPONSE_JOINGAME,

	FRONTMSG_RESPONSE_NETMARBLEJP_LOGIN,
	FRONTMSG_RESPONSE_DIFFHELLMAN_KEY,

	FRONTMSG_NOTIFY_MESSAGE = 9000,
	FRONTMSG_NOTIFY_WORLDUPDATE,
	FRONTMSG_NOTIFY_WORLDREFRESHALL,

	FRONTMSG_REPORT_GAMEHEARTBEAT = 10000,
	FRONTMSG_REQUEST_GAMESERVERINFO,
	FRONTMSG_RESPONSE_GAMESERVERINFO,

	FRONTMSG_RESPONSE_PROMOTER_INFO = 11000,

	FRONTMSG_RESPONSE_SECRETKEY = 30000
};

#pragma pack(1)


//-----------------------------------------------------------------------------
// HeartBeat
//-----------------------------------------------------------------------------
struct sResponse_HeartBeat
{
	sPacketHeader		packetHeader;

};

//-----------------------------------------------------------------------------
// 암호화키전송
//-----------------------------------------------------------------------------
struct sResponse_SecretKey
{
	sPacketHeader		packetHeader;

	UI08				ucTemp1;
	UI08				ucTemp2;
	UI08				ucKey1;
	UI08				ucTemp3;
	UI08				ucKey2;
};

//-----------------------------------------------------------------------------
// 게임 버전 정보 요청 (클라이언트 -> 프론트서버) 패치클라이언트 
//-----------------------------------------------------------------------------
struct sRequest_PatchInfo
{

	sPacketHeader		packetHeader;

	UI32				uiCurrentVersion;
};

//-----------------------------------------------------------------------------
// 게임 버전 정보 응답 (프론트서버 -> 클라이언트 ) 패치클라이언트  
//-----------------------------------------------------------------------------
struct sResponse_PatchInfo
{
	sPacketHeader		packetHeader;

	UI32				uiCount;
	UI32				uiVersionHistory[ 100 ];

	TCHAR				szAddr[ 128 ];						// 패치 서버 주소 
	TCHAR				szPath[ 128 ];						// 패치 서버 폴더 
	TCHAR				szID[ 32 ];							// 패치 서버 ID
	TCHAR				szPW[ 32 ];							// 패치 서버 PW
	UI16				usPort;								// 패치 서버 Port

	TCHAR				szAddr_2[ 128 ];						// 패치 서버2 주소 
	TCHAR				szPath_2[ 128 ];						// 패치 서버2 폴더 
	TCHAR				szID_2[ 32 ];							// 패치 서버2 ID
	TCHAR				szPW_2[ 32 ];							// 패치 서버2 PW
	UI16				usPort_2;								// 패치 서버2 Port


};


//-----------------------------------------------------------------------------
// 패치 텍스트 요청 (클라이언트 -> 프론트서버) 패치클라이언트 
//-----------------------------------------------------------------------------
struct sRequest_PatchText
{
	sPacketHeader		packetHeader;
};

//-----------------------------------------------------------------------------
// 패치 텍스트 요청2 (클라이언트 -> 프론트서버) 패치클라이언트 
//-----------------------------------------------------------------------------
struct sRequest_PatchText2
{
	sPacketHeader		packetHeader;
};


//-----------------------------------------------------------------------------
// 패치 텍스트 응답 (프론트서버 -> 클라이언트 ) 패치클라이언트  
//-----------------------------------------------------------------------------
struct sResponse_PatchText
{
	sPacketHeader		packetHeader;

	UI16				usTextSize;
	TCHAR				szText[ 25000 ];
};


//-----------------------------------------------------------------------------
// 패치 텍스트 응답 (프론트서버 -> 클라이언트 ) 패치클라이언트  
//-----------------------------------------------------------------------------
struct sResponse_PatchText2
{
	sPacketHeader		packetHeader;

	BOOL				IsLastPacket;

	UI16				usTextSize;
	TCHAR				szText[ 5000 ];
};


//-----------------------------------------------------------------------------
// 넷마블 로그인(복호화) 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sRequest_NetmarbleJP_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				userid[ 100 ];
	TCHAR				userpw[ 100 ];
	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// 대만 로그인 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sRequest_Taiwan_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				szGameAccount[ 30 ];
	TCHAR				szOneTimePassword[ 40 ];
	TCHAR				macaddress[ 20 ];
};


//-----------------------------------------------------------------------------
// 넷마블 로그인(복호화) 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sResponse_NetmarbleJP_LogIn
{
	sPacketHeader		packetHeader;

	SI32				siResult;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
};

//---------------------------------------------------------
//KHY - 0617 - 일본 채널링 추가.



//-----------------------------------------------------------------------------
// 한게임로그인 요청 (클라이언트 -> 프론트서버) - ID뒤에 [~@HG]  
//-----------------------------------------------------------------------------
struct sRequest_HANGAMEJP_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];

	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
//  감마니아 로그인 요청 (클라이언트 -> 프론트서버) - ID뒤에 [~@GA]  
//-----------------------------------------------------------------------------
struct sRequest_GAMANIAJP_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];

	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// 게임팟 로그인 요청 (클라이언트 -> 프론트서버) - ID뒤에 [~@GP] 
//-----------------------------------------------------------------------------
struct sRequest_GAMEPOTJP_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];

	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// NDE로그인 요청 (클라이언트 -> 프론트서버) - * ID뒤에 [~@ND] 삽입 시켜서 보내야 한다..* 
//-----------------------------------------------------------------------------
struct sRequest_NDEJP_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];

	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// 넥슨 로그인 요청 (클라이언트 -> 프론트서버) -ID안에 기타정보가 다 들어온다.(파싱은 뒤에 가서)
//-----------------------------------------------------------------------------
struct sRequest_NEXONJP_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				userid[ 512 ]; // 319 지만, 넉넉하게.
	TCHAR				userpw[ 20 ];

	TCHAR				macaddress[ 20 ];
};

//---------------------------------------------------------

//-----------------------------------------------------------------------------
// 로그인 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sRequest_LogIn
{
	sPacketHeader		packetHeader;

/*#ifdef _CHINA
	TCHAR				userid[ 40 ];
	TCHAR				userpw[ 40 ];
#else*/
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
//#endif
	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// HanAuth 로그인 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sRequest_HanAuth_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				authstring[ 2048 ];
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
//KHY - 0204 -  중국 PurpleAuth 인증 모듈
// 로그인 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sRequest_PorteAuth_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				authstring[ 2048 ];  // SIZE_AUTHTICKET 와 같아야 한다.
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
//  IOVATION 로그인 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sRequest_English_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				macaddress[ 20 ];

	TCHAR				Blackbox[ 2048 ];
};

//-----------------------------------------------------------------------------
// [진성] USA 새로운 인증. 
//-----------------------------------------------------------------------------
struct sRequest_USA_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				authstring[ 4096 ];

	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// [영훈] Europe 로그인 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sRequest_EUROPE_LogIn
{
	sPacketHeader		packetHeader;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];

	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// 로그인 응답 (프론트서버 -> 클라이언트)
//-----------------------------------------------------------------------------
struct sResponse_LogIn
{
	sPacketHeader		packetHeader;

	UI16				usResult;											// 0: 접속성공  1:아이디오류 2:패스워드오류 3:군주직원아님 4:인증서버오류 5.DB오류
	SI32				siAuthKey;

	// 넥슨 채널링을 위한 ID체크 -넥슨은 DB에서 ID가 변환되어 들어와야 한다.
	char			userid[ 20 ];
	
	SI16				siGameMasterLevel;

	SI32				siBlockMode;			// 블럭의 상태. 
	SI32				siBlockYear, siBlockMonth, siBlockDay, siBlockHour, siBlockMinute;
	TCHAR				szBlockReason[30];

	bool				bAlreadyMakeCharSwitch;

	TCHAR				szServerName[20];
	SI32				siAccountStatus;

	UI08				m_uiRealAge;			// 주민등록상 나이(현제한국적용)

};


//-----------------------------------------------------------------------------
// 게임 가입 요청 
//-----------------------------------------------------------------------------
struct sRequest_JoinGame
{
	sPacketHeader		packetHeader;

	TCHAR				userid[ 20 ];
	TCHAR				cookie[ 20 ];
	TCHAR				jumin[ 8 ];
	TCHAR				userpw[ 20 ];


	sRequest_JoinGame()
	{
		memset( this, 0, sizeof( sRequest_JoinGame ) );
	}
};

//-----------------------------------------------------------------------------
// 게임 가입 요청 
//-----------------------------------------------------------------------------
struct sResponse_JoinGame
{
	sPacketHeader		packetHeader;

	SI32				siResult;
};

//-----------------------------------------------------------------------------
// 공지 사항 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sRequest_Notice
{
	sPacketHeader		packetHeader;
};


//-----------------------------------------------------------------------------
// 월드 리스트 요청 (클라이언트 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sRequest_WorldList
{
	sPacketHeader		packetHeader;

	TCHAR				szAccountID[20];
};



//-----------------------------------------------------------------------------
// 공지 사항 응답 (프론트서버 -> 클라이언트)
//-----------------------------------------------------------------------------
struct sResponse_Notice
{
	sPacketHeader		packetHeader;

	TCHAR				text[ 8000 ];
};


//-----------------------------------------------------------------------------
// 서버 리스트 응답 (프론트서버 -> 클라이언트)
//-----------------------------------------------------------------------------
struct sResponse_WorldList
{
	sPacketHeader			packetHeader;	

	UI16					usCount;
	WORLDINFO				worldinfo[ 1 ];

};

//-----------------------------------------------------------------------------
// 서버 갱신 정보 알림 (프론트서버 -> 클라이언트)
//-----------------------------------------------------------------------------
struct sNotify_WorldRefreshAll
{
	sPacketHeader			packetHeader;	

	UI16					usCount;
	GAMESERVER_REFRESHINFO	refreshinfo[ 1 ];
};


//-----------------------------------------------------------------------------
// 메시지 알림 (프론트서버 -> 클라이언트)
//-----------------------------------------------------------------------------
struct sNotify_WorldShortMessage
{
	sPacketHeader			packetHeader;

	UI16					worldId;
	TCHAR					shortmessage[ 64 ];
};


//-----------------------------------------------------------------------------
// 게임서버상태 보고 (게임서버 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sReportToFront_GameHeartbeat
{
	sPacketHeader			packetHeader;

	UI16					serverId;
	GAMESERVER_REFRESHINFO	refreshinfo;
};

//-----------------------------------------------------------------------------
// 게임서버정보 (게임서버 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sResponseToFront_GameServerInfo
{
	sPacketHeader				packetHeader;

	UI16						serverId;
	GAMESERVER_GAMESERVERINFO	gameserverinfo;

};

//***************************************************************************************************
//
// Class Name 		: sRequest_PingTest
//
// Last Modified 	: 2007 / 03 / 21
// Created		 	: 김광명
//
// Function			: 패치클라이언트랑 핑테스트를 한다.
//
//***************************************************************************************************
struct sRequest_PingTest
{
	sPacketHeader		packetHeader;

	UI08	m_uiIDCType;
	SI32	m_siFrontServerIndex;	
};

//***************************************************************************************************
//
// Class Name 		: sResponse_PingTest
//
//Last Modified 	: 2007 / 03 / 21
// Created		 	: 김광명
//
// Function			: 패치클라이언트랑 핑테스트를 한다.
//
//***************************************************************************************************
struct sResponse_PingTest
{
	sPacketHeader		packetHeader;

	UI08	m_uiIDCType;
	SI32	m_siFrontServerIndex;	
};

//***************************************************************************************************
//
// Class Name 		: sRequest_DiffHellManKey
//
// Last Modified 	: 2007 / 07 / 02
// Created		 	: 김광명
//
// Function			: 중국은 인증때 암호화를 한번 더 한다
//
//***************************************************************************************************
struct sRequest_DiffHellManKey
{
	sPacketHeader		packetHeader;

	TCHAR	tempKey[20];
};

//***************************************************************************************************
//
// Class Name 		: sResponse_DiffHellManKey
//
// Last Modified 	: 2007 / 07 / 02
// Created		 	: 김광명
//
// Function			: 중국은 인증때 암호화를 한번 더 한다
//
//***************************************************************************************************
struct sResponse_DiffHellManKey
{
	sPacketHeader		packetHeader;

	TCHAR	tempKey[20];
};


//***************************************************************************************************
//
// Class Name 		: sResponse_Promoter_Info
//
// Last Modified 	: 2009 / 07 / 27
// Created		 	: 황진성
//
// Function			: 중국은 서버리스트 요청시에 프로모터 정보도 클라이언트에 보낸다.
//
//***************************************************************************************************
struct sResponse_Promoter_Info
{
	sPacketHeader		packetHeader;

	TCHAR				m_szPromoterName[20];
	SI32				m_siServerID;
	int					m_ret;
};

#pragma pack()

#endif

