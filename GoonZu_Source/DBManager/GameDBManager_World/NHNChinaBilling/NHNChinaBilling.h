/*!
  * @file nhnchinabillinglib.h
  * @brief
  * @author kelovon <kelovon@nhncorp.com>
  * @date $Date: 2007-04-18$
  * @version $Revision: $
  * $Id: $
  */

#ifndef __NHNCHINABILLINGLIB_H_BFDB17F1_37A7_476A_A49E_A453FFF7B8C3__
#define __NHNCHINABILLINGLIB_H_BFDB17F1_37A7_476A_A49E_A453FFF7B8C3__

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

#define NHNCHINABILLING_MSG_SIZE			200
#define NHNCHINABILLING_LOGSEQ_SIZE			20+1

namespace NHNChinaBilling
{
	typedef NHNChinaUtil::Connection Connection;

	/*!
	  * @brief 각 함수의 결과 코드
	  */
	enum ReturnCode
	{
		SUCC,	// 성공
		FAIL,	// 시스템 에러
		R001,	// 공통 Invalid Parameter  파라미터가 널이거나 없는 경우  
		R002,	// 공통 Invalid Parameter (svrid)  등록된 서버가 아닌 경우  
		R003,	// 공통 Invalid Account (status)  계좌 상태가 Y 가 아닌 경우  
		R004,	// 공통 Invalid Coinlog (status)  해당 logseq 건이 이미 취소가 된 경우  
		R005,	// 공통 Invalid Coinlog (logseq)  해당 logseq 건이 없는 경우  
		C001,	// 충전 Invalid Parameter (pgid)  등록된 결제수단이 아닌 경우  
		C002,	// 충전 Invalid Parameter (logseq)  해당 logseq 가 충전로그 table 에 없는 경우  
		U001,	// 소진 Invalid Parameter (itemid)  등록된 상품이 아닌 경우  
		U002,	// 소진 Invalid Parameter (price)  금액 / 건수 가 등록된 가격과 다른 경우  
		U003,	// 소진 Invalid Account (balance)  잔액이 부족한 경우  
		U004,	// 소진 Invalid Account (userid)  해당 아이디의 계좌가 없는 경우
		U005,	// 소진 Duplication (authno) authno가 중복인 경우
		NONE
	};

	NHNCHINADLL_API inline const char* getStateString(ReturnCode s);
	/*!
	  * @brief 사용
	  * @param con Connection instance
	  * @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	  * @param svrid world id
	  * @param userid customer id
	  * @param charid character id
	  * @param itemid item id
	  * @param authno 결제 번호(ourgame FK)
	  * @param cnt 건수
	  * @param amt 출전 금액
	  * @param clientip 아이피
	  * @param memo 메모
	  * @param joincode 제휴사코드
	  * @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINABILLING_MSG_SIZE]을 가정함
	  * @param logseq 빌링 주문 번호를 저장할 NHNCHINABILLING_LOGSEQ_SIZE 버퍼, 성공일 때는 20 byte string, 그 외에는 NULL
	  * @return 처리 결과
	  */
	NHNCHINADLL_API ReturnCode UseCoin(const Connection &con,
							const char *gameid,
							int svrid,
							const char *userid,
							const char *charid,
							const char *itemid,
							__int64 authno,
							int cnt,
							__int64 amt,
							const char *clientip,
							const char *memo,
							const char *joincode,
							char *msg,
							char *logseq);

	/*!
	  * @brief 잔액 조회
	  * @param con Connection instance
	  * @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	  * @param svrid world id
	  * @param userid customer id
	  * @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINABILLING_MSG_SIZE]을 가정함
	  * @param balance 잔액
	  * @return 처리 결과
	  */
	NHNCHINADLL_API ReturnCode GetBalance(const Connection &con,
							const char *gameid,
							int svrid,
							const char *userid,
							char *msg,
							__int64 *balance1,
							__int64 *balance2
							);

	/*!
	* @brief Gift 조회
	* @param con Connection instance
	* @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	* @param svrid world id
	* @param userid customer id
	* @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINABILLING_MSG_SIZE]을 가정함
	* @param balance 잔액
	* @param balance1 잔액1
	* @param balance2 잔액2
	* @return 처리 결과
	*/
	NHNCHINADLL_API ReturnCode GiftBalance(const Connection &con,
		const char *gameid,
		int svrid,
		const char *userid,
		char *msg,
		__int64 *balance,
		__int64 *balance1,
		__int64 *balance2
		);

	/*!
	* @brief 사용
	* @param con Connection instance
	* @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	* @param svrid world id
	* @param userid customer id
	* @param charid character id
	* @param itemid item id
	* @param authno 결제 번호(ourgame FK)
	* @param cnt 건수
	* @param amt 출전 금액
	* @param clientip 아이피
	* @param memo 메모
	* @param joincode 제휴사코드
	* @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINABILLING_MSG_SIZE]을 가정함
	* @param logseq 빌링 주문 번호를 저장할 NHNCHINABILLING_LOGSEQ_SIZE 버퍼, 성공일 때는 20 byte string, 그 외에는 NULL
	* @return 처리 결과
	*/
	NHNCHINADLL_API ReturnCode GiftCoin(const Connection &con,
		const char *gameid,
		int svrid,
		const char *userid,
		const char *charid,
		const char *itemid,
		__int64 authno,
		int cnt,
		__int64 amt,
		__int64 giftamt,
		const char *clientip,
		const char *memo,
		const char *joincode,
		char *msg,
		char *logseq);

	/*!
	* @brief 충전
	* @param gameid		게임ID (C_R2,C_BD,C_GZ..etc)
	* @param userid		customer id
	* @param svrid		world id
	* @param activecode	
	* @param authno		
	* @param giftamt	
	* @param start_dt	
	* @param end_dt		
	* @param clientip	ip
	* @param chn_id
	* @param msg
	* @param giftno
	* @return 처리 결과
	*/
// 	NHNCHINADLL_API ReturnCode ChargeCoin(const Connection &con,
// 		const char *gameid,
// 		const char *userid,
// 		int svrid,
// 		int activecode,
// 		__int64 authno,
// 		__int64 giftamt,
// 		const char *start_dt,
// 		const char *end_dt,
// 		const char *clientip,
// 		const char *chn_id,
// 		char *msg,
// 		__int64 *giftno
// 		);
	/*!
	  * @brief 환전 등록
	  * @param exgkind	환전구분, 0: 골드, 1:실버
	  * @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	  * @param svrid	world id
	  * @param userid	customer id
	  * @param charid	character id
	  * @param authno	결제번호 (R2 FK)
	  * @param amt	골드금액
	  * @param silveramt	골드금액
	  * @param clientip	ip
	  * @param memo	메모
	  * @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINABILLING_MSG_SIZE]을 가정함
	  * @param logseq_out 빌링 주문 번호를 저장할 NHNCHINABILLING_LOGSEQ_SIZE 버퍼, 성공일 때는 20 byte string, 그 외에는 NULL
	  * @return 처리 결과
	  */
	NHNCHINADLL_API ReturnCode RegisterExchange(const Connection &con,
							int exgkind,
							const char *gameid,
							int svrid,
							const char *userid,
							const char *charid,
							__int64 authno,
							__int64 amt,
							__int64 silveramt,
							const char *clientip,
							const char *memo,
							char *msg,
							char *logseq_out);

	/*!
	  * @brief 환전 취소
	  * @param logseq	환전번호
	  * @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	  * @param svrid	world id
	  * @param userid	customer id
	  * @param authno	결제번호 (R2 FK)
	  * @param clientip	ip
	  * @param memo	메모
	  * @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINABILLING_MSG_SIZE]을 가정함
	  * @param logseq_out 빌링 주문 번호를 저장할 NHNCHINABILLING_LOGSEQ_SIZE 버퍼, 성공일 때는 20 byte string, 그 외에는 NULL
	  * @return 처리 결과
	  */
	NHNCHINADLL_API ReturnCode CancelExchange(const Connection &con,
							const char *logseq,
							const char *gameid,
							int svrid,	
							const char *userid,	
							__int64 authno,	
							const char *clientip,
							const char *memo,	
							char *msg,
							char *logseq_out);

	/*!
	  * @brief 환전 완료
	  * @param cmpKind  0: 판매자 완료, 1: 구매자 완료
	  * @param logseq	환전번호
	  * @param gameid 게임ID (C_R2,C_BD,C_GZ..etc)
	  * @param svrid	world id
	  * @param userid	customer id
	  * @param charid	character id
	  * @param authno	결제번호 (R2 FK)
	  * @param amt	골드금액
	  * @param silveramt	실버금액
	  * @param clientip	ip
	  * @param memo	메모
	  * @param msg 실패 메시지를 저장할 버퍼, char[NHNCHINABILLING_MSG_SIZE]을 가정함
	  * @param logseq_out 빌링 주문 번호를 저장할 NHNCHINABILLING_LOGSEQ_SIZE 버퍼, 성공일 때는 20 byte string, 그 외에는 NULL
	  * @return 처리 결과
	  */
	NHNCHINADLL_API ReturnCode CompleteExchange(const Connection &con,
							int cmpKind,
							const char *logseq,
							const char *gameid,
							int svrid,
							const char *userid,
							const char *charid,
							__int64 authno,
							__int64 amt,
							__int64 silveramt,
							const char *clientip,
							const char *memo,
							char *msg,
							char *logseq_out);

} // end of NHNChinaBilling

#endif /*__NHNCHINABILLINGLIB_H_BFDB17F1_37A7_476A_A49E_A453FFF7B8C3__*/
