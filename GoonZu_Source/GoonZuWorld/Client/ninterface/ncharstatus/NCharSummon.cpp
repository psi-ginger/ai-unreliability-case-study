//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
/* ==========================================================================
클래스 :

작성일 :		05/03/14
작성자 :		정용래

변경사항 :		없음

차후작업 :		
1. // leon todo - wParam == nControlID ? 확인 요망
2. // leon todo - hDlg == NULL => IsShow() 로 변환 가능 여부 확인 요망!!
3. // leon todo - hDlg => NULL 로 변환 확인 요망!!

========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#include "..\Client\Client.h"
#include "./NCharStatus.h"
#include "./NCharSummon.h"
#include "../../../Common/Father/Father.h"

#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "Msg\MsgType-Horse.h"
#include "Msg\MsgType-Summon.h"
#include "../../../Common/JWLib/IsCorrectID/IsCorrectID.h"
#include "../../lib/WebHTML/WebHTML.h"
#include "..\..\Client\Music\Music.h"

#include "../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../Client/InterfaceMgr/Interface/Static.h"
#include "../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../Client/InterfaceMgr/Interface/Button.h"
#include "../../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../../Client/InterfaceMgr/Interface/CheckBox.h"
#include "../../Client/InterfaceMgr/Interface/RadioButton.h"
#include "../../Client/InterfaceMgr/Interface/OutLine.h"


#include "../../Client/InterfaceMgr/InterfaceMgr.h"
#include "../../Client/InterfaceMgr/InterfaceFile.h"

#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../Client/ninterface/ncharstatus/NCharSummonReset.h"

#include "abusefilter/AbuseFilter.h"

#include "../common/Event/event.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "HintDlg/HintDlg.h"
#include "Char\CharManager\CharManager.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "..\CommonLogic\msgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"

extern cltCommonLogic* pclClient;

cltNSummonStatus::cltNSummonStatus(cltCharManager* pclcm)
{
	m_pImagestatic_portrait	 = NULL; 
	m_pStatictext_level	 = NULL; 
	m_pButton_takeinout	 = NULL; 
	m_pStatictext_exp	 = NULL; 
	m_pStatictext_expvalue	 = NULL; 
	m_pStatictext_magic1	 = NULL; 
	m_pStatictext_magic1value	 = NULL; 
	m_pStatictext_name	 = NULL; 
	m_pStatictext_life	 = NULL; 
	m_pStatictext_lifevalue	 = NULL; 
	m_pStatictext_life2	 = NULL; 
	m_pStatictext_strength	 = NULL; 
	m_pStatictext_dex	 = NULL; 
	m_pStatictext_magic	 = NULL; 
	m_pStatictext_bonus	 = NULL; 
	m_pEditbox_life2	 = NULL; 
	m_pEditbox_strength	 = NULL; 
	m_pEditbox_dex	 = NULL; 
	m_pEditbox_magic	 = NULL; 
	m_pEditbox_bonus	 = NULL; 
	m_pButton_life2_plus	 = NULL; 
	m_pButton_strength_plus	 = NULL; 
	m_pButton_dex_plus	 = NULL; 
	m_pButton_magic_plus	 = NULL; 
	m_pButton_free	 = NULL; 
	m_pButton_explain	 = NULL; 
	m_pButton_init	 = NULL; 
	m_pEditbox_health_explain	 = NULL; 
	m_pStatictext_levelvalue	 = NULL; 
	m_pStatictext_attackattribute	 = NULL; 
	m_pButton_revive	 = NULL; 
	m_pButton_strength_minus	 = NULL; 
	m_pButton_dex_minus	 = NULL; 
	m_pButton_magic_minus	 = NULL; 
	m_pButton_life2_minus	 = NULL; 
	m_pCombobox_strength	 = NULL; 
	m_pCombobox_dex			= NULL; 
	m_pCombobox_magic		 = NULL; 
	m_pCombobox_life2		= NULL; 
	m_pStatictext_totaldamage	 = NULL; 
	m_pStatictext_armourdefense	 = NULL; 
	m_pEditbox_totaldamage	 = NULL; 
	m_pEditbox_armourdefense	 = NULL; 
	m_pCombobox_apowername	 = NULL; 
	m_pCombobox_dpowername	 = NULL; 
	m_pEditbox_name	 = NULL; 
	m_pButton_changename	 = NULL; 
	m_pCombobox_summons	 = NULL; 
	m_pCheckbox_autoheal	 = NULL; 
	m_pRadiobutton_autoheal30	 = NULL; 
	m_pRadiobutton_autoheal50	 = NULL; 
	m_pRadiobutton_autoheal70	 = NULL; 
	m_pRadiobutton_attack	 = NULL; 
	m_pRadiobutton_wait	 = NULL; 
	m_pCombobox_scroll	 = NULL; 
	m_pStatictext_summon = NULL; 

	m_pOutline_noname1	 = NULL; 
	m_pOutline_noname2	 = NULL; 

	if(pclClient->IsCountrySwitch(Switch_Summon_StaminaCharge))
	{
		// PCK - 소환수 스태미너 충전 버튼 추가 (08.07.16)
		m_pButton_SPTCharge					= NULL;
	}

	//[진성]
	m_pStatictext_hand					= NULL;
	m_pEditbox_hand						= NULL;
	m_pStatictext_Wisdom				= NULL;
	m_pEditbox_Wisdom					= NULL;
	m_pStatictext_HP					= NULL;
	m_pStatictext_MP					= NULL;
	m_pStatictext_EXP					= NULL;
	m_pStatictext_FULL_EXP				= NULL;
	m_pStatictext_TransformStamina		= NULL;
	m_pStatictext_physical_attack		= NULL; 	
	m_pEditbox_physical_attack			= NULL;		
	m_pStatictext_pysical_defense		= NULL;	
	m_pEditbox_pysical_defense			= NULL;		
	m_pStatictext_masic_attack			= NULL;		
	m_pEditbox_magic_attack				= NULL;		
	m_pStatictext_magic_defense			= NULL;	
	m_pEditbox_magic_defense			= NULL;		
	m_pStatictext_pysical_hit_rank		= NULL;	
	m_pEditbox_pysical_hit_rank			= NULL;	
	m_pStatictext_pysical_dodge_rank	= NULL;
	m_pEditbox_pysical_dodge_rank		= NULL;	
	m_pStatictext_magic_hit_rank		= NULL;	
	m_pEditbox_magic_hit_rank			= NULL;		
	m_pStatictext_magic_dodge_rank		= NULL;	
	m_pEditbox_magic_dodge_rank			= NULL;	
	m_pStatictext_move_speed			= NULL;		
	m_pEditbox_move_speed				= NULL;	
	m_pOutline_noname3					= NULL;				
	m_pOutline_noname4					= NULL;
	m_pOutline_noname5					= NULL;
	m_pOutline_noname6					= NULL;
	m_pStatictext_hand					= NULL;	
	m_pEditbox_hand						= NULL;	
	m_pStatictext_Wisdom				= NULL;	
	m_pEditbox_Wisdom					= NULL;	
	m_pStatictext_HP					= NULL;	
	m_pStatictext_MP					= NULL;
	m_pStatictext_EXP					= NULL;
	m_pStatictext_FULL_EXP				= NULL;
	m_pStatictext_TransformStamina		= NULL;
	m_pButton_hand_plus					= NULL;
	m_pButton_hand_minus				= NULL;
	m_pButton_wisdom_plus				= NULL;
	m_pButton_wisdom_minus				= NULL;
	m_pImageStatic_bonus				= NULL;
	m_pEdit_life2_new					= NULL;
	m_pEdit_strength_new				= NULL;
	m_pEdit_dex_new						= NULL;
	m_pEdit_magic_new					= NULL;
	m_pEdit_hand_new					= NULL;
	m_pEdit_Wisdom_new					= NULL;
	m_pButton_Mysummon					= NULL;
	m_pButton_Summon_Transform			= NULL;


	


	m_bShow = false;
	CharUnique = 0;
	pclCM	= pclcm;

	//[추가 : 황진성 2008. 2. 19 => 소환수 능력치 조절 기능 수정에 사용될 변수 초기화.]
	m_clSummon_Use_Bonus.Clear();

	
	if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) )
	{
		//[진성] 게이지에 사용할 이미지 로드.
		GP.LoadSprFromFile( TEXT("gimg/StatusGaugeBack3.SPR"),	&m_sprGageBack );
	}
	else
	{
		//[진성] 게이지에 사용할 이미지 로드.
		GP.LoadSprFromFile( TEXT("gimg/StatusGaugeBack2.SPR"),	&m_sprGageBack );
	}
	
	
	GP.LoadSprFromFile( TEXT("Interface/Pannel/IMAGE_2x10_00_000.SPR"), &m_sprGage );

	if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) )
	{
		//[진성] 소환수 변신 관련 이미지. 2009-9-11
		GP.LoadSprFromFile( TEXT("NInterface/image/TTIME.SPR"),  &m_sprTTime );
		GP.LoadSprFromFile( TEXT("NInterface/image/TGage1.SPR"), &m_sprTGage1 );
	}
		
	

	// 현재 소환수의 버프상태 세팅.
	BufStatusChange = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUMMONATACUP);
}

cltNSummonStatus::~cltNSummonStatus()
{
	if( 	m_pImagestatic_portrait	 )  	delete 	m_pImagestatic_portrait	;
	if( 	m_pStatictext_level	 )  	delete 	m_pStatictext_level	;
	if( 	m_pButton_takeinout	 )  	delete 	m_pButton_takeinout	;
	if( 	m_pStatictext_exp	 )  	delete 	m_pStatictext_exp	;
	if( 	m_pStatictext_expvalue	 )  	delete 	m_pStatictext_expvalue	;
	if( 	m_pStatictext_magic1	 )  	delete 	m_pStatictext_magic1	;
	if( 	m_pStatictext_magic1value	 )  	delete 	m_pStatictext_magic1value	;
	if( 	m_pStatictext_name	 )  	delete 	m_pStatictext_name	;
	if( 	m_pStatictext_life	 )  	delete 	m_pStatictext_life	;
	if( 	m_pStatictext_lifevalue	 )  	delete 	m_pStatictext_lifevalue	;
	if( 	m_pStatictext_life2	 )  	delete 	m_pStatictext_life2	;
	if( 	m_pStatictext_strength	 )  	delete 	m_pStatictext_strength	;
	if( 	m_pStatictext_dex	 )  	delete 	m_pStatictext_dex	;
	if( 	m_pStatictext_magic	 )  	delete 	m_pStatictext_magic	;
	if( 	m_pStatictext_bonus	 )  	delete 	m_pStatictext_bonus	;
	if( 	m_pEditbox_life2	 )  	delete 	m_pEditbox_life2	;
	if( 	m_pEditbox_strength	 )  	delete 	m_pEditbox_strength	;
	if( 	m_pEditbox_dex	 )  	delete 	m_pEditbox_dex	;
	if( 	m_pEditbox_magic	 )  	delete 	m_pEditbox_magic	;
	if( 	m_pEditbox_bonus	 )  	delete 	m_pEditbox_bonus	;
	if( 	m_pButton_life2_plus	 )  	delete 	m_pButton_life2_plus	;
	if( 	m_pButton_strength_plus	 )  	delete 	m_pButton_strength_plus	;
	if( 	m_pButton_dex_plus	 )  	delete 	m_pButton_dex_plus	;
	if( 	m_pButton_magic_plus	 )  	delete 	m_pButton_magic_plus	;
	if( 	m_pButton_free	 )  	delete 	m_pButton_free	;
	if( 	m_pButton_explain	 )  	delete 	m_pButton_explain	;
	if( 	m_pButton_init	 )  	delete 	m_pButton_init	;
	if( 	m_pEditbox_health_explain	 )  	delete 	m_pEditbox_health_explain	;
	if( 	m_pStatictext_levelvalue	 )  	delete 	m_pStatictext_levelvalue	;
	if( 	m_pStatictext_attackattribute	 )  	delete 	m_pStatictext_attackattribute	;
	if( 	m_pButton_revive	 )  	delete 	m_pButton_revive	;
	if( 	m_pButton_strength_minus	 )  	delete 	m_pButton_strength_minus	;
	if( 	m_pButton_dex_minus	 )  	delete 	m_pButton_dex_minus	;
	if( 	m_pButton_magic_minus	 )  	delete 	m_pButton_magic_minus	;
	if( 	m_pButton_life2_minus	 )  	delete 	m_pButton_life2_minus	;
	if( 	m_pButton_Ability_Ok	 )		delete  m_pButton_Ability_Ok;	
	if( 	m_pButton_Ability_Cancel )		delete  m_pButton_Ability_Cancel;
	if( 	m_pCombobox_strength	 )  	delete 	m_pCombobox_strength;
	if( 	m_pCombobox_dex	 )  	delete 	m_pCombobox_dex	;
	if( 	m_pCombobox_magic	 )  	delete 	m_pCombobox_magic	;
	if( 	m_pCombobox_life2	 )  	delete 	m_pCombobox_life2	;
	if( 	m_pStatictext_totaldamage	 )  	delete 	m_pStatictext_totaldamage	;
	if( 	m_pStatictext_armourdefense	 )  	delete 	m_pStatictext_armourdefense	;
	if( 	m_pEditbox_totaldamage	 )  	delete 	m_pEditbox_totaldamage	;
	if( 	m_pEditbox_armourdefense	 )  	delete 	m_pEditbox_armourdefense	;
	if( 	m_pCombobox_apowername	 )  	delete 	m_pCombobox_apowername	;
	if( 	m_pCombobox_dpowername	 )  	delete 	m_pCombobox_dpowername	;
	if( 	m_pEditbox_name	 )  	delete 	m_pEditbox_name	;
	if( 	m_pButton_changename	 )  	delete 	m_pButton_changename	;
	if( 	m_pCombobox_summons	 )  	delete 	m_pCombobox_summons	;
	if( 	m_pCheckbox_autoheal	 )  	delete 	m_pCheckbox_autoheal	;
	if( 	m_pRadiobutton_autoheal30	 )  	delete 	m_pRadiobutton_autoheal30	;
	if( 	m_pRadiobutton_autoheal50	 )  	delete 	m_pRadiobutton_autoheal50	;
	if( 	m_pRadiobutton_autoheal70	 )  	delete 	m_pRadiobutton_autoheal70	;
	if( 	m_pRadiobutton_attack	 )  	delete 	m_pRadiobutton_attack	;
	if( 	m_pRadiobutton_wait	 )  	delete 	m_pRadiobutton_wait	;
	if( 	m_pCombobox_scroll	 )  	delete 	m_pCombobox_scroll	;
	if( 	m_pStatictext_summon	 )  	delete 	m_pStatictext_summon	;

	if( 	m_pOutline_noname1	 )  	delete 	m_pOutline_noname1	;
	if( 	m_pOutline_noname2	 )  	delete 	m_pOutline_noname2	;

	if(pclClient->IsCountrySwitch(Switch_Summon_StaminaCharge))
	{
		//PCK - 소환수 스태미너 충전 버튼 추가 (08.07.16)
		if(m_pButton_SPTCharge				)   delete 	m_pButton_SPTCharge			;
	}

	//[진성] 소환수 상태창 변경.
	if(m_pStatictext_physical_attack	)  	delete 	m_pStatictext_physical_attack	;
	if(m_pEditbox_physical_attack		)   delete 	m_pEditbox_physical_attack		;
	if(m_pStatictext_pysical_defense	)   delete 	m_pStatictext_pysical_defense	;
	if(m_pEditbox_pysical_defense		)  	delete 	m_pEditbox_pysical_defense		;
	if(m_pStatictext_masic_attack		)   delete 	m_pStatictext_masic_attack		;
	if(m_pEditbox_magic_attack			)  	delete 	m_pEditbox_magic_attack			;
	if(m_pStatictext_magic_defense		)  	delete 	m_pStatictext_magic_defense		;
	if(m_pEditbox_magic_defense			)	delete 	m_pEditbox_magic_defense		;
	if(m_pStatictext_pysical_hit_rank	)  	delete 	m_pStatictext_pysical_hit_rank	;
	if(m_pEditbox_pysical_hit_rank		)  	delete 	m_pEditbox_pysical_hit_rank		;
	if(m_pStatictext_pysical_dodge_rank )  	delete 	m_pStatictext_pysical_dodge_rank;
	if(m_pEditbox_pysical_dodge_rank	)   delete 	m_pEditbox_pysical_dodge_rank	;
	if(m_pStatictext_magic_hit_rank		)	delete 	m_pStatictext_magic_hit_rank	;
	if(m_pEditbox_magic_hit_rank		)   delete 	m_pEditbox_magic_hit_rank		;
	if(m_pStatictext_magic_dodge_rank	)  	delete 	m_pStatictext_magic_dodge_rank	;
	if(m_pEditbox_magic_dodge_rank		)  	delete 	m_pEditbox_magic_dodge_rank		;
	if(m_pStatictext_move_speed			)	delete 	m_pStatictext_move_speed		;
	if(m_pEditbox_move_speed			)  	delete 	m_pEditbox_move_speed			;
	if(m_pOutline_noname3				)  	delete 	m_pOutline_noname3				;
	if(m_pOutline_noname4				)  	delete 	m_pOutline_noname4				;
	if(m_pOutline_noname5				)   delete 	m_pOutline_noname5				;
	if(m_pOutline_noname6				)   delete 	m_pOutline_noname6				;
	if(m_pStatictext_hand				)   delete 	m_pStatictext_hand				;
	if(m_pEditbox_hand					)  	delete 	m_pEditbox_hand					;
	if(m_pStatictext_Wisdom				)  	delete 	m_pStatictext_Wisdom			;
	if(m_pEditbox_Wisdom				)	delete 	m_pEditbox_Wisdom				;
	if(m_pStatictext_HP					)  	delete 	m_pStatictext_HP				;
	if(m_pStatictext_MP					)  	delete 	m_pStatictext_MP				;
	if(m_pStatictext_EXP				)  	delete 	m_pStatictext_EXP				;
	if(m_pStatictext_FULL_EXP			)   delete 	m_pStatictext_FULL_EXP			;
	if(m_pStatictext_TransformStamina	)   delete 	m_pStatictext_TransformStamina	;
	
	if(m_pButton_hand_plus				)  	delete 	m_pButton_hand_plus				;
	if(m_pButton_hand_minus				)  	delete 	m_pButton_hand_minus			;
	if(m_pButton_wisdom_plus			)  	delete 	m_pButton_wisdom_plus			;
	if(m_pButton_wisdom_minus			)   delete 	m_pButton_wisdom_minus			;
	if(m_pImageStatic_bonus				)   delete 	m_pImageStatic_bonus			;

	if(m_pEdit_life2_new				)   delete 	m_pEdit_life2_new			;
	if(m_pEdit_strength_new				)  	delete 	m_pEdit_strength_new		;
	if(m_pEdit_dex_new					)  	delete 	m_pEdit_dex_new				;
	if(m_pEdit_magic_new				)  	delete 	m_pEdit_magic_new			;
	if(m_pEdit_hand_new					)   delete 	m_pEdit_hand_new			;
	if(m_pEdit_Wisdom_new				)   delete 	m_pEdit_Wisdom_new			;
	
	if(m_pButton_Mysummon				)   delete 	m_pButton_Mysummon			;
	if(m_pButton_Summon_Transform		)   delete 	m_pButton_Summon_Transform	;
	
	GP.FreeSpr( m_sprGageBack	);
	GP.FreeSpr( m_sprGage		);
	
	if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) )
	{	
		GP.FreeSpr( m_sprTTime		);
		GP.FreeSpr( m_sprTGage1		);
	}
	
	SetTimerOnOff( false );    
}

void cltNSummonStatus::Create( CControllerMgr *pParent )
{
	//if ( ! IsCreate() )
	{
		cltClient *pclclient = (cltClient*)pclClient;
		CharUnique = pclclient->MyCharUnique;

		CInterfaceFile file;

		
		if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) )
		{
			file.LoadFile( TEXT("NInterface/Data/NCharStatusDlg/DLG_NCharStatus_Child_NStatusSummon_Transform.ddf") );
		}
		//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
		else if(pclClient->IsCountrySwitch(Switch_SummonReform))
		{
			file.LoadFile( TEXT("NInterface/Data/NCharStatusDlg/DLG_NCharStatus_Child_NStatusSummon_New.ddf") );
		}
		else
		{
			file.LoadFile( TEXT("NInterface/Data/NCharStatusDlg/DLG_NCharStatus_Child_NStatusSummon.ddf") );
		}
		file.CreateChildDialog( this, CLTNSUMMONSTATUS_DLG, TEXT("dialog_summon"), NSummonStatusStaticProc, pParent );

		// 다이얼로그 위치 변경
		SetDialogPos( ((cltNCharStatus*)pParent)->GetChildDlgXPos(), ((cltNCharStatus*)pParent)->GetChildDlgYPos(), GetWidth(), GetHeight() );

		m_pImagestatic_portrait			= new 	CImageStatic ( this )	;
		m_pStatictext_level				= new 	CStatic	( this )	;
		m_pButton_takeinout				= new 	CButton	( this )	;
		m_pStatictext_exp				= new 	CStatic	( this )	;
		m_pStatictext_expvalue			= new 	CStatic	( this )	;
		m_pStatictext_magic1			= new 	CStatic	( this )	;
		m_pStatictext_magic1value		= new 	CStatic	( this )	;
		m_pStatictext_name				= new 	CStatic	( this )	;
		m_pStatictext_life				= new 	CStatic	( this )	;
		m_pStatictext_lifevalue			= new 	CStatic	( this )	;
		m_pStatictext_life2				= new 	CStatic	( this )	;
		m_pStatictext_strength			= new 	CStatic	( this )	;
		m_pStatictext_dex				= new 	CStatic	( this )	;
		m_pStatictext_magic				= new 	CStatic	( this )	;
		m_pStatictext_bonus				= new 	CStatic	( this )	;
		m_pEditbox_life2				= new 	CEdit	( this )	;
		m_pEditbox_strength				= new 	CEdit	( this )	;
		m_pEditbox_dex					= new 	CEdit	( this )	;
		m_pEditbox_magic				= new 	CEdit	( this )	;
		m_pEditbox_bonus				= new 	CEdit	( this )	;
		m_pButton_life2_plus			= new 	CButton	( this )	;
		m_pButton_strength_plus			= new 	CButton	( this )	;
		m_pButton_dex_plus				= new 	CButton	( this )	;
		m_pButton_magic_plus			= new 	CButton	( this )	;
		m_pButton_free					= new 	CButton	( this )	;
		m_pButton_explain				= new 	CButton	( this )	;
		m_pButton_init					= new 	CButton	( this )	;
		m_pEditbox_health_explain		= new 	CEdit	( this )	;
		m_pStatictext_levelvalue		= new 	CStatic	( this )	;
		m_pStatictext_attackattribute	= new 	CStatic	( this )	;
		m_pButton_revive				= new 	CButton	( this )	;
		m_pButton_strength_minus		= new 	CButton	( this )	;
		m_pButton_dex_minus				= new 	CButton	( this )	;
		m_pButton_magic_minus			= new 	CButton	( this )	;
		m_pButton_life2_minus			= new 	CButton	( this )	;
		m_pButton_Ability_Ok			= new 	CButton	( this );
		m_pButton_Ability_Cancel		= new 	CButton	( this );
		m_pStatictext_totaldamage		= new 	CStatic	( this )	;
		m_pStatictext_armourdefense		= new 	CStatic	( this )	;
		m_pEditbox_totaldamage			= new 	CEdit	( this )	;
		m_pEditbox_armourdefense		= new 	CEdit	( this )	;
		m_pEditbox_name					= new 	CEdit	( this )	;
		m_pButton_changename			= new 	CButton	( this )	;
		m_pCheckbox_autoheal			= new 	CCheckBox	( this )	;
		m_pRadiobutton_autoheal30		= new 	CRadioButton	( this )	;
		m_pRadiobutton_autoheal50		= new 	CRadioButton	( this )	;
		m_pRadiobutton_autoheal70		= new 	CRadioButton	( this )	;
		m_pRadiobutton_attack			= new 	CRadioButton	( this )	;
		m_pRadiobutton_wait				= new 	CRadioButton	( this )	;
		m_pCombobox_scroll				= new 	CComboBox	( this )	;
		m_pStatictext_summon			= new 	CStatic	( this )	;
		m_pCombobox_summons				= new 	CComboBox	( this )	;
		m_pCombobox_dpowername			= new 	CComboBox	( this )	;
		m_pCombobox_apowername			= new 	CComboBox	( this )	;
		m_pCombobox_life2				= new 	CComboBox	( this )	;
		m_pCombobox_magic				= new 	CComboBox	( this )	;
		m_pCombobox_dex					= new 	CComboBox	( this )	;
		m_pCombobox_strength			= new 	CComboBox	( this )	;
		m_pOutline_noname1				= new 	COutLine	( this )	;
		m_pOutline_noname2				= new 	COutLine	( this )	;

		if(pclClient->IsCountrySwitch(Switch_Summon_StaminaCharge))
		{
			// PCK - 소환수 스태미너 충전 버튼 추가 (08.07.16)
			m_pButton_SPTCharge					= new	CButton ( this );
		}

		//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
		//if(pclClient->IsCountrySwitch(Switch_SummonReform))
		{
			m_pStatictext_physical_attack		= new	CStatic	( this ); 		
			m_pEditbox_physical_attack			= new 	CEdit	( this );		
			m_pStatictext_pysical_defense		= new	CStatic	( this );		
			m_pEditbox_pysical_defense			= new 	CEdit	( this );		
			m_pStatictext_masic_attack			= new	CStatic	( this );			
			m_pEditbox_magic_attack				= new 	CEdit	( this );			
			m_pStatictext_magic_defense			= new	CStatic	( this );		
			m_pEditbox_magic_defense			= new 	CEdit	( this );			
			m_pStatictext_pysical_hit_rank		= new	CStatic	( this );		
			m_pEditbox_pysical_hit_rank			= new 	CEdit	( this );		
			m_pStatictext_pysical_dodge_rank	= new	CStatic	( this );	
			m_pEditbox_pysical_dodge_rank		= new 	CEdit	( this );	
			m_pStatictext_magic_hit_rank		= new	CStatic	( this );		
			m_pEditbox_magic_hit_rank			= new 	CEdit	( this );		
			m_pStatictext_magic_dodge_rank		= new	CStatic	( this );		
			m_pEditbox_magic_dodge_rank			= new 	CEdit	( this );		
			m_pStatictext_move_speed			= new	CStatic	( this );			
			m_pEditbox_move_speed				= new 	CEdit	( this );			
			m_pOutline_noname3					= new 	COutLine( this );
			m_pOutline_noname4					= new 	COutLine( this );
			m_pOutline_noname5					= new 	COutLine( this );
			m_pOutline_noname6					= new 	COutLine( this );
			m_pStatictext_hand					= new	CStatic	( this );
			m_pEditbox_hand						= new 	CEdit	( this );
			m_pStatictext_Wisdom				= new	CStatic	( this );
			m_pEditbox_Wisdom					= new 	CEdit	( this );
			m_pStatictext_HP					= new	CStatic	( this );
			m_pStatictext_MP					= new	CStatic	( this );
			m_pStatictext_EXP					= new	CStatic	( this );
			m_pStatictext_FULL_EXP				= new	CStatic	( this );
			m_pStatictext_TransformStamina		= new	CStatic	( this );
			m_pButton_hand_plus					= new 	CButton	( this );
			m_pButton_hand_minus				= new 	CButton	( this );
			m_pButton_wisdom_plus				= new 	CButton	( this );
			m_pButton_wisdom_minus				= new 	CButton	( this );
			m_pImageStatic_bonus				= new   CImageStatic ( this );

			m_pEdit_life2_new					= new 	CEdit	( this );
			m_pEdit_strength_new				= new 	CEdit	( this );
			m_pEdit_dex_new						= new 	CEdit	( this );
			m_pEdit_magic_new					= new 	CEdit	( this );
			m_pEdit_hand_new					= new 	CEdit	( this );
			m_pEdit_Wisdom_new					= new 	CEdit	( this );
		}

		m_pButton_Mysummon						= new 	CButton	( this );
		m_pButton_Summon_Transform				= new 	CButton	( this );
		

		file.CreateControl( m_pImagestatic_portrait,		CLTNSUMMONSTATUS_DLG_IMAGESTATIC_PORTRAIT,		TEXT("imagestatic_portrait") );
		//PCK - 소환수 스태미너 충전 버튼 추가 (08.07.16)
		if(pclClient->IsCountrySwitch(Switch_Summon_StaminaCharge))
		{
			file.CreateControl( 	m_pButton_SPTCharge	, 		CLTNSUMMONSTATUS_DLG_BUTTION_SPTCHARGE, 		TEXT("button_SPTCharge") )	;

		}
		file.CreateControl( m_pStatictext_level,			CLTNSUMMONSTATUS_DLG_STATICTEXT_LEVEL,			TEXT("statictext_level") );
		file.CreateControl( m_pButton_takeinout,			CLTNSUMMONSTATUS_DLG_BUTTON_TAKEINOUT,			TEXT("button_takeinout") );
		file.CreateControl( m_pStatictext_name,				CLTNSUMMONSTATUS_DLG_STATICTEXT_NAME,			TEXT("statictext_name") );
		file.CreateControl( m_pStatictext_life2,			CLTNSUMMONSTATUS_DLG_STATICTEXT_LIFE2,			TEXT("statictext_life2") );
		file.CreateControl( m_pStatictext_strength,			CLTNSUMMONSTATUS_DLG_STATICTEXT_STRENGTH,		TEXT("statictext_strength") );
		file.CreateControl( m_pStatictext_dex,				CLTNSUMMONSTATUS_DLG_STATICTEXT_DEX,			TEXT("statictext_dex") );
		file.CreateControl( m_pStatictext_magic,			CLTNSUMMONSTATUS_DLG_STATICTEXT_MAGIC,			TEXT("statictext_magic") );
		file.CreateControl( m_pStatictext_bonus,			CLTNSUMMONSTATUS_DLG_STATICTEXT_BONUS,			TEXT("statictext_bonus") );
		file.CreateControl( m_pEditbox_life2,				CLTNSUMMONSTATUS_DLG_EDITBOX_LIFE2,				TEXT("editbox_life2") );
		file.CreateControl( m_pEditbox_strength,			CLTNSUMMONSTATUS_DLG_EDITBOX_STRENGTH,			TEXT("editbox_strength") );
		file.CreateControl( m_pEditbox_dex,					CLTNSUMMONSTATUS_DLG_EDITBOX_DEX,				TEXT("editbox_dex") );
		file.CreateControl( m_pEditbox_magic,				CLTNSUMMONSTATUS_DLG_EDITBOX_MAGIC,				TEXT("editbox_magic") );
		file.CreateControl( m_pEditbox_bonus,				CLTNSUMMONSTATUS_DLG_EDITBOX_BONUS,				TEXT("editbox_bonus") );
		file.CreateControl( m_pButton_life2_plus,			CLTNSUMMONSTATUS_DLG_BUTTON_LIFE2_PLUS,			TEXT("button_life2_plus") );
		file.CreateControl( m_pButton_strength_plus,		CLTNSUMMONSTATUS_DLG_BUTTON_STRENGTH_PLUS,		TEXT("button_strength_plus") );
		file.CreateControl( m_pButton_dex_plus,				CLTNSUMMONSTATUS_DLG_BUTTON_DEX_PLUS,			TEXT("button_dex_plus") );
		file.CreateControl( m_pButton_magic_plus,			CLTNSUMMONSTATUS_DLG_BUTTON_MAGIC_PLUS,			TEXT("button_magic_plus") );
		file.CreateControl( m_pButton_free,					CLTNSUMMONSTATUS_DLG_BUTTON_FREE,				TEXT("button_free") );
		file.CreateControl( m_pButton_explain,				CLTNSUMMONSTATUS_DLG_BUTTON_EXPLAIN,			TEXT("button_explain") );
		file.CreateControl( m_pButton_init,					CLTNSUMMONSTATUS_DLG_BUTTON_INIT,				TEXT("button_init") );
		file.CreateControl( m_pStatictext_levelvalue,		CLTNSUMMONSTATUS_DLG_STATICTEXT_LEVELVALUE,		TEXT("statictext_levelvalue") );
		file.CreateControl( m_pStatictext_attackattribute,	CLTNSUMMONSTATUS_DLG_STATICTEXT_ATTACKATTRIBUTE,TEXT("statictext_attackattribute") );
		file.CreateControl( m_pButton_revive,				CLTNSUMMONSTATUS_DLG_BUTTON_REVIVE,				TEXT("button_revive") );
		file.CreateControl( m_pButton_strength_minus,		CLTNSUMMONSTATUS_DLG_BUTTON_STRENGTH_MINUS,		TEXT("button_strength_minus") );
		file.CreateControl( m_pButton_dex_minus,			CLTNSUMMONSTATUS_DLG_BUTTON_DEX_MINUS,			TEXT("button_dex_minus") );
		file.CreateControl( m_pButton_magic_minus,			CLTNSUMMONSTATUS_DLG_BUTTON_MAGIC_MINUS,		TEXT("button_magic_minus") );
		file.CreateControl( m_pButton_life2_minus,			CLTNSUMMONSTATUS_DLG_BUTTON_LIFE2_MINUS,		TEXT("button_life2_minus") );
		file.CreateControl( m_pButton_Ability_Ok	 ,		CLTNSUMMONSTATUS_DLG_BUTTON_ABILITY_OK,			TEXT("button_ability_ok") );
		file.CreateControl( m_pButton_Ability_Cancel,		CLTNSUMMONSTATUS_DLG_BUTTON_ABILITY_CANCEL,		TEXT("button_ability_cancel") );
		file.CreateControl( m_pStatictext_totaldamage,		CLTNSUMMONSTATUS_DLG_STATICTEXT_TOTALDAMAGE,	TEXT("statictext_totaldamage") );
		file.CreateControl( m_pStatictext_armourdefense,	CLTNSUMMONSTATUS_DLG_STATICTEXT_ARMOURDEFENSE,	TEXT("statictext_armourdefense") );
		file.CreateControl( m_pEditbox_totaldamage,			CLTNSUMMONSTATUS_DLG_EDITBOX_TOTALDAMAGE,		TEXT("editbox_totaldamage") );
		file.CreateControl( m_pEditbox_armourdefense,		CLTNSUMMONSTATUS_DLG_EDITBOX_ARMOURDEFENSE,		TEXT("editbox_armourdefense") );
		file.CreateControl( m_pEditbox_name,				CLTNSUMMONSTATUS_DLG_EDITBOX_NAME,				TEXT("editbox_name") );
		file.CreateControl( m_pButton_changename,			CLTNSUMMONSTATUS_DLG_BUTTON_CHANGENAME,			TEXT("button_changename") );
		file.CreateControl( m_pCheckbox_autoheal,			CLTNSUMMONSTATUS_DLG_CHECKBOX_AUTOHEAL,			TEXT("checkbox_autoheal") );
		file.CreateControl( m_pRadiobutton_autoheal30,		CLTNSUMMONSTATUS_DLG_RADIOBUTTON_AUTOHEAL30,	TEXT("radiobutton_autoheal30") );
		file.CreateControl( m_pRadiobutton_autoheal50,		CLTNSUMMONSTATUS_DLG_RADIOBUTTON_AUTOHEAL50,	TEXT("radiobutton_autoheal50") );
		file.CreateControl( m_pRadiobutton_autoheal70,		CLTNSUMMONSTATUS_DLG_RADIOBUTTON_AUTOHEAL70,	TEXT("radiobutton_autoheal70") );
		file.CreateControl( m_pRadiobutton_attack,			CLTNSUMMONSTATUS_DLG_RADIOBUTTON_ATTACK,		TEXT("radiobutton_attack") );
		file.CreateControl( m_pRadiobutton_wait,			CLTNSUMMONSTATUS_DLG_RADIOBUTTON_WAIT,			TEXT("radiobutton_wait") );
		file.CreateControl( m_pCombobox_scroll,				CLTNSUMMONSTATUS_DLG_COMBOBOX_SCROLL,			TEXT("combobox_scroll") );
		file.CreateControl( m_pStatictext_summon,			CLTNSUMMONSTATUS_DLG_STATICTEXT_SUMMON,			TEXT("statictext_summon") );
		file.CreateControl( m_pCombobox_summons,			CLTNSUMMONSTATUS_DLG_COMBOBOX_SUMMONS,			TEXT("combobox_summons") );
		if(pclClient->IsCountrySwitch(Switch_SummonHouse))
		{
			file.CreateControl( 	m_pButton_Mysummon, 				CLTNSUMMONSTATUS_DLG_BUTTON_MYSUMMON,					TEXT("button_MySummon"));
		}

		if(pclClient->IsCountrySwitch(Switch_SummonTramsform))
		{
			file.CreateControl( 	m_pButton_Summon_Transform , 		CLTNSUMMONSTATUS_DLG_BUTTON_SUMMON_TRANSFORM,			TEXT("button_summon_tranform"));
		}

		//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
		if(pclClient->IsCountrySwitch(Switch_SummonReform)) 
		{
			file.CreateControl( 	m_pStatictext_physical_attack,		CLTNSUMMONSTATUS_DLG_STATICTEXT_PHYSICAL_ATTACK,		TEXT("statictext_NONAME6"));
			file.CreateControl( 	m_pEditbox_physical_attack,		 	CLTNSUMMONSTATUS_DLG_EDIT_PHYSICAL_ATTACK,				TEXT("editbox_physical_attack"));
			file.CreateControl( 	m_pStatictext_pysical_defense,		CLTNSUMMONSTATUS_DLG_STATICTEXT_PHYSICAL_DEFENSE,		TEXT("statictext_NONAME7"));
			file.CreateControl( 	m_pEditbox_pysical_defense,			CLTNSUMMONSTATUS_DLG_EDIT_PHYSICAL_DEFENSE,				TEXT("editbox_pysical_defense"));
			file.CreateControl( 	m_pStatictext_masic_attack,			CLTNSUMMONSTATUS_DLG_STATICTEXT_MAGIC_ATTACK,			TEXT("statictext_NONAME8"));
			file.CreateControl( 	m_pEditbox_magic_attack,			CLTNSUMMONSTATUS_DLG_EDIT_MAGIC_ATTACK,					TEXT("editbox_magic_attack"));
			file.CreateControl( 	m_pStatictext_magic_defense,		CLTNSUMMONSTATUS_DLG_STATICTEXT_MAGIC_DEFENSE,			TEXT("statictext_NONAME9"));
			file.CreateControl( 	m_pEditbox_magic_defense,			CLTNSUMMONSTATUS_DLG_EDIT_MAGIC_DEFENSE,				TEXT("editbox_magic_defense"));
			file.CreateControl( 	m_pStatictext_pysical_hit_rank,		CLTNSUMMONSTATUS_DLG_STATICTEXT_PHYSICAL_HIT_RANK,		TEXT("statictext_NONAME10"));
			file.CreateControl( 	m_pEditbox_pysical_hit_rank	,	 	CLTNSUMMONSTATUS_DLG_EDIT_PHYSICAL_HIT_RANK,			TEXT("editbox_pysical_hit_rank"));
			file.CreateControl( 	m_pStatictext_pysical_dodge_rank,	CLTNSUMMONSTATUS_DLG_STATICTEXT_PHYSICAL_DODGE_RANK,	TEXT("statictext_NONAME11"));
			file.CreateControl( 	m_pEditbox_pysical_dodge_rank,		CLTNSUMMONSTATUS_DLG_EDIT_PHYSICAL_DODGE_RANK,			TEXT("editbox_pysical_dodge_rank"));
			file.CreateControl( 	m_pStatictext_magic_hit_rank,		CLTNSUMMONSTATUS_DLG_STATICTEXT_MAGIC_HIT_RANK,			TEXT("statictext_NONAME12"));
			file.CreateControl( 	m_pEditbox_magic_hit_rank,		 	CLTNSUMMONSTATUS_DLG_EDIT_MAGIC_HIT_RANK,				TEXT("editbox_magic_hit_rank"));
			file.CreateControl( 	m_pStatictext_magic_dodge_rank,		CLTNSUMMONSTATUS_DLG_STATICTEXT_MAGIC_DODGE_RANK,		TEXT("statictext_NONAME13"));
			file.CreateControl( 	m_pEditbox_magic_dodge_rank,		CLTNSUMMONSTATUS_DLG_EDIT_MAGIC_DODGE_RANK,				TEXT("editbox_magic_dodge_rank"));
			file.CreateControl( 	m_pStatictext_move_speed,			CLTNSUMMONSTATUS_DLG_STATICTEXT_MOVE_SPEED,				TEXT("statictext_NONAME14"));
			file.CreateControl( 	m_pEditbox_move_speed,			 	CLTNSUMMONSTATUS_DLG_EDIT_MOVE_SPEED,					TEXT("editbox_move_speed"));
			file.CreateControl( 	m_pStatictext_hand,					CLTNSUMMONSTATUS_DLG_STATICTEXT_HAND,					TEXT("statictext_hand"));
			file.CreateControl( 	m_pEditbox_hand,					CLTNSUMMONSTATUS_DLG_EDIT_HAND,							TEXT("editbox_hand"));
			file.CreateControl( 	m_pStatictext_Wisdom,				CLTNSUMMONSTATUS_DLG_STATICTEXT_WISDOM,					TEXT("statictext_wisdom"));
			file.CreateControl( 	m_pEditbox_Wisdom,					CLTNSUMMONSTATUS_DLG_EDIT_WISDOM,						TEXT("editbox_wisdom"));
			file.CreateControl( 	m_pStatictext_HP,					CLTNSUMMONSTATUS_DLG_STATICTEXT_HP,						TEXT("statictext_NONAME15"));
			file.CreateControl( 	m_pStatictext_MP,					CLTNSUMMONSTATUS_DLG_STATICTEXT_MP,						TEXT("statictext_NONAME16"));
			file.CreateControl( 	m_pStatictext_EXP,					CLTNSUMMONSTATUS_DLG_STATICTEXT_EXP_NEW,				TEXT("statictext_NONAME17"));
			file.CreateControl( 	m_pStatictext_FULL_EXP,				CLTNSUMMONSTATUS_DLG_STATICTEXT_FULL_EXP,				TEXT("statictext_NONAME18"));
			file.CreateControl( 	m_pStatictext_TransformStamina,		CLTNSUMMONSTATUS_DLG_STATICTEXT_TRANSFORM_STAMINA,		TEXT("statictext_NONAME19"));

			
			file.CreateControl( 	m_pButton_hand_plus,				CLTNSUMMONSTATUS_DLG_BUTTON_HAND_PLUS,					TEXT("button_NONAME1"));
			file.CreateControl( 	m_pButton_hand_minus,				CLTNSUMMONSTATUS_DLG_BUTTON_HAND_MINUS,					TEXT("button_NONAME2"));
			file.CreateControl( 	m_pButton_wisdom_plus,				CLTNSUMMONSTATUS_DLG_BUTTON_WISDOM_PLUS,				TEXT("button_NONAME3"));
			file.CreateControl( 	m_pButton_wisdom_minus,				CLTNSUMMONSTATUS_DLG_BUTTON_WISDOM_MINUS,				TEXT("button_NONAME4"));
			file.CreateControl( 	m_pImageStatic_bonus, 				CLTNSUMMONSTATUS_DLG_IMAGESTATIC_BONUS,					TEXT("imagestatic_bonus"));

			file.CreateControl( 	m_pEdit_life2_new,					CLTNSUMMONSTATUS_DLG_EDIT_LIFE2_NEW,					TEXT("editbox_life2_new"));
			file.CreateControl( 	m_pEdit_strength_new,				CLTNSUMMONSTATUS_DLG_EDIT_STRENGTH_NEW,					TEXT("editbox_strength_new"));
			file.CreateControl( 	m_pEdit_dex_new,					CLTNSUMMONSTATUS_DLG_EDIT_DEX_NEW,						TEXT("editbox_dex_new"));
			file.CreateControl( 	m_pEdit_magic_new,					CLTNSUMMONSTATUS_DLG_EDIT_MAGIC_NEW,					TEXT("editbox_magic_new"));
			file.CreateControl( 	m_pEdit_hand_new,					CLTNSUMMONSTATUS_DLG_EDIT_HAND_NEW,						TEXT("editbox_hand_new"));
			file.CreateControl( 	m_pEdit_Wisdom_new,					CLTNSUMMONSTATUS_DLG_EDIT_WISDOM_NEW,					TEXT("editbox_wisdom_new"));

			file.CreateControl( 	m_pOutline_noname3, 				CLTNSUMMONSTATUS_DLG_OUTLINE_NONAME3,					TEXT("outline_NONAME3"));
			file.CreateControl( 	m_pOutline_noname4, 				CLTNSUMMONSTATUS_DLG_OUTLINE_NONAME4,					TEXT("outline_NONAME4"));
			file.CreateControl( 	m_pOutline_noname5, 				CLTNSUMMONSTATUS_DLG_OUTLINE_NONAME5,					TEXT("outline_NONAME5"));
			file.CreateControl( 	m_pOutline_noname6, 				CLTNSUMMONSTATUS_DLG_OUTLINE_NONAME6,					TEXT("outline_NONAME6"));
			
			SetBaseStatusToolTip();			

			m_pStatictext_HP->SetFontSize(11); 
			m_pStatictext_HP->SetFontColor(RGB(255,255,255));
			m_pStatictext_HP->Refresh();
			m_pStatictext_MP->SetFontSize(11);  
			m_pStatictext_MP->SetFontColor(RGB(255,255,255));
			m_pStatictext_MP->Refresh();  
			m_pStatictext_EXP->SetFontSize(11);   
			m_pStatictext_EXP->SetFontColor(RGB(255,255,255));
			m_pStatictext_EXP->Refresh(); 
			m_pStatictext_FULL_EXP->SetFontSize(11);   
			m_pStatictext_FULL_EXP->SetFontColor(RGB(255,255,255));
			m_pStatictext_FULL_EXP->Refresh(); 

			if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) )
			{
				//[진성] 변신 소환수 스태미너.
				m_pStatictext_TransformStamina->SetFontSize(11);   
				m_pStatictext_TransformStamina->SetFontColor(RGB(255,255,255));
				m_pStatictext_TransformStamina->Refresh(); 
			}


			//변경전에 사용하던것은 보이지만 않게 한다.(ddf에서 관련 컨트롤을 삭제하면... 메모리 풕~ 날지도 몰라~.)
			m_pStatictext_exp->Show(false);
			m_pStatictext_expvalue->Show(false);
			m_pStatictext_life->Show(false);
			m_pStatictext_lifevalue->Show(false);
			m_pStatictext_magic1->Show(false);	 
			m_pStatictext_magic1value->Show(false);
			m_pCombobox_strength->Show(false);
			m_pCombobox_dex->Show(false);		
			m_pCombobox_magic->Show(false);	
			m_pCombobox_life2->Show(false);	
			m_pButton_explain->Show(false);	
			m_pButton_revive->Show(false);	
			m_pStatictext_totaldamage->Show(false);	
			m_pStatictext_armourdefense->Show(false);	
			m_pEditbox_totaldamage->Show(false);	
			m_pEditbox_armourdefense->Show(false);	

			m_pButton_init->Show(false);
			m_pCombobox_scroll->Show(false);
			m_pCombobox_dpowername->Show(false);
			m_pCombobox_apowername->Show(false);
			m_pEditbox_health_explain->Show(false);

			m_pStatictext_life2->Show(false);	
			m_pStatictext_strength->Show(false);
			m_pStatictext_dex->Show(false);	
			m_pStatictext_magic->Show(false);	
			m_pStatictext_hand->Show(false);	
			m_pStatictext_Wisdom->Show(false);	

		}
		else
		{
			file.CreateControl( m_pEditbox_health_explain,		CLTNSUMMONSTATUS_DLG_EDITBOX_HEALTH_EXPLAIN,	TEXT("editbox_health_explain") );
			file.CreateControl( m_pStatictext_exp,				CLTNSUMMONSTATUS_DLG_STATICTEXT_EXP,			TEXT("statictext_exp") );
			file.CreateControl( m_pStatictext_life,				CLTNSUMMONSTATUS_DLG_STATICTEXT_LIFE,			TEXT("statictext_life") );
			file.CreateControl( m_pStatictext_expvalue,			CLTNSUMMONSTATUS_DLG_STATICTEXT_EXPVALUE,		TEXT("statictext_expvalue") );
			file.CreateControl( m_pStatictext_magic1,			CLTNSUMMONSTATUS_DLG_STATICTEXT_MAGIC1,			TEXT("statictext_magic1") );
			file.CreateControl( m_pStatictext_magic1value,		CLTNSUMMONSTATUS_DLG_STATICTEXT_MAGIC1VALUE,	TEXT("statictext_magic1value") );
			file.CreateControl( m_pStatictext_lifevalue,		CLTNSUMMONSTATUS_DLG_STATICTEXT_LIFEVALUE,		TEXT("statictext_lifevalue") );
			file.CreateControl( m_pCombobox_life2,				CLTNSUMMONSTATUS_DLG_COMBOBOX_LIFE2,			TEXT("combobox_life2") );
			file.CreateControl( m_pCombobox_magic,				CLTNSUMMONSTATUS_DLG_COMBOBOX_MAGIC,			TEXT("combobox_magic") );
			file.CreateControl( m_pCombobox_dex,				CLTNSUMMONSTATUS_DLG_COMBOBOX_DEX,				TEXT("combobox_dex") );
			file.CreateControl( m_pCombobox_strength,			CLTNSUMMONSTATUS_DLG_COMBOBOX_STRENGTH,			TEXT("combobox_strength") );
			file.CreateControl( m_pCombobox_dpowername,			CLTNSUMMONSTATUS_DLG_COMBOBOX_DPOWERNAME,		TEXT("combobox_dpowername") );
			file.CreateControl( m_pCombobox_apowername,			CLTNSUMMONSTATUS_DLG_COMBOBOX_APOWERNAME,		TEXT("combobox_apowername") );
		}

		file.CreateControl( 	m_pOutline_noname1	, 		CLTNSUMMONSTATUS_DLG_OUTLINE_NONAME1,	 		TEXT("outline_NONAME1")	 )	;
		file.CreateControl( 	m_pOutline_noname2	, 		CLTNSUMMONSTATUS_DLG_OUTLINE_NONAME2, 			TEXT("outline_NONAME2")	 )	;

		m_pStatictext_armourdefense->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_attackattribute->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_bonus->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_dex->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_exp->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_expvalue->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_level->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_levelvalue->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_life->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_life2->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_lifevalue->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_magic->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_magic1->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_magic1value->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_name->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_strength->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_summon->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatictext_totaldamage->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;

		m_pStatictext_armourdefense->Refresh();
		m_pStatictext_attackattribute->Refresh();
		m_pStatictext_bonus->Refresh();
		m_pStatictext_dex->Refresh();
		m_pStatictext_exp->Refresh();
		m_pStatictext_expvalue->Refresh();
		m_pStatictext_level->Refresh();
		m_pStatictext_levelvalue->Refresh();
		m_pStatictext_life->Refresh();
		m_pStatictext_life2->Refresh();
		m_pStatictext_lifevalue->Refresh();
		m_pStatictext_magic->Refresh();
		m_pStatictext_magic1->Refresh();
		m_pStatictext_magic1value->Refresh();
		m_pStatictext_name->Refresh();
		m_pStatictext_strength->Refresh();
		m_pStatictext_summon->Refresh();
		m_pStatictext_totaldamage->Refresh();

		m_pEditbox_life2->SetText("0");
		m_pEditbox_strength->SetText("0");
		m_pEditbox_dex->SetText("0");
		m_pEditbox_magic->SetText("0");
		m_pEditbox_bonus->SetText("0");
		if(pclClient->IsCountrySwitch(Switch_SummonReform))
		{
			m_pEditbox_Wisdom->SetText("0");
			m_pEditbox_hand->SetText("0");

			m_pStatictext_bonus->SetTextPrintOption(DT_CENTER);
			m_pStatictext_bonus->Refresh(); 

			m_pEditbox_bonus->SetTextPrintOption(DT_CENTER);
			m_pEditbox_bonus->Refresh();
		}


		SetTimerOnOff( true );
		// 타이머를 설정한다. 
		BonusSummonButtonEnableSwitch = FALSE;
		//// 글자수를 제한한다.
		//SendDlgItemMessage( hDlg, IDC_EDIT_STATUS_SUMMONNAME, EM_LIMITTEXT, MAX_SUMMON_NAME-1, 0 );

		// SummonStatus 정보들을 처음 초기화 설정한다.
		m_bChangedInitSummonStatus = TRUE;
		m_siLastSummonComboList = 0;

		// PCK - 내 인벤의 아이템 정보를 가져온다.
		m_pPersonItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;

		InitSummonStatusInfo();
		if(pclClient->IsCountrySwitch(Switch_SummonReform))
		{
			if(m_pCombobox_summons->GetItemNum() > 0 )
			{
				m_siLastSummonComboList = 0;
				m_pCombobox_summons->SetCursel( 0 );
			}
		}

		SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
		if(id != 0)	
		{
			cltSummonInfo *pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;
			m_pCheckbox_autoheal->SetCheck( pclSummonInfo->bAutoHeal );

			switch(pclSummonInfo->siSelectAutoHeal)
			{
			case 0 : 
				if(m_pRadiobutton_autoheal30->IsCheck() == false )
				{
					m_pRadiobutton_autoheal30->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON, false ); 
				}
				break;
			case 1 : 
				if(m_pRadiobutton_autoheal50->IsCheck() == false )
				{
					m_pRadiobutton_autoheal50->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON, false ); 
				}
				break;
			case 2 : 
				if(m_pRadiobutton_autoheal70->IsCheck() == false )
				{
					m_pRadiobutton_autoheal70->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON, false ); 
				}
				break;
			}

			if( m_siLastSummonComboList >= 0)
			{
				cltSummon *pclSummon = pclSummonInfo->GetSummon(m_siLastSummonComboList);

				switch(pclSummon->siAttackType)
				{
				case SUMMON_ATTACKTYPE_ACTIVE:
					{
						if(m_pRadiobutton_attack->IsCheck() == false )
							m_pRadiobutton_attack->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON);
					}
					break;
				case SUMMON_ATTACKTYPE_AVOID:	
					{
						if(m_pRadiobutton_wait->IsCheck() == false )
							m_pRadiobutton_wait->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON);
					}
					break;
				}
			}

		}

		UpdateSummonStatusInfo();
		UpdateSummonPortrait();

		Hide();

		if( false == pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
		{
			m_pButton_Ability_Ok->Show(false);	
			m_pButton_Ability_Cancel->Show(false);
		}
	}
	//else
	//{
	//	DeleteDialog();
	//}
}


void CALLBACK cltNSummonStatus::NSummonStatusStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNSummonStatus* pThis = (cltNSummonStatus*)pControllerMgr;

	pThis->NSummonStatusProc( nEvent, nControlID, pControl);
}

// 소환수 
void CALLBACK cltNSummonStatus::NSummonStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;
	SI16 siTmp = 0;

	CControllerMgr *pParent = GetParentDialog();
	if(pParent == NULL ) return;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_ABILITY_OK:
		{
			if ( true == m_clSummon_Use_Bonus.IsZeroAbilityPoint() )
			{
				return;
			}

			CControllerMgr *pParent = this->GetParentDialog();
			if ( NULL == pParent )
			{
				return;
			}

			TCHAR*		pText	= GetTxtFromMgr(6437);
			TCHAR*		pTitle	= GetTxtFromMgr(6438);

			stMsgBox	MsgBox;

			MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_OKCANCEL, 31 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );
		}
		break;

	case CLTNSUMMONSTATUS_DLG_BUTTON_ABILITY_CANCEL:
		{
			UpdateSummonStatusInfo();	
		}
		break;

	case CLTNSUMMONSTATUS_DLG_CHECKBOX_AUTOHEAL:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bool bautoheal  = false ;
					SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
					cltSummonInfo *pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;

					if( m_pCheckbox_autoheal->IsCheck() ==false )
					{
						bautoheal = false;
					}
					else
					{
						bautoheal = true;
					}

					SI16 selectautoheal = pclclient->pclCM->CR[1]->pclCI->clSummonInfo.siSelectAutoHeal ;
					SendSummonAutoHealMessageToServer(selectautoheal,bautoheal) ;
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_RADIOBUTTON_AUTOHEAL30:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pRadiobutton_autoheal30->IsCheck() == true )
						SendSummonAutoHealMessageToServer(0,true) ;
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_RADIOBUTTON_AUTOHEAL50:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pRadiobutton_autoheal50->IsCheck() == true )
						SendSummonAutoHealMessageToServer(1,true) ;
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_RADIOBUTTON_AUTOHEAL70:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pRadiobutton_autoheal70->IsCheck() == true )
						SendSummonAutoHealMessageToServer(2,true) ;
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_HAND_PLUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
					if(GetKeyState(VK_SHIFT) & 0x8000)
					{
						m_clSummon_Use_Bonus.SetHand(10);
					}
					else
					{
						m_clSummon_Use_Bonus.SetHand(1);
					}

					StatusChangeClick( Summon_Bonus_Use::TYPE_HAND );
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_HAND_MINUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
					if(GetKeyState(VK_SHIFT) & 0x8000)
					{
						m_clSummon_Use_Bonus.SetHand(-10);
					}
					else
					{
						m_clSummon_Use_Bonus.SetHand(-1);
					}

					StatusChangeClick( Summon_Bonus_Use::TYPE_HAND );
				}
				break;
			}
		}
		break;	
	case CLTNSUMMONSTATUS_DLG_BUTTON_WISDOM_PLUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
					if(GetKeyState(VK_SHIFT) & 0x8000)
					{
						m_clSummon_Use_Bonus.SetWisdom(10);
					}
					else
					{
						m_clSummon_Use_Bonus.SetWisdom(1);
					}

					StatusChangeClick( Summon_Bonus_Use::TYPE_WISDOM );
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_WISDOM_MINUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
					if(GetKeyState(VK_SHIFT) & 0x8000)
					{
						m_clSummon_Use_Bonus.SetWisdom(-10);
					}
					else
					{
						m_clSummon_Use_Bonus.SetWisdom(-1);
					}

					StatusChangeClick( Summon_Bonus_Use::TYPE_WISDOM );
				}
				break;
			}
		}
		break;

	case CLTNSUMMONSTATUS_DLG_BUTTON_STRENGTH_MINUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				// summon status common logic!!
				{
					if( pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
					{
						// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
						if(GetKeyState(VK_SHIFT) & 0x8000)
						{
							m_clSummon_Use_Bonus.SetStr(-10);
						}
						else
						{
							m_clSummon_Use_Bonus.SetStr(-1);
						}

						StatusChangeClick( Summon_Bonus_Use::TYPE_STR );
					}
					else
					{
						siTmp = 2265;

						TCHAR buffer[256] = TEXT("") ;
						//cyj 상태부분초기화이용권 unique바뀜
						//SI32 itemunique = 14095 ;  // 상태부분초기화이용권
						SI32 itemunique = ITEMUNIQUE(24035) ;  // 상태부분초기화이용권
						if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
						{
							TCHAR* pTitle = GetTxtFromMgr(2835);
							TCHAR* pText = GetTxtFromMgr(604);

							StringCchPrintf(buffer, 256, pText);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);

							return ;
						}

						TCHAR* pText  = GetTxtFromMgr(2836);
						TCHAR* pTitle = GetTxtFromMgr(2835);

						stMsgBox MsgBox;
						// 현재 dialog가 탭 창 같은 Child Dialog일때
						MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 20 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( siTmp );
						memcpy( TempBuffer, &siTmp, Size );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

						//strcpy(buffer, pText);
						//if (  MessageBox(NULL, pTitle,buffer,
						//	MB_ICONQUESTION | MB_YESNO) == IDNO )
						//{
						//	return ;
						//}
						//// leon todo - wParam == nControlID ? 확인 요망
						//SendUseSummonBonusMessageToServer( siTmp,true);
					}
				}

				break;
			default:
				return;
			}
			return;
		}
	case CLTNSUMMONSTATUS_DLG_BUTTON_DEX_MINUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				// summon status common logic!!
				{
					if( pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
					{
						// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
						if(GetKeyState(VK_SHIFT) & 0x8000)
						{
							m_clSummon_Use_Bonus.SetAgi(-10);
						}
						else
						{
							m_clSummon_Use_Bonus.SetAgi(-1);
						}

						StatusChangeClick( Summon_Bonus_Use::TYPE_AGI );
					}					
					else
					{
						siTmp = 1116;

						TCHAR buffer[256] = TEXT("") ;
						//cyj 상태부분초기화이용권 unique바뀜
						//SI32 itemunique = 14095 ;  // 상태부분초기화이용권
						SI32 itemunique = ITEMUNIQUE(24035) ;  // 상태부분초기화이용권
						if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
						{
							TCHAR* pTitle = GetTxtFromMgr(2835);
							TCHAR* pText = GetTxtFromMgr(604);

							StringCchPrintf(buffer, 256, pText);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);

							return ;
						}

						TCHAR* pText = GetTxtFromMgr(2836);
						TCHAR* pTitle = GetTxtFromMgr(2835);

						stMsgBox MsgBox;
						// 현재 dialog가 탭 창 같은 Child Dialog일때
						MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 21 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( siTmp );
						memcpy( TempBuffer, &siTmp, Size );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

						//strcpy(buffer, pText);
						//if (  MessageBox(NULL, buffer,pTitle,
						//	MB_ICONQUESTION | MB_YESNO) == IDNO )
						//{
						//	return ;
						//}
						//// leon todo - wParam == nControlID ? 확인 요망
						//SendUseSummonBonusMessageToServer( siTmp,true);
					}
				}
				break;
			default:
				return;
			}
			return;
		}
	case CLTNSUMMONSTATUS_DLG_BUTTON_MAGIC_MINUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
					{
						// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
						if(GetKeyState(VK_SHIFT) & 0x8000)
						{
							m_clSummon_Use_Bonus.SetWis(-10);
						}
						else
						{
							m_clSummon_Use_Bonus.SetWis(-1);
						}

						StatusChangeClick( Summon_Bonus_Use::TYPE_WIS );
					}
					else
					{
						siTmp = 1118;

						TCHAR buffer[256] = TEXT("") ;
						//cyj 상태부분초기화이용권 unique바뀜
						//SI32 itemunique = 14095 ;  // 상태부분초기화이용권
						SI32 itemunique = ITEMUNIQUE(24035) ;  // 상태부분초기화이용권
						if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
						{
							TCHAR* pTitle = GetTxtFromMgr(2835);
							TCHAR* pText = GetTxtFromMgr(604);

							StringCchPrintf(buffer, 256, pText);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);

							return ;
						}

						TCHAR* pText = GetTxtFromMgr(2836);
						TCHAR* pTitle = GetTxtFromMgr(2835);

						stMsgBox MsgBox;
						// 현재 dialog가 탭 창 같은 Child Dialog일때
						MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 22 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( siTmp );
						memcpy( TempBuffer, &siTmp, Size );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
						//strcpy(buffer, pText);
						//if (  MessageBox(NULL, buffer,pTitle,
						//	MB_ICONQUESTION | MB_YESNO) == IDNO )
						//{
						//	return ;
						//}
						//// leon todo - wParam == nControlID ? 확인 요망
						//SendUseSummonBonusMessageToServer( siTmp,true);
					}
				}
				break;
			default:
				return;
			}
			return;
		}
	case CLTNSUMMONSTATUS_DLG_BUTTON_LIFE2_MINUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
					{
						// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
						if(GetKeyState(VK_SHIFT) & 0x8000)
						{
							m_clSummon_Use_Bonus.SetVit(-10);
						}
						else
						{
							m_clSummon_Use_Bonus.SetVit(-1);
						}

						StatusChangeClick( Summon_Bonus_Use::TYPE_VIT );
					}	
					else
					{
						siTmp = 1119;
						TCHAR buffer[256] = TEXT("") ;
						//cyj 상태부분초기화이용권 unique바뀜
						//SI32 itemunique = 14095 ;  // 상태부분초기화이용권
						SI32 itemunique = ITEMUNIQUE(24035) ;  // 상태부분초기화이용권
						if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
						{
							TCHAR* pTitle = GetTxtFromMgr(2835);
							TCHAR* pText = GetTxtFromMgr(604);

							StringCchPrintf(buffer, 256, pText);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);

							return ;
						}

						TCHAR* pText = GetTxtFromMgr(2836);
						TCHAR* pTitle = GetTxtFromMgr(2835);

						stMsgBox MsgBox;
						// 현재 dialog가 탭 창 같은 Child Dialog일때
						MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 23 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( siTmp );
						memcpy( TempBuffer, &siTmp, Size );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
						//strcpy(buffer, pText);
						//if (  MessageBox(NULL, buffer,pTitle,
						//	MB_ICONQUESTION | MB_YESNO) == IDNO )
						//{
						//	return ;
						//}
						//// leon todo - wParam == nControlID ? 확인 요망
						//SendUseSummonBonusMessageToServer( siTmp,true);
					}
				}
				break;
			default:
				return;
			}
			return;
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_INIT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SendSummonStatusInitMessageToServer();
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_STRENGTH_PLUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
					{
						// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
						if(GetKeyState(VK_SHIFT) & 0x8000)
						{
							m_clSummon_Use_Bonus.SetStr(10);
						}
						else
						{
							m_clSummon_Use_Bonus.SetStr(1);
						}

						StatusChangeClick( Summon_Bonus_Use::TYPE_STR );
					}
					else
					{
						SendUseSummonBonusMessageToServer(0);	return ;
					}
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_DEX_PLUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
					{
						// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
						if(GetKeyState(VK_SHIFT) & 0x8000)
						{
							m_clSummon_Use_Bonus.SetAgi(10);
						}
						else
						{
							m_clSummon_Use_Bonus.SetAgi(1);
						}

						StatusChangeClick( Summon_Bonus_Use::TYPE_AGI );
					}
					else
					{
						SendUseSummonBonusMessageToServer(1);	return ;
					}
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_MAGIC_PLUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
					{
						// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
						if(GetKeyState(VK_SHIFT) & 0x8000)
						{
							m_clSummon_Use_Bonus.SetWis(10);
						}
						else
						{
							m_clSummon_Use_Bonus.SetWis(1);
						}

						StatusChangeClick( Summon_Bonus_Use::TYPE_WIS );
					}
					else
					{
						SendUseSummonBonusMessageToServer(2);	return ;
					}
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_LIFE2_PLUS:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
					{
						// Shift키를 누른상태에서 스탯 분배버튼을 누르면 10씩 감소한다.
						if(GetKeyState(VK_SHIFT) & 0x8000)
						{
							m_clSummon_Use_Bonus.SetVit(10);
						}
						else
						{
							m_clSummon_Use_Bonus.SetVit(1);
						}

						StatusChangeClick( Summon_Bonus_Use::TYPE_VIT );
					}
					else
					{
						SendUseSummonBonusMessageToServer(3);	return ;
					}
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_RADIOBUTTON_ATTACK:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pRadiobutton_attack->IsCheck() == true )
						SendSummonAttackTypeMessageToServer(SUMMON_ATTACKTYPE_ACTIVE);
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_RADIOBUTTON_WAIT:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pRadiobutton_wait->IsCheck() == true )
						SendSummonAttackTypeMessageToServer(SUMMON_ATTACKTYPE_AVOID);
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_CHANGENAME:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SendSummonChangeNameMessageToServer();
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_TAKEINOUT:		// 일단 부활시키는데 사용한다.
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SendSummonTakeInOutMessageToServer(-1);
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_FREE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SendSummonFreeMessageToServer();
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_REVIVE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SendSummonReviveMessageToServer();
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_EXPLAIN:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{

					//if ( g_SoundOnOffDlg.m_bFullScreenMode )
					//{
					pclclient->ShowHelpWindow(TEXT("summonbreed"));
					//if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
					//{
					//	pclclient->CreateInterface( NHINT_DLG );
					//}
					//((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(TEXT("summonbreed"));

					//}
					//else
					//pclclient->m_pHintDlg->Show(TEXT("summonbreed"));
				}
				break;
			}
		}
		break;			
	case CLTNSUMMONSTATUS_DLG_COMBOBOX_SUMMONS:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					if(m_pCombobox_summons && m_pCombobox_summons->GetItemNum() > 0)
					{
						TCHAR *pTmpChar = NULL;
						m_siLastSummonComboList = m_pCombobox_summons->GetSelectedIndex(); 

						UpdateSummonStatusInfo();
						UpdateSummonPortrait();

						//=======================================================
						// 소환수 이름 재 갱신 
						//=======================================================
						TCHAR SummonName[256]= TEXT("");
						SI32 SummonIndex = 0;

						//pTmpChar = m_pCombobox_summons->GetText(m_siLastSummonComboList);
						//if(pTmpChar)
						//{
						//	strcpy( SummonName, pTmpchar );
						//	m_pEditbox_name->SetText( SummonName );
						//}
						if(GetComboSummonNameIndex( SummonName, 256, &SummonIndex))
						{
							m_pEditbox_name->SetText( SummonName );
						}
					}
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_MYSUMMON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//소환수 보관 시스템 테스트
					pclclient->CreateInterface( NMYSUMMON_DLG );
				}
				break;
			}
		}
		break;
	case CLTNSUMMONSTATUS_DLG_BUTTON_SUMMON_TRANSFORM:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_Summon_Transform clInfo;
					cltMsg clMsg( GAMEMSG_REQUEST_SUMMON_TRANSFORM, sizeof(clInfo), (BYTE*)&clInfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
		
		// PCK : 소환수 스태미너 충전 버튼 눌렀을때 처리 (08.07.16)
	case CLTNSUMMONSTATUS_DLG_BUTTION_SPTCHARGE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 itemunique	= ITEMUNIQUE(13625);
					SI32 pos = m_pPersonItemInfo->GetItemPos(itemunique);

					TCHAR SummonName[256]=TEXT("");
					SI32 SummonIndex = 0;

					cltSummonInfo *pclSummonInfo = &pclCM->CR[1]->pclCI->clSummonInfo;
					if( pclSummonInfo != NULL && pclSummonInfo->HaveSummon() )
					{
						if(GetComboSummonNameIndex( SummonName, 256, &SummonIndex))
						{
							cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);							
							if( pclSummon != NULL && pclClient->pclKindInfoSet->IsValidKind(pclSummon->siKind) )
							{
								cltKindInfo* pclKI = NULL;
								if( pclClient->IsCountrySwitch(Switch_Summon_Separate) )
								{	pclKI = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];		}
								else
								{	pclKI = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];			}

								if (pclKI == NULL)								 {		return;			}
								if (pclSummon->siStatus == SUMMON_STATUS_DEAD)	 {		return;			}

								SI32 summonskill = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 );

								SI32 amt = 0;
								// PCK : ATB_SUMMONSTAMINASTATIC이 있으면 기본 소환수 스태미너감소량을 적용시킨다. (08.07.18)
								if (pclKI->IsAtb(ATB_SUMMONSTAMINASTATIC) == TRUE )
								{
									amt = SUMMON_DECREASESTAMINA_AMT;
								}
								else
								{
									amt =  SUMMON_DECREASESTAMINA_AMT - (pclKI->siSummonRank / 5) ;

									// 소모되는 스태미너의 양이 0보다 작아 지면 1로 고정시킨다.
									if(amt <= 0) { amt = 1; }
								}

								// 최대 스태미너 시간을 구한다.
								SI32 siStaminaminute = 0;
								siStaminaminute = pclClient->GetMaxStaminaValue(summonskill, pclSummon->siKind) / amt;

								// 소환수의 스테미너가 없는 경우만 충전이 가능하다.
								if( pclSummon->clIP.GetLuk() <= 0 )
								{
									// 소환수의 스테미너 충전 아이템 확인
									if( pos > 0 )
									{
										NTCHARString512	kText(GetTxtFromMgr(8585));
										NTCHARString512	kTitle(GetTxtFromMgr(736));

										kText.Replace("#summonname#", pclSummon->szName);
										kText.Replace("#staminaminute#", SI32ToString(siStaminaminute));

										stMsgBox MsgBox;
										MsgBox.Set( pParent, kTitle, kText, MSGBOX_TYPE_YESNO, 29 );

										BYTE TempBuffer[ 32 ];
										SI16 Size = sizeof( pclSummon->siSummonID );
										memcpy( TempBuffer, &pclSummon->siSummonID, Size );
										pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
									}
									else
									{
										TCHAR* pText = GetTxtFromMgr(8587);
										TCHAR* pTitle = GetTxtFromMgr(736);

										stMsgBox MsgBox;
										MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_OK, 31 );
										pclclient->SetMsgBox( &MsgBox, NULL, 0 );
									}
								}
								// 소환수 스태미너가 있을 경우
								else
								{
									NTCHARString512	kText(GetTxtFromMgr(8586));
									NTCHARString512	kTitle(GetTxtFromMgr(736));

									stMsgBox MsgBox;
									MsgBox.Set( pParent, kTitle, kText, MSGBOX_TYPE_OK, 30 );
									pclclient->SetMsgBox( &MsgBox, NULL, 0 );
								}
							}
						}
					}
				}
				break;
			}
		}
		break;
	}
}


void cltNSummonStatus::UpdateSummonStatusInfo()
{
	TCHAR buffer[256];
	TCHAR* pTmpChar = NULL;
	BOOL bSummonExist= FALSE;


	SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
	if(id == 0)	{return ;}

	//==============================================
	// 소환수가 없으면 돌아간다.
	//==============================================
	cltClient*   pclclient = (cltClient*)pclClient;
	// 소환수정보에만 의존한다.
	cltSummonInfo *pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;
	if(pclSummonInfo == NULL) 
	{
		return;
	}

	if ( pclSummonInfo->bAutoHeal )
	{
		m_pCheckbox_autoheal->SetCheck( true, CONTROLLER_KIND_CHECKBOX, false );			

		m_pRadiobutton_autoheal30->Enable( true );
		m_pRadiobutton_autoheal50->Enable( true );
		m_pRadiobutton_autoheal70->Enable( true );
	}
	else
	{
		m_pCheckbox_autoheal->SetCheck( false, CONTROLLER_KIND_CHECKBOX, false );			

		m_pRadiobutton_autoheal30->Enable( false );
		m_pRadiobutton_autoheal50->Enable( false );
		m_pRadiobutton_autoheal70->Enable( false );
	}

	if(pclSummonInfo->HaveSummon())
	{
		//=======================================
		// 현재 ComboList의 소환수를 얻어온다.
		//=======================================
		TCHAR SummonName[256]=TEXT("");
		SI32 SummonIndex = 0;
		//SummonIndex = m_pCombobox_summons->GetSelectedIndex();
		//pTmpChar = m_pCombobox_summons->GetText(SummonIndex);
		//
		//if(pTmpChar)
		//{
		//	strcpy( SummonName, pTmpChar );
		if(GetComboSummonNameIndex( SummonName, 256, &SummonIndex))
		{
			cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
			// 소환력이 없어서 회복중일때는
			if (pclSummon->clIP.GetLuk() <= 0)
			{
				// PCK : ATB_SUMMONSTAMINASTATIC가 있으면 회복시간 표시 안함.
				BOOL bSummonStaminaRecovery = TRUE;

				cltKindInfo*	pclKI = NULL;
				if(pclClient->IsCountrySwitch(Switch_Summon_Separate))
				{	pclKI = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];	}
				else
				{	pclKI = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];			}

				if (pclKI != NULL && pclKI->IsAtb(ATB_SUMMONSTAMINASTATIC) == TRUE)
				{
					bSummonStaminaRecovery = FALSE;
				}

				SI32 clientminute = pclClient->GetMinuteFromsTime(pclclient->sTime);
				SI32 minute = TABS(pclSummon->siTakeInMinute - clientminute);

				if(bSummonStaminaRecovery)
				{
					if (minute < 20)
					{
						TCHAR* pText = GetTxtFromMgr(6299);
						TCHAR buf[256];

						// 남은회복시간 표시
						StringCchPrintf(buf, 256, pText, 20 - minute);
						m_pStatictext_summon->SetText( buf, DT_LEFT|DT_VCENTER|DT_SINGLELINE  );

					}
				}
				else
				{
					// 스태미너를 충전해야한다는 메시지를 나오게함

					TCHAR* pText = GetTxtFromMgr(8619);
					TCHAR buf[256];
					StringCchPrintf(buf, 256, pText);
					m_pStatictext_summon->SetText(buf, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
				}
			}
			// 소환력이 있을때는
			else
			{
				SI32 amt = 0;
				// 소모되는 스태미너의 양을 구한다.

				cltKindInfo* pclKI = NULL;
				if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
				{
					pclKI = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
				}
				else
				{
					pclKI = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
				}
				if (pclKI)
				{
					// PCK : ATB_SUMMONSTAMINASTATIC이 있으면 기본 소환수 스태미너감소량을 적용시킨다. (08.07.18)
					if (pclKI->IsAtb(ATB_SUMMONSTAMINASTATIC) == TRUE )
					{
						amt = SUMMON_DECREASESTAMINA_AMT;
					}
					else
					{
						// 소모되는 스태미너의 양을 구한다.
						amt =  SUMMON_DECREASESTAMINA_AMT - (pclKI->siSummonRank / 5) ;

						// 소모되는 스태미너의 양이 0보다 작아 지면 1로 고정시킨다.
						if(amt <= 0) { amt = 1; }
					}

					SI32 minute = 0;
					minute = pclSummon->clIP.GetLuk() / amt;

					TCHAR* pText = GetTxtFromMgr(6298);
					TCHAR buf[256];

					// 남은소환시간 표시
					StringCchPrintf(buf, 256, pText, minute);
					m_pStatictext_summon->SetText( buf, DT_LEFT|DT_VCENTER|DT_SINGLELINE  );
				}

			}

			bSummonExist = TRUE;

			/*
			NTCHARString1024 kBuffer(TEXT(""));

			if(pclClient->IsCountrySwitch(Switch_Summon_EquipSton))
			{
				// 소환수 무기 장착석
				cltKindInfo* pclKI = NULL;
				if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
				{
					pclKI = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
				}
				else
				{
					pclKI = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
				}

				if (pclSummon != NULL && pclKI !=NULL && pclKI->siSummonEquip != SUMMON_EQUIPTYPE_NONE ) 
				{
					NTCHARString1024	kText1(GetTxtFromMgr(8643));
					kText1.Replace(TEXT("#Weaponstone#"),SI16ToString(pclSummon->siAttackWearStoneNum) );	// 무기석
					kText1.Replace(TEXT("#ArmorStone#"),SI16ToString(pclSummon->siDefenseWearStoneNum) );	// 방어구석
					StringCchCat(kBuffer, 1024, kText1);				
				}
			}

			// PCK : 소환수 채팅커맨드를 보여준다.
			SI32 siSummonLevel	=	pclSummon->clIP.siLevel;	// 소환수 레벨
			SI32 siSummonKind	=	pclSummon->siKind;			// 소환수 카인드

			for(SI32 i = 0;i < MAX_SUMMON_CHATCOMMANDNUMBER;i++)
			{
				if( pclClient->pclKindInfoSet->pclSummonChatCommand[i] == NULL )	continue;

				if(siSummonKind == pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siCharKind)
				{
					TCHAR* pChatAction = GetTxtFromMgr(pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siChatTextLine);

					if(siSummonLevel >= pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siLevel)
					{
						NTCHARString1024	kText2( GetTxtFromMgr(9747) );
						kText2.Replace(TEXT("#level#"),SI16ToString(pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siLevel) );
						kText2.Replace(TEXT("#command#"),pChatAction );
						StringCchCat(kBuffer, 024, kText2);
					}
				}
			}
			
			TCHAR* pszCompareText = m_pEditbox_health_explain->GetText();

			if( pszCompareText )
			{
				if(kBuffer.Compare(pszCompareText) != 0)
				{
					m_pEditbox_health_explain->SetText(kBuffer);
				}
			}
			else
			{
				m_pEditbox_health_explain->SetText(kBuffer);
			}
			*/
		}
		else 
			return;
	}

	//SI32 ButtonDlg[] = {IDC_BUTTON_STR, IDC_BUTTON_DEX,IDC_BUTTON_INT,IDC_BUTTON_VIT,-1};

	//==============================================
	// 소환수가 존재하면 
	//==============================================
	if(bSummonExist)
	{
		//=======================================
		// 현재 ComboList의 소환수를 얻어온다.
		//=======================================
		TCHAR SummonName[256]=TEXT("");
		SI32 SummonIndex = 0;
		if (GetComboSummonNameIndex( SummonName, 256, &SummonIndex ) == FALSE)	return;
		if ( (0 > SummonIndex) || (MAX_SUMMON_OWN <= SummonIndex) )				return;

		cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
		if(pclSummon==NULL) return;

		cltParametaBox clPB;	
		//cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
		cltKindInfo* pclki = NULL;
		cltGMCommandKindInfo* pclGMki = NULL;

		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			pclki = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
			pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclSummon->siKind];
		}
		else
		{
			pclki = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
			pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclSummon->siKind];
		}

		if(pclki == NULL )
			return;

		if(pclGMki == NULL)
			return;

		cltPIPInfo			clipinfo2;
		clipinfo2.Set(&pclSummon->clIP);
		clPB.Init(pclClient->pclItemManager, &clipinfo2, NULL, pclki, pclGMki);

		//--------------------------------------
		// 소환수용 장비를 확보한다. 
		//--------------------------------------
		cltItem* pclitemlist = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonEquipItemList( SummonIndex );
		if ( NULL == pclitemlist )
		{
			return;
		}

		SI16 siAttackWearStoneNum	= pclSummon->siAttackWearStoneNum;
		SI16 siDefenseWearStoneNum	= pclSummon->siDefenseWearStoneNum;
		SI32 siSummonID				= pclCM->CR[id]->GetSummonID();
		bool bSummoed				= false;

		// 현재 나와있는 소환수라면 장비에 의한 능력치까지 계산한다
		if ( (pclSummon->siStatus == SUMMON_STATUS_TAKEOUT) && (true == pclCM->IsValidID( siSummonID )) )
		{
			cltCharCommon* pclSummonCommon = (cltCharCommon*)pclCM->CR[siSummonID];
			if ( NULL == pclSummonCommon )
			{
				return;
			}

			clPB.Action(pclSummonCommon, 0, PERSONITEM_WEAPON1, pclitemlist, &pclSummonCommon->pclCI->clHorseInfo, &pclSummonCommon->pclCI->clHealthInfo, &pclSummonCommon->pclCI->clCharSkill, &pclSummonCommon->clIdentity, siAttackWearStoneNum, siDefenseWearStoneNum, GAMEMODE_CLIENT, true );

			bSummoed = true;

		}
		else
		{
			clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, siAttackWearStoneNum, siDefenseWearStoneNum, GAMEMODE_CLIENT, true );
		}

		//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
		if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
		{
			StringCchPrintf(buffer, 256, TEXT("%d / %d"), pclSummon->m_siTranformStamina / 10, 60);
			m_pStatictext_TransformStamina->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );		
		}

		//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
		if(pclClient->IsCountrySwitch(Switch_SummonReform))
		{	
			StringCchPrintf(buffer, 256, TEXT("%d / %d"), pclSummon->clIP.GetLife(), clPB.GetMaxLife());
			m_pStatictext_HP->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

			StringCchPrintf(buffer, 256, TEXT("%d / %d"), pclSummon->clIP.GetMana(), clPB.GetMaxMana());
			m_pStatictext_MP->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );		

			StringCchPrintf(buffer, 256, TEXT("%d"), pclSummon->clIP.GetCurExp());
			m_pStatictext_EXP->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );		

			StringCchPrintf(buffer, 256, TEXT("%d"), clPB.GetNextExp());
			m_pStatictext_FULL_EXP->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		}


		// 레벨
		TCHAR* pText = GetTxtFromMgr(624);
		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			//cltKindInfo* pclSummonKi = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
			cltKindInfo* pclSummonKi = NULL;
			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				pclSummonKi = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
			}
			else
			{
				pclSummonKi = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
			}
			if(pclSummonKi)
			{
				StringCchPrintf(buffer, 256, pText, pclSummon->clIP.GetLevel(), MAX_CHARACTER_LEVEL, pclSummonKi->siEvolveLevel);
				m_pStatictext_levelvalue->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE  );
			}
		}
		else
		{
			StringCchPrintf(buffer, 256, pText, pclSummon->clIP.GetLevel(), MAX_CHARACTER_LEVEL, pclki->siEvolveLevel);
			m_pStatictext_levelvalue->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE  );
		}

		// 경험치
		StringCchPrintf(buffer, 256, TEXT("%I64d / %I64d"), pclSummon->clIP.GetCurExp(), clPB.GetNextExp());
		m_pStatictext_expvalue->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE  );		

		// 체력
		StringCchPrintf(buffer, 256, TEXT("%d / %d"), pclSummon->clIP.GetLife(), clPB.GetMaxLife());
		m_pStatictext_lifevalue->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE  );

		// 마법력 
		StringCchPrintf(buffer, 256, TEXT("%d / %d"), pclSummon->clIP.GetMana(), clPB.GetMaxMana());
		m_pStatictext_magic1value->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE  );
		//==============================================
		// 
		//==============================================
		// 근력
		StringCchPrintf(buffer, 256, TEXT("%d"), clPB.clTotalBA.GetStr() );
		m_pEditbox_strength->SetText( buffer );
		// 근력력 상세 정보를 설정한다. 
		ShowDetailStrInfo(&clPB);

		m_clSummon_Use_Bonus.Clear();
		//[추가 : 황진성 2008. 2. 19 => 소환수 능력치에 사용 가능한 보너스 세팅. ]
		m_clSummon_Use_Bonus.SetPreStr( atoi( buffer ) );

		//  순발력
		StringCchPrintf(buffer, 256, TEXT("%d"), clPB.clTotalBA.GetDex() );
		m_pEditbox_dex->SetText( buffer );
		// 순발력 상세 정보를 설정한다. 
		ShowDetailDexInfo(&clPB);

		//[추가 : 황진성 2008. 2. 19 => 소환수 능력치에 세팅. ]
		m_clSummon_Use_Bonus.SetPreAgi( atoi( buffer ) );

		// 도력
		StringCchPrintf(buffer, 256, TEXT("%d"), clPB.clTotalBA.GetMag() );
		m_pEditbox_magic->SetText( buffer );		
		// 도력 상세 정보를 설정한다. 
		ShowDetailMagInfo(&clPB);

		//[추가 : 황진성 2008. 2. 19 => 소환수 능력치에 세팅. ]
		m_clSummon_Use_Bonus.SetPreWis( atoi( buffer ) );

		// 생명력 
		StringCchPrintf(buffer, 256, TEXT("%d"), clPB.clTotalBA.GetVit() );
		m_pEditbox_life2->SetText( buffer );
		// 생명력 상세 정보를 설정한다. 
		ShowDetailVitInfo(&clPB);
		ShowDetailHndInfo(&clPB); 
		ShowDetailWisdomInfo(&clPB);

		//[추가 : 황진성 2008. 2. 19 => 소환수 능력치에 세팅. ]
		m_clSummon_Use_Bonus.SetPreVit( atoi( buffer ) );

		// 전투보너스  
		StringCchPrintf(buffer, 256, TEXT("%d"), pclSummon->clIP.GetBonus() );
		m_pEditbox_bonus->SetText( buffer );

		//[추가 : 황진성 2008. 2. 19 => 소환수 능력치에 사용 가능한 보너스 세팅. ]
		m_clSummon_Use_Bonus.SetUseBonus( atoi( buffer ) );

		//[진성]  2008-3-24	
		if(pclClient->IsCountrySwitch(Switch_SummonReform))
		{
			StringCchPrintf(buffer, 256, TEXT("%d"), clPB.clTotalBA.GetWis() );
			m_pEditbox_Wisdom->SetText( buffer );

			//[추가 : 황진성 2008. 2. 19 => 소환수 능력치 세팅. ]
			m_clSummon_Use_Bonus.SetPreWisdom( atoi( buffer ) );

			StringCchPrintf(buffer, 256, TEXT("%d"), clPB.clTotalBA.GetHnd() );
			m_pEditbox_hand->SetText( buffer );

			//[추가 : 황진성 2008. 2. 19 => 소환수 능력치 세팅. ]
			m_clSummon_Use_Bonus.SetPreHand( atoi( buffer ) );

			SetNewAbility( &clPB );
		}


		//==============================================
		// 이동속도 
		//==============================================
		/*
		hwnd = GetDlgItem(hDlg, IDC_EDIT_TOTALSPEED); 
		sprintf(buffer, TEXT("%d"), clPB.TotalMoveSpeed);
		SetWindowText(hwnd, buffer);

		// 이동속도 상세 정보를 설정한다. 
		ShowDetailSpeedInfo(hDlg, &clPB, NULL);
		*/

			SI32 totalMinDamage = clPB.GetTotalMinDamage();
		SI32 totalMaxDamage = clPB.GetTotalMaxDamage();

		/*
		// 소환수는 기본적으로 공격력 + 10% 의 보너스를 받는다
		totalMinDamage = totalMinDamage + (SI32)(totalMinDamage * 0.1f);
		totalMaxDamage = totalMaxDamage + (SI32)(totalMaxDamage * 0.1f);

		SI32 summonskill = 0;
		summonskill = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 );

		SI32 attackBonusPercent = 0;
		attackBonusPercent = pclClient->GetSummonAttackBonus( summonskill );

		// 소환술에 따라 공격력 보너스를 받는다.
		if( attackBonusPercent > 0 )
		{
			totalMinDamage = totalMinDamage + totalMinDamage * attackBonusPercent / 100;
			totalMaxDamage = totalMaxDamage + totalMaxDamage * attackBonusPercent / 100;
		}

		SI32 bonusPercent = 0;

		// 버프에 따라 공격력 보너스를 받는다.
		if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUMMONATACUP) == true )
		{
			bonusPercent = max( bonusPercent, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_SUMMONATACUP) );
		}
		if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) == true )
		{
			bonusPercent = max( bonusPercent, 20 );
		}

		totalMinDamage += totalMinDamage * bonusPercent / 100;
		totalMaxDamage += totalMaxDamage * bonusPercent / 100;
		*/

			// 공격력  
			StringCchPrintf(buffer, 256, TEXT("%d - %d"), totalMinDamage ,  totalMaxDamage );
		m_pEditbox_totaldamage->SetText( buffer );

		// 공격력 상세 정보를 설정한다. 
		if ( bSummoed )
		{
			ShowDetailDamageInfo( &clPB, pclitemlist, PERSONITEM_WEAPON1, pclSummon);
		}
		else
		{
			m_pCombobox_apowername->Clear();
		}

		SI32 totalAC = clPB.GetTotalAC();
		/*
		SI32 defBonusPercent = 0;
		defBonusPercent = pclClient->GetSummonDefBonus( summonskill );

		if( defBonusPercent > 0 )
		{
			totalAC += totalAC * defBonusPercent / 100;
		}

		bonusPercent = 0;
		// 버프에 따라 공격력 보너스를 받는다.
		if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUMMONATACUP) == true )
		{
			bonusPercent = max( bonusPercent, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_SUMMONATACUP) );
		}
		if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) == true )
		{
			bonusPercent = max( bonusPercent, 20 );
		}

		totalAC += totalAC * bonusPercent / 100;

		*/
			// 방어력  
			StringCchPrintf(buffer, 256, TEXT("%d"), totalAC );
		m_pEditbox_armourdefense->SetText( buffer );

		// 방어력 상세 정보를 설정한다. 
		if ( bSummoed )
		{
			ShowDetailArmourInfo(&clPB);
		}
		else
		{
			m_pCombobox_dpowername->Clear();
		}

		//==============================================
		// 보너스 버튼 
		//==============================================
		SI32 index = 0;
		// 보너스 수치 조정용 버튼을 설정한다. 
		if(pclSummon->clIP.GetBonus())
		{
			if(BonusSummonButtonEnableSwitch == FALSE)
			{
				BonusSummonButtonEnableSwitch = TRUE;
				//index = 0;
				//while(ButtonDlg[index] >0)
				//{
				//	hwnd = GetDlgItem(hDlg, ButtonDlg[index]); 
				//	EnableWindow(hwnd, TRUE);
				//	index++;
				//}
			}
			m_pButton_strength_plus->Enable( true );
			m_pButton_dex_plus->Enable( true );
			m_pButton_magic_plus->Enable( true );
			m_pButton_life2_plus->Enable( true );

			//[진성]  2008-3-24	
			if(pclClient->IsCountrySwitch(Switch_SummonReform))
			{
				m_pButton_wisdom_plus->Enable( true );
				m_pButton_hand_plus->Enable( true );
			}
		}
		else
		{
			if(BonusSummonButtonEnableSwitch == TRUE)
			{
				BonusSummonButtonEnableSwitch = FALSE;
				//index = 0;
				//while(ButtonDlg[index] >0)
				//{
				//	hwnd = GetDlgItem(hDlg, ButtonDlg[index]); 
				//	EnableWindow(hwnd, FALSE);
				//	index++;
				//}
			}
			m_pButton_strength_plus->Enable( false );
			m_pButton_dex_plus->Enable( false );
			m_pButton_magic_plus->Enable( false );
			m_pButton_life2_plus->Enable( false );

			//[진성]  2008-3-24	
			if(pclClient->IsCountrySwitch(Switch_SummonReform))
			{
				m_pButton_wisdom_plus->Enable( false );
				m_pButton_hand_plus->Enable( false );
			}
		}

		//==============================================
		// 
		//==============================================
		// 레벨
		switch(pclSummon->siStatus)
		{
		case SUMMON_STATUS_TAKEIN:
			{
				switch(SummonIndex)
				{
				case 0: 
					{
						TCHAR* pText = GetTxtFromMgr(625);
						StringCchCopy(buffer, 256, pText);		
					}
					break;
				case 1:
					{
						TCHAR* pText = GetTxtFromMgr(626);
						StringCchCopy(buffer, 256, pText);		
					}
					break;
				default:
					{
						TCHAR* pText = GetTxtFromMgr(627);
						StringCchCopy(buffer, 256, pText);
					}
					break;
				}
			}
			break;
		case SUMMON_STATUS_TAKEOUT:	
			{
				if( pclSummon->clIP.GetLife() > 0 ) 
				{
					switch(SummonIndex)
					{
					case 0:
						{
							TCHAR* pText = GetTxtFromMgr(628);
							StringCchCopy(buffer, 256, pText);		
						}
						break;
					case 1:
						{
							TCHAR* pText = GetTxtFromMgr(629);
							StringCchCopy(buffer, 256, pText);
						}
						break;
					default:
						{
							TCHAR* pText = GetTxtFromMgr(630);
							StringCchCopy(buffer, 256, pText);
						}
						break;
					}
				}
				else
				{
					TCHAR* pText = GetTxtFromMgr(631);
					StringCchCopy(buffer, 256, pText);
				}
			}
			break;
		case SUMMON_STATUS_DEAD:	//strcpy(buffer, TEXT("부활"));		break;
		default:	
			{
				TCHAR* pText = GetTxtFromMgr(631);
				StringCchCopy(buffer, 256, pText);	
			}break;
		}

		m_pButton_takeinout->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

		m_pRadiobutton_attack->Enable( true );
		m_pRadiobutton_wait->Enable( true );

		// 소환수 회수 상황일때 공격/대기 콤보를 누르면 원상태로 회복한다.
		switch(pclSummon->siAttackType)
		{
		case SUMMON_ATTACKTYPE_ACTIVE:
			{
				if(m_pRadiobutton_attack->IsCheck() == false )
				{
					m_pRadiobutton_wait->SetCheck( false, CONTROLLER_KIND_RADIOBUTTON);
					m_pRadiobutton_attack->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON);
				}
			}
			break;
		case SUMMON_ATTACKTYPE_AVOID:	
			{
				if(m_pRadiobutton_wait->IsCheck() == false )
				{
					m_pRadiobutton_attack->SetCheck( false, CONTROLLER_KIND_RADIOBUTTON);
					m_pRadiobutton_wait->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON);
				}
			}
			break;
		}

		TCHAR szScroolInfo[512] = {0,};

		ShowDetailScrollInfo(&pclSummon->clScroll, szScroolInfo, 512);

		NTCHARString1024 kBuffer(szScroolInfo);

		if(pclClient->IsCountrySwitch(Switch_Summon_EquipSton))
		{// 소환수 무기 장착석{
			cltKindInfo* pclKI = NULL;
			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				pclKI = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
			}
			else
			{
				pclKI = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
			}
			if (pclSummon != NULL && pclKI != NULL && pclKI->siSummonEquip != SUMMON_EQUIPTYPE_NONE ) {
				NTCHARString1024	kText1(GetTxtFromMgr(8643));
				kText1.Replace(TEXT("#Weaponstone#"),SI16ToString(pclSummon->siAttackWearStoneNum) )	;	// 무기석
				kText1.Replace(TEXT("#ArmorStone#"),SI16ToString(pclSummon->siDefenseWearStoneNum) )		;	// 방어구석
				StringCchCat(kBuffer, 1024, kText1);
			}

		}

		// PCK : 소환수 채팅커맨드를 보여준다.
		SI32 siSummonLevel	=	pclSummon->clIP.siLevel;	// 소환수 레벨
		SI32 siSummonKind	=	pclSummon->siKind;			// 소환수 카인드

		for(SI32 i = 0;i < MAX_SUMMON_CHATCOMMANDNUMBER;i++)
		{
			if( pclClient->pclKindInfoSet->pclSummonChatCommand[i] == NULL )	continue;

			if(siSummonKind == pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siCharKind)
			{
				TCHAR* pChatAction = GetTxtFromMgr(pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siChatTextLine);

				if(siSummonLevel >= pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siLevel)
				{
					NTCHARString1024	kText2( GetTxtFromMgr(9747) );
					kText2.Replace(TEXT("#level#"),SI16ToString(pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siLevel) );
					kText2.Replace(TEXT("#command#"),pChatAction );
					StringCchCat(kBuffer, 1024, kText2);
				}
			}
		}

		// 내용이 변경되지 않으면 기존에 스크롤 위치로
		if ( kBuffer.Compare(szScroolInfo) != 0 ) 
		{
			m_pEditbox_health_explain->SetText(kBuffer);
		}
	}
	else
	{
		StringCchCopy(buffer, 256, TEXT(""));
		// 이름 
		m_pEditbox_name->SetText( buffer );
		StringCchCopy(strSummonName, MAX_SUMMON_NAME, TEXT(""));

		m_pCombobox_summons->Clear();

		// 레벨
		m_pStatictext_levelvalue->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE  );
		// 경험치
		m_pStatictext_magic1->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE  );
		// 체력
		m_pStatictext_lifevalue->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE  );
		// 마법력 
		m_pStatictext_magic1value->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE  );
		// 근력
		m_pEditbox_strength->SetText( buffer );
		//  순발력
		m_pEditbox_dex->SetText( buffer );
		// 도력
		m_pEditbox_magic->SetText( buffer );
		// 생명력 
		m_pEditbox_life2->SetText( buffer );
		// 전투보너스  
		m_pEditbox_bonus->SetText( buffer );
		// 공격력  
		m_pEditbox_totaldamage->SetText( buffer );
		// 방어력  
		m_pEditbox_armourdefense->SetText( buffer );
		
		if(pclClient->IsCountrySwitch(Switch_SummonReform))
		{
			m_pEditbox_Wisdom->SetText(buffer);
			m_pEditbox_hand->SetText(buffer);
			m_pEditbox_physical_attack->SetText(buffer);			
			m_pEditbox_pysical_defense->SetText(buffer);			
			m_pEditbox_magic_attack->SetText(buffer);			
			m_pEditbox_magic_defense->SetText(buffer);			
			m_pEditbox_pysical_hit_rank->SetText(buffer);		
			m_pEditbox_pysical_dodge_rank->SetText(buffer);		
			m_pEditbox_magic_hit_rank->SetText(buffer);			
			m_pEditbox_magic_dodge_rank->SetText(buffer);		
			m_pEditbox_move_speed->SetText(buffer);	
			m_pStatictext_HP->SetText(buffer);	
			m_pStatictext_MP->SetText(buffer);	
			m_pStatictext_EXP->SetText(buffer);		
			m_pStatictext_FULL_EXP->SetText(buffer);	
		}

		if(pclClient->IsCountrySwitch(Switch_SummonTramsform))
		{
			m_pStatictext_TransformStamina->SetText(buffer);
		}

		//SI32 index = 0;
		if(BonusSummonButtonEnableSwitch == TRUE)
		{
			BonusSummonButtonEnableSwitch = FALSE;
			//index = 0;
			//while(ButtonDlg[index] >0)
			//{
			//	hwnd = GetDlgItem(hDlg, ButtonDlg[index]); 
			//	EnableWindow(hwnd, FALSE);
			//	index++;
			//}
			m_pButton_strength_plus->Enable( false );
			m_pButton_dex_plus->Enable( false );
			m_pButton_magic_plus->Enable( false );
			m_pButton_life2_plus->Enable( false );

			//[진성]  2008-3-24	
			if(pclClient->IsCountrySwitch(Switch_SummonReform))
			{
				m_pButton_wisdom_plus->Enable( false );
				m_pButton_hand_plus->Enable( false );
			}

		}
		TCHAR* pText = GetTxtFromMgr(631);
		StringCchCopy(buffer, 256, pText);
		m_pButton_takeinout->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

		//SendMessage(GetDlgItem( hDlg,  IDC_RADIO_SUMMON_ACTIVE ),      BM_SETCHECK, BST_UNCHECKED,	0);
		//SendMessage(GetDlgItem( hDlg,  IDC_RADIO_SUMMON_AVOID ),       BM_SETCHECK, BST_UNCHECKED,  0);
		m_pRadiobutton_attack->SetCheck( false, CONTROLLER_KIND_RADIOBUTTON );

		//EnableWindow(GetDlgItem( hDlg,  IDC_RADIO_SUMMON_ACTIVE ),      FALSE);
		//EnableWindow(GetDlgItem( hDlg,  IDC_RADIO_SUMMON_AVOID ),       FALSE);
		m_pRadiobutton_attack->Enable( false );
		m_pRadiobutton_wait->Enable( false );

		m_pButton_strength_plus->Enable(false );
		m_pButton_dex_plus->Enable( false );
		m_pButton_magic_plus->Enable( false );
		m_pButton_life2_plus->Enable( false );
		if(pclClient->IsCountrySwitch(Switch_SummonReform))
		{
			m_pButton_hand_plus->Enable( false );
			m_pButton_wisdom_plus->Enable( false );
		}
	}
}


void cltNSummonStatus::SendSummonAttackTypeMessageToServer( SI32 siAttackType )
{
	SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
	TCHAR* pTmpChar = NULL;

	if(id)
	{
		cltClient* pclclient = (cltClient*)pclClient;
		SI32 id = 1;
		if(pclCM->IsAlive(id) == false)	return;

		cltSummonInfo* pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;		
		if(pclSummonInfo == NULL) return;

		// 소환수가 없으면 실패. 
		if(pclSummonInfo->HaveSummon() == FALSE)
		{
			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			TCHAR* pText = GetTxtFromMgr(632);
			m_pEditbox_health_explain->SetText( pText );
			return;
		}
		//=======================================
		// 현재 ComboList의 소환수를 얻어온다.
		//=======================================
		TCHAR SummonName[256]=TEXT("");
		SI32 SummonIndex = 0;

		//SummonIndex = m_pCombobox_summons->GetSelectedIndex();
		//pTmpChar = m_pCombobox_summons->GetText(SummonIndex);
		//if(pTmpChar == NULL) return;
		//strcpy(	SummonName, pTmpChar );
		if(GetComboSummonNameIndex( SummonName, 256,  &SummonIndex) == FALSE) return;

		cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
		if(pclSummon==NULL) return;
		//=======================================

		if(pclSummon)
		{
			if(pclSummon->siStatus != SUMMON_STATUS_TAKEOUT)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
				TCHAR* pText = GetTxtFromMgr(633);
				m_pEditbox_health_explain->SetText( pText );
				return;
			}

			if(pclSummon->clIP.GetLife() <= 0)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
				TCHAR* pText = GetTxtFromMgr(634);
				m_pEditbox_health_explain->SetText( pText );
				return;
			}

			//-----------------------------------
			// 공격 속성변경을 요청한다.
			//-----------------------------------
			if( siAttackType == SUMMON_ATTACKTYPE_ACTIVE ||
				siAttackType == SUMMON_ATTACKTYPE_AVOID)
			{
				cltGameMsgRequest_SummonAttackChange clinfo(pclCM->CR[id]->GetCharUnique(), pclSummon->siSummonID, siAttackType);
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMONATTACKCHANGE, sizeof(clinfo), (BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

				m_pEditbox_health_explain->SetText( TEXT("") );
			}
		}
	}

}

void cltNSummonStatus::StatusChangeClick( SI16 _type )
{
	cltClient*   pclclient = (cltClient*)pclClient;

	SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
	if(id == 0)	
	{
		return; 
	}

	//==============================================
	// 소환수가 없으면 돌아간다.
	//==============================================
	// 소환수정보에만 의존한다.
	cltSummonInfo *pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;
	if(pclSummonInfo == NULL) 
	{
		return;
	}

	CControllerObj* pController = NULL;

	SI32 siValue = 0;

	switch(_type)
	{
	case Summon_Bonus_Use::TYPE_STR: 
		{
			pController = m_pEditbox_strength;
			siValue = m_clSummon_Use_Bonus.m_siPreStr + m_clSummon_Use_Bonus.m_siStr;
		}
		break;
	case Summon_Bonus_Use::TYPE_AGI: 
		{
			pController = m_pEditbox_dex;
			siValue = m_clSummon_Use_Bonus.m_siPreAgi + m_clSummon_Use_Bonus.m_siAgi;
		}
		break;
	case Summon_Bonus_Use::TYPE_WIS: 
		{
			pController = m_pEditbox_magic;
			siValue = m_clSummon_Use_Bonus.m_siPreWis + m_clSummon_Use_Bonus.m_siWis;
		}
		break;
	case Summon_Bonus_Use::TYPE_VIT: 
		{
			pController = m_pEditbox_life2;
			siValue = m_clSummon_Use_Bonus.m_siPreVit + m_clSummon_Use_Bonus.m_siVit;
		}	
		break;
	case Summon_Bonus_Use::TYPE_WISDOM: 
		{
			pController = m_pEditbox_Wisdom;
			siValue = m_clSummon_Use_Bonus.m_siPreWisdom + m_clSummon_Use_Bonus.m_siWisdom;
		}	
		break;
	case Summon_Bonus_Use::TYPE_HAND: 
		{
			pController = m_pEditbox_hand;
			siValue = m_clSummon_Use_Bonus.m_siPreHand + m_clSummon_Use_Bonus.m_siHand;
		}	
		break;

	}

	if(NULL == pController)
		return;

	TCHAR szBuffer[8] = "";
	itoa( siValue, szBuffer, 10 );
	pController->SetText( szBuffer );

	itoa( m_clSummon_Use_Bonus.m_siUseBunus, szBuffer, 10 );
	m_pEditbox_bonus->SetText(szBuffer);

}
void cltNSummonStatus::SendUseSummonBonus(void)
{
	SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
	TCHAR* pTmpChar = NULL;
	if(id)
	{
		cltClient* pclclient = (cltClient*)pclClient;
		SI32 id = 1;
		if(pclCM->IsAlive(id) == false)	
			return;

		cltSummonInfo* pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;		
		if( pclSummonInfo == NULL ) 
			return;

		// 소환수가 없으면 실패. 
		if( pclSummonInfo->HaveSummon() == FALSE )
		{
			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			TCHAR* pText = GetTxtFromMgr(632);
			m_pEditbox_health_explain->SetText( pText );

			if(pclClient->IsCountrySwitch(Switch_SummonReform))
			{
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(632));
			}

			return;
		}
		//=======================================
		// 현재 ComboList의 소환수를 얻어온다.
		//=======================================
		TCHAR SummonName[256]=TEXT("");
		SI32 SummonIndex = 0;
		if( GetComboSummonNameIndex( SummonName, 256,  &SummonIndex ) == FALSE )
			return;

		cltSummon *pclSummon = pclSummonInfo->GetSummon( SummonIndex );
		if( pclSummon == NULL ) 
		{			
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(633));
			return;
		}
		if(pclSummon)
		{
			if(pclSummon->siStatus != SUMMON_STATUS_TAKEOUT)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);

				TCHAR*	pTitle	= GetTxtFromMgr(422);
				TCHAR*	pText	= GetTxtFromMgr(633);

				if ( pclClient->IsCountrySwitch(Switch_SummonReform) )
				{
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				}
				else
				{
					m_pEditbox_health_explain->SetText( pText );
				}

				return;
			}

			if(pclSummon->clIP.GetLife() <= 0)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);

				TCHAR*	pTitle	= GetTxtFromMgr(422);
				TCHAR*	pText	= GetTxtFromMgr(634);

				if ( pclClient->IsCountrySwitch(Switch_SummonReform) )
				{
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				}
				else
				{
					m_pEditbox_health_explain->SetText( pText );
				}

				return;
			}

			cltGameMsgRequest_SummonUseBonus clinfo(pclCM->CR[id]->GetCharUnique(), pclSummon->siSummonID, 
				m_clSummon_Use_Bonus.m_siStr,
				m_clSummon_Use_Bonus.m_siAgi,
				m_clSummon_Use_Bonus.m_siWis,
				m_clSummon_Use_Bonus.m_siVit, 
				m_clSummon_Use_Bonus.m_siWisdom,
				m_clSummon_Use_Bonus.m_siHand,
				m_clSummon_Use_Bonus.GetDecreaseBonus() );

			cltMsg clMsg( GAMEMSG_REQUEST_USESUMMONBONUS, sizeof(clinfo), (BYTE*)&clinfo);
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

			m_clSummon_Use_Bonus.InitForAbilityPoint();

			m_pEditbox_health_explain->SetText( TEXT("") );
		}
	}
}

void cltNSummonStatus::SendUseSummonBonusMessageToServer( SI16 data,bool bInit )//소환수 스탯을 올린다.일단은 받을 인자를 늘려야 하겠지?
{
	SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
	TCHAR* pTmpChar = NULL;
	if(id)
	{
		cltClient* pclclient = (cltClient*)pclClient;
		SI32 id = 1;
		if(pclCM->IsAlive(id) == false)	return;

		cltSummonInfo* pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;		
		if(pclSummonInfo == NULL) return;

		// 소환수가 없으면 실패. 
		if(pclSummonInfo->HaveSummon() == FALSE)
		{
			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			TCHAR* pText = GetTxtFromMgr(632);
			m_pEditbox_health_explain->SetText( pText );
			return;
		}
		//=======================================
		// 현재 ComboList의 소환수를 얻어온다.
		//=======================================
		TCHAR SummonName[256]=TEXT("");
		SI32 SummonIndex = 0;
		//SummonIndex = m_pCombobox_summons->GetSelectedIndex();
		//pTmpChar = m_pCombobox_summons->GetText(SummonIndex);
		//if(pTmpChar == NULL) return;
		//strcpy(	SummonName, pTmpChar );
		if(GetComboSummonNameIndex( SummonName, 256,  &SummonIndex ) == FALSE) return;

		cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
		if(pclSummon==NULL) return;
		//=======================================
		if(pclSummon)
		{
			if(pclSummon->siStatus != SUMMON_STATUS_TAKEOUT)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
				TCHAR* pText = GetTxtFromMgr(633);
				m_pEditbox_health_explain->SetText( pText );
				return;
			}

			if(pclSummon->clIP.GetLife() <= 0)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
				TCHAR* pText = GetTxtFromMgr(634);
				m_pEditbox_health_explain->SetText( pText );
				return;
			}

			if ( bInit == false && pclSummon->clIP.GetBonus() <= 0 ) return ;

			cltGameMsgRequest_SummonUseBonusPoint clinfo(pclCM->CR[id]->GetCharUnique(), pclSummon->siSummonID, data,bInit);
			cltMsg clMsg( GAMEMSG_REQUEST_USESUMMONBONUS, sizeof(clinfo), (BYTE*)&clinfo);
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

			m_pEditbox_health_explain->SetText( TEXT("") );
		}
	}
}


void cltNSummonStatus::SendSummonTakeInOutMessageToServer( SI32 siSummonUnique )
{
	TCHAR* pTmpChar = NULL;
	//=====================================================
	// 300 ms 이하에서는 계속 검색을 하지 못하게 한다.
	//=====================================================
	if(TABS(pclClient->CurrentClock - m_LastTakeInOutTime) < 300) return;
	m_LastTakeInOutTime = pclClient->CurrentClock ;
	//=====================================================

	cltClient* pclclient = (cltClient*)pclClient;
	SI32 id = 1;
	if(pclclient->pclCM->IsAlive(id) == false)	return;

	cltSummonInfo* pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;		
	if(pclSummonInfo == NULL) return;
	// 소환수가 없으면 실패. 
	if(pclSummonInfo->HaveSummon() == FALSE)
	{
		pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
		TCHAR* pText = GetTxtFromMgr(632);
		if(m_pEditbox_health_explain) m_pEditbox_health_explain->SetText( pText );
		return;
	}

	cltSummon *pclSummon = NULL ;
	cltSummon *pclSummonOut = NULL ; // 밖으로 나올 놈의 정보
	//=======================================	
	// 단축기 VK_F6,VK_F7 추가로 VK_F8
	//=======================================	
	// leon todo - hDlg == NULL => IsShow() 로 변환 가능 여부 확인 요망!!
	//if(!IsShow())
	if( pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ] == NULL ) return;
	if( ((cltNCharStatus*)pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ])->IsShow() == false )
	{
		// 누른 키에 대응하는 소환수가 밖에 있으면 안으로 집어 넣는다.
		SI32 siSummonID = pclCM->CR[id]->GetSummonID();
		if(pclCM->IsValidID(siSummonID) == TRUE && pclCM->CR[id]->siSummonIndex == siSummonUnique )
		{
			pclSummon = pclSummonInfo->GetSummon(pclCM->CR[id]->siSummonIndex);
		}

		if(pclSummon == NULL)
		{
			// 누른 키에 대응하는 소환수가 안에 있으면 밖으로 뺀다.
			pclSummon = pclSummonInfo->GetSummon(siSummonUnique);
			if(pclSummon==NULL) return;

			// 없으면 취소한다.
			// TAKE IN OUT 시킬 소환수의 정보가 없다면 전체를 찾는다.
			/*if(pclSummon->siKind == 0)
			{
				for(SI32 i = 0 ; i < MAX_SUMMON_OWN; ++i)
				{
					pclSummon = pclSummonInfo->GetSummon(i);
					if(pclSummon && pclSummon->siKind)	break;
				}
			}*/
		}
		if(pclSummon==NULL) return;
	}
	else
	{
		SI32 siSummonID = pclCM->CR[id]->GetSummonID();
		if(pclCM->IsValidID(siSummonID) == TRUE && pclCM->CR[id]->siSummonIndex == siSummonUnique )
		{
			// 현재 소환수가 나와 있으면 나와있는 놈을 일단 집어 넣는다.
			pclSummon = pclSummonInfo->GetSummon(pclCM->CR[id]->siSummonIndex);
		}
		if(pclSummon == NULL)
		{
			//=======================================
			// 현재 ComboList의 소환수를 얻어온다.
			//=======================================
			TCHAR SummonName[256]=TEXT("");
			SI32 SummonIndex = 0;
			//SummonIndex = m_pCombobox_summons->GetSelectedIndex();
			//pTmpChar = m_pCombobox_summons->GetText(SummonIndex);
			//if(pTmpChar == NULL) return;
			//strcpy(	SummonName, pTmpChar );

			if( siSummonUnique == -1 )
			{
				if(GetComboSummonNameIndex( SummonName, 256,  &SummonIndex ) == FALSE) return;
				pclSummon = pclSummonInfo->GetSummon(SummonIndex);
			}
			else
				pclSummon = pclSummonInfo->GetSummon(siSummonUnique);

			//=======================================
		}
		if(pclSummon==NULL) return;
	}

	if(pclSummon)
	{
		switch(pclSummon->siStatus)
		{
		case SUMMON_STATUS_TAKEIN:	//strcpy(buffer, TEXT("소환"));		break;
			{
				if( pclSummon->siKind &&
					pclSummon->clIP.GetLife() > 0 &&
					pclSummon->siStatus == SUMMON_STATUS_TAKEIN )//&&
					//pclCM->CR[id]->siChildCharUnique == 0)
				{
					cltGameMsgRequest_SummonTakeInOut  clinfo( pclCM->CR[id]->GetCharUnique(), pclSummon->siSummonID,  SUMMON_STATUS_TAKEOUT);
					cltMsg clMsg( GAMEMSG_REQUEST_TAKEINOUT, sizeof(clinfo), (BYTE*)&clinfo);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				else
				{
					/*if(pclCM->CR[id]->siChildCharUnique != 0)
					{
						pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
						switch(pclSummon->siUnique)
						{
						case 0:	if(hDlg) SetWindowText( GetDlgItem( hDlg, IDC_EDIT_STATUS_SUMMON_EXPLAIN ), 
									"소환된 소환수를 먼저 회수하여야 합니다. \r\n"
									"단축키(F6)을 사용하시면 편리합니다." );	break;
						case 1: if(hDlg) SetWindowText( GetDlgItem( hDlg, IDC_EDIT_STATUS_SUMMON_EXPLAIN ), 
									"소환된 소환수를 먼저 회수하여야 합니다. \r\n"
									"단축키(F7)을 사용하시면 편리합니다." );	break;
						}
					}
					else*/ if(pclSummon->clIP.GetLife() <= 0)
					{
						pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
						TCHAR* pText = GetTxtFromMgr(634);
						if(m_pEditbox_health_explain) m_pEditbox_health_explain->SetText( pText );
					}
					else
					{
						pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
						TCHAR* pText = GetTxtFromMgr(637);
						if(m_pEditbox_health_explain) m_pEditbox_health_explain->SetText( pText );
					}
				}
			}
			break;
		case SUMMON_STATUS_TAKEOUT:	//strcpy(buffer, TEXT("회수"));		break;
			{
				if( pclSummon->siKind &&
					pclSummon->clIP.GetLife() > 0 &&
					pclSummon->siStatus == SUMMON_STATUS_TAKEOUT &&
					pclCM->CR[id]->siChildCharUnique != 0)
				{
					cltGameMsgRequest_SummonTakeInOut  clinfo( pclCM->CR[id]->GetCharUnique(), pclSummon->siSummonID,  SUMMON_STATUS_TAKEIN);
					cltMsg clMsg( GAMEMSG_REQUEST_TAKEINOUT, sizeof(clinfo), (BYTE*)&clinfo);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				else
				{
					if(pclSummon->clIP.GetLife() <= 0)
					{
						pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
						TCHAR* pText = GetTxtFromMgr(638);
						if(m_pEditbox_health_explain) m_pEditbox_health_explain->SetText( pText );
					}
					else
					{
						pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
						TCHAR* pText = GetTxtFromMgr(637);
						if(m_pEditbox_health_explain) m_pEditbox_health_explain->SetText( pText );
					}
				}
			}
			break;
		case SUMMON_STATUS_DEAD:    
			{
				if( pclSummon->siKind )
				{
					/*					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
					if(hDlg) SetWindowText( GetDlgItem( hDlg, IDC_EDIT_STATUS_SUMMON_EXPLAIN ), 
						"소환수가 사망하였습니다.\r\n"
						"[선무당]을 찾아가 부활시켜야 합니다.\r\n"
						"바로 부활 시키려면 [소환수부활이용권]이 필요합니다."
						);

					return;*/
					SI32 itemunique = ITEMUNIQUE(24020);  // 소환수부활이용권
					SI32 siGoonzuEvent_ItemUnique = ITEMUNIQUE(24130); // Goonzu Event Item
					if (	pclCM->CR[id]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 &&
							pclCM->CR[id]->pclCI->clCharItem.GetItemNum(siGoonzuEvent_ItemUnique) <= 0)
					{
						TCHAR buffer[256] = TEXT("") ;
						pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
						TCHAR* pText = GetTxtFromMgr(2837);
						StringCchPrintf(buffer, 256, pText);

						pText = GetTxtFromMgr(619);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
						return ;
					}

					TCHAR* pTitle = GetTxtFromMgr(578);
					TCHAR* pText = GetTxtFromMgr(2838);

					stMsgBox MsgBox;
					// 현재 dialog가 탭 창 같은 Child Dialog일때
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 24 );
					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof( id ) + sizeof( SI32 );
					memcpy( TempBuffer, &id, sizeof( id ) );
					memcpy( &TempBuffer[ sizeof( id ) ], &(pclSummon->siSummonID), sizeof( SI32 ) );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
				}
				else
				{
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
					TCHAR* pText = GetTxtFromMgr(632);
					if(m_pEditbox_health_explain) m_pEditbox_health_explain->SetText( pText );
					return;
				}
			}
			break;
		}
	}
}

void cltNSummonStatus::SendSummonChangeNameMessageToServer()
{
	cltClient* pclclient = (cltClient*)pclClient;
	TCHAR ChangeSummonName[ MAX_SUMMON_NAME ] = TEXT("");
	TCHAR* pTmpChar = NULL;
	SI32 id = 1;
	if(pclCM->IsAlive(id) == false)	return ;

	cltSummonInfo* pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;		
	if(pclSummonInfo == NULL) return;

	if(pclSummonInfo->HaveSummon() == FALSE)
	{
		pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
		TCHAR* pText = GetTxtFromMgr(632);
		m_pEditbox_health_explain->SetText( pText );
		m_pEditbox_name->SetText( TEXT("") );
		StringCchCopy(strSummonName, MAX_SUMMON_NAME, TEXT("") );

		return ;
	}

	//-----------------------------------
	// 소환수 이름을 얻어온다.
	//-----------------------------------
	pTmpChar = m_pEditbox_name->GetText();
	if(pTmpChar ==  NULL) return;

	if ( !pTmpChar || _tcslen(pTmpChar) > MAX_SUMMON_NAME - 1 )
	{
		TCHAR buffer[256] = TEXT("") ;
		TCHAR* pText = GetTxtFromMgr(566);
		StringCchPrintf(buffer, 256, pText,(MAX_SUMMON_NAME-1)/2);

		pText = GetTxtFromMgr(567);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
		return ;
	}

	StringCchCopy( ChangeSummonName, MAX_SUMMON_NAME, pTmpChar );

	//-----------------------------------
	// 소환수의 이름을 검사한다.
	//-----------------------------------
	//=======================================
	// 현재 ComboList의 소환수를 얻어온다.
	//=======================================
	TCHAR SummonName[256]=TEXT("");
	SI32 SummonIndex = 0;
	//pTmpChar = NULL;
	//SummonIndex = m_pCombobox_summons->GetSelectedIndex();
	//pTmpChar = m_pCombobox_summons->GetText(SummonIndex);
	//if(pTmpChar == NULL) return;
	//strcpy( SummonName, pTmpChar );
	if(GetComboSummonNameIndex( SummonName, 256,  &SummonIndex ) == FALSE) return;

	cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
	if(pclSummon==NULL) return;

	TCHAR *pName = pclSummon->szName;
	if ( pName != NULL && _tcslen( pName ) > 0 && _tcscmp( pName, ChangeSummonName ) == 0 )
	{
		pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
		TCHAR* pText = GetTxtFromMgr(639);
		m_pEditbox_health_explain->SetText( pText );
		return ;
	}

	//cyj 캐릭이름에도 욕설필터 적용
	TCHAR buf[ MAX_SUMMON_NAME ];
	StringCchPrintf(buf, MAX_SUMMON_NAME, ChangeSummonName);
#ifdef _IAF_EXPORT
	if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
	{
		TCHAR ResultBuf[1024] = "";
		IAF::Filtering( buf, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
		memcpy( buf, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
	}
#else
	pclclient->m_pCAbuseFilter->CheckAbuse(buf);
#endif

	if ( IsCorrectID( buf ) == FALSE )
	{
		pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
		
		//m_pEditbox_health_explain->SetText( pText );
		TCHAR* pTitle= GetTxtFromMgr(839);
		TCHAR* pText = GetTxtFromMgr(640);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

		return ;
	}

	//-----------------------------------
	// 이름 변경 요청을 한다.
	//-----------------------------------
	cltGameMsgRequest_SummonNameChange  clinfo( pclCM->CR[id]->GetCharUnique(), pclSummon->siSummonID, buf );
	cltMsg clMsg( GAMEMSG_REQUEST_SUMMONNAMECHANGE, sizeof(clinfo), (BYTE*)&clinfo);
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

}


void cltNSummonStatus::SendSummonFreeMessageToServer()
{
	stComboBoxItemData tmpComboBoxItemData;

	cltClient* pclclient = (cltClient*)pclClient;
	SI32 id = 1;
	if(pclclient->pclCM->IsAlive(id) == false)return ;

	cltSummonInfo* pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;		
	if(pclSummonInfo == NULL) return;

	// 소환수가 없으면 실패. 
	if(pclSummonInfo->HaveSummon() == FALSE)
	{
		pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
		TCHAR* pText = GetTxtFromMgr(632);
		m_pEditbox_health_explain->SetText( pText );
		return ;
	}
	else
	{
		//=======================================
		// 현재 ComboList의 소환수를 얻어온다.
		//=======================================
		TCHAR SummonName[256]=TEXT("");
		SI32 SummonIndex = 0;
		if(GetComboSummonNameIndex( SummonName, 256,  &SummonIndex ) == FALSE) return;
		cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
		if(pclSummon==NULL) return;

		if(pclSummon)
		{
			if(pclSummon->siKind == 0)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
				TCHAR* pText = GetTxtFromMgr(641);
				m_pEditbox_health_explain->SetText( pText );
				return ;
			}
			if(pclSummon->siStatus == SUMMON_STATUS_DEAD)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
				TCHAR* pText = GetTxtFromMgr(642);
				m_pEditbox_health_explain->SetText( pText );
				return ;
			}

			TCHAR* pTitle = GetTxtFromMgr(578);
			TCHAR* pText = GetTxtFromMgr(643);

			stMsgBox MsgBox;
			// 현재 dialog가 탭 창 같은 Child Dialog일때
			CControllerMgr *pParent = NULL;
			pParent = this->GetParentDialog();
			if(pParent == NULL ) return;
			MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 25 );
			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( id ) + sizeof( SI32 );
			memcpy( TempBuffer, &id, sizeof( id ) );
			memcpy( &TempBuffer[ sizeof( id ) ], &(pclSummon->siSummonID), sizeof( SI32 ) );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
	}
}


void cltNSummonStatus::SendSummonReviveMessageToServer()
{

	cltClient* pclclient = (cltClient*)pclClient;
	SI32 id = 1;
	if(pclclient->pclCM->IsAlive(id) == false)return ;

	cltSummonInfo* pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;		
	if(pclSummonInfo == NULL) return;

	// 소환수가 없으면 실패. 
	if(pclSummonInfo->HaveSummon() == FALSE)
	{
		pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
		TCHAR* pText = GetTxtFromMgr(632);
		m_pEditbox_health_explain->SetText( pText );
		return ;
	}
	else
	{
		//=======================================
		// 현재 ComboList의 소환수를 얻어온다.
		//=======================================
		TCHAR SummonName[256]=TEXT("");
		SI32 SummonIndex = 0;
		if(GetComboSummonNameIndex( SummonName, 256,  &SummonIndex) == FALSE) return;
		cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
		if(pclSummon==NULL) return;

		if(pclSummon)
		{
			if(pclSummon->siKind == 0)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
				TCHAR* pText = GetTxtFromMgr(633);
				m_pEditbox_health_explain->SetText( pText );
				return ;
			}

			if(pclSummon->siStatus != SUMMON_STATUS_DEAD)
			{
				if(pclSummon->clIP.GetLife()<=0)
				{
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
					TCHAR* pText = GetTxtFromMgr(644);
					m_pEditbox_health_explain->SetText( pText );
					return ;
				}
				else
				{
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
					TCHAR* pText = GetTxtFromMgr(645);
					m_pEditbox_health_explain->SetText( pText );
					return ;
				}
			}

			if(pclSummon->siStatus == SUMMON_STATUS_DEAD )//&& pclSummon->clIP.GetLife()<=0)
			{

				SI32 itemunique = ITEMUNIQUE(24020);  // 소환수부활이용권
				SI32 siGoonzuEvent_ItemUnique = ITEMUNIQUE(24130); // Goonzu Event Item
				if (	pclCM->CR[id]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 &&
					pclCM->CR[id]->pclCI->clCharItem.GetItemNum(siGoonzuEvent_ItemUnique) <= 0)
				{
					TCHAR* pText = GetTxtFromMgr(646);
					TCHAR* pTitle = GetTxtFromMgr(647);

					stMsgBox MsgBox;
					// 현재 dialog가 탭 창 같은 Child Dialog일때
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_OKCANCEL, 26 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

					// leon todo - hDlg => NULL 로 변환 확인 요망!!
					//MessageBox(NULL, pTitle, pText,
					//	MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
					return ;
				}

				TCHAR* pTitle = GetTxtFromMgr(578);
				TCHAR* pText = GetTxtFromMgr(648);

				stMsgBox MsgBox;
				// 현재 dialog가 탭 창 같은 Child Dialog일때
				CControllerMgr *pParent = NULL;
				pParent = this->GetParentDialog();
				if(pParent == NULL ) return;
				MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 27 );
				BYTE TempBuffer[ 128 ];
				SI16 Size = sizeof( id ) + sizeof( SI32 );
				memcpy( TempBuffer, &id, sizeof( id ) );
				memcpy( &TempBuffer[ sizeof( id ) ], &(pclSummon->siSummonID), sizeof( SI32 ) );
				pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
			}
		}
	}
}

//===================================================================
//
//===================================================================
//BOOL cltNSummonStatus::GetComboSummonNameIndex( TCHAR *CurrSummonName, SI32 *siSummonIndex )
//{
//	SI32 siIndex = -1;
//	// NULL 이 들어오면 VK_F8 의한 요청 
//
//	// IDC_EDIT_STATUS_SUMMONNAME
//	TCHAR TempSummonName[60] = TEXT("");
//	TCHAR TempSummonIndex[60] = TEXT("");
//	TCHAR *pTmpChar = NULL;
//
//	//SendDlgItemMessage(hDlg, IDC_COMBO_SUMMON_LIST, CB_GETLBTEXT, m_siLastSummonComboList, (LPARAM)TempSummonName);
//	siIndex=m_pCombobox_summons->GetSelectedIndex();
//	pTmpChar = m_pCombobox_summons->GetText(siIndex);
//
//	if(pTmpChar == NULL ) return FALSE;
//
//	strcpy( TempSummonName, pTmpChar );
//
//	
//	if(_tcslen(TempSummonName) >4)
//	{
//		strncpy(TempSummonIndex, TempSummonName, 1);
//		//SI32 siIndex = _tstoi(TempSummonIndex);
//		siIndex--;
//		if(siIndex<0 || siIndex>= MAX_SUMMON_OWN) return FALSE;
//
//		strcpy(CurrSummonName,  pclCM->CR[1]->pclCI->clSummonInfo.clSummon[siIndex].szName);
//
//		*siSummonIndex = siIndex;
//		return TRUE;
//	}
//	return FALSE;
//}

void cltNSummonStatus::InitSummonStatusInfo()
{
	stComboBoxItemData tmpComboBoxItemData;

	if(m_bChangedInitSummonStatus)
	{
		//==============================================
		// COMBO LIST에 소환수 이름을 설정한다.
		//==============================================
		//SI32 siIndex = SendDlgItemMessage(hDlg, IDC_COMBO_SUMMON_LIST, CB_GETCURSEL, 0, 0);	
		SI32 siIndex = m_pCombobox_summons->GetSelectedIndex();
		//SendDlgItemMessage(hDlg, IDC_COMBO_SUMMON_LIST, CB_RESETCONTENT, 0, 0);				
		m_pCombobox_summons->Clear();

		SI32 siSummonCount = 0;
		TCHAR SummonName[256]=TEXT("");
		for(SI32 i=0; i < MAX_SUMMON_OWN; ++i)
		{
			cltSummon *pclSummon = pclCM->CR[1]->pclCI->clSummonInfo.GetSummon(i);
			if(pclSummon && pclSummon->siKind )
			{				
				//cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
				cltKindInfo* pclki = NULL;
				if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
				{
					pclki = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
				}
				else
				{
					pclki = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
				}
				if(pclki)
				{
					StringCchPrintf(SummonName, 256, TEXT("%d: %s"), i+1, pclki->GetName());
					tmpComboBoxItemData.Init();
					tmpComboBoxItemData.Set( SummonName );
					m_pCombobox_summons->AddItem( &tmpComboBoxItemData );
					//ComboAddString(hDlg, IDC_COMBO_SUMMON_LIST, (LPARAM)SummonName);
					siSummonCount++;
				}
			}
		}
		if(siIndex == CB_ERR)			siIndex = 0;
		if(siIndex >= siSummonCount)	siIndex = 0;
		//SendDlgItemMessage(hDlg, IDC_COMBO_SUMMON_LIST, CB_SETCURSEL, siIndex, 0);	
		m_pCombobox_summons->SetCursel( siIndex );
		//m_siLastSummonComboList = siIndex;


		// Combo List에서 선택된 소환수의 이름을 설정한다.
		SI32 SummonIndex = 0;
		if(GetComboSummonNameIndex( SummonName, 256,  &SummonIndex))
		{
			cltSummon *pclSummon = pclCM->CR[1]->pclCI->clSummonInfo.GetSummon(SummonIndex);
			if(pclSummon)
			{
				m_pEditbox_health_explain->SetText( pclSummon->szName );
			}
		}

		UpdateSummonStatusInfo();
	}
	//==============================================
	m_bChangedInitSummonStatus = FALSE;
}

void cltNSummonStatus::UpdateSummonPortrait()
{
	//==========================================
	// 초상화 
	//==========================================
	cltClient* pclclient = (cltClient*)pclClient;
	SI32 siSummonKind = 0;
	//HBITMAP Portrait = m_hSummonPortraitBitmap;
	if(pclCM->CR[1]->pclCI->clSummonInfo.HaveSummon())
	{
		TCHAR SummonName[256]=TEXT("");
		TCHAR SummonKindName[256]=TEXT("");
		SI32 SummonIndex = 0;
		if(GetComboSummonNameIndex( SummonName, 256,  &SummonIndex ) )
		{
			siSummonKind = pclCM->CR[1]->pclCI->clSummonInfo.clSummon[SummonIndex].siKind;
			SI32 siSummonLevel	=	pclCM->CR[1]->pclCI->clSummonInfo.clSummonBase[SummonIndex].siLevel;	// 소환수 레벨

			if( pclclient->m_pDialog[ NCHARSUMMONRESET_DLG ] != NULL && 
				pclclient->m_pDialog[ NCHARSUMMONRESET_DLG ]->IsShow() ==  true)
			{
				NTCHARString1024 kBuffer(TEXT(""));

				// 소환수 무기 장착석
				if(pclClient->IsCountrySwitch(Switch_Summon_EquipSton))
				{
					SI16 siAttackStonNumNow = pclCM->CR[1]->pclCI->clSummonInfo.clSummon[SummonIndex].siAttackWearStoneNum;
					SI16 siDeffenceStonNumNow = pclCM->CR[1]->pclCI->clSummonInfo.clSummon[SummonIndex].siDefenseWearStoneNum;

					NTCHARString1024	kText1(GetTxtFromMgr(8643));
					kText1.Replace(TEXT("#Weaponstone#"),SI16ToString(siAttackStonNumNow) );	// 무기석
					kText1.Replace(TEXT("#ArmorStone#"),SI16ToString(siDeffenceStonNumNow) );	// 방어구석
					StringCchCat(kBuffer, 1024, kText1);
				}

				// PCK : 소환수 채팅커맨드를 보여준다.
				for(SI32 i = 0;i < MAX_SUMMON_CHATCOMMANDNUMBER;i++)
				{
					if( pclClient->pclKindInfoSet->pclSummonChatCommand[i] == NULL )	continue;
					
					if(siSummonKind == pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siCharKind)
					{
						TCHAR* pChatAction = GetTxtFromMgr(pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siChatTextLine);

						if(siSummonLevel >= pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siLevel)
						{
							NTCHARString1024	kText2( GetTxtFromMgr(9747) );
							kText2.Replace(TEXT("#level#"),SI16ToString(pclClient->pclKindInfoSet->pclSummonChatCommand[i]->siLevel) );
							kText2.Replace(TEXT("#command#"),pChatAction );
							StringCchCat(kBuffer, 1024, kText2);
						}
					}
				}

				((CNCharSummonReset*)pclclient->m_pDialog[ NCHARSUMMONRESET_DLG ])->ViewStoneResetDlg( kBuffer );
			}

			if((m_pImagestatic_portrait == NULL) || (siSummonKind && m_siPortraitSummonKind != siSummonKind ))			
			{
				//TCHAR strFileName[ MAX_PATH ] = TEXT("");
				//sprintf( strFileName, TEXT("Interface/Portrait/%s.bmp"),	pclClient->pclKindInfoSet->pclKI[ siSummonKind ]->szCode );
				//Portrait = (HBITMAP)LoadImage( pclClient->GetInstance(), strFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

				TCHAR* tmp_pChar = NULL;

				m_siPortraitSummonKind = siSummonKind;

				tmp_pChar = (TCHAR*)pclClient->pclKindInfoSet->pclKI[ m_siPortraitSummonKind ]->GetName();

				if(tmp_pChar == NULL )
					return ;

				StringCchCopy( SummonKindName, 256, tmp_pChar );

				SI16 tmpSelectedIndex = pclClient->pclKindInfoSet->FindKindFromName( SummonKindName );

				TCHAR szPortrait[256];
				SI32 siPortraitFont;
				pclClient->pclKindInfoSet->GetNPCImageFileAndFont(tmpSelectedIndex, szPortrait, sizeof(szPortrait), &siPortraitFont);
				m_pImagestatic_portrait->SetFileName(  szPortrait );
				m_pImagestatic_portrait->SetFontIndex( siPortraitFont );

			}
		}
	}
	else
	{
		m_pImagestatic_portrait->SetFileName(TEXT(""));
		// 소환수가 없으므로 데이터 초기화
		m_siPortraitSummonKind = 0;
	}

}

void cltNSummonStatus::SendSummonStatusInitMessageToServer()
{
	cltClient* pclclient = (cltClient*)pclClient;
	SI32 id = 1;
	if(pclclient->pclCM->IsAlive(id) == false)return ;

	cltSummonInfo* pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;		
	if(pclSummonInfo == NULL) return;

	// 소환수가 없으면 실패. 
	if(pclSummonInfo->HaveSummon() == FALSE)
	{
		pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
		TCHAR* pText = GetTxtFromMgr(632);
		m_pEditbox_health_explain->SetText( pText );
		return ;
	}
	else
	{
		//=======================================
		// 현재 ComboList의 소환수를 얻어온다.
		//=======================================
		TCHAR SummonName[256]=TEXT("");
		SI32 SummonIndex = 0;
		if(GetComboSummonNameIndex( SummonName, 256,  &SummonIndex) == FALSE) return;
		cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
		if(pclSummon==NULL) return;

		if(pclSummon)
		{
			if(pclSummon->siKind == 0)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
				TCHAR* pText = GetTxtFromMgr(641);
				m_pEditbox_health_explain->SetText( pText );
				return ;
			}

			if(pclSummon->siStatus == SUMMON_STATUS_DEAD || pclSummon->clIP.GetLife()<=0)
			{
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
				TCHAR* pText = GetTxtFromMgr(2839);
				m_pEditbox_health_explain->SetText( pText );
				return ;
			}

			TCHAR buffer[256] = TEXT("") ;

			//[진성] 캐릭터 상태 초기화 일정기간동안 마음것 사용. 
			if(pclClient->pclEventTimeManager->InEventTime("char_ability_init", &pclclient->sTime) )
			{
				BYTE TempBuffer[ 128 ];
				SI16 Size = sizeof( id ) + sizeof( SI32 );
				memcpy( TempBuffer, &id, sizeof( id ) );
				memcpy( &TempBuffer[ sizeof( id ) ], &(pclSummon->siSummonID), sizeof( SI32 ) );

				SI32 id = *((SI32*)TempBuffer);
				SI32 siSummonID = *((SI32*)&TempBuffer[ 4 ] );

				m_pEditbox_health_explain->SetText( TEXT("") );

				cltGameMsgRequest_Summon_StatusInit clinfo( pclCM->CR[id]->GetCharUnique(), siSummonID);
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMON_STATUSINIT, sizeof(clinfo), (BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				return;
			}

			//cyj 상태초기화이용권 유니크변경 14090->24030
			//SI32 itemunique = 14090 ;  // 상태초기화이용권
			SI32 itemunique = ITEMUNIQUE(24030) ;  // 상태초기화이용권
			SI32 siEventItemUnique = ITEMUNIQUE(24125) ;  // [종호] 상태초기화이용권(이벤트)

			// 상태초기화 아이템이 하나도 없는경우
			if (	pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 &&
					pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(siEventItemUnique) <= 0)
			{
				TCHAR* pText = GetTxtFromMgr(608);
				StringCchPrintf(buffer, 256, pText);

				TCHAR* pTitle = GetTxtFromMgr(605);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
				return ;
			}

			TCHAR* pText = GetTxtFromMgr(2840);
			TCHAR* pTitle = GetTxtFromMgr(605);
			StringCchCopy(buffer, 256, pText);

			stMsgBox MsgBox;
			// 현재 dialog가 탭 창 같은 Child Dialog일때
			CControllerMgr *pParent = NULL;
			pParent = this->GetParentDialog();
			if(pParent == NULL ) return;
			MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 28 );
			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( id ) + sizeof( SI32 );
			memcpy( TempBuffer, &id, sizeof( id ) );
			memcpy( &TempBuffer[ sizeof( id ) ], &(pclSummon->siSummonID), sizeof( SI32 ) );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
	}
}

void cltNSummonStatus::SendSummonAutoHealMessageToServer(SI16 data,bool autoheal)
{
	cltClient* pclclient = (cltClient*)pclClient;
	SI32 id = 1;
	if(pclclient->pclCM->IsAlive(id) == false)return ;

	cltSummonInfo* pclSummonInfo = &pclCM->CR[id]->pclCI->clSummonInfo;		
	if(pclSummonInfo == NULL) return;

	cltGameMsgRequest_Summon_SetAutoHeal clinfo( pclCM->CR[id]->GetCharUnique(), data, autoheal);
	cltMsg clMsg( GAMEMSG_REQUEST_SUMMON_SETAUTOHEAL, sizeof(clinfo), (BYTE*)&clinfo);
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
}

// 이동속도 상세 정보를 설정한다. 
void cltNSummonStatus::ShowDetailScrollInfo(IN cltSummonScroll *pScroll , OUT TCHAR* pszScrollInfo, SI32 siTextSize)
{
	if(pszScrollInfo == NULL)	return;
	if(siTextSize <= 0 || siTextSize > 512)	return;

	TCHAR szScrollExplain[512];
	if(pScroll)
	{
		StringCchCopy(szScrollExplain, 512, TEXT(""));
		for(SI32 i = 0 ; i<MAX_SCROLL_SUMMON_PER_KIND; ++i )
		{
			if(pScroll->siUnique[i])
			{
				TCHAR buffer[256] = TEXT("");
				StringCchPrintf(buffer, 256, TEXT("%s:%0.1f%%\r\n"), 
					pclClient->pclItemManager->GetName(pScroll->siUnique[i]),
					(float)(pScroll->siPercent[i] / 10.0));

				StringCchCat(szScrollExplain, 512, buffer);
			}
		}

		StringCchCopy(pszScrollInfo, siTextSize, szScrollExplain);
	}
}

BOOL cltNSummonStatus::GetComboSummonNameIndex( TCHAR *CurrSummonName, SI16 txtSize, SI32 *siSummonIndex )
{
	// IDC_EDIT_STATUS_SUMMONNAME
	TCHAR TempSummonName[60] = TEXT("");
	TCHAR TempSummonIndex[60] = TEXT("");
	TCHAR *pTmpChar = NULL;

	if( m_siLastSummonComboList < 0 ) return FALSE;
	pTmpChar = m_pCombobox_summons->GetText( m_siLastSummonComboList );
	if(pTmpChar == NULL ) return FALSE;

	StringCchCopy( TempSummonName, 60, pTmpChar );

	if(_tcslen(TempSummonName) > 4)
	{
		_tcsncpy(TempSummonIndex, TempSummonName, 1);
		SI32 siIndex = _tstoi(TempSummonIndex);
		siIndex--;
		if(siIndex < 0 || siIndex >= MAX_SUMMON_OWN) return FALSE;

		StringCchCopy(CurrSummonName,  txtSize, pclCM->CR[1]->pclCI->clSummonInfo.clSummon[siIndex].szName);

		*siSummonIndex = siIndex;
		return TRUE;
	}
	return FALSE;
}

// 공격력 상세 정보를 설정한다. 
void cltNSummonStatus::ShowDetailDamageInfo( cltParametaBox* pclPB, cltItem* pclitemlist, SI32 activesweapon, cltSummon* pclSummon )
{
	TCHAR buffer[256];
	stComboBoxItemData tmpComboBoxItemData;

	// 모든 항목을 삭제한다. 
	m_pCombobox_apowername->Clear();

	// 레벨에 의한 공격력. 
	TCHAR* pText = GetTxtFromMgr(649);
	StringCchPrintf(buffer, 256, pText, pclPB->siLevelDamage);
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );

	if(pclitemlist && pclClient->pclItemManager->GetWeaponMinDamage(pclitemlist, activesweapon, pclSummon) )
	{
		pText = GetTxtFromMgr(650);
		if ( pclPB->siAttackType == ATTACKTYPE_STAFFWIND )
			StringCchPrintf(buffer, 256, pText, pclPB->TotalEquipMinDamage*2, pclPB->TotalEquipMaxDamage*2);
		else
			StringCchPrintf(buffer, 256, pText, pclPB->TotalEquipMinDamage, pclPB->TotalEquipMaxDamage);
	}
	else
	{
		pText = GetTxtFromMgr(651);
		StringCchPrintf(buffer, 256, pText, pclPB->TotalEquipMinDamage, pclPB->TotalEquipMaxDamage);
	}

	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );

	// 진화 
	if(pclPB->siEvolveDamage)
	{
		pText = GetTxtFromMgr(652);
		StringCchPrintf(buffer, 256, pText, pclPB->siEvolveDamage );
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( buffer );
		m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );
	}


	if(pclPB->siAttackType == ATTACKTYPE_STAFF1 || pclPB->siAttackType == ATTACKTYPE_STAFF2 || pclPB->siAttackType == ATTACKTYPE_NEW_STAFF2)
	{
		// 도력에 의한 공격력. 
		pText = GetTxtFromMgr(653);
		StringCchPrintf(buffer, 256, pText, pclPB->GetAddMagicDamage());
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( buffer );
		m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );

		// 지팡이술에 의한 공격력. 
		pText = GetTxtFromMgr(654);
		StringCchPrintf(buffer, 256, pText, pclPB->siStaffSkillDamage);
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( buffer );
		m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );
	}
	else if ( pclPB->siAttackType == ATTACKTYPE_STAFFWIND ) // 사명대사일 경우
	{
		// 도력에 의한 공격력. 
		pText = GetTxtFromMgr(653);
		StringCchPrintf(buffer, 256, pText, pclPB->GetAddMagicDamage());
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( buffer );
		m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );
	}
	else
	{
		// 근력에 의한 공격력. 
		pText = GetTxtFromMgr(655);
		StringCchPrintf(buffer, 256, pText, pclPB->siStrDamage);
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( buffer );
		m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );
	}

	if ( pclPB->siGeneralDamage )
	{
		// 지휘효과
		pText = GetTxtFromMgr(656);
		StringCchPrintf(buffer, 256, pText, pclPB->siGeneralDamage, '%');
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( buffer );
		m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );
	}


	// 소환수효과
	pText = GetTxtFromMgr(6754);
	StringCchPrintf(buffer, 256, pText, 10);
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );

	SI32 bufAttBonusPercent = 0;

	if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUMMONATACUP ) == true )
	{
		bufAttBonusPercent = max( bufAttBonusPercent , pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent( BUF_SUMMONATACUP ) );
	}

	if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER ) == true )
	{
		bufAttBonusPercent = max( bufAttBonusPercent , 20 );
	}

	// 버프효과
	pText = GetTxtFromMgr(6752);
	StringCchPrintf(buffer, 256, pText, bufAttBonusPercent );
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );		


	SI32 summonskill = 0;
	summonskill = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 );

	SI32 skillAttBonusPercent = 0;
	skillAttBonusPercent = pclClient->GetSummonAttackBonus( summonskill );

	// 버프효과
	pText = GetTxtFromMgr(6753);
	StringCchPrintf(buffer, 256, pText, skillAttBonusPercent );
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_apowername->AddItem( &tmpComboBoxItemData );	

}


// 근력 상세 정보를 설정한다. 
void cltNSummonStatus::ShowDetailStrInfo( cltParametaBox* pclPB)
{
	TCHAR buffer[256];
	TCHAR bufferNew[256]	= "";
	SI32  Lenth				= 0;
	stComboBoxItemData tmpComboBoxItemData;

	// 모든 항목을 삭제한다. 
	m_pCombobox_strength->Clear();

	// 기본 근력 
	TCHAR* pText = GetTxtFromMgr(657);
	StringCchPrintf(buffer, 256, pText, pclPB->pclIP->GetStr());
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_strength->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(bufferNew, 256, pText, pclPB->pclIP->GetStr());
	Lenth = strlen( bufferNew );
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	// 장비에 의한 근력
	pText = GetTxtFromMgr(658);
	StringCchPrintf(buffer, 256, pText, pclPB->clEquipBA.GetStr());
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_strength->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->clEquipBA.GetStr());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	// 말에 의한 근력 
	pText = GetTxtFromMgr(659);
	StringCchPrintf(buffer, 256, pText, pclPB->siHorseStr);
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_strength->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->siHorseStr);
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	// 건강에 의한 근력 
	pText = GetTxtFromMgr(660);
	StringCchPrintf(buffer, 256, pText, pclPB->siHealthStrRate, '%');
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_strength->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->siHealthStrRate);
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
	if(pclClient->IsCountrySwitch(Switch_SummonReform))
	{
		//m_pEditbox_strength->SetUseToolTip( bufferNew );  
	}
}

// 순발력 상세 정보를 설정한다. 
void cltNSummonStatus::ShowDetailDexInfo( cltParametaBox* pclPB )
{
	TCHAR buffer[256];
	TCHAR bufferNew[256]	= "";
	SI32  Lenth				= 0;
	stComboBoxItemData tmpComboBoxItemData;

	// 모든 항목을 삭제한다. 
	m_pCombobox_dex->Clear();

	// 기본 근력 
	TCHAR* pText = GetTxtFromMgr(657);
	StringCchPrintf(buffer, 256, pText, pclPB->pclIP->GetDex());
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_dex->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(bufferNew, 256, pText, pclPB->pclIP->GetDex());
	Lenth = strlen( bufferNew );  
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	// 장비에 의한 근력 
	pText = GetTxtFromMgr(658);
	StringCchPrintf(buffer, 256, pText, pclPB->clEquipBA.GetDex());
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_dex->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->clEquipBA.GetDex());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	// 말에 의한 근력 
	pText = GetTxtFromMgr(659);
	StringCchPrintf(buffer, 256, pText, pclPB->siHorseDex);
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_dex->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->siHorseDex);
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	// 건강에 의한 순발력 
	pText = GetTxtFromMgr(660);
	StringCchPrintf(buffer, 256, pText, pclPB->siHealthDexRate, '%');
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_dex->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->siHealthDexRate );
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
	if(pclClient->IsCountrySwitch(Switch_SummonReform))
	{
		//m_pEditbox_dex->SetUseToolTip( bufferNew );  
	}
}

// 도력 상세 정보를 설정한다. 
void cltNSummonStatus::ShowDetailMagInfo( cltParametaBox* pclPB)
{
	TCHAR buffer[256];
	TCHAR bufferNew[256]	= "";
	SI32  Lenth				= 0;
	stComboBoxItemData tmpComboBoxItemData;

	// 모든 항목을 삭제한다. 
	m_pCombobox_magic->Clear();

	// 기본 근력 
	TCHAR* pText = GetTxtFromMgr(657);
	StringCchPrintf(buffer, 256, pText, pclPB->pclIP->GetMag());
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_magic->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(bufferNew, 256, pText, pclPB->pclIP->GetMag());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	// 장비에 의한 도력 
	pText = GetTxtFromMgr(658);
	StringCchPrintf(buffer, 256, pText, pclPB->clEquipBA.GetMag());
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_magic->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->clEquipBA.GetMag());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	// 말에 의한 도력 
	pText = GetTxtFromMgr(659);
	StringCchPrintf(buffer, 256, pText, pclPB->siHorseMag);
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_magic->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->siHorseMag);
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';


	// 건강에 의한 도력 
	pText = GetTxtFromMgr(660);
	StringCchPrintf(buffer, 256, pText, pclPB->siHealthMagRate, '%');
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_magic->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->siHealthMagRate);
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
	if(pclClient->IsCountrySwitch(Switch_SummonReform))
	{
		//m_pEditbox_magic->SetUseToolTip( bufferNew );  
	}
}

// 생명력 상세 정보를 설정한다. 
void cltNSummonStatus::ShowDetailVitInfo( cltParametaBox* pclPB )
{
	TCHAR buffer[256];
	TCHAR bufferNew[256]	= "";
	SI32  Lenth				= 0;
	stComboBoxItemData tmpComboBoxItemData;

	// 모든 항목을 삭제한다. 
	m_pCombobox_life2->Clear();

	// 기본 근력 
	TCHAR* pText = GetTxtFromMgr(657);
	StringCchPrintf(buffer, 256, pText, pclPB->pclIP->GetVit());
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_life2->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(bufferNew, 256, pText, pclPB->pclIP->GetVit());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';


	// 장비에 의한 근력 
	pText = GetTxtFromMgr(658);
	StringCchPrintf(buffer, 256, pText, pclPB->clEquipBA.GetVit());
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_life2->AddItem( &tmpComboBoxItemData );

	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->clEquipBA.GetVit());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
	if(pclClient->IsCountrySwitch(Switch_SummonReform))
	{
		//m_pEditbox_life2->SetUseToolTip( bufferNew );  
	}
}

// 손재주 상세 정보를 설정한다. 
void cltNSummonStatus::ShowDetailHndInfo( cltParametaBox* pclPB )
{
	TCHAR bufferNew[256]	= "";
	SI32  Lenth				= 0;
	stComboBoxItemData tmpComboBoxItemData;

	TCHAR* pText = GetTxtFromMgr(657);
	StringCchPrintf(bufferNew, 256, pText, pclPB->pclIP->GetHnd());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	pText = GetTxtFromMgr(658);
	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->clEquipBA.GetHnd());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';


	//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
	if(pclClient->IsCountrySwitch(Switch_SummonReform))
	{
		//m_pEditbox_hand->SetUseToolTip( bufferNew );  
	}
}

// 지혜 상세 정보를 설정한다. 
void cltNSummonStatus::ShowDetailWisdomInfo( cltParametaBox* pclPB )
{
	TCHAR bufferNew[256]	= "";
	SI32  Lenth				= 0;

	// 기본 행운
	TCHAR* pText = GetTxtFromMgr(657);
	StringCchPrintf(bufferNew, 256, pText, pclPB->pclIP->GetWis());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r';
	bufferNew[Lenth+1] = ' ';

	// 장비에 의한 근력 
	pText = GetTxtFromMgr(658);
	StringCchPrintf(&bufferNew[Lenth+2], 256, pText, pclPB->clEquipBA.GetWis());
	Lenth = strlen( bufferNew ); 
	bufferNew[Lenth] = '\r'; 
	bufferNew[Lenth+1] = ' ';

	//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
	if(pclClient->IsCountrySwitch(Switch_SummonReform))
	{
		//m_pEditbox_Wisdom->SetUseToolTip( bufferNew );  
	}
}

// 방어력 상세 정보를 설정한다. 
void cltNSummonStatus::ShowDetailArmourInfo( cltParametaBox* pclPB )
{
	TCHAR buffer[256];
	stComboBoxItemData tmpComboBoxItemData;

	// 모든 항목을 삭제한다. 
	m_pCombobox_dpowername->Clear();

	// 레벨에 의한 방어력. 
	TCHAR* pText = GetTxtFromMgr(649);
	StringCchPrintf(buffer, 256, pText, pclPB->siLevelAC);
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_dpowername->AddItem( &tmpComboBoxItemData );

	// 기본 방어력 
	pText = GetTxtFromMgr(657);
	StringCchPrintf(buffer, 256, pText, pclPB->siDefaultAC );
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_dpowername->AddItem( &tmpComboBoxItemData );

	// 진화 
	if(pclPB->siEvolveAC)
	{
		pText = GetTxtFromMgr(652);
		StringCchPrintf(buffer, 256, pText, pclPB->siEvolveAC );
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( buffer );
		m_pCombobox_dpowername->AddItem( &tmpComboBoxItemData );
	}

	// 무장 방어력 
	pText = GetTxtFromMgr(809);
	StringCchPrintf(buffer, 256, pText, pclPB->siEquipAC);
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_dpowername->AddItem( &tmpComboBoxItemData );

	if ( pclPB->siGeneralAC )
	{
		// 지휘효과
		pText = GetTxtFromMgr(656);
		StringCchPrintf(buffer, 256, pText, pclPB->siGeneralAC, '%');
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( buffer );
		m_pCombobox_dpowername->AddItem( &tmpComboBoxItemData );
	}

	SI32 bufDefBonusPercent = 0;

	if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUMMONATACUP ) == true )
	{
		bufDefBonusPercent = max( bufDefBonusPercent , pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent( BUF_SUMMONATACUP ) );
	}

	if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER ) == true )
	{
		bufDefBonusPercent = max( bufDefBonusPercent , 20 );
	}

	// 버프효과
	pText = GetTxtFromMgr(6752);
	StringCchPrintf(buffer, 256, pText, bufDefBonusPercent );
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_dpowername->AddItem( &tmpComboBoxItemData );		



	SI32 summonskill = 0;
	summonskill = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 );

	SI32 skillDefBonusPercent = 0;
	skillDefBonusPercent = pclClient->GetSummonDefBonus( summonskill );

	// 버프효과
	pText = GetTxtFromMgr(6753);
	StringCchPrintf(buffer, 256, pText, skillDefBonusPercent );
	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set( buffer );
	m_pCombobox_dpowername->AddItem( &tmpComboBoxItemData );	
}

void cltNSummonStatus::Action()
{
	InitSummonStatusInfo();

	//[진성] 개편 후 소환수 버프 실시간 적용이 안되 추가. => 2008-5-22
	if(		pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform )								
		&&	BufStatusChange != pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUMMONATACUP))
	{
		UpdateSummonStatusInfo();
		BufStatusChange = !BufStatusChange;
	}
	else if( pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
	{
		UpdateSummon_Hp_Mp_Exp();
	}
	
		

	if( false == pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
	{
		UpdateSummonStatusInfo();
	}

	UpdateSummonPortrait();

}

void cltNSummonStatus::Hide()
{
	m_bShow = false;
	CControllerMgr::Show( false );
}

void cltNSummonStatus::Show()
{
	m_bShow = true;
	CControllerMgr::Show( true );
}

bool cltNSummonStatus::IsShow()
{
	return m_bShow;
}

void cltNSummonStatus::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 20:	// Summon Tab => Strength Minus
	case 21:	// Summon Tab => Dex Minus
	case 22:	// Summon Tab => Maic Minus
	case 23:	// Summon Tab => Life Minus
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SendUseSummonBonusMessageToServer(*((SI16*)pData), true );
			}
		}
		break;
	case 24: // SUMMON_STATUS_DEAD
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 id = *((SI32*)pData);
				SI32 siSummonID = *((SI32*)&pData[ 4 ] );

				cltGameMsgRequest_SummonRevive clinfo( pclCM->CR[id]->GetCharUnique(), siSummonID, SUMMON_REVIVE_TICKET);
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMONREVIVE, sizeof(clinfo), (BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 25:	// summon free!!
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 id = *((SI32*)pData);
				SI32 siSummonID = *((SI32*)&pData[ 4 ] );

				cltGameMsgRequest_SummonFree  clinfo( pclCM->CR[id]->GetCharUnique(), siSummonID);
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMONFREE, sizeof(clinfo), (BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 26:	// 소환수부활이용권 없음 알림창
		{

		}
		break;
	case 27:	// summon revive
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 id = *((SI32*)pData);
				SI32 siSummonID = *((SI32*)&pData[ 4 ] );

				m_pEditbox_health_explain->SetText( TEXT("") );

				cltGameMsgRequest_SummonRevive clinfo( pclCM->CR[id]->GetCharUnique(), siSummonID, SUMMON_REVIVE_TICKET);
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMONREVIVE, sizeof(clinfo), (BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 28 :	// 상태초기화이용권
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 id = *((SI32*)pData);
				SI32 siSummonID = *((SI32*)&pData[ 4 ] );

				m_pEditbox_health_explain->SetText( TEXT("") );

				cltGameMsgRequest_Summon_StatusInit clinfo( pclCM->CR[id]->GetCharUnique(), siSummonID);
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMON_STATUSINIT, sizeof(clinfo), (BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 29 : // PCK - 스태미너 충전 아이템 사용시
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 siSummonID = *((SI32*)pData);

				SI32 itemunique	= ITEMUNIQUE(13625);
				SI32 pos = m_pPersonItemInfo->GetItemPos(itemunique);
				if( pos <= 0 )										{		return;			}

				cltSummonInfo* pclSummonInfo = &pclCM->CR[1]->pclCI->clSummonInfo;		
				if( pclSummonInfo == NULL )							{		return;			}				

				cltSummon *pclSummon = pclSummonInfo->GetSummonFromSummonID(siSummonID);
				if( pclSummon == NULL )								{		return;			}

				if( pclClient->pclKindInfoSet->IsValidKind(pclSummon->siKind) )
				{
					if (pclSummon->siStatus == SUMMON_STATUS_DEAD)	 {		return;			}
				}

				cltGameMsgRequest_Summon_ChargeStamina	clChargeStamina(siSummonID);
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMON_SPTCHARGE, sizeof(clChargeStamina), (BYTE*)&clChargeStamina);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;

	case 31: // 소환스 스텟 적용
		{
			if ( RetValue )
			{
				SendUseSummonBonus();
			}
		}
		break;
	}
	return;
}

void cltNSummonStatus::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) ) 
	{	
		GP.PutSpr( &m_sprGageBack, GetX()+ScreenRX+104, GetY()+ScreenRY+42, 0 );
		DrawCharStatus( ScreenRX+6, ScreenRY );               
	}
	//[진성] 캐릭터 새로운 정보창으로 변경. => 2008-3-24	
	else if(pclClient->IsCountrySwitch(Switch_SummonReform))
	{
		GP.PutSpr( &m_sprGageBack, GetX()+ScreenRX+104, GetY()+ScreenRY+45, 0 );
		DrawCharStatus( ScreenRX+6, ScreenRY );               
	}    
}

void cltNSummonStatus::DrawCharStatus( SI32 ScreenRX, SI32 ScreenRY )
{ 	
	cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclChar == NULL )
	{
		return;
	}

	cltSummonInfo *pclSummonInfo = &pclCM->CR[1]->pclCI->clSummonInfo;
	if(pclSummonInfo == NULL) 
	{
		return;
	}
	TCHAR SummonName[256]=TEXT("");
	SI32 SummonIndex = 0;
	if(GetComboSummonNameIndex( SummonName, 256, &SummonIndex ) == FALSE) 
		return;

	cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
	if(pclSummon==NULL) return;


	//cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
	cltKindInfo* pclki = NULL;
	cltGMCommandKindInfo*	pclGMki = NULL;

	if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
	{
		pclki = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
		pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclSummon->siKind];
	}
	else
	{
		pclki = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
		pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclSummon->siKind];
	}

	if(pclki == NULL )
		return;
	if(pclGMki == NULL)
		return;

	cltPIPInfo			clipinfo2;
	clipinfo2.Set(&pclSummon->clIP);
	cltParametaBox clPB;
	clPB.Init(pclClient->pclItemManager, &clipinfo2, NULL, pclki, pclGMki);

	cltItem* pclitemlist = NULL;

	// 소환수 장비를 갖고 온다
	if ( (0 <= SummonIndex) || (MAX_SUMMON_OWN > SummonIndex) )
	{
		pclitemlist = pclCM->CR[1]->pclCI->clSummonInfo.GetSummonEquipItemList( SummonIndex );
	}

	if ( NULL == pclitemlist )
	{
		return;
	}

	SI32 siSummonID				= pclCM->CR[1]->GetSummonID();
	SI16 siAttackWearStoneNum	= pclSummon->siAttackWearStoneNum;
	SI16 siDefenseWearStoneNum	= pclSummon->siDefenseWearStoneNum;

	if ( (pclSummon->siStatus == SUMMON_STATUS_TAKEOUT) && (true == pclCM->IsValidID( siSummonID )) )
	{
		cltCharCommon* pclSummonCommon = (cltCharCommon*)pclCM->CR[siSummonID];
		if ( NULL == pclSummonCommon )
		{
			return;
		}

		clPB.Action(pclSummonCommon, pclSummonCommon->pclCI->GetPersonID(), PERSONITEM_WEAPON1, pclitemlist, &pclSummonCommon->pclCI->clHorseInfo, &pclSummonCommon->pclCI->clHealthInfo, &pclSummonCommon->pclCI->clCharSkill, &pclSummonCommon->clIdentity, siAttackWearStoneNum, siDefenseWearStoneNum, GAMEMODE_CLIENT, true );

	}
	else
	{
		clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, siAttackWearStoneNum, siDefenseWearStoneNum, GAMEMODE_CLIENT, true);
	}

	SI32 siTransformPos = 0;
	if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
	{
		siTransformPos = -3;  
	}

	SI32 i;
	SI32 siNowX							= GetX();
	SI32 siNowY							= GetY();

	SI32 siRealHPStartX					= siNowX + ScreenRX + CHARSTATUS_HP_START_X;
	SI32 siRealHPStartY					= siNowY + ScreenRY + CHARSTATUS_HP_START_Y + siTransformPos;

	SI32 siRealMPStartX					= siNowX + ScreenRX + CHARSTATUS_MP_START_X;
	SI32 siRealMPStartY					= siNowY + ScreenRY + CHARSTATUS_MP_START_Y + siTransformPos;

	SI32 siRealEXPStartX				= siNowX + ScreenRX + CHARSTATUS_EXP_START_X;
	SI32 siRealEXPStartY				= siNowY + ScreenRY + CHARSTATUS_EXP_START_Y + siTransformPos;

	SI32 siRealTransformStaminaStartX	= siNowX + ScreenRX + CHARSTATUS_TRANSFORM_STAMINA_START_X;  
	SI32 siRealTransformStaminaStartY	= siNowY + ScreenRY + CHARSTATUS_TRANSFORM_STAMINA_START_Y + siTransformPos;

	//-----------------------------------------------------------------------------
	//	HP 표시 [시작]
	//-----------------------------------------------------------------------------
	SI32	siNowHp			= pclSummon->clIP.GetLife();
	SI32	siMaxHp			= clPB.GetMaxLife();
	SI32	siHealHp		= pclSummon->clIP.Life + clPB.siReservedLife;

	REAL32	f32LifeRate		= 0.0f;
	REAL32	f32HealLifeRate = 0.0f;

	if(siNowHp == siMaxHp)
	{
		f32LifeRate = 1.f;
	}

	if ( (siNowHp > siMaxHp) && (siMaxHp > 0) )
	{
		siNowHp = siMaxHp;
		f32LifeRate =	siNowHp / ( siMaxHp * 1.0f );
	}
	else if ( siMaxHp > 0 )
	{
		f32LifeRate	=	siNowHp / ( siMaxHp * 1.0f );
	}

	if ( (siHealHp > siMaxHp) && (siMaxHp > 0) )
	{
		siHealHp = siMaxHp ;
		f32HealLifeRate =	siHealHp / ( siMaxHp * 1.0f );
	}
	else if ( siHealHp > 0 )
	{
		f32HealLifeRate	=	siHealHp / ( siMaxHp * 1.0f );
	}

	SI32 siHpWidth		= (SI32)(CHARSTATUS_BAR_WIDTH * f32LifeRate);
	SI32 siHealWidth	= (SI32)(CHARSTATUS_BAR_WIDTH * f32HealLifeRate) ;

	if ( siHpWidth > 2 )
	{
		GP.PutSprScreen( &m_sprGage, siRealHPStartX , siRealHPStartY, CHARSTATUS_HP_NUM );
		for ( i = 2; i < siHealWidth - 2; i+=2 )
		{
			GP.PutSprScreen( &m_sprGage, siRealHPStartX + i, siRealHPStartY, CHARSTATUS_HP_NUM + 1 );
		}
		GP.PutSprScreen( &m_sprGage, siRealHPStartX + i, siRealHPStartY, CHARSTATUS_HP_NUM + 2 );

		GP.PutSpr( &m_sprGage, siRealHPStartX , siRealHPStartY, CHARSTATUS_HP_NUM );
		for ( i = 2; i < siHpWidth - 2; i+=2 )
		{
			GP.PutSpr( &m_sprGage, siRealHPStartX + i, siRealHPStartY, CHARSTATUS_HP_NUM + 1 );
		}
		GP.PutSpr( &m_sprGage, siRealHPStartX + i, siRealHPStartY, CHARSTATUS_HP_NUM + 2 );
	}
	else if ( siHpWidth > 0 )
	{
		GP.PutSprScreen( &m_sprGage, siRealHPStartX , siRealHPStartY, CHARSTATUS_HP_NUM + 2 );
		GP.PutSpr( &m_sprGage, siRealHPStartX , siRealHPStartY, CHARSTATUS_HP_NUM + 2 );
	}
	//-----------------------------------------------------------------------------
	//	HP 표시 [종료]
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	//	MP 표시 [시작]
	//-----------------------------------------------------------------------------
	SI32	siNowMp	= pclSummon->clIP.GetMana();
	SI32	siMaxMP	= clPB.GetMaxMana();

	REAL32	f32ManaRate	= 0.0f; 


	if ( (siNowMp > siMaxMP) && (siMaxMP > 0) )
	{
		siNowMp = siMaxMP;
		f32ManaRate = siNowMp / ( siMaxMP * 1.0f );
	}
	else if ( siMaxMP > 0 )
	{
		f32ManaRate = siNowMp / ( siMaxMP * 1.0f );
	}

	SI32 siMpWidth = (SI32)(CHARSTATUS_BAR_WIDTH * f32ManaRate);

	if ( siMpWidth > 2 )
	{
		GP.PutSpr( &m_sprGage, siRealMPStartX , siRealMPStartY, CHARSTATUS_MP_NUM );
		for ( i = 2; i < siMpWidth - 2 ; i+=2 )
		{
			GP.PutSpr( &m_sprGage, siRealMPStartX + i, siRealMPStartY, CHARSTATUS_MP_NUM + 1 );
		}
		GP.PutSpr( &m_sprGage, siRealMPStartX + i, siRealMPStartY, CHARSTATUS_MP_NUM + 2 );
	}
	else if ( siMpWidth > 0 )
	{
		GP.PutSpr( &m_sprGage, siRealMPStartX , siRealMPStartY, CHARSTATUS_MP_NUM + 2 );
	}
	//-----------------------------------------------------------------------------
	//	MP 표시 [종료]
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	//	EXP 표시 [시작]
	//-----------------------------------------------------------------------------
	SI64	siCurExp	= pclSummon->clIP.GetCurExp();// SI32->SI64 경험치 오버플로우때문에 수정
	SI64	siPreExp	= clPB.GetPrevExp();// SI32->SI64 경험치 오버플로우때문에 수정
	SI64	siNextExp	= clPB.GetNextExp();// SI32->SI64 경험치 오버플로우때문에 수정

	REAL64	f32ExpRate	= 0.0f;// SI32->SI64 경험치 오버플로우때문에 수정


	if ( siNextExp == siPreExp )
	{
		f32ExpRate = 1.0f;
	}
	else
	{
		f32ExpRate = ( siCurExp - siPreExp ) / ( ( siNextExp - siPreExp ) * 1.0f );
	}

	if ( f32ExpRate > 1.0f )
	{
		f32ExpRate = 1.0f;
	}
	else if ( f32ExpRate < 0.0f )
	{
		f32ExpRate = 0.0f;
	}
	
	// 공용 바 길이 기준
	SI32 siExpWidth = (SI32)(BASE_BAR_WIDTH * f32ExpRate);
	// 공용 바에 대한 상태 정보 바의 비율 
	REAL32 fBarRate = (REAL32) CHARSTATUS_BAR_WIDTH / (REAL32)BASE_BAR_WIDTH;
	SI32 siTmpIncreaseValue = 0;
	for ( i = 0; i < siExpWidth  ; i+=2 )
	{
		siTmpIncreaseValue = i * fBarRate ; 
		//홀수라면
		if((siTmpIncreaseValue % 2) == 1)
			--siTmpIncreaseValue;
		GP.PutSpr( &m_sprGage, siRealEXPStartX + siTmpIncreaseValue, siRealEXPStartY, CHARSTATUS_EXP_NUM + 1 );
	}
	//-----------------------------------------------------------------------------
	//	EXP 표시 [종료]
	//-----------------------------------------------------------------------------


	if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) )
	{
		//-----------------------------------------------------------------------------
		//	소환수 변신 스태미너 표시 [시작]
		//-----------------------------------------------------------------------------
		siNowMp	= pclSummon->m_siTranformStamina;
		siMaxMP	= MAX_SUMMON_TRANSFORM_TIME; 

		f32ManaRate	= 0.0f; 


		if ( (siNowMp > siMaxMP) && (siMaxMP > 0) )
		{
			siNowMp = siMaxMP;
			f32ManaRate = siNowMp / ( siMaxMP * 1.0f );
		}
		else if ( siMaxMP > 0 )
		{
			f32ManaRate = siNowMp / ( siMaxMP * 1.0f );
		}

		siMpWidth = (SI32)(CHARSTATUS_TRANSFORM_STAMINA_BAR_WIDTH * f32ManaRate); 

		if ( siMpWidth > 2 )
		{
			GP.PutSpr( &m_sprTGage1, siRealTransformStaminaStartX , siRealTransformStaminaStartY, 0 );
			for ( i = 2; i < siMpWidth - 2 ; i+=2 )
			{
				GP.PutSpr( &m_sprTGage1, siRealTransformStaminaStartX + i, siRealTransformStaminaStartY, 1 );
			}
			GP.PutSpr( &m_sprTGage1, siRealTransformStaminaStartX + i, siRealTransformStaminaStartY, 2 );
		}
		else if ( siMpWidth > 0 )
		{
			GP.PutSpr( &m_sprTGage1, siRealTransformStaminaStartX , siRealTransformStaminaStartY, 2 );
		}
		
		GP.PutSpr( &m_sprTTime, siRealTransformStaminaStartX+5 , siRealTransformStaminaStartY, 0 );  
		//-----------------------------------------------------------------------------
		//	소환수 변신 스태미너 표시 [종료]
		//-----------------------------------------------------------------------------
	}
}

void cltNSummonStatus::SetNewAbility( cltParametaBox* pclPB )
{
	TCHAR buffer[64];

	SI32 bonusPercent	= 0;
	SI32 totalMinDamage = pclPB->GetTotalPHysicalMinAttackDmg();
	SI32 totalMaxDamage = pclPB->GetTotalPHysicalMaxAttackDmg();
	SI32 totalAC		= pclPB->GetPHysicalDefensive();

	//[진성] 소환수 관련 버프 능력 적용 => 2008-5-19
	/*
	if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUMMONATACUP) == true )
	{
		bonusPercent = max( bonusPercent, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_SUMMONATACUP) );
	}
	if( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) == true )
	{
		bonusPercent = max( bonusPercent, 20 );
	}
	*/

	totalMinDamage += totalMinDamage * bonusPercent / 100;
	totalMaxDamage += totalMaxDamage * bonusPercent / 100;
	totalAC		   += totalAC		 * bonusPercent / 100;

	/*StringCchPrintf(buffer, sizeof(buffer), "%d-%d",	pclPB->GetTotalPHysicalMinAttackDmg(),
	pclPB->GetTotalPHysicalMaxAttackDmg());*/
		StringCchPrintf(buffer, sizeof(buffer), "%d-%d",	totalMinDamage,	totalMaxDamage);
	m_pEditbox_physical_attack->SetText(buffer, RGB(128,0,0));			

	/*StringCchPrintf(buffer, sizeof(buffer), "%d",		pclPB->GetPHysicalDefensive());*/
	StringCchPrintf(buffer, sizeof(buffer), "%d",		totalAC);
	m_pEditbox_pysical_defense->SetText(buffer, RGB(128,0,0));			

	StringCchPrintf(buffer, sizeof(buffer), "%d-%d",    pclPB->GetTotalMagicalMinAttackDmg(), 
		pclPB->GetTotalMagicalMaxAttackDmg());
	m_pEditbox_magic_attack->SetText(buffer,	RGB(128,0,0));			

	StringCchPrintf(buffer, sizeof(buffer), "%d",		pclPB->GetMagicalDefensive()); 
	m_pEditbox_magic_defense->SetText(buffer,	RGB(128,0,0));			

	StringCchPrintf(buffer, sizeof(buffer), "%d",		pclPB->GetPHysicalHitRate()); 
	m_pEditbox_pysical_hit_rank->SetText(buffer, RGB(128,0,0));			

	StringCchPrintf(buffer, sizeof(buffer), "%d",		pclPB->GetPHysicalDodgeRateRank()); 
	m_pEditbox_pysical_dodge_rank->SetText(buffer, RGB(128,0,0));			

	StringCchPrintf(buffer, sizeof(buffer), "%d",		pclPB->GetMagicalHitRate()); 
	m_pEditbox_magic_hit_rank->SetText(buffer,	RGB(128,0,0));			

	StringCchPrintf(buffer, sizeof(buffer), "%d",		pclPB->GetMagicalDodgeRateRank()); 
	m_pEditbox_magic_dodge_rank->SetText(buffer, RGB(128,0,0));			

	StringCchPrintf(buffer, sizeof(buffer), "%d",		pclPB->GetTotalMoveSpeed()); 
	m_pEditbox_move_speed->SetText(buffer, RGB(128,0,0));			
}

void cltNSummonStatus::SetBaseStatusToolTip(void)
{
	m_pEdit_strength_new->SetText(GetTxtFromMgr(4369));	
	m_pEdit_strength_new->SetUseToolTip(GetTxtFromMgr(8269));

	m_pEdit_hand_new->SetText(GetTxtFromMgr(4373));		
	m_pEdit_hand_new->SetUseToolTip(GetTxtFromMgr(8270));

	m_pEdit_dex_new->SetText(GetTxtFromMgr(4370));			
	m_pEdit_dex_new->SetUseToolTip(GetTxtFromMgr(8271));

	m_pEdit_magic_new->SetText(GetTxtFromMgr(4371));		
	m_pEdit_magic_new->SetUseToolTip(GetTxtFromMgr(8272));

	m_pEdit_Wisdom_new->SetText(GetTxtFromMgr(8000));	 	 
	m_pEdit_Wisdom_new->SetUseToolTip(GetTxtFromMgr(8273));

	m_pEdit_life2_new->SetText(GetTxtFromMgr(4368));		
	m_pEdit_life2_new->SetUseToolTip(GetTxtFromMgr(8274));
}

// 게이지 관련 정보만 갱신하자.
void cltNSummonStatus::UpdateSummon_Hp_Mp_Exp()
{
	cltSummonInfo *pclSummonInfo = &pclCM->CR[1]->pclCI->clSummonInfo;
	if(pclSummonInfo == NULL) 
	{
		return;
	}
	TCHAR SummonName[256]=TEXT("");
	SI32 SummonIndex = 0;
	if(GetComboSummonNameIndex( SummonName, 256, &SummonIndex ) == FALSE) 
		return;

	cltSummon *pclSummon = pclSummonInfo->GetSummon(SummonIndex);
	if(pclSummon==NULL) return;


	//cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
	cltKindInfo* pclki = NULL;
	cltGMCommandKindInfo* pclGMki = NULL;

	if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
	{
		pclki = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
		pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclSummon->siKind];
	}
	else
	{
		pclki = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
		pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclSummon->siKind];
	}


	if(pclki == NULL )
		return;

	if(pclGMki == NULL)
		return;

	cltPIPInfo			clipinfo2;
	clipinfo2.Set(&pclSummon->clIP);
	cltParametaBox clPB;
	clPB.Init(pclClient->pclItemManager, &clipinfo2, NULL, pclki, pclGMki);

	//--------------------------------------
	// 소환수용 장비를 확보한다. 
	//--------------------------------------
	cltItem* pclitemlist = pclCM->CR[1]->pclCI->clSummonInfo.GetSummonEquipItemList( SummonIndex );
	if ( NULL == pclitemlist )
	{
		return;
	}

	SI16 siAttackWearStoneNum	= pclSummon->siAttackWearStoneNum;
	SI16 siDefenseWearStoneNum	= pclSummon->siDefenseWearStoneNum;
	SI32 siSummonID				= pclCM->CR[1]->GetSummonID();
	bool bSummoed				= false;

	// 현재 나와있는 소환수라면 장비에 의한 능력치까지 계산한다
	if ( (pclSummon->siStatus == SUMMON_STATUS_TAKEOUT) && (true == pclCM->IsValidID( siSummonID )) )
	{
		cltCharCommon* pclSummonCommon = (cltCharCommon*)pclCM->CR[siSummonID];
		if ( NULL == pclSummonCommon )
		{
			return;
		}

		clPB.Action(pclSummonCommon, 0, PERSONITEM_WEAPON1, pclitemlist, &pclSummonCommon->pclCI->clHorseInfo, &pclSummonCommon->pclCI->clHealthInfo, &pclSummonCommon->pclCI->clCharSkill, &pclSummonCommon->clIdentity, siAttackWearStoneNum, siDefenseWearStoneNum, GAMEMODE_CLIENT, true );

		bSummoed = true;

	}
	else
	{
		clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, siAttackWearStoneNum, siDefenseWearStoneNum, GAMEMODE_CLIENT, true );
	}

	
	TCHAR buffer[256] = "";
	StringCchPrintf(buffer, 256, TEXT("%d / %d"), pclSummon->clIP.GetLife(), clPB.GetMaxLife());
	m_pStatictext_HP->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	StringCchPrintf(buffer, 256, TEXT("%d / %d"), pclSummon->clIP.GetMana(), clPB.GetMaxMana());
	m_pStatictext_MP->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );		

	StringCchPrintf(buffer, 256, TEXT("%I64d"), pclSummon->clIP.GetCurExp());
	m_pStatictext_EXP->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );		

	StringCchPrintf(buffer, 256, TEXT("%I64d"), clPB.GetNextExp());
	m_pStatictext_FULL_EXP->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	if(pclClient->IsCountrySwitch(Switch_SummonTramsform))
	{
		StringCchPrintf(buffer, 256, TEXT("%d / %d"), pclSummon->m_siTranformStamina / 10, 60);
		m_pStatictext_TransformStamina->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );		
	}
}

//#endif