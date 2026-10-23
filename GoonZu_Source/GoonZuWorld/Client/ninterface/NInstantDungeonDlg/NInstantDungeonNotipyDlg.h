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


class InstanstDungeonNotipyDlg : public CDialogController
{
public:
	InstanstDungeonNotipyDlg();
	~InstanstDungeonNotipyDlg();

	void					Create();
	
	//[추가 : 황진성 2008. 1. 11 => 인던 보상 정보창 세팅.]
	void					SetInstantDungeon(SI32 _siGuildMarkIndex, TCHAR* _szGuildName, TCHAR* _szGuildMasterName);

	//[추가 : 황진성 2008. 1. 17 => 인던 최고 길드변경 알림.]
	void					SetInstantDungeonTopChange(SI32 _siGuildMarkIndex, TCHAR* _szGuildName, SI32 _Score, TCHAR* _szGuildMasterName);
	
	static void CALLBACK	StatiCNInstanstDungeonNotipyDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NInstanstDungeonNotipyDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);


	void                    Action();
	void					DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );

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
	SI32			m_siGuildMarkIndex;	// 길드 마크 인덱스.	
};


