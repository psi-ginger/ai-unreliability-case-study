#include "../Client/MiniGame/AniSpr.h"
#include "../Lib/Graphic/Graphic_Capsule.h"

CAniSpr::CAniSpr()
{
//	m_pSpr = NULL;
	m_MaxFrame = 0;		// 프래임 개수
	m_NowFrame = -1;		// 현재 프레임

	m_DelayTime = 0;	// 지연 시간
	m_LastTime  = GetTickCount();		
}

CAniSpr::~CAniSpr()
{
	Release();
}

void CAniSpr::Release()
{	
	if(m_pSpr.pImage)
	{
		GP.FreeSpr(m_pSpr);
	}
	
	m_MaxFrame = 0;
	m_NowFrame = 0;

	m_DelayTime	= 0;
	m_LastTime	= GetTickCount();


}

BOOL CAniSpr::LoadSpr(TCHAR* pName, SI32 delay)
{
	if(!GP.LoadSprFromFile(pName, &m_pSpr))
		return FALSE;

	SetAniData(delay);

	return TRUE;
}

void CAniSpr::SetAniData(SI32 delay)
{
	m_MaxFrame = m_pSpr.GetImageNum(); //3;//m_pSpr.clHeader.clImgInfo[MAX_IMG_PER_TSPR].siStartPos;
	m_DelayTime = delay;
}

void CAniSpr::ResetFrame()
{
	m_NowFrame = -1;
	m_LastTime  = GetTickCount();
}
int CAniSpr::AniProc()
{
	if(GetTickCount() - m_LastTime >= m_DelayTime)
	{
		m_LastTime = GetTickCount();
		++m_NowFrame %= m_MaxFrame;
	
		if(m_NowFrame == 0) return TRUE;	// 끝나면 TRUE
	}

	return FALSE;	// 안끝나면 FALSE
}

int CAniSpr::AniProcEnd()
{
	if(GetTickCount() - m_LastTime >= m_DelayTime)
	{
		m_LastTime = GetTickCount();

		if(++m_NowFrame >= m_MaxFrame)
		{
			m_NowFrame = m_MaxFrame -1;
			return TRUE;	// 끝나면 TRUE
		}
	}

	return FALSE;	// 안끝나면 FALSE
}

void CAniSpr::DrawAniSpr(SI32 x, SI32 y, SI32 imagenum)
{
//	g_graphics.PutSpr16CT(x, y, &m_pSpr.Image[m_pSpr.clHeader.clImgInfo[m_NowFrame].siStartPos], m_pSpr.clHeader.siXsize, m_pSpr.clHeader.siYsize);
//	m_pSpr.PutSprT(x, y, m_NowFrame);
	// 일단 테스트
	GP.PutSprAlpha(&m_pSpr,x, y, 8, m_NowFrame);
}

void CAniSpr::DrawAniSprT(SI32 x, SI32 y, SI32 imagenum)
{
	//	g_graphics.PutSpr16CT(x, y, &m_pSpr.Image[m_pSpr.clHeader.clImgInfo[m_NowFrame].siStartPos], m_pSpr.clHeader.siXsize, m_pSpr.clHeader.siYsize);
	GP.PutSpr(&m_pSpr, x, y, m_NowFrame);
}