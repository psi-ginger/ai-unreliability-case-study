#ifndef _STOCKDISTRIBUTIONDLG_H_
#define _STOCKDISTRIBUTIONDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

class CStockDistributionDlg
{
public:
	HWND m_hDlg;

	CStockDistributionDlg();
	~CStockDistributionDlg();
	
	void Initialize();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticStockDistributionDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK StockDistributionDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();

	void Set( SI32 villageUnique, SI32 StockDistributionNum, SI32 StockPrice, bool VillageWarStockDistribution );

private:
	bool m_bShow;

	SI32 m_siVillageUnique;
	SI32 m_siAvailableStockBuyingNum;
	SI32 m_siStockPrice;
	bool m_bVillageWarStockDistribution;
};

#endif