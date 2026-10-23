#include "BillManager_nDoors_Korea.h"

// [종호_NHN->NDOORS] Billing모듈적용
#include <comutil.h>
#include <objbase.h>
#import "bxpgincli.tlb" named_guids
#import "bxPGCLI.tlb" named_guids

using namespace BXPGINCLILib;
using namespace BXPGCLILib;

extern IPGTXCli *g_pIPGTXCli;
extern IIPGCli	*g_plPGCli;


CBillManager_nDoors_Korea::CBillManager_nDoors_Korea()
{

}

CBillManager_nDoors_Korea::~CBillManager_nDoors_Korea()
{
	
}

SI32 CBillManager_nDoors_Korea::GetUserCash(char*accountid,SI32*real,SI32*bonus,SI32*etc)
{
	// 잔액 조회
	CoInitialize(NULL);

	IIPGCli *pIPGCli = NULL;

	HRESULT hr = CoCreateInstance( 
		CLSID_IPGCli, 
		NULL, 
		CLSCTX_LOCAL_SERVER,
		IID_IIPGCli,
		(void**)&pIPGCli );

	if( FAILED( hr ) )
	{
		CoUninitialize();
		return -1;
	}

	_variant_t ret;
	_variant_t user_no,company_cd,user_id;
	_variant_t RemainAmt,cashshop,RealCash,BonusCash,EtcCash;
	_variant_t MileageShop,MileageContents,ret_val,ret_message;



	user_no.SetString("");
	company_cd.SetString("BX00");
	user_id.SetString(accountid);
	RemainAmt.SetString("");
	cashshop.SetString("");
	RealCash.SetString("");
	BonusCash.SetString("");
	EtcCash.SetString("");
	MileageShop.SetString("");
	MileageContents.SetString("");
	ret_val.SetString("");
	ret_message.SetString("");

	try
	{
		ret = pIPGCli->GetUserCash( &user_no,&company_cd,&user_id,
			&RemainAmt,&cashshop,&RealCash,&BonusCash,&EtcCash,
			&MileageShop,&MileageContents,&ret_val,&ret_message);
	}
	catch (_com_error&e)
	{
		if ( pIPGCli )
			pIPGCli->Release();

		//g_LogPrintDB.FilePrint("IPGERROR.txt", "GetUserCash : %s", e.ErrorMessage());
		CoUninitialize();
		return -3;
	}

	if ( pIPGCli )
		pIPGCli->Release();

	char* result = NULL ;
	if ( ret_message.bstrVal != NULL )
		result = _com_util::ConvertBSTRToString(ret_message.bstrVal);
	else
	{
		CoUninitialize();
		return -4 ;
	}

	if ( result == NULL )
	{
		CoUninitialize();
		return -5 ;
	}

	// 조회 성공
	if ( result != NULL && !strcmp(result,"Success") )
	{
		*real = RealCash.intVal ;
		*bonus = BonusCash.intVal ;
		*etc = EtcCash.intVal ;

		CoUninitialize();
		return 0 ;
	}

	CoUninitialize();
	return -2 ;
}

SI32 CBillManager_nDoors_Korea::Get_Approve(char* accountid,SI32 *price,char*code, bool bGift)
{
	// 승인 요청
	IIPGCli *pIPGCli = NULL;

	CoInitialize(NULL);

	HRESULT hr = CoCreateInstance( 
		CLSID_IPGCli, 
		NULL, 
		CLSCTX_LOCAL_SERVER,
		IID_IIPGCli,
		(void**)&pIPGCli );

	if( FAILED( hr ) )
	{
		CoUninitialize();
		return -1;
	}

	_variant_t ret;
	_variant_t menu_id, company_cd, user_id, user_no, agTxNo;
	_variant_t amout, tx_no, user_url, ret_val, ret_message;//,KeyValue;

	menu_id.SetString("000000003400000000010000000001");
	company_cd.SetString("BX00");
	user_id.SetString(accountid);
	user_no.SetString("");
	agTxNo.SetString("ABCEDFGHIJK");
	amout = (long)(*price);
	tx_no.SetString("");
	ret_val.SetString("");
	ret_message.SetString("");

	try
	{
		ret = pIPGCli->Get_Approve( &menu_id, &company_cd, &user_id, &user_no, &agTxNo,
			&amout, &tx_no, &ret_val, &ret_message);
	}
	catch (_com_error&e)
	{
		if ( pIPGCli )
			pIPGCli->Release();

		CoUninitialize();
		return -4;
	}

	if ( pIPGCli )
		pIPGCli->Release();

	char* result = NULL ;
	if ( ret_message.bstrVal != NULL )
		result = _com_util::ConvertBSTRToString(ret_message.bstrVal);
	else
	{
		CoUninitialize();
		return -5 ;
	}

	if ( result == NULL )
	{
		CoUninitialize();
		return -6 ;
	}

	// 승인 성공
	if ( !strcmp(result,"Success") )
	{
		if ( tx_no.bstrVal != NULL )
		{
			char szCode[50] = "" ;
			sprintf(szCode,"%s",_com_util::ConvertBSTRToString(tx_no.bstrVal));
			strcpy(code,szCode);
		}
		else
		{
			CoUninitialize();
			return -2 ;
		}

		CoUninitialize();
		return 0 ;
	}
	else
	{
		if ( amout.intVal > 0 )
			*price = amout.intVal ;
	}

	CoUninitialize();
	return -3 ;
}

SI32 CBillManager_nDoors_Korea::ConfirmTX(char* accountid,SI32 price,char*code, bool bGift)
{
	// 결제 요청
	CoInitialize(NULL);

	IIPGCli *pIPGCli = NULL;

	HRESULT hr = CoCreateInstance( 
		CLSID_IPGCli, 
		NULL, 
		CLSCTX_LOCAL_SERVER,
		IID_IIPGCli,
		(void**)&pIPGCli );

	if( FAILED( hr ) )
	{
		CoUninitialize();
		return -1;
	}

	_variant_t ret;
	_variant_t menu_id, company_cd, user_id, user_no,amount;
	_variant_t tx_no, user_url, ret_val, ret_message;

	
	menu_id.SetString("000000003400000000010000000001");
	company_cd.SetString("BX00");
	user_id.SetString(accountid);
	user_no.SetString("");
	amount = (long)price;
	tx_no.SetString(code);
	user_url.SetString("CITEM");
	ret_val.SetString("");
	ret_message.SetString("");

	try
	{
		ret = pIPGCli->ConfirmTX( &menu_id, &company_cd, &user_id, &user_no, &amount,
			&tx_no, &user_url, &ret_val, &ret_message);
	}
	catch (_com_error&e)
	{
		if ( pIPGCli )
			pIPGCli->Release();

		//g_LogPrintDB.FilePrint("IPGERROR.txt", "ConfirmTX : %s", e.ErrorMessage());
		CoUninitialize();
		return -3;
	}

	if ( pIPGCli )
		pIPGCli->Release();
	else
	{
		CoUninitialize();
		return -4 ;
	}

	char* result = NULL ;
	if ( ret_message.bstrVal != NULL )
		result = _com_util::ConvertBSTRToString(ret_message.bstrVal);
	else
	{
		CoUninitialize();
		return -5 ;
	}

	if ( result == NULL )
	{
		CoUninitialize();
		return -6 ;
	}

	// 출금 성공
	if ( result != NULL && !strcmp(result,"Success") )
	{
		CoUninitialize();
		return 0 ;
	}

	CoUninitialize();
	return -2 ;
}

SI32 CBillManager_nDoors_Korea::GetUsePkgCash(char*accountid,SI32*eventmoney) 
{
	// 이벤트 잔액 조회
	CoInitialize(NULL);

	IPGTXCli *pIPGTXCli = NULL;

	HRESULT hr = CoCreateInstance( 
		CLSID_PGTXCli, 
		NULL, 
		CLSCTX_LOCAL_SERVER,
		IID_IPGTXCli,
		(void**)&pIPGTXCli );

	if( FAILED( hr ) )
	{
		CoUninitialize();
		return -1;
	}

	_variant_t ret;
	_variant_t user_no,company_cd,user_id,pkg_no,pkgRemainAmt;
	_variant_t ret_val,ret_message;

	/*	// SBS라면
	if( usertype == 1 )
	{
	menu_id.SetString("000000131700000000010000000001");
	company_cd.SetString("BX00");
	user_id.SetString(&accountid[2]);
	}
	else*/
	{
		user_no.SetString("");
		company_cd.SetString("BX00");
		user_id.SetString(accountid);
		pkg_no = (long)22 ;
		pkgRemainAmt.SetString("");
		ret_val.SetString("");
		ret_message.SetString("");
	}

	try
	{
		ret = pIPGTXCli->GetUserPkgCash	( &user_no,&company_cd,&user_id,&pkg_no,&pkgRemainAmt,
			&ret_val,&ret_message);
	}
	catch (_com_error&e)
	{
		if ( pIPGTXCli )
			pIPGTXCli->Release();

		//g_LogPrintDB.FilePrint("IPGERROR.txt", "GetUsePkgCash : %s", e.ErrorMessage());
		CoUninitialize();
		return -3;
	}

	if ( pIPGTXCli )
		pIPGTXCli->Release();

	char* result = NULL ;
	if ( ret_message.bstrVal != NULL )
		result = _com_util::ConvertBSTRToString(ret_message.bstrVal);
	else
	{
		CoUninitialize();
		return -4 ;
	}

	if ( result == NULL )
	{
		CoUninitialize();
		return -5 ;
	}

	// 조회 성공
	if ( result != NULL && !strcmp(result,"Success") )
	{
		*eventmoney = pkgRemainAmt.intVal ;

		CoUninitialize();
		return 0 ;
	}

	CoUninitialize();
	return -2 ;

}
