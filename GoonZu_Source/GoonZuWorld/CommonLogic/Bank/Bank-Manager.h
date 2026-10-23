#ifndef _BANKMANAGER_H
#define _BANKMANAGER_H

#include "Bank.h"

class cltTaxPaymentListDlg;

//----------------------------------------
// 서버에서 뱅크를 총관리하는 클래스. 
//----------------------------------------
class cltBankManager : public cltStructureMngBasic{
public:

	cltBankManager(cltCharManager* pclCM, SI32 ranktype, TCHAR *strFileName );
	~cltBankManager();

	// 전장을 신설한다.
	BOOL CreateBank(SI32 siunique);

	// 전장 정보를 보여준다
	void ShowAccountInfo( HWND hDlg );

	void DrawBankInfo( HDC hdc );

	// 수수료 정보를 보여준다
	void DrawFeeInfo( HWND hDlg );

	// 계좌 정보를 보여준다. 
	void DrawAcountInfo( HWND hDlg );

	// 전장의 계좌 정보를 얻어온다. 
	BOOL GetAcountInfo( cltAccountNum* pclaccountnum, cltBankAccount* pclAcount );

	// PersonID를 가지고 그 사람의 cltBankAcountInfo를 구한다. 
	// 전체 전장에서 검색하기 때문에 상당한 부하가 걸린다. 
	BOOL GetAcountInfo( SI32 personid, cltBankAccount* pclaccount );

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 charunique);

	// 수수료 정보를 설정한다.
	BOOL SetFee(SI32 villageunique, cltFee* pclfee);

	// 전장 정보를 업데이트 한다. 
	bool SetBankVary(SI32 villageunique, cltBankVary* pclvary);	

	// 전장 정보의 포인터를 구한다. 
	cltBankVary* GetBankVary(SI32 villageunique);

	// 전장의 전반적인 정보를 표시한다. 
	void ShowBankInfo(HWND hDlg);
	// 전장 분석 정보를 보여준다. 
	void ShowHelpAnalyze();

	
	// 수수료율을 보여준다. 
	BOOL ShowFeeRate(HWND hDlg, SI32 rate);
	// 수수료범위를 보여준다. 
	BOOL ShowFeeRange(HWND hDlg);
	


};

#endif
