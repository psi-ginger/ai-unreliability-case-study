#ifndef _NOTIFYGETITEMFROMMONSTERDLG_H_
#define _NOTIFYGETITEMFROMMONSTERDLG_H_

#include < Directives.h >

#include "..\..\..\Resource.h"

#include "..\..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "..\..\..\CommonLogic\ImageList\ImageList.h"
#include <CommonLogic.h>
#include "..\..\..\CommonLogic\House\House.h"

#define	MAX_SET_TIMER_GET_ITEM_FROM_MONSTER_DLG_DELAY_TIME		1500
#define	MAX_NOTIFY_GET_ITEM_FROM_MONSTER_DLG_DELAY_TIME			4000

class CNotifyGetItemFromMonsterDlg;
extern CNotifyGetItemFromMonsterDlg g_NotifyGetItemFromMonsterDlg;

class CRememberWindowPosMgr;

struct	stNotifyImageList
{
	bool	bImageStatus;			// 이미지가 차있는 상태
	SI32	si32Index;
	SI32	si32ItemUnique;

	stNotifyImageList()
	{
		memset( this, 0, sizeof( stNotifyImageList ) );
	}
};

class CNotifyGetItemFromMonsterDlg
{
public:
	CNotifyGetItemFromMonsterDlg();
	~CNotifyGetItemFromMonsterDlg();

	void			Initialize();
	void			Destroy();

	void			Create( HINSTANCE hInst, HWND hWnd, CRememberWindowPosMgr *pRememberWindowPosMgr );

	static	BOOL	CALLBACK StaticNotifyGetItemFromMonsterDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL			CALLBACK NotifyGetItemFromMonsterDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void			Show( cltItem *pclItem, SI64 pricevalue = 0 ,SI32 siCenterPosX = 512, SI32 siCenterPosY = 384 );
	void			Hide();
	void			Update();

private:
	HWND				m_hDlg;
	HWND				m_hGetItemExplainEdit;

	bool				m_bShowFlag;
	SI32				m_siCount;

	HWND				m_hImageWnd;
	
	HBITMAP				m_hItemImage;
	
	HBRUSH				m_hEditBrush;

	stNotifyImageList	m_stImageListMgr[ 100 ];
	SI32				m_si32ImageListIndex;
	bool				m_bIsListRotate;

	CRememberWindowPosMgr	*m_pRememberWindowPosMgr;

};

#endif