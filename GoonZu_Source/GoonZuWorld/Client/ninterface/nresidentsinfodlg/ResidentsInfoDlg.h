#ifndef _NRESIDENTS_INFO_DLG_H_
#define _NRESIDENTS_INFO_DLG_H_

#include "../../InterfaceMgr/DialogController.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>
#include "TSpr.h"
#include "../../../NLib/NTCHARString.h"
#define LEAST_COMMON_MULTILPLE	30

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNResidentsInfoDlg : public CDialogController
{
public:

	CNResidentsInfoDlg();
	~CNResidentsInfoDlg();

	void Create(CControllerMgr * pParent);

	static void CALLBACK StaticCallBackDialogNResidentsInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNResidentsInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	void SetRegenPos( TCHAR *text );
	void SetRegenPos( UI08 mapindex [] , SI16 maxmap );
	void AddRegenPos( TCHAR *text );

	void ShowResidentInfo( SI32 index );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
	bool IsSelectedNPC( SI32 siKind );
	
	void ShowResidentInfoByKind( SI32 Kind, TCHAR * Name );

	void FindNpc(TCHAR * szNpcname);

	//검색기능을 위해
	void Action();
	void MonsterFind(TCHAR* pStrname);
	void Find(TCHAR* pItemname);
	void AddListItem(TCHAR* findname);
	void Update();
 
private:
	
	SI32	   m_siSelectedIndex;
	SI32	   m_siSelectVillageInx;
	SI32	   m_siResidentIndex;

	bool	   m_bComboBoxInit;
	SI32			m_timerCount;

	TCHAR m_strBaseInfo[ 512 ];

	CStatic*   m_pStatic[2];	// 0 : 주민 1 : 설명 
	CComboBox* m_pComboBox;
	CList*	   m_pList;
	CButton*   m_pButton;
	CEdit*     m_pEdit;
	CImageStatic* m_pImageStatic_Portrait;
	// 검색기능
	CList			*m_pListBox_AutoMake;
	CEdit			*m_pEdit_Name;
	CButton			*m_pButton_Accept;
	CStatic			*m_pStatic_Back;//백그라운드

private:
	NTCHARString64		m_LastInput;

	TSpr	   m_SprResidentsIcon;
};

#endif _NRESIDENTS_INFO_DLG_H_