
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/DialogController.h"

#define MAX_DIVINATION_COUNT	50

class CNewYearEventSurfaceObj
{
public:
	CNewYearEventSurfaceObj();
	~CNewYearEventSurfaceObj();

	void Create( SI16 SurfaceWidth, SI16 SurfaceHeight );

	LPDIRECTDRAWSURFACE7 GetSurface()		{		return m_lpSurface;			}

private:
	LPDIRECTDRAWSURFACE7 m_lpSurface;

	SI16 m_siWidth;
	SI16 m_siHeight;
};

class CNNewYearEventRoulette : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory m_InterfaceFactory;

	SI16				m_siDivinationList[MAX_DIVINATION_COUNT];
	SI16				m_siDivinationCount;
	SI16				m_siResultDivination;
	SI16				m_siCurIndex;
	SI32				m_siTotalCount;
	SI32				m_siCurCount;
	SI16				m_siStartY;

	CNewYearEventSurfaceObj*	m_pDivinationSurfaceObj;
	RGB16*				m_ImgDivinationBuffer;
	SI16				m_siSpeed;
	DWORD				m_siStartClock;
	TSpr				m_SprSelect;

	bool				m_bCreateImgBuffer;

	SI64				m_siNeedPrice;

	SI16				m_siBufKind_Type;

public:
	CNNewYearEventRoulette();
	~CNNewYearEventRoulette();

	void	Create(void);
	void	Destroy(void);
	void	DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	void	Action(void);

	void	MakeDivinationImage();

	void	SetDivinations(SI16* psiDivinations, SI16 siCount, SI16 siResultDivination, SI16 siBufKind_Type)
	{
		if(psiDivinations != NULL && siCount > 0)
		{
			memcpy(m_siDivinationList, psiDivinations, min(sizeof(m_siDivinationList), sizeof(SI16) * siCount));
			m_siDivinationCount = siCount;
		}

		m_siResultDivination = siResultDivination;
		m_siBufKind_Type = siBufKind_Type;
	}

	void	Init();

	bool	Start();
	bool	End();

	static void CALLBACK StaticCallBackDialogNNewYearEventRoulette( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNNewYearEventRoulette( UINT nEvent, int nControlID, CControllerObj* pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};