#include "CushionEffect.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"
#include "CommonLogic.h"
#include "../../../common/char/KindInfo/kindinfo.h"

CCushionEffect::CCushionEffect(TSpr *pEffect1 , TSpr *pEffect2 , TSpr * pEffect3 , TSpr * pEffect4, TSpr * pEffect5, TSpr * pEffect6, TSpr * pEffect7, TSpr * pEffect8 )
{
	m_pEffectTSPR[0] = pEffect1;
	m_pEffectTSPR[1] = pEffect2;
	m_pEffectTSPR[2] = pEffect3;
	m_pEffectTSPR[3] = pEffect4;
	m_pEffectTSPR[4] = pEffect5;
	m_pEffectTSPR[5] = pEffect6;
	m_pEffectTSPR[6] = pEffect7;
	m_pEffectTSPR[7] = pEffect8;


	m_siDelayFrame = 3;

	m_bEffect = false;

	m_siCurrentFrame = 0;

	m_Clock  = 0;

	m_siSelectedSpr = 0;
	m_siSelectedFont = 0;
	m_bReverse = false;


	m_siAdjustX = 0;
	m_siAdjustY = 0;

}
CCushionEffect::~CCushionEffect()
{

}

void CCushionEffect::Action( SI32 bEffect , SI32 charkind , SI32 direction )	// 이펙트를 그려야 할지 말지를 인자로 받음
{
	//KHY - 1109 - 가챠레어- 강아지방석추가
	m_bEffect = bEffect; //그려져야 하는 아이템 유니트 값이 들어 온다.

	if( m_bEffect == false ) return; // 그려지지 않는 상황이라면 엑션 없다.

	m_siAdjustX = 0;
	m_siAdjustY = 0;

	switch( m_bEffect )
	{
		case ITEMUNIQUE(13608): // 검은고양이  - 서양남
			m_siSelectedSpr = 0;
			break;
		case ITEMUNIQUE(13609): // 페르시안 고양이 - 서양여
			{
				m_siSelectedSpr = 1;
				m_siAdjustY = - 20;
			}
			break;
		case ITEMUNIQUE(13610): // 갈색고양이 - 동양남
			m_siSelectedSpr = 2;
			break;
		case ITEMUNIQUE(13611): //하얀고양이 - 동양여
			m_siSelectedSpr = 3;
			break;
		case ITEMUNIQUE(13617): // 닥스훈트 - 동양남
			{
				m_siSelectedSpr = 4; 
				m_siAdjustY = - 22;
			}
			break;
		case ITEMUNIQUE(13618): // 시츄 - 서양남
			{
				m_siSelectedSpr = 5;
				m_siAdjustY = - 3;
			}
			break;
		case ITEMUNIQUE(13619): // 토이푸들 - 동양여
			{
				m_siSelectedSpr = 6;
				m_siAdjustY = - 4;
			}
			break;
		case ITEMUNIQUE(13620): // 시베리안 허스키 - 서양여
			{
				m_siSelectedSpr = 7;
				m_siAdjustY = - 27;
			}
			break;
		default:
			m_bEffect = 0;
			break;
	}

	// 이불류는 그리지 않기 위해서.
	if(m_bEffect ==0) return;

	switch(direction)
	{
		case SOUTH:			
			{
				m_siSelectedFont = 1;
				m_bReverse = false;
			}
			break;
		case SOUTH|WEST:	
			{
				
				m_siSelectedFont = 2;
				m_bReverse = false;
			}
			break;
		case WEST:
			{
				
				m_siSelectedFont = 3;
				m_bReverse = false;
			}
			break;
		case NORTH|WEST:
			{
				
				m_siSelectedFont = 4;
				m_bReverse = false;
			}
			break;
		case NORTH:	
			{
				
				m_siSelectedFont = 3;
				m_bReverse = true;
			}
			break;
		case NORTH|EAST:
			{
				
				m_siSelectedFont = 2;
				m_bReverse = true;
			}
			break;
		case EAST:	
			{
				
				m_siSelectedFont = 1;
				m_bReverse = true;
			}
			break;
		case SOUTH|EAST:
			{
				
				m_siSelectedFont = 0;
				m_bReverse = false;
			}
			break;
		default:	
			{
				m_siSelectedFont = 0;
				m_bReverse = false;
			}
			break;
	}
	

}

void CCushionEffect::Draw( SI32 si32CenterXPos, SI32 si32CenterYPos )
{
   	if ( m_bEffect == false )	return; // 그려지지 않는 상황이라면  그리기 없다.

	if( m_siSelectedSpr < 0 || m_siSelectedSpr > 7 ) return;
	

	SI32 XPos = si32CenterXPos - m_pEffectTSPR[m_siSelectedSpr]->GetXSize() / 2;
	SI32 YPos = si32CenterYPos - m_pEffectTSPR[m_siSelectedSpr]->GetYSize() / 2 - 35;

	/*
	if( m_bReverse == false )
	{
		GP.PutSpr(m_pEffectTSPR[m_siSelectedSpr], XPos, YPos, m_siSelectedFont );    
	}
	else
	{
		GP.PutSpr(m_pEffectTSPR[m_siSelectedSpr], XPos, YPos, m_siSelectedFont , true );    
	}*/

	GP.PutSpr(m_pEffectTSPR[m_siSelectedSpr], XPos + m_siAdjustX, YPos  + m_siAdjustY, 0 );    
	
}


