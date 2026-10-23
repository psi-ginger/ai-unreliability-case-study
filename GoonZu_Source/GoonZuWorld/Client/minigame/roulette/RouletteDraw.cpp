#include "../Client\MiniGame/Roulette/RouletteDraw.h"
#include "../Lib/Graphic/Graphic_Capsule.h"

#include "../Client/Client.h"

extern cltCommonLogic* pclClient;


CRouletteDraw::CRouletteDraw()
{
}

CRouletteDraw::~CRouletteDraw()
{
	FreeImage();
}


bool CRouletteDraw::LoadImage()
{
	if(!GP.LoadSprFromFile(TEXT("MiniGame/MonsterCorporation/Number.spr"), &m_sprNumber))
		return false;
	if(!GP.LoadSprFromFile(TEXT("NInterface/Common/COMMON_10X13_00_000.SPR"), &m_sprNumber2))
		return false;
	if(!GP.LoadSprFromFile(TEXT("NInterface/Image/Image_60x60_00_000.SPR"), &m_sprCpuSelect))
		return false;
	if(!GP.LoadSprFromFile(TEXT("NInterface/Image/Image_16X16_00_000.SPR"), &m_sprDot))
		return false;

	if(!m_aniBird.LoadSpr(TEXT("NInterface/Image/Image_142x144_00_000.SPR"), 100))
		return false;
	if(!m_aniRoll.LoadSpr(TEXT("NInterface/Image/Image_56x55_00_000.SPR"), 10))
		return false;

	return true;
}

void CRouletteDraw::FreeImage()
{
	if(m_sprNumber.pImage)
	{
		GP.FreeSpr(m_sprNumber);
	}
	if(m_sprNumber2.pImage)
	{
		GP.FreeSpr(m_sprNumber2);
	}
	if(m_sprCpuSelect.pImage)
	{
		GP.FreeSpr(m_sprCpuSelect);
	}
	if(m_sprDot.pImage)
	{
		GP.FreeSpr(m_sprDot);
	}
}

void CRouletteDraw::DrawNumber(SI32 x, SI32 y, SI32 imagenum)
{
	GP.PutSpr(&m_sprNumber, x, y, imagenum);
}

void CRouletteDraw::DrawNumber2(SI32 x, SI32 y, SI32 imagenum)
{
	GP.PutSpr(&m_sprNumber2, x, y, imagenum);
}

void CRouletteDraw::DrawUserSelect(SI32 x, SI32 y)
{
	TSpr* pSpr = pclClient->GetGlobalSpr(GIMG_ITEMIMG02);

	GP.PutSpr(pSpr, x, y, 120);
}

void CRouletteDraw::DrawCpuSelect(SI32 x, SI32 y, SI32 imagenum)
{
	GP.PutSpr(&m_sprCpuSelect,x, y, imagenum);
}

void CRouletteDraw::DrawBird(SI32 x, SI32 y)
{
	m_aniBird.DrawAniSprT(x, y);
	m_aniBird.AniProc();

	GP.PutSpr(&m_sprDot, x - 20, y + 60, 10);
	GP.PutSpr(&m_sprDot, x - 50, y + 60, 10);
	GP.PutSpr(&m_sprDot, x - 80, y + 60, 10);
	GP.PutSpr(&m_sprDot, x - 110, y + 60, 10);
}

void CRouletteDraw::DrawRoll(SI32 x, SI32 y, SI32 delay)
{
//	m_aniRoll.SetAniData(delay);
	m_aniRoll.DrawAniSprT(x, y);
	m_aniRoll.AniProc();
}
