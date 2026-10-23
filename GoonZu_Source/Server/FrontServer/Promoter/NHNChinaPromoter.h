#ifndef __NHNCHINAPROMOTERLIB_H_BFDB17F1_37A7_476A_A49E_A453FFF7B8C3__
#define __NHNCHINAPROMOTERLIB_H_BFDB17F1_37A7_476A_A49E_A453FFF7B8C3__

#ifdef _NHNCHINA_DLL

#ifdef NHNCHINADLL_EXPORTS
#define NHNCHINADLL_API __declspec(dllexport)
#else
#define NHNCHINADLL_API __declspec(dllimport)
#endif

#else // _NHNCHINA_DLL

#define NHNCHINADLL_API

#endif // _NHNCHINA_DLL

#include "nhnchinautil.h"

#define NHNCHINAPROMOTER_MSG_SIZE			200
#define NHNCHINAPROMOTER_LOGSEQ_SIZE		20+1

#define	NHNCHINAPROMOTER_PROMOTER_USERID_SIZE		64
#define	NHNCHINAPROMOTER_PROMOTER_CHARID_SIZE		180
#define	NHNCHINAPROMOTER_NEWBIE_USERID_SIZE			64
#define	NHNCHINAPROMOTER_NEWBIE_CHARID_SIZE			180

#define	NHNCHINAPROMOTER_SVRID_SIZE			80
#define NHNCHINAPROMOTER_CODE_SIZE			200
#define NHNCHINAPROMOTER_AUTH_SIZE          10
#define NHNCHINAPROMOTER_URL_SIZE			1000

namespace NHNChinaPromoter
{
	typedef NHNChinaUtil::Connection Connection;

	/*!
	  * @brief 각 함수의 결과 코드
	  */
	enum ReturnCode
	{
		SUCC,	// 성공
		FAIL,	// 시스템 에러
		P000,	// undefined error
		P001,	// promoter found with no svrid and charid
		P002,	// no promoter found
		P003,	//
		P004,	//
		P005,	//
		P006,	//
		P007,	//
		P008,	//
		P009,	//
		NONE
	};


	/*!
	  * @brief 사용
	  * @param con Connection instance
	  * @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	  * @param newbie customer id
	  * @param newbie customer charid
	  * @param svrid world id
	  * @param promoter customer id
	  * @param promoter customer charid
	  * @param auth 
	  * @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINAPROMOTER_MSG_SIZE]을 가정함
	  * @return 처리 결과
	  */
	NHNCHINADLL_API ReturnCode GetPromoterInfoForNewbie(const Connection &con,
							const char *gameid,
							const char *newbieUserid,
							const char *newbieCharid,
							char *svrid,
							char *promoterUserid,
							char *promoterCharid,
							char *auth,
							char *msg);


	/*!
	  * @brief 사용
	  * @param con Connection instance
	  * @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	  * @param newbie customer id
	  * @param newbie customer charid
	  * @param svrid world id
	  * @param promoter customer id
	  * @param promoter customer charid
	  * @param charid character id
	  * @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINAPROMOTER_MSG_SIZE]을 가정함
	  * @return 처리 결과
	  */
	NHNCHINADLL_API ReturnCode SetFirstCharidInfoForNewbie(const Connection &con,
							const char *gameid,
						    const char *svrid,	
							const char *newbieUserid,
							const char *newbieCharid,
							const char *promoterUserid,
							const char *promoterCharid,
							char *msg);

	/*!
	  * @brief 사용
	  * @param con Connection instance
	  * @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	  * @param promoter customer id
	  * @param promoter customer charid
	  * @param code promoter code
      * @param code promoter url
	  * @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINAPROMOTER_MSG_SIZE]을 가정함
	  * @return 처리 결과
	  */
	NHNCHINADLL_API ReturnCode GetPromoterCode(const Connection &con,
							const char *gameid,
							const char *promoterUserid,
							const char *promoterCharid,
							char *code,
							char *url,
							char *msg);



} // end of NHNChinaPromoter

#endif 
