#include "client.h"
#include "main.h"
#include "InterfaceFile.h"

#include "Interface/Static.h"
#include "Interface/Edit.h"
#include "Interface/List.h"
#include "Interface/ListView.h"
#include "Interface/Button.h"
#include "Interface/ComboBox.h"
#include "Interface/ImageStatic.h"
#include "Interface/CheckBox.h"
#include "Interface/RadioButton.h"
#include "Interface/OutLine.h"
#include "Interface/ChatEdit.h"
#include "DialogController.h"


#include "commonlogic.h"

#include "InterfaceMgr.h"
#include "InterfaceImgMgr/InterfaceImgMgr.h"

#include "DLGPositionSave/DLGPositionSave.h"

extern cltCommonLogic* pclClient;

#define MAX_NCONTROL_NUMBER		12

enum {
	NCONTROL_DIALOG = 0,
	NCONTROL_BUTTON,
	NCONTROL_COMBOBOX,
	NCONTROL_EDITBOX,
	NCONTROL_STATICTEXT,
	NCONTROL_IMAGESTATIC,
	NCONTROL_LISTBOX,
	NCONTROL_LISTVIEW,
	NCONTROL_CHECKBOX,
	NCONTROL_RADIOBUTTON,
	NCONTROL_OUTLINE,
	NCONTROL_CHATEDIT,
};



TCHAR *szControlKind[] = {
	TEXT("dialog>dialog"),
	TEXT("button>button"),
	TEXT("combobox>combobox"),
	TEXT("editbox>editbox"),
	TEXT("statictext>statictext"),
	TEXT("imagestatic>imagestatic"),
	TEXT("listbox>listbox"),
	TEXT("listview>listview"),
	TEXT("checkbox>checkbox"),
	TEXT("radiobutton>radiobutton"),
	TEXT("outline>outline"),
	TEXT("chatedit>chatedit")
};

// ddf 파일 최신으로 다운로드 필요 여부
bool CInterfaceFile::m_MustDownload = false;
SI32 CInterfaceFile::m_ClientServiceCode = -1;

CInterfaceFile::CInterfaceFile()
{
	m_controlNum = 0;

	ZeroMemory( m_control, sizeof( m_control ) );

	if( m_ClientServiceCode == -1 )
	{
		// 디폴트 서비스 지역을 찾는다. 
		SI32 defaultservicearea = 0;
		if(GetDefaultServiceArea(&defaultservicearea) == false)
		{
		}

		m_ClientServiceCode = defaultservicearea;
	}
}

CInterfaceFile::~CInterfaceFile()
{

}

SI32 CInterfaceFile::GetInfoWithStringID( TCHAR *szID )
{
	SI32 controlindex;

	controlindex = GetControlIndex( szID );

	if( controlindex == -1 ) return -1;

	return controlindex;
}

bool CInterfaceFile::CreatePopupDialog( CDialogController *pDlg, SI16 id, TCHAR *szID, GUICallBackEvent pCallBack, BOOL readPosFromFile )
{
	cltClient *pclclient = (cltClient*)pclClient;

	int controlindex;

	controlindex = GetControlIndex( szID );

	if( controlindex == -1 ) return false;

	if( m_control[ controlindex ].type != NCONTROL_DIALOG ) return false;

	NFControl *pControl = &m_control[ controlindex ];
	NFDialog *pDialog = &m_control[ controlindex ].dialog;
	
	SI16 xPos = g_DlgPositionSave.GetPositionX(id);
	SI16 yPos = g_DlgPositionSave.GetPositionY(id);

	if(g_SoundOnOffDlg.m_bChatFixed == false)
		readPosFromFile = false;
	
	if( g_DlgPositionSave.GetSaveState(id) && id != NITEMMALLMANAGER_DLG && readPosFromFile
		&& pclclient->m_pInterfaceMgr->IsAvailableMove(xPos,yPos,pControl->width,pControl->height)
		)
	{
		pDlg->Create(	id, 
						CONTROLLER_KIND_POPUPDIALOG, 
						NULL, 
						pCallBack,
						pDialog->title,
						pControl->imageFilename,
						pControl->startFont,
						pControl->bImageTile,
						pControl->imageTileType,
						g_DlgPositionSave.GetPositionX(id),// pControl->x,
						g_DlgPositionSave.GetPositionY(id),//pControl->y,
						pControl->width,
						pControl->height,
						pDialog->bMovable,
						pDialog->bTitleVisible,
						pDialog->bClosable		);
	}
	else	// DDf 파일을 통해서만 위치를 알아오는 방식을 피함, 다이얼로그가 제한영역 밖일경우를 대비
	{
		//cyj Dialog 위치가 저장되어있지 않고 .ddf의 좌표가 0,0 일때 80, 135로 변환해줌
		//[영진] Dialog 위치가 0 이하일때 기본 위치로 설정
		int PosX = 0, PosY = 0;

		if (pControl->x <= 0)
			PosX = DEFAULT_DIALOGPOSX;
		else
			PosX = pControl->x;

		if (pControl->y >= 0)
			PosY = DEFAULT_DIALOGPOSY;
		else
			PosY = pControl->y;

		pDlg->Create(	id, 
						CONTROLLER_KIND_POPUPDIALOG, 
						NULL, 
						pCallBack,
						pDialog->title,
						pControl->imageFilename,
						pControl->startFont,
						pControl->bImageTile,
						pControl->imageTileType,
						PosX, //pControl->x,
						PosY, //pControl->y,
						pControl->width,
						pControl->height,
						pDialog->bMovable,
						pDialog->bTitleVisible,
						pDialog->bClosable,
						false					);
	}
				
	SI32 index= _tstoi(pDialog->title);

	if(index>0)
	{
		TCHAR* pTitle = GetTxtFromMgr(index);
		pDlg->SetTitle(pTitle);
	}

	return true;

}

bool CInterfaceFile::CreateChildDialog( CDialogController* pDlg, SI16 id, TCHAR *szID, GUICallBackEvent pCallBack, CControllerMgr *pParent )
{
	int controlindex;

	controlindex = GetControlIndex( szID );

	if( controlindex == -1 ) return false;

	if( m_control[ controlindex ].type != NCONTROL_DIALOG ) return false;

	NFControl *pControl = &m_control[ controlindex ];
	NFDialog *pDialog = &m_control[ controlindex ].dialog;

	pDlg->Create( id, CONTROLLER_KIND_CHILDDIALOG, pParent, pCallBack,
		pDialog->title,
		pControl->imageFilename,
		pControl->startFont,
		pControl->bImageTile,
		pControl->imageTileType,
		pControl->x,
		pControl->y,
		pControl->width,
		pControl->height,
		pDialog->bMovable,
		pDialog->bTitleVisible,
		pDialog->bClosable
		);

	return true;		
}

bool CInterfaceFile::CreateControl( CControllerMgr *pParent, CControllerObj** pControlObj, SI16 id, TCHAR *szID, int index )
{

	int controlindex;

	if( szID ) controlindex = GetControlIndex( szID );
	else controlindex = index;

	if( controlindex < 0 || controlindex >= m_controlNum ) return false;

	switch( m_control[ controlindex ].type ) {

		case NCONTROL_BUTTON:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFButton *pButton = &m_control[ controlindex ].button;

				*pControlObj = new CButton( pParent );

				((CButton*)(*pControlObj))->Create(
					id, 
					pButton->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight 
					);

					SI32 index= _tstoi(pButton->text);

					if(index>0)
					{
						TCHAR* pText = GetTxtFromMgr(index);
						((CButton*)(*pControlObj))->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						
					}
					// 1월 18일 부로 에니메이션 버튼은 사라졌습니다.[영진]
#ifndef _NEW_INTERFACE_
					if( pButton->animation == 0 ) 
						((CButton*)(*pControlObj))->SetAniType(false);
					else if( pButton->animation == 1 )
						((CButton*)(*pControlObj))->SetAniType(true);
					else
						((CButton*)(*pControlObj))->SetAniType(false);
#else
					/*
					if( pButton->animation == 0 ) 
						((CButton*)(*pControlObj))->SetAniType(false);
					else if( pButton->animation == 1 )
						((CButton*)(*pControlObj))->SetAniType(true);
					else
						((CButton*)(*pControlObj))->SetAniType(false);
					*/

#endif

				
			}
			return true;

		case NCONTROL_COMBOBOX:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFComboBox *pComboBox = &m_control[ controlindex ].combobox;

				*pControlObj = new CComboBox( pParent );

				((CComboBox*)(*pControlObj))->Create(
					id, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pComboBox->popupHeight 
					);		
					
			

				
			
			}
			return true;

		case NCONTROL_EDITBOX:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFEditBox *pEditBox = &m_control[ controlindex ].editbox;

				*pControlObj = new CEdit( pParent );

				((CEdit*)(*pControlObj))->Create(
					id, 
					pEditBox->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pEditBox->bMultiLine,
					pEditBox->bReadOnly
					);
			}
			return true;

		case NCONTROL_STATICTEXT:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFStaticText *pStaticText = &m_control[ controlindex ].statictext;

				*pControlObj = new CStatic( pParent );

				((CStatic*)(*pControlObj))->Create(
					id, 
					pStaticText->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight
					);
				
				SI32 index= _tstoi(pStaticText->text);

					if(index>0)
					{
						TCHAR* pText = GetTxtFromMgr(index);
						if ( pclClient->siServiceArea == ConstServiceArea_Japan )
						{
							((CStatic*)(*pControlObj))->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						}
						else
						{
							((CStatic*)(*pControlObj))->SetText(pText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						}
						
					}
			}
			return true;

		case NCONTROL_IMAGESTATIC:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFImageStatic *pImageStatic = &m_control[ controlindex ].imagestatic;

				*pControlObj = new CImageStatic( pParent );

				((CImageStatic*)(*pControlObj))->Create(
					id, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight 
					);
			}
			return true;

		case NCONTROL_LISTBOX:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFListBox *pListBox = &m_control[ controlindex ].listbox;

				*pControlObj = new CList( pParent );

				((CList*)(*pControlObj))->Create(
					id, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pListBox->bMultiLine,
					pListBox->bMultiSelect,
					pListBox->bFixedRow,
					true,
					pListBox->lineHeight
					);
			}
			return true;

		case NCONTROL_LISTVIEW:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFListView *pListView = &m_control[ controlindex ].listview;

				*pControlObj = new CListView( pParent );

				((CListView*)(*pControlObj))->Create(
					id, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pListView->startX,
					pListView->startY,
					pListView->offsetX,
					pListView->offsetY,
					pListView->itemsizeX,
					pListView->itemsizeY,
					pListView->bEmpty,
					pListView->bCountVisible,
					pListView->bUsable,
					pListView->tooltipWidth,
					pListView->tooltipHeight
					);
			}
			return true;

		case NCONTROL_CHECKBOX:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFCheckBox *pCheckBox = &m_control[ controlindex ].checkbox;

				*pControlObj = new CCheckBox( pParent );

				((CCheckBox*)(*pControlObj))->Create(
					id, 
					pCheckBox->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight 
					);
					
				SI32 index= _tstoi(pCheckBox->text);

				if(index>0)
				{
					TCHAR* pText = GetTxtFromMgr(index);
					if( pclClient->siServiceArea == ConstServiceArea_Japan )
					{
						((CCheckBox*)(*pControlObj))->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
					else
					{
						((CCheckBox*)(*pControlObj))->SetText(pText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
					}
				}
			}
			return true;

		case NCONTROL_RADIOBUTTON:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFRadioButton *pRadioButton = &m_control[ controlindex ].radiobutton;

				*pControlObj = new CRadioButton( pParent );

				((CRadioButton*)(*pControlObj))->Create(
					id, 
					pRadioButton->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pRadioButton->groupid
					);

					SI32 index= _tstoi(pRadioButton->text);

				if(index>0)
				{
					TCHAR* pText = GetTxtFromMgr(index);
					if( pclClient->siServiceArea == ConstServiceArea_Japan )
					{
						((CRadioButton*)(*pControlObj))->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
					else
					{
						((CRadioButton*)(*pControlObj))->SetText(pText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
					}
					
				}
			}
			return true;

		case NCONTROL_OUTLINE:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFOutLine *pOutLine = &m_control[ controlindex ].outline;

				*pControlObj = new COutLine( pParent );

				((COutLine*)(*pControlObj))->Create(
					id, 
					pOutLine->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight
					);
			}
			return true;

		case NCONTROL_CHATEDIT:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFChatEdit *pChatEdit = &m_control[ controlindex ].chatedit;

				*pControlObj = new CChatEdit( pParent );

				((CChatEdit*)(*pControlObj))->Create(
					id, 
					pChatEdit->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pChatEdit->lineHeight,
					pChatEdit->bAutoScroll,
					pChatEdit->offsetX,
					pChatEdit->offsetY
					);
			}
			return true;

	}

	return false;

}


bool CInterfaceFile::CreateControl( CControllerObj* pControlObj, SI16 id, TCHAR *szID, int index )
{
	int controlindex;

	if( szID ) controlindex = GetControlIndex( szID );
	else controlindex = index;

	if( controlindex < 0 || controlindex >= m_controlNum ) return false;

	switch( m_control[ controlindex ].type ) {

		case NCONTROL_BUTTON:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFButton *pButton = &m_control[ controlindex ].button;

				((CButton*)(pControlObj))->Create(
					id, 
					pButton->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight 
					);
			
				
					SI32 index= _tstoi(pButton->text);					
					if(index>0)
					{
						TCHAR* pText = GetTxtFromMgr(index);
						((CButton*)(pControlObj))->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				

					
#ifndef _NEW_INTERFACE_
					if( pButton->animation == 0 ) 
						((CButton*)(pControlObj))->SetAniType(false);
					else if( pButton->animation == 1 )
						((CButton*)(pControlObj))->SetAniType(true);
					else
						((CButton*)(pControlObj))->SetAniType(false);
#else
					// 1월 18일 부로 에니메이션 버튼은 사라졌습니다.[영진]
					/*
					if( pButton->animation == 0 ) 
						((CButton*)(pControlObj))->SetAniType(false);
					else if( pButton->animation == 1 )
						((CButton*)(pControlObj))->SetAniType(true);
					else
						((CButton*)(pControlObj))->SetAniType(false);*/

#endif


			}
			return true;

		case NCONTROL_COMBOBOX:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFComboBox *pComboBox = &m_control[ controlindex ].combobox;

				((CComboBox*)(pControlObj))->Create(
					id, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pComboBox->popupHeight 
					);
			}
			return true;

		case NCONTROL_EDITBOX:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFEditBox *pEditBox = &m_control[ controlindex ].editbox;

				((CEdit*)(pControlObj))->Create(
					id, 
					pEditBox->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pEditBox->bMultiLine,
					pEditBox->bReadOnly
					);
			}
			return true;

		case NCONTROL_STATICTEXT:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFStaticText *pStaticText = &m_control[ controlindex ].statictext;

				((CStatic*)(pControlObj))->Create(
					id, 
					pStaticText->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight
					);
						
				
				SI32 index= _tstoi(pStaticText->text);

				if(index>0)
				{
					TCHAR* pText = GetTxtFromMgr(index);
					if( pclClient->siServiceArea == ConstServiceArea_Japan )
						((CStatic*)(pControlObj))->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					else
						((CStatic*)(pControlObj))->SetText(pText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						
				}
			}
			return true;

		case NCONTROL_IMAGESTATIC:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFImageStatic *pImageStatic = &m_control[ controlindex ].imagestatic;

				((CImageStatic*)(pControlObj))->Create(
					id, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight 
					);
			}
			return true;

		case NCONTROL_LISTBOX:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFListBox *pListBox = &m_control[ controlindex ].listbox;

				((CList*)(pControlObj))->Create(
					id, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pListBox->bMultiLine,
					pListBox->bMultiSelect,
					pListBox->bFixedRow,
					true,
					pListBox->lineHeight
					);
			}
			return true;

		case NCONTROL_LISTVIEW:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFListView *pListView = &m_control[ controlindex ].listview;

				((CListView*)(pControlObj))->Create(
					id, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pListView->startX,
					pListView->startY,
					pListView->offsetX,
					pListView->offsetY,
					pListView->itemsizeX,
					pListView->itemsizeY,
					pListView->bEmpty,
					pListView->bCountVisible,
					pListView->bUsable,
					pListView->tooltipWidth,
					pListView->tooltipHeight
					);
			}
			return true;

		case NCONTROL_CHECKBOX:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFCheckBox *pCheckBox = &m_control[ controlindex ].checkbox;

				((CCheckBox*)(pControlObj))->Create(
					id, 
					pCheckBox->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight 
					);
					
				
				SI32 index = _tstoi(pCheckBox->text);

				if(index>0)
				{
					TCHAR* pText = GetTxtFromMgr(index);
					if( pclClient->siServiceArea == ConstServiceArea_Japan )
						((CCheckBox*)(pControlObj))->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					else
						((CCheckBox*)(pControlObj))->SetText(pText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				}
			}
			return true;

		case NCONTROL_RADIOBUTTON:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFRadioButton *pRadioButton = &m_control[ controlindex ].radiobutton;

				((CRadioButton*)(pControlObj))->Create(
					id, 
					pRadioButton->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pRadioButton->groupid
					);
					
				SI32 index= _tstoi(pRadioButton->text);

				if(index>0)
				{
					TCHAR* pText = GetTxtFromMgr(index);
					if( pclClient->siServiceArea == ConstServiceArea_Japan )
						((CRadioButton*)(pControlObj))->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					else
						((CRadioButton*)(pControlObj))->SetText(pText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);     	
				}

			}
			return true;

		case NCONTROL_OUTLINE:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFOutLine *pOutLine = &m_control[ controlindex ].outline;

				((COutLine*)(pControlObj))->Create(
					id, 
					pOutLine->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight
					);
			}
			return true;

		case NCONTROL_CHATEDIT:
			{
				NFControl *pControl = &m_control[ controlindex ];
				NFChatEdit *pChatEdit = &m_control[ controlindex ].chatedit;

				((CChatEdit*)(pControlObj))->Create(
					id, 
					pChatEdit->text, 
					pControl->imageFilename,
					pControl->startFont,
					pControl->bImageTile,
					pControl->imageTileType,
					pControl->x,
					pControl->y,
					pControl->width,
					pControl->height,
					pControl->surfaceWidth,
					pControl->surfaceHeight,
					pChatEdit->lineHeight,
					pChatEdit->bAutoScroll,
					pChatEdit->offsetX,
					pChatEdit->offsetY
					);
			}
			return true;
	}

	return false;

}


int CInterfaceFile::GetControlIndex( TCHAR *szID )
{
	for( int i = 0; i < m_controlNum; ++i ) {
		if( _tcsicmp( m_control[ i ].id, szID ) == 0 ) return i;
	}

	return -1;
}

bool CInterfaceFile::LoadFile( TCHAR *szFilename )
{

	TCHAR buf[ 4096 ];
	int	bufPos = 0;

	TCHAR ch;
	DWORD dwRead;


	HANDLE hFile = CreateFile( szFilename, 
							GENERIC_READ, 
							FILE_SHARE_READ, 
							NULL, 
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL );

	if( hFile == INVALID_HANDLE_VALUE ) 
	{
#ifdef _DEBUG
		MessageBox(NULL, szFilename, TEXT("DDF File Loading Error"), MB_OK|MB_TOPMOST);
#endif
		return false;
	}
	
	bool loop = true;
	while( loop ) 
	{
		if( ReadFile( hFile, &ch, 1, &dwRead, NULL ) == false ) 
		{
			break;
		}

		if( dwRead != 1 ) 
		{
			break;
		}

		if( ch == 10 || ch == 13 ) 
		{
			if( bufPos == 0 ) continue;

			buf[ bufPos ] = NULL;
			ReadControlInfo( buf );

			bufPos = 0;

			continue;
		} 

		buf[ bufPos++ ] = ch;
	}


	CloseHandle( hFile );


	return true;
}


void CInterfaceFile::ReadControlInfo( TCHAR *szText )
{

	int nCount = 0;

	int readPos = 0;
	int startPos = 0;

	int size;

	TCHAR buffer[ 128 ];

	bool loop = true;
	while( loop ) 
	{
		if( szText[ readPos ] == NULL ) 
		{
			break;
		}


		if( szText[ readPos ] == ',' ||
			szText[ readPos ] == ';' ) 
		{
			size = readPos - startPos;

			if( size < 1 || size > 127 ) 
			{
				buffer[ 0 ] = NULL;
			} 
			else 
			{
				memcpy( buffer, &szText[ startPos ], size );
				buffer[ size ] = NULL;
			}

			startPos = readPos + 1;

			// 첫번째이면, 컨트롤 종류 읽기

			if( nCount == 0 ) 
			{
				m_control[ m_controlNum ].type = GetControlKind( buffer );
			} 
			else 
			{
				SetBaseInfo( m_controlNum, nCount, buffer );

				switch( m_control[ m_controlNum ].type ) 
				{
					case NCONTROL_DIALOG:
						SetDialogInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_BUTTON:
						SetButtonInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_COMBOBOX:
						SetComboBoxInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_EDITBOX:
						SetEditBoxInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_STATICTEXT:
						SetStaticTextInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_IMAGESTATIC:
						SetImageStaticInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_LISTBOX:
						SetListBoxInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_LISTVIEW:
						SetListViewInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_CHECKBOX:
						SetCheckBoxInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_RADIOBUTTON:
						SetRadioButtonInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_OUTLINE:
						SetOutLineInfo( m_controlNum, nCount, buffer );
						break;

					case NCONTROL_CHATEDIT:
						SetChatEditInfo( m_controlNum, nCount, buffer );
						break;
				}
			}

			++nCount;
		}

		++readPos;
	}

	++m_controlNum;
}


int CInterfaceFile::GetControlKind( TCHAR *szText )
{

	for( int i = 0; i < MAX_NCONTROL_NUMBER; ++i ) {

		if( _tcscmp( szText, szControlKind[ i ] ) == 0 ) { 

			return i;
		}
	}

	return -1;
}


void CInterfaceFile::SetBaseInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 0:			
			break;
		case 1:
			memcpy( m_control[ controlindex ].id, szText, 32 );
			m_control[ controlindex ].id[ 31 ] = NULL;
			break;
		case 2:
			m_control[ controlindex ].x = num;
			break;
		case 3:
			m_control[ controlindex ].y = num;
			break;
		case 4:
			m_control[ controlindex ].width = num;
			break;
		case 5:
			m_control[ controlindex ].height = num;
			break;
		case 6:
			memcpy( m_control[ controlindex ].imageFilename, szText, 256 );
			m_control[ controlindex ].imageFilename[ 255 ] = NULL;
			break;
		case 7:
			m_control[ controlindex ].surfaceWidth = num;
			break;
		case 8:
			m_control[ controlindex ].surfaceHeight = num;
			break;
		case 9:
			if( num )
			m_control[ controlindex ].bImageTile = true;
			else
			m_control[ controlindex ].bImageTile = false;
			break;
		case 10:
			m_control[ controlindex ].imageTileType = num;
			break;
		case 11:
			m_control[ controlindex ].startFont = num;
			break;
		case 12:
			m_control[ controlindex ].textColor = num;
			break;
		case 13:
			memcpy( m_control[ controlindex ].textFont, szText, 32 );
			m_control[ controlindex ].textFont[ 31 ] = NULL;
			break;
		case 14:
			m_control[ controlindex ].textSize = num;
			break;
	}

}

void CInterfaceFile::SetDialogInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		
		case 15:
			memcpy( m_control[ controlindex ].dialog.title, szText, 64 );
			m_control[ controlindex ].dialog.title[ 64 ] = NULL;
			return;

		case 16:
			if( num ) {
				m_control[ controlindex ].dialog.bTitleVisible = true;
			} else {
				m_control[ controlindex ].dialog.bTitleVisible = false;
			}

			return;

		case 17:
			if( num ) {
				m_control[ controlindex ].dialog.bMovable = true;
			} else {
				m_control[ controlindex ].dialog.bMovable = false;
			}
			return;

		case 18:
			if( num ) {
				m_control[ controlindex ].dialog.bClosable = true;
			} else {
				m_control[ controlindex ].dialog.bClosable = false;
			}

			return;
	}
}

void CInterfaceFile::SetButtonInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].button.groupnum = num;
			return;
		case 16:
			memcpy( m_control[ controlindex ].button.text, szText, 64 );
			m_control[ controlindex ].button.text[ 63 ] = NULL;
			return;
		case 17:
#ifndef _NEW_INTERFACE_
			m_control[ controlindex ].button.animation = num;   // 버튼 에니메이션 적용 일괄 해제
#else
			//m_control[ controlindex ].button.animation = num;
#endif

			return;
	}
}

void CInterfaceFile::SetComboBoxInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].combobox.groupnum = num;
			return;
		case 16:
			m_control[ controlindex ].combobox.popupHeight = num;
			return;
		case 17:
			m_control[ controlindex ].combobox.lineHeight = num;
			return;
	}

}

void CInterfaceFile::SetEditBoxInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].editbox.groupnum = num;
			return;
		case 16:
			memcpy( m_control[ controlindex ].editbox.text, szText, 64 );
			m_control[ controlindex ].editbox.text[ 63 ] = NULL;
			return;
		case 17:
			if( num ) {
				m_control[ controlindex ].editbox.bMultiLine = true;
			
			} else {
				m_control[ controlindex ].editbox.bMultiLine = false;
			}
			return;

		case 18:
			if( num ) {
				m_control[ controlindex ].editbox.bReadOnly = true;
			
			} else {
				m_control[ controlindex ].editbox.bReadOnly = false;
			}
			return;
	}

}

void CInterfaceFile::SetStaticTextInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].statictext.groupnum = num;
			return;
		case 16:
			memcpy( m_control[ controlindex ].statictext.text, szText, 64 );
			m_control[ controlindex ].statictext.text[ 63 ] = NULL;
			return;
	}

}

void CInterfaceFile::SetImageStaticInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].imagestatic.groupnum = num;
			return;
	}
}

void CInterfaceFile::SetListBoxInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].listbox.groupnum = num;
			return;

		case 16:
			if( num ) {
				m_control[ controlindex ].listbox.bMultiLine = true;
			} else {
				m_control[ controlindex ].listbox.bMultiLine = false;
			}			
			return;

		case 17:
			if( num ) {
				m_control[ controlindex ].listbox.bMultiSelect = true;
			} else {
				m_control[ controlindex ].listbox.bMultiSelect = false;
			}
			return;

		case 18:
			m_control[ controlindex ].listbox.lineHeight = num;
			return;

		case 19:
			if( num ) {
				m_control[ controlindex ].listbox.bFixedRow = true;
			} else {
				m_control[ controlindex ].listbox.bFixedRow = false;
			}
			return;
	}

}

void CInterfaceFile::SetListViewInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].listview.groupnum = num;
			return;

		case 16:
			if( num ) {
				m_control[ controlindex ].listview.bEmpty = true;
			} else {
				m_control[ controlindex ].listview.bEmpty = false;
			}			
			return;

		case 17:
			if( num ) {
				m_control[ controlindex ].listview.bCountVisible = true;
			} else {
				m_control[ controlindex ].listview.bCountVisible = false;
			}
			return;

		case 18:
			if( num ) {
				m_control[ controlindex ].listview.bUsable = true;
			} else {
				m_control[ controlindex ].listview.bUsable = false;
			}
			return;

		case 19:
			m_control[ controlindex ].listview.tooltipWidth = num;
			return;

		case 20:
			m_control[ controlindex ].listview.tooltipHeight = num;
			return;

		case 21:
			m_control[ controlindex ].listview.startX = num;
			return;

		case 22:
			m_control[ controlindex ].listview.startY = num;
			return;

		case 23:
			m_control[ controlindex ].listview.offsetX = num;
			return;

		case 24:
			m_control[ controlindex ].listview.offsetY = num;
			return;

		case 25:
			m_control[ controlindex ].listview.itemsizeX = num;
			return;

		case 26:
			m_control[ controlindex ].listview.itemsizeY = num;
			return;
	}
}

void CInterfaceFile::SetCheckBoxInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].checkbox.groupnum = num;
			return;
		case 16:
			memcpy( m_control[ controlindex ].checkbox.text, szText, 64 );
			m_control[ controlindex ].checkbox.text[ 63 ] = NULL;
			return;
	}
}

void CInterfaceFile::SetRadioButtonInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].radiobutton.groupnum = num;
			return;
		case 16:
			memcpy( m_control[ controlindex ].radiobutton.text, szText, 64 );
			m_control[ controlindex ].radiobutton.text[ 63 ] = NULL;
			return;
		case 17:
			m_control[ controlindex ].radiobutton.groupid = num;
			return;
	}
}

void CInterfaceFile::SetOutLineInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].outline.groupnum = num;
			return;
		case 16:
			memcpy( m_control[ controlindex ].outline.text, szText, 64 );
			m_control[ controlindex ].outline.text[ 63 ] = NULL;
			return;
	}
}

void CInterfaceFile::SetChatEditInfo( int controlindex, int index, TCHAR *szText )
{
	int num = _tstoi( szText );

	switch( index ) {
		case 15:
			m_control[ controlindex ].chatedit.groupnum = num;
			return;
		case 16:
			memcpy( m_control[ controlindex ].chatedit.text, szText, 64 );
			m_control[ controlindex ].chatedit.text[ 63 ] = NULL;
			return;
		case 17:
			m_control[ controlindex ].chatedit.lineHeight = num;
			return;
		case 18:
			if( num ) {
				m_control[ controlindex ].chatedit.bAutoScroll = true;
			} else {
				m_control[ controlindex ].chatedit.bAutoScroll = false;
			}			
			return;
		case 19:
			m_control[ controlindex ].chatedit.offsetX = num;
			return;
		case 20:
			m_control[ controlindex ].chatedit.offsetY = num;
			return;
	}
}
