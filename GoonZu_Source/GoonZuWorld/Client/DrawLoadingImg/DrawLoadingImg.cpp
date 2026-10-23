// DrawLoadingImg.cpp: implementation of the CDrawLoadingImg class.
//
//////////////////////////////////////////////////////////////////////

#include <Graphic.h>
#include <Graphic_Capsule.h>
#include "DrawLoadingImg.h"

#include "../Client.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern cltCommonLogic* pclClient;

CDrawLoadingImg::CDrawLoadingImg(HWND hWnd,SI32 sx,SI32 sy)
{
	GP.LoadSprFromFile(TEXT("GImg\\Loading.SPR"),&m_pLoadingImg);

	if(	pclClient->siServiceArea != ConstServiceArea_Japan 
		&& pclClient->siServiceArea != ConstServiceArea_China
		&& pclClient->siServiceArea != ConstServiceArea_Korea
		&& pclClient->siServiceArea != ConstServiceArea_Taiwan
		&& pclClient->siServiceArea != ConstServiceArea_USA
		&& pclClient->siServiceArea != ConstServiceArea_English 
		&& pclClient->siServiceArea != ConstServiceArea_NHNChina
		&& pclClient->siServiceArea != ConstServiceArea_EUROPE
	)
	{
		m_siX = ( sx - 1024 ) / 2 ;
		m_siY = ( sy - 768 ) / 2 ;
	}
	else
	{
		if (!g_SoundOnOffDlg.m_bMaxWindowMode)
		{
			m_siX = (sx - 1024) / 2;
			m_siY = -8;
		}
		else
		{
			m_siX = 80;
			m_siY = 40;
		}
	}
}

CDrawLoadingImg::~CDrawLoadingImg()
{
	if(m_pLoadingImg.pImage)
	{
		GP.FreeSpr(m_pLoadingImg);
	}
}

void CDrawLoadingImg::Draw(LPDIRECTDRAWSURFACE7 pSurface, SI32 scX, SI32 scY )
{
	if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
	{
		if( pclClient->GetGameStatus() >= GS_INITCONNECTFRONTSERVER &&
			pclClient->GetGameStatus() < GS_INITSELECTWORLDLIST)
		{
			if ( 18 <= pclClient->siServiceAge )
			{
				GP.LoadSprFromFile(TEXT("GImg\\GovernMentRankBackground_00_001.SPR"),&m_pLoadingImg);
			}
			else
			{
				GP.LoadSprFromFile(TEXT("GImg\\GovernMentRankBackground_00_000.SPR"),&m_pLoadingImg);
			}
		}
		else
		{
			GP.LoadSprFromFile(TEXT("GImg\\Loading.SPR"),&m_pLoadingImg);
		}
	}
	if(GP.LockSurface(pSurface)==TRUE)
	{
		SI32 siScreenDrawX = m_siX + scX , siScreenDrawY = m_siY + scY ;

		GP.PutSpr(&m_pLoadingImg, siScreenDrawX, siScreenDrawY, 0 );
		
		GP.UnlockSurface(pSurface);
	}
}