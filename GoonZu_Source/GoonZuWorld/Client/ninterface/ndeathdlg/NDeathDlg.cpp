#include "NDeathDlg.h"

#include "Client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/interface/Static.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"


extern cltCommonLogic* pclClient;

#define DEATHDLG_TIME		200



CNDeathDlg::CNDeathDlg()
{
	m_siBitmapAnimationIndex = 0 ;
	m_siSex = 0 ;

	m_pDeathInfo1Image = NULL;
	m_pDeathStatic = NULL;     
	Popupflag = 0;
	
	SetTimerOnOff( true );
}

CNDeathDlg::~CNDeathDlg()
{
	m_siBitmapAnimationIndex = 0;
	SAFE_DELETE(m_pDeathInfo1Image);
	SAFE_DELETE(m_pDeathStatic);
	Popupflag = 0;
	SetTimerOnOff( false );
}

void CNDeathDlg::Create()
{	
	if(!IsCreate())
	{		
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/DLG_Death.ddf"));
		file.CreatePopupDialog( this, NDEATH_DLG, TEXT("dialog_death"), StatiCNDeathDlgProc);		

		m_pDeathInfo1Image = new CImageStatic( this );             
		m_pDeathStatic = new CStatic( this );       

		file.CreateControl(m_pDeathInfo1Image, NDEATH_DIALOG_DEATHINFO1IMAGE, TEXT("imagestatic_death_info1"));
		file.CreateControl(m_pDeathStatic, NDEATH_DIALOG_DEATHSTATIC, TEXT("statictext_death"));
	}
	
	SI32 charKind = pclClient->pclCM->CR[1]->GetKind();
	switch( charKind )
	{
	case KIND_HERO3:
        m_pDeathInfo1Image->SetFileName(TEXT("Interface/DeathHero/DeathHero0.SPR"));
		break;
	case KIND_HERO4:
		m_pDeathInfo1Image->SetFileName(TEXT("Interface/DeathHero/DeathHero1.SPR"));
		break;
	case KIND_HEROINE3:
		m_pDeathInfo1Image->SetFileName(TEXT("Interface/DeathHeroine/DeathHeroine0.SPR"));
		break;
	case KIND_HEROINE4:
		m_pDeathInfo1Image->SetFileName(TEXT("Interface/DeathHeroine/DeathHeroine1.SPR"));
		break;
	default:
		if( charKind % 2 == 1 )
		{
			m_pDeathInfo1Image->SetFileName(TEXT("Interface/DeathHero/DeathHero0.SPR"));
		}
		else
		{
			m_pDeathInfo1Image->SetFileName(TEXT("Interface/DeathHeroine/DeathHerine0.SPR"));
		}
		break;
	}
}

void CALLBACK CNDeathDlg::StatiCNDeathDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNDeathDlg *pThis = (CNDeathDlg*) pControllerMgr;
	pThis->NDeathDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNDeathDlg::NDeathDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}

	return;
}

void CNDeathDlg::set(GMONEY money , SI64 exp, SI16 killmode, TCHAR* pname )// SI32->SI64 경험치 오버플로우때문에 수정
{
	Popupflag = 0;
	SetActionTimer( DEATHDLG_TIME );

	if(!IsCreate())
	{		
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/DLG_Death.ddf"));
		file.CreatePopupDialog( this, NDEATH_DLG, TEXT("dialog_death"), StatiCNDeathDlgProc);		

		m_pDeathInfo1Image = new CImageStatic( this );             
		m_pDeathStatic = new CStatic( this );       

		file.CreateControl(m_pDeathInfo1Image, NDEATH_DIALOG_DEATHINFO1IMAGE, TEXT("imagestatic_death_info1"));
		file.CreateControl(m_pDeathStatic, NDEATH_DIALOG_DEATHSTATIC, TEXT("statictext_death"));
	}
	

	TCHAR strbuf[256] = TEXT("") ;

	TCHAR* pText= GetTxtFromMgr(3357);
	TCHAR money1[128]= TEXT(" ") ;
	TCHAR exp1[128]= TEXT(" ") ;
	
	StringCchPrintf(money1, 128, TEXT("%I64d"),money);
	StringCchPrintf(exp1, 128, TEXT("%d"),exp);
	Nsprintf(strbuf,pText,TEXT("monster"),  pname  ,TEXT("money"),   money1 ,TEXT("exp"),  exp1  ,NULL);

	// 내구도 소모에 관련된 텍스트도 추가해준다
	if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		NTCHARString128 strDurabilityText = GetTxtFromMgr(40168);

		StringCchCat( strbuf, sizeof(strbuf), strDurabilityText );
	}
	

	m_pDeathStatic->SetBorder(true);
	m_pDeathStatic->SetBKColorUse( true );
	m_pDeathStatic->SetText( strbuf ,DT_WORDBREAK);

	return;
}

void CNDeathDlg::Action()
{
	m_pDeathInfo1Image->SetFontIndex(m_siBitmapAnimationIndex++);		

	if ( m_siBitmapAnimationIndex >= 4 )
		m_siBitmapAnimationIndex = 0;

	Popupflag++;
	if(Popupflag>200)
	{
		
		DeleteDialog();
	}
}