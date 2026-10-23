#ifndef _AUTHMSG_H
#define _AUTHMSG_H

#include "itzbase.h"

enum {
		AUTHMSG_REQUEST_FRONTAUTH = 1,
		AUTHMSG_REQUEST_GAMEAUTH,
		AUTHMSG_REQUEST_MASTERAUTH,
		AUTHMSG_REQUEST_FRONTJOINGAME,
		AUTHMSG_REQUEST_GAMELOGOUT,
		AUTHMSG_REQUEST_GAMELOGIN,

		AUTHMSG_REQUEST_HANAUTH_FRONTAUTH,
		AUTHMSG_REQUEST_HANAUTH_GAMEAUTH,

		//---------------------------------------------------------
		//KHY - 0204 -  중국 PurpleAuth 인증 모듈
		AUTHMSG_REQUEST_PORTEAUTH_FRONTAUTH,
		AUTHMSG_REQUEST_PORTEAUTH_GAMEAUTH,

		AUTHMSG_REQUEST_TAIWAN_FRONTAUTH,
		AUTHMSG_REQUEST_TAIWAN_GAMEAUTH,

		AUTHMSG_REQUEST_HANAUTH_GAMERUNAUTH,

		AUTHMSG_REQUEST_GMTOOLAUTH,

		//---------------------------------------------------------
		//KHY - 0617 - 일본 채널링 추가.
		AUTHMSG_REQUEST_HANGAMEJP_FRONTAUTH,
		AUTHMSG_REQUEST_GAMANIAJP_FRONTAUTH,
		AUTHMSG_REQUEST_GAMEPOTJP_FRONTAUTH,
		AUTHMSG_REQUEST_NDEJP_FRONTAUTH,
		AUTHMSG_REQUEST_NEXONJP_FRONTAUTH,
		//---------------------------------------------------------
		
		// PCK : IOVATION 인증 추가 (2009.02.03)
		AUTHMSG_REQUEST_ENGLISH_FRONTAUTH,

		// [진성] USA 새로운 인증.
		AUTHMSG_REQUEST_USA_FRONTAUTH,

		// [영훈] Europe 인증
		AUTHMSG_REQUEST_EUROPE_FRONTAUTH,

		// [종호_NHN->NDOORS] Korea 인증
		AUTHMSG_REQUEST_KOREA_FRONTAUTH,


		//---------------------------------------------------------
		AUTHMSG_RESPONSE_FRONTAUTH = 1000,
		AUTHMSG_RESPONSE_GAMEAUTH,
		AUTHMSG_RESPONSE_MASTERAUTH,
		AUTHMSG_RESPONSE_FRONTJOINGAME,
		AUTHMSG_RESPONSE_GAMELOGOUT,
		AUTHMSG_RESPONSE_GAMELOGIN,

		AUTHMSG_RESPONSE_HANAUTH_FRONTAUTH,
		AUTHMSG_RESPONSE_HANAUTH_GAMEAUTH,

		AUTHMSG_RESPONSE_HANAUTH_GAMERUNAUTH,

		AUTHMSG_RESPONSE_GMTOOLAUTH,

		AUTHMSG_REPORT_GAMEHEARTBEAT = 10000
};

#pragma pack(1)

//-----------------------------------------------------------------------------
// 인증 요청 (프론트서버 -> 인증서버)
//-----------------------------------------------------------------------------
struct sRequest_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;
	
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};

//---------------------------------------------------------
//KHY - 0617 - 일본 채널링 추가.
//-----------------------------------------------------------------------------
// 인증 요청 (프론트서버 -> 인증서버) : 한게임, 감마니아, 게임팟, NDE , NEXON
//-----------------------------------------------------------------------------
struct sRequest_HANGAMEJP_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;
	
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};
struct sRequest_GAMANIAJP_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;
	
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};
struct sRequest_GAMEPOTJP_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;
	
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};
struct sRequest_NDEJP_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;
	
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};
struct sRequest_NEXONJP_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;
	
	TCHAR				userid[ 512 ]; // 넥슨은 ID에 정보를 담아서 넘긴다.
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};

//---------------------------------------------------------

//-----------------------------------------------------------------------------
// 인증 요청 (프론트서버 -> 인증서버) : TAIWAN
//-----------------------------------------------------------------------------
struct sRequest_Taiwan_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;

	TCHAR				gameaccount[ 30 ];
	TCHAR				onetimepassword[ 40 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// 인증 요청 (프론트서버 -> 인증서버) : NHN : HANAUTH
//-----------------------------------------------------------------------------
struct sRequest_HanAuth_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;

	TCHAR				authstring[ 2048 ];
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};


//-----------------------------------------------------------------------------
//KHY - 0204 -  중국 PurpleAuth 인증 모듈
// 인증 요청 (프론트서버 -> 인증서버) : NHN : PorteAuth
//-----------------------------------------------------------------------------
struct sRequest_PorteAuth_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;

	TCHAR				authstring[ 2048 ];
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// IOVATION 인증 요청 (프론트서버 -> 인증서버)
//-----------------------------------------------------------------------------
struct sRequest_English_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];

	TCHAR				Blackbox[ 2048 ];
};


//-----------------------------------------------------------------------------
// [진성] USA 새로운 인증.
//-----------------------------------------------------------------------------
struct sRequest_USA_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;

	TCHAR				authstring[ 4096 ];

	TCHAR				Blackbox[ 2048 ];
};

//-----------------------------------------------------------------------------
// [영훈] Europe 인증.
//-----------------------------------------------------------------------------
struct sRequest_EUROPE_FrontAuth
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	TCHAR				userIP[ 20 ];
	TCHAR				macaddress[ 20 ];
};

//-----------------------------------------------------------------------------
// 인증 요청 (게임서버 -> 인증서버)
//-----------------------------------------------------------------------------
struct sRequest_GameAuth
{
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siAuthKey;			
	
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
	// 유저가 선택한 서버의 정보 IDC구분(1,2), 지역구분, 서버구분이 문자로 된다.
	// 20102 ctc 제 1구역의 2번째 서버
	TCHAR				userServerID[ 20 ];	
	TCHAR				userIP[ 20 ];
};

//-----------------------------------------------------------------------------
// 인증 요청 (게임서버 -> 인증서버) : NHN HANAUTH
//-----------------------------------------------------------------------------
struct sRequest_HanAuth_GameAuth
{
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siAuthKey;			

	TCHAR				authstring[ 2048 ];
	TCHAR				userid[ 20 ];

	UI16				usServerAge;
};

//-----------------------------------------------------------------------------
// 인증 요청 (게임서버 -> 인증서버) : 대만
//-----------------------------------------------------------------------------
struct sRequest_Taiwan_GameAuth
{
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siAuthKey;			

	TCHAR				gameaccount[ 30 ];
	TCHAR				onetinepassword[ 40 ];
};

//-----------------------------------------------------------------------------
// 인증 요청 (마스터서버 -> 인증서버)
//-----------------------------------------------------------------------------
struct sRequest_MasterAuth
{
	sPacketHeader		packetHeader;			
	
	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
};


//-----------------------------------------------------------------------------
// 인증 응답 (인증서버 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sResponse_FrontAuth
{
	sPacketHeader	packetHeader;

	UI16			usResult;				// 0:성공   1:실패

	// 넥슨 채널링을 위한 ID추가.
	char			userid[ 20 ];

	UI16			usPacketUnique;
	UI16			usFrontCharID;

	SI32			siAuthKey;

	SI16			siGameMasterLevel;
	
	SI32			siBlockMode;				// 0:일반, 1:블럭, 2:영구블럭
	SI32			siBlockYear;
	SI32			siBlockMonth;
	SI32			siBlockDay;
	SI32			siBlockHour;
	SI32			siBlockMin;
	TCHAR			szBlockReason[ 30 ];

	bool			bAlreadyMakeCharSwitch;

	TCHAR			szServerName[ 20 ];

	SI32			siAccountStatus;

	UI08			m_uiRealAge;			// (한국) 주민등록상의 나이

};



//-----------------------------------------------------------------------------
// 인증 응답 (인증서버 -> 게임서버)
//-----------------------------------------------------------------------------
struct sResponse_GameAuth
{
	sPacketHeader		packetHeader;

	UI32				uiIDNum;	
	SI16				siSex;						// 1 남자 , 0 여자 				
	UI16				usCharID;				 
	SI16				siGameMasterLevel;

	SI32				siBlockMode;				// 0:일반, 1:블럭, 2:영구블럭
	SI32				siBlockYear;
	SI32				siBlockMonth;
	SI32				siBlockDay;
	SI32				siBlockHour;
	SI32				siBlockMin;
	TCHAR				szBlockReason[ 30 ];

	SI32				siAccountStatus;
	SI32				siPlayingTime;

	UI32				uiIDNo;						// 한게임용 IDNO - by LEEKH 2007.11.11
	SI16				siDormancyState;			// 휴면 계정인지 아닌지
};

//-----------------------------------------------------------------------------
// 로그 아웃 응답 (인증서버 -> 게임서버)
//-----------------------------------------------------------------------------
struct sRequest_GameLogout
{
	sPacketHeader		packetHeader;

	UI32				uiIDNum;	
	TCHAR				szAccountID[ 20 ];
	TCHAR				userServerID[ 20 ];	
	TCHAR				szPlayingCharNmae[ 20 ];
	SI32				siLevel;
	SI32 				siPlayTimeMinute;
	
};


//-----------------------------------------------------------------------------
// 인증 응답 (인증서버 -> 마스터서버)
//-----------------------------------------------------------------------------
struct sResponse_MasterAuth
{
	sPacketHeader		packetHeader;

	UI16				usResult;
};


//-----------------------------------------------------------------------------
// 허트 비트 (게임서버 -> 인증서버 )
//-----------------------------------------------------------------------------
struct sReportToAuth_GameHeartbeat
{
	sPacketHeader		packetHeader;
	
	UI16				serverId;
	UI16				worldId;
};


//-----------------------------------------------------------------------------
// 게임 가입 요청 (프론트서버 -> 인증서버)
//-----------------------------------------------------------------------------
struct sRequest_FrontJoinGame
{
	sPacketHeader		packetHeader;

	UI16				usPacketUnique;
	UI16				usFrontCharID;

	TCHAR				userid[ 20 ];
	TCHAR				cookie[ 20 ];
	TCHAR				szIP[ 20 ];
	TCHAR				jumincode[ 8 ];
	TCHAR				userpw[ 20 ];
};

//-----------------------------------------------------------------------------
// 게임 가입 응답 (인증서버 -> 프론트서버)
//-----------------------------------------------------------------------------
struct sResponse_FrontJoinGame
{
	sPacketHeader		packetHeader;

	SI32				siResult;

	UI16				usPacketUnique;
	UI16				usFrontCharID;

};

//-----------------------------------------------------------------------------------------------
// 인증 요청(게임실행도중에 나이제한체크) (게임서버 -> 인증서버) : NHN HANAUTH (PCK : 2007.09.19)
//-----------------------------------------------------------------------------------------------
struct sRequest_HanAuth_GameRunAuth
{
	sPacketHeader		packetHeader;

	UI16				usCharID;			

	TCHAR				authstring[ 2048 ];
	TCHAR				userid[ 20 ];

	UI16				usServerAge;
};

//---------------------------------------------------------------------------------
// 인증 응답(게임실행도중에 나이제한체크) (인증서버 -> 게임서버) : PCK - 2007.09.19
//---------------------------------------------------------------------------------
struct sResponse_GameRunAuth
{
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siResult;	// 0 : 인증실패, 1 : 인증성공
};

struct sRequest_GMToolAuth
{
	sPacketHeader		packetHeader;

	UI16				usCharID;

	TCHAR				userid[ 20 ];
	TCHAR				userpw[ 20 ];
};


struct sResponse_GMToolAuth
{
	sPacketHeader		packetHeader;

	UI16				usCharID;
	TCHAR				userid[ 20 ];

	SI32				siMasterLevel;
};


#pragma pack()

#endif

