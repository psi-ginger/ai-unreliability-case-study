#include "NButtonMenuDlg.h"

#include "../../CommonLogic/Msg/MsgType-PrivateTrade.h"
#include "../../CommonLogic/Msg/MsgType-Party.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"
#include "../../CommonLogic/Msg/MsgType-PersonalShop.h"
#include "../../CommonLogic/Msg/MsgType-Web.h"
#include "../../CommonLogic/Msg/MsgType-Friend.h"

#include "../Client/NInterface/NPartyDlg/NPartyDlg.h"
#include "../Client/NInterface/NFamilyInfoDlg/NFamilyInfoDlg.h"
#include "../Client/NInterface/NCommonMsgDlg/NCommonMsgDlg.h"

#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../Common/Char/CharCommon/Char-Common.h"
#include "../../Common/PrivateTradeOrderMgr/PrivateTradeOrderBaseInfo.h"
#include "Char\CharManager\CharManager.h"
#include "../../Client/client.h"
#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;
extern SI16 g_siChannelingCode;

CNButtonMenuDlg::CNButtonMenuDlg()
 : m_pkImageStatic_Name(NULL),
   m_pkImageStatic_Back(NULL),
   m_pkStatic_Name(NULL)
{
	ZeroMemory(m_pkButton_Command, sizeof(m_pkButton_Command));
}

CNButtonMenuDlg::~CNButtonMenuDlg()
{
	SAFE_DELETE(m_pkImageStatic_Name);
	SAFE_DELETE(m_pkImageStatic_Back);
	SAFE_DELETE(m_pkStatic_Name);
}

void CNButtonMenuDlg::Create()
{
	SI32	i					= 0;
	SI32	siButtonIndex		= 0;
	SI32	siButtonControlID	= 0;
	SI32	siYPos				= 0;

	if ( IsCreate() )
	{
		DeleteDialog();
	}
	else
	{
		SI32 siWidth	=	120	;
		SI32 siHieght	=	20	;
		SI32 siTileNum	=	0	;
		CDialogController::Create( NBUTTONMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNButtonMenuDlg, NULL, NULL, 0, false, 0, 0, 0, siWidth, 24 + siHieght, false, false, false );
		siYPos				= 19;
		siButtonControlID	= NButtonMenuDlg_DIALOG_BUTTON_COMMAND0;

		m_pkStatic_Name = new CStatic( this );
		m_pkStatic_Name->Create(NButtonMenuDlg_DIALOG_STATIC_CHARNAME, "", NULL, 0, true, 0, 0, 0, siWidth, 20, siWidth, 20);
		m_pkStatic_Name->SetTextPrintOption(DT_TOP | DT_CENTER);
		m_pkImageStatic_Name = new CImageStatic( this );
		m_pkImageStatic_Name->Create(NButtonMenuDlg_DIALOG_IMAGESTATIC_NAME, "NINTERFACE/IMAGE/IMAGE_12X12_02_000.SPR", 0, true, 9, 0, 0, siWidth, siHieght, siWidth, siHieght );


		//-- 버튼 생성 

		TCHAR* pText = GetTxtFromMgr(30080)	;
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_120X20_00_000.SPR", 0, false, siTileNum, 0, siYPos, siWidth, siHieght, siWidth, siHieght);
		siButtonIndex++;	siButtonControlID++;	siYPos += 20;
		pText = GetTxtFromMgr(1418)	;
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_120X20_00_000.spr", 0, false, siTileNum, 0, siYPos, siWidth, siHieght, siWidth, siHieght);
		siButtonIndex++;	siButtonControlID++;	siYPos += 20;
		pText = GetTxtFromMgr(4637)	;
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_120X20_00_000.spr", 0, false, siTileNum, 0, siYPos, siWidth, siHieght, siWidth, siHieght);
		siButtonIndex++;	siButtonControlID++;	siYPos += 20;
		pText = GetTxtFromMgr(30119)	;
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_120X20_00_000.spr", 0, false, siTileNum, 0, siYPos, siWidth, siHieght, siWidth, siHieght);
		siButtonIndex++;	siButtonControlID++;	siYPos += 20;
		pText = GetTxtFromMgr(30120)	;
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_120X20_00_000.spr", 0, false, siTileNum, 0, siYPos, siWidth, siHieght, siWidth, siHieght);
		siButtonIndex++;	siButtonControlID++;	siYPos += 20;
		m_pkImageStatic_Back = new CImageStatic( this );
		m_pkImageStatic_Back->Create(NButtonMenuDlg_DIALOG_IMAGESTATIC_BACK, "NINTERFACE/IMAGE/IMAGE_8X8_00_000.SPR",  0, true, 5, 0, 15, siWidth, siHieght, siWidth, siHieght );

		// 기본 셋팅 
		m_pkButton_Command[0].SetUse(false);	// 가족탈퇴
		m_pkButton_Command[1].SetUse(false);	// 순간이동
		m_pkButton_Command[2].SetUse(false);	// 귓속말하기
		m_pkButton_Command[3].SetUse(false);	// 가족효과연장
		m_pkButton_Command[4].SetUse(false);	// 가족에서제외

	}

	return;
}

void CNButtonMenuDlg::Action()
{
	cltClient * pclclient = (cltClient*)pclClient;

	TCHAR * pTextCharName = (TCHAR*)m_pkStatic_Name->GetText();

	//if( pTextCharName == NULL || pTextCharName[0] == '\0' )
	{
		// 이름 대신 해당 자식의 남은 기간을 보여준다.

		cltCharCommon* pChar  = pclClient->pclCM->GetChar(1)	;
		if(pChar != NULL	)
		{
			for ( SI32 siSlot = 0 ; siSlot<MAX_CHILDREN_NUM; siSlot++)
			{
				 if( m_siTargetPeronId == pChar->pclFamily->GetChildPersonID( siSlot) )
				 {
					 SIZE sizeString = {0, };
					  SYSTEMTIME stTime = pChar->pclFamily->GetChild_Add_Date( siSlot )	;

					  NTCHARString128 strText = GetTxtFromMgr( 30213 )	;

					  strText.Replace(TEXT("#year#"),SI32ToString(stTime.wYear));
					  strText.Replace(TEXT("#Month#"),SI32ToString(stTime.wMonth));
					  strText.Replace(TEXT("#day#"),SI32ToString(stTime.wDay));

					  m_pkStatic_Name->SetText( strText, DT_CENTER | DT_VCENTER | DT_SINGLELINE, &sizeString);

					  CalcWidth( sizeString );

				 }
			}
		}
	}
}

void CALLBACK CNButtonMenuDlg::StaticCallBackDialogNButtonMenuDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNButtonMenuDlg *pThis = (CNButtonMenuDlg*)pControllerMgr;
	pThis->CallBackDialogNButtonMenuDlg( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNButtonMenuDlg::CallBackDialogNButtonMenuDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch(nControlID)
			{
			case NButtonMenuDlg_DIALOG_BUTTON_COMMAND0:// 가족탈퇴
				{
					// 메시지 창 띄우기
					if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
					{
						pclclient->CreateInterface( NCOMMONMSG_DLG );
					}
					NTCHARString256	kText( GetTxtFromMgr(30217) );

					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_REQUEST_FAMILY_DELL_CHILDREN);	
					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetText(kText);
					cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 )	;
					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetParentPersonID(pclchar->pclCI->GetPersonID());	
					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetShowTimeSec( 10 );
					// 이건 이혼기능 자식이 있으면 삭제 불가능
					DeleteDialog();
				}
				break;
			case NButtonMenuDlg_DIALOG_BUTTON_COMMAND1:// 순간이동
				{
					// 해당 케릭터로 순간 이동
					if ( pclclient->m_pDialog[ NFAMILYINFO_DLG ] != NULL )
					{
						if(pclclient->m_pDialog[ NFAMILYINFO_DLG ]->IsCreate()==true)
						{
							((CNFamilyInfoDlg *)pclclient->m_pDialog[ NFAMILYINFO_DLG ])->WarpFamily( m_siTargetPeronId, m_szTargetName)	;	// 배우자에게 날라간다.

						}

					}
					DeleteDialog();
				}
				break;
			case NButtonMenuDlg_DIALOG_BUTTON_COMMAND2:// 귓속말하기
				{
					if ( m_szTargetName == NULL )	return	;
					pclclient->RequestSecretChatByCharName(m_szTargetName);
					DeleteDialog();
				}
				break;
			case NButtonMenuDlg_DIALOG_BUTTON_COMMAND3:// 가족효과연장
				{
					// 메시지 창 띄우기
					if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
					{
						pclclient->CreateInterface( NCOMMONMSG_DLG );
					}
					cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 )	;
					NTCHARString256	kText(GetTxtFromMgr(30139));
					kText.Replace( TEXT("#name#"),m_szTargetName);
					kText.Replace( TEXT("#item#"),pclClient->pclItemManager->GetName(ITEMUNIQUE(24122)));

					SI32 siChildPersonID = pclchar->pclFamily->GetChildPersonIDfromName( m_szTargetName )	;

					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_REQUEST_FAMILY_PLUS_FAMILYEFFECTDATE);	
					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetText(kText);					
					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetParentPersonID(siChildPersonID);	
					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetShowTimeSec( 10 );

					DeleteDialog();
				}
				break;
			case NButtonMenuDlg_DIALOG_BUTTON_COMMAND4:// 가족에서제외
				{
					// 메시지 창 띄우기
					if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
					{
						pclclient->CreateInterface( NCOMMONMSG_DLG );
					}
					cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 )	;
					SI32 siChildPersonID = pclchar->pclFamily->GetChildPersonIDfromName( m_szTargetName )	;
					if ( siChildPersonID == 0 )
					{
						siChildPersonID = pclchar->pclFamily->GetParentPersonIDfromName( m_szTargetName )	;
					}

					NTCHARString256	kText1(GetTxtFromMgr(30216));
					kText1.Replace( TEXT("#name#"),m_szTargetName);

					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_REQUEST_FAMILY_REMOVE_TARGET_CHILDREN);	
					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetText(kText1);
					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetParentPersonID(siChildPersonID);	
					((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetShowTimeSec( 10 );
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}

void CNButtonMenuDlg::Set( SI32 Roc_X, SI32 Roc_Y, SI16 Mode, SI32 width, SI32 Height , SI32 siTargetPeronId,TCHAR* szTargrtCharName )	// 일단 여기서는 메뉴만 보이도록 하자. 필요한 정보도 넘겨 줘야 한다.
{
	cltClient* pclclient = (cltClient*)pclClient;

	m_pkButton_Command[0].SetUse(false);	// 가족탈퇴
	m_pkButton_Command[1].SetUse(false);	// 순간이동
	m_pkButton_Command[2].SetUse(false);	// 귓속말하기
	m_pkButton_Command[3].SetUse(false);	// 가족효과연장
	m_pkButton_Command[4].SetUse(false);	// 가족에서제외
	
	m_siTargetPeronId	=	siTargetPeronId	;
	m_szTargetName		=	szTargrtCharName	;

	switch ( Mode)
	{
	case CLICK_MODE_PARENT_ME :	// 부모이고 내 자신을 찍었을때
		{
			// 버튼 배열 설정
			m_pkButton_Command[0].SetUse(true);		// 가족탈퇴
		}
		break;
	case CLICK_MODE_PARENT_MATE :	// 부모이고 배우자를 찍었을때
		{
			// 버튼 배열 설정
			
			m_pkButton_Command[1].SetUse(true);	// 순간이동
			m_pkButton_Command[2].SetUse(true);	// 귓속말하기			
		}
		break;
	case CLICK_MODE_PARENT_CHILDREN :	// 부모이고 자식을 찍었을때
		{
			// 버튼 배열 설정
			m_pkButton_Command[2].SetUse(true);	// 귓속말하기
			m_pkButton_Command[3].SetUse(true);	// 가족효과연장
			m_pkButton_Command[4].SetUse(true);	// 가족에서제외
		}
		break;
	case CLICK_MODE_CHILDREN_ME :	// 자식이고 나를 찍었을때
		{
			// 버튼 배열 설정
			m_pkButton_Command[0].SetUse(true);		// 가족탈퇴
		}
		break;
	case CLICK_MODE_CHILDREN_OTHER :	// 자식이고 다른 자식을 찍었을때
		{
			// 버튼 배열 설정
			m_pkButton_Command[2].SetUse(true);	// 귓속말하기
		}
		break;
	case CLICK_MODE_PARENT_NONE :	// 내가 부모이고 빈공간을 찍었을때
		{
			if ( pclclient->m_pDialog[ NFAMILY_DLG_ADD_CHILDREN ] == NULL )
			{
				pclclient->CreateInterface( NFAMILY_DLG_ADD_CHILDREN );
			}
		}
		break;
	case CLICK_MODE_CHILDREN_PARENT:
		{
			m_pkButton_Command[2].SetUse(true);	// 순간이동
		}
	}
	SetPosDlg(Roc_X , Roc_Y , width, Height) ;
}

void CNButtonMenuDlg::ClearFocus()
{
	CDialogController::ClearFocus();
	DeleteDialog();

	return;
}

void CNButtonMenuDlg::SetFocus( bool Focus )
{
	CDialogController::SetFocus( Focus );

	if ( ! Focus )
	{
		cltClient * pclclient = (cltClient*) pclClient;

		if( pclclient->m_pDialog[NBUTTONMENU_DLG] &&  pclclient->m_pDialog[NBUTTONMENU_DLG]->IsShow())
		{
			pclclient->m_pDialog[NBUTTONMENU_DLG]->Show(false);

		}

		DeleteDialog();
	}

	return;
}

void CNButtonMenuDlg::CalcWidth( SIZE sizeString )
{
	// <<4 >>4연산은 8의 배수를 만들기 위해, 8은 보정값, 16은 양쪽 백그라운드 8픽셀씩 여유값
	SI16 siCalcWidth = ((sizeString.cx >> 3) << 3) +8 +16;
	// 기본 크기보다 작다면 무조건 기본 크기로 변경
	if (BUTTONMENU_BASE_WIDTH_SIZE > siCalcWidth )
	{
		siCalcWidth = BUTTONMENU_BASE_WIDTH_SIZE;
	}

	// 원래 설정한 크기와 계산된 크기가 같다면 리턴
	if ( m_pkStatic_Name->GetWidth() == siCalcWidth )
	{
		return;
	}

	m_pkStatic_Name->SetSurfaceWidth( siCalcWidth );
	m_pkStatic_Name->SetWidth( siCalcWidth );

	m_pkImageStatic_Name->SetSurfaceWidth( siCalcWidth );
	m_pkImageStatic_Name->SetWidth( siCalcWidth );

	// 위치 조정
	SI16 siWidth	= ((siCalcWidth - m_pkImageStatic_Back->GetWidth()) / 2);
	SI16 siX		= m_pkImageStatic_Back->GetX() - siWidth;

	m_pkStatic_Name->SetX( siX );
	m_pkImageStatic_Name->SetX( siX );
}
void CNButtonMenuDlg::SetPosDlg( SI32 siRoc_X, SI32 siRoc_Y ,SI32 width, SI32 Height )
{
	SI32	i					= 0;
	SI32	siViewButtonCount	= 0;
	SI32	siWidth				= width ;
	SI32	siHeight			= Height;
	SI32 siYPos				= 19;

	// 크기 설정 및 위치 재정리
	for(i=0; i<BUTTONMENU_MAX_BUTTON_COUNT; i++)
	{
		CButton* pkButton = m_pkButton_Command[i].m_pkButton_Command;
		if(pkButton == NULL)						continue;
		if(m_pkButton_Command[i].m_bUse == false)	continue;

		siViewButtonCount++;
		pkButton->SetControllerPos(pkButton->GetX(), siYPos, pkButton->GetWidth(), pkButton->GetHeight(), pkButton->GetWidth(), pkButton->GetHeight());
		siYPos += 20;
	}

	siHeight = 16 + siViewButtonCount * 16;

	m_pkImageStatic_Back->SetControllerPos( m_pkImageStatic_Back->GetX(), m_pkImageStatic_Back->GetY()+2,
		m_pkImageStatic_Back->GetWidth(), siViewButtonCount * 16 + 8,
		m_pkImageStatic_Back->GetWidth(), siViewButtonCount * 16 + 8 );

	SetDialogPos( siRoc_X+siWidth, siRoc_Y, siWidth, siHeight );
	SetTimerOnOff(TRUE);
	SetActionTimer(100);

	Show(true);
}