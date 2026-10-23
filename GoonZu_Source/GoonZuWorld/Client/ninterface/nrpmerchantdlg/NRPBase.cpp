#include "../../../Client/Client.h"
#include "../../../CommonLogic/Msg/MsgType-NPC.h"
#include "../../../Common/SystemNPC/SystemNPC.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "NRPBase.h"
#include "NRPMerchant.h"
#include "NRPList.h"
#include "../../../common/Char/CharCommon/Char-Common.h"
#include "../../../common/Char/CharManager/CharManager.h"

#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/Msg/MsgType-Quest.h"

#include "../NPromoter/NPromoter_InfoDlg.h"
#include "../NPromoter/NPromoter_UrlDlg.h"

extern cltCommonLogic* pclClient;

CNRPBase::CNRPBase()
{
	Initialize();
}	

CNRPBase::~CNRPBase()
{
	Destroy();
}

void CNRPBase::Initialize()
{
	RPListTab			= NULL;
	RPMerchantTab		= NULL;
	m_NPromoter_InfoDlg = NULL;
	m_NPromoter_UrlDlg	= NULL;
	
	m_siRecommandPoint	= 0;
}

void CNRPBase::Destroy()
{
	SAFE_DELETE(RPListTab);
	SAFE_DELETE(RPMerchantTab);
	SAFE_DELETE(m_NPromoter_InfoDlg);
	SAFE_DELETE(m_NPromoter_UrlDlg);
}

void CNRPBase::Create()
{

	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

#ifdef USE_PROMOTOR_CLIENT
			file.LoadFile(TEXT("NInterface/Data/NRPMerchant/DLG_RPBase_China.ddf"));
#else
			file.LoadFile(TEXT("NInterface/Data/NRPMerchant/DLG_RPBase.ddf"));
#endif
		
		file.CreatePopupDialog( this, NRPBASE_DLG, TEXT("dialog_RPbase"), StatiCNRPBaseDlgProc);		

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NRPBASE_STATIC_POTRAIT ,			this)  , NRPBASE_STATIC_POTRAIT ,		TEXT("statictext_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NRPBASE_STATIC_POINT ,				this)  , NRPBASE_STATIC_POINT ,			TEXT("statictext_NONAME2"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NRPBASE_EDIT_GREETING ,				this)  , NRPBASE_EDIT_GREETING ,		TEXT("editbox_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NRPBASE_EDIT_MYPOINT ,				this)  , NRPBASE_EDIT_MYPOINT ,			TEXT("editbox_NONAME2"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NRPBASE_BUTTON_LIST ,				this)  , NRPBASE_BUTTON_LIST ,			TEXT("button_List"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NRPBASE_BUTTON_MERCHANT ,			this)  , NRPBASE_BUTTON_MERCHANT ,		TEXT("button_Merchant"));

#ifdef USE_PROMOTOR_CLIENT		
		
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NRPBASE_BUTTON_RECOMMENDEE_URL,		this), NRPBASE_BUTTON_RECOMMENDEE_URL,	TEXT("button_recommendee_url"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NRPBASE_BUTTON_RECOMMENDEE_INFO,	this), NRPBASE_BUTTON_RECOMMENDEE_INFO,	TEXT("button_recommendee_info"));

#endif

		TCHAR * pText = GetTxtFromMgr(6241);
		m_InterfaceFactory.GetEdit(NRPBASE_EDIT_GREETING)->SetText(pText);

		cltMsg clMsg2( GAMEMSG_REQUEST_GETRECOMMANDPOINT,0,NULL );
		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg2 );

		RPListTab		= new	CNRPList;
		RPMerchantTab	= new	CNRPMerchant;

		RPListTab->Create(this);
		RPMerchantTab->Create(this);
		RPMerchantTab->Hide();
		RPListTab->Show();

#ifdef USE_PROMOTOR_CLIENT
		m_NPromoter_InfoDlg = new NPromoter_InfoDlg;
		m_NPromoter_UrlDlg  = new NPromoter_UrlDlg;
		m_NPromoter_InfoDlg->Create(this);
		m_NPromoter_UrlDlg->Create(this);

		RPMerchantTab->Hide();
		RPListTab->Hide();
		m_NPromoter_InfoDlg->Hide();
		m_NPromoter_UrlDlg->Show();
#endif
		
		// 초상설정
		m_InterfaceFactory.GetStatic(NRPBASE_STATIC_POTRAIT)->SetFontIndex(pclClient->GetUniqueFromHash(TEXT("KIND_RECOMMENDER"))-1);

		SI32 charkind = pclClient->GetUniqueFromHash(TEXT("KIND_RECOMMENDER"));

		// 추천인NPC용 창에 퀘스트 클리어 조건 추가 - by LEEKH 2007.11.20
		SI16 questtype, para1, para2;
		SI16 questunique;
		SI16 questclearcond =  QUEST_CLEAR_COND_GONPC;
		if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			if(para1 == charkind )
			{
				cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
				cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}

	}
	else
	{
		DeleteDialog();
	}
}

void CALLBACK CNRPBase::StatiCNRPBaseDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNRPBase *pThis = (CNRPBase*) pControllerMgr;
	pThis->NRPBaseDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNRPBase::NRPBaseDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	
	case NRPBASE_BUTTON_MERCHANT:
		{		
#ifdef USE_PROMOTOR_CLIENT
			RPListTab->Hide();
			RPMerchantTab->Show();
			m_NPromoter_InfoDlg->Hide();
			m_NPromoter_UrlDlg->Hide();
#else			
			if(RPMerchantTab->IsShow())
				break;
			if(RPListTab->IsShow())
				RPListTab->Hide();
			RPMerchantTab->Show();			
#endif
		}
		break;

	case NRPBASE_BUTTON_LIST:
		{	
#ifdef USE_PROMOTOR_CLIENT
			RPListTab->Show();
			RPMerchantTab->Hide();
			m_NPromoter_InfoDlg->Hide();
			m_NPromoter_UrlDlg->Hide();
#else
			if(RPListTab->IsShow())
				break;
			if(RPMerchantTab->IsShow())
				RPMerchantTab->Hide();
			RPListTab->Show();				
#endif
		}
		break;

	case NRPBASE_BUTTON_RECOMMENDEE_URL:
		{	
			RPListTab->Hide();
			RPMerchantTab->Hide();

#ifdef USE_PROMOTOR_CLIENT
			m_NPromoter_InfoDlg->Hide();
			m_NPromoter_UrlDlg->Show();
#endif
		}
		break;

	case NRPBASE_BUTTON_RECOMMENDEE_INFO:
		{	
			RPListTab->Hide();
			RPMerchantTab->Hide();
#ifdef USE_PROMOTOR_CLIENT
			m_NPromoter_InfoDlg->Show();
			m_NPromoter_UrlDlg->Hide();
#endif
	}
		break;
	}
}

void CNRPBase::SetRecommandPoint( SI32 RecommandPoint)
{

	m_siRecommandPoint = RecommandPoint;

	TCHAR strBuffer[256];

	StringCchPrintf( strBuffer , 256, " %d ",RecommandPoint );

	m_InterfaceFactory.GetEdit(NRPBASE_EDIT_MYPOINT)->SetText(strBuffer);
	m_InterfaceFactory.GetEdit(NRPBASE_EDIT_MYPOINT)->Refresh();

}

void CNRPBase::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{
    RPMerchantTab->VDirectDrawToBackScreen(lpBackScreen, ScreenRX, ScreenRY);
	
#ifdef USE_PROMOTOR_CLIENT
	m_NPromoter_InfoDlg->VDirectDrawToBackScreen(lpBackScreen, ScreenRX, ScreenRY);
#endif
}


SI32 CNRPBase::GetRecommandPoint()
{
	return m_siRecommandPoint;	
}

void CNRPBase::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
		
	switch( MsgIndex )
	{
	case 0:
		{
			RPMerchantTab->RetMsgBox(RetValue,MsgIndex,pData,DataLen);
		}
		break;

	}

}

