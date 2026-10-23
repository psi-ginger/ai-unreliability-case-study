
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../common/Item/ItemCommon/ItemUnit.h"
#include "../../NLib/NUpdateTimer.h"


/*

*/

class CNNewSelectBoxDlg : public CDialogController
{
public:
	enum NEWSELECTBOX_MODE
	{
		NEWSELECTBOX_MODE_OPEN = 0,
		NEWSELECTBOX_MODE_SELECT,
		NEWSELECTBOX_MODE_GETITEM,
		NEWSELECTBOX_MODE_SUCCESS,
	};
// -- Member Variable Area
private:
	InterfaceFactory	m_InterfaceFactory;

	SI16				m_siBoxItemUnique;
	SI16				m_siSelectIndex;
	SI16				m_siSelectItemList[MAX_NEWSELECTBOX_ITEM_LIST];

	SI16				m_siGetItemUnique;
	SI16				m_siGetItemNum;

	SI32				m_siMode;

	SI32				m_siDrawIndex;

	REAL32				m_fSpeed;
	DWORD				m_dwDrawTickCount;

	NUpdateTimer		m_timerSendGetItem;
protected:

public:


// -- Member Function Area
private:
	void OpenNewYearSelectBox( SI16 siItemUnique );

	void SelectButtonIndex( SI16 siSelectIndex );

	void InitForDraw( void );

	void ShowGetItemInfo( void );
	void InsertGetItem( void );
protected:

public:
	CNNewSelectBoxDlg( void );
	~CNNewSelectBoxDlg( void );

	void Init();
	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNNewSelectBoxDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNNewSelectBoxDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );

	void SetSelectInfo( SI16 siSelectIndex, SI16* psiItemList );
	void SetGetItemInfo( cltItem* pclGetItem );

	void OpenNewSelectBox( SI16 siItemUnique );

	void ChangeAllSelectButtonEnableMode( bool bEnable );


};