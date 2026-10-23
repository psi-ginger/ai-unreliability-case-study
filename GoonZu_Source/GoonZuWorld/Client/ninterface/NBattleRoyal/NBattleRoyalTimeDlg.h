/* ==========================================================================
	클래스 :		CNBattleRoyalTimeDlg

	작성일 :		09/10/22
	작성자 :		박기형

	목  적 :		CNBattleRoyalTimeDlg 클래스는  다음에 시작될 배틀로얄의 시간을 알려주는 창 역할을 한다.

	용  법 :		
	변경사항 :		없음

	차후작업 :		
   ========================================================================*/
#include <Directives.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CStatic;

class CNBattleRoyalTimeDlg : public CDialogController
{
public:
	CNBattleRoyalTimeDlg();
	~CNBattleRoyalTimeDlg();

	void					Create();

	static void CALLBACK	StatiCNBattleRoyalTimeDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NBattleRoyalTimeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	
	void					SetDescription(TCHAR* szDescription);

private:
	CStatic                *m_pNTimeStatic;    
};

