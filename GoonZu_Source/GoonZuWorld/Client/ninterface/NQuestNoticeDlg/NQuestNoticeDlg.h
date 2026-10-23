#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"

class cltSystemNPC;

class CNQuestNoticeDlg : public CDialogController
{
	// 변수영역
private:
	InterfaceFactory	m_InterfaceFactory;
	SI32				m_siMapIndex;
	SI32				m_siMapNameNum;


	// 함수영역
public:
	CNQuestNoticeDlg();
	~CNQuestNoticeDlg();

	void Create();

	static void CALLBACK StaticQuestNoticeDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NQuestNoticeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Show();
	void Hide();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void CheckNotice( void );

	void NoticeEvent( SI32 siNpcKind, SI32 siMapIndex );// 이벤트시 해당 위치와 엔피시 얼굴을 나타내기 위해서
	void NoticeEvent( SI32 siNpcKind, const TCHAR* pszTitle, const TCHAR* pszExplain )	; // 텍스트 내용을 강제로 채워주는 함수
	void NoticeEvent2( SI32 siNpcKind, const TCHAR* pszTitle, const TCHAR* pszExplain )	;

private:
	void CheckCondition( IN cltSystemNPC* pclConditionNPC, OUT cltSystemNPC* pclResultNPC );

	void DisplayNoticeDlg( cltSystemNPC* pclNoticeNPCInfo );

	void CheckWarp( void );
	void SetRetMsgBox( SI32 siTitleNum, SI32 siTextNum );
};
