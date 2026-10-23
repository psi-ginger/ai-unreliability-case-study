#include "CharPartyEmoticon.h"

#include "..\..\..\..\CommonLogic\CommonLogic.h"
#include "Graphic_Capsule.h"
extern cltCommonLogic* pclClient;


CCharPartyEmoticon::CCharPartyEmoticon()
{	
	Initialize();
}

CCharPartyEmoticon::~CCharPartyEmoticon()
{
}

void CCharPartyEmoticon::Initialize()
{
	m_cCurrentFrame = 0;
	m_cCurrentDelayFrame = 0;
	return;
}

void CCharPartyEmoticon::Draw( TSpr *pXSPR, SI32 XPos, SI32 YPos )
{
	if(pXSPR->pImage == NULL) return;

	SI32 font = max(0, pclClient->CurrentClock / 50)  / 4 % pXSPR->GetImageNum();
		GP.PutSpr( pXSPR,
		XPos, 			
		YPos,
		font);		

/*	if ( m_cCurrentDelayFrame++ == 0 )
	{
		pXSPR->PutSprT( 
		XPos, 			
		YPos,
		m_cCurrentFrame
		);		
	}
	else
	{
		if ( m_cCurrentDelayFrame == DELAYFRAME_NUM )
		{
			m_cCurrentDelayFrame = 0;
			m_cCurrentFrame++;
			if ( m_cCurrentFrame == pXSPR->clHeader.siFontNum )
			{
				m_cCurrentFrame = 0;
			}
		}
	}
*/
	return;
}