//---------------------------------
// 2004/9/01 성준엽
//---------------------------------
/* ==========================================================================
	클래스 :		CNStaticNAutoHuntingDlgMsgProc

	작성일 :		05/03/25
	작성자 :		김태양

	목  적 :		

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/
#pragma once

#include <Directives.h>

#define MAX_SUSPECT_NUM		3000

#define PRISON_TIME			300

#define CLIENT_UPDATE_TIME			2
#define NOSUSPICTION_PRISON_TIME	5	// 매크로 사용자 허위 신고시 수감 시간(분) 

#ifdef _CHINA
	#define REPORT_MINLEVEL			70
#else
	#define REPORT_MINLEVEL			20
#endif

#include "../../../Resource.h"
#include "../../InterfaceMgr/DialogController.h"

class CStatic;
class CEdit;
class CButton;


class CNcltAutoHuntingManager : public CDialogController
{
public: // Client
	CNcltAutoHuntingManager();
	~CNcltAutoHuntingManager();

	void Create();

	static void CALLBACK StatiCNAutoHuntingDlgMsgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NAutoHuntingDlgMsgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	// Txt : 요구단어,request : 의뢰자 이름
	void Set(TCHAR * Txt,SI32 slot,SI32 minuts,SI16 prisontime,SI16 prisonerpenalty);
	
	void Action();
	void SetStaticTime(SI32 totaltime);
	
	void SetLeftTime(SI32 time)
	{
		m_siLeftTime = time;
	}
	SI32 GetLeftTime()
	{
		return m_siLeftTime ;
	}

	
private: // Client
	
	CStatic     	*m_pAutoHuntStatic1;
	CEdit       	*m_pAutoHuntStatic2;
	CStatic     	*m_pAutoHuntStatic3;
	CStatic     	*m_pAutoHuntStatic4;
	CButton			*m_pAutoHuntButton;
	CEdit           *m_pAutoHuntEdit;    

	SI32	m_siLeftTime;
	SI32	m_siSlot;

	



};
