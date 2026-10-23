#ifndef _NTRADEMERCHANT_HORSE_H_
#define _NTRADEMERCHANT_HORSE_H_


#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

struct stNTradeMerchantHorseData
{
	SI32 siFirst;
	SI64 siSecond;
	SI16 siParam;
};

class CNTradeMerchantHorseDlg : public CDialogController
{
public:

	CNTradeMerchantHorseDlg();
	~CNTradeMerchantHorseDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNTradeMerchantHorseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNTradeMerchantHorseDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void LoadHorse();
	void SetHorsePrice( SI16 HorseUnique, GMONEY HorsePrice );
	void SellToNPCSuccess();

	void SetData( void *pData );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

private:

	GMONEY m_siHorsePrice;
	SI32 m_siHorseIndex;

	CStatic*	m_pStatic[2];		// 0 : 말상인 얼굴, 1: 말얼굴
	CEdit*		m_pEdit[2];			// 0 : 말상인이 하는말 1: 말 능력치

	CButton*	m_pButtonSellHorse;
	CButton*	m_pButtonClose;
};

#endif