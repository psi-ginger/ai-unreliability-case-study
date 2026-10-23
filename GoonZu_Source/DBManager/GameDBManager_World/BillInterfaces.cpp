#include "BillComponent/Bill.h"
#include "BillInterfaces.h"

/**--------------------------------------------------------------
-- ProcedureName   : GetUserBalance
-- Description     : 잔액조회 인터페이스
-- Return Value    : long
--                   0 = 성공, 그외 = 에러발생
-- Parameter       :
--    [IN]szBillServer : 빌링서버IP:Port (복수개 지정시 ; 로 분리)
--                       (예)111.111.111.111:50050;111.111.111.111:50050
--    [IN]szUserID     : 이용자ID
--    [OUT]lngRealCash : RealCash 잔액
--    [OUT]lngBonus    : Bonus 캐시 잔액
--    [OUT]lngMileage  : 마일리지 잔액
--
-- Copyright ⓒ 2006 by PayLetter Inc. All rights reserved.
---------------------------------------------------------------*/
long BillInterfaces::GetUserBalance
(
	char*	szBillServer,	// BillServerIP:Port (111.111.111.111:50050;111.111.111.111:50050)
	char*	szUserID,		// UserID

	long*	lngRealCash,	// RealCash 잔액
	long*	lngBonus,		// Bonus 캐시 잔액
	long*	lngMileage		// 마일리지 잔액
)	
{
	CBill		Bill;
	long		nTxResult = 0;

	// 초기화
	*lngRealCash	= 0;
	*lngBonus		= 0;
	*lngMileage		= 0;

	try
	{
		Bill.PutHOST(szBillServer);
		Bill.PutTxCmd(TXCMD_ACCOUNTINFO);

		Bill.SetField(BILL_SITECODE,	MY_SITECODE);
		Bill.SetField(BILL_GAMECODE,	MY_GAMECODE);
		Bill.SetField(BILL_USERID,		szUserID);

		nTxResult = Bill.StartAction();
	}
	catch(...)
	{
		return (-2);
	}

	// 잔액 조회 성공, RealCash, Bonus, Mileage를 구한다.
	if (nTxResult == BILL_SUCCESS)
	{
		char szTmp[32];

		Bill.GetVal(BILL_CASHREAL, szTmp, sizeof(szTmp));
		*lngRealCash = atol(szTmp);

		Bill.GetVal(BILL_CASHBONUS, szTmp, sizeof(szTmp));
		*lngBonus	= atol(szTmp);

		Bill.GetVal(BILL_MILEAGE, szTmp, sizeof(szTmp));
		*lngMileage	= atol(szTmp);
	}

	return(nTxResult);
}

/**--------------------------------------------------------------
-- ProcedureName   : ChargeItem
-- Description     : 아이템 구매(차감) 인터페이스
-- Return Value    : long
--                   0 = 차감성공, 그외 = 에러발생
-- Parameter       :
--    [IN]szBillServer : 빌링서버IP:Port (복수개 지정시 ; 로 분리)
--                       (예)111.111.111.111:50050;111.111.111.111:50050
--    [IN]szUserID     : 이용자ID
--    [IN]szGameItemKey: 구매 아이템 유일키
--    [IN]szGameItemName: 구매 아이템 명
--    [IN]szUserIP     : 이용자 IP주소 (xxx.xxx.xxx.xxx 형)
--    [IN]lngItemPrice : 구매아이템 가격(단가)
--    [IN]lngItemPiece : 구매할 개수(대부분 1일 것임.)
--    [IN]lngPresentFlag: 선물여부(1:일반구매, 2:선물)
--    [IN]lngCPID      : 아이템 구매 CP(1:군주,2:아틀란티카)
--    [OUT]lngRealCash : RealCash 잔액
--    [OUT]lngBonus    : Bonus 캐시 잔액
--    [OUT]lngMileage  : 마일리지 잔액
--    [OUT]szChargeNo  : 과금번호 (구매취소 할 경우 필요함.)
--
-- Copyright ⓒ 2006 by PayLetter Inc. All rights reserved.
---------------------------------------------------------------*/
long BillInterfaces::ChargeItem
(
	char*	szBillServer,	// BillServerIP:Port (111.111.111.111:50050;111.111.111.111:50050)
	char*	szUserID,		// UserID

	char*	szGameItemKey,	// 게임 아이템 키
	char*	szGameItemName,	// 게임 아이템 명
	char*	szUserIP,		// 이용자의 IP 어드레스
	long	lngItemPrice,	// 과금(차감) 금액
	long	lngItemPiece,	// 1로 고정
	long	lngItemCnt,		// 구매할 개수
	long	lngPresentFlag,	// 선물 여부(선물이면 1, 아니면 0)
	long	lngCPID,		// 아이템 구매 CP(1:군주,2:아틀란티카)

	long*	lngRealCash,	// RealCash 잔액
	long*	lngBonus,		// Bonus 캐시 잔액
	long*	lngMileage,		// 마일리지 잔액
	char*	szChargeNo		// 과금번호(19 Bytes) (취소할때 필요함.)
)	
{
	CBill		Bill;
	long		nTxResult = 0;

	// 초기화
	*lngRealCash	= 0;
	*lngBonus		= 0;
	*lngMileage		= 0;

	try
	{
		Bill.PutHOST(szBillServer);
		Bill.PutTxCmd(TXCMD_CHARGEGAMEITEM); // chargegameitem

		Bill.SetField(BILL_SITECODE,	MY_SITECODE);
		Bill.SetField(BILL_GAMECODE,	MY_GAMECODE);
		Bill.SetField(BILL_USERID,		szUserID);
		Bill.SetField(BILL_GAMEITEMKEY,	szGameItemKey);
		Bill.SetField(BILL_PRODNAME,	szGameItemName);
		Bill.SetField(BILL_IPADDR,		szUserIP);
	
		Bill.SetFieldLong(BILL_CPID,		lngCPID);
		Bill.SetFieldLong(BILL_CHARGEAMT,	lngItemPrice);
		Bill.SetFieldLong(BILL_ITEMPIECE,	lngItemPiece);
		Bill.SetFieldLong(BILL_ITEMCNT,		lngItemCnt);
		Bill.SetFieldLong(BILL_PRESENTFLAG,	lngPresentFlag);

		nTxResult = Bill.StartAction();
	}
	catch(...)
	{
		return (-2);
	}

	// 아이템 구매(차감) 성공
	// 차감후 RealCash, Bonus, Mileage 잔액 및 과금번호를 구한다.
	if (nTxResult == BILL_SUCCESS)
	{
		char szTmp[32];

		Bill.GetVal(BILL_CASHREAL, szTmp, sizeof(szTmp));
		*lngRealCash = atol(szTmp);

		Bill.GetVal(BILL_CASHBONUS, szTmp, sizeof(szTmp));
		*lngBonus	= atol(szTmp);

		Bill.GetVal(BILL_MILEAGE, szTmp, sizeof(szTmp));
		*lngMileage	= atol(szTmp);

		szChargeNo[0] = NULL;
		if (szChargeNo != NULL)
			Bill.GetVal(BILL_CHARGENO,	szChargeNo, 20);
	}

	return(nTxResult);
}

/**--------------------------------------------------------------
-- ProcedureName   : CancelPurchase
-- Description     : 구매취소 인터페이스
-- Return Value    : long
--                   0 = 성공, 그외 = 에러발생
-- Parameter       :
--    [IN]szBillServer : 빌링서버IP:Port (복수개 지정시 ; 로 분리)
--                       (예)111.111.111.111:50050;111.111.111.111:50050
--    [IN]szUserID     : 이용자ID
--    [IN]szChargeNo   : 취소할 과금번호 (구매시 저장해야 함.)
--    [OUT]lngRealCash : 구매취소 후 RealCash 잔액
--    [OUT]lngBonus    : 구매취소 후 Bonus 캐시 잔액
--    [OUT]lngMileage  : 구매취소 후 마일리지 잔액
--
-- Copyright ⓒ 2006 by PayLetter Inc. All rights reserved.
---------------------------------------------------------------*/
long BillInterfaces::CancelPurchase
(
	char* szBillServer,	// BillServerIP:Port (111.111.111.111:50050;111.111.111.111:50050)
	char* szUserID,		// UserID
	char* szChargeNo,		// 과금번호

	long*		lngRealCash,	// 구매취소 후 RealCash 잔액
	long*		lngBonus,		// 구매취소 후 Bonus 캐시 잔액
	long*		lngMileage		// 구매취소 후 마일리지 잔액
)	
{
	CBill		Bill;
	long		nTxResult = 0;

	// 초기화
	*lngRealCash	= 0;
	*lngBonus		= 0;
	*lngMileage		= 0;

	try
	{
		Bill.PutHOST(szBillServer);
		Bill.PutTxCmd(TXCMD_USECANCELDIRECT);

		Bill.SetField(BILL_SITECODE,	MY_SITECODE);
		Bill.SetField(BILL_USERID,		szUserID);
		Bill.SetField(BILL_CHARGENO,	szChargeNo);

		nTxResult = Bill.StartAction();
	}
	catch(...)
	{
		return (-2);
	}

	// 구매취소 성공
	// 구매취소 후 RealCash, Bonus, Mileage를 구한다.
	if (nTxResult == BILL_SUCCESS)
	{
		char szTmp[32];

		Bill.GetVal(BILL_CASHREAL, szTmp, sizeof(szTmp));
		*lngRealCash = atol(szTmp);

		Bill.GetVal(BILL_CASHBONUS, szTmp, sizeof(szTmp));
		*lngBonus	= atol(szTmp);

		Bill.GetVal(BILL_MILEAGE, szTmp, sizeof(szTmp));
		*lngMileage	= atol(szTmp);
	}

	return(nTxResult);
}

/**--------------------------------------------------------------
-- ProcedureName   : ChargeByMileage
-- Description     : 마일리지로 아이템 구매 인터페이스
-- Return Value    : long
--                   0 = 차감성공, 그외 = 에러발생
-- Parameter       :
--    [IN]szBillServer : 빌링서버IP:Port (복수개 지정시 ; 로 분리)
--                       (예)111.111.111.111:50050;111.111.111.111:50050
--    [IN]szUserID     : 이용자ID
--    [IN]szGameItemName: 구매 아이템 명
--    [IN]lngMileageAmt : 과금(차감) 금액(소요 마일리지)
--    [OUT]lngMileage  : 마일리지 잔액
--    [OUT]szChargeNo  : 과금번호 (구매취소 할 경우 필요함.)
--
-- Copyright ⓒ 2006 by PayLetter Inc. All rights reserved.
---------------------------------------------------------------*/
long BillInterfaces::ChargeByMileage
(
	char*	szBillServer,	// BillServerIP:Port (111.111.111.111:50050;111.111.111.111:50050)
	char*	szUserID,		// UserID
	char*	szGameItemName,	// 구매 게임 아이템 명
	long	lngMileageAmt,	// 과금(차감) 금액(소요 마일리지)

	long*	lngMileage,		// 구매후 마일리지 잔액
	char*	szChargeNo		// 과금번호(19 Bytes) (취소할때 필요함.)
)	
{
	CBill		Bill;
	long		nTxResult = 0;

	*lngMileage		= 0;

	try
	{
		Bill.PutHOST(szBillServer);
		Bill.PutTxCmd(TXCMD_CHARGEBYMILEAGE); // chargebymileage

		Bill.SetField(BILL_SITECODE,		MY_SITECODE);
		Bill.SetField(BILL_GAMECODE,		MY_GAMECODE);
		Bill.SetField(BILL_USERID,			szUserID);
		Bill.SetField(BILL_PRODNAME,		szGameItemName);
		Bill.SetFieldLong(BILL_CHARGEAMT,	lngMileageAmt);

		nTxResult = Bill.StartAction();
	}
	catch(...)
	{
		return (-2);
	}

	// 마일리지로 아이템 구매(차감) 성공
	// 차감후 Mileage 잔액 및 과금번호를 구한다.
	if (nTxResult == BILL_SUCCESS)
	{
		char szTmp[32];

		Bill.GetVal(BILL_MILEAGE, szTmp, sizeof(szTmp));
		*lngMileage	= atol(szTmp);

		szChargeNo[0] = NULL;
		if (szChargeNo != NULL)
			Bill.GetVal(BILL_CHARGENO,	szChargeNo, 20);
	}

	return(nTxResult);
}

/**--------------------------------------------------------------
-- ProcedureName   : PayBonus
-- Description     : 보너스 캐시 지급 인터페이스
-- Return Value    : long
--                   0 = 지급성공, 그외 = 에러발생
-- Parameter       :
--    [IN]szBillServer : 빌링서버IP:Port (복수개 지정시 ; 로 분리)
--                       (예)111.111.111.111:50050;111.111.111.111:50050
--    [IN]szUserID     : 이용자ID
--    [IN]szUserName   : 이용자명(캐릭터명이 아님)
--    [IN]szReason     : 지급사유(반드시 영어로, 50 Bytes 이내)
--    [IN]lngBonusAmt  : 지급 보너스캐시 금액
--    [OUT]lngRealCash : 보너스 캐시 지급 후 RealCash 잔액
--    [OUT]lngBonus    : 보너스 캐시 지급 후 Bonus 캐시 잔액
--    [OUT]lngMileage  : 보너스 캐시 지급 후 마일리지 잔액
--
-- Copyright ⓒ 2006 by PayLetter Inc. All rights reserved.
---------------------------------------------------------------*/
long BillInterfaces::PayBonus
(
	char*	szBillServer,	// BillServerIP:Port (111.111.111.111:50050;111.111.111.111:50050)
	char*	szUserID,		// UserID
	char*	szUserName,		// 이용자명(캐릭터명이 아님)
	char*	szReason,		// 지급사유(반드시 영어로, 50 Bytes 이내)
	long	lngBonusAmt,	// 지급 보너스캐시 금액

	long*	lngRealCash,	// 보너스 캐시 지급 후 RealCash 잔액
	long*	lngBonus,		// 보너스 캐시 지급 후 Bonus 캐시 잔액
	long*	lngMileage		// 보너스 캐시 지급 후 마일리지 잔액
)	
{
	CBill		Bill;
	long		nTxResult = 0;

	// 초기화
	*lngRealCash	= 0;
	*lngBonus		= 0;
	*lngMileage		= 0;

	try
	{
		Bill.PutHOST(szBillServer);
		Bill.PutTxCmd(TXCMD_PAYCASH); // paycash

		Bill.SetField(BILL_SITECODE,	MY_SITECODE);
		Bill.SetField(BILL_USERID,		szUserID);
		Bill.SetField(BILL_USERNAME,	szUserName);
		Bill.SetField(BILL_PAYTOOLNAME,	szReason);
		Bill.SetField(BILL_PGCODE,		BILL_PGCODE_BONUSCASH);

		Bill.SetFieldLong(BILL_CASHAMT,	lngBonusAmt);
		Bill.SetFieldLong(BILL_PAYTOOL,	BILL_PAYTOOL_BONUSCASH);

		nTxResult = Bill.StartAction();
	}
	catch(...)
	{
		return (-2);
	}

	// 보너스 캐시 지급 성공
	// 차감후 Mileage 잔액 및 과금번호를 구한다.
	if (nTxResult == BILL_SUCCESS)
	{
		char szTmp[32];

		Bill.GetVal(BILL_CASHREAL, szTmp, sizeof(szTmp));
		*lngRealCash = atol(szTmp);

		Bill.GetVal(BILL_CASHBONUS, szTmp, sizeof(szTmp));
		*lngBonus	= atol(szTmp);

		Bill.GetVal(BILL_MILEAGE, szTmp, sizeof(szTmp));
		*lngMileage	= atol(szTmp);
	}

	return(nTxResult);
}
