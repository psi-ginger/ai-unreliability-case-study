#include "CommonHeader.h"

#include "../Client/ninterface/NCharTransformDlg/CharTransformDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"

extern cltCommonLogic* pclClient;

#include "../NLib/NTCHARString.h"

#include "../../Client/InterfaceMgr/InterfaceMgr.h"

CNChartransformDlg::CNChartransformDlg()
{	 
	m_siCharKind = -1;   

	Animation	= ANITYPE_WAIT0;
	siDirection	= SOUTH | EAST;
	Font		= 0;
	
	m_claniinfo.Init1();

	if(false == m_Spr.LoadTSprFromFile("NInterface/image/Transform_168X144_00_000.SPR"))
	{
		MsgBox("파일열기 실패", "Transform_168X144_00_000.SPR");
	}
}

CNChartransformDlg::~CNChartransformDlg()
{
	Destroy();
}



void CNChartransformDlg::Init()
{

}

void CNChartransformDlg::Destroy()
{

}

void CNChartransformDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/CharTransformDlg/TransformCharInfoDlg.ddf"));

	file.CreatePopupDialog( this, CHARTRANSFORMINFO_DLG, TEXT("dialog_CharTransformInfo"), StaticChartransformDlgProc);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	 NCHARTRANSFORM_OUTLINE_IMAGE_LINE,   	this), NCHARTRANSFORM_OUTLINE_IMAGE_LINE,		TEXT("outline_image_line"));
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NCHARTRANSFORM_ITEM_SEALED,		  	this ), NCHARTRANSFORM_ITEM_SEALED,				TEXT("imagestatic_item_sealed"));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NCHARTRANSFORM_IMAGESTATIC_PORTRAIT, 	this),  NCHARTRANSFORM_IMAGESTATIC_PORTRAIT,	TEXT("imagestatic_PORTRAIT"));
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NCHARTRANSFORM_STATIC_NAME,			 		this), NCHARTRANSFORM_STATIC_NAME,			 	TEXT("statictext_Name"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NCHARTRANSFORM_STATIC_REMAIN_TIME,	 		this), NCHARTRANSFORM_STATIC_REMAIN_TIME,	 	TEXT("statictext_remain_time"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NCHARTRANSFORM_STATIC_SYNCHRO,		 		this), NCHARTRANSFORM_STATIC_SYNCHRO,		 	TEXT("statictext_synchro"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NCHARTRANSFORM_STATIC_STRENTH,		 		this), NCHARTRANSFORM_STATIC_STRENTH,		 	TEXT("statictext_strength"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NCHARTRANSFORM_STATIC_DEX,			 		this), NCHARTRANSFORM_STATIC_DEX,			 	TEXT("statictext_dex"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NCHARTRANSFORM_STATIC_MAGIC,		 		this), NCHARTRANSFORM_STATIC_MAGIC,				TEXT("statictext_magic"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NCHARTRANSFORM_STATIC_LIFE,			 		this), NCHARTRANSFORM_STATIC_LIFE,			 	TEXT("statictext_life"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NCHARTRANSFORM_STATIC_DEFFENSE,		 		this), NCHARTRANSFORM_STATIC_DEFFENSE,		 	TEXT("statictext_armourdefense"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NCHARTRANSFORM_STATIC_SPIRIT,		 		this), NCHARTRANSFORM_STATIC_SPIRIT,		 	TEXT("statictext_spirit"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NCHARTRANSFORM_STATIC_LUCK,			 		this), NCHARTRANSFORM_STATIC_LUCK,			 	TEXT("statictext_luck"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NCHARTRANSFORM_STATIC_DEXTERITY,	 		this), NCHARTRANSFORM_STATIC_DEXTERITY,			TEXT("statictext_dexterity"));
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_ITEM_USE,  				this ), NCHARTRANSFORM_BTN_ITEM_USE,			TEXT("button_transform_item_use"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_OK,		  				this ), NCHARTRANSFORM_BTN_OK,					TEXT("button_transform_ok"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_CANCEL,	  				this ), NCHARTRANSFORM_BTN_CANCEL,				TEXT("button_transform_cancel"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_1_DAY,	  				this ), NCHARTRANSFORM_BTN_1_DAY,				TEXT("button_1_day"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_7_DAY,	  				this ), NCHARTRANSFORM_BTN_7_DAY,				TEXT("button_7_day"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_30_DAY,	  				this ), NCHARTRANSFORM_BTN_30_DAY,				TEXT("button_30_day"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_STORY,	  				this ), NCHARTRANSFORM_BTN_STORY,				TEXT("button_char_story"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_CLOSE,	  				this ), NCHARTRANSFORM_BTN_CLOSE,				TEXT("button_close"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_LEFT,	  				this ), NCHARTRANSFORM_BTN_LEFT,				TEXT("button_left"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORM_BTN_RIGHT,	  				this ), NCHARTRANSFORM_BTN_RIGHT,				TEXT("button_right"));


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_REMAIN_TIME,				this ), NCHARTRANSFORM_EDIT_REMAIN_TIME,		TEXT("editbox_remain_time"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_SYNCHRO,					this ), NCHARTRANSFORM_EDIT_SYNCHRO,			TEXT("editbox_read"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_STRENTH,					this ), NCHARTRANSFORM_EDIT_STRENTH,			TEXT("editbox_strength"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_DEX,						this ), NCHARTRANSFORM_EDIT_DEX,				TEXT("editbox_dex"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_MAGIC,					this ), NCHARTRANSFORM_EDIT_MAGIC,				TEXT("editbox_magic"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_LIFE,						this ), NCHARTRANSFORM_EDIT_LIFE,				TEXT("editbox_life"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_DEFFENSE,					this ), NCHARTRANSFORM_EDIT_DEFFENSE,			TEXT("editbox_armourdefense"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_LUCK,						this ), NCHARTRANSFORM_EDIT_LUCK,				TEXT("editbox_luck"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_DEXTERITY,				this ), NCHARTRANSFORM_EDIT_DEXTERITY,			TEXT("editbox_dexterity"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHARTRANSFORM_EDIT_WEAPON_EFFECT,			this),  NCHARTRANSFORM_EDIT_WEAPON_EFFECT,  	TEXT("editbox_weapon_effect"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NCHARTRANSFORM_STATIC_INTELLIGENCE,			this), NCHARTRANSFORM_STATIC_INTELLIGENCE,		TEXT("statictext_int"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NCHARTRANSFORM_EDIT_INTELLIGENCE,			this), NCHARTRANSFORM_EDIT_INTELLIGENCE,		TEXT("editbox_int"));


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NCHARTRANSFORM_OUTLINE_STAT,				this ), NCHARTRANSFORM_OUTLINE_STAT,			TEXT("outline_stat_line"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	 NCHARTRANSFORM_OUTLINE_SPIRIT,			this), NCHARTRANSFORM_OUTLINE_SPIRIT,			TEXT("outline_spirit"));


	SetActionTimer( 500 );
	SetTimerOnOff( true );

	if( pclClient->pclCM->CR[1]->clTransFormInfo.uiNowTransFormKind > 0)
	{
		m_siCharKind = pclClient->pclCM->CR[1]->clTransFormInfo.uiNowTransFormKind;
	}

	cltKindInfo* pclKindInfo = NULL;
	if(-1 == m_siCharKind)
	{		
		for(int i = m_siCharKind + 1; i < MAX_KINDINFO_NUMBER; ++i)
		{
			pclKindInfo = pclClient->pclKindInfoSet->pclKI[i];
			if( NULL == pclKindInfo )  
				continue;

			if(pclKindInfo->siAtb & ATB_TRANSFORM)
			{
				m_siCharKind = i;
				break;
			}
		}
	}

	if( m_siCharKind > 0)
		pclKindInfo = pclClient->pclKindInfoSet->pclKI[m_siCharKind];

	if ( pclKindInfo == NULL )
		return;

	if ( pclKindInfo != NULL )
	{
		SI32 aniref		= pclKindInfo->clCharAniManager.FindRefFromAnitype(Animation);
		if(aniref > 0)
		{
			SI32 delay			= pclKindInfo->clCharAniManager.GetDelay(aniref);
			SI32 instant		= pclKindInfo->clCharAniManager.GetInstant(aniref);

			m_claniinfo.SetAniDelay(delay);
			m_claniinfo.SetAniInstant( instant );
		}

		SI32 ref = pclKindInfo->clCharAniManager.FindFirstAni();
		
		SI32 ImageXsize = pclKindInfo->clCharAniManager.GetImageXsize(ref, pclClient->CurrentClock);
		SI32 ImageYsize = pclKindInfo->clCharAniManager.GetImageYsize(ref, pclClient->CurrentClock);

		realxsize = pclKindInfo->clKIDrawInfo.siXsizePercent*ImageXsize/100;
		realysize = pclKindInfo->clKIDrawInfo.siYsizePercent*ImageYsize/100;

		xstart = (ImageXsize - realxsize) / 2;
		ystart = ImageYsize - pclKindInfo->clKIDrawInfo.siBottomIndent - realysize;
	}

	NTCHARString256 NTCharString(pclKindInfo->GetName());
	CStatic* pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_NAME);
	pStatic->SetText(NTCharString);

	// 스토리 버튼 일단 사용안함.
	if(m_InterfaceFactory.GetButton(NCHARTRANSFORM_BTN_STORY) != NULL)
		m_InterfaceFactory.GetButton(NCHARTRANSFORM_BTN_STORY)->Show(false);

	Status();

	m_InterfaceFactory.GetImageStatic(NCHARTRANSFORM_ITEM_SEALED)->SetAlphaValue(80);

	CEdit* pEdit  = NULL; 
	TCHAR* buffer = NULL;
	
	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_STRENTH);
	buffer = GetTxtFromMgr(7800);   //근력
	pEdit->SetUseToolTip(buffer);

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEX);
	buffer = GetTxtFromMgr(7801);	// 덱스
	pEdit->SetUseToolTip(buffer);

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_MAGIC);
	buffer = GetTxtFromMgr(7802); // 지력
	pEdit->SetUseToolTip(buffer);

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_INTELLIGENCE);
	buffer = GetTxtFromMgr(10332); // 지혜
	pEdit->SetUseToolTip(buffer);

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_LIFE);
	buffer = GetTxtFromMgr(7803); // 생명력
	pEdit->SetUseToolTip(buffer);

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_WEAPON_EFFECT);
	// 변신 중.
	if( true == pclclient->pclCM->CR[1]->clTransFormInfo.bTransFormMode )
	{
		pEdit->SetUseToolTip( GetWeaponEffectExplain() );
	}
	else
	{
		buffer = GetTxtFromMgr(7806);	// 무기효과
		pEdit->SetUseToolTip(buffer);
	}
	

	buffer = GetTxtFromMgr(7809);	// 무기 에디트 박스에 문자 넣기.
	pEdit->SetText(buffer);
	
	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEFFENSE);
	buffer = GetTxtFromMgr(7804);	// 방어력
	pEdit->SetUseToolTip(buffer);
	//pEdit->Show(false);   

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_LUCK);
	buffer = GetTxtFromMgr(7805);	// 손재주, 행운
	pEdit->SetUseToolTip(buffer);
	pEdit->Show(false);

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEXTERITY);
	buffer = GetTxtFromMgr(7805);	// 손재주, 행운
	pEdit->SetUseToolTip(buffer);
	pEdit->Show(false);
	
	pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_DEFFENSE);
	//pStatic->Show(false);

	pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_LUCK);
	pStatic->Show(false);
	
	pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_DEXTERITY);
	pStatic->Show(false);

	bool result = false;
	// 싱크로율이 100 이면.
	if(100 == (SI32)pclClient->pclCM->CR[1]->clTransFormInfo.GetSynchroTransForm(m_siCharKind))
	{
		result = true;
	}

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEFFENSE);
	buffer = GetTxtFromMgr(7804);	// 방어력
	pEdit->SetUseToolTip(buffer);
	//pEdit->Show(result); 

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_LUCK);
	buffer = GetTxtFromMgr(7805);	// 손재주, 행운
	pEdit->SetUseToolTip(buffer);
	pEdit->Show(result);

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEXTERITY); 
	buffer = GetTxtFromMgr(7805);	// 손재주, 행운
	pEdit->SetUseToolTip(buffer);
	pEdit->Show(result);

	pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_DEFFENSE);
	//pStatic->Show(result);

	pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_LUCK);
	pStatic->Show(result);

	pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_DEXTERITY);
	pStatic->Show(result);

}

void CALLBACK CNChartransformDlg::StaticChartransformDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNChartransformDlg *pThis = (CNChartransformDlg*) pControllerMgr;
	pThis->NChartransformDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNChartransformDlg::NChartransformDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case NCHARTRANSFORM_BTN_LEFT:
		{
			if(0 == m_siCharKind)	
			{
				return;
			}

			cltKindInfo* pclKindInfo = NULL;
			for(int i = m_siCharKind - 1; i >= 0; --i)
			{
				pclKindInfo = pclClient->pclKindInfoSet->pclKI[i];
				if(NULL == pclKindInfo)
					continue;

				if( pclKindInfo->siAtb & ATB_TRANSFORM )
				{
					m_siCharKind = i;
					break;
				}
			}

			if( NULL == pclKindInfo )
				return;

			if ( pclKindInfo != NULL )
			{
				SI32 aniref		= pclKindInfo->clCharAniManager.FindRefFromAnitype(Animation);
				if(aniref > 0)
				{
					SI32 delay			= pclKindInfo->clCharAniManager.GetDelay(aniref);
					SI32 instant		= pclKindInfo->clCharAniManager.GetInstant(aniref);

					m_claniinfo.SetAniDelay(delay);
					m_claniinfo.SetAniInstant( instant );
				}

				SI32 ref = pclKindInfo->clCharAniManager.FindFirstAni();

				SI32 ImageXsize = pclKindInfo->clCharAniManager.GetImageXsize(ref, pclClient->CurrentClock);
				SI32 ImageYsize = pclKindInfo->clCharAniManager.GetImageYsize(ref, pclClient->CurrentClock);

				realxsize = pclKindInfo->clKIDrawInfo.siXsizePercent*ImageXsize/100;
				realysize = pclKindInfo->clKIDrawInfo.siYsizePercent*ImageYsize/100;

				xstart = (ImageXsize - realxsize) / 2;
				ystart = ImageYsize - pclKindInfo->clKIDrawInfo.siBottomIndent - realysize;
			}

			NTCHARString256 NTCharString(pclKindInfo->GetName());
			CStatic* pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_NAME);
			pStatic->SetText(NTCharString);

			Status();
		}
		break;
	case NCHARTRANSFORM_BTN_RIGHT:
		{
			if(MAX_KINDINFO_NUMBER - 1 == m_siCharKind)	
			{
				return;
			}

			cltKindInfo* pclKindInfo = NULL;
			for(int i = m_siCharKind + 1; i < MAX_KINDINFO_NUMBER; ++i)
			{
				pclKindInfo = pclClient->pclKindInfoSet->pclKI[i];
				if( NULL == pclKindInfo )
					continue;

				if( pclKindInfo->siAtb & ATB_TRANSFORM )
				{
					m_siCharKind = i;
					break;
				}
			}
			
			if( (NULL == pclKindInfo) || !(pclKindInfo->siAtb & ATB_TRANSFORM) )
			{
				return;
			}

			if ( pclKindInfo != NULL )
			{
				SI32 aniref		= pclKindInfo->clCharAniManager.FindRefFromAnitype(Animation);
				if(aniref > 0)
				{
					SI32 delay			= pclKindInfo->clCharAniManager.GetDelay(aniref);
					SI32 instant		= pclKindInfo->clCharAniManager.GetInstant(aniref);

					m_claniinfo.SetAniDelay(delay);
					m_claniinfo.SetAniInstant( instant );
				}

				SI32 ref = pclKindInfo->clCharAniManager.FindFirstAni();

				SI32 ImageXsize = pclKindInfo->clCharAniManager.GetImageXsize(ref, pclClient->CurrentClock);
				SI32 ImageYsize = pclKindInfo->clCharAniManager.GetImageYsize(ref, pclClient->CurrentClock);

				realxsize = pclKindInfo->clKIDrawInfo.siXsizePercent*ImageXsize/100;
				realysize = pclKindInfo->clKIDrawInfo.siYsizePercent*ImageYsize/100;

				xstart = (ImageXsize - realxsize) / 2;
				ystart = ImageYsize - pclKindInfo->clKIDrawInfo.siBottomIndent - realysize;
			}

			NTCHARString256 NTCharString(pclKindInfo->GetName());
			CStatic* pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_NAME);
			pStatic->SetText(NTCharString);

			Status();
		}
		break;
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;	
	case NCHARTRANSFORM_BTN_ITEM_USE:	// 변신 아이템 사용.
		{
			SI32 itemunique = 0;
			
			SI32 Slot = GetFindCharSlot(m_siCharKind);
			/*if(Slot >= 0)
			{
				if(pclclient->pclCM->CR[1]->clTransFormInfo.clTransForm[Slot].uiCanTransFormKind)
				{
					TCHAR* pTitle = GetTxtFromMgr(5614);
					TCHAR* pText  = GetTxtFromMgr(7771);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return;
				}
			}
			if(Slot < 0) return;*/

			SI32 pos = -1;
			itemunique = pclclient->pclCM->CR[1]->GetItemUniqueFromCharKind( m_siCharKind );
			if(itemunique)	pos = pclclient->pclCM->CR[1]->pclCI->clCharItem.FindItemInv(itemunique);

			if(pos < 0)
			{
				TCHAR* pTitle = GetTxtFromMgr(5614);
				TCHAR* pText  = GetTxtFromMgr(7772);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return;	
			}

			stMsgBox MsgBox;
			TCHAR* pTitle = GetTxtFromMgr(5614);
			TCHAR* pText  = GetTxtFromMgr(7773);
			MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, RETMSG_USE_ITEM );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );
		}
		break;
	case NCHARTRANSFORM_BTN_OK:			// 변신하기.
		{
			if( false == pclClient->pclCM->IsAlive(1) )
			{
				return;
			}
			
			// 이미 변신 중이면.
			if(true == pclclient->pclCM->CR[1]->clTransFormInfo.bTransFormMode)
			{
				TCHAR* pTitle = GetTxtFromMgr(5614);
				TCHAR* pText  = GetTxtFromMgr(7793);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return;
			}

			SI32 Slot = GetFindCharSlot(m_siCharKind);
			if(Slot < 0)
			{
				return;
			}

			// 변신에 필요한 시간 확인
			if( pclClient->pclCM->CR[1]->pclCI->clBI.clTimeMeterRateInfo.GetLeftTime( TIMEMETERRATE_TRANSFORM ) <= 0)
			{
				TCHAR* pTitle = GetTxtFromMgr(5614);
				TCHAR* pText  = GetTxtFromMgr(7780);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return;	
			}


			cltTransFormInfo* pTransformInfo = &pclClient->pclCM->CR[1]->clTransFormInfo;
			if(m_siCharKind == pTransformInfo->clTransForm[Slot].uiCanTransFormKind)
			{
				bool Result = pclClient->pclCM->CR[1]->CanTransFormAttackType( m_siCharKind );

				if(Result)
				{
					stMsgBox MsgBox;
					TCHAR* pTitle = GetTxtFromMgr(5614);
					TCHAR* pText  = GetTxtFromMgr(7774);
					MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, TRANSFORM );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				else
				{
					TCHAR* pTitle = GetTxtFromMgr(5614);
					TCHAR* pText  = GetTxtFromMgr(7779);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return;	
				}
			}
			else
			{
				TCHAR* pTitle = GetTxtFromMgr(5614);
				TCHAR* pText  = GetTxtFromMgr(7775);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return;	
			}	
		}
		break;
	case NCHARTRANSFORM_BTN_CANCEL:		// 변신해제.
		{
			// 변신 상태가 아니면 리턴.
			if(false == pclclient->pclCM->CR[1]->clTransFormInfo.bTransFormMode)
			{
				return;
			}

			// 변신이 30 초가 안되면 보여줄 메시지.
			//TCHAR* pText  = GetTxtFromMgr(7778);

			SI32 NowTransformkind = pclclient->pclCM->CR[1]->clTransFormInfo.uiNowTransFormKind;

			//if(pclClient->pclCM->CR[1]->siEnemUnique > 0 )
			//{
			//	TCHAR* pTitle = GetTxtFromMgr(512);
			//	TCHAR* pText  = GetTxtFromMgr(9575);
			//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			//	return;	

			//}
			cltGameMsgRequest_SetTransForm clinfo(NowTransformkind);
			cltMsg clMsg( GAMEMSG_REQUEST_TRANSFORM_END,sizeof(clinfo), (BYTE*)&clinfo );
			pclclient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );	
		}
		break;
	case NCHARTRANSFORM_BTN_1_DAY:	// 기한연장.
		{
			SI32 itemunique = ITEMUNIQUE(5410);
			SI32 pos = pclclient->pclCM->CR[1]->pclCI->clCharItem.FindItemInv(itemunique);
			if(pos < 0)
			{
				TCHAR* pTitle = GetTxtFromMgr(5614);
				TCHAR* pText  = GetTxtFromMgr(7776);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return;	
			}

			stMsgBox MsgBox;
			TCHAR* pTitle = GetTxtFromMgr(5614);
			TCHAR* pText  = GetTxtFromMgr(7777);
			MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, RETMSG_ADD_1_DAY );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );
		}	
		break;
	case NCHARTRANSFORM_BTN_7_DAY:	// 기한연장.
		{
			SI32 itemunique = ITEMUNIQUE(5411);
			SI32 pos = pclclient->pclCM->CR[1]->pclCI->clCharItem.FindItemInv(itemunique);
			if(pos < 0)
			{
				TCHAR* pTitle = GetTxtFromMgr(5614);
				TCHAR* pText  = GetTxtFromMgr(7776);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return;	
			}

			stMsgBox MsgBox;
			TCHAR* pTitle = GetTxtFromMgr(5614);
			TCHAR* pText  = GetTxtFromMgr(7777);
			MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, RETMSG_ADD_7_DAY);
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );
		}	
		break;
	case NCHARTRANSFORM_BTN_30_DAY:	// 기한연장.
		{
			SI32 itemunique = ITEMUNIQUE(5412);
			SI32 pos = pclclient->pclCM->CR[1]->pclCI->clCharItem.FindItemInv(itemunique);
			if(pos < 0)
			{
				TCHAR* pTitle = GetTxtFromMgr(5614);
				TCHAR* pText  = GetTxtFromMgr(7776);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return;	
			}

			stMsgBox MsgBox;
			TCHAR* pTitle = GetTxtFromMgr(5614);
			TCHAR* pText  = GetTxtFromMgr(7777);
			MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, RETMSG_ADD_30_DAY);
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );
		}	
		break;
	case NCHARTRANSFORM_BTN_STORY:		// 스토리.
		{
			//[보류 : 황진성 2008. 1. 2 스토리창 보류.]
			pclclient->CreateInterface(CHARTRANSFORMINFO_STORY_DLG);
		}
		break;
	case NCHARTRANSFORM_BTN_CLOSE:		// 닫기.
		{
			DeleteDialog();
		}
		break;
	default:
		{

		}
		break;
	}
}


void CNChartransformDlg::Action()
{
	if(IsShow())
	{		
		CEdit* pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_REMAIN_TIME);
		NTCHARString256	kBuf(GetTxtFromMgr(7795));

		cltTimeMeterRate* pclTimeMeterRate = pclClient->pclCM->CR[1]->pclCI->clBI.clTimeMeterRateInfo.GetTimeMeterRate( TIMEMETERRATE_TRANSFORM );
		if(pclTimeMeterRate != NULL)
		{
			SI32 siMin;
			SI32 siTemp = pclTimeMeterRate->siLeftTimeSec;
			siMin = siTemp / 60;
			kBuf.Replace("#value#", SI32ToString(siMin));
		}
		else
		{
			kBuf.Replace("#value#", SI32ToString(0));
		}

		pEdit->SetText(kBuf);

		cltTransFormInfo* pTransformInfo = &pclClient->pclCM->CR[1]->clTransFormInfo;
		REAL32			  siTransFormSyc =  pTransformInfo->GetSynchroTransForm(m_siCharKind);

		NTCHARStringBuilder kBuilder;
		NTCHARString32	kSynchro;
		kBuilder.SetFormat("{1} %") << (const TCHAR*)REAL32ToString(siTransFormSyc, 6, 2);
		pEdit	 = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_SYNCHRO);
		kSynchro = kBuilder;

		pEdit->SetText(kSynchro);

		// 능력치 적용
		Status();
	}
}

void CNChartransformDlg::DrawChar( SI32 ScreenRX, SI32 ScreenRY )
{
	if(m_siCharKind <= 0)		return;

	cltKindInfo* pclKI = pclClient->pclKindInfoSet->pclKI[m_siCharKind];
	if(pclKI == NULL)			return;

	SI32	delay;
	SI32	framenumber;

	SI32 aniref	= pclKI->clCharAniManager.FindRefFromAnitype(Animation);
	framenumber = pclKI->clCharAniManager.GetFrameNum(aniref);

	if ( ((cltClient*)pclClient)->GetFrameDelay() == 25 )
	{
		delay = m_claniinfo.siAniDelay * 2;
	}
	else
	{
		delay = m_claniinfo.siAniDelay;
	}

	m_claniinfo.IncreaseAniDelayor(1);

	if(m_claniinfo.GetAniDelayor() >= delay)
	{
		m_claniinfo.SetAniDelayor(0);

		m_claniinfo.IncreaseAniStep(1);

		if(framenumber == 0)
		{
			m_claniinfo.SetAniStep(0);
		}
		else
		{
			m_claniinfo.SetAniStep(m_claniinfo.GetAniStep() % framenumber );
		}
	}

	TSpr* pSpr = NULL ;
	bool reverse = false ;
	SI32 effect = 0 ;

	pclKI->clCharAniManager.DecideDrawImg(Animation, siDirection, m_claniinfo, TABS(pclClient->CurrentClock), &pSpr, &Font, &reverse, &effect );
	
	//pclKI->clKIDrawInfo.siXsizePercent;
	
	CImageStatic* pImageStatic = NULL;
	pImageStatic = m_InterfaceFactory.GetImageStatic(NCHARTRANSFORM_IMAGESTATIC_PORTRAIT);
	SI32 siX = pImageStatic->GetX() + (pImageStatic->GetWidth() / 2 - realxsize / 2);
	SI32 siY = pImageStatic->GetY() + (pImageStatic->GetHeight() / 2 - realysize / 2);

	GP.PutSpr(pSpr, GetX() + siX - xstart + ScreenRX , GetY() + siY - ystart + ScreenRY , Font, reverse );

	bool	bCanTransform = false;
	for (int i=0; i<MAX_TRANSFORM_CHAR; i++)
	{
		cltTransFormInfo* pTransformInfo = &pclClient->pclCM->CR[1]->clTransFormInfo;
		if( pTransformInfo->clTransForm[i].uiCanTransFormKind == m_siCharKind )
		{
			bCanTransform = true;
			break;
		}
	}
	
	if( !bCanTransform )
	{
		GP.PutSprAlpha(&m_Spr, GetX() + ScreenRX +15, GetY() + ScreenRY + 25, 12, Font, reverse );
	}
}

void CNChartransformDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	DrawChar( ScreenRX, ScreenRY );
}

void CNChartransformDlg::Status(void)
{
	if( m_siCharKind <= 0 || m_siCharKind >= MAX_KINDINFO_NUMBER)		return;

	cltTransFormInfo* pTransformInfo = &pclClient->pclCM->CR[1]->clTransFormInfo;

	cltBasicAbility	clTransFormBA;
	clTransFormBA.Init();

	// 현재 사용중인 무기 타입 얻기.
	SI32 itemtype = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].GetType(pclClient->pclItemManager);

    cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[m_siCharKind];
	if(pclki == NULL)		return;

	// 해당 변신 캐릭터의 기본 능력 설정.
	clTransFormBA.Set(&pclki->clBA);
	
	//싱크로율에 따라 추가 효과를 더한다.
	SI32 temState		= 0;
	SI32 siTransFormSyc = (SI32)pTransformInfo->GetSynchroTransForm(m_siCharKind);
	TCHAR		buf[128] = "";
	CEdit*		pEdit    = NULL;

	// 민첩 증가.
	temState = clTransFormBA.GetDex() * (siTransFormSyc/2) / 100;

	pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEX);
	StringCchPrintf(buf, sizeof(buf), TEXT("%d (%d+%d)"), clTransFormBA.GetDex() + temState, clTransFormBA.GetDex(), temState);
	pEdit->SetText(buf);

	clTransFormBA.IncreaseDex(temState);
	//---------------------------------------------------------------------------------------------------------------------------//
	// 힘 증가.
	temState = clTransFormBA.GetStr() * (siTransFormSyc/2) / 100;

	pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_STRENTH);
	StringCchPrintf(buf, sizeof(buf), TEXT("%d (%d+%d)"), clTransFormBA.GetStr() + temState, clTransFormBA.GetStr(), temState);
	pEdit->SetText(buf);

	clTransFormBA.IncreaseStr(temState);
	//---------------------------------------------------------------------------------------------------------------------------//
	// 체력 증가.
	temState = clTransFormBA.GetVit() * (siTransFormSyc/2) / 100;

	pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_LIFE);
	StringCchPrintf(buf, sizeof(buf), TEXT("%d (%d+%d)"), clTransFormBA.GetVit() + temState, clTransFormBA.GetVit(), temState);
	pEdit->SetText(buf);

	clTransFormBA.IncreaseVit(temState);
	//---------------------------------------------------------------------------------------------------------------------------//
	// 지력 증가.
	temState = clTransFormBA.GetMag() * (siTransFormSyc/2) / 100;

	// 지팡이  = 지력 싱크로 %가 더 해진다. 
	if(itemtype == ITEMTYPE_STAFF)
		temState += (SI32)(siTransFormSyc/5);

	pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_MAGIC);
	StringCchPrintf(buf, sizeof(buf), TEXT("%d (%d+%d)"), clTransFormBA.GetMag() + temState, clTransFormBA.GetMag(), temState);
	pEdit->SetText(buf);

	clTransFormBA.IncreaseMag(temState);
	//---------------------------------------------------------------------------------------------------------------------------//

	// 지혜 증가.
	temState = clTransFormBA.GetWis() * (siTransFormSyc/2) / 100;

	pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_INTELLIGENCE);
	StringCchPrintf(buf, sizeof(buf), TEXT("%d (%d+%d)"), clTransFormBA.GetWis() + temState, clTransFormBA.GetWis(), temState);
	pEdit->SetText(buf);
	clTransFormBA.IncreaseWis(temState);

	//---------------------------------------------------------------------------------------------------------------------------//

	// 방어력 증가.
	SI32 siTransFormAC = 0; // 기본.
	
	if( pclClient->GetUniqueFromHash("KIND_NINJAMAN")  == m_siCharKind  		|| 
		pclClient->GetUniqueFromHash("KIND_NINJAGIRL") == m_siCharKind			||
		pclClient->GetUniqueFromHash("KIND_CATMELEE")  == m_siCharKind			||
		pclClient->GetUniqueFromHash("KIND_CATRANGE")  == m_siCharKind			||
		pclClient->GetUniqueFromHash("KIND_HALLOWEENPUMPKIN")  == m_siCharKind	||
		pclClient->GetUniqueFromHash("KIND_HALLOWEENGHOST")  == m_siCharKind	||
		pclClient->GetUniqueFromHash("KIND_CHRISTMASMELEE")  == m_siCharKind	||
		pclClient->GetUniqueFromHash("KIND_CHRISTMASRANGE")  == m_siCharKind	||
		pclClient->GetUniqueFromHash("KIND_FIGHTINGMENMELEE")     == m_siCharKind	||	// 검사스피릿(여)
		pclClient->GetUniqueFromHash("KIND_FIGHTINGMENRANGE")     == m_siCharKind	||	// 거너스피릿(남)
		pclClient->GetUniqueFromHash("KIND_FAIRYMELEE")     == m_siCharKind			||	// 요정스피릿(남)
		pclClient->GetUniqueFromHash("KIND_FAIRYRANGE")     == m_siCharKind			||	// 요정스피릿(여)
		// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
		pclClient->GetUniqueFromHash("KIND_ICERANGE")     == m_siCharKind			||	// 눈의 여왕
		pclClient->GetUniqueFromHash("KIND_ICEMELEE")     == m_siCharKind				// 사스콰치
	   )		
	{
		siTransFormAC = 80; // 닌자 방어력.
	}
	
	temState	  = siTransFormAC * (siTransFormSyc/2) / 100;
			
	pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEFFENSE);
	StringCchPrintf(buf, sizeof(buf), TEXT("%d (%d+%d)"), siTransFormAC + temState, siTransFormAC, temState);
	pEdit->SetText(buf);

	SI32 HandAndLuck = 0;
	if(siTransFormSyc >= 100) // 싱크로율 100% 보너스. - 10 고정.
	{
		// [추가 : 황진성 2008. 1. 24 => 가챠 캐릭터인 닌자 효과 주기.]
		if( pclClient->GetUniqueFromHash("KIND_NINJAMAN")  == m_siCharKind  		|| 
			pclClient->GetUniqueFromHash("KIND_NINJAGIRL") == m_siCharKind			||	
			pclClient->GetUniqueFromHash("KIND_CATMELEE")  == m_siCharKind			||	
			pclClient->GetUniqueFromHash("KIND_CATRANGE")  == m_siCharKind			||
			pclClient->GetUniqueFromHash("KIND_HALLOWEENPUMPKIN")  == m_siCharKind	||
			pclClient->GetUniqueFromHash("KIND_HALLOWEENGHOST")    == m_siCharKind	||
			pclClient->GetUniqueFromHash("KIND_CHRISTMASMELEE")    == m_siCharKind	||
			pclClient->GetUniqueFromHash("KIND_CHRISTMASRANGE")    == m_siCharKind	||
			pclClient->GetUniqueFromHash("KIND_FIGHTINGMENMELEE")  == m_siCharKind	||	// 검사스피릿(여)
			pclClient->GetUniqueFromHash("KIND_FIGHTINGMENRANGE")  == m_siCharKind	||	// 거너스피릿(남)
			pclClient->GetUniqueFromHash("KIND_FAIRYMELEE")  == m_siCharKind		||	// 요정스피릿(남)
			pclClient->GetUniqueFromHash("KIND_FAIRYRANGE")  == m_siCharKind		||	// 요정스피릿(여)
			// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
			pclClient->GetUniqueFromHash("KIND_ICERANGE")  == m_siCharKind		||	// 눈의 여왕
			pclClient->GetUniqueFromHash("KIND_ICEMELEE")  == m_siCharKind			// 사스콰치
		   )	
		{
			HandAndLuck = 15;
			clTransFormBA.IncreaseHnd(HandAndLuck);
			clTransFormBA.IncreaseLuk(HandAndLuck);
		}
		else
		{
			HandAndLuck = 10;
			clTransFormBA.IncreaseHnd(HandAndLuck);
			clTransFormBA.IncreaseLuk(HandAndLuck);
		}
	}
	
	//---------------------------------------------------------------------------------------------------------------------------//
	if(0 != HandAndLuck)
	{
		// 손재주 증가.
		pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEXTERITY);
		pEdit->Show(true);
		pEdit->SetText( SI32ToString( HandAndLuck ) );

		//clTransFormBA.IncreaseHnd(10);
		////---------------------------------------------------------------------------------------------------------------------------//

		//  행운.
		pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_LUCK);
		pEdit->Show(true);
		pEdit->SetText( SI32ToString( HandAndLuck ) );
		
		//clTransFormBA.IncreaseLuk(10);
	}
	else
	{
		pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEXTERITY);
		pEdit->Show(false);

		pEdit = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_LUCK);
		pEdit->Show(false);    
	}

	bool result = false;
	// 싱크로율이 100 이면.
	if(100 == (SI32)pclClient->pclCM->CR[1]->clTransFormInfo.GetSynchroTransForm(m_siCharKind))
	{
		result = true;
	}

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_LUCK);
	pEdit->Show(result); 

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_DEXTERITY);
	pEdit->Show(result);
	
	CStatic* pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_LUCK);
	pStatic->Show(result);

	pStatic = m_InterfaceFactory.GetStatic(NCHARTRANSFORM_STATIC_DEXTERITY);
	pStatic->Show(result);

	pEdit  = m_InterfaceFactory.GetEdit(NCHARTRANSFORM_EDIT_WEAPON_EFFECT);
	// 변신 중.
	if( true == pclClient->pclCM->CR[1]->clTransFormInfo.bTransFormMode )
	{
		pEdit->SetUseToolTip( GetWeaponEffectExplain() );
	}
	else
	{
		pEdit->SetUseToolTip(GetTxtFromMgr(7806));
	}
}

SI32 CNChartransformDlg::GetFindCharSlot(SI32 _kind)
{
	for (int i=0; i<MAX_TRANSFORM_CHAR; i++)
	{
		cltTransFormInfo* pTransformInfo = &pclClient->pclCM->CR[1]->clTransFormInfo;
		if( pTransformInfo->clTransForm[i].uiCanTransFormKind == _kind )
		{
			return i;
			break;
		}
	}

	return -1;
}

void CNChartransformDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	if(false == RetValue)
	{
		return;
	}

	switch( MsgIndex )
	{
		case RETMSG_USE_ITEM:
			{
				cltClient* pclclient	= (cltClient *)pclClient;
				SI32	   siItemUnique	=  pclclient->pclCM->CR[1]->GetItemUniqueFromCharKind( m_siCharKind );
				if( siItemUnique == 0 )				return;

				cltGameMsgRequest_UseTransFormItem clinfo( siItemUnique );
				cltMsg clMsg( GAMEMSG_REQUEST_TRANSFORM_USEITEM , sizeof(clinfo), (BYTE*)&clinfo );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );	
			}
			break;
	
		case RETMSG_ADD_1_DAY:	
			{		
				cltGameMsgRequest_UseTransFormItem clinfo(ITEMUNIQUE(5410));
				cltMsg clMsg( GAMEMSG_REQUEST_TRANSFORM_USEITEM , sizeof(clinfo), (BYTE*)&clinfo );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );	
			}
			break;
		case RETMSG_ADD_7_DAY:	
			{		
				cltGameMsgRequest_UseTransFormItem clinfo(ITEMUNIQUE(5411));
				cltMsg clMsg( GAMEMSG_REQUEST_TRANSFORM_USEITEM , sizeof(clinfo), (BYTE*)&clinfo );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );	
			}
			break;
		case RETMSG_ADD_30_DAY:	
			{		
				cltGameMsgRequest_UseTransFormItem clinfo(ITEMUNIQUE(5412));
				cltMsg clMsg( GAMEMSG_REQUEST_TRANSFORM_USEITEM , sizeof(clinfo), (BYTE*)&clinfo );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );	
			}
			break;
		case TRANSFORM:
			{
				if( false == pclClient->pclCM->IsAlive(1) )
				{
					return;
				}

				cltClient* pclclient = (cltClient*)pclClient;

				// 변신에 필요한 시간 확인
				if( pclClient->pclCM->CR[1]->pclCI->clBI.clTimeMeterRateInfo.GetLeftTime( TIMEMETERRATE_TRANSFORM ) <= 0)
				{
					TCHAR* pTitle = GetTxtFromMgr(5614);
					TCHAR* pText  = GetTxtFromMgr(7780);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return;	
				}

				bool Result = pclClient->pclCM->CR[1]->CanTransFormAttackType( m_siCharKind );
				//// 전투중에는 변신 할수 없음 
				//if(pclClient->pclCM->CR[1]->siEnemUnique > 0 )
				//{
				//	TCHAR* pTitle = GetTxtFromMgr(512);
				//	TCHAR* pText  = GetTxtFromMgr(9575);
				//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				//	return;	

				//}

				if(Result)
				{
					cltGameMsgRequest_SetTransForm clinfo(m_siCharKind); 
					cltMsg clMsg( GAMEMSG_REQUEST_TRANSFORM, sizeof(clinfo), (BYTE*)&clinfo );
					pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );	
				}
				else
				{
					TCHAR* pTitle = GetTxtFromMgr(5614);
					TCHAR* pText  = GetTxtFromMgr(7779);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return;	
				}
			}
			break;
	}
}

void CNChartransformDlg::Msg(SI32 MsgNum)
{
	switch(MsgNum)
	{
		default:
			{

			}
	}
}


void CNChartransformDlg::SetStatus(CStatus& _Status)
{
	
}

TCHAR* CNChartransformDlg::GetWeaponEffectExplain( void )
{
	static NTCHARString256 szWeaponEffectExplain;
	szWeaponEffectExplain = TEXT("");

	switch( pclClient->pclCM->CR[1]->GetAttackType() )
	{
	case ATTACKTYPE_SWORD:
		{
			szWeaponEffectExplain  = GetTxtFromMgr(40306);
			szWeaponEffectExplain += GetTxtFromMgr(40307);
		}
		break;
	case ATTACKTYPE_SPEAR:
		{
			szWeaponEffectExplain  = GetTxtFromMgr(40310);
			szWeaponEffectExplain += GetTxtFromMgr(40307);
		}
		break;						
	case ATTACKTYPE_AXE:
		{
			szWeaponEffectExplain  = GetTxtFromMgr(40308);
			szWeaponEffectExplain += GetTxtFromMgr(40309);			
		}
		break;						
	case ATTACKTYPE_STAFF:
	case ATTACKTYPE_STAFF1:						
		{
			szWeaponEffectExplain  = GetTxtFromMgr(40311);
			szWeaponEffectExplain += GetTxtFromMgr(40312);
		}
		break;
	case ATTACKTYPE_BOW:
		{
			szWeaponEffectExplain  = GetTxtFromMgr(40310);
			szWeaponEffectExplain += GetTxtFromMgr(40313);
		}
		break;
	case ATTACKTYPE_GUN:							
		{
			szWeaponEffectExplain  = GetTxtFromMgr(40308);
			szWeaponEffectExplain += GetTxtFromMgr(40313);
		}
		break;
	}

	return szWeaponEffectExplain;
}



