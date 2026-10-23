#include "../Client\MiniGame/ZooKeeperDraw.h"
#include "../Lib/Graphic/Graphic_Capsule.h"

CZooDraw::CZooDraw()
{
}

CZooDraw::~CZooDraw()
{
	FreeImage();
}

bool CZooDraw::LoadImage()
{
	if(!GP.LoadSprFromFile(TEXT("MiniGame/MonsterCorporation/MonsterCorporationBack.SPR"), &m_sprBlockBack))
		return false;
	if(!GP.LoadSprFromFile(TEXT("MiniGame/MonsterCorporation/MonsterCorporationBlock.SPR"), &m_sprBlock))
		return false;
	if(!GP.LoadSprFromFile(TEXT("MiniGame/MonsterCorporation/Select.spr"), &m_sprSelectBlock))
		return false;
	if(!GP.LoadSprFromFile(TEXT("MiniGame/MonsterCorporation/Number.spr"), &m_sprNumber))
		return false;
	if(!GP.LoadSprFromFile(TEXT("GImg/MakeItemDrawPart1.SPR"), &m_sprTime1))
		return false;
	if(!GP.LoadSprFromFile(TEXT("GImg/MakeItemDrawPart2.SPR"), &m_sprTime2))
		return false;
	if(!GP.LoadSprFromFile(TEXT("GImg/MakeItemDrawPart3.SPR"), &m_sprTime3))
		return false;

	if(!m_AniBlock.LoadSpr(TEXT("MiniGame/MonsterCorporation/Cut1.spr"), 100))
		return false;
	if(!m_AniSpecialBlock.LoadSpr(TEXT("MiniGame/MonsterCorporation/MonsterCorporationBlock.SPR"), 10))
		return false;
	if(!m_AniEffectBlock.LoadSpr(TEXT("MiniGame/MonsterCorporation/Cut1.spr"), 50))
		return false;

	return true;
}

void CZooDraw::FreeImage()
{
	if(m_sprBlockBack.pImage)
	{
		GP.FreeSpr(m_sprBlockBack);
	}
	if(m_sprBlock.pImage)
	{
		GP.FreeSpr(m_sprBlock);
	}
	if(m_sprSelectBlock.pImage)
	{
		GP.FreeSpr(m_sprSelectBlock);
	}
	if(m_sprNumber.pImage)
	{
		GP.FreeSpr(m_sprNumber);
	}
	if(m_sprTime1.pImage)
	{
		GP.FreeSpr(m_sprTime1);
	}
	if(m_sprTime2.pImage)
	{
		GP.FreeSpr(m_sprTime2);
	}
	if(m_sprTime3.pImage)
	{
		GP.FreeSpr(m_sprTime3);
	}
}

void CZooDraw::DrawBlockBack(SI32 x, SI32 y, int imagenum)
{
	GP.PutSpr(&m_sprBlockBack, x, y, imagenum);
}

void CZooDraw::DrawBlock(SI32 x, SI32 y, int imagenum )
{
	GP.PutSpr(&m_sprBlock, x, y, imagenum );
}

void CZooDraw::DrawSelectBlock(SI32 x, SI32 y, SI32 imagenum)
{
	GP.PutSpr(&m_sprBlock, x, y, 7 + imagenum );
}
// 임시로 쓰고 있다
void CZooDraw::DrawRemoveBlock(SI32 x, SI32 y, SI32 imagenum /* = 0 */)
{
	GP.PutSpr(&m_sprBlock, x, y, 14 + imagenum );
}
void CZooDraw::DrawSelect(SI32 x, SI32 y, SI32 imagenum)
{
	GP.PutSpr(&m_sprSelectBlock,x, y, imagenum);
}

void CZooDraw::DrawNumber(SI32 x, SI32 y, SI32 imagenum)
{
	//m_sprNumber.PutSprT(x, y, 0, imagenum);
	GP.PutSpr(&m_sprNumber, x, y,  imagenum);
}

void CZooDraw::DrawDeleteBlock(SI32 x, SI32 y)
{
	m_AniBlock.DrawAniSpr(x, y);
}

void CZooDraw::DrawSpecialBlock(SI32 x, SI32 y)
{
	m_AniSpecialBlock.DrawAniSpr(x, y);
	m_AniSpecialBlock.AniProc();
}

void CZooDraw::DrawEffectBlock(SI32 x, SI32 y)
{
	m_AniEffectBlock.DrawAniSpr(x, y);
//	m_AniEffectBlock.AniProcEnd();
}

void CZooDraw::DrawTime1(SI32 x, SI32 y, SI32 imagenum)
{
	GP.PutSpr(&m_sprTime1, x, y, imagenum);
}

void CZooDraw::DrawTime2(SI32 x, SI32 y, SI32 imagenum)
{
	GP.PutSpr(&m_sprTime2, x, y, imagenum);
}

void CZooDraw::DrawTime3(SI32 x, SI32 y, SI32 imagenum)
{
	GP.PutSpr(&m_sprTime3, x, y, imagenum);
}