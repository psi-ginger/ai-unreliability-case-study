#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CStatus;

/**************************************************************************************/
//
//	[추가 : 황진성 2007. 12. 20 변신창 클레스 추가.]
//
/**************************************************************************************/
class CNChartransformDlg : public CDialogController
{ 
	enum { TRANSFORM, RETMSG_USE_ITEM, RETMSG_ADD_1_DAY, RETMSG_ADD_7_DAY, RETMSG_ADD_30_DAY  };	

public:
	CNChartransformDlg();
	~CNChartransformDlg();

	void Init();
	void Destroy();
	void Create();
	void Action();

	void DrawChar( SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );

	void Status(void);										// 상태정보 세팅하기.
	SI32 GetFindCharSlot(SI32 _kind);						// kind로 변신 캐릭의 슬롯 번호 얻기.
		
	static void CALLBACK StaticChartransformDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NChartransformDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void Msg(SI32 MsgNum);

	void SetStatus(CStatus& _Status);
	TCHAR* CNChartransformDlg::GetWeaponEffectExplain( void );
	

private:
	InterfaceFactory	m_InterfaceFactory;
	SI32				m_siCharKind;

	cltAniInfo			m_claniinfo;
	SI32				Animation;
	SI32				siDirection;
	SI32				Font;

	SI32				realxsize;
	SI32				realysize;

	SI32				xstart;
	SI32				ystart;

	TSpr				m_Spr;
};

// 스탯 정보.
class CStatus
{
public:
	SI32	Dex;
	SI32	Str;
	SI32	Vit;
	SI32	Mag;
	SI32	m_AC;
	
	CStatus()
	{
		ZeroMemory( this, sizeof( CStatus ) );
	}
	~CStatus()
	{
		
	}
};