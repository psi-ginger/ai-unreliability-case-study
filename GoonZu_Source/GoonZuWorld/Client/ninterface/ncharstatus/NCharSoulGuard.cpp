#include "./NCharSoulGuard.h"
#include "./NCharSoulGuardHouse.h"
#include "./NCharStatus.h"

#include "../Client/Client.h"

#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "Msg/MsgType-Horse.h"
#include "Msg/MsgType-Summon.h"
#include "Msg/MsgType-SoulGuard.h"
#include "../../../Common/JWLib/IsCorrectID/IsCorrectID.h"

#include "Char/CharManager/CharManager.h"

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

extern cltCommonLogic* pclClient;

cltNSoulGuardStatus::cltNSoulGuardStatus(cltCharManager* pclcm)
{

	m_pclCM					= pclcm;

	m_bShow					= false;
	m_siCharUnique			= 0;
	m_siShowSoulGuardKind	= 0;
	m_siShowStatus			= 0;
	m_siShowFellowShip		= 0;

	ZeroMemory( m_szShowSoulGuardName, sizeof(m_szShowSoulGuardName) );

	m_siFreeSoulGuardID		= 0;

	// 초기값을 0으로 할경우 보관소에 아무것도 없다면 업데이트를 하지 못한다 그래서 -1로 초기화
	memset( m_siCheckSoulGuardHouse, -1, sizeof(m_siCheckSoulGuardHouse) );
	m_siShowHouseNum		= 0;

	m_siCurrentSelectHouse	= -1;	// 배열 첨자는 0부터 시작이니까 -1로 초기화

	// 컨트롤러
	m_pclImage_Portrait		= NULL;

	m_pclStatic_Noname1		= NULL;
	m_pclStatic_Noname2		= NULL;
	m_pclStatic_Noname3		= NULL;
	m_pclStatic_Noname4		= NULL;
	m_pclStatic_FellowShip	= NULL;
	m_pclStatic_Life		= NULL;
	m_pclStatic_Noname7		= NULL;
	m_pclStatic_Noname8		= NULL;

	m_pclButton_NameChange	= NULL;
	m_pclButton_InOut		= NULL;
	m_pclButton_Free		= NULL;
	m_pclButton_HouseCreate	= NULL;

	m_pclEdit_Name			= NULL;
	m_pclEdit_Basic			= NULL;
	m_pclEdit_Special		= NULL;

	m_pclOutLine_Noname1	= NULL;
	m_pclOutLine_Noname2	= NULL;
	m_pclOutLine_Noname3	= NULL;

	for ( SI32 siIndex=0; siIndex<MAX_SOULGUARD_HOUSE_NUM; siIndex++ )
	{
		m_pclImage_Back[siIndex]	= NULL;
		m_pclImage_House[siIndex]	= NULL;
		m_pclButton_House[siIndex]	= NULL;
	}
	
	m_clAniInfo.Init1();
}

cltNSoulGuardStatus::~cltNSoulGuardStatus()
{
	SAFE_DELETE( m_pclImage_Portrait );

	SAFE_DELETE( m_pclStatic_Noname1 );
	SAFE_DELETE( m_pclStatic_Noname2 );
	SAFE_DELETE( m_pclStatic_Noname3 );
	SAFE_DELETE( m_pclStatic_Noname4 );
	SAFE_DELETE( m_pclStatic_FellowShip );
	SAFE_DELETE( m_pclStatic_Life );
	SAFE_DELETE( m_pclStatic_Noname7 );
	SAFE_DELETE( m_pclStatic_Noname8 );

	SAFE_DELETE( m_pclButton_NameChange );
	SAFE_DELETE( m_pclButton_InOut );
	SAFE_DELETE( m_pclButton_Free );
	SAFE_DELETE( m_pclButton_HouseCreate );

	SAFE_DELETE( m_pclEdit_Name );
	SAFE_DELETE( m_pclEdit_Basic );
	SAFE_DELETE( m_pclEdit_Special );

	SAFE_DELETE( m_pclOutLine_Noname1 );
	SAFE_DELETE( m_pclOutLine_Noname2 );
	SAFE_DELETE( m_pclOutLine_Noname3 );

	for ( SI32 siIndex=0; siIndex<MAX_SOULGUARD_HOUSE_NUM; siIndex++ )
	{
		SAFE_DELETE( m_pclImage_Back[siIndex] );
		SAFE_DELETE( m_pclImage_House[siIndex] );
		SAFE_DELETE( m_pclButton_House[siIndex] );
	}

	SetTimerOnOff( false );
}

void cltNSoulGuardStatus::Create( CControllerMgr *pParent )
{
	cltClient *pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	if ( NULL == pParent )
	{
		return;
	}
	
	// 초기 설정
	m_siCharUnique = pclclient->MyCharUnique;
	

	// 다이얼 로그 생성
	CInterfaceFile file;

	file.LoadFile( TEXT("NInterface/Data/NCharStatusDlg/DLG_NCharStatus_Child_NStatusSoulGuard.ddf") );
	file.CreateChildDialog( this, CLTNSOULGUARDSTATUS_DLG, TEXT("dialog_soulguard"), NSoulGuardStatusStaticProc, pParent );

	// 다이얼로그 위치 변경
	SetDialogPos( ((cltNCharStatus*)pParent)->GetChildDlgXPos(), ((cltNCharStatus*)pParent)->GetChildDlgYPos(), GetWidth(), GetHeight() );

	// 컨트롤러 생성
	NEWCIMAGESTATIC( m_pclImage_Portrait );	file.CreateControl( m_pclImage_Portrait,	CLTNSOULGUARDSTATUS_DLG_IMAGESTATIC_PORTRAIT,	TEXT("imagestatic_portrait") );

	NEWCSTATIC( m_pclStatic_Noname1 );		file.CreateControl( m_pclStatic_Noname1,	CLTNSOULGUARDSTATUS_DLG_STATICTEXT_NONAME1,		TEXT("statictext_NONAME1") );
	NEWCSTATIC( m_pclStatic_Noname2 );		file.CreateControl( m_pclStatic_Noname2,	CLTNSOULGUARDSTATUS_DLG_STATICTEXT_NONAME2,		TEXT("statictext_NONAME2") );
	NEWCSTATIC( m_pclStatic_Noname3 );		file.CreateControl( m_pclStatic_Noname3,	CLTNSOULGUARDSTATUS_DLG_STATICTEXT_NONAME3,		TEXT("statictext_NONAME3") );
	NEWCSTATIC( m_pclStatic_Noname4 );		file.CreateControl( m_pclStatic_Noname4,	CLTNSOULGUARDSTATUS_DLG_STATICTEXT_NONAME4,		TEXT("statictext_NONAME4") );
	NEWCSTATIC( m_pclStatic_FellowShip );	file.CreateControl( m_pclStatic_FellowShip,	CLTNSOULGUARDSTATUS_DLG_STATICTEXT_FELLOWSHIP,	TEXT("statictext_fellowship") );
	NEWCSTATIC( m_pclStatic_Life );			file.CreateControl( m_pclStatic_Life,		CLTNSOULGUARDSTATUS_DLG_STATICTEXT_LIFE,		TEXT("statictext_life") );
	NEWCSTATIC( m_pclStatic_Noname7 );		file.CreateControl( m_pclStatic_Noname7,	CLTNSOULGUARDSTATUS_DLG_STATICTEXT_NONAME7,		TEXT("statictext_NONAME7") );
	NEWCSTATIC( m_pclStatic_Noname8 );		file.CreateControl( m_pclStatic_Noname8,	CLTNSOULGUARDSTATUS_DLG_STATICTEXT_NONAME8,		TEXT("statictext_NONAME8") );

	NEWCBUTTON( m_pclButton_NameChange );	file.CreateControl( m_pclButton_NameChange,	CLTNSOULGUARDSTATUS_DLG_BUTTON_NAMECHANGE,		TEXT("button_namechange") );
	NEWCBUTTON( m_pclButton_InOut );		file.CreateControl( m_pclButton_InOut,		CLTNSOULGUARDSTATUS_DLG_BUTTON_INOUT,			TEXT("button_inout") );
	NEWCBUTTON( m_pclButton_Free );			file.CreateControl( m_pclButton_Free,		CLTNSOULGUARDSTATUS_DLG_BUTTON_FREE,			TEXT("button_free") );
	NEWCBUTTON( m_pclButton_HouseCreate );	file.CreateControl( m_pclButton_HouseCreate,CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSECREATE,		TEXT("button_housecreate") );

	NEWCEDIT( m_pclEdit_Name );				file.CreateControl( m_pclEdit_Name,			CLTNSOULGUARDSTATUS_DLG_EDITBOX_NAME,			TEXT("editbox_name") );
	NEWCEDIT( m_pclEdit_Basic );			file.CreateControl( m_pclEdit_Basic,		CLTNSOULGUARDSTATUS_DLG_EDITBOX_BASIC,			TEXT("editbox_basic") );
	NEWCEDIT( m_pclEdit_Special );			file.CreateControl( m_pclEdit_Special,		CLTNSOULGUARDSTATUS_DLG_EDITBOX_SPECIAL,		TEXT("editbox_special") );

	NTCHARString64 strControllerName;

	for ( SI32 siIndex=0; siIndex<MAX_SOULGUARD_HOUSE_NUM; siIndex++ )
	{
		// 버튼
		strControllerName.FormatString( TEXT("button_house%d"), (siIndex+1) );
		NEWCBUTTON( m_pclButton_House[siIndex] );
		file.CreateControl( m_pclButton_House[siIndex],	(CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE1+siIndex),	strControllerName );

		// 초상 이미지
		strControllerName.FormatString( TEXT("imagestatic_house%d"), (siIndex+1) );
		NEWCIMAGESTATIC( m_pclImage_House[siIndex] );
		file.CreateControl( m_pclImage_House[siIndex],	0,	strControllerName );

		// 배경 이미지
		strControllerName.FormatString( TEXT("imagestatic_back%d"), (siIndex+1) );
		NEWCIMAGESTATIC( m_pclImage_Back[siIndex] );
		file.CreateControl( m_pclImage_Back[siIndex],	0,	strControllerName );
	}

	NEWCOUTLINE( m_pclOutLine_Noname1 );	file.CreateControl( m_pclOutLine_Noname1,	CLTNSOULGUARDSTATUS_DLG_OUTLINE_NONAME1,		TEXT("outline_NONAME1") );
	NEWCOUTLINE( m_pclOutLine_Noname2 );	file.CreateControl( m_pclOutLine_Noname2,	CLTNSOULGUARDSTATUS_DLG_OUTLINE_NONAME2,		TEXT("outline_NONAME2") );
	NEWCOUTLINE( m_pclOutLine_Noname3 );	file.CreateControl( m_pclOutLine_Noname3,	CLTNSOULGUARDSTATUS_DLG_OUTLINE_NONAME3,		TEXT("outline_NONAME3") );

	m_pclEdit_Name->SetMaxEditTextLength( MAX_SOULGUARD_NAME );

	SetTimerOnOff( true );
		
}

void CALLBACK cltNSoulGuardStatus::NSoulGuardStatusStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNSoulGuardStatus* pThis = (cltNSoulGuardStatus*)pControllerMgr;
	if ( pThis )
	{
		pThis->NSoulGuardStatusProc( nEvent, nControlID, pControl);
	}
}

void CALLBACK cltNSoulGuardStatus::NSoulGuardStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_NAMECHANGE:
		{
			OnButtonClickNameChange();
		}
		break;

	case CLTNSOULGUARDSTATUS_DLG_BUTTON_INOUT:
		{
			OnButtonClickInOut();
		}
		break;

	case CLTNSOULGUARDSTATUS_DLG_BUTTON_FREE:
		{
			OnButtonClickFree();
		}
		break;

	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE1:
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE2:
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE3:
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE4:
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE5:
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE6:
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE7:
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE8:
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE9:
	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE10:
		{
			OnButtonClickHouse( nControlID );
		}
		break;

	case CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSECREATE:
		{
			OnButtonClickHouseCreate();
		}
		break;
	}
}

bool cltNSoulGuardStatus::IsShow()
{
	return m_bShow;
}

void cltNSoulGuardStatus::Show()
{
	CControllerMgr::Show( true );

	m_bShow = true;
}

void cltNSoulGuardStatus::Hide()
{
	CControllerMgr::Show( false );

	m_bShow = false;
}

void cltNSoulGuardStatus::Action()
{
	SoulGuardUpdate();

	if ( IsSoulGuardHouseUpdate() )
	{
		SoulGuardHouseUpdate();
	}
}

void cltNSoulGuardStatus::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	switch( MsgIndex )
	{
	case 40:	// 수호정령 풀어주기
		{
			if ( RetValue )
			{
				// 풀어달라고 요청한다
				cltMsg clMsg( GAMEMSG_REQUEST_SOULGUARD_DEAD, 0, NULL );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;

	case 41:
		{
			if ( RetValue )
			{
				// 보관소 생성을 요청한다
				cltMsg clMsg( GAMEMSG_REQUEST_SOULGUARD_HOUSE_CREATE, 0, NULL  );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}

void cltNSoulGuardStatus::SoulGuardUpdate( void )
{
	cltClient*		pclclient	= (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	SI32			siCharID	= cltCharCommon::CharUniqueInfo[m_siCharUnique];
	cltCharClient*	pclChar		= m_pclCM->GetCharClient( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	SI16 siGetKind = pclChar->pclSoulGuard->GetKind();
	// Kind가 0보다 작거나 같으면 없는 것이다 이럴때는 다 초기화 시켜주고 리턴
	if ( 0 >= siGetKind )
	{
		AllShowControllerAndVariableClear();
		return;
	}

	if ( siGetKind != m_siShowSoulGuardKind )
	{
		// 바뀐 정보를 저장하고
		m_siShowSoulGuardKind = siGetKind;

		// 수호정령 초상정보를 가져온다
		TCHAR	szPortrait[256] = TEXT("");
		SI32	siPortraitFont	= 0;

		// 초상화 설정
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont( m_siShowSoulGuardKind, szPortrait, sizeof(szPortrait), &siPortraitFont );

		m_pclImage_Portrait->SetFileName( szPortrait );
		m_pclImage_Portrait->SetFontIndex( siPortraitFont );

		// Kind가 바뀌면 특수능력도 다시 샛팅해준다
		m_pclEdit_Special->ClearText();

		NTCHARString512	strSpecial = TEXT("");

		SI32 siHaveAbility	= pclClient->m_pclSoulGuardMgr->GetHaveAbility( m_siShowSoulGuardKind );
		SI32 siCheckAbility	= 1;
		SI32 siGetTextNum	= 0;

		for ( SI32 siIndex=1; siIndex<32; siIndex++ )
		{
			if ( siHaveAbility & siCheckAbility )
			{
				siGetTextNum = pclClient->m_pclSoulGuardMgr->GetSoulGuardExplainTextNum( siCheckAbility );
				if ( 0 < siGetTextNum )
				{
					strSpecial += GetTxtFromMgr( siGetTextNum );
					strSpecial += TEXT("\r\n");
				}
			}

			siCheckAbility = siCheckAbility << 1;
		}

		m_pclEdit_Special->SetText( strSpecial );
	}

	// 이름 보여주기
	TCHAR* pszGetName = pclChar->pclSoulGuard->GetName();
	if ( _tcscmp(pszGetName, m_szShowSoulGuardName) )
	{	
		MStrCpy( m_szShowSoulGuardName, pszGetName, MAX_SOULGUARD_NAME );

		m_pclEdit_Name->SetText( pszGetName );
	}

	// 친화력 표시
	SI16 siGetFellowShip	= pclChar->pclSoulGuard->GetFellowShip();
	if ( siGetFellowShip != m_siShowFellowShip )
	{
		// 먼저 설정해 준다
		m_siShowFellowShip	= siGetFellowShip;

		// 친화력 표시하기
		NTCHARString128 strShowFellowShip = TEXT("#now# / #max#");

		strShowFellowShip.Replace( TEXT("#now#"), SI16ToString(m_siShowFellowShip) );
		strShowFellowShip.Replace( TEXT("#max#"), SI32ToString(Const_Max_SoulGuard_FellowShip) );

		m_pclStatic_FellowShip->SetText( strShowFellowShip );
	}

	// 기본 능력 표시하기
	if ( pclClient->pclKindInfoSet->IsValidKind(m_siShowSoulGuardKind) )
	{
		NTCHARString512	strBasic	= TEXT("");
		cltKindInfo*	pclGetKI	= pclClient->pclKindInfoSet->pclKI[m_siShowSoulGuardKind];
		if ( pclGetKI )
		{
			NTCHARString64 strValue	= TEXT(" + #now# (#max#)");

			// 근력(실제 능력이 있을때만 나오게 한다)
			if ( 0 < pclGetKI->clBA.GetStr() )
			{
				NTCHARString64 strStr	= GetTxtFromMgr(4369);

				strStr += strValue;
				strStr.Replace( TEXT("#now#"), SI32ToString(pclChar->clPB.clSoulGuardAdd.GetStr()) );
				strStr.Replace( TEXT("#max#"), SI32ToString(pclGetKI->clBA.GetStr()) );

				strBasic += strStr;
				strBasic += TEXT("\r\n");
			}

			// 손재주(실제 능력이 있을때만 나오게 한다)
			if ( 0 < pclGetKI->clBA.GetHnd() )
			{
				NTCHARString64 strHand	= GetTxtFromMgr(4373);

				strHand += strValue;
				strHand.Replace( TEXT("#now#"), SI32ToString(pclChar->clPB.clSoulGuardAdd.GetHnd()) );
				strHand.Replace( TEXT("#max#"), SI32ToString(pclGetKI->clBA.GetHnd()) );

				strBasic += strHand;
				strBasic += TEXT("\r\n");
			}

			// 순발력(실제 능력이 있을때만 나오게 한다)
			if ( 0 < pclGetKI->clBA.GetDex() )
			{
				NTCHARString64 strDex	= GetTxtFromMgr(4370);

				strDex += strValue;
				strDex.Replace( TEXT("#now#"), SI32ToString(pclChar->clPB.clSoulGuardAdd.GetDex()) );
				strDex.Replace( TEXT("#max#"), SI32ToString(pclGetKI->clBA.GetDex()) );

				strBasic += strDex;
				strBasic += TEXT("\r\n");
			}

			// 지력(실제 능력이 있을때만 나오게 한다)
			if ( 0 < pclGetKI->clBA.GetMag() )
			{
				NTCHARString64 strMag	= GetTxtFromMgr(4371);

				strMag += strValue;
				strMag.Replace( TEXT("#now#"), SI32ToString(pclChar->clPB.clSoulGuardAdd.GetMag()) );
				strMag.Replace( TEXT("#max#"), SI32ToString(pclGetKI->clBA.GetMag()) );

				strBasic += strMag;
				strBasic += TEXT("\r\n");
			}

			// 지혜(실제 능력이 있을때만 나오게 한다)
			if ( 0 < pclGetKI->clBA.GetWis() )
			{
				NTCHARString64 strWis	= GetTxtFromMgr(10333);

				strWis += strValue;
				strWis.Replace( TEXT("#now#"), SI32ToString(pclChar->clPB.clSoulGuardAdd.GetWis()) );
				strWis.Replace( TEXT("#max#"), SI32ToString(pclGetKI->clBA.GetWis()) );

				strBasic += strWis;
				strBasic += TEXT("\r\n");
			}

			// 생명령(실제 능력이 있을때만 나오게 한다)
			if ( 0 < pclGetKI->clBA.GetVit() )
			{
				NTCHARString64 strVital	= GetTxtFromMgr(4368);

				strVital += strValue;
				strVital.Replace( TEXT("#now#"), SI32ToString(pclChar->clPB.clSoulGuardAdd.GetVit()) );
				strVital.Replace( TEXT("#max#"), SI32ToString(pclGetKI->clBA.GetVit()) );

				strBasic += strVital;
				strBasic += TEXT("\r\n");
			}
			// 펌킨 추가 능령 표시
			if ( m_siShowSoulGuardKind == pclClient->GetUniqueFromHash( "KIND_SOULGUARD_PUMPKIN" )) 
			{
				NTCHARString64 strSpecialComent	= GetTxtFromMgr(40164);
				strBasic += strSpecialComent;
				strBasic += TEXT("\r\n");
			}
			// 캔들 추가 능력 표시
			if( m_siShowSoulGuardKind == pclClient->GetUniqueFromHash( "KIND_SOULGUARD_FIRESPIRIT" ))
			{
				NTCHARString64 strSpecialComent	= GetTxtFromMgr(30289);
				strBasic += strSpecialComent;
				strBasic += TEXT("\r\n");
			}
			// 얼음정령 추가 능력 표시
			if( m_siShowSoulGuardKind == pclClient->GetUniqueFromHash( "KIND_SOULGUARD_ICESPIRIT" ))
			{
				NTCHARString64 strSpecialComent	= GetTxtFromMgr(40274);
				strBasic += strSpecialComent;
				strBasic += TEXT("\r\n");
			}			
		}

		m_pclEdit_Basic->SetText( strBasic );
	}
	else
	{
		m_pclEdit_Basic->ClearText();
	}

	// 수명
	NTCHARString64	strShowLife		= TEXT("");
	SI32			siGetLimitVery	= pclChar->pclSoulGuard->GetLimitDateVary();

	// 무한대 수명일 경우
	if ( MAX_SI32 == siGetLimitVery )
	{
		strShowLife = GetTxtFromMgr(1287);
	}
	else
	{
		SI32 siShowLife = siGetLimitVery-pclclient->clClientDate.GetDateVary();
		if ( 0 > siShowLife )
		{
			siShowLife = 0;
		}

		strShowLife.FormatString( TEXT("%d %s"), siShowLife, GetTxtFromMgr(4084) );
	}

	m_pclStatic_Life->SetText( strShowLife );
		
}

void cltNSoulGuardStatus::SoulGuardHouseUpdate( void )
{
	SI32			siCharID	= cltCharCommon::CharUniqueInfo[m_siCharUnique];
	cltCharClient*	pclChar		= m_pclCM->GetCharClient( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	SI16 siKindUnique		= 0;
	SI32 siFontIndex		= 0;
	SI32 siMaxHaveHouseNum	= pclChar->pclSoulGuard->m_clHouse.GetMaxHaveHouseNum();

	for ( SI32 siIndex=0; siIndex<MAX_SOULGUARD_HOUSE_NUM; siIndex++ )
	{
		// 유효 보관소이기 때문에 갖고 있는 수호정령을 표시해 준다
		if ( siIndex < siMaxHaveHouseNum )
		{
			siKindUnique	= pclChar->pclSoulGuard->m_clHouse.GetKindUniqueFromIndex( siIndex );
			siFontIndex		= GetSoulGuardPortraitFontNum( siKindUnique );
			if ( 0 <= siFontIndex )
			{
				m_pclImage_House[siIndex]->SetFileName( TEXT("Interface\\Pannel\\IMAGE_33x30_00_000.SPR") );
				m_pclImage_House[siIndex]->SetFontIndex( siFontIndex );
				m_pclImage_House[siIndex]->Enable( true );
			}
			else
			{
				m_pclImage_House[siIndex]->SetFileName( TEXT("") );
				m_pclImage_House[siIndex]->Enable( false );
			}
		}
		else	// 사용할 수 없다는 표시를 해준다
		{
			m_pclImage_House[siIndex]->SetFileName( TEXT("Interface\\Pannel\\IMAGE_32x30_00_000.SPR") );
			m_pclImage_House[siIndex]->SetFontIndex( 1 );
		}
	}
	
	
}

void cltNSoulGuardStatus::AllShowControllerAndVariableClear( void )
{
	// Controller
	m_pclImage_Portrait->SetFileName( TEXT("") );
	m_pclImage_Portrait->SetFontIndex( 0 );

	m_pclStatic_FellowShip->SetText( TEXT("") );
	m_pclStatic_Life->SetText( TEXT("") );

	m_pclEdit_Name->ClearText();
	m_pclEdit_Basic->ClearText();
	m_pclEdit_Special->ClearText();

	// Variable
	m_siShowSoulGuardKind	= 0;
	m_siShowStatus			= 0;
	m_siShowFellowShip		= 0;

	ZeroMemory( m_szShowSoulGuardName, sizeof(m_szShowSoulGuardName) );
}

SI32 cltNSoulGuardStatus::GetSoulGuardPortraitFontNum( SI16 siKindUnique )
{
	if ( false == pclClient->pclKindInfoSet->IsValidKind(siKindUnique) )
	{
		return -1;
	}

	cltKindInfo* pclGetInfo = pclClient->pclKindInfoSet->pclKI[siKindUnique];
	if ( NULL == pclGetInfo )
	{
		return -1;
	}

	return (pclGetInfo->siSummonPortrait * 2);
}

void cltNSoulGuardStatus::SetSoulGuardHouseBackFontIndex( SI32 siSelectIndex )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	for ( SI32 siIndex=0; siIndex<MAX_SOULGUARD_HOUSE_NUM; siIndex++ )
	{
		m_pclImage_Back[siIndex]->SetFontIndex( 1 );
	}

	if ( (0 <= siSelectIndex) && (MAX_SOULGUARD_HOUSE_NUM > siSelectIndex) )
	{
		m_pclImage_Back[siSelectIndex]->SetFontIndex( 0 );

		if ( NULL == pclclient->m_pDialog[CLTNSOULGUARDHOUSESTATUS_DLG] )
		{
			pclclient->CreateInterface( CLTNSOULGUARDHOUSESTATUS_DLG );
		}

		((cltNSoulGuardHouseStatus*)pclclient->m_pDialog[CLTNSOULGUARDHOUSESTATUS_DLG])->SetCurrentSelectIndex( siSelectIndex );
	}
	else
	{
		if ( pclclient->m_pDialog[CLTNSOULGUARDHOUSESTATUS_DLG] )
		{
			pclclient->DestroyInterface( pclclient->m_pDialog[CLTNSOULGUARDHOUSESTATUS_DLG] );
		}
	}
}

void cltNSoulGuardStatus::OnButtonClickNameChange( void )
{
	cltClient*		pclclient	= (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	SI32			siCharID	= cltCharCommon::CharUniqueInfo[m_siCharUnique];
	cltCharClient*	pclChar		= m_pclCM->GetCharClient( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	// 캐릭터가 죽어있으면 실패
	if ( FALSE == pclclient->pclCM->IsAlive(siCharID) )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 수호 정령을 갖고 있지 않다면
	if ( TRUE == pclChar->pclSoulGuard->IsEmpty() )
	{
		return;
	}

	// 이름을 가져온다
	TCHAR	szOriginalName[MAX_SOULGUARD_NAME]	= TEXT("");
	TCHAR	szChangeName[MAX_SOULGUARD_NAME]	= TEXT("");

	StringCchCopy( szOriginalName,	MAX_SOULGUARD_NAME,	pclChar->pclSoulGuard->GetName() );
	StringCchCopy( szChangeName,	MAX_SOULGUARD_NAME,	m_pclEdit_Name->GetText() );
	
	// 글자 길이 가져오기
	size_t sizeLength = 0;
	StringCchLength( szChangeName, MAX_SOULGUARD_NAME, &sizeLength );

	// 문자열의 길이가 0일때
	if ( 0 == sizeLength )
	{
		NTCHARString32	strTitle	= GetTxtFromMgr( 30146 );
		NTCHARString128	strText		= GetTxtFromMgr( 2039 );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		return;
	}

	// 기존 이름과 동일할때
	if ( 0 == _tcscmp(szOriginalName, szChangeName) )
	{
		NTCHARString32	strTitle	= GetTxtFromMgr( 30146 );
		NTCHARString128	strText		= GetTxtFromMgr( 2039 );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		return;
	}

	// 욕설필터링 적용
	pclclient->m_pCAbuseFilter->CheckAbuse( szChangeName );

	// 유효한 이름인지 검사한다
	if ( FALSE == IsCorrectID(szChangeName) )
	{
		NTCHARString32	strTitle	= GetTxtFromMgr( 30146 );
		NTCHARString128	strText		= GetTxtFromMgr( 2039 );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		return;
	}

	// 이름 변경 요청한다
	cltGameMsgRequest_SoulGuardChangeName clInfo( pclChar->pclSoulGuard->GetSoulGuardID(), szChangeName );
	cltMsg clMsg( GAMEMSG_REQUEST_SOULGUARD_CHANGENAME, sizeof(clInfo), (BYTE*)&clInfo );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

}

void cltNSoulGuardStatus::OnButtonClickInOut( void )
{
	cltClient*		pclclient	= (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	SI32			siCharID	= cltCharCommon::CharUniqueInfo[m_siCharUnique];
	cltCharClient*	pclChar		= m_pclCM->GetCharClient( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	// 캐릭터가 죽어있으면 실패
	if ( FALSE == pclclient->pclCM->IsAlive(siCharID) )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 수호 정령을 갖고 있지 않다면
	if ( TRUE == pclChar->pclSoulGuard->IsEmpty() )
	{
		return;
	}

	SI32 siGetSoulGuardID	= pclChar->pclSoulGuard->GetSoulGuardID();
	SI32 siGetStatus		= pclChar->pclSoulGuard->GetStatus();

	if ( SOULGUARD_STATUS_OUT == siGetStatus )	// 소환되어 있다면 회수하라고 보내고
	{
		cltGameMsgRequest_SoulGuardStatusInOut clInfo( siGetSoulGuardID, SOULGUARD_STATUS_IN );
		cltMsg clMsg( GAMEMSG_REQUEST_SOULGUARD_STATUSINOUT, sizeof(clInfo), (BYTE*)&clInfo );
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
	else if ( SOULGUARD_STATUS_IN == siGetStatus )	// 회수된 상태라면 소환하라고 보낸다
	{
		cltGameMsgRequest_SoulGuardStatusInOut clInfo( siGetSoulGuardID, SOULGUARD_STATUS_OUT );
		cltMsg clMsg( GAMEMSG_REQUEST_SOULGUARD_STATUSINOUT, sizeof(clInfo), (BYTE*)&clInfo );
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

}

void cltNSoulGuardStatus::OnButtonClickFree( void )
{
	cltClient*		pclclient	= (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	SI32			siCharID	= cltCharCommon::CharUniqueInfo[m_siCharUnique];
	cltCharClient*	pclChar		= m_pclCM->GetCharClient( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	// 캐릭터가 죽어있으면 실패
	if ( FALSE == pclclient->pclCM->IsAlive(siCharID) )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 수호 정령을 갖고 있지 않다면
	if ( TRUE == pclChar->pclSoulGuard->IsEmpty() )
	{
		return;
	}

	CControllerMgr* pclParentDlg = GetParentDialog();
	if ( NULL == pclParentDlg )
	{
		return;
	}

	// 우선 수호정령 ID를 설정해 주고
	m_siFreeSoulGuardID = pclChar->pclSoulGuard->GetSoulGuardID();

	// 메시지 박스 띄우기
	NTCHARString32	strTitle	= GetTxtFromMgr( 30146 );
	NTCHARString128	strText		= GetTxtFromMgr( 30153 );

	stMsgBox MsgBox;
	MsgBox.Set( pclParentDlg, strTitle, strText, MSGBOX_TYPE_YESNO, 40 );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

}

BOOL cltNSoulGuardStatus::IsSoulGuardHouseUpdate( void )
{
	SI32			siCharID	= cltCharCommon::CharUniqueInfo[m_siCharUnique];
	cltCharClient*	pclChar		= m_pclCM->GetCharClient( siCharID );
	if ( NULL == pclChar )
	{
		return FALSE;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return FALSE;
	}

	SI32	siIndex	= 0;
	BOOL	bCheck	= FALSE;

	// 수호정령 보관소 카인드가 변경 되었을때
	for ( siIndex=0; siIndex<MAX_SOULGUARD_HOUSE_NUM; siIndex++ )
	{
		if ( pclChar->pclSoulGuard->m_clHouse.GetKindUniqueFromIndex(siIndex) != m_siCheckSoulGuardHouse[siIndex] )
		{
			bCheck = TRUE;
			break;
		}
	}

	if ( TRUE == bCheck )
	{
		for ( siIndex=0; siIndex<MAX_SOULGUARD_HOUSE_NUM; siIndex++ )
		{
			m_siCheckSoulGuardHouse[siIndex] = pclChar->pclSoulGuard->m_clHouse.GetKindUniqueFromIndex(siIndex);
		}
	}

	// 수호정령 보관소 갯수가 변경 되었을때
	if ( pclChar->pclSoulGuard->m_clHouse.GetMaxHaveHouseNum() != m_siShowHouseNum )
	{
		m_siShowHouseNum = pclChar->pclSoulGuard->m_clHouse.GetMaxHaveHouseNum();

		bCheck = TRUE;
	}

	return bCheck;
}

void cltNSoulGuardStatus::OnButtonClickHouse( SI32 siControllerID )
{
	SI32 siRealIndex = siControllerID - CLTNSOULGUARDSTATUS_DLG_BUTTON_HOUSE1;
	if ( (0 > siRealIndex) || (MAX_SOULGUARD_HOUSE_NUM <= siRealIndex) )
	{
		return;
	}

	SI32			siCharID	= cltCharCommon::CharUniqueInfo[m_siCharUnique];
	cltCharClient*	pclChar		= m_pclCM->GetCharClient( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	if ( pclChar->pclSoulGuard->m_clHouse.GetMaxHaveHouseNum() > siRealIndex )
	{
		SetSoulGuardHouseBackFontIndex( siRealIndex );
	}
	else
	{
		SetSoulGuardHouseBackFontIndex( -1 );
	}
}

void cltNSoulGuardStatus::OnButtonClickHouseCreate()
{
	cltClient*		pclclient	= (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	SI32			siCharID	= cltCharCommon::CharUniqueInfo[m_siCharUnique];
	cltCharClient*	pclChar		= m_pclCM->GetCharClient( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	// 캐릭터가 죽어있으면 실패
	if ( FALSE == pclclient->pclCM->IsAlive(siCharID) )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	// 이미 최대 갯수의 보관소를 갖고 있다면
	if ( MAX_SOULGUARD_HOUSE_NUM <= pclChar->pclSoulGuard->m_clHouse.GetMaxHaveHouseNum() )
	{
		NTCHARString64	strTitle( GetTxtFromMgr(30146) );
		NTCHARString256 strText( GetTxtFromMgr(1089) );

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );

		return;
	}

	// 보관소 아이템을 갖고 있지 않다면
	if ( Const_SoulGuardHouse_ItemNum > pclChar->pclCI->clCharItem.GetItemNum(Const_SoulGuardHouse_ItemUnique) )
	{
		TCHAR* pszItemName = (TCHAR*)pclclient->pclItemManager->GetName( Const_SoulGuardHouse_ItemUnique );
		if ( pszItemName )
		{
			NTCHARString64	strTitle( GetTxtFromMgr(30146) );
			NTCHARString256 strText( GetTxtFromMgr(9986) );

			strText.Replace( TEXT("#itemname#"), pszItemName );

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}

		return;
	}

	CControllerMgr* pclParentDlg = GetParentDialog();
	if ( NULL == pclParentDlg )
	{
		return;
	}

	// 메시지 박스 띄우기
	NTCHARString32	strTitle	= GetTxtFromMgr( 30146 );
	NTCHARString128	strText		= GetTxtFromMgr( 30247 );

	stMsgBox MsgBox;
	MsgBox.Set( pclParentDlg, strTitle, strText, MSGBOX_TYPE_YESNO, 41 );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

}