/* ==========================================================================
클래스 :		CNGachaDlgNew

작성일 :		07/04/11
작성자 :		김영진

변경사항 :		가차(뽑기) 시스템 인터페이스

차후작업 :		
========================================================================*/
 
#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../server/GachaManager/GachaManager.h"

#define ANIMATION_FISH_NUM		24		// 낚시     애니메이션 이미지 수.
#define ANIMATION_OPENBOX_NUM	 8		// 상자열기 애니메이션 이미지 수.
#define SELECT_FISH_NUM			 5		// 선택 가능한 물고기 수.

class CNOpenBoxDlg : public CDialogController
{
public:
	enum { NONE, ANISTEP_FISH, ANISTEP_OPENBOX };

public:

	CNOpenBoxDlg();
	~CNOpenBoxDlg();

	void Init();

	void Create();
	void Destroy();

	void DrawDialogFirst(SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );

	static void CALLBACK StaticNOpenBoxDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NOpenBoxDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void Action();	

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	bool CheckHaveKey( SI32 GachaUseKeyKind );
	void SetGachaUseKeyKind( SI32 _siGachaUseKeyKind );
	void SetCarryOverInfo( SI32 siGachaUseKeyKind, SI32 siSelectGachaProductIndex, SI32 siSelectGachaProductUnique );

private:
	InterfaceFactory	m_InterfaceFactory;
	SI32				m_siFontIndex;

	DWORD				m_siFrameTime[ANIMATION_FISH_NUM];	// 각각의 이미지의 프레임 시간.
	DWORD				m_dwPrevTime;						// 애니메이션 하는 이미지의 변경 시간.

	SI32				m_siGachaUseKeyKind;				// 어떤 열쇠를 사용하느냐
	SI32				m_siAniStep;						// 애니메이션 단계.(낚시, 상자열기)
	bool				m_bFishClick;						// 물고기를 선택했는가?

	// Switch_CarryOverGacha On일때 쓰임
	SI32				m_siSelectGachaProductIndex;		// 초기값이 -1이여야함
	SI32				m_siSelectGachaProductUnique;		// 가챠 아이템 유니크
};
 