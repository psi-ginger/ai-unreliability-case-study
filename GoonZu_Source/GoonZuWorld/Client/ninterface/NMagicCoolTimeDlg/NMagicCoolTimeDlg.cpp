#include "NMagicCoolTimeDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../Client.h"

#include "../../../CommonLogic/MagicMgr/MagicMgr.h"
#include "../../../CommonLogic/MagicMgr/MagicAttribute/MagicAttribute.h"
#include "../../../common/Char/CharCommon/Char-Common.h"
#include "../../../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

CNMagicCoolTimeDlg::CNMagicCoolTimeDlg() : CDialogController()
{
	for ( SI32 siIndex=0; siIndex<MCT_ARRAY_MAX; ++siIndex )
	{
		m_pctrlStatic[siIndex] = NULL;
	}

	m_siActiveMagicCoolTime			= 0;
	m_siActiveMagicCoolTimeStart	= 0;
}

CNMagicCoolTimeDlg::~CNMagicCoolTimeDlg()
{
	for ( SI32 siIndex=0; siIndex<MCT_ARRAY_MAX; ++siIndex )
	{
		SAFE_DELETE( m_pctrlStatic[siIndex] );
	}
}

void CNMagicCoolTimeDlg::Create()
{
	if ( IsCreate() )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NMagicCoolTimeDlg/DLG_NMagicCoolTime.ddf"));
	file.CreatePopupDialog( this, NMAGICCOOLTIME_DLG, TEXT("dialog_magiccooltime"), StaticCallBackDialogNMagicCoolTime );

	//------------------------------------------------------------------------------------------------------------
	// 각종 컨트롤러들을 생성한다
	//------------------------------------------------------------------------------------------------------------
	TCHAR szControllerName[MCT_ARRAY_MAX][64] = {
		{	TEXT("statictext_remaintime1")		},
		{	TEXT("statictext_remaintime2")		},
		{	TEXT("statictext_remaintime1Shadow")},
		{	TEXT("statictext_remaintime2Shadow")},
		{	TEXT("statictext_activeBack")		},
		{	TEXT("statictext_buf1Back")			},
		{	TEXT("statictext_buf2Back")			},
		{	TEXT("statictext_active")			},
		{	TEXT("statictext_buf1")				},
		{	TEXT("statictext_buf2")				},
		{	TEXT("statictext_activeCooltime")	},
		{	TEXT("statictext_buf1Cooltime")		},
		{	TEXT("statictext_buf2Cooltime")		}
	};

	SI32	siAlphaValue	= 10;
	SI32	siPrintOption	= DT_SINGLELINE | DT_RIGHT;
	SI16	siFontWeight	= FW_BOLD;

	for ( SI32 siIndex=0; siIndex<MCT_ARRAY_MAX; ++siIndex )
	{
		NEWCSTATIC( m_pctrlStatic[siIndex] );

		if ( m_pctrlStatic[siIndex] )
		{
			file.CreateControl( m_pctrlStatic[siIndex], siIndex, szControllerName[siIndex] );

			switch ( siIndex )
			{
				case MCT_ARRAY_REMAINTIME1:
				case MCT_ARRAY_REMAINTIME2:
					{
						m_pctrlStatic[siIndex]->SetFontColor( RGB(0xFF, 0xFF, 0xFF) );
						m_pctrlStatic[siIndex]->SetTextPrintOption( siPrintOption );
						m_pctrlStatic[siIndex]->SetFontWeight( siFontWeight );
					}
					break;

				case MCT_ARRAY_REMAINTIME1SHADOW:
				case MCT_ARRAY_REMAINTIME2SHADOW:
					{
						m_pctrlStatic[siIndex]->SetFontColor( RGB(0, 0, 0) );
						m_pctrlStatic[siIndex]->SetTextPrintOption( siPrintOption );
						m_pctrlStatic[siIndex]->SetFontWeight( siFontWeight );
					}
					break;

				case MCT_ARRAY_ACTIVE:
					{
						m_pctrlStatic[siIndex]->SetFileName( TEXT("interface/magicmgrdlg/magicicon.spr") );
						m_pctrlStatic[siIndex]->SetDrawOption( SPR_PUTSPRSCREENT );
					}
					break;

				case MCT_ARRAY_BUF1:
				case MCT_ARRAY_BUF2:
					{
						m_pctrlStatic[siIndex]->SetFileName( TEXT("interface/magicmgrdlg/magicicon.spr") );
						m_pctrlStatic[siIndex]->SetDrawOption( SPR_PUTSPRALPHAT );
						m_pctrlStatic[siIndex]->SetAlphaValue( siAlphaValue );
					}
					break;
				case MCT_ARRAY_ACTIVECOOLTIME:
				case MCT_ARRAY_BUF1COOLTIME:
				case MCT_ARRAY_BUF2COOLTIME:
					{
						m_pctrlStatic[siIndex]->SetFileName( TEXT("interface/magicmgrdlg/MagicCoolTime.SPR") );
						//m_pctrlStatic[siIndex]->SetDrawOption( SPR_PUTSPRSCREENT );
					}
					break;
				case MCT_ARRAY_ACTIVEBACK:
					{
						m_pctrlStatic[siIndex]->SetFileName( TEXT("interface/magicmgrdlg/Magicstroke.SPR") );
						m_pctrlStatic[siIndex]->SetFontIndex( 0 );
						m_pctrlStatic[siIndex]->SetDrawOption( SPR_PUTSPRALPHAT );
						m_pctrlStatic[siIndex]->SetAlphaValue( siAlphaValue );
					}
					break;

				case MCT_ARRAY_BUF1BACK:
				case MCT_ARRAY_BUF2BACK:
					{
						m_pctrlStatic[siIndex]->SetFileName( TEXT("interface/magicmgrdlg/Magicstroke.SPR") );
						m_pctrlStatic[siIndex]->SetFontIndex( 1 );
						m_pctrlStatic[siIndex]->SetDrawOption( SPR_PUTSPRALPHAT );
						m_pctrlStatic[siIndex]->SetAlphaValue( siAlphaValue );
					}
					break;

			}
			
			m_pctrlStatic[siIndex]->Show( false );
		}
	}
	//------------------------------------------------------------------------------------------------------------
	// 다이얼로그 위치를 조정한다
	//------------------------------------------------------------------------------------------------------------
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	SI16 siDialogXpos	= ( pclclient->GetScreenXSize() - GetWidth() - 10 );
	SI16 siDialogYpos	= ( pclclient->GetScreenYSize() - GetHeight() - 60 );;

	SetDialogPos( siDialogXpos, siDialogYpos, 0, 0 );

	//------------------------------------------------------------------------------------------------------------
	// Action Timer시작
	//------------------------------------------------------------------------------------------------------------
	SetActionTimer( 500 );
	SetTimerOnOff( true );
}

void CALLBACK CNMagicCoolTimeDlg::StaticCallBackDialogNMagicCoolTime( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMagicCoolTimeDlg *pThis = (CNMagicCoolTimeDlg *)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNMagicCoolTime( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNMagicCoolTimeDlg::CallBackDialogNMagicCoolTime( UINT nEvent, int nControlID, CControllerObj* pControl )
{

}

void CNMagicCoolTimeDlg::Action()
{
	SI32 siCharID = 1;
	if ( false == pclClient->pclCM->IsValidID( siCharID ) )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( siCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	CMagicAttribute *pclMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	if ( NULL == pclMagicAttributePtr )
	{
		return;
	}

	// 엑티브 마법 표시
	ShowActiveMagicInfo( pclchar, pclMagicAttributePtr );

	// 버프 마법 표시
	ShowMagicBufInfo( pclchar, pclMagicAttributePtr );
	
}

void CNMagicCoolTimeDlg::SetActiveMagicCoolTime( SI32 siCoolTime )
{
	m_siActiveMagicCoolTime			= siCoolTime;
	m_siActiveMagicCoolTimeStart	= pclClient->CurrentClock;

}

void CNMagicCoolTimeDlg::ShowActiveMagicInfo( cltCharClient* pclchar, CMagicAttribute* pclMagicAttributePtr )
{
	if ( (NULL == pclchar) || (NULL == pclMagicAttributePtr) )
	{
		return;
	}

	//--------------------------------------------------------------------
	// 컨트롤러 포인터 받아오기
	//--------------------------------------------------------------------
	CStatic* pclActiveMagic		= m_pctrlStatic[MCT_ARRAY_ACTIVE];
	CStatic* pclActiveCoolTime	= m_pctrlStatic[MCT_ARRAY_ACTIVECOOLTIME];
	CStatic* pclActiveBack		= m_pctrlStatic[MCT_ARRAY_ACTIVEBACK];
	if ( (NULL == pclActiveMagic) || (NULL == pclActiveCoolTime) || (NULL == pclActiveBack) )
	{
		return;
	}

	//--------------------------------------------------------------------
	// 현재 유저가 선택한 마법
	//--------------------------------------------------------------------
	SI32 siMagicKind = pclchar->siCurrentMagic;
	if ( 0 >= siMagicKind )
	{
		pclActiveMagic->Show( false );
		pclActiveCoolTime->Show( false );
		pclActiveBack->Show( false );
		return;
	}

	// 현재 선택한 마법
	SI32 siImageFont = pclMagicAttributePtr->GetMagicGroup( siMagicKind );

	pclActiveMagic->Show( true );
	pclActiveMagic->SetFontIndex( siImageFont );
	pclActiveMagic->Refresh();

	pclActiveBack->Show( true );

	//--------------------------------------------------------------------
	// 마법 쿨타임 표시
	//--------------------------------------------------------------------
	stMagicAttributeData* psMagicData = pclClient->pclMagicManager->GetMagicAttribute( siMagicKind );
	if ( NULL == psMagicData )
	{
		return;
	}
	
	// 사용자가 엑티브 마법을 마법버프로 선택할 수 있기 때문에 이럴때는 쿨타임을 보여주지 않는다
	if ( (MAGIC_CASTTYPE_BUF == psMagicData->si08MagicCastType) || (0 >= m_siActiveMagicCoolTime) )
	{
		pclActiveCoolTime->Show( false );
		return;
	}

	// 마법 쿨타임 실제 표시
	SI32 siDiff = TABS( pclClient->CurrentClock - m_siActiveMagicCoolTimeStart );
	if ( siDiff <= m_siActiveMagicCoolTime )
	{
		SI32 siCoolTimeFontIndex = (MCT_VARIABLE_COOLTIMEIMAGE_NUM * siDiff) / m_siActiveMagicCoolTime;

		pclActiveCoolTime->Show( true );
		pclActiveCoolTime->SetFontIndex( siCoolTimeFontIndex );
		pclActiveCoolTime->Refresh( );
	}
	else
	{
		m_siActiveMagicCoolTime = 0;
		pclActiveCoolTime->Show( false );
	}
}

void CNMagicCoolTimeDlg::ShowMagicBufInfo( cltCharClient* pclchar, CMagicAttribute* pclMagicAttributePtr )
{
	if ( (NULL == pclchar) || (NULL == pclMagicAttributePtr) )
	{
		return;
	}

	SI32 siDecMagicDelayRate = pclchar->clPB.siMagicDecDelay;

	for ( SI32 siIndex=0; siIndex<MAX_MAGICBUF_NUM; ++siIndex )
	{
		CNMagicBuf* pclMagicBuf = pclchar->m_clMagicBufMgr.GetMagicBuf( siIndex );
		if ( NULL == pclMagicBuf )
		{
			continue;
		}

		if ( true == pclMagicBuf->IsEmpty() )
		{
			HideMagicBufControllers( siIndex );
			continue;
		}

		CStatic* pclBuf				= m_pctrlStatic[ siIndex + MCT_ARRAY_BUF1 ];
		CStatic* pclCoolTime		= m_pctrlStatic[ siIndex + MCT_ARRAY_BUF1COOLTIME ];
		CStatic* pclRemain			= m_pctrlStatic[ siIndex + MCT_ARRAY_REMAINTIME1 ];
		CStatic* pclShadow			= m_pctrlStatic[ siIndex + MCT_ARRAY_REMAINTIME1SHADOW ];
		CStatic* pclBack			= m_pctrlStatic[ siIndex + MCT_ARRAY_BUF1BACK ];
		if ( (NULL == pclBuf) || (NULL == pclCoolTime) || (NULL == pclRemain) || (NULL == pclShadow) )
		{
			continue;
		}

		SI32 siMagicKind		= pclMagicBuf->GetMagicKind();
		SI32 siRemainActionTime	= ((pclMagicBuf->GetStartTime()+pclMagicBuf->GetActionTime()) - pclClient->CurrentClock) / 1000;
		stMagicAttributeData* psMagicData = pclClient->pclMagicManager->GetMagicAttribute( siMagicKind );

		//-----------------------------------------------------------------------------------------
		// 버프 마법 이미지
		//-----------------------------------------------------------------------------------------
		SI32 siFontIndex = pclMagicAttributePtr->GetMagicGroup( siMagicKind );
		pclBuf->SetFontIndex( siFontIndex );

		bool bShowBuf = true;
		// MCT_VARIABLE_FLICKERSTART_TIME 시간으로 떨어지면 깜빡인다
		if ( (MCT_VARIABLE_FLICKERSTART_TIME >= siRemainActionTime) && (true == pclBuf->IsShow()) )
		{
			bShowBuf = false;
		}
		pclBuf->Show( bShowBuf );
		pclBack->Show( true );


		//-----------------------------------------------------------------------------------------
		// 버프 효과 남은 시간
		//-----------------------------------------------------------------------------------------
		NTCHARString64 strRemainActionTime;
		strRemainActionTime.FormatString( TEXT("%02d : %02d"), (siRemainActionTime/60), (siRemainActionTime%60) );

		pclRemain->Show( true );
		pclRemain->SetText( strRemainActionTime );
		pclShadow->Show( true );
		pclShadow->SetText( strRemainActionTime );


		//-----------------------------------------------------------------------------------------
		// 마법 쿨타임 표시
		//-----------------------------------------------------------------------------------------
		// [진성] 보조 마법에 관한 계산.
		SI32	siSubMagickind		= 0;
		SI32 	siSubDelay 			= 0;
		if( pclClient->IsCountrySwitch(Switch_SkillBook) && pclchar->pclCharSkillBookInfo )
		{
			stMagicAttributeData*	psSubMagicData = pclchar->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( siMagicKind, siSubMagickind );
			if( psSubMagicData )
			{
				siSubDelay = psSubMagicData->si32MagicDelay;
			}
		}

		SI32 siDiff			= TABS( pclClient->CurrentClock - pclMagicBuf->GetStartTime() );
		SI32 siDelay		= psMagicData->si32MagicDelay;
		if ( 0 < siDecMagicDelayRate )
		{
			siDelay -= (psMagicData->si32MagicDelay* siDecMagicDelayRate / 100);
		}
		siDelay += (siDelay * siSubDelay) / 100;
		
		SI32 siCoolTime		= siDelay * (1000 / ConstFrameRate_Low);
		
		if ( siDiff <= siCoolTime )
		{
			SI32 siCoolTimeFontIndex = (MCT_VARIABLE_COOLTIMEIMAGE_NUM * siDiff) / siCoolTime;

			pclCoolTime->Show( true );
			pclCoolTime->SetFontIndex( siCoolTimeFontIndex );
			pclCoolTime->Refresh( );
		}
		else
		{
			pclCoolTime->Show( false );
		}
	}	// for ( SI32 siIndex=0; siIndex<MAX_MAGICBUF_NUM; ++siIndex ) 종료
}

void CNMagicCoolTimeDlg::ShowMagicBufControllers( SI32 siIndex )
{
	if ( (0 > siIndex) || (MAX_MAGICBUF_NUM <= siIndex) )
	{
		return;
	}

	m_pctrlStatic[siIndex+MCT_ARRAY_BUF1]->Show( true );
	m_pctrlStatic[siIndex+MCT_ARRAY_BUF1COOLTIME]->Show( true );

	m_pctrlStatic[siIndex+MCT_ARRAY_REMAINTIME1]->Show( true );
	m_pctrlStatic[siIndex+MCT_ARRAY_REMAINTIME1]->SetText( TEXT("") );

	m_pctrlStatic[siIndex+MCT_ARRAY_REMAINTIME1SHADOW]->Show( true );
	m_pctrlStatic[siIndex+MCT_ARRAY_REMAINTIME1SHADOW]->SetText( TEXT("") );

	m_pctrlStatic[siIndex+MCT_ARRAY_BUF1BACK]->Show( true );
}

void CNMagicCoolTimeDlg::HideMagicBufControllers( SI32 siIndex )
{
	if ( (0 > siIndex) || (MAX_MAGICBUF_NUM <= siIndex) )
	{
		return;
	}

	m_pctrlStatic[siIndex+MCT_ARRAY_BUF1]->Show( false );
	m_pctrlStatic[siIndex+MCT_ARRAY_BUF1COOLTIME]->Show( false );

	m_pctrlStatic[siIndex+MCT_ARRAY_REMAINTIME1]->Show( false );
	m_pctrlStatic[siIndex+MCT_ARRAY_REMAINTIME1]->SetText( TEXT("") );

	m_pctrlStatic[siIndex+MCT_ARRAY_REMAINTIME1SHADOW]->Show( false );
	m_pctrlStatic[siIndex+MCT_ARRAY_REMAINTIME1SHADOW]->SetText( TEXT("") );

	m_pctrlStatic[siIndex+MCT_ARRAY_BUF1BACK]->Show( false );
}