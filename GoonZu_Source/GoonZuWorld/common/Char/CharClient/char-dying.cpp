#include "..\..\Common\CommonHeader.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Client\Client.h"

#include "..\..\Client\Music\Music.h"
#include "..\..\Order\Order.h"

#include "Bullet\Bullet.h"
#include "Smoke\Smoke.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

extern cltCommonLogic* pclClient;

//----------------------------------------
// 죽는 행위 
//----------------------------------------
bool cltCharClient::Action_Dying()
{

	cltCharCommon::Action_Dying();

	//-----------------------------
	// 사망시 한번만 이루어져야 할 행위 
	//-----------------------------
	if(bDyingOperationSwitch == false)
	{
		bDyingOperationSwitch = true;
		TCHAR *pTitle =GetTxtFromMgr(5612);
		TCHAR *pText = GetTxtFromMgr(5613);
	
		cltClient *pclclient = (cltClient*)pclClient;
		if ( pclclient->pclCM->CR[1]->pclCI->clIP.GetLevel() < INIT_LEVEL )
		{
			SI32 mapindex = pclclient->pclCM->CR[1]->GetMapIndex()	;
			if( false == IsGuildWarMap_NEW( mapindex) && false == IsGuildWarMap_Use_New_GuildWar(mapindex ) )	// 길드맵이 아닐때만 나온다
			{
				if(pclclient->IsUser(GetCharUnique()))
				{
					(pclclient->m_pDialog[ NOTIFYMSG_DLG ])->SetText( pTitle, pText);
				}
			}			
		}
		// by LEEKH 2007.12.24 - TRANSFORM
		// 비명 소리. 
		if(GetKindInfo()->clKIEffectInfo.siDyingEffect)
		{
			pclclient->PushEffect(GetKindInfo()->clKIEffectInfo.siDyingEffect, GetX(), GetY());
		}

		// Clone은 죽을떄 일종의 펑 연기를 만든다.
		if(IsAtb(ATB_CLONE) && clInnerStatus.IsStatus(INNERSTATUS_CLONE) == true)
		{
			if(GetKind()==pclClient->GetUniqueFromHash( TEXT("KIND_JAR")) )	
				pclClient->pclSmokeManager->SetSmoke(SMOKE_JARSMOKE, siCenterX, siCenterY, pclMap);
		}

		// [영훈] 기승동물 죽는 사운드 추가 (2008.06.18)

		cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();

		// 기승동물을 보유하고 있고, 올라탄 상태에서만 사운드가 난다
		if ( (NULL != pclHorse) && (true == pclCI->clHorseInfo.siCurrentHorseRIDEHORSE) )
		{
			SI32 siSoundUnique = 0;

			// 각 기승동물별로 사운드 하드코딩
			switch ( pclHorse->siHorseUnique )
			{
				case HORSEUNIQUE_ELEPHANT:
					{
						siSoundUnique = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_ELEPHANT_DIE"));
					}
					break;
			}

			// 죽는 사운드가 있을때만 연주한다
			if ( siSoundUnique )
			{
				pclclient->PushEffect( siSoundUnique, GetX(), GetY() );
			}
		}
	}

	SetAnimation(ANITYPE_DYING);
	if(DoAnimationDelay(ANIATB_ONLYONCE) == TRUE)
	{
		// 애니메이션이 다 끝나면, 
		if(clAniInfo.IsLast() )
		{
			if(bGhostShowSwitch == false)  
			{
				bGhostShowSwitch = true;
				
				// 혼령이 올라가는 모습을 만든다. 
				//if((GetKind () != pclClient->GetUniqueFromHash(TEXT("KIND_TOYO1")) ) && (clInnerStatus.IsStatus(INNERSTATUS_CLONE) == false))
				//{
				//	pclClient->pclSmokeManager->SetSmoke(SMOKE_GHOST, siCenterX, siCenterY, pclMap);
				//}
				
				// 배경음악을 중단하고 사망곡을 연주한다. 
				cltClient* pclclient = (cltClient*)pclClient;
				
				// 주인공이 죽었다면, 
				if(pclclient->IsUser(GetCharUnique()))
				{
					pclclient->pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_HeroDie1.ogg"), false, false );

				}
				return TRUE;          
			}
		}
	}


    return FALSE;
}



