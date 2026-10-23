/* ==========================================================================
클래스 :		CNGacha2Dlg

작성일 :		08/11/04
작성자 :		김희영

변경사항 :		가차(뽑기)2 시스템 인터페이스

차후작업 :		
========================================================================*/
 
#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../server/GachaManager/GachaManager.h"
#include "../../server/Gacha2Manager/Gacha2Manager.h"

enum eGacha2Step{
	GACHA2STEP_TOP	= 1	,
	GACHA2STEP_CHOOSEKEY	,
	GACHA2STEP_DROPBOX	,
	GACHA2STEP_SELECTBOX ,
	GACHA2STEP_OPENBOX ,
	GACHA2STEP_ITEMGET ,
	GACHA2STEP_RAREITEMGET ,
	GACHA2STEP_KEYNEED 
};

const SI32 GACHA2_BOX_NUM = 5;

class CNGacha2Dlg : public CDialogController
{
public:

	CNGacha2Dlg();
	~CNGacha2Dlg();

	void Initialize();
	void Destroy();

	void Create();

	static void CALLBACK StaticNGacha2DlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NGacha2DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	//Action
	void Action();	
	
	//void ActionStepTop();
	void ActionStepDropBox();
	void ActionStepOpenBox();
	//void ActionStepSelectBox();
	//void ActionStepOpenBox();

	//Draw
	void DrawDialogFirst(SI32 ScreenRX, SI32 ScreenRY );

	void DrawStepTop( SI32 ScreenRX, SI32 ScreenRY );
	void DrawStepDropBox( SI32 ScreenRX, SI32 ScreenRY );
	void DrawStepSelectBox( SI32 ScreenRX, SI32 ScreenRY );
	void DrawStepOpentBox( SI32 ScreenRX, SI32 ScreenRY );
	void DrawStepItemGet( SI32 ScreenRX, SI32 ScreenRY );
	void DrawStepRareItemGet( SI32 ScreenRX, SI32 ScreenRY );
	void DrawStepKeyNeed( SI32 ScreenRX, SI32 ScreenRY );

	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );
	void DrawLastStepTop( SI32 ScreenRX , SI32 ScreenRY );
	void DrawLastStepChooseKey( SI32 ScreenRX , SI32 ScreenRY );
	void DrawLastStepDropBox( SI32 ScreenRX, SI32 ScreenRY );
	void DrawLastStepItemGet( SI32 ScreenRX , SI32 ScreenRY );
	void DrawLastStepRareItemGet( SI32 ScreenRX , SI32 ScreenRY );
	void DrawLastStepKeyNeed( SI32 ScreenRX , SI32 ScreenRY );


	//Init
	void ChangeCurrentStepAndInit(SI16 Step );	// 외부에서도 호출되기도 함
	void InitStepTop();
	void InitStepChooseKey();
	void InitStepDropBox();
	void InitStepOpenBox();
	void InitStepItemGet();
	void InitStepRareItemGet();
	void InitStepKeyNeed();
	
	// Etc
	bool CheckHaveKey( SI32 GachaUseKeyKind );
    bool CheckHaveKeyPiece();

	// 외부에서만 호출되는  함수
	void SetGachaProductAndCount( cltItem * pclItem , UI16 GachaCnt );  // 가차로 얻을수 있는 아이템 세팅
	void SetGachaGetProductAndCount( cltItem * pclItem , UI16 GachaCnt , bool BestProduct );	// 가차로 획득한 아이템 세팅
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
private:

	InterfaceFactory	m_InterfaceFactory;
	
	SI32 m_FrameCnt;

	TSpr m_NumberImg;
	TSpr m_RunRaccoonImg;
	TSpr m_ClickImg;
	TSpr m_GachaGageImg;
	TSpr m_GachaGageOutLineImg;
	TSpr m_GachaResultImg;
	TSpr m_GachaFieldImg;

	UI16 m_uiGachaCnt; // 누적된 가차 횟수

	SI16 m_siCurrentStep; // 현제 가차 진행 단계
	
	cltItem m_GachaProduct[ MAX_SHOW_GACHA2_PRODUCT ];

	POINT m_GachaProductPoint[ MAX_SHOW_GACHA2_PRODUCT ];

	POINT m_RunRaccoonPoint;
	bool  m_bRaccoonDraw;

	cltItem m_GachaGetProduct;
	
	SI32 m_myGachaRaccoonKeyNum;
	//SI32 m_myGachaSilverKeyNum;
	//SI32 m_myGachaBronzeKeyNum;

	//SI32 m_myGachaPieceNum;
		
	//bool m_bUseCashKey;	// 과금열쇠를 사용하여 게임을 하느냐 마느냐
	// 어떤 열쇠를 사용하느냐
	SI32 m_myGachaUseKeyKind;

	

	//POINT m_BoxPoint[GACHA_BOX_NUM];

};
 
