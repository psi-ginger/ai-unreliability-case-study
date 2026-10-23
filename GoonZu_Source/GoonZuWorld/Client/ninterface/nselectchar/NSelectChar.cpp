
#include "../Client/NInterface/NSelectChar/NSelectChar.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../Client/NInterface/NLogin/NLogin.h"

#include "..\..\resource.h"
#include "..\..\Common\CommonHeader.h"
#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"

#include "../Client/Interface/RecommendPersonDlg/RecommendPersonDlg.h"
#include "../Client/Interface/ResidenceCheck/ResidenceCheck.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../Client/Interface/LoginClient/LoginClient.h"
#include "../Client/NInterface/NMakeChar/NMakeChar.h"
#include "../Client/NInterface/NRecommendPersonDlg/NRecommendPersonDlg.h"
#include "../Client/NInterface/NDeleteCharDlg/NDeleteCharDlg.h"
#include "../common/Char/ccd/CharacterDrawing.h"
#include "../Common/Char/CharManager/CharManager.h"
#include "DLGPositionSave/DLGPositionSave.h"
#include "../Client/NInterface/NChangeCharName/NChangeCharName.h"

#include "../lib/WebHTML/WebHTML.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;
extern DWORD dwQuitTimer;

void CN3DCharDraw::Draw()
{
	if( m_pkBack )					GP.PutSpr(m_pkBack, m_siX, m_siY, 0);

	if( m_pkImage && m_pkAura )
	{
		SI32 siX, siY;
		siX = m_siX;
		siX += (m_pkAura->GetXSize() - m_pkImage->GetXSize()) / 2 + 4;

		siY = m_siY;
		siY += (m_pkAura->GetYSize() - m_pkImage->GetYSize()) / 2 + 28;

		GP.PutSprAlpha(m_pkImage, siX, siY, m_siAlphaValue, m_siFontIndex);
	}

	if(m_pkImage && m_pkBack)
	{
		SI32 siX, siY;
		siX = m_siX;
		siX += (m_pkBack->GetXSize() - m_pkImage->GetXSize()) / 2;

		siY = m_siY;
		siY += (m_pkBack->GetYSize() - m_pkImage->GetYSize()) / 2;

		GP.PutSprAlpha(m_pkImage, siX, siY, m_siAlphaValue, m_siFontIndex);
	}

	if(m_bDrawAura && m_pkAura)		GP.PutSprScreen(m_pkAura, m_siX, m_siY, m_siAruaFontIndex);
	if(m_bDrawMask && m_pkBack)		GP.PutSprAlpha(m_pkBack, m_siX, m_siY, 8, 0);
}

CNSelectCharDlg::CNSelectCharDlg() : CDialogController()
{
	m_siHeartBeatCount = 0;
	
	for(SI32 i=0; i<MAX_CHAR_PER_USER; i++)
	{
		m_pButtonCancelDel[i]	= NULL;			// �������
		m_pImageStaticdeldateBack[i]	= NULL;	// ĳ���� �������� ��¥�� ���϶� ���
		m_pEditboxDeldate[i]	= NULL;			// �Ⱓ�� ���� ����Ʈ�ڽ�
		m_pButtonCharSlot[i]	= NULL;
		m_pImageStaticSlot[i]	= NULL;
		m_pImageStaticChar[i]	= NULL;
		m_pEditboxwaitdel[i]	= NULL;	//������������
		
	}

	m_pButtonSelect			= NULL;
	m_pButtonMake			= NULL;
	m_pButtonDelete			= NULL;
	m_pButtonCancelDelete	= NULL;
	m_pButtonReturn			= NULL;
}

CNSelectCharDlg::~CNSelectCharDlg()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL != pclclient )
	{
		if ( NULL != pclclient->m_pDialog[NCHARHOUSE_DLG] )
		{
			pclclient->DestroyInterface( pclclient->m_pDialog[NCHARHOUSE_DLG] );
		}

		if(pclclient->m_pDialog[NCHANGECHARNAME_DLG] != NULL)
		{
			pclclient->DestroyInterface(pclclient->m_pDialog[NCHANGECHARNAME_DLG]);
		}
	}

	GP.FreeSpr(sprLoginChar[0]);
	GP.FreeSpr(sprLoginChar[1]);
	GP.FreeSpr(sprLoginChar[2]);
	GP.FreeSpr(sprLoginChar[3]);

	for(SI32 i=0; i<MAX_CHAR_PER_USER; i++)
	{
		SAFE_DELETE(m_pButtonCancelDel[i]);
		SAFE_DELETE(m_pImageStaticdeldateBack[i]);
		SAFE_DELETE(m_pEditboxDeldate[i]);	
		SAFE_DELETE(m_pButtonCharSlot[i]);
		SAFE_DELETE(m_pImageStaticSlot[i]);
		SAFE_DELETE(m_pImageStaticChar[i]);
		SAFE_DELETE(m_pEditboxwaitdel[i]);

	
	}
	
	SAFE_DELETE(m_pButtonSelect);
	SAFE_DELETE(m_pButtonMake);
	SAFE_DELETE(m_pButtonDelete);
	SAFE_DELETE(m_pButtonCancelDelete);
	SAFE_DELETE(m_pButtonReturn);

	SetTimerOnOff( false );
}

void CNSelectCharDlg::Create()
{
	CInterfaceFile file;
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 i;

	if(gsiResolution == 4)
	{
		file.LoadFile(TEXT("NInterface/Data/DLG_NSelectChar_1024x600.ddf"));
	}
	else
	{
		file.LoadFile(TEXT("NInterface/Data/DLG_NSelectChar.ddf"));
	}
	file.CreatePopupDialog( this, NSELECTCHAR_DLG, TEXT("dialog_NSelectChar"), StaticCallBackDialogNSelectCharDlg,false );


	if( pclClient->siServiceArea == ConstServiceArea_Japan
		|| pclClient->siServiceArea == ConstServiceArea_China
		|| pclClient->siServiceArea == ConstServiceArea_Korea
		|| pclClient->siServiceArea == ConstServiceArea_Taiwan
		|| pclClient->siServiceArea == ConstServiceArea_USA
		|| pclClient->siServiceArea == ConstServiceArea_English
		|| pclClient->siServiceArea == ConstServiceArea_NHNChina
		|| pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		if (!g_bFullScreen)			MovePos( 50 + 48, -8 );
		else						MovePos( 50 + 80, 0 + 40);
	}
	else
	{
		if( g_bFullScreen)			MovePos( 80 , 40 );
		else						MovePos( 55 , 30 );
	}
 
	for(i=0; i<MAX_CHAR_PER_USER; i++)
	{
		m_pButtonCancelDel[i]	= new CButton( this );
		m_pImageStaticdeldateBack[i]	= new CImageStatic( this );
		m_pEditboxDeldate[i]	= new CEdit(this);
		m_pButtonCharSlot[i]	= new CButton( this );
		m_pImageStaticSlot[i]	= new CImageStatic( this );
		m_pImageStaticChar[i]	= new CImageStatic(this);
		m_pEditboxwaitdel[i]	= new CEdit(this);
				
	}

	m_pButtonSelect			= new CButton( this );
	m_pButtonMake			= new CButton( this );
	m_pButtonDelete			= new CButton( this );
	m_pButtonCancelDelete	= new CButton( this );
	m_pButtonReturn			= new CButton( this );


	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Japan:
	case ConstServiceArea_Korea:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_USA:
	case ConstServiceArea_English:
	case ConstServiceArea_NHNChina:
	case ConstServiceArea_EUROPE:
		{
			//������� ��ư
			file.CreateControl( m_pButtonCancelDel[0], NSELECTCHAR_DIALOG_BUTTONCANCELDEL1, TEXT("button_CancelDel1"));
			file.CreateControl( m_pButtonCancelDel[1], NSELECTCHAR_DIALOG_BUTTONCANCELDEL2, TEXT("button_CancelDel2"));
			file.CreateControl( m_pButtonCancelDel[2], NSELECTCHAR_DIALOG_BUTTONCANCELDEL3, TEXT("button_CancelDel3"));

			//������� ����ƽ ����
			file.CreateControl( m_pEditboxwaitdel[0], NSELECTCHAR_DIALOG_EDITWAITDEL1, TEXT("editbox_waitdel1"));
			file.CreateControl( m_pEditboxwaitdel[1], NSELECTCHAR_DIALOG_EDITWAITDEL2, TEXT("editbox_waitdel2"));
			file.CreateControl( m_pEditboxwaitdel[2], NSELECTCHAR_DIALOG_EDITWAITDEL3, TEXT("editbox_waitdel3"));

			//������� ��¥ ����Ʈ�ڽ�
			file.CreateControl( m_pEditboxDeldate[0], NSELECTCHAR_DIALOG_EDITDELDATE1, TEXT("editbox_Deldate1"));
			file.CreateControl( m_pEditboxDeldate[1], NSELECTCHAR_DIALOG_EDITDELDATE2, TEXT("editbox_Deldate2"));
			file.CreateControl( m_pEditboxDeldate[2], NSELECTCHAR_DIALOG_EDITDELDATE3, TEXT("editbox_Deldate3"));

			//���� ��ҳ�¥ ���
			file.CreateControl( m_pImageStaticdeldateBack[0], NSELECTCHAR_DIALOG_STATICDELDATEBACK1, TEXT("imagestatic_deldateBack1"));
			file.CreateControl( m_pImageStaticdeldateBack[1], NSELECTCHAR_DIALOG_STATICDELDATEBACK2, TEXT("imagestatic_deldateBack2"));
			file.CreateControl( m_pImageStaticdeldateBack[2], NSELECTCHAR_DIALOG_STATICDELDATEBACK3, TEXT("imagestatic_deldateBack3"));

			//�̹��� ����ƽ ����������
			file.CreateControl( m_pImageStaticChar[0], NSELECTCHAR_DIALOG_STATICCHAR1, TEXT("imagestatic_Char1"));
			file.CreateControl( m_pImageStaticChar[1], NSELECTCHAR_DIALOG_STATICCHAR2, TEXT("imagestatic_Char2"));
			file.CreateControl( m_pImageStaticChar[2], NSELECTCHAR_DIALOG_STATICCHAR3, TEXT("imagestatic_Char3"));

			file.CreateControl( m_pButtonCharSlot[0], NSELECTCHAR_DIALOG_BUTTONSLOT1, TEXT("button_CharSlot1"));
			file.CreateControl( m_pButtonCharSlot[1], NSELECTCHAR_DIALOG_BUTTONSLOT2, TEXT("button_CharSlot2"));
			file.CreateControl( m_pButtonCharSlot[2], NSELECTCHAR_DIALOG_BUTTONSLOT3, TEXT("button_CharSlot3"));

			//�����뿹���� �̹��� ����ƽ�� �ϴ� false��Ų��.
			m_pImageStaticChar[0]->Show(false);
			m_pImageStaticChar[1]->Show(false);
			m_pImageStaticChar[2]->Show(false);

			m_pEditboxwaitdel[0]->Show(false);
			m_pEditboxwaitdel[1]->Show(false);
			m_pEditboxwaitdel[2]->Show(false);
		
			m_pEditboxDeldate[0]->Show(false);
			m_pEditboxDeldate[1]->Show(false);
			m_pEditboxDeldate[2]->Show(false);

			m_pImageStaticdeldateBack[0]->Show(false);
			m_pImageStaticdeldateBack[1]->Show(false);
			m_pImageStaticdeldateBack[2]->Show(false);

			m_pButtonCancelDel[0]->Show(false);
			m_pButtonCancelDel[1]->Show(false);
			m_pButtonCancelDel[2]->Show(false);
		}
		break;
	default:
		{
			file.CreateControl( m_pImageStaticSlot[0], NSELECTCHAR_DIALOG_STATICSLOT1, TEXT("imagestatic_NONAME1"));
			file.CreateControl( m_pImageStaticSlot[1], NSELECTCHAR_DIALOG_STATICSLOT2, TEXT("imagestatic_NONAME2"));
			file.CreateControl( m_pImageStaticSlot[2], NSELECTCHAR_DIALOG_STATICSLOT3, TEXT("imagestatic_NONAME3"));
		}
		break;
	}
	
	file.CreateControl( m_pButtonSelect,	NSELECTCHAR_DIALOG_BUTTON1, TEXT("button_Select"));
	file.CreateControl( m_pButtonMake,		NSELECTCHAR_DIALOG_BUTTON2, TEXT("button_Make"));
	file.CreateControl( m_pButtonDelete,	NSELECTCHAR_DIALOG_BUTTON3, TEXT("button_Delete"));

	file.CreateControl( m_pButtonCancelDelete,	NSELECTCHAR_DIALOG_BUTTON4, TEXT("button_CancelDelete"));
	file.CreateControl( m_pButtonReturn,		NSELECTCHAR_DIALOG_BUTTON5, TEXT("button_Return"));

	m_pButtonCancelDelete->Show( false );
	m_pButtonReturn->Show( false );

	FILE *fp = _tfopen( TEXT("Config/SelectChar.dat"), TEXT("rt") );

	if ( fp == NULL )
	{
		pclclient->CurrentCharInfoList = 0;
	}
	else
	{
		_ftscanf( fp, TEXT("%d"), &pclclient->CurrentCharInfoList );
		if ( pclclient->CurrentCharInfoList < 0 || pclclient->CurrentCharInfoList > MAX_CHAR_PER_USER-1 )
		{
			pclclient->CurrentCharInfoList = 0;
		}
		fclose( fp );
	}

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
		{
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar1J.SPR "),&sprLoginChar[0]);
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar2J.SPR "),&sprLoginChar[1]);
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/CharBack.SPR"),&sprCharBack);	
			m_3DCharDraw[0].SetXY( GetX() + 218 + pclClient->GetScreenRX(), 428  + pclClient->GetScreenRY() );
			m_3DCharDraw[1].SetXY( GetX() + 450 + pclClient->GetScreenRX(), 428  + pclClient->GetScreenRY() );
			m_3DCharDraw[2].SetXY( GetX() + 682 + pclClient->GetScreenRX(), 428  + pclClient->GetScreenRY() );
		}
		break;
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_Japan:
	case ConstServiceArea_NHNChina:
		{
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar1J.SPR "),&sprLoginChar[0]);
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar2J.SPR "),&sprLoginChar[1]);
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/CharBack.SPR"),&sprCharBack);	
			m_3DCharDraw[0].SetXY( GetX() + 218 + pclClient->GetScreenRX(), 428  + pclClient->GetScreenRY() );
			m_3DCharDraw[1].SetXY( GetX() + 450 + pclClient->GetScreenRX(), 428  + pclClient->GetScreenRY() );
			m_3DCharDraw[2].SetXY( GetX() + 682 + pclClient->GetScreenRX(), 428  + pclClient->GetScreenRY() );
		}
		break;
	case ConstServiceArea_Korea:
	case ConstServiceArea_USA:
	case ConstServiceArea_English:
	case ConstServiceArea_EUROPE:
		{
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar1E.SPR "),&sprLoginChar[0]);
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar2E.SPR "),&sprLoginChar[1]);
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/CharBack.SPR"),&sprCharBack);	
			m_3DCharDraw[0].SetXY( GetX() + 218 + pclClient->GetScreenRX(), 428  + pclClient->GetScreenRY() );
			m_3DCharDraw[1].SetXY( GetX() + 450 + pclClient->GetScreenRX(), 428  + pclClient->GetScreenRY() );
			m_3DCharDraw[2].SetXY( GetX() + 682 + pclClient->GetScreenRX(), 428  + pclClient->GetScreenRY() );
		}
		break;
	default:
		{
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/CharImg.SPR"),	&sprCharImg);
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/EmptySlot.SPR"),	&sprEmptySlot);	
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar1E.SPR "),&sprLoginChar[0]);
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar2E.SPR "),&sprLoginChar[1]);
			GP.LoadSprFromFile(TEXT("Interface/CharSelect/CharAura.spr"),&sprCharAura);	
			m_3DCharDraw[0].SetXY( 50+ pclClient->GetScreenRX(), -35 + pclClient->GetScreenRY() );
			m_3DCharDraw[1].SetXY( 50+ pclClient->GetScreenRX(), 185 + pclClient->GetScreenRY() );
			m_3DCharDraw[2].SetXY( 50+ pclClient->GetScreenRX(), 405 + pclClient->GetScreenRY() );
		}
		break;
	}

	GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar3.SPR"),&sprLoginChar[2]);
	GP.LoadSprFromFile(TEXT("Interface/CharSelect/LoginChar4.SPR"),&sprLoginChar[3]);

	SetSelectChar(pclclient->CurrentCharInfoList);

	SetActionTimer(1000);
	SetTimerOnOff(true);


	//// [��ȣ_NHN->NDOORS] �̰��������� �ȳ�
	//if(pclClient->siServiceArea == ConstServiceArea_Korea)
	//{
	//	// �̰��� ����ĳ���� ���԰� ĳ���� ����� ���� üũ
	//	if(IsCharSlotEmpty() == true)
	//	{
	//		TCHAR* pTitle	= GetTxtFromMgr(5614);
	//		NTCHARString256	pBody(GetTxtFromMgr(40191));

	//		stMsgBox MsgBox;
	//		MsgBox.Set(this, pTitle, pBody, MSGBOX_TYPE_YESNO, 40);
	//		pclclient->SetMsgBox(&MsgBox, NULL, 0);
	//	}

	//}
}

void CALLBACK CNSelectCharDlg::StaticCallBackDialogNSelectCharDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNSelectCharDlg *pThis = (CNSelectCharDlg *)pControllerMgr;
	pThis->CallBackDialogNSelectCharDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNSelectCharDlg::CallBackDialogNSelectCharDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case NSELECTCHAR_DIALOG_BUTTONSLOT1:
	case NSELECTCHAR_DIALOG_BUTTONSLOT2:
	case NSELECTCHAR_DIALOG_BUTTONSLOT3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{	
					SI32 siSlot = nControlID - NSELECTCHAR_DIALOG_BUTTONSLOT1;
					// ĳ���� ���� Ŭ������ �ٸ��� ���� - 2007.08.22
					SI16 siCharKind = pclclient->clCharListInfo[siSlot].clBI.GetKind();
					switch(siCharKind)
					{
					case KIND_HERO3:
						{
							if(pclClient->IsCountrySwitch(Switch_Sound))
							{
								pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HERO")), 0, 0);
							}
						}
						break;
					case KIND_HEROINE3:		
						{
							if(pclClient->IsCountrySwitch(Switch_Sound))
							{
								pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HEROINE")), 0, 0);
							}
						}
						break;
					case KIND_HERO4:
						{
							if(pclClient->IsCountrySwitch(Switch_Sound))
							{
								pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HERO2")), 0, 0);
							}
						}
						break;
					case KIND_HEROINE4:
						{
							if(pclClient->IsCountrySwitch(Switch_Sound))
							{
								pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HEROINE2")), 0, 0);
							}
						}
						break;
					}
					if(siSlot >= 0 && siSlot < MAX_CHAR_PER_USER)
						SetSelectChar(siSlot);
				}
				break;
			case EVENT_BUTTON_DBCLICKED:
				{
					SelectChar();
				}
				break;
			}
		}
		break;

	case NSELECTCHAR_DIALOG_STATICSLOT1:
	case NSELECTCHAR_DIALOG_STATICSLOT2:
	case NSELECTCHAR_DIALOG_STATICSLOT3:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					SI32 siSlot = nControlID - NSELECTCHAR_DIALOG_STATICSLOT1;
					if(siSlot >= 0 && siSlot < MAX_CHAR_PER_USER)
						SetSelectChar(siSlot);
				}
				break;
			case EVENT_IMAGESTATIC_DBCLICKED:
				{
					SelectChar();
				}
				break;
			}
		}
		break;
	case NSELECTCHAR_DIALOG_STATICCHAR1:
	case NSELECTCHAR_DIALOG_STATICCHAR2:
	case NSELECTCHAR_DIALOG_STATICCHAR3:
		{
		}
	case NSELECTCHAR_DIALOG_BUTTONCANCELDEL1:
	case NSELECTCHAR_DIALOG_BUTTONCANCELDEL2:
	case NSELECTCHAR_DIALOG_BUTTONCANCELDEL3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 siIndex = nControlID - NSELECTCHAR_DIALOG_BUTTONCANCELDEL1;
					if(siIndex >= 0 && siIndex <= 2)//���� ���
					{
						/*TCHAR buffer[256] = TEXT("") ;*/
						TCHAR* pTitle = GetTxtFromMgr(7692);
						TCHAR* pText = GetTxtFromMgr(7693);
				/*		StringCchPrintf(buffer, 256, pText);*/
						stMsgBox MsgBox;
						MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 30 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = 0;
						memcpy( &TempBuffer[Size], &siIndex, sizeof(siIndex) );		Size += sizeof(siIndex);
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

						//if (  MessageBox(NULL, pTitle,pText,MB_YESNO ) == IDYES )
						//{
						//	cltGameMsgRequest_PreDelUserChar clinfo(siIndex, TEXT(" "), 0);
						//	cltMsg clMsg(GAMEMSG_REQUEST_PREDELUSERCHAR, sizeof(clinfo), (BYTE*)&clinfo);
						//	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

						//	pclclient->CurrentCharInfoList = -1;
						//	((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->Show();
						//}
						//else{
						//	break;
						//}
						
					}
				}				
			}break;			
		}
		break;
	case NSELECTCHAR_DIALOG_EDITDELDATE1:
	case NSELECTCHAR_DIALOG_EDITDELDATE2:
	case NSELECTCHAR_DIALOG_EDITDELDATE3:
		{
		}
		break;

	case NSELECTCHAR_DIALOG_STATICDELDATEBACK1:
	case NSELECTCHAR_DIALOG_STATICDELDATEBACK2:
	case NSELECTCHAR_DIALOG_STATICDELDATEBACK3:
		{
		}break;

	case NSELECTCHAR_DIALOG_BUTTON1: // ����
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->IsCountrySwitch(Switch_Sound))
					{
						pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GAME_START")), 0, 0);
					}
					SelectChar();
				}
			}
			break;
		}
		break;
	case NSELECTCHAR_DIALOG_BUTTON2:	// �����
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->IsCountrySwitch(Switch_Sound))
					{
						pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_CLICK_BUTTON")), 0, 0);
					}
					
					if ( pclclient->clCharListInfo[pclclient->CurrentCharInfoList].clBI.GetKind() > 0)
					{
						return;
					}

					if( pclclient->m_pDialog[ NMAKECHAR_DLG ] == NULL )
					{
						pclclient->CreateInterface( NMAKECHAR_DLG );
						if(pclClient->siServiceArea == ConstServiceArea_NHNChina)
						{
							pclclient->HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(pclclient->clSelectedWorldInfo.baseinfo.siServerID ,pclclient->m_szPorteAuth_UserID, HANREPORT_CLIENT_MAKECHAR);
						}
						else
						{
							pclclient->HanAuth_Lib_HanReportSendPosLog_Client(pclclient->m_szHanAuth_UserID, HANREPORT_CLIENT_MAKECHAR);

						}
					}

					// ���� ����â �����
					Hide();
				}
				break;
			}
		}
		break;
	case NSELECTCHAR_DIALOG_BUTTON3:	// �ɸ��� ����
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->IsCountrySwitch(Switch_Sound))
					{
						pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_CLICK_BUTTON")), 0, 0);
					}
					
					if(pclclient->clCharListInfo[pclclient->CurrentCharInfoList].clBI.GetKind() != 0)
					{
						if (pclclient->pclCM->CR[1] != NULL)
						{
							if (pclclient->clCharListInfo[pclclient->CurrentCharInfoList].clBI.clPerson.siPersonID
								== pclclient->pclCM->CR[1]->pclCI->clBI.clPerson.siPersonID)
							{
								if (pclclient->m_pDialog[ NOTIFYMSG_DLG ] != NULL)
								{
									if(pclClient->IsCountrySwitch(Switch_Sound))
									{
										pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_CLICK_BUTTON")), 0, 0);
									}
									TCHAR *pTitle = GetTxtFromMgr(836);
									TCHAR *pText = GetTxtFromMgr(5740);
									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
								}
								return;
							}
						}
						if(pclclient->clCharListInfo[pclclient->CurrentCharInfoList].DelCharTime.wYear > 0)return;//����������̸� ����
						TCHAR buffer[1024] = TEXT("") ;
						TCHAR* pTitle = GetTxtFromMgr(1110);
						TCHAR* pText = GetTxtFromMgr(1109);


						StringCchPrintf(buffer, 1024, pText);

						stMsgBox MsgBox;
						MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 2 );
						pclclient->SetMsgBox( &MsgBox, NULL, 0 );
					}
				}
				break;
			}
		}
		break;
	}
}

void CNSelectCharDlg::Hide()
{
	SetTimerOnOff( false );

	CControllerMgr::Show( false );

	return;
}

void CNSelectCharDlg::Show()
{
	SetTimerOnOff( true );

	CControllerMgr::Show( true );

	cltClient *pclclient = (cltClient *)pclClient;
	if(pclclient == NULL)				return;

//	for(SI32 i=0; i<MAX_CHAR_PER_USER; i++){
//		if (pclclient->clCharListInfo[i].DelCharTime.wYear <= 0) {
			SetSelectChar(pclclient->CurrentCharInfoList);
//		}
//	}

	
	

	return;
}

void CNSelectCharDlg::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if (m_siHeartBeatCount >= 20)
	{
		m_siHeartBeatCount = 0;

		cltMsg clMsg(GAMEMSG_REQUEST_HEARTBEAT, 0, NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
	m_siHeartBeatCount++;
}


// [��ȣ_NHN->NDOORS] ĳ���� ������ ����ִ��� üũ
bool CNSelectCharDlg::IsCharSlotEmpty()
{
	cltClient *pClient = (cltClient *)pclClient;
	cltCharListInfo* pclListInfo = NULL;

	for(SI32 siIndex = 0; siIndex < MAX_TOTAL_CHAR_PER_USER; siIndex++)
	{
		pclListInfo = pClient->GetCharListInfo(siIndex);
		
		if(pclListInfo == NULL)
			continue;

		if(pclListInfo->clBI.clPerson.GetPersonID() > 0)
			return false;
	}
	
	return true;
}

void CNSelectCharDlg::SetSelectChar( SI32 slot )
{
 	cltClient *pclclient = (cltClient *)pclClient;
	if(pclclient == NULL)					return;

	for(SI32 i=0; i<MAX_CHAR_PER_USER; i++)
	{

		if (pclclient->clCharListInfo[i].DelCharTime.wYear > 0) {//���� ��¥�� ������ �̹��� ����ƽ�� ������ ���úҰ���.
			m_pImageStaticChar[i]->Show(true);
			m_pEditboxwaitdel[i]->Show(true);
			m_pEditboxDeldate[i]->SetFontColor(RGB(255, 0, 0));
			m_pEditboxDeldate[i]->Show(true);
			m_pImageStaticdeldateBack[i]->Show(true);
			m_pButtonCancelDel[i]->Show(true);

            SYSTEMTIME st_CheckTime;
			TimeAddDate(&pclclient->clCharListInfo[i].DelCharTime, 3, &st_CheckTime);
			TCHAR buffer[256 ] = TEXT("");
			TCHAR* pDyear = (TCHAR*)st_CheckTime.wYear;
			TCHAR* pDymonth = (TCHAR*)st_CheckTime.wMonth;
			TCHAR* pDyDay = (TCHAR*)st_CheckTime.wDay;
			TCHAR* pHour = (TCHAR*)st_CheckTime.wHour;
			TCHAR* pMinute = (TCHAR*)st_CheckTime.wMinute;

			TCHAR* pText = GetTxtFromMgr(7616);
			TCHAR* pTextTop = GetTxtFromMgr(7615);
			StringCchPrintf(buffer, 256, pText, pDyear, pDymonth,pDyDay, pHour,pMinute);
			m_pEditboxDeldate[i]->SetText(buffer);
			m_pEditboxwaitdel[i]->SetText(pTextTop);



		}
		if (pclclient->clCharListInfo[i].DelCharTime.wYear == 0) {//���� ��¥�� ������ �̹��� ����ƽ�� ������ ���úҰ���.
			m_pImageStaticChar[i]->Show(false);
			m_pEditboxwaitdel[i]->Show(false);
			m_pEditboxDeldate[i]->SetFontColor(RGB(255, 0, 0));
			m_pEditboxDeldate[i]->Show(false);
			m_pImageStaticdeldateBack[i]->Show(false);
			m_pButtonCancelDel[i]->Show(false);
		}
		if( pclclient->clCharListInfo[i].clBI.GetKind() > 0 )
		{
			SI16 siCharKind = pclclient->clCharListInfo[i].clBI.GetKind();
			SI32 siFontIndex = 0;
			switch(siCharKind)
			{
			case KIND_HERO3:		siFontIndex = 0;	break;
			case KIND_HEROINE3:		siFontIndex = 8;	break;
			case KIND_HERO4:		siFontIndex = 16;	break;
			case KIND_HEROINE4:		siFontIndex = 24;	break;
			}

			switch( pclClient->siServiceArea )
			{
			case ConstServiceArea_China:
			case ConstServiceArea_Japan:
			case ConstServiceArea_Korea:
			case ConstServiceArea_Taiwan:
			case ConstServiceArea_USA:
			case ConstServiceArea_English:
			case ConstServiceArea_NHNChina:
			case ConstServiceArea_EUROPE:
				{
					m_pButtonCharSlot[i]->SetFileName( TEXT("Interface/CharSelect/CharImg.SPR") );
					m_pButtonCharSlot[i]->SetFontIndex( siFontIndex );
				}
				break;
			default:
				{
					m_pImageStaticSlot[i]->SetFileName( TEXT("Interface/CharSelect/CharImg.SPR") );
					m_pImageStaticSlot[i]->SetFontIndex( siFontIndex / 8 );
				}
				break;
			}

			if (pclclient->clCharListInfo[i].DelCharTime.wYear > 0)
			{
				m_pButtonCharSlot[i]->SetFileName( TEXT("") );
				m_pButtonCharSlot[i]->SetFontIndex( 0 );

				m_pImageStaticChar[i]->SetFileName( TEXT("Interface/CharSelect/CharImg.SPR") );
				m_pImageStaticChar[i]->SetFontIndex( siFontIndex );
			}

			TCHAR StrName[256];
			if( pclclient->clCharListInfo[i].clIP.siLevel > 0 )
				StringCchPrintf(StrName, 256, TEXT("%s(Lv.%d)"), pclclient->clCharListInfo[i].clBI.GetName(), pclclient->clCharListInfo[i].clIP.siLevel);
			else
				StringCchPrintf(StrName, 256, TEXT("%s"), pclclient->clCharListInfo[i].clBI.GetName());

			switch( pclClient->siServiceArea )
			{
			case ConstServiceArea_China:
			case ConstServiceArea_Japan:
			case ConstServiceArea_Korea:
			case ConstServiceArea_Taiwan:
			case ConstServiceArea_USA:
			case ConstServiceArea_English:
			case ConstServiceArea_NHNChina:
			case ConstServiceArea_EUROPE:
				{
					if (pclclient->clCharListInfo[i].DelCharTime.wYear <= 0) {
					m_3DCharDraw[i].SetBack(&sprCharBack);
					m_3DCharDraw[i].DrawMask( true );
					}
					
				}
				break;
			default:
				{
					m_3DCharDraw[i].SetAlpha( 5 );
					m_3DCharDraw[i].SetAura(&sprCharAura);
					m_3DCharDraw[i].DrawAura( false );
				}
				break;
			}

			m_3DCharDraw[i].SetImage(NULL, 0);
			m_3DCharDraw[i].ResetFrame();

			switch( pclClient->siServiceArea )
			{
			case ConstServiceArea_China:
			case ConstServiceArea_Japan:
			case ConstServiceArea_Korea:
			case ConstServiceArea_Taiwan:
			case ConstServiceArea_USA:
			case ConstServiceArea_English:
			case ConstServiceArea_NHNChina:
			case ConstServiceArea_EUROPE:
				{
					switch(siCharKind)
					{
					case KIND_HERO3:		m_3DCharDraw[i].SetImage(&sprLoginChar[0], 40);	break;
					case KIND_HEROINE3:		m_3DCharDraw[i].SetImage(&sprLoginChar[1], 57);	break;
					case KIND_HERO4:		m_3DCharDraw[i].SetImage(&sprLoginChar[2], 40);	break;
					case KIND_HEROINE4:		m_3DCharDraw[i].SetImage(&sprLoginChar[3], 49);	break;
					}

					if ( slot == i )
					{
						if (pclclient->clCharListInfo[i].DelCharTime.wYear <= 0) {
							m_3DCharDraw[i].DrawMask( false);//true�� ��© ȸ��.
						}
						
					}
				}
				break;
			default:
				{
					switch(siCharKind)
					{
					case KIND_HERO3:		m_3DCharDraw[i].SetImage(&sprLoginChar[0], 46);	break;
					case KIND_HEROINE3:		m_3DCharDraw[i].SetImage(&sprLoginChar[1], 49);	break;
					case KIND_HERO4:		m_3DCharDraw[i].SetImage(&sprLoginChar[2], 40); break;
					case KIND_HEROINE4:		m_3DCharDraw[i].SetImage(&sprLoginChar[3], 49);	break;
					}

					if ( slot == i )
					{
						m_3DCharDraw[i].DrawAura( false );
					}
				}
				break;
			}
		}
		else
		{
			switch( pclClient->siServiceArea )
			{
			case ConstServiceArea_China:
			case ConstServiceArea_Japan:
			case ConstServiceArea_Korea:
			case ConstServiceArea_Taiwan:
			case ConstServiceArea_USA:
			case ConstServiceArea_English:
			case ConstServiceArea_NHNChina:
			case ConstServiceArea_EUROPE:
				{
					if (pclclient->clCharListInfo[i].DelCharTime.wYear <= 0) {
						m_pButtonCharSlot[i]->SetFileName(TEXT(""));
						m_pButtonCharSlot[i]->SetFontIndex(0);
						m_3DCharDraw[i].SetBack(&sprCharBack);
						m_3DCharDraw[i].SetImage(NULL, 0);
					}
					
				}
				break;
			default:
				{
					m_pImageStaticSlot[i]->SetFileName( TEXT("Interface/CharSelect/EmptySlot.SPR") );
					m_pImageStaticSlot[i]->SetFontIndex( 0 );
					m_3DCharDraw[i].SetImage(NULL, 0);
					m_3DCharDraw[i].DrawAura( false );
				}
				break;
			}
		}
	}

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Japan:
	case ConstServiceArea_Korea:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_USA:
	case ConstServiceArea_English:
	case ConstServiceArea_NHNChina:
	case ConstServiceArea_EUROPE:
		{
			// ���õ� ������ �̹����� �����Ų��.
			if(slot >= 0 && slot < MAX_CHAR_PER_USER)
			{
				pclclient->CurrentCharInfoList = slot;

				SI32 siFontIndex = m_pButtonCharSlot[slot]->GetFontIndex();
				m_pButtonCharSlot[slot]->SetFontIndex(siFontIndex + 4);
			}
		}
		break;
	default:
		{
			if(slot >= 0 && slot < MAX_CHAR_PER_USER)
			{
				pclclient->CurrentCharInfoList = slot;

				if( pclclient->clCharListInfo[slot].clBI.GetKind() <= 0 )
				{
					if( pclClient->siServiceArea != ConstServiceArea_Japan
						&& pclClient->siServiceArea != ConstServiceArea_Korea
						&& pclClient->siServiceArea != ConstServiceArea_Taiwan
						&& pclClient->siServiceArea != ConstServiceArea_USA
						&& pclClient->siServiceArea != ConstServiceArea_English
						&& pclClient->siServiceArea != ConstServiceArea_NHNChina
						&& pclClient->siServiceArea != ConstServiceArea_EUROPE
					)
						m_pImageStaticSlot[slot]->SetFontIndex(1);
				}
				else
				{
					SI16 siCharKind = pclclient->clCharListInfo[slot].clBI.GetKind();
					if( pclClient->siServiceArea != ConstServiceArea_Japan
						&& pclClient->siServiceArea != ConstServiceArea_Korea
						&& pclClient->siServiceArea != ConstServiceArea_Taiwan
						&& pclClient->siServiceArea != ConstServiceArea_USA
						&& pclClient->siServiceArea != ConstServiceArea_English
						&& pclClient->siServiceArea != ConstServiceArea_NHNChina
						&& pclClient->siServiceArea != ConstServiceArea_EUROPE
					)
					{
						switch(siCharKind)
						{
						case KIND_HERO3:		m_pImageStaticSlot[slot]->SetFileName( TEXT("Interface/CharSelect/CharSelect_FX01.SPR") );	siCharEffectMaxFrame = 10;	break;
						case KIND_HEROINE3:		m_pImageStaticSlot[slot]->SetFileName( TEXT("Interface/CharSelect/CharSelect_FX02.SPR") );	siCharEffectMaxFrame = 10;	break;
						case KIND_HERO4:		m_pImageStaticSlot[slot]->SetFileName( TEXT("Interface/CharSelect/CharSelect_FX03.SPR") );	siCharEffectMaxFrame = 10;	break;
						case KIND_HEROINE4:		m_pImageStaticSlot[slot]->SetFileName( TEXT("Interface/CharSelect/CharSelect_FX04.SPR") );	siCharEffectMaxFrame = 10;	break;
						default:				m_pImageStaticSlot[slot]->SetFileName( TEXT("") );	siCharEffectMaxFrame = 0;		break;
						}
						siCharEffectFrame = 0;				
						m_pImageStaticSlot[slot]->SetFontIndex(siCharEffectFrame);
					}

					m_3DCharDraw[slot].SetAlpha( MAX_ALPHA_LEVEL - 1 );
					m_3DCharDraw[slot].DrawAura( true );
				}
			}
		}
		break;
	}
}


void CNSelectCharDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if(IsShow() == false)	return;

	cltClient * pclclient = (cltClient *)pclClient;
	if(pclclient == NULL)	return;

	SI32 i;
	HDC hdc;	
	if(pclclient->lpBackScreen->GetDC(&hdc) != DD_OK) return;

	HFONT hFont;
	hFont = g_FontManager.SetFont( 15 );

 	HFONT hOldFont = NULL;
	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}

 	SetBkMode(hdc, TRANSPARENT);	
	TCHAR StrName[256];

	for(i=0; i<MAX_CHAR_PER_USER; i++)
	{
		StrName[0] = '\0';
		if(pclclient->clCharListInfo[i].clBI.GetKind() > 0)
		{
			switch( pclClient->siServiceArea )
			{
			case ConstServiceArea_China:
			case ConstServiceArea_Japan:
			case ConstServiceArea_Korea:
			case ConstServiceArea_Taiwan:
			case ConstServiceArea_USA:				
			case ConstServiceArea_English:
			case ConstServiceArea_NHNChina:
			case ConstServiceArea_EUROPE:
				{
					if(pclclient->clCharListInfo[i].clIP.siLevel > 0)
						StringCchPrintf(StrName, 256, TEXT("%s\n(Lv.%d)"), pclclient->clCharListInfo[i].clBI.GetName(), pclclient->clCharListInfo[i].clIP.siLevel);
					else
						StringCchPrintf(StrName, 256, TEXT("%s"), pclclient->clCharListInfo[i].clBI.GetName());
				}
				break;
			default:
				{
					if(pclclient->clCharListInfo[i].clIP.siLevel > 0)
						StringCchPrintf(StrName, 256, TEXT("%s(Lv.%d)"), pclclient->clCharListInfo[i].clBI.GetName(), pclclient->clCharListInfo[i].clIP.siLevel);
					else
						StringCchPrintf(StrName, 256, TEXT("%s"), pclclient->clCharListInfo[i].clBI.GetName());
				}
				break;
			}
		}
		else
		{
			switch( pclClient->siServiceArea )
			{
			case ConstServiceArea_China:
			case ConstServiceArea_Japan:
			case ConstServiceArea_Korea:
			case ConstServiceArea_Taiwan:
			case ConstServiceArea_USA:
			case ConstServiceArea_English:
			case ConstServiceArea_NHNChina:
			case ConstServiceArea_EUROPE:
				{
					TCHAR* pMakeCharater;
					pMakeCharater = GetTxtFromMgr(6987);

					StringCchPrintf(StrName, 256, pMakeCharater);
				}
				break;
			}
		}

		RECT rect;


		switch( pclClient->siServiceArea )
		{
		case ConstServiceArea_China:
		case ConstServiceArea_Japan:
		case ConstServiceArea_Korea:
		case ConstServiceArea_Taiwan:
		case ConstServiceArea_USA:
		case ConstServiceArea_English:
		case ConstServiceArea_NHNChina:
		case ConstServiceArea_EUROPE:
			{
				rect.left	= GetX() + 121 + (i * 232)  + ScreenRX;
				rect.top	= GetY() + 106   + ScreenRY;
				rect.right	= rect.left + 220;
				rect.bottom	= rect.top	+ 50;

				if( pclclient->CurrentCharInfoList == i || m_pButtonCharSlot[i]->IsMouseOver() )
				{
					if( pclClient->siServiceArea == ConstServiceArea_Japan
						|| pclClient->siServiceArea == ConstServiceArea_China
						|| pclClient->siServiceArea == ConstServiceArea_Korea
						|| pclClient->siServiceArea == ConstServiceArea_Taiwan
						|| pclClient->siServiceArea == ConstServiceArea_USA
						|| pclClient->siServiceArea == ConstServiceArea_English 
						|| pclClient->siServiceArea == ConstServiceArea_NHNChina
						|| pclClient->siServiceArea == ConstServiceArea_EUROPE
					)					
					{
						SetTextColor(hdc, RGB(178, 50, 116));

						// �ܰ��� �׸���
						RECT temprc;
						CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, StrName, lstrlen(StrName), &temprc, DT_CENTER | DT_VCENTER  | DT_WORDBREAK);
						CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, StrName, lstrlen(StrName), &temprc, DT_CENTER | DT_VCENTER  | DT_WORDBREAK);
						CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, StrName, lstrlen(StrName), &temprc, DT_CENTER | DT_VCENTER  | DT_WORDBREAK);
						CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, StrName, lstrlen(StrName), &temprc, DT_CENTER | DT_VCENTER  | DT_WORDBREAK);

						CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, StrName, lstrlen(StrName), &temprc, DT_CENTER | DT_VCENTER  | DT_WORDBREAK);
						CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, StrName, lstrlen(StrName), &temprc, DT_CENTER | DT_VCENTER  | DT_WORDBREAK);
						CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, StrName, lstrlen(StrName), &temprc, DT_CENTER | DT_VCENTER  | DT_WORDBREAK);
						CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, StrName, lstrlen(StrName), &temprc, DT_CENTER | DT_VCENTER  | DT_WORDBREAK);

						SetTextColor(hdc, RGB(255, 255, 255));
						DrawText( hdc, StrName, lstrlen(StrName), &rect, DT_CENTER | DT_VCENTER );
					}
				}
				else
				{
					SetTextColor(hdc,RGB(122, 114, 174));
					DrawText( hdc, StrName, lstrlen(StrName), &rect, DT_CENTER | DT_VCENTER );
				}
			}
			break;
		default:
			{
				if( pclclient->CurrentCharInfoList == i )
					rect.top	= GetY() + 130 + (i * 220) + 20;
				else
					rect.top	= GetY() + 130 + (i * 220);
				rect.left	= GetX() + 20;
				rect.right	= rect.left + 260;
				rect.bottom	= rect.top	+ 20;

				SetTextColor(hdc, RGB(255, 255, 255));
				DrawText( hdc, StrName, lstrlen(StrName), &rect, DT_CENTER | DT_VCENTER  | DT_WORDBREAK);
			}
			break;
		}
	}

	if( pclclient->CurrentClock - DelayClock > 100 )
	{
		for(i=0; i<MAX_CHAR_PER_USER; i++)
		{
			if(pclclient->CurrentCharInfoList == i)		m_3DCharDraw[i].MoveNextFrame();
		}

		switch( pclClient->siServiceArea )
		{
		case ConstServiceArea_China:
		case ConstServiceArea_Japan:
		case ConstServiceArea_Korea:
		case ConstServiceArea_Taiwan:
		case ConstServiceArea_USA:
		case ConstServiceArea_English:
		case ConstServiceArea_NHNChina:
		case ConstServiceArea_EUROPE:
			break;
		default:
			{
				// ���õ� ĳ������ ���ϸ��̼� ó��
				SI16 siCharKind = pclclient->clCharListInfo[pclclient->CurrentCharInfoList].clBI.GetKind();
				if(siCharKind > 0)
				{
					siCharEffectFrame++;
					if(siCharEffectMaxFrame == 0)	siCharEffectFrame = 0;
					else							siCharEffectFrame %= siCharEffectMaxFrame;

					m_pImageStaticSlot[pclclient->CurrentCharInfoList]->SetFontIndex( siCharEffectFrame );
				}

			}
			break;
		}
		DelayClock = pclclient->CurrentClock ;
	}

	for(i=0; i<MAX_CHAR_PER_USER; i++)//�ִϸ��̼��� �׸��µ�...���� ���� ���ڰ� ������ �ȱ׸�����
	{
		if(pclclient->clCharListInfo[i].DelCharTime.wYear <= 0)
		{
			m_3DCharDraw[i].Draw();
		}
		
	}

	if(hOldFont)
	{
		SelectObject(hdc,hOldFont);
	}
	
	pclclient->lpBackScreen->ReleaseDC(hdc);
}

void CNSelectCharDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			return;
		}
		break;

	case 1:
		break;

	case 2:
		{
			if ( RetValue )
			{
				TCHAR name[MAX_PLAYER_NAME] = TEXT("") ;
				TCHAR level[8] = TEXT("") ;
				StringCchCopy(name, MAX_PLAYER_NAME, pclclient->clCharListInfo[pclclient->CurrentCharInfoList].clBI.GetName());
				StringCchPrintf(level, 8, TEXT("%d"),pclclient->clCharListInfo[pclclient->CurrentCharInfoList].clIP.siLevel);

				//=====================================================
				// �Ϻ�, �۷ι�, �븸, USA ������ �ֹι�ȣ ������ �ʵ��� ����
				//=====================================================
				//if( ((CNDeleteCharDlg*)pclclient->m_pDialog[ NDELETECHAR_DLG ]) )
				//	((CNDeleteCharDlg*)pclclient->m_pDialog[ NDELETECHAR_DLG ])->set(name,level,pclclient->CurrentCharInfoList);
				//=====================================================
				// ���׼��� : PCK - 2007
				// ���� �÷������� ĳ���ʹ� ������ ���ϰ� �Ѵ�.
				if( pclclient->clCharListInfo[pclclient->CurrentCharInfoList].clBI.clPerson.siPersonID == pclclient->pclCM->CR[1]->pclCI->clBI.clPerson.siPersonID)
				{
					if (pclclient->m_pDialog[ NOTIFYMSG_DLG ] != NULL)
					{
						if(pclclient->clCharListInfo[pclclient->CurrentCharInfoList].DelCharTime.wYear > 0)return;
						TCHAR *pTitle = GetTxtFromMgr(836);
						TCHAR *pText = GetTxtFromMgr(5740);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
					}
					return;
				}

				if ( pclClient->siServiceArea == ConstServiceArea_Japan		|| 
					 pclClient->siServiceArea == ConstServiceArea_English	|| 
					 pclClient->siServiceArea == ConstServiceArea_Taiwan	||
					 pclClient->siServiceArea == ConstServiceArea_USA		||
					 pclClient->siServiceArea == ConstServiceArea_English	||
					 pclClient->siServiceArea == ConstServiceArea_Korea		||
					 pclClient->siServiceArea == ConstServiceArea_NHNChina	||
					 pclClient->siServiceArea == ConstServiceArea_EUROPE
				)
				{//Ŭ���̾�Ʈ���� ������ ������ 1
                   
					if ( pclClient->IsCountrySwitch(Switch_PreDeleteChar))
					{
						 if(pclclient->clCharListInfo[pclclient->CurrentCharInfoList].DelCharTime.wYear > 0)return;
						//�ɸ��͸� ���� ���� �ʰ��� ���·� �Ѵ�.
						cltGameMsgRequest_PreDelUserChar clinfo(pclclient->CurrentCharInfoList, TEXT(""),1);
						cltMsg clMsg(GAMEMSG_REQUEST_PREDELUSERCHAR, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						pclclient->CurrentCharInfoList = -1;
						((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->Show();
					}
					else
					{
						cltGameMsgRequest_DelUserChar clinfo(pclclient->CurrentCharInfoList, TEXT(""));
						cltMsg clMsg(GAMEMSG_REQUEST_DELUSERCHAR, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					
				}
				else
				{
					
					if( ((CNDeleteCharDlg*)pclclient->m_pDialog[ NDELETECHAR_DLG ]) )
						((CNDeleteCharDlg*)pclclient->m_pDialog[ NDELETECHAR_DLG ])->set(name,level,pclclient->CurrentCharInfoList);
				}
				//Hide();
				//=====================================================
			}
			else
				break;

		}
		break;
	case 30 :
		if (RetValue) {
				SI32 siDataPtr = 0;
				SI32 siIndex = *(SI32*)&pData[siDataPtr];		siDataPtr+= sizeof(siIndex);
				cltGameMsgRequest_PreDelUserChar clinfo(siIndex, TEXT(" "), 0);
				cltMsg clMsg(GAMEMSG_REQUEST_PREDELUSERCHAR, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				pclclient->CurrentCharInfoList = -1;
				((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->Show();
		}
		break;

	case 40:
		{
			if(RetValue)
			{
				// Ŭ���̾�Ʈ ���� �� �̰��������� �̵�
				ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), TEXT("http://goonzus.ndoors.com/"), NULL, SW_SHOWNORMAL );
				dwQuitTimer = 1;
			}
		}
		break;
	}
}


void CNSelectCharDlg::SelectChar()
{
	cltClient * pclclient = (cltClient*)pclClient;
	if(pclclient == NULL)			return;

	if( pclclient->CurrentCharInfoList < 0 || pclclient->CurrentCharInfoList >= MAX_CHAR_PER_USER )		return;

	// ��ȿ�� �������� Ȯ���Ѵ�.
	if(pclclient->clCharListInfo[pclclient->CurrentCharInfoList].clBI.GetKind() == 0)
	{
		TCHAR* pTitle = GetTxtFromMgr(836);
		TCHAR* pText = GetTxtFromMgr(1113);

		stMsgBox MsgBox;
		MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_OK, 1 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0 );
	}					
	else
	{
		// ���� ��� ���� ĳ���ʹ� ���ӿ� ���� ���Ѵ�.
		if(pclclient->clCharListInfo[pclclient->CurrentCharInfoList].DelCharTime.wYear > 0)
			return;

		FILE *fp = _tfopen( TEXT("Config/SelectChar.dat"), TEXT("wt") );
		if ( fp != NULL )
		{
			if ( pclclient->CurrentCharInfoList < 0 || pclclient->CurrentCharInfoList > MAX_CHAR_PER_USER-1 )
			{
				pclclient->CurrentCharInfoList = 0;
			}
			_ftprintf( fp, TEXT("%d"), pclclient->CurrentCharInfoList );
			fclose( fp );
		}


		if(	pclclient->IsWhereServiceArea( ConstServiceArea_Korea|ConstServiceArea_English ))
		{
			// [��ȣ_NHN->NDOORS] Ư�����ڰ� �� ĳ���ʹ� ĳ���͸� ������ �ؾ� �մϴ�.
			TCHAR* pStrName = (TCHAR*)pclclient->clCharListInfo[pclclient->CurrentCharInfoList].clBI.GetName();

			// Ư�� üũ
			if(!IsCorrectID(pStrName))
			{
				pclclient->CreateInterface(NCHANGECHARNAME_DLG);

				pclclient->m_pDialog[NCHANGECHARNAME_DLG]->SetTopMostWindow(true);

				if(pclclient->m_pDialog[NCHANGECHARNAME_DLG] != NULL)
				{
					((CNChangeCharName*)pclclient->m_pDialog[NCHANGECHARNAME_DLG])->SetCharName(pStrName);
					((CNChangeCharName*)pclclient->m_pDialog[NCHANGECHARNAME_DLG])->SetSlotIndex(pclclient->CurrentCharInfoList);
				}
				return;
			}
		}

		cltCharClient* pclChar = pclclient->pclCM->GetCharClient(1);
		if( NULL != pclChar)
		{
			if(pclChar->pclCI->clBI.uiIndex != pclclient->CurrentCharInfoList)
			{
				// [����] �ٸ� ĳ���Ͱ� ��� ���� Ÿ�̸Ӹ� �����Ѵ�.
				pclclient->bTimeCount = false;
			}
		}
		
		cltMsg clMsg(GAMEMSG_REQUEST_SELECTCHAR, pclclient->CurrentCharInfoList);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		// ���̾�α׸� �����Ѵ�.
		DeleteDialog();
	}
}
