#include "../Client/ninterface/NMonsterAttackEventDlg/NMonsterAttackEventDlg.h"
#include "../common/Event/MonsterAttackEvent/MonsterAttack.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"


extern cltCommonLogic* pclClient;


NMonsterAttackEventDlg::NMonsterAttackEventDlg()
{	
	if(false == m_Spr.LoadTSprFromFile("NInterface/image/image_42x42_00_008_monster_attack_monster_face.SPR"))
	{
		MsgBox("파일열기 실패", "monster_attack_monster_face.SPR");
	}

	ZeroMemory(&m_bMonsterDie,	sizeof(m_bMonsterDie));
	
	m_dwStartTickCount	 = GetTickCount();
	m_dwRemainSecondTime = 0;
}

NMonsterAttackEventDlg::~NMonsterAttackEventDlg()
{
	
}

void NMonsterAttackEventDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NMonsterAttackEventDlg/NMonsterAttackEventDlg.ddf" ) );
	file.CreatePopupDialog( this, NMONSTER_ATTACK_EVENT, TEXT( "dialog_MonsterAttackEvent" ), StaticNMonsterAttackEventDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,  NMONSTER_ATTACK_EVENT_IMAGE_STATIC_WANTED,	this ), NMONSTER_ATTACK_EVENT_IMAGE_STATIC_WANTED,	 TEXT( "imagestatic_wanted" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		  NMONSTER_ATTACK_EVENT_STATIC_REMAIN_TIME,		this ), NMONSTER_ATTACK_EVENT_STATIC_REMAIN_TIME,	 TEXT( "statictext_remain_time" ) );	
		
	CStatic* pStatic = m_InterfaceFactory.GetStatic(NMONSTER_ATTACK_EVENT_STATIC_REMAIN_TIME);
	pStatic->SetTextPrintOption(DT_CENTER);
	pStatic->SetFontColor(RGB( 255, 255, 255));
	pStatic->Refresh();


	SetHeight(50);
	SetAlphaMode(7);

	SetX( 380 );                        
	SetY( 80 );     

	SetActionTimer( 0 );
	SetTimerOnOff( true );
}


void NMonsterAttackEventDlg::Action()
{
	DWORD dwTickCount	  = GetTickCount();
	DWORD dwTime		  = dwTickCount - m_dwStartTickCount;
	DWORD SecondTimeCount = 0;
	if(dwTime > 1000)
	{
		m_dwStartTickCount = dwTickCount;

		SecondTimeCount = (dwTime / 1000);
			
		m_dwRemainSecondTime -= SecondTimeCount;

		CStatic* pStatic		= m_InterfaceFactory.GetStatic(NMONSTER_ATTACK_EVENT_STATIC_REMAIN_TIME);
		TCHAR    szbuffer[64]	= "";
		
		StringCchPrintf(szbuffer, sizeof(szbuffer), GetTxtFromMgr(1379), m_dwRemainSecondTime);
		pStatic->SetText(szbuffer);
	}
}

void NMonsterAttackEventDlg::SetRemainSecondTime(DWORD _dwRemainSecondTime)
{
	m_dwRemainSecondTime = _dwRemainSecondTime;
}

void NMonsterAttackEventDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{	
	const SI32 ImageWidthSize = 42;
		  SI32 Xpos = 0 ;	
		  SI32 Ypos = 0 ;	

	for(SI32 ImageCount = 0; ImageCount < CMonsterAttackEvent::MOSTER_NUM; ++ImageCount)
	{
		// x좌표 만들기.
		Xpos = GetX() + ScreenRX + (ImageWidthSize * ImageCount) + 15; 
		Ypos = GetY() + ScreenRY + 80;

		GP.PutSpr(&m_Spr, Xpos, Ypos, ImageCount); 

		// 몬스터가 죽었으면 엑스표시를 보이게.
		if(true == m_bMonsterDie[ImageCount])
		{
			GP.PutSprAlpha(&m_Spr, Xpos, Ypos, 12, 7);   
		}
	}

}

void CALLBACK NMonsterAttackEventDlg::StaticNMonsterAttackEventDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NMonsterAttackEventDlg *pThis = (NMonsterAttackEventDlg*) pControllerMgr;
	pThis->NMonsterAttackEventDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NMonsterAttackEventDlg::NMonsterAttackEventDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch ( nControlID )
	{
		
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;	
	}
}


void NMonsterAttackEventDlg::MonsterDie(SI32 _Slot)
{
	if(0 > _Slot || CMonsterAttackEvent::MOSTER_NUM <= _Slot)
		return;

	m_bMonsterDie[_Slot] = true;
}