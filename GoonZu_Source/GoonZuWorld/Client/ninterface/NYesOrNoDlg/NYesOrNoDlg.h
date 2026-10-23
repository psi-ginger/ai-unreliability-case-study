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

enum REASON
{
	NONE
	,REASON_VILLAGE_RESIDENT_INVITE		// 마을 주민 가입 초대.(스승이 초대.)
	,REASON_HELPER_ALREADY_QUESTION		// 도우미. 중복으로 질문을 하려는 경우.
	,REASON_ELIXIR_SET					// 엘릭서 세팅.
};


class CNYesOrNoDlg : public CDialogController
{
public:

	CNYesOrNoDlg();
	~CNYesOrNoDlg();

	void Init();

	void Create();
	void Destroy();
	void Action();

	static void CALLBACK StaticNYesOrNoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NYesOrNoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	void Yes( void );
	void No( void );
	void SetText( TCHAR* pStr );

	void SetVillageResidentInvite( SI32 _siCreateReason, NTCHARStringName _kCharName, SI32 _siPara1 );
	void SetHelpMe( SI32 _siCreateReason, NTCHARStringName _kCharName, SI32 _siPara1 );
	void SetHelpYesorNo( SI32 _siCreateReason, SI32 _siPara1 );
	void SetHelperAlreadyQuestion( void );
	void SetElixirNew( void );

private:
	InterfaceFactory	m_InterfaceFactory;

	SI32				m_siCreateReason;			// 이창이 생성된 이유.(이것으로 예, 아니오 버튼을 누를시에 처리를 하자.)
	SI32				m_siPara1;					// REASON_VILLAGE_RESIDENT_INVITE : 스승 마을 유니크.
	NTCHARStringName	m_kCharName;				// REASON_VILLAGE_RESIDENT_INVITE : 사용안함. REASON_HELP_ME : 도움 요청을 한 캐릭터 이름.
};
