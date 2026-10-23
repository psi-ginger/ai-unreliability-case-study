
// MakeItemDlg.cpp: implementation of the CNMakeItemDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "NMakeItemDlg.h"

#include "..\..\..\Common\Skill\Skill-Manager.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/Static.h"


#include "../../CommonLogic/MsgType-System.h"
#include "../../StatusBar/ChatStatusBar/ChatStatusBarBaseInfo.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#define MAKEITEM_SHOWTIME_FRAME	10

extern cltCommonLogic* pclClient;

enum
{
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNMakeItemDlg::CNMakeItemDlg()
{
	m_pNMakeitemImage = NULL;
	m_pNMakeitemStatic = NULL;

	m_siCurrentFrame = 0;
	SetTimerOnOff(true);
}

CNMakeItemDlg::~CNMakeItemDlg()
{
	SetTimerOnOff(false);

	SAFE_DELETE(m_pNMakeitemImage);
	SAFE_DELETE(m_pNMakeitemStatic);	
}



void CNMakeItemDlg::Create( )
{
	TCHAR* pText= GetTxtFromMgr(3359);

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/DLG_NMakeItem.ddf"));
	file.CreatePopupDialog(this, NMAKEITEM_DLG, TEXT("dialog_Makeitemdlg"), StatiCNMakeItemDlgProc);

	m_pNMakeitemImage = new CImageStatic( this );             
	m_pNMakeitemStatic = new CStatic( this );      

	file.CreateControl(m_pNMakeitemImage, NMAKEITEM_DIALOG_IMAGE, TEXT("imagestatic_MakeItem"));
	file.CreateControl(m_pNMakeitemStatic,NMAKEITEM_DIALOG_STATIC , TEXT("statictext_MakeItem"));

}

void CALLBACK CNMakeItemDlg::StatiCNMakeItemDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMakeItemDlg *pThis = (CNMakeItemDlg*) pControllerMgr;
	pThis->NMakeItemDlgProc( nEvent, nControlID, pControl );
}



void CALLBACK CNMakeItemDlg::NMakeItemDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
	return ;
}



void CNMakeItemDlg::Set( cltItem *pclItem, SI32 baseexp, SI32 skillunique,SI32 skillexp,SI32 monstercorp)
{
	TCHAR buffer[1024] = TEXT("") ;
	TCHAR itemname[128] = TEXT("");
	TCHAR skillname[128] = TEXT("");

	cltClient *pclclient = (cltClient *)pclClient;	
		
	SI32 itemunique= pclItem->siUnique;


	if(itemunique)
	{
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) )
		{}
		else return;
	}

	//switch( m_siImageType )
	//{
	//case GIMG_ITEMIMG0 : m_pNMakeitemImage->SetFileName(TEXT("GImg/ItemImg0.SPR")); break;
	//case GIMG_ITEMIMG1 : m_pNMakeitemImage->SetFileName(TEXT("GImg/ItemImg1.SPR")); break;
	//case GIMG_ITEMIMG2 : m_pNMakeitemImage->SetFileName(TEXT("GImg/ItemImg2.SPR")); break;
	//case GIMG_ITEMIMG3 : m_pNMakeitemImage->SetFileName(TEXT("GImg/ItemImg3.SPR")); break;
	//}
	m_pNMakeitemImage->SetItemSprType(m_siImageType);
	m_pNMakeitemImage->SetFontIndex(m_siImageIndex);

	//if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) ){}

	//	
	//if(m_siImageType==105)
	//{	
	//	m_pNMakeitemImage->SetFileName(TEXT("GImg/ItemImg0.SPR"));
	//	m_pNMakeitemImage->SetFontIndex(((SI16)m_siImageIndex));
	//}
	//	
	//else if(m_siImageType==106)
	//{
	//	m_pNMakeitemImage->SetFileName(TEXT("GImg/ItemImg1.SPR"));
	//	m_pNMakeitemImage->SetFontIndex(((SI16)m_siImageIndex));
	//
	//}
	//
	//else if(m_siImageType==107)
	//{
	//	m_pNMakeitemImage->SetFileName(TEXT("GImg/ItemImg2.SPR"));
	//	m_pNMakeitemImage->SetFontIndex(((SI16)m_siImageIndex));	
	//}


	float fMonsterCorp = max(-1000.0f,min(1000.0f,(float)monstercorp / 1000.0f)) ;

	if ( pclItem->GetFullName(pclClient->pclItemManager,itemname, 128) == true)
	{
		TCHAR* pText = GetTxtFromMgr(1948);
		StringCchPrintf(buffer, 1024, pText,itemname,pclItem->GetItemNum(),baseexp,fMonsterCorp);
	}
	else return ;

	if(pclClient->pclSkillManager->GetSkillName(skillunique,skillname) == true )
	{
		TCHAR temp[256];
		TCHAR* pText = GetTxtFromMgr(1949);
		StringCchPrintf(temp, 256, pText,skillname,skillexp);
		StringCchCat(buffer, 1024, temp);
	}

	m_pNMakeitemStatic->SetBorder(true);
	m_pNMakeitemStatic->SetBKColorUse(true);
	m_pNMakeitemStatic->SetText(buffer,DT_WORDBREAK);

}

void CNMakeItemDlg::Action()
{
	// 자동으로 사라지게 하는 코드
	if( IsShow() )
	{
		if ( m_siCurrentFrame++ > MAKEITEM_SHOWTIME_FRAME )
		{
			DeleteDialog();
		}
	}

	return;
}
