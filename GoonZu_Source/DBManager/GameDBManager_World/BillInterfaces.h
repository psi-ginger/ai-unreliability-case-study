#ifndef __BILL_INTERFACES_H__
#define __BILL_INTERFACES_H__


// TX 명령어
#define	TXCMD_ACCOUNTINFO		"accountinfo"		// 잔액조회 명령어
#define	TXCMD_CHARGEGAMEITEM	"chargegameitem"	// 과금(차감) 명령어
#define TXCMD_USECANCELDIRECT	"usecanceldirect"	// 과금(구매)취소
#define	TXCMD_CHARGEBYMILEAGE	"chargebymileage"	// 마일리지로 구매하기
#define	TXCMD_PAYCASH			"paycash"			// 보너스캐시 발행

#define	MY_SITECODE				"ndoor"		// 5 bytes
#define	MY_GAMECODE				"goonzu"	// 20 bytes

#define BILL_SUCCESS			0
#define	BILL_PAYTOOL_BONUSCASH	4

#define	BILL_PGCODE_BONUSCASH	"bonuscash"

#define BILL_PGCODE				"pgcode"
#define BILL_USERID				"userid"
#define BILL_USERNAME			"username"
#define BILL_SITECODE			"sitecode"
#define BILL_CASHREAL			"cashreal"
#define BILL_CASHBONUS			"cashbonus"
#define BILL_MILEAGE			"mileage"
#define BILL_ITEMID				"itemid"
#define BILL_GAMEITEMKEY		"gameitemkey"
#define BILL_PRESENTFLAG		"presentflag"
#define BILL_PRODNAME			"prodname"
#define BILL_CHARGEAMT			"chargeamt"
#define BILL_ITEMPIECE			"itempiece"
#define BILL_ITEMCNT			"itemcnt"
#define BILL_GAMECODE			"gamecode"
#define BILL_IPADDR				"ipaddr"
#define BILL_CHARGENO			"chargeno"
#define BILL_PAYTOOL			"paytool"
#define BILL_PAYTOOLNAME		"paytoolname"
#define BILL_CASHAMT			"cashamt"
#define BILL_CPID				"cpid"

class BillInterfaces
{
public:
	// 잔액조회 인터페이스
	static long GetUserBalance
		(
			char*	szBillServer,	// BillServerIP:Port (111.111.111.111:50090;111.111.111.111:50090)
			char*	szUserID,		// UserID

			long*	lngRealCash,	// RealCash 잔액
			long*	lngBonus,		// Bonus 캐시 잔액
			long*	lngMileage		// 마일리지 잔액
		);
	// 아이템 구매(차감) 인터페이스
	static	long BillInterfaces::ChargeItem
		(
			char*	szBillServer,	// BillServerIP:Port (111.111.111.111:50090;111.111.111.111:50090)
			char*	szUserID,		// UserID

			char*	szGameItemKey,	// 게임 아이템 키
			char*	szGameItemName,	// 게임 아이템 명
			char*	szUserIP,		// 이용자의 IP 어드레스
			long	lngItemPrice,	// 과금(차감) 금액
			long	lngItemPiece,	// 1로 고정
			long	lngItemCnt,		// 구매할 개수
			long	lngPresentFlag,	// 선물 여부(선물이면 1, 아니면 0)
			long	IngCPID,		// 아이템 구매 CP(1:군주,2:아틀란티카)

			long*	lngRealCash,	// RealCash 잔액
			long*	lngBonus,		// Bonus 캐시 잔액
			long*	lngMileage,		// 마일리지 잔액
			char*	szChargeNo		// 과금번호(19 Bytes) (취소할때 필요함.)
		);
	// 구매 취소 인터페이스
	static	long BillInterfaces::CancelPurchase
		(
			char*	szBillServer,	// BillServerIP:Port (111.111.111.111:50090;111.111.111.111:50090)
			char*	szUserID,		// UserID
			char*	szChargeNo,		// 과금번호

			long*	lngRealCash,			// 구매취소 후 RealCash 잔액
			long*	lngBonus,				// 구매취소 후 Bonus 캐시 잔액
			long*	lngMileage			// 구매취소 후 마일리지 잔액
		);
	// 마일리지로 아이템 구매 인터페이스
	static	long BillInterfaces::ChargeByMileage
		(
			char*	szBillServer,	// BillServerIP:Port (111.111.111.111:50090;111.111.111.111:50090)
			char*	szUserID,		// UserID

			char*	szGameItemName,	// 구매 게임 아이템 명
			long	lngMileageAmt,	// 과금(차감) 금액(소요 마일리지)

			long*	lngMileage,		// 구매후 마일리지 잔액
			char*	szChargeNo		// 과금번호(19 Bytes) (취소할때 필요함.)
		);
	// 보너스 캐시 지급 인터페이스
	static	long BillInterfaces::PayBonus
		(
			char*	szBillServer,	// BillServerIP:Port (111.111.111.111:50090;111.111.111.111:50090)
			char*	szUserID,		// UserID
			char*	szUserName,		// 이용자명(캐릭터명이 아님)
			char*	szReason,		// 지급사유(50 Bytes 이내)
			long	lngBonusAmt,	// 지급 보너스캐시 금액

			long*	lngRealCash,	// 보너스 캐시 지급 후 RealCash 잔액
			long*	lngBonus,		// 보너스 캐시 지급 후 Bonus 캐시 잔액
			long*	lngMileage		// 보너스 캐시 지급 후 마일리지 잔액
		);
};

#endif
