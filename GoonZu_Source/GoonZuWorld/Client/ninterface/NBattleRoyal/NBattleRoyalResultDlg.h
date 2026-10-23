/* ==========================================================================
	클래스 :		CNBattleRoyalResultDlg

	작성일 :		09/10/22
	작성자 :		박기형

	목  적 :		CNBattleRoyalResultDlg 클래스는   배틀로얄 경기의 결과를 알려준다.

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
class CButton;
class CImageStatic;

#define POPUP_TIME		200

class CNBattleRoyalResultDlg : public CDialogController
{
public:
	CNBattleRoyalResultDlg();
	~CNBattleRoyalResultDlg();

	void					Create();

	static void CALLBACK	StatiCNBattleRoyalResultDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NBattleRoyalResultDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	
	void                    Action();

	void					SetDescription(TCHAR* szDescription);
	void					SetImage(TCHAR* ImagePath, SI32 siIndex = 0);
	void					SetResize(SI32 siWidth, SI32 siHeight);
	void					SetPosition(SI32 siX, SI32 siY);
	void					SetResult(bool bResult);
private:

	CStatic                *m_pResultStatic;
	CImageStatic           *m_pResultImageStatic;   
	
	bool			m_bResult;
	TCHAR			m_strPrintMsg[ 2048 ];
	TCHAR			m_strTitleMsg[ 64 ];
	TCHAR			m_strPlayerName[ MAX_PLAYER_NAME ];
	SI32			m_siHintDlg;
	SI16			m_siBitmapAnimationIndex;
	SI32			m_siImageType;	// 아이템 이미지 타입(이미지가 3장)
	SI32			m_siImageIndex; // 아이템 이미지에서 순서
	SI16            Popupflag;
};

//extern	CNRareItemDlg		g_RareItemMsg;

