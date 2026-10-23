
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../common/Family/Family.h"

enum
{
	CLICK_MODE_PARENT_NONE	=	0,	// 빈공간 찍었을때
	CLICK_MODE_PARENT_ME,			// 부모이고 내 자신을 찍었을때
	CLICK_MODE_PARENT_MATE,			// 부모이고 배우자를 찍었을때
	CLICK_MODE_PARENT_CHILDREN,		// 부모이고 자식을 찍었을때
	CLICK_MODE_CHILDREN_ME,			// 자식이고 나를 찍었을때
	CLICK_MODE_CHILDREN_OTHER,		// 자식이고 다른 자식을 찍었을때
	CLICK_MODE_CHILDREN_PARENT,		// 자식이고 부모를 찍었을때
};

#define MAX_RICHEDIT_OBJ_SIZE							500		// 최대 500 Byte를 데이터에 할당 함.

class CNFamilyInfoDlg : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory m_InterfaceFactory;
protected:

public:


// -- Member Function Area
private:

protected:

public:
	CNFamilyInfoDlg( void );
	~CNFamilyInfoDlg( void );

	SI32		siWarpTargetFamily	;
	TCHAR*		m_strChatData;
	
	BOOL		m_bMini	;
	SI16		m_siOldPosX;
	SI16		m_siOldPosY;
	SI32		m_siOldXSize;
	SI32		m_siOldYSize;

	SI16		m_siMinimizedSlot;

	bool		m_bShowparents	;
	bool		m_bShowchildres	;
	bool		m_bShowchat	;

	void Init();
	void Create();

	void Action();

	static void CALLBACK StaticCallBackDialogNFamilyInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNFamilyInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void OnButtonClickMate( void );
	void DisplayFamilyInfo( void );
	void CheckMateConnect( void );
	void OpenPopUp(SI32 ButtonIndex, bool bParent ,SI32 siPersonID , TCHAR* CharName)	;
	void WarpFamily(SI32 siTargetGamilyPersonID, TCHAR* TarGetCharName )	;
	void SendChat(void)	;
	void RecvChat( stChatEditData* szChatDat )	;
	void Minimi(BOOL bMini);
	SI16 GetMinimizedSlot(){return m_siMinimizedSlot;};
	void SetMinimizedSlot(SI16 MinimizedSlot){  m_siMinimizedSlot = MinimizedSlot; };
	void ClearDlg();
	void OpenPopUpSet( SI32 siButtonindex );
	
	// 가족 정보창 개편
	void ShowParent( bool bShow )	;
	void ShowChindren( bool bShow )	;
	void ShowChat( bool bShow )	;
	void MoveCParent( SI32 sihight)	;
	void MoveChindren( SI32 sihight)	;
	void MoveChat( SI32 sihight)	;

};