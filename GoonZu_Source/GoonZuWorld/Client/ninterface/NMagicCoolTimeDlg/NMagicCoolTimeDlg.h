#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CStatic;
class CNMagicBuf;
class cltCharClient;
class CMagicAttribute;

class CNMagicCoolTimeDlg : public CDialogController
{
public:
	enum MCT_ARRAY 
	{
		MCT_ARRAY_NONE			= -1,

		MCT_ARRAY_REMAINTIME1	= 0,
		MCT_ARRAY_REMAINTIME2,
		MCT_ARRAY_REMAINTIME1SHADOW,
		MCT_ARRAY_REMAINTIME2SHADOW,
		MCT_ARRAY_ACTIVEBACK,
		MCT_ARRAY_BUF1BACK,
		MCT_ARRAY_BUF2BACK,
		MCT_ARRAY_ACTIVE,
		MCT_ARRAY_BUF1,
		MCT_ARRAY_BUF2,
		MCT_ARRAY_ACTIVECOOLTIME,
		MCT_ARRAY_BUF1COOLTIME,
		MCT_ARRAY_BUF2COOLTIME,
		
		MCT_ARRAY_MAX
	};

	enum MCT_VARIABLE
	{
		MCT_VARIABLE_FLICKERSTART_TIME = 5,		// 버프 마법이 깜빡이기 시작하는 시간(초)
		MCT_VARIABLE_COOLTIMEIMAGE_NUM = 24,	// 쿨타임 표시용 이미지 총 갯수
	};
// Vriable Area
private:
	CStatic*	m_pctrlStatic[MCT_ARRAY_MAX];
	SI32		m_siActiveMagicCoolTime;
	SI32		m_siActiveMagicCoolTimeStart;

public:

// Function Area
private:
	void ShowActiveMagicInfo( cltCharClient* pclchar, CMagicAttribute* pclMagicAttributePtr );

	void ShowMagicBufInfo( cltCharClient* pclchar, CMagicAttribute* pclMagicAttributePtr );
	void ShowMagicBufControllers( SI32 siIndex );
	void HideMagicBufControllers( SI32 siIndex );

	void DisplayMagicBufInfo( SI32 siIndex, CNMagicBuf* pclMagicBuf, SI32 siFontIndex );

	
	

public:
	CNMagicCoolTimeDlg();
	~CNMagicCoolTimeDlg();

	void Init();
	void Create();

	void Action();

	static void CALLBACK StaticCallBackDialogNMagicCoolTime( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNMagicCoolTime( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetActiveMagicCoolTime( SI32 siCoolTime );

};