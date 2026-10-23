/* ==========================================================================
클래스 :		 

작성일 :		 
작성자 :		 

변경사항 :		 

차후작업 :		
========================================================================*/

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

#define CONTRIBUTION_ITEM_NUM	6



class NContributionEvent_CommonDlg : public CDialogController
{
public:
	enum EVENT
	{
		EVENT_CONTRIBUTION_BUTTON_CLICK,	// 기부 버튼 클릭시 이벤트.
		EVENT_ACTION_BUTTON_CLICK,			// npc만들기 버튼 이벤트.
		EVENT_ACTION_CONTRIBUTION_COMPLETE	// 기부하다가 기부가 완료됐을때 이벤트.
	};
public:

	NContributionEvent_CommonDlg();
	~NContributionEvent_CommonDlg();

	void	Create( SI16 siControllID, CControllerMgr* _pChildControllerMgr, GUIChildCallBackEvent _pChildCallBack );	
	bool	EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
	
	static void CALLBACK StaticNContributionEvent_CommonDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NContributionEvent_CommonDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetExplainText( TCHAR* pszString );
	void SetContributeButtonText( TCHAR* pszString );
	void SetActionButtonText( TCHAR* pszString );

	static void StaticSetContributionItemList( SI32* _psiItemUnique,  SI16* _psiItemNum );		// 이것은 이 창이 공용으로 쓰일거라 만들었음.
	static void StaticSetContributionInputItemList( SI32* _psiInputItemUnique,  SI16* _psiInputItemNum );
	
	void SetContributionItemList( SI32* _psiItemUnique,  SI16* _psiItemNum );					// 관련 정보 세팅.
	void GetContributionItemList( SI32 NPCKind );												// 기부할 아이템의 정보와 넣은 아이템의 정보 얻기.
	
	void SetContributionInputItemList( SI32* _psiInputItemUnique,  SI16* _psiInputItemNum );	// 기부 한 아이템의 정보 세팅.
	void GetContributionInputItemList( SI32 NPCKind );											// 기부 한 아이템의 정보 얻기.
	
	void SetContributionUserItemList( void );													// 기부하는 아이템을 유저의 인벤에서 검사하여 정보를 세팅.

	void InputItem( void );																		// 현재 등록된 아이템을 넣기.(기부)
	bool IsComplete( void );																	// 기부를 완료했는지.
	void MakeNPC( void );																		// 기부가 끝났으면 만들수 있는 NPC로 변경.

	void MakeItemNum( SI32 _siEditIndex );

private:
	InterfaceFactory		m_InterfaceFactory;

	CControllerMgr*			m_pChildControllerMgr;
	GUIChildCallBackEvent	m_pChildCallBack;

	static SI32				m_siControllID;

	SI32					m_siItemUnique[CONTRIBUTION_ITEM_NUM];
	SI16					m_siItemNum[CONTRIBUTION_ITEM_NUM];

	SI32					m_siInputItemUnique[CONTRIBUTION_ITEM_NUM];
	SI16					m_siInputItemNum[CONTRIBUTION_ITEM_NUM];

	bool					m_bComplete;
	SI32					m_siNPCKind;
};
