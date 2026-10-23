
#include "NBuffstatus.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../Graphic/Graphic.h"
#include "../Graphic/Graphic_Capsule.h"
#include "../../GlobalImgFile/GlobalImgFile.h"
#include "../../common/CommonHeader.h"
#include "../../Client.h"
#include "../NPremiumStatus/NPremiumStatus.h"
#include "../../GlobalImgFile/GlobalImgFile.h"
#include "../../ninterface/NBuffRoulette/NBuffRoulette.h"



extern cltCommonLogic* pclClient;


CNBuffstatus::CNBuffstatus()
{
	Initialize();
}

CNBuffstatus::~CNBuffstatus()
{
	Destroy();
}

void CNBuffstatus::Initialize()
{
	SI16 i =0;
	for (i=0; i<NBuffstatus_MAX_Buffstatus; i++)
	{
		m_BuffStatic[i] = NULL;

	}
	m_BuffShow = false;
}

void CNBuffstatus::Destroy()
{

	if(m_BuffImg.pImage) GP.FreeSpr(m_BuffImg);
	
	SI16 i=0;

	for (i=0; i< NBuffstatus_MAX_Buffstatus; i++) {
		SAFE_DELETE(m_BuffStatic[i]);
	}

}

void CNBuffstatus::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	GP.LoadSprFromFile(TEXT("GIMG\\BufImage.SPR"),&m_BuffImg);

	m_siImageNum   = m_BuffImg.GetImageNum();
	m_siImageXsize = m_BuffImg.GetXSize();
	m_siImageYSize = m_BuffImg.GetYSize();

	CDialogController::Create(NBUFFSTATUS_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticBuffProc, NULL, NULL, 0, false, 0,
							MAP_TILE_XSIZE + 600 + (g_SoundOnOffDlg.m_bMaxWindowMode ? 52:0), 40,m_siImageXsize, m_siImageYSize,false, false, false, false);

	if(m_siImageNum > 0)
	{
		for (SI16 i = 0; i < m_siImageNum; i++) 
		{
			m_BuffStatic[i] = new CImageStatic(this);
			if ( m_BuffStatic[i] == NULL) continue;
			
			m_BuffStatic[i]->Create(NBUFFSTATUS_IMAGESTATIC_PREMIUM_0,TEXT("GIMG/BUFIMAGE.SPR"), i ,false, 0 ,
				0-i*m_siImageXsize, 0,
				m_siImageXsize, m_siImageYSize,m_siImageXsize,m_siImageYSize);

			m_BuffStatic[i]->Show(false);
		}
		if(pclClient->IsCountrySwitch(Switch_ShowBuffToolTip))
		{
			//이미지마다 툴팁설정
			if(m_BuffStatic[0])		m_BuffStatic[0]->SetUseToolTip( GetTxtFromMgr(7518));
			if(m_BuffStatic[1])		m_BuffStatic[1]->SetUseToolTip( GetTxtFromMgr(7519));
			if(m_BuffStatic[2])		m_BuffStatic[2]->SetUseToolTip( GetTxtFromMgr(7520));
			if(m_BuffStatic[3])		m_BuffStatic[3]->SetUseToolTip( GetTxtFromMgr(7521));
			if(m_BuffStatic[4])		m_BuffStatic[4]->SetUseToolTip( GetTxtFromMgr(7522));
			if(m_BuffStatic[5])		m_BuffStatic[5]->SetUseToolTip( GetTxtFromMgr(7523));
			if(m_BuffStatic[6])		m_BuffStatic[6]->SetUseToolTip( GetTxtFromMgr(7524));
			if(m_BuffStatic[7])		m_BuffStatic[7]->SetUseToolTip( GetTxtFromMgr(7525));
			if(m_BuffStatic[8])		m_BuffStatic[8]->SetUseToolTip( GetTxtFromMgr(7526));
			if(m_BuffStatic[9])		m_BuffStatic[9]->SetUseToolTip( GetTxtFromMgr(7527));
			if(m_BuffStatic[10])	m_BuffStatic[10]->SetUseToolTip( GetTxtFromMgr(7528));
			if(m_BuffStatic[11])	m_BuffStatic[11]->SetUseToolTip( GetTxtFromMgr(7529));
			if(m_BuffStatic[12])	m_BuffStatic[12]->SetUseToolTip( GetTxtFromMgr(7530));
			if(m_BuffStatic[13])	m_BuffStatic[13]->SetUseToolTip( GetTxtFromMgr(7531));
			if(m_BuffStatic[14])	m_BuffStatic[14]->SetUseToolTip( GetTxtFromMgr(7563));//해피빈 이벤트 [2007.08.22 손성웅]
			if(m_BuffStatic[15])	m_BuffStatic[15]->SetUseToolTip( GetTxtFromMgr(7564));//전투력 증가약 30일간[2007.08.30 손성웅]
			if(m_BuffStatic[16])	m_BuffStatic[16]->SetUseToolTip( GetTxtFromMgr(7565));//방어력 증가약 30일간[2007.08.30 손성웅]
			if(m_BuffStatic[17])	m_BuffStatic[17]->SetUseToolTip( GetTxtFromMgr(7187));//열혈경험치증가약 [2007.09.18 손성웅]
			if(m_BuffStatic[18])	m_BuffStatic[18]->SetUseToolTip( GetTxtFromMgr(8676));//[진성] 추수 감사절에 쓰던 버프를 영웅동상에 쓰기위해 툴팁 변경. => 2008-8-4

			if(m_BuffStatic[19])	m_BuffStatic[19]->SetUseToolTip( GetTxtFromMgr(7736));//[추가 : 황진성 2007. 12. 14. 신년 이벤트. 버프 정보 보이기]
			if(m_BuffStatic[20])	m_BuffStatic[20]->SetUseToolTip( GetTxtFromMgr(7736));//[추가 : 황진성 2007. 12. 14. 신년 이벤트. 버프 정보 보이기]
			if(m_BuffStatic[21])	m_BuffStatic[21]->SetUseToolTip( GetTxtFromMgr(7939));//[추가 : 황진성 2007. 12. 14. 신년 이벤트. 버프 정보 보이기]
			if(m_BuffStatic[22])	m_BuffStatic[22]->SetUseToolTip( GetTxtFromMgr(7960));//농경 광산 음식			
			
			//TCHAR buffer[32] = "";
			//StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8473), 30);
			//if(m_BuffStatic[23])	m_BuffStatic[23]->SetUseToolTip(buffer);	//[진성] 스텟 적용 버프. 힘     + a=> 2008-6-19
			//StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8474), 30);
			//if(m_BuffStatic[24])	m_BuffStatic[24]->SetUseToolTip(buffer);	//[진성] 스텟 적용 버프. 순발력 + a=> 2008-6-19
			//StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8475), 30);
			//if(m_BuffStatic[25])	m_BuffStatic[25]->SetUseToolTip(buffer);	//[진성] 스텟 적용 버프. 마력   + a=> 2008-6-19
			//StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8476), 30);
			//if(m_BuffStatic[26])	m_BuffStatic[26]->SetUseToolTip(buffer);	//[진성] 스텟 적용 버프. 생명력 + a=> 2008-6-19
			//StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8477), 30);
			//if(m_BuffStatic[27])	m_BuffStatic[27]->SetUseToolTip(buffer);	//[진성] 스텟 적용 버프. 손재주 + a=> 2008-6-19
			//StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8541), 200);
			//if(m_BuffStatic[28])	m_BuffStatic[28]->SetUseToolTip(buffer);	//[진성] 경험치					+ a=> 2008-6-19
			
			if(m_BuffStatic[29])	m_BuffStatic[29]->SetUseToolTip( GetTxtFromMgr(8516));//[강형] 이관 보상 아이템
			if(m_BuffStatic[30])	m_BuffStatic[30]->SetUseToolTip( GetTxtFromMgr(9553));//[춘기] 제조 프리미엄 아이템
			if(m_BuffStatic[31])	m_BuffStatic[31]->SetUseToolTip( GetTxtFromMgr(8676));//[춘기] 신년이벤트 버프
			// 32 제조 프리미엄
			// 33 스승제자버프
			// 34 셋트아이템
			//if(m_BuffStatic[35])	m_BuffStatic[35]->SetUseToolTip( GetTxtFromMgr(2790));//[지연] 스페셜 엠플
			if(m_BuffStatic[36])	m_BuffStatic[36]->SetUseToolTip( GetTxtFromMgr(9937));//[영훈] 퀘스트 안티포션
			if(m_BuffStatic[37])	m_BuffStatic[37]->SetUseToolTip( GetTxtFromMgr(10334));//[춘기] 수박버프
			if(m_BuffStatic[38])	m_BuffStatic[38]->SetUseToolTip( GetTxtFromMgr(30048));//[성웅] 보물찾기 이벤트
			if(m_BuffStatic[39])	m_BuffStatic[39]->SetUseToolTip( GetTxtFromMgr(30048));//[춘기] 잃어버린 세계 퀘스트 - 공격강화제
		}
	}

	SetTopMostWindow(true);
	SetActionTimer(3000);
	SetTimerOnOff(true);
		        
}

void CALLBACK CNBuffstatus::StaticBuffProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNBuffstatus *pThis = (CNBuffstatus *)pControllerMgr;
	pThis->NBuffDlgProc(nEvent,nControlID,pControl);
	return;
}

void CNBuffstatus::NBuffDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
}

void CNBuffstatus ::Action()
{
	SI32 siXpos= 0, siYpos=0;
	TSpr* pSpr = pclClient->GetGlobalSpr(GIMG_BUFIMAGE);
	TSpr* pSpr2 = pclClient->GetGlobalSpr(GIMG_BUFPERCENT);
	SI32 getscreanRX = pclClient->GetScreenRX();
	SI32 getscreanRY = pclClient->GetScreenRY();
	siXpos = siClientScreenXsize - 300 - pSpr->GetXSize()+getscreanRX-(g_SoundOnOffDlg.m_bMaxWindowMode? 30:0);
	siYpos = MAP_TILE_YSIZE + getscreanRY +5;
	cltBufInfo cltbuf;
	cltCharCommon* chlChar = pclClient->pclCM->CR[1];
	cltClient * pclclient  = (cltClient*)pclClient;	
	SI16 i = 0;
	SI32 ylndet = 0, xIndet =0;

	// 툴팁용 버퍼
	TCHAR			szszToolTipBuffer[256]	= { '\0', };
	const size_t	sizeBufferSize			= sizeof( szszToolTipBuffer );
	///////////////////////////////////////////////////////////////////////////////

	//KHY - 0910 - 의리초코와 우정캔디 버프버그수정.
	if (m_BuffStatic[0]) 
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_ATTACKUP))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_ATTACKUP))
				return;

			SI32 buffpercent = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_ATTACKUP);
			if (buffpercent == 20) 
			{
				m_BuffStatic[0]->SetUseToolTip(GetTxtFromMgr(7518));
			}
			else
			{
				m_BuffStatic[0]->SetUseToolTip(GetTxtFromMgr(40806));
			}
			

			m_BuffStatic[0]->Show(true);
			m_BuffStatic[0]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<=-300&&xIndet<-320){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[0]->Show(false);
		}
	}
		
/////////////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[1]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_DEFENSEUP))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_DEFENSEUP))
				return;
			
			SI32 buffpercent = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_DEFENSEUP);
			if (buffpercent == 20) 
			{
				m_BuffStatic[1]->SetUseToolTip(GetTxtFromMgr(7519));
			}
			else
			{
				m_BuffStatic[1]->SetUseToolTip(GetTxtFromMgr(40807));
			}

			
			m_BuffStatic[1]->Show(true);
			m_BuffStatic[1]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<=-300&&xIndet<-320){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[1]->Show(false);
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	//무브스피드가 증가하는 버프가 걸리면 표시
	/////////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[2]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SPEEDUP)
			|| 	pclClient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_INCREASEMOVESPEED) )
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_SPEEDUP))
				return;
			
			m_BuffStatic[2]->Show(true);
			m_BuffStatic[2]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<=-300&&xIndet<-320){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[2]->Show(false);
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[3])
	{
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_MAKEEXPUP) ||
			 pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_NOTBONUSTIME_MAKEEXPUP) )
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_MAKEEXPUP))
				return;

			SI32 buffpercent = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_MAKEEXPUP);
			if (buffpercent == 20) 
			{
				m_BuffStatic[3]->SetUseToolTip(GetTxtFromMgr(7574));
			}
			else
			{
				m_BuffStatic[3]->SetUseToolTip(GetTxtFromMgr(7521));
			}

			m_BuffStatic[3]->Show(true);
			m_BuffStatic[3]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[3]->Show(false);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[4]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_NOFIRSTATTACK))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_NOFIRSTATTACK))
				return;

			m_BuffStatic[4]->Show(true);
			m_BuffStatic[4]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[4]->Show(false);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[5]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUMMONATACUP))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_SUMMONATACUP))
				return;

			m_BuffStatic[5]->Show(true);
			m_BuffStatic[5]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[5]->Show(false);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[6]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_LOVEPOWER))
				return;
			
			m_BuffStatic[6]->Show(true);
			m_BuffStatic[6]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[6]->Show(false);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[7]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FRIENDSHIP)) 
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_FRIENDSHIP))
				return;

			m_BuffStatic[7]->Show(true);
			m_BuffStatic[7]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[7]->Show(false);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[8])
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HINAMOCHI)) 
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_HINAMOCHI))
				return;

			m_BuffStatic[8]->Show(true);
			m_BuffStatic[8]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
					m_BuffStatic[8]->Show(false);
		}
	}
    //////////////////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[9]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUCCESS_MANUFACTURE))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_SUCCESS_MANUFACTURE))
				return;

			m_BuffStatic[9]->Show(true);
			m_BuffStatic[9]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[9]->Show(false);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	if (m_BuffStatic[10]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_MOFUMOFU_PERFUME))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_MOFUMOFU_PERFUME))
				return;
			
			m_BuffStatic[10]->Show(true);
			m_BuffStatic[10]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340) {
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40) {
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[10]->Show(false);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////****
	/////////////////////////////////////////////////////////////////////////////////////
	//네잎클로버 사용시 표기
	if (m_BuffStatic[11]){
		if (pclClient->pclCM->CR[1]->bFourLeafSwitch)
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_FOURLEAF))
				return;
						
			m_BuffStatic[11]->Show(true);
			m_BuffStatic[11]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340) {
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[11]->Show(false);
		}
	}
	/////////////////////////////////////////////////
	if (m_BuffStatic[12]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_PREMIUMEXPUP))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_PREMIUMEXPUP))
				return;

			m_BuffStatic[12]->Show(true);
			m_BuffStatic[12]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[12]->Show(false);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	if(m_BuffStatic[13]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp){

			m_BuffStatic[13]->Show(true);
			m_BuffStatic[13]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if (xIndet<40) {
				xIndet -= m_siImageXsize;
			}
		}

		else{
			m_BuffStatic[13]->Show(false);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	if (m_BuffStatic[14]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HAPPYBEAN)){
			m_BuffStatic[14]->Show(true);
			m_BuffStatic[14]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[14]->Show(false);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	//전투력 증가약 30일간[2007.08.30 손성웅]
	if (m_BuffStatic[15]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_ATTACKUP30))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_ATTACKUP30))
				return;
			
			m_BuffStatic[15]->Show(true);
			m_BuffStatic[15]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[15]->Show(false);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	////방어력 증가약 30일간[2007.08.30 손성웅]
	if (m_BuffStatic[16]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_DEFENSEUP30))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_DEFENSEUP30))
				return;
			
			m_BuffStatic[16]->Show(true);
			m_BuffStatic[16]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[16]->Show(false);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	
	if (m_BuffStatic[17]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUPEREXPUP))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_SUPEREXPUP))
				return;
			
			m_BuffStatic[17]->Show(true);
			m_BuffStatic[17]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{
			m_BuffStatic[17]->Show(false);
		}
	}

	//[추가 : 황진성 2007. 11. 12 추수 감사절 이벤트. 버프]
	//[진성] 영웅 동상이 있는 마을 버프 => 2008-7-22
	if (m_BuffStatic[18])
	{
		bool HeroBronze = false;
		cltCharClient* pclcharclient = (cltCharClient*)pclClient->pclCM->CR[1];
		if( pclcharclient && IsPC( pclClient->pclCM->CR[1]->GetID() ) == 1)
		{
			for(SI32 Count = 0; Count < MAX_HIRED_EVENTNPC_NUM; ++Count)
			{
				if( pclcharclient->siEventNPCKind[Count] == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE")))
				{
					HeroBronze = true;
				}
			}
		}
				
		if (   pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_THANKSGIVING_DAY) 
			|| (true == HeroBronze) )
		{
			m_BuffStatic[18]->Show(true);
			m_BuffStatic[18]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[18]->Show(false);
		}
	}
	
	//[추가 : 황진성 2007. 12. 14 신년 이벤트. 버프 이미지 그리기.]
	cltCharClient* pclcharclient = (cltCharClient*)pclclient->pclCM->CR[1];
	if (m_BuffStatic[19])
	{
		for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++){
			if(pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH")))
			{
				m_BuffStatic[19]->Show(true);
				m_BuffStatic[19]->MoveControl(xIndet,ylndet);
				m_BuffShow = true;
				if (xIndet<= -300 && xIndet< -340)
				{
					ylndet += m_siImageYSize;
					xIndet -= m_siImageXsize+300;
				}
				if(xIndet<40)
				{
					xIndet -= m_siImageXsize;
				}
			}
			else
			{
				m_BuffStatic[19]->Show(false);
			}
		}
		
	}
	
	//[추가 : 황진성 2007. 12. 14 신년 이벤트. 버프 이미지 그리기.]
	if (m_BuffStatic[20])
	{
		//for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++){
		//	if(pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")))
		//	{
		//		m_BuffStatic[20]->Show(true);
		//		m_BuffStatic[20]->MoveControl(xIndet,ylndet);
		//		m_BuffShow = true;
		//		if (xIndet<= -300 && xIndet< -340)
		//		{
		//			ylndet += m_siImageYSize;
		//			xIndet -= m_siImageXsize+300;
		//		}
		//		if(xIndet<40)
		//		{
		//			xIndet -= m_siImageXsize;
		//		}
		//	}
		//	else
		//	{
		//		m_BuffStatic[20]->Show(false);
		//	}
		//}
		
	}
	//[추가 : 황진성 2008. 2. 18 히나마쓰리. 버프 이미지 그리기.]
	if (m_BuffStatic[21])
	{
		m_BuffShow = false;
		for (SI32 i = 0; i < MAX_HIRED_EVENTNPC_NUM; i++)
		{
			if(pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL")))
			{
				m_BuffStatic[21]->Show(true);
				m_BuffStatic[21]->MoveControl(xIndet,ylndet);
				m_BuffShow = true;
				if (xIndet<= -300 && xIndet< -340)
				{
					ylndet += m_siImageYSize;
					xIndet -= m_siImageXsize+300;
				}
				if(xIndet<40)
				{
					xIndet -= m_siImageXsize;
				}
			}
			else
			{
				m_BuffStatic[21]->Show(false);
			}
			if(true == m_BuffShow)
				break;
		}
	}
	////채광 농경용 음식
	if (m_BuffStatic[22]){
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FARMINGNMININGFOREVER))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_FARMINGNMININGFOREVER))
				return;

			m_BuffStatic[22]->Show(true);
			m_BuffStatic[22]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else{		
			m_BuffStatic[22]->Show(false);
		}
	}	


	bool bBufOlympic = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_OLYMPICMEDAL) ;

	cltBasicAbility clValentineAbility;
	clValentineAbility.Init();

	if( NULL != pclClient->m_pclValentineEventMgr)
	{
		if(pclClient->m_pclValentineEventMgr->IsBufTime())
			pclClient->m_pclValentineEventMgr->GetValentineBuf(pclClient->pclCM->CR[1],clValentineAbility);
	}

	if (m_BuffStatic[23])
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_STR) || bBufOlympic == true || clValentineAbility.GetStr() > 0) 
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_STATUE_UP_STR))
				return;

			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(8473), pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_STR) + clValentineAbility.GetStr());
			if(m_BuffStatic[23])	m_BuffStatic[23]->SetUseToolTip(szszToolTipBuffer);	

			m_BuffStatic[23]->Show(true);
			m_BuffStatic[23]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[23]->Show(false);
		}
	}
	
	if (m_BuffStatic[24])
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_DEX) || bBufOlympic == true ||  clValentineAbility.GetDex() > 0)
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_STATUE_UP_DEX))
				return;

			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(8474), pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_DEX) + clValentineAbility.GetDex());
			if(m_BuffStatic[24])	m_BuffStatic[24]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[24]->Show(true);
			m_BuffStatic[24]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[24]->Show(false);
		}
	}

	if(m_BuffStatic[25])
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_MAG) || bBufOlympic == true || clValentineAbility.GetMag() > 0)
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_STATUE_UP_MAG))
				return;

			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(8475), pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_MAG)+ clValentineAbility.GetMag());
			if(m_BuffStatic[25])	m_BuffStatic[25]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[25]->Show(true);
			m_BuffStatic[25]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[25]->Show(false);
		}
	}
	
	if(m_BuffStatic[26])
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_VIT) || bBufOlympic == true || clValentineAbility.GetVit() > 0)
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_STATUE_UP_VIT))
				return;

			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(8476), pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_VIT)+ clValentineAbility.GetVit());
			if(m_BuffStatic[26])	m_BuffStatic[26]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[26]->Show(true);
			m_BuffStatic[26]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[26]->Show(false);
		}
	}

	if(m_BuffStatic[27])
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_HAND) || bBufOlympic == true || clValentineAbility.GetHnd() > 0)
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_STATUE_UP_HAND))
				return;

			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(8477), pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_HAND)+ clValentineAbility.GetHnd());
			if(m_BuffStatic[27])	m_BuffStatic[27]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[27]->Show(true);
			m_BuffStatic[27]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[27]->Show(false);
		}
	}

	if(m_BuffStatic[28])//28
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_EXP_UP))
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(true == pclclient->IsBuffRouletteAction(BUF_EXP_UP))
				return;
			
			SI32 siBufPercent = (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_EXP_UP) + 100);

			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf( szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(8541), siBufPercent );
			if(m_BuffStatic[28])	m_BuffStatic[28]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[28]->Show(true);
			m_BuffStatic[28]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[28]->Show(false);
		}
	}

	if(m_BuffStatic[29])//29
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_REWARDBONUS))
		{
			m_BuffStatic[29]->Show(true);
			m_BuffStatic[29]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[29]->Show(false);
		}
	}
	
	if(m_BuffStatic[30])//30
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_EVENT_EGGMOONCAKE))
		{
			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(9573), pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_EVENT_EGGMOONCAKE));
			if(m_BuffStatic[30])	m_BuffStatic[30]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[30]->Show(true);
			m_BuffStatic[30]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[30]->Show(false);
		}
	}

	// 춘기 - 신년이벤트 버프이미지
	if(m_BuffStatic[31])//30
	{
		for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++)
		{
			if(pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")))
			{
				m_BuffStatic[31]->Show(true);
				m_BuffStatic[31]->MoveControl(xIndet,ylndet);
				m_BuffShow = true;
				if (xIndet<= -300 && xIndet< -340)
				{
					ylndet += m_siImageYSize;
					xIndet -= m_siImageXsize+300;
				}
				if(xIndet<40)
				{
					xIndet -= m_siImageXsize;
				}
				break;
			}
			else
			{
				m_BuffStatic[31]->Show(false);
			}
			
		}
		
	}

	// PCK : 제조 프리미엄 아이템
	if(m_BuffStatic[32])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_PREMIUMMAKEEXPUP))
		{
			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(9553), pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_PREMIUMMAKEEXPUP));
			if(m_BuffStatic[32])	m_BuffStatic[32]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[32]->Show(true);
			m_BuffStatic[32]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[32]->Show(false);
		}
	}

	if(m_BuffStatic[33])//33
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_MENTOR))
		{
			//TCHAR buffer[32] = "";
			//StringCchPrintf(buffer, sizeof(buffer), "MENTOR BUF");
			if(m_BuffStatic[33])	m_BuffStatic[33]->SetUseToolTip( GetTxtFromMgr( 9564 ) );

			m_BuffStatic[33]->Show(true);
			m_BuffStatic[33]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[33]->Show(false);
		}
	}

	//[진성] 세트 아이템 버프 보이기.
	if(m_BuffStatic[34]) //34
	{
		if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
		{
			if( pclClient->pclCM->CR[1]->m_siSetItemType )
			{
				TCHAR* pszMakeToolTip = NULL;

				pszMakeToolTip = MakeSetItemToolTip( pclClient->pclCM->CR[1] );

				if(m_BuffStatic[34])	m_BuffStatic[34]->SetUseToolTip( pszMakeToolTip );

				//	[종호] m_siSetItemType이 있다고 해도 Step불충분으로 받는 버프가 없을수 있습니다.
				//	[종호] 없는경우는 아예 세트버프아이콘을 비활성화 시킵니다.
				if(strlen(pszMakeToolTip) > 0)
					m_BuffStatic[34]->Show(true);
				else
					m_BuffStatic[34]->Show(false);

				m_BuffStatic[34]->MoveControl(xIndet,ylndet);
				m_BuffShow = true;
				if (xIndet<= -300 && xIndet< -340)
				{
					ylndet += m_siImageYSize;
					xIndet -= m_siImageXsize+300;
				}
				if(xIndet<40)
				{
					xIndet -= m_siImageXsize;
				}
			}
			else
			{
				m_BuffStatic[34]->Show(false);
			}
		}
	}

	// [지연] : 스페셜 엠플.
	if(m_BuffStatic[35])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SPECIALAMPUL_EXPUP300))
		{
			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(9889), pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_SPECIALAMPUL_EXPUP300));
			if(m_BuffStatic[35])	m_BuffStatic[35]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[35]->Show(true);
			m_BuffStatic[35]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[35]->Show(false);
		}
	}

	// [영훈] 퀘스트 안티 포션
	if(m_BuffStatic[36])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_QUEST_ANTIPOTION))
		{
			m_BuffStatic[36]->Show(true);
			m_BuffStatic[36]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[36]->Show(false);
		}
	}
	// [춘기] 수박버프
	if(m_BuffStatic[37])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_WATERMELON))
		{
			m_BuffStatic[37]->Show(true);
			m_BuffStatic[37]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[37]->Show(false);
		}
	}
	// [성웅] 보물찾기 이벤트
	if(m_BuffStatic[38])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_EVENT_FINDTREASURE))
		{
			m_BuffStatic[38]->Show(true);
			m_BuffStatic[38]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[38]->Show(false);
		}
	}
	// [춘기] 공격방어제
	if(m_BuffStatic[39])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_QUEST_ATTACK))
		{
			m_BuffStatic[39]->Show(true);
			m_BuffStatic[39]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[39]->Show(false);
		}
	}

	// [지연] : 새로 추가된 지혜(WIS) 증가 버프
	if(m_BuffStatic[41])
	{
		if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_WIS) || bBufOlympic == true )
		{
			//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			//if(true == pclclient->IsBuffRouletteAction(BUF_STATUE_UP_MAG))
			//	return;

			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(40281), pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_WIS));
			if(m_BuffStatic[41])	m_BuffStatic[41]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[41]->Show(true);
			m_BuffStatic[41]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340){
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}if(xIndet<40){
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[41]->Show(false);
		}
	}

	// [지연] : 2010신년이벤트- '길' 버프 이미지 (길 버프는 실제 버프는 아니고 클라쪽에서 버프칸에 이미지만 출력) 
	if ( m_BuffStatic[42] )
	{
		SI16 index = -1;
		index = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.FindBufGetTypeSlot(BUF_GET_TYPE_FORTUNECOOKIE);
		if ( -1 < index )
		{
			m_BuffStatic[42]->Show(true);
			m_BuffStatic[42]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[42]->Show(false);
		}
	}
	
	// '흉' 버프 이미지 (흉 버프는 실제 버프)
	if ( true == pclClient->IsCountrySwitch(Switch_NewYearEvent_2010) )
	{
		if(m_BuffStatic[43])
		{
			if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_EVENT_FORTUNE_BAD))
			{
				m_BuffStatic[43]->Show(true);
				m_BuffStatic[43]->MoveControl(xIndet,ylndet);
				m_BuffShow = true;
				if (xIndet<= -300 && xIndet< -340)
				{
					ylndet += m_siImageYSize;
					xIndet -= m_siImageXsize+300;
				}
				if(xIndet<40)
				{
					xIndet -= m_siImageXsize;
				}
			}
			else
			{
				m_BuffStatic[43]->Show(false);
			}
		}
	}

	// [춘기] 눈의 축복, 크리스마스 장식 (임시)
	if(m_BuffStatic[44])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_CHRISTMASEVENT_SNOWBLESS))
		{
			m_BuffStatic[44]->Show(true);
			m_BuffStatic[44]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
			m_BuffStatic[44]->SetUseToolTip(GetTxtFromMgr(20090));
		}
		else
		{
			m_BuffStatic[44]->Show(false);
		}
	}
	//[진성] 자이언트 라쿤 이벤트 버프.
	if(m_BuffStatic[45])
	{
		if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT))
		{
			m_BuffStatic[45]->Show(true);
			m_BuffStatic[45]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
			
			TCHAR szToolTip[256]	 = TEXT("");
			TCHAR szToolTipTemp[256] = TEXT("");
			stBuf* pstBuf = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf( BUF_GIANTRACCOON_RESURRECT_EVENT );
			if( 1 <= pstBuf->siPercent )	
			{
				StringCchPrintf(szToolTipTemp, sizeof(szToolTipTemp), "%s 5%% \r\n", GetTxtFromMgr(8234) );
				StringCchCat( szToolTip, sizeof(szToolTip), szToolTipTemp );
			}
			if( 2 <= pstBuf->siPercent )	
			{
				StringCchPrintf(szToolTipTemp, sizeof(szToolTipTemp), "%s 5%% \r\n", GetTxtFromMgr(8236) );
				StringCchCat( szToolTip, sizeof(szToolTip), szToolTipTemp );
			}
			if( 3 <= pstBuf->siPercent )	
			{
				StringCchPrintf(szToolTipTemp, sizeof(szToolTipTemp), "%s 10%% \r\n", GetTxtFromMgr(4393) );
				StringCchCat( szToolTip, sizeof(szToolTip), szToolTipTemp );
			}
			if( 4 <= pstBuf->siPercent )	
			{
				StringCchPrintf(szToolTipTemp, sizeof(szToolTipTemp), "%s 5%% \r\n", GetTxtFromMgr(8235) );
				StringCchCat( szToolTip, sizeof(szToolTip), szToolTipTemp );
			}
			if( 5 <= pstBuf->siPercent )	
			{
				StringCchPrintf(szToolTipTemp, sizeof(szToolTipTemp), "%s 5%% \r\n", GetTxtFromMgr(8237) );
				StringCchCat( szToolTip, sizeof(szToolTip), szToolTipTemp );
			}
			
			m_BuffStatic[45]->SetUseToolTip( szToolTip );
		}
		else
		{
			m_BuffStatic[45]->Show(false);
		}
	}
	if(m_BuffStatic[46])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_GIANTRACCOON_RESURRECT_EVENT_FIRST_VILLAGE ))
		{
			m_BuffStatic[46]->Show(true);
			m_BuffStatic[46]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
			m_BuffStatic[46]->SetUseToolTip(GetTxtFromMgr(40816));
		}
		else
		{
			m_BuffStatic[46]->Show(false);
		}
	}

	// [지연] 마을 강화제.
	if(m_BuffStatic[47])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_RESIDENTS_STRONG))
		{
			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(39998)/*, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_RESIDENTS_STRONG)*/);
			if(m_BuffStatic[47])	m_BuffStatic[47]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[47]->Show(true);
			m_BuffStatic[47]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[47]->Show(false);
		}
	}

	// [지연] 마을 EXP 캡슐 버프.
	if(m_BuffStatic[48])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_RESIDENTS_EXPUP_10))
		{
			ZeroMemory( szszToolTipBuffer, sizeBufferSize );
			StringCchPrintf(szszToolTipBuffer, sizeBufferSize, GetTxtFromMgr(39997)/*, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_RESIDENTS_EXPUP_10)*/);
			if(m_BuffStatic[48])	m_BuffStatic[48]->SetUseToolTip(szszToolTipBuffer);

			m_BuffStatic[48]->Show(true);
			m_BuffStatic[48]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[48]->Show(false);
		}
	}

	if(m_BuffStatic[49])
	{
		if(pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_VALENTINEDAY))
		{
			if(m_BuffStatic[49])	
				m_BuffStatic[49]->SetUseToolTip(GetTxtFromMgr(40869));

			m_BuffStatic[49]->Show(true);
			m_BuffStatic[49]->MoveControl(xIndet,ylndet);
			m_BuffShow = true;
			if (xIndet<= -300 && xIndet< -340)
			{
				ylndet += m_siImageYSize;
				xIndet -= m_siImageXsize+300;
			}
			if(xIndet<40)
			{
				xIndet -= m_siImageXsize;
			}
		}
		else
		{
			m_BuffStatic[49]->Show(false);
		}
	}

	SetBuffTooltip();// 일단 숫자나 나오게 하고..'';
}
bool CNBuffstatus::IsBuffShow()
{
	return m_BuffShow;
}

void CNBuffstatus::SetBuffTooltip()
{
	// 일단은 나오게만 하고..툴팁을 실시간으로 갱신 하는 방법을 궁리 해야 겠다.
	if(pclClient->IsCountrySwitch(Switch_ShowBuffToolTip))
	{
		SI32 siBufPercent				 = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_OLYMPICMEDAL  );
		SI32 siBufPercentServerEventStr  = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_STR );
		SI32 siBufPercentServerEventDex  = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_DEX );
		SI32 siBufPercentServerEventMag  = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_MAG );
		SI32 siBufPercentServerEventWis  = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_WIS );
		SI32 siBufPercentServerEventVit  = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_VIT );
		SI32 siBufPercentServerEventHand = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_HAND);
		if ( true == pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_OLYMPICMEDAL)) 
		{
			TCHAR buffer[256] = "";
			StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8473), siBufPercent+siBufPercentServerEventStr);
			if(m_BuffStatic[23])	m_BuffStatic[23]->SetUseToolTip(buffer);	
			StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8474), siBufPercent+siBufPercentServerEventDex);
			if(m_BuffStatic[24])	m_BuffStatic[24]->SetUseToolTip(buffer);	
			StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8475), siBufPercent+siBufPercentServerEventMag);
			if(m_BuffStatic[25])	m_BuffStatic[25]->SetUseToolTip(buffer);	
			StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8476), siBufPercent+siBufPercentServerEventVit);
			if(m_BuffStatic[26])	m_BuffStatic[26]->SetUseToolTip(buffer);	
			StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(8477), siBufPercent+siBufPercentServerEventVit);
			if(m_BuffStatic[27])	m_BuffStatic[27]->SetUseToolTip(buffer);	
			StringCchPrintf(buffer, sizeof(buffer), GetTxtFromMgr(40281), siBufPercent+siBufPercentServerEventWis);
			if(m_BuffStatic[41])	m_BuffStatic[41]->SetUseToolTip(buffer);
		}
	}
}

TCHAR* CNBuffstatus::MakeAddText( SI32 TextNum, SI32 Value )
{
	static TCHAR szAddText[32] = "";
	ZeroMemory( szAddText, sizeof(szAddText) );

	NTCHARString32 kbuffer( GetTxtFromMgr(TextNum) );

	kbuffer.Replace( "#N#", SI32ToString(Value) );
	
	memcpy(szAddText, kbuffer, sizeof(szAddText));

	return szAddText;
}

TCHAR* CNBuffstatus::MakeSetItemToolTip( cltCharCommon* _pclChar )
{
	cltSetItemAbility* pcltSetItemAbility = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityData( _pclChar, USE_SETITEM_1 );
	if( NULL == pcltSetItemAbility ) return NULL;

	cltSetItemAbility* pcltSetItemAbility2 = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityData( _pclChar, USE_SETITEM_2 );
	if( NULL == pcltSetItemAbility2 ) return NULL;

	cltSetItemAbility* pcltSetItemAbility3 = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityData( _pclChar, USE_SETITEM_3 );
	if( NULL == pcltSetItemAbility3 ) return NULL;

	cltSetItemAbility* pcltSetItemAbility4 = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityData( _pclChar, USE_SETITEM_4 );
	if( NULL == pcltSetItemAbility4 ) return NULL;

	cltSetItemAbility* pcltSetItemAbility5 = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityData( _pclChar, USE_SETITEM_5 );
	if( NULL == pcltSetItemAbility5 ) return NULL;

	static TCHAR szToolTipBuffer[512] = "";
	ZeroMemory( szToolTipBuffer, sizeof(szToolTipBuffer) );

	(0 < pcltSetItemAbility->m_siAttack					) || (0 < pcltSetItemAbility2->m_siAttack					)  || (0 < pcltSetItemAbility3->m_siAttack						)|| (0 < pcltSetItemAbility4->m_siAttack					)  || (0 < pcltSetItemAbility5->m_siAttack						)? _tcscat(szToolTipBuffer, MakeAddText(9634, pcltSetItemAbility->m_siAttack					+ pcltSetItemAbility2->m_siAttack					 + pcltSetItemAbility3->m_siAttack						+ pcltSetItemAbility4->m_siAttack					+ pcltSetItemAbility5->m_siAttack							)) : 0;					
	(0 < pcltSetItemAbility->m_siDefensive				) || (0 < pcltSetItemAbility2->m_siDefensive				)  || (0 < pcltSetItemAbility3->m_siDefensive					)|| (0 < pcltSetItemAbility4->m_siDefensive					)  || (0 < pcltSetItemAbility5->m_siDefensive					)? _tcscat(szToolTipBuffer, MakeAddText(9635, pcltSetItemAbility->m_siDefensive					+ pcltSetItemAbility2->m_siDefensive				 + pcltSetItemAbility3->m_siDefensive					+ pcltSetItemAbility4->m_siDefensive				+ pcltSetItemAbility5->m_siDefensive						)) : 0;					
	(0 < pcltSetItemAbility->m_siStr					) || (0 < pcltSetItemAbility2->m_siStr						)  || (0 < pcltSetItemAbility3->m_siStr							)|| (0 < pcltSetItemAbility4->m_siStr						)  || (0 < pcltSetItemAbility5->m_siStr							)? _tcscat(szToolTipBuffer, MakeAddText(9636, pcltSetItemAbility->m_siStr						+ pcltSetItemAbility2->m_siStr						 + pcltSetItemAbility3->m_siStr							+ pcltSetItemAbility4->m_siStr						+ pcltSetItemAbility5->m_siStr								)) : 0;					
	(0 < pcltSetItemAbility->m_siDex					) || (0 < pcltSetItemAbility2->m_siDex						)  || (0 < pcltSetItemAbility3->m_siDex							)|| (0 < pcltSetItemAbility4->m_siDex						)  || (0 < pcltSetItemAbility5->m_siDex							)? _tcscat(szToolTipBuffer, MakeAddText(9637, pcltSetItemAbility->m_siDex						+ pcltSetItemAbility2->m_siDex						 + pcltSetItemAbility3->m_siDex							+ pcltSetItemAbility4->m_siDex						+ pcltSetItemAbility5->m_siDex								)) : 0;				
	(0 < pcltSetItemAbility->m_siVit 					) || (0 < pcltSetItemAbility2->m_siVit 						)  || (0 < pcltSetItemAbility3->m_siVit 						)|| (0 < pcltSetItemAbility4->m_siVit 						)  || (0 < pcltSetItemAbility5->m_siVit 						)? _tcscat(szToolTipBuffer, MakeAddText(9638, pcltSetItemAbility->m_siVit 						+ pcltSetItemAbility2->m_siVit 						 + pcltSetItemAbility3->m_siVit 						+ pcltSetItemAbility4->m_siVit 						+ pcltSetItemAbility5->m_siVit 								)) : 0;					
	(0 < pcltSetItemAbility->m_siMag 					) || (0 < pcltSetItemAbility2->m_siMag 						)  || (0 < pcltSetItemAbility3->m_siMag 						)|| (0 < pcltSetItemAbility4->m_siMag 						)  || (0 < pcltSetItemAbility5->m_siMag 						)? _tcscat(szToolTipBuffer, MakeAddText(9639, pcltSetItemAbility->m_siMag 						+ pcltSetItemAbility2->m_siMag 						 + pcltSetItemAbility3->m_siMag 						+ pcltSetItemAbility4->m_siMag 						+ pcltSetItemAbility5->m_siMag 								)) : 0;				
	(0 < pcltSetItemAbility->m_siHnd 					) || (0 < pcltSetItemAbility2->m_siHnd 						)  || (0 < pcltSetItemAbility3->m_siHnd 						)|| (0 < pcltSetItemAbility4->m_siHnd 						)  || (0 < pcltSetItemAbility5->m_siHnd 						)? _tcscat(szToolTipBuffer, MakeAddText(9640, pcltSetItemAbility->m_siHnd 						+ pcltSetItemAbility2->m_siHnd 						 + pcltSetItemAbility3->m_siHnd 						+ pcltSetItemAbility4->m_siHnd 						+ pcltSetItemAbility5->m_siHnd 								)) : 0;				
	(0 < pcltSetItemAbility->m_siLuk 					) || (0 < pcltSetItemAbility2->m_siLuk 						)  || (0 < pcltSetItemAbility3->m_siLuk 						)|| (0 < pcltSetItemAbility4->m_siLuk 						)  || (0 < pcltSetItemAbility5->m_siLuk 						)? _tcscat(szToolTipBuffer, MakeAddText(9641, pcltSetItemAbility->m_siLuk 						+ pcltSetItemAbility2->m_siLuk 						 + pcltSetItemAbility3->m_siLuk 						+ pcltSetItemAbility4->m_siLuk 						+ pcltSetItemAbility5->m_siLuk 								)) : 0;				
	(0 < pcltSetItemAbility->m_siWis 					) || (0 < pcltSetItemAbility2->m_siWis 						)  || (0 < pcltSetItemAbility3->m_siWis 						)|| (0 < pcltSetItemAbility4->m_siWis 						)  || (0 < pcltSetItemAbility5->m_siWis 						)? _tcscat(szToolTipBuffer, MakeAddText(9642, pcltSetItemAbility->m_siWis 						+ pcltSetItemAbility2->m_siWis 						 + pcltSetItemAbility3->m_siWis 						+ pcltSetItemAbility4->m_siWis 						+ pcltSetItemAbility5->m_siWis 								)) : 0;				
	(0 < pcltSetItemAbility->m_siFireEnchantDefensive	) || (0 < pcltSetItemAbility2->m_siFireEnchantDefensive		)  || (0 < pcltSetItemAbility3->m_siFireEnchantDefensive		)|| (0 < pcltSetItemAbility4->m_siFireEnchantDefensive		)  || (0 < pcltSetItemAbility5->m_siFireEnchantDefensive		)? _tcscat(szToolTipBuffer, MakeAddText(9643, pcltSetItemAbility->m_siFireEnchantDefensive		+ pcltSetItemAbility2->m_siFireEnchantDefensive		 + pcltSetItemAbility3->m_siFireEnchantDefensive		+ pcltSetItemAbility4->m_siFireEnchantDefensive		+ pcltSetItemAbility5->m_siFireEnchantDefensive				)) : 0;
	(0 < pcltSetItemAbility->m_siWaterEnchantDefensive	) || (0 < pcltSetItemAbility2->m_siWaterEnchantDefensive	)  || (0 < pcltSetItemAbility3->m_siWaterEnchantDefensive		)|| (0 < pcltSetItemAbility4->m_siWaterEnchantDefensive		)  || (0 < pcltSetItemAbility5->m_siWaterEnchantDefensive		)? _tcscat(szToolTipBuffer, MakeAddText(9644, pcltSetItemAbility->m_siWaterEnchantDefensive		+ pcltSetItemAbility2->m_siWaterEnchantDefensive	 + pcltSetItemAbility3->m_siWaterEnchantDefensive		+ pcltSetItemAbility4->m_siWaterEnchantDefensive	+ pcltSetItemAbility5->m_siWaterEnchantDefensive			)) : 0;
	(0 < pcltSetItemAbility->m_siWindEnchantDefensive	) || (0 < pcltSetItemAbility2->m_siWindEnchantDefensive		)  || (0 < pcltSetItemAbility3->m_siWindEnchantDefensive		)|| (0 < pcltSetItemAbility4->m_siWindEnchantDefensive		)  || (0 < pcltSetItemAbility5->m_siWindEnchantDefensive		)? _tcscat(szToolTipBuffer, MakeAddText(9645, pcltSetItemAbility->m_siWindEnchantDefensive		+ pcltSetItemAbility2->m_siWindEnchantDefensive		 + pcltSetItemAbility3->m_siWindEnchantDefensive		+ pcltSetItemAbility4->m_siWindEnchantDefensive		+ pcltSetItemAbility5->m_siWindEnchantDefensive				)) : 0;
	(0 < pcltSetItemAbility->m_siLandEnchantDefensive	) || (0 < pcltSetItemAbility2->m_siLandEnchantDefensive		)  || (0 < pcltSetItemAbility3->m_siLandEnchantDefensive		)|| (0 < pcltSetItemAbility4->m_siLandEnchantDefensive		)  || (0 < pcltSetItemAbility5->m_siLandEnchantDefensive		)? _tcscat(szToolTipBuffer, MakeAddText(9646, pcltSetItemAbility->m_siLandEnchantDefensive		+ pcltSetItemAbility2->m_siLandEnchantDefensive		 + pcltSetItemAbility3->m_siLandEnchantDefensive		+ pcltSetItemAbility4->m_siLandEnchantDefensive		+ pcltSetItemAbility5->m_siLandEnchantDefensive				)) : 0;
	//	(0 < pcltSetItemAbility->m_siDrainSuccessPercent	) || (0 < pcltSetItemAbility2->m_siDrainSuccessPercent	) 	) || (0 < pcltSetItemAbility3->m_siDrainSuccessPercent		)	) || (0 < pcltSetItemAbility4->m_siDrainSuccessPercent	) 	) || (0 < pcltSetItemAbility5->m_siDrainSuccessPercent		)? _tcscat(szToolTipBuffer, MakeAddText(9648, pcltSetItemAbility->m_siDrainSuccessPercent		+ pcltSetItemAbility2->m_siDrainSuccessPercent		 + pcltSetItemAbility3->m_siDrainSuccessPercent			+ pcltSetItemAbility4->m_siDrainSuccessPercent		+ pcltSetItemAbility5->m_siDrainSuccessPercent				)) : 0;
	(0 < pcltSetItemAbility->m_siDrain					) || (0 < pcltSetItemAbility2->m_siDrain					)  || (0 < pcltSetItemAbility3->m_siDrain						)|| (0 < pcltSetItemAbility4->m_siDrain						)  || (0 < pcltSetItemAbility5->m_siDrain						)? _tcscat(szToolTipBuffer, MakeAddText(9649, pcltSetItemAbility->m_siDrain						+ pcltSetItemAbility2->m_siDrain					 + pcltSetItemAbility3->m_siDrain						+ pcltSetItemAbility4->m_siDrain					+ pcltSetItemAbility5->m_siDrain							)) : 0;	
	(0 < pcltSetItemAbility->m_siHitRate				) || (0 < pcltSetItemAbility2->m_siHitRate					)  || (0 < pcltSetItemAbility3->m_siHitRate						)|| (0 < pcltSetItemAbility4->m_siHitRate					)  || (0 < pcltSetItemAbility5->m_siHitRate						)? _tcscat(szToolTipBuffer, MakeAddText(9650, pcltSetItemAbility->m_siHitRate					+ pcltSetItemAbility2->m_siHitRate					 + pcltSetItemAbility3->m_siHitRate						+ pcltSetItemAbility4->m_siHitRate					+ pcltSetItemAbility5->m_siHitRate							)) : 0;				
	(0 < pcltSetItemAbility->m_siWeaponUseSkill			) || (0 < pcltSetItemAbility2->m_siWeaponUseSkill			)  || (0 < pcltSetItemAbility3->m_siWeaponUseSkill				)|| (0 < pcltSetItemAbility4->m_siWeaponUseSkill			)  || (0 < pcltSetItemAbility5->m_siWeaponUseSkill				)? _tcscat(szToolTipBuffer, MakeAddText(9651, pcltSetItemAbility->m_siWeaponUseSkill			+ pcltSetItemAbility2->m_siWeaponUseSkill			 + pcltSetItemAbility3->m_siWeaponUseSkill				+ pcltSetItemAbility4->m_siWeaponUseSkill			+ pcltSetItemAbility5->m_siWeaponUseSkill					)) : 0;		
	(0 < pcltSetItemAbility->m_siHP 					) || (0 < pcltSetItemAbility2->m_siHP 						)  || (0 < pcltSetItemAbility3->m_siHP 							)|| (0 < pcltSetItemAbility4->m_siHP 						)  || (0 < pcltSetItemAbility5->m_siHP 							)? _tcscat(szToolTipBuffer, MakeAddText(9652, pcltSetItemAbility->m_siHP 						+ pcltSetItemAbility2->m_siHP 						 + pcltSetItemAbility3->m_siHP 							+ pcltSetItemAbility4->m_siHP 						+ pcltSetItemAbility5->m_siHP 								)) : 0;					
	(0 < pcltSetItemAbility->m_siMP 					) || (0 < pcltSetItemAbility2->m_siMP 						)  || (0 < pcltSetItemAbility3->m_siMP 							)|| (0 < pcltSetItemAbility4->m_siMP 						)  || (0 < pcltSetItemAbility5->m_siMP 							)? _tcscat(szToolTipBuffer, MakeAddText(9653, pcltSetItemAbility->m_siMP 						+ pcltSetItemAbility2->m_siMP 						 + pcltSetItemAbility3->m_siMP 							+ pcltSetItemAbility4->m_siMP 						+ pcltSetItemAbility5->m_siMP 								)) : 0;					
	(0 < pcltSetItemAbility->m_siCritical				) || (0 < pcltSetItemAbility2->m_siCritical					)  || (0 < pcltSetItemAbility3->m_siCritical					)|| (0 < pcltSetItemAbility4->m_siCritical					)  || (0 < pcltSetItemAbility5->m_siCritical					)? _tcscat(szToolTipBuffer, MakeAddText(9654, pcltSetItemAbility->m_siCritical					+ pcltSetItemAbility2->m_siCritical					 + pcltSetItemAbility3->m_siCritical					+ pcltSetItemAbility4->m_siCritical					+ pcltSetItemAbility5->m_siCritical							)) : 0;			
	(0 < pcltSetItemAbility->m_siIceMagicTargetAttack	) || (0 < pcltSetItemAbility2->m_siIceMagicTargetAttack		)  || (0 < pcltSetItemAbility3->m_siIceMagicTargetAttack		)|| (0 < pcltSetItemAbility4->m_siIceMagicTargetAttack		)  || (0 < pcltSetItemAbility5->m_siIceMagicTargetAttack		)? _tcscat(szToolTipBuffer, MakeAddText(9655, pcltSetItemAbility->m_siIceMagicTargetAttack		+ pcltSetItemAbility2->m_siIceMagicTargetAttack		 + pcltSetItemAbility3->m_siIceMagicTargetAttack		+ pcltSetItemAbility4->m_siIceMagicTargetAttack		+ pcltSetItemAbility5->m_siIceMagicTargetAttack				)) : 0;
	(0 < pcltSetItemAbility->m_siIceMagicGetAttack		) || (0 < pcltSetItemAbility2->m_siIceMagicGetAttack		)  || (0 < pcltSetItemAbility3->m_siIceMagicGetAttack			)|| (0 < pcltSetItemAbility4->m_siIceMagicGetAttack			)  || (0 < pcltSetItemAbility5->m_siIceMagicGetAttack			)? _tcscat(szToolTipBuffer, MakeAddText(9656, pcltSetItemAbility->m_siIceMagicGetAttack			+ pcltSetItemAbility2->m_siIceMagicGetAttack		 + pcltSetItemAbility3->m_siIceMagicGetAttack			+ pcltSetItemAbility4->m_siIceMagicGetAttack		+ pcltSetItemAbility5->m_siIceMagicGetAttack				)) : 0;		
	(0 < pcltSetItemAbility->m_siMagicDefence			) || (0 < pcltSetItemAbility2->m_siMagicDefence				)  || (0 < pcltSetItemAbility3->m_siMagicDefence				)|| (0 < pcltSetItemAbility4->m_siMagicDefence				)  || (0 < pcltSetItemAbility5->m_siMagicDefence				)? _tcscat(szToolTipBuffer, MakeAddText(40009, pcltSetItemAbility->m_siMagicDefence				+ pcltSetItemAbility2->m_siMagicDefence				 + pcltSetItemAbility3->m_siMagicDefence				+ pcltSetItemAbility4->m_siMagicDefence				+ pcltSetItemAbility5->m_siMagicDefence						)) : 0;
	(0 < pcltSetItemAbility->m_siMagicAttack			) || (0 < pcltSetItemAbility2->m_siMagicAttack				)  || (0 < pcltSetItemAbility3->m_siMagicAttack					)|| (0 < pcltSetItemAbility4->m_siMagicAttack				)  || (0 < pcltSetItemAbility5->m_siMagicAttack					)? _tcscat(szToolTipBuffer, MakeAddText(9657, pcltSetItemAbility->m_siMagicAttack				+ pcltSetItemAbility2->m_siMagicAttack				 + pcltSetItemAbility3->m_siMagicAttack					+ pcltSetItemAbility4->m_siMagicAttack				+ pcltSetItemAbility5->m_siMagicAttack						)) : 0;
	(0 < pcltSetItemAbility->m_siPoisonMagicTargetAttack) || (0 < pcltSetItemAbility2->m_siPoisonMagicTargetAttack	)  || (0 < pcltSetItemAbility3->m_siPoisonMagicTargetAttack		)|| (0 < pcltSetItemAbility4->m_siPoisonMagicTargetAttack	)  || (0 < pcltSetItemAbility5->m_siPoisonMagicTargetAttack		)? _tcscat(szToolTipBuffer, MakeAddText(40008, pcltSetItemAbility->m_siPoisonMagicTargetAttack	+ pcltSetItemAbility2->m_siPoisonMagicTargetAttack	 + pcltSetItemAbility3->m_siPoisonMagicTargetAttack		+ pcltSetItemAbility4->m_siPoisonMagicTargetAttack	+ pcltSetItemAbility5->m_siPoisonMagicTargetAttack			)) : 0;		
	(0 < pcltSetItemAbility->m_siNoFirstAttack			) || (0 < pcltSetItemAbility2->m_siNoFirstAttack			)  || (0 < pcltSetItemAbility3->m_siNoFirstAttack				)|| (0 < pcltSetItemAbility4->m_siNoFirstAttack				)  || (0 < pcltSetItemAbility5->m_siNoFirstAttack				)? _tcscat(szToolTipBuffer, MakeAddText(9658, pcltSetItemAbility->m_siNoFirstAttack				+ pcltSetItemAbility2->m_siNoFirstAttack			 + pcltSetItemAbility3->m_siNoFirstAttack				+ pcltSetItemAbility4->m_siNoFirstAttack			+ pcltSetItemAbility5->m_siNoFirstAttack					)) : 0;		
	(0 < pcltSetItemAbility->m_siNature					) || (0 < pcltSetItemAbility2->m_siNature					)  || (0 < pcltSetItemAbility3->m_siNature						)|| (0 < pcltSetItemAbility4->m_siNature					)  || (0 < pcltSetItemAbility5->m_siNature						)? _tcscat(szToolTipBuffer, MakeAddText(9647, pcltSetItemAbility->m_siNature					+ pcltSetItemAbility2->m_siNature					 + pcltSetItemAbility3->m_siNature						+ pcltSetItemAbility4->m_siNature					+ pcltSetItemAbility5->m_siNature							)) : 0;				
	(0 < pcltSetItemAbility->m_siFireEnchantAttack		) || (0 < pcltSetItemAbility2->m_siFireEnchantAttack		)  || (0 < pcltSetItemAbility3->m_siFireEnchantAttack			)|| (0 < pcltSetItemAbility4->m_siFireEnchantAttack			)  || (0 < pcltSetItemAbility5->m_siFireEnchantAttack			)? _tcscat(szToolTipBuffer, MakeAddText(9659, pcltSetItemAbility->m_siFireEnchantAttack			+ pcltSetItemAbility2->m_siFireEnchantAttack		 + pcltSetItemAbility3->m_siFireEnchantAttack			+ pcltSetItemAbility4->m_siFireEnchantAttack		+ pcltSetItemAbility5->m_siFireEnchantAttack				)) : 0;
	(0 < pcltSetItemAbility->m_siWaterEnchantAttack		) || (0 < pcltSetItemAbility2->m_siWaterEnchantAttack		)  || (0 < pcltSetItemAbility3->m_siWaterEnchantAttack			)|| (0 < pcltSetItemAbility4->m_siWaterEnchantAttack		)  || (0 < pcltSetItemAbility5->m_siWaterEnchantAttack			)? _tcscat(szToolTipBuffer, MakeAddText(9660, pcltSetItemAbility->m_siWaterEnchantAttack		+ pcltSetItemAbility2->m_siWaterEnchantAttack		 + pcltSetItemAbility3->m_siWaterEnchantAttack			+ pcltSetItemAbility4->m_siWaterEnchantAttack		+ pcltSetItemAbility5->m_siWaterEnchantAttack				)) : 0;		
	(0 < pcltSetItemAbility->m_siWindEnchantAttack		) || (0 < pcltSetItemAbility2->m_siWindEnchantAttack		)  || (0 < pcltSetItemAbility3->m_siWindEnchantAttack			)|| (0 < pcltSetItemAbility4->m_siWindEnchantAttack			)  || (0 < pcltSetItemAbility5->m_siWindEnchantAttack			)? _tcscat(szToolTipBuffer, MakeAddText(9661, pcltSetItemAbility->m_siWindEnchantAttack			+ pcltSetItemAbility2->m_siWindEnchantAttack		 + pcltSetItemAbility3->m_siWindEnchantAttack			+ pcltSetItemAbility4->m_siWindEnchantAttack		+ pcltSetItemAbility5->m_siWindEnchantAttack				)) : 0;	
	(0 < pcltSetItemAbility->m_siLandEnchantAttack		) || (0 < pcltSetItemAbility2->m_siLandEnchantAttack		)  || (0 < pcltSetItemAbility3->m_siLandEnchantAttack			)|| (0 < pcltSetItemAbility4->m_siLandEnchantAttack			)  || (0 < pcltSetItemAbility5->m_siLandEnchantAttack			)? _tcscat(szToolTipBuffer, MakeAddText(9662, pcltSetItemAbility->m_siLandEnchantAttack			+ pcltSetItemAbility2->m_siLandEnchantAttack		 + pcltSetItemAbility3->m_siLandEnchantAttack			+ pcltSetItemAbility4->m_siLandEnchantAttack		+ pcltSetItemAbility5->m_siLandEnchantAttack				)) : 0;	

														 										 
	return szToolTipBuffer;								 					  
}														 					  





