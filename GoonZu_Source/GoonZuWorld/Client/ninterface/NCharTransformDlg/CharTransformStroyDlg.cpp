#include "../Client/ninterface/NCharTransformDlg/CharTransformStoryDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "Char\CharManager\CharManager.h"


extern cltCommonLogic* pclClient;



CNChartransformStroyDlg::CNChartransformStroyDlg()
{	 
	m_siMouseOverIndex = -1;
}

CNChartransformStroyDlg::~CNChartransformStroyDlg()
{
	Destroy();
}



void CNChartransformStroyDlg::Init()
{

}

void CNChartransformStroyDlg::Destroy()
{

}

void CNChartransformStroyDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/CharTransformDlg/TransformCharStoryDlg.ddf"));


	file.CreatePopupDialog( this, CHARTRANSFORMINFO_STORY_DLG, TEXT("dialog_CharTransformStory"), StaticChartransformStroyDlgProc);


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,	NCHARTRANSFORMSTORY_STORY_LIST, this ), NCHARTRANSFORMSTORY_STORY_LIST,		"listbox_transform_char_story");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORMSTORY_BTN_READ,	this ), NCHARTRANSFORMSTORY_BTN_READ,		"button_read");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORMSTORY_BTN_CLOSE,	this ), NCHARTRANSFORMSTORY_BTN_CLOSE,		"button_close");

		
	CList* pList = m_InterfaceFactory.GetList(NCHARTRANSFORMSTORY_STORY_LIST);
	if ( pList )
	{
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr( 601 );
		pList->SetColumn(0, 200, "力格");
		pList->SetColumn(1,  92, "鞘夸 教农肺啦");
		
		stListBoxItemData	tempItemData;
		TCHAR				rate[256];
		for ( SI32 i = 1; i < 23; ++i )
		{
			pText = GetTxtFromMgr( i );
			tempItemData.Set(0, pText);
			
			itoa(i, rate, 10);
			tempItemData.Set(1, rate);
			
			pList->AddItem( &tempItemData );
		}
		
		pList->Refresh();
	}
}

void CALLBACK CNChartransformStroyDlg::StaticChartransformStroyDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNChartransformStroyDlg *pThis = (CNChartransformStroyDlg*) pControllerMgr;
	pThis->NChartransformStroyDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNChartransformStroyDlg::NChartransformStroyDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
		case NCHARTRANSFORMSTORY_STORY_LIST:
			{
				if ( EVENT_LISTBOX_SELECTION == nEvent )
				{
					CList* pList	   = m_InterfaceFactory.GetList(NCHARTRANSFORMSTORY_STORY_LIST);
					m_siMouseOverIndex = pList->GetMouseOverIndex();
				}
			}
			break;
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;	
		case NCHARTRANSFORMSTORY_BTN_READ:
			{
				CList* pList = m_InterfaceFactory.GetList(NCHARTRANSFORMSTORY_STORY_LIST);
				
				stListBoxItemData* pItemData = NULL;
				pItemData = pList->GetItemData( m_siMouseOverIndex);
				
				if(NULL != pItemData)
				{
					SI32 rate = atoi(pItemData->strText[1]);
					
					if(rate > 5)
					{
						pclclient->CreateInterface(CHARTRANSFORMINFO_STORY_READ_DLG);
					}
				}
			}
			break;
		case NCHARTRANSFORMSTORY_BTN_CLOSE:
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


void CNChartransformStroyDlg::Action()
{
	if(IsShow())
	{		


	}
}