#pragma  once

#include "../../Client/Client.h"
#include "../../InterfaceMgr/DialogController.h"

#define MAX_CHARBUFF_COUNT	30

class CCharBufSurfaceObj
{
public:
	CCharBufSurfaceObj();
	~CCharBufSurfaceObj();

	void Create( SI16 SurfaceWidth, SI16 SurfaceHeight );

	LPDIRECTDRAWSURFACE7 GetSurface()		{		return m_lpSurface;			}

private:
	LPDIRECTDRAWSURFACE7 m_lpSurface;

	SI16 m_siWidth;
	SI16 m_siHeight;
};

class CNCharRouletteDlg : public CDialogController
{
public:
	CNCharRouletteDlg();
	~CNCharRouletteDlg();

	void	Create(void);
	void	Destroy(void);
	void	DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	void	Action(void);

	void	MakeBufRouletteImage();

	void	SetCharKinds(SI16* psiCharKinds, SI16 siCount, SI16 siResultKind)
	{
		if(psiCharKinds != NULL && siCount > 0)
		{
			memcpy(m_siCharKind, psiCharKinds, min(sizeof(m_siCharKind), sizeof(SI16) * siCount));
			m_siCharBufCount = siCount;
		}

		m_siResultCharKind = siResultKind;
	}

	void	Init();

	bool	Start();
	bool	End();

	static void CALLBACK StaticNCharfRouletteProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NCharfRouletteDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

private:
	InterfaceFactory	m_InterfaceFactory;

	SI16				m_siCharKind[MAX_CHARBUFF_COUNT];
	SI16				m_siCharBufCount;
	SI16				m_siResultCharKind;
	SI16				m_siCurIndex;
	SI32				m_siTotalCount;
	SI32				m_siCurCount;
	SI16				m_siStartY;

	CCharBufSurfaceObj*	m_pCharBufSurfaceObj;
	RGB16*				m_ImgCharBufBuffer;
	SI16				m_siSpeed;
	DWORD				m_siStartClock;
	TSpr				m_SprSelect;

	bool				m_bCreateImgBuffer;

	SI64				m_siNeedPrice;
	bool				b_bUseTicketItem;
};