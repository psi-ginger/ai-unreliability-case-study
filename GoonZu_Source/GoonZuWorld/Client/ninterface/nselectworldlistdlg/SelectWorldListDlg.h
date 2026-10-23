#ifndef _NSELCTWORLDLIST_H_
#define _NSELCTWORLDLIST_H_

#include "../../InterfaceMgr/DialogController.h"
#include "../../Server/FrontServer/WorldList.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "../../Promoter/NHNChinaPromoter.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
class CImageButton;

//#define MAX_WORLD_LIST 24
#define MAX_WORLD_LIST 36

class CNSelectWorldListDlg : public CDialogController
{
public:

	CNSelectWorldListDlg();
	~CNSelectWorldListDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNSelectWorldListDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNSelectWorldListDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

	void ShowServerInfo(TCHAR* GoonzuName, GMONEY CurrentPrice, SI32 NPCRate, SI32 PeopleNum, GMONEY BonusMoney );

    // promoter
	void PromoterShow( void );
	void PromoterHide( void );
	void SetPromoterText( TCHAR* _pPromoterName, SI32 _siServerUnique );

private:

	SI32				m_siSelectListIndex;
//	cltWorldList		clWorldList;			// 서버로 부터 받은 월드 리스트. 
//	WORLDINFO			clSelectedWorldInfo;	// 선택된 월드 정보. 
	WORLDINFO*			pWorldInfo;
	SI32				m_siHeartBeatCount;

	//cyj CList*			m_pList;
	CStatic*			m_pStatic;
	CEdit*				m_pEdit[2];

	CButton*			m_pButton;
	CImageButton*		m_pImageButton[MAX_WORLD_LIST];

	CImageStatic*		m_pImage_PCRoomLeft;
	CImageStatic*		m_pImage_PCRoomRight;

	InterfaceFactory	m_InterfaceFactory;

	// promoter
	bool				m_bPromterInfoShow;
	char 				m_svrid[NHNCHINAPROMOTER_SVRID_SIZE];
	char 				m_promoterUserid[NHNCHINAPROMOTER_PROMOTER_USERID_SIZE];
	char 				m_promoterCharid[NHNCHINAPROMOTER_PROMOTER_CHARID_SIZE];
	char 				m_auth[NHNCHINAPROMOTER_AUTH_SIZE];

	SI32				m_PromoterServerID;
};

#endif