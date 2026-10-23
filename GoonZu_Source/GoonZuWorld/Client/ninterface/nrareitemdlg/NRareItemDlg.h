/* ==========================================================================
	클래스 :		CNRareItemDlg

	작성일 :		05/03/17
	작성자 :		김태양

	목  적 :		CNRareItemDlg 클래스는  전체 화면 모드 실행시 NInterface에
					따라 게임 화면 안에서 명품창 다이얼로그를 생성한다.

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/
#include <Directives.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"


#define HITBUTTON_RECOMMEND		1
#define HITBUTTON_SUMMON		2
#define HITBUTTON_RAREITEM		3

class CStatic;
class CButton;
class CImageStatic;
class COutLine;

#define POPUP_TIME		200


class CNRareItemDlg : public CDialogController
{
public:
	CNRareItemDlg();
	~CNRareItemDlg();

	void					Create();
	void					Set( cltItem *pclItem, TCHAR *PlayerName,TCHAR *strTitleData, TCHAR *strPrintData,
								 SI32 siHorse = 0 ,SI32 siSummon = 0 ,SI32 siHintButton = 0 );
	
	void					SetBestGacha( cltItem *pclItem, TCHAR *PlayerName,TCHAR *strTitleData, TCHAR *strPrintData );
	void					SetVillageEvent( SI32 siVillageUnique, TCHAR *strTitleData, TCHAR *strPrintDatabool, SI16 _siConstructType, SI32 _siKind ); //[진성] _siConstructType 구조물 타입별 처리를 위해. => 2008-7-22
	void					SetVillageEventGetItem(TCHAR *strTitleData, TCHAR *strPrintData, SI16 _siConstructType ,SI32 siItemunique,TCHAR *PlayerName );// 빌리지이벤트로 아이템 받은걸 동네 방네 알리기 위해

	//void					CreateDlg();

	static void CALLBACK	StatiCNRareItemDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NRareItemDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	
	void                    Action();

	void					SetCharName(TCHAR* szName);
	void					SetDescription(TCHAR* szDescription);
	void					SetImage(TCHAR* ImagePath, SI32 siIndex = 0);
	void					SetMainImage(TCHAR* ImagePath, SI32 siIndex = 0);
	void					SetResize(SI32 siWidth, SI32 siHeight);
	void					SetPosition(SI32 siX, SI32 siY);

private:
	CStatic                *m_pNRareItemStatic;    
	CStatic                *m_pNRareItemStatic1;
	CStatic                *m_pNRareItemStatic2;
	CImageStatic           *m_pNRareItemImageStatic1;   
	CImageStatic           *m_pNRareItemImageStatic2;   
	CButton                *m_pNRareItemButton1;
	CButton                *m_pNRareItemButton2;
	COutLine               *m_pNRareItemOutLine;
	
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

