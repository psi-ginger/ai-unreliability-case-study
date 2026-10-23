//Switch_Change_manufacture_request_interface
#include "NManufactureDlg_New.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-Personalshop.h"
#include "../../Client/Client.h"
#include "../../Client/Music/Music.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"
#include "../NManufactureSerchDlg/NManufactureSerchDlg.h"


CNManufactureDlg_New::CNManufactureDlg_New()
{	
	Initialize();
}

CNManufactureDlg_New::~CNManufactureDlg_New()
{
	Destroy();
}


void CNManufactureDlg_New::Initialize()
{

	m_bSelf		 = false;  
	m_bShopStarted = false;

	m_bShopDataChanged = false;

	memset(	m_szShopBanner, 0 , sizeof(m_szShopBanner));
	m_clShopData.Init();

	m_siShopperCharUnique = 0;		// MINI HOMEPAGE를 띄우기 위한 
	m_siShopperPersonID	  = 0;
	memset(m_szAccountID, 0, sizeof(m_szAccountID));
	memset(m_szCharName,  0, sizeof(m_szCharName));
	
	m_bShow = true;

	m_siImageType = -1;
	m_siImageIndex = -1;

	m_bClient	 = true ;	// 기본적으로는 의뢰인 

}

void CNManufactureDlg_New::Destroy()
{

}

void CNManufactureDlg_New::Create()
{
	if ( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NManufacture_New/DLG_NManufacture_New.ddf"));
		file.CreatePopupDialog( this, NMANUFACTURE_NEW_DLG , TEXT("dialog_NManufacture_new"), StaticNManufactureDlg_NewProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_6 , this ), NMANUFACTURE_NEW_STATIC_6,   "statictext_NONAME6"); // 제조의뢰
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_5 , this ), NMANUFACTURE_NEW_STATIC_5,   "statictext_NONAME5"); // 의뢰인
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_16 , this ), NMANUFACTURE_NEW_STATIC_16,   "statictext_NONAME16"); // 제작자
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_2 , this ), NMANUFACTURE_NEW_STATIC_2,   "statictext_NONAME2"); // 물품
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_7 , this ), NMANUFACTURE_NEW_STATIC_7,   "statictext_NONAME7"); // 물품명
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_8 , this ), NMANUFACTURE_NEW_STATIC_8,   "statictext_NONAME8"); // 기술레벨
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_12 , this ), NMANUFACTURE_NEW_STATIC_12,   "statictext_NONAME12"); // 이상
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_9 , this ), NMANUFACTURE_NEW_STATIC_9,   "statictext_NONAME9"); // 행운
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_13 , this ), NMANUFACTURE_NEW_STATIC_13,   "statictext_NONAME13"); // 이상
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_10 , this ), NMANUFACTURE_NEW_STATIC_10,   "statictext_NONAME10"); // 개수
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_14 , this ), NMANUFACTURE_NEW_STATIC_14,   "statictext_NONAME14"); // 개
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_11 , this ), NMANUFACTURE_NEW_STATIC_11,   "statictext_NONAME11"); // 개당 수수료
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_15 , this ), NMANUFACTURE_NEW_STATIC_15,   "statictext_NONAME15"); // 골드
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_3 , this ), NMANUFACTURE_NEW_STATIC_3,   "statictext_NONAME3"); // 총 수수료
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_NEW_STATIC_4 , this ), NMANUFACTURE_NEW_STATIC_4,   "statictext_NONAME4"); // 골드

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_NEW_BUTTON_SERCH , this ), NMANUFACTURE_NEW_BUTTON_SERCH,   "button_item_search"); // 제조의뢰 검색
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_NEW_BUTTON_START , this ), NMANUFACTURE_NEW_BUTTON_START,   "button_NONAME3"); // 제조의뢰 시작
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_NEW_BUTTON_STOP , this ), NMANUFACTURE_NEW_BUTTON_STOP,   "button_NONAME4"); // 제조의뢰 중
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_NEW_BUTTON_CLOSE , this ), NMANUFACTURE_NEW_BUTTON_CLOSE,   "button_NONAME5"); // 닫기

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NMANUFACTURE_NEW_COMBOBOX_GOODS , this ), NMANUFACTURE_NEW_COMBOBOX_GOODS,   "combobox_NONAME2"); // 물품
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME , this ), NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME,   "combobox_NONAME1"); // 물품명

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_NEW_EDIT_GOOD_EXPLAIN , this ), NMANUFACTURE_NEW_EDIT_GOOD_EXPLAIN,   "editbox_NONAME1");	// 만들 물건 설명
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_NEW_EDIT_SKILLLEVEL , this ), NMANUFACTURE_NEW_EDIT_SKILLLEVEL,   "editbox_NONAME4");	// 기술레벨 적는 곳
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_NEW_EDIT_LUCKLEVEL , this ), NMANUFACTURE_NEW_EDIT_LUCKLEVEL,   "editbox_NONAME5");	// 행운레벨 적는 곳
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_NEW_EDIT_GOODS_NUM , this ), NMANUFACTURE_NEW_EDIT_GOODS_NUM,   "editbox_NONAME6");	// 개수 적는 곳
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_NEW_EDIT_FEE_NUM , this ), NMANUFACTURE_NEW_EDIT_FEE_NUM,   "editbox_NONAME7");	// 개당 수수료 적는 곳
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_NEW_EDIT_FEE_TOTAL , this ), NMANUFACTURE_NEW_EDIT_FEE_TOTAL,   "editbox_NONAME2");	// 총수수료 적는 곳
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_NEW_EDIT_COMMENT , this ), NMANUFACTURE_NEW_EDIT_COMMENT,   "editbox_NONAME8");	// 제조의뢰 이용권이 1장 필요 합니다

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON, NMANUFACTURE_NEW_RADIOBUTTON_CLIENT , this ), NMANUFACTURE_NEW_RADIOBUTTON_CLIENT,   "radiobutton_NONAME1");	// 의뢰인 선택 radio button
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON, NMANUFACTURE_NEW_RADIOBUTTON_MANUFACTURER , this ), NMANUFACTURE_NEW_RADIOBUTTON_MANUFACTURER,   "radiobutton_NONAME2");	// 제작자 선택 radio button

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NMANUFACTURE_NEW_IMAGESTATIC_PICTURE, this ), NMANUFACTURE_NEW_IMAGESTATIC_PICTURE, "imagestatic_NONAME1");// 만들 아이템 그림

		CRadioButton* pRadioButton =  m_InterfaceFactory.GetRadioButton( NMANUFACTURE_NEW_RADIOBUTTON_CLIENT )	;
		if(pRadioButton != NULL )
		{
			pRadioButton->SetCheck(true)	;
		}
		CEdit* pTotalFeeEdit =  m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_FEE_TOTAL )	;
		if(pTotalFeeEdit != NULL )
		{
			pTotalFeeEdit->SetReadOnly(true);
		}

		MovePos(250,105);
		SetMakeableItemList();
		SetActionTimer( 100 )	;
		SetTimerOnOff(true);

		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENMAKEREQUEST);
	}
}

void CALLBACK CNManufactureDlg_New::StaticNManufactureDlg_NewProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNManufactureDlg_New *pThis = (CNManufactureDlg_New*) pControllerMgr;
	pThis->NManufactureDlg_NewProc( nEvent, nControlID, pControl );
}

void CALLBACK CNManufactureDlg_New::NManufactureDlg_NewProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMANUFACTURE_NEW_BUTTON_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NMANUFACTURE_NEW_BUTTON_SERCH:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient *pclclient = (cltClient *)pclClient;
					pclclient->CreateInterface( MANUFACTURESEARCH_DLG );
					
					if(pclclient->m_pDialog[MANUFACTURESEARCH_DLG])
					{
						(( CNManufactureSearchDlg * )pclclient->m_pDialog[MANUFACTURESEARCH_DLG])->NManufactureSearchDlgProc(EVENT_LISTBOX_SELECTION,NPERSONALSHOPSEARCH_LISTBOX_SHOP_SHOWCASE,NULL);

					}					
				}
				break;
			}
		}
		break;
	case NMANUFACTURE_NEW_BUTTON_START :
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(m_bSelf	==	TRUE )
					{
						// 제작자 일때
						if ( m_bClient == false ) 
						{
							// 제작의뢰자 일때
							if(true == CheckMakeData_Maker() )
							{
								StartPersonalShop();
							}
						}
						else
						{
							// 제작의뢰자 일때
							if(true == CheckMakeData_Client() )
							{
								StartPersonalShop();
							}
						}
					}
					else
					{
						MakeShopItem();
						DeleteDialog()	;
					}
				}
			}
			break;
		}
		break;
	case NMANUFACTURE_NEW_BUTTON_STOP :
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					StopPersonalShop();
				}
			}
            break;
		}
		break;
	case NMANUFACTURE_NEW_COMBOBOX_GOODS :	// 콤보 박스를 선택하면 
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					if(m_bClient == false )	// 제작자 등록을 할때 
					{
						if(m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS)->GetSelectedIndex() < 0 )	return	;
						SetComboBox_ItemCategory_Select();

						
					}
				}break	;
			}
			break;
		}
		break;
	case NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME :	// 콤보 박스를 선택하면 
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
		
					if ( m_bClient == false )	// 제작자 일때 아이템을 선택했다.
					{
						CEdit* pSkillLevel_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_SKILLLEVEL )	;									if(pSkillLevel_EditBox == NULL )	return	;
						CEdit* pLuckStatLevel_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_LUCKLEVEL )	;								if(pLuckStatLevel_EditBox == NULL )	return	;
						CEdit* pItemnum_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_GOODS_NUM )	;										if(pItemnum_EditBox == NULL )	return	;
						pSkillLevel_EditBox->SetReadOnly(false)	;
						pLuckStatLevel_EditBox->SetReadOnly(false)	;						
						pSkillLevel_EditBox->Enable( true )	;	
						pLuckStatLevel_EditBox->Enable( true )	;

						// 선택한 아이템 정보 
						CComboBox* pSelect_Item_ComboBox = m_InterfaceFactory.GetComboBox( NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME )	;					if(pSelect_Item_ComboBox == NULL )	return	;
						SI16 siSelectIndex = pSelect_Item_ComboBox->GetSelectedIndex()	;																if (siSelectIndex < 0 )				return	;
						TCHAR* pItemName = pSelect_Item_ComboBox->GetText( siSelectIndex)	;															if(pItemName	==	NULL	)		return	;
						SI32 siItemUnique = pclClient->pclItemManager->FindUniqueFromName( pItemName )	;												if(siItemUnique	<=	0	)			return	;
						cltCharClient* pChar = pclClient->pclCM->GetCharClient(1)	;																	if ( pChar == NULL )				return	;

						for(SI32 i = 0;i < MAX_ITEMINFO_NUMBER;i++)
						{
							cltItemInfo* pinfo = pclClient->pclItemManager->pclItemInfo[i];
							if(pinfo == NULL)continue;

							if(siItemUnique	==	pinfo->clItem.siUnique)
							{
								SI32 siTotalSkillLevel = pChar->pclCI->clCharSkill.GetTotalSkillLevel( pinfo->clSkillInfo.siSkill,0,0,&pChar->pclCI->clCharItem.clItem[0] )	;
								SI32 siLeckLevel = pChar->pclCI->clIP.GetLuk()	;	// 행운 레벨 

								pSkillLevel_EditBox->SetText( SI32ToString( siTotalSkillLevel ) )	;
								pLuckStatLevel_EditBox->SetText( SI32ToString( siLeckLevel ) )	;
								pItemnum_EditBox->SetText(TEXT("1") )	;
								pSkillLevel_EditBox->SetReadOnly(true)	;
								pLuckStatLevel_EditBox->SetReadOnly(true)	;
								pItemnum_EditBox->SetReadOnly(true)	;
								pSkillLevel_EditBox->Enable( false )	;	
								pLuckStatLevel_EditBox->Enable( false )	;
								pItemnum_EditBox->Enable(false)	;
								break	;
							}
						}
					}
					else
					{
						//제작의뢰로서 아이템을 선택
						m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS)->Enable( false )	;

						TCHAR szSelText[128];
						TCHAR *pITemName = NULL;
						TCHAR Nbuffer[512]=TEXT(" ");
						SI16 siTmpIndex = -1;
						szSelText[0] = '\0';

						siTmpIndex = m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->GetSelectedIndex();		if(siTmpIndex < 0 ) return	;
						pITemName = m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->GetText( siTmpIndex );
						// 선택을 했을 경우
						if(pITemName != NULL )
						{
							StringCchCopy(szSelText,sizeof(szSelText), pITemName )	;
							//_tcscpy( szSelText, pITemName );
							TCHAR *pTemp1 = _tcsrchr( szSelText, _T('-') );	 //뒤에서부터 '-' 문자를 검색함
							TCHAR* pText = GetTxtFromMgr(2219);
							TCHAR *pTemp2 = _tcsrchr( szSelText, _T('-') );
							if(pTemp1 == NULL || pTemp2 == NULL) return	;
							*pTemp1 = '\0';	pTemp1++;
							*pTemp2 = '\0';
							SI32 siMaxMakeItem = _tstoi(pTemp1);
							if(siMaxMakeItem <= 0)				{ return ;   }
							// 이름을 토대로 아이템을 가지고 온다.
							SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(szSelText);

							SI64 atb = pclClient->pclItemManager->GetItemAtb(itemunique);
							if(atb & ITEMATB_OBJECT)
							{
								m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->Clear();	
								m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetText(TEXT("1"));
								m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetReadOnly(true);
								m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->Refresh()	;
							}
							else
							{
								m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->Clear();
								m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetReadOnly(false);
								m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->Refresh()	;
							}

						}


					}
				}
			}
			break;
		}
		break;
	case NMANUFACTURE_NEW_RADIOBUTTON_CLIENT:
		{
			switch(nEvent) {
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(true == m_InterfaceFactory.GetRadioButton( NMANUFACTURE_NEW_RADIOBUTTON_CLIENT )->IsCheck())
					{
						Set_Client_Mode();
						
					}
				}
				break;
			}
		}break	;
	case NMANUFACTURE_NEW_RADIOBUTTON_MANUFACTURER:	// 제작자 
		{
			switch(nEvent) 
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(true == m_InterfaceFactory.GetRadioButton( NMANUFACTURE_NEW_RADIOBUTTON_MANUFACTURER )->IsCheck() )
					{
						Set_Manufacture_Mode()	;
					}
				}
				break;
			}
		}break	;
	case NMANUFACTURE_NEW_EDIT_GOODS_NUM:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szGoodsNum = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_GOODS_NUM )->GetText() ;
					SI32 siNum = atoi( szGoodsNum ) ; 

					if(siNum >=MAX_ITEM_PILE_NUMBER)
					{
						m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_GOODS_NUM )->SetText( SI32ToString( MAX_ITEM_PILE_NUMBER ))	;
					}
				}
			}
		}break	;
	case NMANUFACTURE_NEW_EDIT_FEE_NUM:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szPrice = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_FEE_NUM )->GetText() ;
					GMONEY siprice = _atoi64( szPrice ) ; 

					if(siprice >=MAX_FEE)
					{
						m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_FEE_NUM )->SetText( SI64ToString( MAX_FEE ))	;
					}
				}
			}
		}break	;
	}
}


void CNManufactureDlg_New::Show( SI32 siCharUnique, SI32 siPersonID, SI16 ShopMode, TCHAR *Name, TCHAR *AccountID, BOOL bShopStarted )
{

	if(m_bShow == false)	
		CControllerMgr::Show( true );
	m_bShow = true;

	if(AccountID && _tcslen(AccountID))	StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, AccountID);
	if(Name&& _tcslen(Name))				StringCchCopy(m_szCharName,  MAX_PLAYER_NAME, Name);

	m_siShopperCharUnique = siCharUnique;
	m_siShopperPersonID   = siPersonID;
	m_bShopStarted		  = bShopStarted;

	Action();
	SetMakeableItemList();
	BOOL bSelf = (siCharUnique == pclClient->pclCM->CR[1]->GetCharUnique());
	ShopInit(bSelf);

	if(ShopMode == PSHOP_MODE_MAKEITEM)
	{
		m_InterfaceFactory.GetRadioButton(NMANUFACTURE_NEW_RADIOBUTTON_CLIENT)->SetCheck(true);
	}
	else
	{
		m_InterfaceFactory.GetRadioButton(NMANUFACTURE_NEW_RADIOBUTTON_MANUFACTURER)->SetCheck(true);
	}

}


void CNManufactureDlg_New::ShopInit(BOOL bSelf)
{
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOOD_EXPLAIN)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_NUM)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_TOTAL)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_COMMENT)->SetText(TEXT(""));
	
	m_InterfaceFactory.GetButton(NMANUFACTURE_NEW_BUTTON_STOP)->Enable(false);

	// DialogBox 수정 
	TCHAR buffer[256];

	TCHAR* pText = GetTxtFromMgr(2194);
	StringCchPrintf(buffer, 256, pText, m_szCharName);	
	this->SetTitle( buffer );

	m_bSelf		 = bSelf;

	// 배너정보를 만든다.
	cltShopBannerData clShopBanner;
	clShopBanner.SetData(&m_clShopData);
	clShopBanner.MakeBannerText(pclClient->pclItemManager, m_szShopBanner);

	//// 정보들 초기화
	//// 자기자신의 상점 설정 

	if(bSelf == TRUE)
	{
		cltClient *pclclient = (cltClient*)pclClient;
					
     	// Edit 텍스트 개수 제한!!
		m_szShopBanner[ MAX_PERSONAL_SHOP_TEXT_SIZE - 1] = '\0';
		
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOOD_EXPLAIN)->SetText(m_szShopBanner);

		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->SetReadOnly(false);
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->SetReadOnly(false);

	//	// 상점이 이미 시작이 되었으면 
		if(m_bShopStarted)
		{
			m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->Enable(false);
			m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS)->Enable(false);
			
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_NUM)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->SetReadOnly(true);

			m_InterfaceFactory.GetButton(NMANUFACTURE_NEW_BUTTON_START)->Enable(false);
			m_InterfaceFactory.GetButton(NMANUFACTURE_NEW_BUTTON_STOP)->Enable(true);
			
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_NUM)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->SetReadOnly(true);

			TCHAR strBuffer[256] = {'\0',};

			StringCchPrintf(strBuffer,256,TEXT(" %d"), m_clShopData.m_siHandPoint );            
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->SetText( strBuffer );

			StringCchPrintf(strBuffer,256,TEXT(" %d"), m_clShopData.m_clItem[0].siItemNum );
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetText( strBuffer );

			StringCchPrintf(strBuffer,256,TEXT(" %d"), m_clShopData.m_siPrice[0] );
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_NUM)->SetText( strBuffer );

			StringCchPrintf(strBuffer,256,TEXT(" %d"), m_clShopData.m_siSkillLevel );
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->SetText( strBuffer );

			SetItemImageData( m_clShopData.m_clItem[0].siUnique);

			m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->Show(false);
		}
		else
		{
			m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->Enable(true);
			m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS)->Enable(true);

			m_InterfaceFactory.GetButton(NMANUFACTURE_NEW_BUTTON_START)->Enable(true);

			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->SetReadOnly(false);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetReadOnly(false);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_NUM)->SetReadOnly(false);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->SetReadOnly(false);
		}
		
	}
	else		// bSelf == false . 자신의 상점이 아니다.
	{
		cltClient *pclclient = (cltClient*)pclClient;

		SI32 id = cltCharCommon::CharUniqueInfo[m_siShopperCharUnique];
		if(id == 0)
		{
			return ;
		}
		if ( m_clShopData.m_clItem[0].siItemNum == 0 ) 
		{
			DeleteDialog()	;
			return	;
		}

		cltCharClient* pclchar = (cltCharClient*)pclclient->pclCM->CR[id];
			
		// Edit 텍스트 개수 제한!!
		m_szShopBanner[ MAX_PERSONAL_SHOP_TEXT_SIZE - 1] = '\0';
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOOD_EXPLAIN)->SetText(m_szShopBanner);

		//라디오버튼선택 불가 .
		m_InterfaceFactory.GetStatic(NMANUFACTURE_NEW_STATIC_6)->Show(false);
		m_InterfaceFactory.GetStatic(NMANUFACTURE_NEW_STATIC_5)->Show(false);
		m_InterfaceFactory.GetStatic(NMANUFACTURE_NEW_STATIC_16)->Show(false);
		m_InterfaceFactory.GetRadioButton(NMANUFACTURE_NEW_RADIOBUTTON_CLIENT)->Show(false);
		m_InterfaceFactory.GetRadioButton(NMANUFACTURE_NEW_RADIOBUTTON_MANUFACTURER)->Show(false);
		
		m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->Enable(false);
		m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS)->Enable(false);

		pText = GetTxtFromMgr(2208);
		
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetText(TEXT(""));		
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->SetReadOnly(true);
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->SetReadOnly(true);

		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_NUM)->SetText(SI64ToString(m_clShopData.m_siPrice[0]) )	;

		m_InterfaceFactory.GetButton(NMANUFACTURE_NEW_BUTTON_START)->Enable(true);
		m_InterfaceFactory.GetButton(NMANUFACTURE_NEW_BUTTON_START)->SetText(pText);
		m_InterfaceFactory.GetButton(NMANUFACTURE_NEW_BUTTON_STOP)->Enable(false);

		// 중간에 정보 세팅
		TCHAR strBuffer[256] = {'\0',};

		// 콤보 박스에 만드는 아이템 이름을 넣는다 
		m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->Enable(false);
		TCHAR* szitemname = (TCHAR*)pclClient->pclItemManager->GetName(m_clShopData.m_clItem[0].siUnique)	;
		if ( szitemname != NULL) 
		{
			m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->SetText(  szitemname )	;
		}
		StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_siHandPoint );
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->SetText(strBuffer);

		StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_siSkillLevel );
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->SetText(strBuffer);

		SI64 atb = pclClient->pclItemManager->GetItemAtb(m_clShopData.m_clItem[0].siUnique);
		if(atb & ITEMATB_OBJECT)
		{
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetText(TEXT("1"));
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetReadOnly(true);            			
		}
		else
		{
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetText( SI32ToString(m_clShopData.m_clItem[0].siItemNum) );
		}
		SetItemImageData( m_clShopData.m_clItem[0].siUnique);
	}

}


void CNManufactureDlg_New::Hide()
{
	if(m_bShow) CControllerMgr::Show(false);
	{
		m_bShow = false;
	}
}
void CNManufactureDlg_New::Action()
{
	SetEditBox()	;
	// 아이템 총 가격을 셋팅한다.
	CEdit* pFee = m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_NUM)	;			if (pFee == NULL)	return	;
	CEdit* pNum = m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)	;		if (pNum == NULL)	return	;
	CEdit* pTotalFee = m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_TOTAL)	;	if (pTotalFee == NULL)	return	;

	TCHAR* szFee = pFee->GetText()	;	if ( szFee == NULL )	return	;
	TCHAR* szNum = pNum->GetText()	;	if ( szNum == NULL )	return	;

	SI64 sifee	=	0	;
	SI64 siNum	=	0	;

	sifee = _atoi64(szFee)	;
	siNum = _atoi64(szNum)	;
	GMONEY siTotalfee = sifee * siNum ;
	pTotalFee->SetText( SI64ToString(siTotalfee) )	;
}

void CNManufactureDlg_New::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( MsgIndex )
	{
	case 0:	// 개인 상점 시작
		{
			if ( RetValue )
			{
				SI32 PersonID = *((SI32*)pData);

				TCHAR szText[MAX_PERSONAL_SHOP_TEXT_SIZE]=TEXT("");
				TCHAR *pTmpChar = NULL;
				pTmpChar = m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOOD_EXPLAIN)->GetText();
				
				if( pTmpChar == NULL )	return ;
				StringCchCopy(szText,sizeof(szText), pTmpChar )	;
				//_tcscpy( szText, pTmpChar );

				if(_tcslen(szText) >= MAX_PERSONAL_SHOP_TEXT_SIZE ) return;
				if(m_bClient == true )
				{
					m_clShopData.m_siShopMode   = PSHOP_MODE_MAKEITEM;
				}
				else
				{
					m_clShopData.m_siShopMode   = PSHOP_MODE_MAKEITEM_WANT;
				}

				cltGameMsgRequest_StartPersonalShop clshopdata( m_siShopperCharUnique,  &m_clShopData);
				cltMsg clMsgDt( GAMEMSG_REQUEST_STARTPERSONALSHOP, sizeof( clshopdata ), (BYTE*)&clshopdata );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsgDt);

				DeleteDialog();
			}
		}
		break;
	}
}


void CNManufactureDlg_New::SetMakeableItemList()
{
	stComboBoxItemData tmpComboBoxItemData;
	// 콤보 박스를 지운다.
	CComboBox *pComboBox =	m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME);
	if(pComboBox !=NULL )
	{
		pComboBox->Clear();

		SI32 m_siMakeableItemNum = 0;
		SI32 *m_pMakeableItemList = NULL;
		SI32 id = 1;

		// 만들수 있는 아이템 종류를 보여준다. 
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		if(pclitemmanager->FindMakeableItem(NULL, 
			&pclClient->pclCM->CR[id]->pclCI->clCharItem,
			NULL,
			MAX_INVITEM_PER_PERSON,
			&pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_INV0],//인벤에 있는 아이템읋 토대로 만들수 있는 아이템을 리턴
			&m_siMakeableItemNum,
			&m_pMakeableItemList,true) == true)
		{
			TCHAR buffer[256];
			for(SI32 i = 0; i < m_siMakeableItemNum; ++i)
			{
				TCHAR* pText = GetTxtFromMgr(2246);
				StringCchPrintf(buffer, 256, pText, pclitemmanager->GetName(m_pMakeableItemList[i*2]), m_pMakeableItemList[i*2 + 1]);
				//-----------------------------------------------------------
				//	만들수 있는 아이템중에 빼야 하는 타입이 있는지 확인 한다 
				//-----------------------------------------------------------
				TCHAR szSelText[128];
				TCHAR *pITemName = NULL;
				szSelText[0] = '\0';

				StringCchCopy(szSelText,sizeof(szSelText),buffer)	;
				//_tcscpy( szSelText, buffer );
				TCHAR *pTemp1 = _tcsrchr( szSelText, _T('-') );	 //뒤에서부터 '-' 문자를 검색함
				TCHAR* pText_num = GetTxtFromMgr(2219);
				TCHAR *pTemp2 = _tcsrchr( szSelText, _T('-') );
				if(pTemp1 == NULL || pTemp2 == NULL) return	;
				*pTemp1 = '\0';	pTemp1++;
				*pTemp2 = '\0';

				// 이름을 토대로 아이템을 가지고 온다.
				SI32 siItemUnique = pclClient->pclItemManager->FindUniqueFromName( szSelText )	;
				SI32 siItemType = pclClient->pclItemManager->GetItemType(siItemUnique )	;
				if (CheckItemType(siItemType ) == false)	continue	;	// 적절한 타입이 아니면 패스 
				
				tmpComboBoxItemData.Init();
				tmpComboBoxItemData.Set( buffer );
				pComboBox->AddItem( &tmpComboBoxItemData );
			}
		}
		if(m_pMakeableItemList)
		{
			delete []m_pMakeableItemList;
			m_pMakeableItemList = NULL;
		}
	}
}



void CNManufactureDlg_New::StartPersonalShop()
{
	cltClient *pclclient = ( cltClient * )pclClient;	

	SI32 siTicketNum = 1;
	if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(ITEMUNIQUE(24065)) >= siTicketNum )
	{
		TCHAR buffer[256] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(6322);
		StringCchPrintf(buffer, 256, pText, siTicketNum);
		pText = GetTxtFromMgr(6323);

		stMsgBox MsgBox;
		MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0 );

	}
	else	
	{
  		TCHAR buffer[256] = TEXT("") ;
		TCHAR* pText = GetTxtFromMgr(6321);
		StringCchPrintf(buffer, 256, pText, siTicketNum);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(TEXT(""),buffer)	;
	}
}


void CNManufactureDlg_New::MakeShopItem()
{
	// 어떤 제작인가에 따라서
	if ( m_clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM ) //제조요청
	{
		if(m_clShopData.m_clItem[ 0 ].siItemNum>0)
		{
			TCHAR AmountBuffer[256];
			SI32  siAmountSuccess	= 0;
			SI64  siAmountValue     = 0;
			TCHAR *pTmpChar = NULL;
			pTmpChar = m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->GetText();

			if(pTmpChar == NULL )
			{
				TCHAR* pText = GetTxtFromMgr(2240);
				DisplayMessage(FALSE, pText); 

				return ;
			}
			StringCchCopy(AmountBuffer,sizeof(AmountBuffer), pTmpChar )	;
			//_tcscpy( AmountBuffer, pTmpChar );

			siAmountSuccess = _tcslen( AmountBuffer );

			if(siAmountSuccess>0)	siAmountValue = _tstoi64(AmountBuffer);
			if(siAmountValue > 0)
			{
				cltClient *pclclient = ( cltClient * )pclClient;
				cltItem clitem;
				clitem.Set(&m_clShopData.m_clItem[ 0 ]);	// 다른 모든 정보는 같으나 
				clitem.siItemNum  = (SI32)siAmountValue;								// 갯수는 다르다.

				GMONEY  siPrice	    = 0;
				SI32  siHandpoint   = 0;
				SI32  siSkillLevel  = 0;
				siPrice			  = m_clShopData.m_siPrice[ 0 ];
				siHandpoint		  = m_clShopData.m_siHandPoint;
				siSkillLevel	  = m_clShopData.m_siSkillLevel;

				SI08 siType = pclClient->pclItemManager->FindMakeAniTypeFromUnique(clitem.siUnique);

				cltGameMsgRequest_MakePersonalShop clInfo(m_siShopperCharUnique, PSHOP_MODE_MAKEITEM, m_siShopperPersonID, &clitem, siPrice, siHandpoint, siSkillLevel,0,siType);
				cltMsg clMsg( GAMEMSG_REQUEST_MAKEPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

				DisplayMessage(TRUE, TEXT(""));
				return ;
			}
			else { 
				TCHAR* pText = GetTxtFromMgr(2240);
				DisplayMessage(FALSE, pText); }
		}
		else 
		{ 
			TCHAR* pText = GetTxtFromMgr(2241);
			DisplayMessage(FALSE, pText);	
		}
	}
	else if(m_clShopData.m_siShopMode == PSHOP_MODE_MAKEITEM_WANT )
	{
		if(m_clShopData.m_clItem[ 0 ].siItemNum>0)
		{
			TCHAR AmountBuffer[256];
			SI32  siAmountSuccess	= 0;
			SI64  siAmountValue     = 0;
			TCHAR *pTmpChar = NULL;
			pTmpChar = m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->GetText();

			if(pTmpChar == NULL )
			{
				TCHAR* pText = GetTxtFromMgr(2240);
				DisplayMessage(FALSE, pText); 

				return ;
			}
			StringCchCopy(AmountBuffer,sizeof(AmountBuffer), pTmpChar )	;
			//_tcscpy( AmountBuffer, pTmpChar );

			siAmountSuccess = _tcslen( AmountBuffer );

			if(siAmountSuccess>0)	siAmountValue = _tstoi64(AmountBuffer);
			if(siAmountValue > 0)
			{
				cltClient *pclclient = ( cltClient * )pclClient;
				cltItem clitem;
				clitem.Set(&m_clShopData.m_clItem[ 0 ]);	// 다른 모든 정보는 같으나 
				clitem.siItemNum  = (SI32)siAmountValue;								// 갯수는 다르다.

				GMONEY  siPrice	    = 0;
				SI32  siHandpoint   = 0;
				SI32  siSkillLevel  = 0;
				siPrice			  = m_clShopData.m_siPrice[ 0 ];
				siHandpoint		  = m_clShopData.m_siHandPoint;
				siSkillLevel	  = m_clShopData.m_siSkillLevel;

				SI08 siType = pclClient->pclItemManager->FindMakeAniTypeFromUnique(clitem.siUnique);

				cltGameMsgRequest_MakePersonalShop_Manufactuer clInfo(m_siShopperCharUnique, PSHOP_MODE_MAKEITEM_WANT, m_siShopperPersonID, &clitem, siPrice, siHandpoint, siSkillLevel,0,siType);
				cltMsg clMsg( GAMEMSG_REQUEST_MAKEPERSONALSHOP_MANUFACTURE, sizeof(clInfo), (BYTE*)&clInfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

				DisplayMessage(TRUE, TEXT(""));
				return ;
			}
			else { 
				TCHAR* pText = GetTxtFromMgr(2240);
				DisplayMessage(FALSE, pText); }
		}
		else 
		{ 
			TCHAR* pText = GetTxtFromMgr(2241);
			DisplayMessage(FALSE, pText);	
		}
	}
	else
	{
		return ;
	}
}


bool CNManufactureDlg_New::DropShopItem()
{
	return true;
}

void CNManufactureDlg_New::StopPersonalShop()
{
	cltGameMsgRequest_StopPersonalShop clshopdata( m_siShopperCharUnique);
	cltMsg clMsg( GAMEMSG_REQUEST_STOPPERSONALSHOP, sizeof( clshopdata ), (BYTE*)&clshopdata );
	cltClient *pclclient = ( cltClient * )pclClient;
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

	DeleteDialog();
}

void CNManufactureDlg_New::SetItemImageData(SI32 ItemUnique)
{
	if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ItemUnique, &m_siImageType, &m_siImageIndex ) )
	{}
	else
	{
		m_siImageType = -1;
		m_siImageIndex = -1;
	}
}
void CNManufactureDlg_New::DisplayMessage(BOOL siSuccess, TCHAR *pBuffer)
{
	if(siSuccess == FALSE)	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")) , 0, 0);
	if(pBuffer)
	{
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_COMMENT)->SetText( pBuffer );		
	}
}


void CNManufactureDlg_New::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{

	if(m_siImageIndex < 0 || m_siImageType < 0)   
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageType );

	if ( pSpr )
	{

		GP.PutSpr( pSpr ,ScreenRX +  GetX() + 45,ScreenRY +  GetY() + 236, m_siImageIndex);

	}
}
bool CNManufactureDlg_New::CheckMakeData_Client()
{
	cltClient *pclclient = (cltClient *)pclClient;

	// 데이터 체크
	// 유니크 체크
	SI16 siItemUnique	=	m_clShopData.m_clItem[0].siUnique;
	if ( siItemUnique <= 0 )	return false ;
	SI16 siMakeItemNum	=	m_clShopData.m_clItem[0].siItemNum;
	GMONEY siMakeItemPrice	=	m_clShopData.m_siPrice[0];
	SI16 siLuckPoint		=	m_clShopData.m_siHandPoint;
	SI16 siSkillPoint		=	m_clShopData.m_siSkillLevel;
	
	// 아이템 갯수 체크
	if(siMakeItemNum <=0 )
	{
		TCHAR* pText = GetTxtFromMgr(1668);
		DisplayMessage(FALSE, pText);
		return false;
	}
	CComboBox* pGoods_Name_ComboBox = m_InterfaceFactory.GetComboBox( NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME )	;
	if(pGoods_Name_ComboBox != NULL)
	{
		TCHAR szText[128];

		SI32 siSelectindex = pGoods_Name_ComboBox->GetSelectedIndex();		if(siSelectindex < 0 ) return	false ;
		TCHAR* szSelectText = pGoods_Name_ComboBox->GetText( siSelectindex );
		if(szSelectText != NULL )
		{
			StringCchCopy(szText,sizeof(szText), szSelectText )	;
			//_tcscpy( szText, szSelectText );
			TCHAR *pTemp1 = _tcsrchr( szText, _T('-') );	 //뒤에서부터 '-' 문자를 검색함
			TCHAR* pText = GetTxtFromMgr(2219);
			TCHAR *pTemp2 = _tcsrchr( szText, _T('-') );
			if(pTemp1 == NULL || pTemp2 == NULL) return	false ;

			*pTemp1 = '\0';	pTemp1++;
			*pTemp2 = '\0';

			SI32 siMaxMakeItem = _tstoi(pTemp1);
			if(siMaxMakeItem <= 0)	return false ;

			if(siMakeItemNum > siMaxMakeItem )
			{
				TCHAR buffer[256];
				TCHAR* pText = GetTxtFromMgr(2221);
				StringCchPrintf(buffer, 256, pText, siMaxMakeItem);
				TCHAR * pTitle = GetTxtFromMgr(512) ;
				if ( !pTitle || !buffer ) return false;
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
				return false;
			}
			if(siMakeItemNum > MAX_ITEM_PILE_NUMBER)
			{
				TCHAR buffer[256];
				TCHAR* pText = GetTxtFromMgr(2211);
				StringCchPrintf(buffer, 256, pText, MAX_ITEM_PILE_NUMBER);
				TCHAR * pTitle = GetTxtFromMgr(512) ;
				if ( !pTitle || !buffer ) return false;
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
				return false;
			}
		}
		else
		{
			return false	;
		}
	}
	else
	{
		return	false	;
	}
	// 가격 체크
	if(siMakeItemPrice < 0)
	{
		TCHAR* pText = GetTxtFromMgr(2222);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !pText ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		return false;
	}
	if(siMakeItemPrice > MAX_FEE)
	{
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(2223);
		StringCchPrintf(buffer, 256, pText,MAX_FEE);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !buffer ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		return false;
	}
	// 행운 체크
	if(siLuckPoint < 0)
	{
		TCHAR* pText = GetTxtFromMgr(9865);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !pText ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		return false;
	}
	if(siLuckPoint > 2000)
	{
		TCHAR buffer[256];
		TCHAR* pText = NULL;
		pText = GetTxtFromMgr(9866);
		StringCchPrintf(buffer, 256, pText, 2000);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !buffer )return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		return false;
	}
	// 스킬레벨 체크
	if(siSkillPoint < 0)
	{
		TCHAR* pText = GetTxtFromMgr(2226);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !pText ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		return false;
	}
	if(siSkillPoint > MAX_SKILL_LEVEL+30)
	{
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(2227);
		StringCchPrintf(buffer, 256, pText,MAX_SKILL_LEVEL+30);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !buffer ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		return false;
	}
	return true	;
}
void CNManufactureDlg_New::SetComboBox_ItemCategory()	// 콤보 박스에 내가 만들수 있는 아이템을 셋팅 한다 .
{
	cltClient *pclclient = (cltClient *)pclClient;

	cltCharClient* pCharClient = pclClient->pclCM->GetCharClient(1)	;	if(pCharClient == NULL )	return	;

	CComboBox *pComboBox_ItemName	=	m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME	);
	if(pComboBox_ItemName == NULL )	return	;
	pComboBox_ItemName->Clear()	;
	CComboBox *pComboBox =	m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS);
	if ( pComboBox == NULL )	return	;
	pComboBox->Clear()	;
	for(SI32 i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{	
		cltItemTypeInfo* pinfo = pclclient->pclItemManager->pclItemTypeInfo[i];
		if(pinfo == NULL)continue;
		// 콤보리스트에서 숨기는 속성이 있다면
		if( pinfo->IsAtb(ITEMATB_HIDECOMBOLIST) ) continue;

		// 해당타입의 아이템중에 만들수 있는 아이템이 있는지 확인 한다 
		// 1. 스킬 체크  2 툴을 가지고 있는지 체크
		TCHAR* szTypeName = (TCHAR*)pinfo->GetName()	;
		SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName( szTypeName );
		if( true == ChekMakeItem_By_ItemType( itemtypeunique ))
		{
			stComboBoxItemData ComboBoxItemData;
			ComboBoxItemData.Init();
			ComboBoxItemData.Set(  szTypeName );
			pComboBox->AddItem( &ComboBoxItemData );   // 타입값에 넣는다
		}
	}
}
void CNManufactureDlg_New::SetComboBox_ItemCategory_Select()	// 카테고리르 선택했다.
{
	cltClient *pclclient = (cltClient *)pclClient;

	CComboBox *pComboBox =	m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS);
	if ( pComboBox == NULL )	return	;

	CComboBox *pComboBox_ItemName	=	m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME	);
	if(pComboBox_ItemName == NULL )	return	;
	pComboBox_ItemName->Clear()	;

	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_NUM	)->Clear()	;
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM	)->Clear()	;

	SI32 siSelectItem = pComboBox->GetSelectedIndex()	;
	if ( siSelectItem < 0 )	return	;

	TCHAR* szItemType = pComboBox->GetText( siSelectItem)	;
	SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName( szItemType );
	if(itemtypeunique >=0)
	{
		int i;
		// 콤보 박스에 아이템의 종류를 설정한다. 
		for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
		{	
			cltItemInfo* pinfo = pclClient->pclItemManager->pclItemInfo[i];
			if(pinfo == NULL)continue;
			if(pinfo->siType != itemtypeunique)continue;

			SI32 siItemUnique = pinfo->clItem.siUnique	;	// 아이템유니크 
			if(false == pclClient->pclItemManager->CanMakableItem( siItemUnique ) ) continue ;	// 만들지 못하는 아이템은 건너뛴다
			// 해당 아이템 타입들은 모두 패스 만들지 말어라
			// 주어진 타입과 같은 아이템만 열거한다. 
			// 사용자에게 보여지는 아이템인 경우에만. 
			if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
			{
				// 해당 아이템을 만들수 있는지 체크 한다 .
				if( ChekMakeItem(pinfo) == false )	continue	;

				stComboBoxItemData ComboBoxItemData;
				ComboBoxItemData.Init();
				ComboBoxItemData.Set( (TCHAR*)pinfo->GetName() );					

				pComboBox_ItemName->AddItem( &ComboBoxItemData );
			}
		}
	}
}

bool CNManufactureDlg_New::ChekMakeItem_By_ItemType(SI32 siItemtypeUnique )	// 아이템타입으로 만들수 있는 아이템이 있나 확인 
{
	cltClient *pclclient = (cltClient *)pclClient;

	if( siItemtypeUnique < 0 )	return false	;
	cltCharClient* pCharClient = pclClient->pclCM->GetCharClient(1)	;	if(pCharClient == NULL )	return	false ;
	for(SI32 i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		cltItemInfo* pinfo = pclClient->pclItemManager->pclItemInfo[i];
		if(pinfo == NULL)					continue;
		if(pinfo->siType != siItemtypeUnique)	continue;
		if( false == CheckItemType(pinfo->siType))	continue	;
		SI32 siItemUnique = pinfo->clItem.siUnique	;	// 아이템유니크 
		if(false == pclClient->pclItemManager->CanMakableItem( siItemUnique ) ) continue ;	// 만들지 못하는 아이템은 건너뛴다
		// 주어진 타입과 같은 아이템만 열거한다. 
		// 사용자에게 보여지는 아이템인 경우에만. 
		if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST) || !pinfo->IsItemInfoAtb( ITEMINFOATB_NOTTRADE ) )
		{
			if( ChekMakeItem(pinfo) == true )	return true	;
		}
		else{	return false; }
	}
	return false ;
}
bool CNManufactureDlg_New::ChekMakeItem( cltItemInfo* pinfo )	// 이 아이템을 만들수 있는 아이템인가 ?
{
	cltClient *pclclient = (cltClient *)pclClient;

	cltCharClient* pCharClient = pclClient->pclCM->GetCharClient(1)	;	if(pCharClient == NULL ){	return	false ;}

	SI32 siSkillUnique = pinfo->clSkillInfo.siSkill;		// 이 아이템을 만들때 필요한 유니크
	SI32 siSkillLevel = pinfo->clSkillInfo.siSkillLevel;	// 이 아이템을 만들때 필요한 스킬레벨 
	SI32 siCharTotalLevel = pCharClient->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique , 0 , 0, &pCharClient->pclCI->clCharItem.clItem[0] )	;
	
	if( siSkillLevel > siCharTotalLevel ){	return false	;	}

	for(SI32 i = 0;i < MAX_TOOL_NUMBER;i++)
	{
		SI32 siToolUnique = pinfo->clProductionInfo.clTool[i].siUnique ;
		if ( siToolUnique <= 0 )	continue	;
		SI32 ToolItemPos = pCharClient->pclCI->clCharItem.FindItemInv( siToolUnique )	;	
		if ( ToolItemPos < 0 )	return false	;	// 해당 아이템 을 만들수 있는 도구가 부족 하다 .만들수 없다 
	}
	return true	;// 도구가 다 있다 
}
bool CNManufactureDlg_New::CheckMakeData_Maker()
{
	cltClient *pclclient = (cltClient *)pclClient;

	// 데이터 체크
	// 유니크 체크
	SI16 siItemUnique	=	m_clShopData.m_clItem[0].siUnique;
	SI16 siMakeItemNum	=	m_clShopData.m_clItem[0].siItemNum;
	SI16 siMakeItemPrice	=	m_clShopData.m_siPrice[0];
	SI16 siLuckPoint		=	m_clShopData.m_siHandPoint;
	SI16 siSkillPoint		=	m_clShopData.m_siSkillLevel;
	if ( siItemUnique <= 0 )	return false ;

	// 아이템 갯수 체크
	if(siMakeItemNum <=0 )
	{
		TCHAR* pText = GetTxtFromMgr(1668);
		DisplayMessage(FALSE, pText);
		return false;
	}
	CComboBox* pGoods_Name_ComboBox = m_InterfaceFactory.GetComboBox( NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME )	;
	if(pGoods_Name_ComboBox != NULL)
	{
		SI32 siSelectindex = pGoods_Name_ComboBox->GetSelectedIndex();		if(siSelectindex < 0 ) return	false ;
		TCHAR* szSelectText = pGoods_Name_ComboBox->GetText( siSelectindex );
		if(szSelectText == NULL )	return false	;
	}
	else
	{
		return	false	;
	}
	// 가격 체크
	if(siMakeItemPrice < 0)
	{
		TCHAR* pText = GetTxtFromMgr(2222);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !pText ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		return false;
	}
	if(siMakeItemPrice > MAX_FEE)
	{
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(2223);
		StringCchPrintf(buffer, 256, pText,MAX_FEE);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !buffer ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		return false;
	}
	// 행운 체크
	if(siLuckPoint < 0)
	{
		TCHAR* pText = GetTxtFromMgr(9865);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !pText ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		return false;
	}
	if(siLuckPoint > 2000)
	{
		TCHAR buffer[256];
		TCHAR* pText = NULL;
		pText = GetTxtFromMgr(9866);
		StringCchPrintf(buffer, 256, pText, 2000);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !buffer )return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		return false;
	}
	// 스킬레벨 체크
	if(siSkillPoint < 0)
	{
		TCHAR* pText = GetTxtFromMgr(2226);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !pText ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		return false;
	}
	if(siSkillPoint > MAX_SKILL_LEVEL+30)
	{
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(2227);
		StringCchPrintf(buffer, 256, pText,MAX_SKILL_LEVEL+30);

		TCHAR * pTitle = GetTxtFromMgr(512) ;
		if ( !pTitle || !buffer ) return false;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		return false;
	}
	return true	;
}
void CNManufactureDlg_New::Set_Manufacture_Mode()
{
	m_bClient	=	FALSE 	;
	// 제작자 셋팅을 한다.
	CEdit* pSkillLevel_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_SKILLLEVEL )	;									if(pSkillLevel_EditBox == NULL )	return	;
	CEdit* pLuckStatLevel_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_LUCKLEVEL )	;								if(pLuckStatLevel_EditBox == NULL )	return	;
	CEdit* pItemNum_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_GOODS_NUM )	;										if(pItemNum_EditBox == NULL )		return	;
	CEdit* pFee_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_FEE_NUM )	;											if(pFee_EditBox == NULL )			return	;
	CEdit* pTotalFee_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_FEE_TOTAL )	;									if(pTotalFee_EditBox == NULL )			return	;
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOOD_EXPLAIN)->Clear()	;
	
	pSkillLevel_EditBox->Clear()	;
	pLuckStatLevel_EditBox->Clear()	;
	pItemNum_EditBox->Clear()	;
	pFee_EditBox->Clear()	;
	pTotalFee_EditBox->Clear()	;

	SetComboBox_ItemCategory();	// 만들수 있는 아이템을 콤보박스에 넣는다 .

	pSkillLevel_EditBox->SetReadOnly(true)	;
	pLuckStatLevel_EditBox->SetReadOnly(true)	;	
	pSkillLevel_EditBox->Enable( false )	;	
	pLuckStatLevel_EditBox->Enable( false )	;
	pItemNum_EditBox->Show(false);
}
void CNManufactureDlg_New::Set_Client_Mode()
{
	m_bClient	=	TRUE 	;
	// 제작자 셋팅을 한다.
	CEdit* pSkillLevel_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_SKILLLEVEL )	;									if(pSkillLevel_EditBox == NULL )	return	;
	CEdit* pLuckStatLevel_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_LUCKLEVEL )	;								if(pLuckStatLevel_EditBox == NULL )	return	;
	CEdit* pItemNum_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_GOODS_NUM )	;										if(pItemNum_EditBox == NULL )		return	;
	CEdit* pFee_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_FEE_NUM )	;											if(pFee_EditBox == NULL )			return	;
	CEdit* pTotalFee_EditBox = m_InterfaceFactory.GetEdit( NMANUFACTURE_NEW_EDIT_FEE_TOTAL )	;									if(pTotalFee_EditBox == NULL )			return	;
	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOOD_EXPLAIN)->Clear()	;

	pSkillLevel_EditBox->Clear()	;
	pLuckStatLevel_EditBox->Clear()	;
	pItemNum_EditBox->Clear()	;
	pFee_EditBox->Clear()	;
	pTotalFee_EditBox->Clear()	;

	SetMakeableItemList();	// 만들수 있는 아이템을 콤보박스에 넣는다 .

	pSkillLevel_EditBox->SetReadOnly(false)	;
	pLuckStatLevel_EditBox->SetReadOnly(false)	;
	pSkillLevel_EditBox->Enable( true )	;	
	pLuckStatLevel_EditBox->Enable( true )	;
	pItemNum_EditBox->Show(true);
}
void CNManufactureDlg_New::MakeItemDataSet(SI32 siItemUnique)
{
	if ( siItemUnique > 0 )	// 콤보 박스에서 아이템을 선택 했다.
	{
		//----------------------------------------
		// 아이템의 기타 정보를 표시한다 
		//----------------------------------------
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(siItemUnique);
		if ( ref )
		{
			// 만들 아이템 유니크를 설정 한다 .
			m_clShopData.m_clItem[0].siUnique		= siItemUnique;	// 만들 데이터에 유니크를 넣는다 .

			SI32 siAmountSuccess= 0, siLuckSuccess= 0,   siSkillSuccess = 0;
			SI32 siAmountValue = 0, siLuckValue = 0,   siSkillValue = 0;
			GMONEY siPriceSuccess = 0	, siPriceValue = 0;
			TCHAR AmountBuffer[256]=TEXT(""), PriceBuffer[256]=TEXT(""),  LuckBuffer[256]=TEXT(""),   SkillBuffer[256]=TEXT("");
			TCHAR *pTmpChar1 = NULL , *pTmpChar2= NULL , *pTmpChar3= NULL , *pTmpChar4= NULL ;
			TCHAR itemname[128];

			pTmpChar1 =	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->GetText();
			pTmpChar2 =	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_FEE_NUM)->GetText();
			pTmpChar3 =	m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->GetText();
			pTmpChar4 = m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->GetText();

			if(pTmpChar1 != NULL )
			{
				StringCchCopy(AmountBuffer,sizeof(AmountBuffer), pTmpChar1 )	;
				//_tcscpy( AmountBuffer, pTmpChar1 );
				siAmountSuccess =  _tcslen( AmountBuffer );
				siAmountValue = _tstoi(AmountBuffer);
				if ( siAmountValue > 0 && siAmountValue <= MAX_ITEM_PILE_NUMBER ) 
				{
					m_clShopData.m_clItem[0].siItemNum		= (SI32)siAmountValue;
				}
			}
			if(pTmpChar2 != NULL )
			{
				StringCchCopy(PriceBuffer,sizeof(PriceBuffer), pTmpChar2 )	;
				//_tcscpy( PriceBuffer, pTmpChar2 );
				siPriceSuccess = _tcslen( PriceBuffer );
				siPriceValue  = _tstoi64(PriceBuffer);
				if ( siPriceValue > 0 && siPriceValue <= MAX_FEE ) 
				{
					m_clShopData.m_siPrice[0]				= (GMONEY)siPriceValue;
				}
			}
			if(pTmpChar3 != NULL )
			{
				StringCchCopy(LuckBuffer,sizeof(LuckBuffer), pTmpChar3 )	;
				//_tcscpy( LuckBuffer, pTmpChar3 );
				siLuckSuccess = _tcslen( LuckBuffer );
				siLuckValue  = _tstoi(LuckBuffer);
				if(siLuckValue >= 0 && siLuckValue <= 2000)
				{
					m_clShopData.m_siHandPoint				= siLuckValue;
				}					
			}
			if(pTmpChar4 != NULL )
			{
				StringCchCopy(SkillBuffer,sizeof(SkillBuffer), pTmpChar4 )	;
				//_tcscpy( SkillBuffer, pTmpChar4 );
				siSkillSuccess = _tcslen( SkillBuffer );
				// 스킬레벨 
				siSkillValue  = _tstoi(SkillBuffer);
				if(siSkillValue > 0 && siSkillValue <= MAX_SKILL_LEVEL+30 )
				{
					m_clShopData.m_siSkillLevel				= siSkillValue;
				}
			}
			m_clShopData.m_siSelectedInventoryPos[0]= 0;				
			if(m_clShopData.m_clItem[ 0 ].GetFullName(pclClient->pclItemManager, itemname, 128) == true)			
			{
				NTCHARString1024 szExplain = GetTxtFromMgr(9863);
				szExplain.Replace( "#item#", itemname );
				szExplain.Replace( "#num#", SI32ToString(siAmountValue) );
				szExplain.Replace( "#money#", SI64ToString(siPriceValue) );
				szExplain.Replace( "#hand#", SI32ToString(siLuckValue) );
				szExplain.Replace( "#skill#", SI32ToString(siSkillValue) );
				DisplayMessage(TRUE, szExplain);

				// EDIT에 배너정보를 표시한다.
				cltShopBannerData clShopBanner;
				if ( m_bClient == true ) 
				{
					m_clShopData.m_siShopMode = PSHOP_MODE_MAKEITEM	;
				}
				else
				{
					m_clShopData.m_siShopMode = PSHOP_MODE_MAKEITEM_WANT	;
				}
				clShopBanner.SetData(&m_clShopData);
				clShopBanner.MakeBannerText(pclClient->pclItemManager, m_szShopBanner);
				// Edit 텍스트 개수 제한!!
				m_szShopBanner[ MAX_PERSONAL_SHOP_TEXT_SIZE - 1] = '\0';
				m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOOD_EXPLAIN)->SetText(m_szShopBanner);

				// 이미지 설정
				SetItemImageData( m_clShopData.m_clItem[0].siUnique);

			}
		}
	}
}
void CNManufactureDlg_New::SetEditBox()
{
	if( true == m_bClient )	// 클라이언트면 
	{
		m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS)->Enable( false )	;

		TCHAR szSelText[128];
		TCHAR *pITemName = NULL;
		TCHAR Nbuffer[512]=TEXT(" ");
		SI16 siTmpIndex = -1;
		szSelText[0] = '\0';

		siTmpIndex = m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->GetSelectedIndex();		if(siTmpIndex < 0 ) return	;
		pITemName = m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->GetText( siTmpIndex );
		// 선택을 했을 경우
		if(pITemName != NULL )
		{
			StringCchCopy(szSelText,sizeof(szSelText), pITemName )	;
			//_tcscpy( szSelText, pITemName );
			TCHAR *pTemp1 = _tcsrchr( szSelText, _T('-') );	 //뒤에서부터 '-' 문자를 검색함
			TCHAR* pText = GetTxtFromMgr(2219);
			TCHAR *pTemp2 = _tcsrchr( szSelText, _T('-') );
			if(pTemp1 == NULL || pTemp2 == NULL) return	;
			*pTemp1 = '\0';	pTemp1++;
			*pTemp2 = '\0';
			SI32 siMaxMakeItem = _tstoi(pTemp1);
			if(siMaxMakeItem <= 0)				{ return ;   }
			// 이름을 토대로 아이템을 가지고 온다.
			SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(szSelText);
			MakeItemDataSet(itemunique );
		}
	}
	else	// 제작자면 
	{	
		m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS)->Enable( true )	;

		SI16 siTmpIndex = m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->GetSelectedIndex();		if(siTmpIndex < 0 ) return	;
		TCHAR* pITemName = m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->GetText( siTmpIndex );
		// 선택을 했을 경우
		if(pITemName != NULL )
		{
			// 이름을 토대로 아이템을 가지고 온다.
			SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(pITemName);
			MakeItemDataSet(itemunique );
		}
	}

}

void CNManufactureDlg_New::RefreshShowData()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if(m_clShopData.m_clItem[0].siItemNum <= 0 )
	{
		DeleteDialog();
	}
	else
	{
		// 중간에 정보 세팅
		TCHAR strBuffer[256] = {'\0',};
		// 콤보 박스에 만드는 아이템 이름을 넣는다 
		m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->Enable(false);
		TCHAR* szitemname = (TCHAR*)pclClient->pclItemManager->GetName(m_clShopData.m_clItem[0].siUnique)	;
		if ( szitemname != NULL) 
		{
			m_InterfaceFactory.GetComboBox(NMANUFACTURE_NEW_COMBOBOX_GOODS_NAME)->SetText(  szitemname )	;
		}
		StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_siHandPoint );
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_LUCKLEVEL)->SetText(strBuffer);

		StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_siSkillLevel );
		m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_SKILLLEVEL)->SetText(strBuffer);

		SI64 atb = pclClient->pclItemManager->GetItemAtb(m_clShopData.m_clItem[0].siUnique);
		if(atb & ITEMATB_OBJECT)
		{
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetText(TEXT("1"));
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetReadOnly(true);            			
		}
		else
		{
			m_InterfaceFactory.GetEdit(NMANUFACTURE_NEW_EDIT_GOODS_NUM)->SetText(SI32ToString(m_clShopData.m_clItem[0].siItemNum));
		}

		SetItemImageData( m_clShopData.m_clItem[0].siUnique);
	}

	

}
bool CNManufactureDlg_New::CheckItemType( SI32 siType )
{
	switch( siType ) 
	{
	case ITEMTYPE_DOLL :
	case ITEMTYPE_ANIMAL :
	case ITEMTYPE_PREMIUM :
	case ITEMTYPE_EVENTITEM :
	case ITEMTYPE_SCROLL :
	case ITEMTYPE_QUEST :
	case ITEMTYPE_CANNON :
	case ITEMTYPE_TORPEDO:
	case ITEMTYPE_MINERAL:
	case ITEMTYPE_TREE:
	case ITEMTYPE_ETC:
		{
			return false	;
		}
		
	}
	return true	;

}