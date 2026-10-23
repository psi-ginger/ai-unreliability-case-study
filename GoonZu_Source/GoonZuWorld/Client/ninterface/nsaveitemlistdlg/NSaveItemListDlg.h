#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "..\CommonLogic\PostOffice\PostOffice.h"

class CButton;
class CStatic;
class CList;

class cltItem;
class cltSaveUersItemInfo;

#define BASIC_SERVICECHARGE 20000  //기승동물 회수 수수료 기본값. - 서버는 따로 수정필요.

// 유저 아이템 찾기
class CNSaveItemListDlg : public CDialogController
{
public:
	
	CStatic*			m_pStatic[2];
	CList*				m_pList;
//	CButton*			m_pButton[2];

	//KHY - 1127 - 클라라 물품 보관 개선
	CButton*			m_pButton[4];
	SI32				m_selectWhatShowList; // 어떤 리스트를 보여줄 것인가?(선택했나)

	SI32				m_siSelectIndex;

	cltSaveUersItemInfo	m_clSaveUsersItemInfo[MAX_SAVEUSERITEM_LIST];

	//KHY - 1127 - 클라라 물품 보관 개선
	cltSaveUersHorseInfo	m_clSaveUsersHorseInfo[MAX_SAVEUSERHORSE_LIST];

	CNSaveItemListDlg();
	~CNSaveItemListDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNSaveItemListDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNSaveItemListDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void	 ShowList( cltSaveUersItemInfo* pSaveUersItemInfo ,cltSaveUersHorseInfo* pSaveUersHorseInfo);
	//KHY - 1127 - 클라라 물품 보관 개선
	void  InDlgShowList(SI32 selectListType ); // 다이알 로그 안에서의 리스트 갱신을 위해서.
	cltHorse* GetHorseFromList( SI32 index );
	cltItem* GetItemFromList( SI32 index );

	void	 RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};
