
//---------------------------------
// 2004/9/01 성준엽
//---------------------------------

#include "CommonLogic.h"
#include "../../Client/Client.h"

#include "AutoHunting.h"

#include "../../Resource.h"

#include "../../lib/WebHTML/WebHTML.h"

//----------------------------
// CommonLogic 
//---------------------------
#include "CommonLogic.h"
#include "MsgType.h"
#include "MsgType-System.h"
#include "MsgType-Person.h"
#include "MsgRval-Define.h"



//----------------------------
// Server
//----------------------------
#include "../Server/Server.h"
#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/MinisterMgr.h"
#include "../Server/Minister/Minister-Hyungzo.h"

//---------------------------
// Client
//---------------------------
#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/FontManager/FontManager.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

//----------------------------
// DBManager
//----------------------------
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"


extern cltCommonLogic* pclClient;

cltAutoHuntingManager::cltAutoHuntingManager()
{
}

cltAutoHuntingManager::~cltAutoHuntingManager()
{
}

/*void cltAutoHuntingManager::Create( HINSTANCE hInst, HWND hParent )
{
	m_hWnd = NULL ;
	m_hInst = NULL ;

	m_hDlg = NULL ;

	m_hStaticBrush = NULL ;
	m_hDlgBrush  = NULL ;

	m_hInst = hInst ;
	m_hWnd = hParent ;

	m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_AUTOHUNTING_ASK ), m_hWnd, StaticAutoHuntingDlgMsgProc );

	Hide();
}

void cltAutoHuntingManager::Set(TCHAR * Txt,SI32 slot,SI32 minuts,SI16 prisontime,SI16 prisonerpenalty)
{
	TCHAR buffer[2048] = TEXT("") ;

	g_FontManager.SetFont( m_hDlg, IDC_STATIC_NEEDTXT, 20 );
	sprintf(buffer,TEXT("입력하실 단어 : %s"),Txt);
	SetWindowText(GetDlgItem(m_hDlg,IDC_STATIC_NEEDTXT),buffer);

	const TCHAR* name = pclClient->pclCM->CR[1]->GetName();
	sprintf(buffer,
		"[%s]님은 현재 다른 사용자에 의해 매크로 사냥 프로그램을 사용하고 있다는 제보가 들어왔습니다.\r\n"
		"주어진 단어를 %d분 이내에 입력하면 결백을 입증할 수 있습니다.\r\n"
		"3번 연속 입력이 틀리거나 시간을 초과하면 %d분간 감옥에 수감됩니다.\r\n"
		"또한 [%d]만큼의 경험치가 회수됩니다.\r\n\r\n"
		TEXT("단어 입력후 꼭 '보내기' 버튼을 눌러주시기 바랍니다.\r\n"),
		name,minuts/60, prisontime,
		pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() * prisonerpenalty );

	SetWindowText(GetDlgItem(m_hDlg,IDC_EDIT_AUTOHUNTING_EXPLAIN),buffer);

	m_siLeftTime = minuts ;
	m_siSlot = slot ;

	ShowWindow(m_hDlg,SW_SHOW);
	SetWindowText(GetDlgItem(m_hDlg,IDC_EDIT_AUTOHUNTING_ANSWER), TEXT(""));
	SetFocus(GetDlgItem(m_hDlg,IDC_EDIT_AUTOHUNTING_ANSWER));
}

void cltAutoHuntingManager::SetStaticTime(SI32 totaltime)
{
	TCHAR buffer[256] = TEXT("") ;

	sprintf(buffer, TEXT("[매크로 사냥 프로그램 사용 확인중] 남은 시간: %d/%d "),
					GetLeftTime(),totaltime);

	SetWindowText(GetDlgItem(m_hDlg,IDC_STATIC_TIME),buffer);
}

void cltAutoHuntingManager::Hide()
{
	ShowWindow(m_hDlg,SW_HIDE);
}

BOOL CALLBACK cltAutoHuntingManager::StaticAutoHuntingDlgMsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclAutoHuntingManager->AutoHuntingDlgMsgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK cltAutoHuntingManager::AutoHuntingDlgMsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			m_hStaticBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_GRAY);
		}
		break;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK:
				{
					TCHAR buffer[256] ;

					GetWindowText(GetDlgItem(hDlg,IDC_EDIT_AUTOHUNTING_ANSWER),buffer,256);

					cltGameMsgRequest_AutoHunting_Ask clinfo(m_siSlot,buffer);
					cltMsg clMsg(GAMEMSG_REQUEST_AUTOHUNTING_ASK, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_AUTOHUNTING_EXPLAIN ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
				return (BOOL)m_hStaticBrush;
			}
			else if ( GetDlgItem(hDlg,IDC_STATIC_TIME) == (HWND)lParam)
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
				return (BOOL)m_hStaticBrush;
			}
			else if ( GetDlgItem(hDlg,IDC_STATIC_NEEDTXT) == (HWND)lParam)
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
				return (BOOL)m_hStaticBrush;
			}
		}
		break;
//	case WM_CTLCOLORDLG:
//		{
//			HDC hdc = (HDC)wParam;
//			SetTextColor( hdc, COLOR_DIALOG_BLACK );
//			SetBkColor( hdc, COLOR_DIALOG_GRAY );
//
//			return (BOOL)m_hDlgBrush;
//		}
//		break;



	case WM_CLOSE:
		{
			Hide();
		}
		break;
	case WM_DESTROY:		
		{
			if ( m_hStaticBrush )
			{
				DeleteObject( m_hStaticBrush );
				m_hStaticBrush = NULL;
			}
			if ( m_hDlgBrush )
			{
				DeleteObject( m_hDlgBrush );
				m_hDlgBrush = NULL;
			}
		}
		break;
	}

	return FALSE ;
}*/

void cltAutoHuntingManager::Init()
{
	for ( int i = 0 ; i < MAX_SUSPECT_NUM ; i ++ )
	{
		m_bActionSwitch[i] = false ;

		StringCchCopy( m_szRequestName[i] , MAX_PLAYER_NAME, TEXT("") );
		StringCchCopy( m_szSuspectName[i] , MAX_PLAYER_NAME, TEXT("") );

		m_szRequestPersonID[i] = 0 ;

		StringCchCopy( m_szRequestTxt[i] , MAX_ITEM_NAME, TEXT("") );
		m_siCheckNum[i] = 0 ;

		m_dwWatingClock[i] = 0 ;
		m_dwUpdateColok[i] = 0 ;
	}

}

bool cltAutoHuntingManager::Set(const TCHAR * Txt,TCHAR * request,TCHAR * suspect,SI32* slot,SI32 requestid)
{
	if ( requestid <= 0 ) return false ;

	for ( int i = 0 ; i < MAX_SUSPECT_NUM ; i ++ )
	{
		if ( m_bActionSwitch[i] ) continue ;

		m_bActionSwitch[i] = true ;

		StringCchCopy( m_szRequestName[i] , MAX_PLAYER_NAME, request );
		StringCchCopy( m_szSuspectName[i] , MAX_PLAYER_NAME, suspect );

		m_szRequestPersonID[i] = requestid ;

		StringCchCopy( m_szRequestTxt[i] , MAX_ITEM_NAME, Txt );

		m_dwWatingClock[i] = ((cltServer*)pclClient)->CurrentClock ;
		m_dwUpdateColok[i] = ((cltServer*)pclClient)->CurrentClock ;

		*slot = i ;
		
		return true ;
	}

	return false ;
}

void cltAutoHuntingManager::Action()
{
	cltServer* pclServer = (cltServer*) pclClient ;
	if ( pclServer == NULL ) return ;

	CMinisterHyungzo *pclminister = (CMinisterHyungzo *)pclServer->pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);
	
	if ( pclminister == NULL ) return ;
	SI32 TimeLimit	= min(300, max(20, pclminister->m_siSuspectTime)) ;
	SI16 PrisonTime = min(60, max(5, pclminister->m_siPrisonTime));

	for ( int i = 0 ; i < MAX_SUSPECT_NUM ; i ++ )
	{
		if ( !m_bActionSwitch[i] ) continue ;

		if ( pclServer->CurrentClock - m_dwWatingClock[i] > (DWORD)TimeLimit * 1000 )
		{
			SI32 requestid = pclServer->pclCM->GetIDFromName(m_szRequestName[i]);
			SI32 suspectid = pclServer->pclCM->GetIDFromName(m_szSuspectName[i]);

			TCHAR *pText = GetTxtFromMgr(6308);
			PrisonUser(m_szSuspectName[i],m_szRequestPersonID[i],PrisonTime, pText);

			SI32 addexp = 0 ;
			SI32 deleteexp = 0 ;

			if ( IsPC(suspectid) )
			{
				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))  
				{
					// TargetID	의심되는 유저 personID
					// para1	오답
					// para2	정답

					SI32 siSuspectPersonID = pclServer->pclCM->GetPersonIDFromName(m_szSuspectName[i]);

					pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRISON, LOGCOMMAND_INDEX_CHECK_SUSPECTUSER_BYQUIZ, 
						0, NULL, NULL, siSuspectPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						TEXT("TIME OVER"), TEXT("TIME OVER"));
				}

				cltGameMsgResponse_AutoHunting_Ask clinfo(false,0,pclminister->m_siPrisonerPenalty);
				cltMsg clMsg(GAMEMSG_RESPONSE_AUTOHUNTING_ASK, 	sizeof(clinfo), (BYTE*)&clinfo);
				pclServer->pclCM->CR[suspectid]->SendNetMsg((sPacketHeader*)&clMsg);

				// 경험치를 줄여준다.(level * 10)
				SI16 penalty = pclminister->m_siPrisonerPenalty ;
				deleteexp = pclServer->pclCM->CR[suspectid]->pclCI->clIP.GetLevel() * penalty ;
				addexp = deleteexp / 10 ;
				pclServer->pclCM->CR[suspectid]->pclCI->clIP.DecreaseExp( deleteexp );
				// 클라이언트로 정보를 보낸다.
				cltGameMsgResponse_GetExp clinfo2(suspectid,  -deleteexp,  0, 0);
				cltMsg clMsg2(GAMEMSG_RESPONSE_GETEXP, sizeof(clinfo2), (BYTE*)&clinfo2);
				pclServer->pclCM->CR[suspectid]->SendNetMsg((sPacketHeader*)&clMsg2);
			}

			if ( IsPC(requestid) )
			{
				// 경험치를 늘여준다.
				cltCharServer* pclchar = (cltCharServer*)pclServer->pclCM->CR[requestid];
				pclchar->IncreaseExp(addexp);

				pclServer->SendServerResponseMsg(0, SRVAL_AUTOHUNTING_SUCCESS_PRISON, addexp, PrisonTime, requestid);
			}

			DeletePerson(i);
		}
		else if ( pclServer->CurrentClock - m_dwUpdateColok[i] > CLIENT_UPDATE_TIME * 1000 )
		{
			m_dwUpdateColok[i] = pclServer->CurrentClock ;

			SI32 suspectid = pclServer->pclCM->GetIDFromName(m_szSuspectName[i]);

			if ( IsPC(suspectid) )
			{
				cltGameMsgResponse_AutoHunting_TimeUpdate clinfo(CLIENT_UPDATE_TIME,TimeLimit);
				cltMsg clMsg(GAMEMSG_RESPONSE_AUTOHUNTING_TIMEUPDATE, sizeof(clinfo), (BYTE*)&clinfo);
				pclServer->pclCM->CR[suspectid]->SendNetMsg((sPacketHeader*)&clMsg);
			}
			else
				DeletePerson(i);
		}
	}
}

void cltAutoHuntingManager::DeletePerson(const TCHAR* szSuspectName)
{
	for ( int i = 0 ; i < MAX_SUSPECT_NUM ; i ++ )
	{
		if ( m_bActionSwitch[i] ) continue ;

		if ( !_tcscmp(m_szSuspectName[i],szSuspectName) )
			DeletePerson(i);
	}

	return ;
}

void cltAutoHuntingManager::DeletePerson(const SI32 siSlot)	
{
	m_bActionSwitch[siSlot] = false ;

	StringCchCopy( m_szRequestName[siSlot] , MAX_PLAYER_NAME, TEXT("") );
	StringCchCopy( m_szSuspectName[siSlot] , MAX_PLAYER_NAME, TEXT("") );

	StringCchCopy( m_szRequestTxt[siSlot] , MAX_ITEM_NAME, TEXT("") );
	m_siCheckNum[siSlot] = 0 ;

	m_dwWatingClock[siSlot] = 0 ;
	m_dwUpdateColok[siSlot] = 0 ;

	return ;
}

bool cltAutoHuntingManager::IsRequest(const TCHAR* name)
{
	if ( name == NULL || !_tcscmp(name, TEXT("")) ) return false ;

	for ( int i = 0 ; i < MAX_SUSPECT_NUM ; i ++ )
	{
		if ( m_bActionSwitch[i] == false ) continue ;

		if ( !_tcscmp(name,m_szSuspectName[i]) ) return true ;
	}

	return false ;
}

bool cltAutoHuntingManager::CheckAnswer(const TCHAR* szSuspectName,const TCHAR* szAnswer,
										SI32 siSlot,SI32* siCheck)
{
	if ( _tcscmp(m_szSuspectName[siSlot],szSuspectName) ) return false ;
	if ( szSuspectName == NULL || !_tcscmp(szSuspectName, TEXT("")) ) return false ;
	if ( szAnswer == NULL || !_tcscmp(szAnswer, TEXT("")) ) return false ;
	if ( siSlot < 0 || siSlot >= MAX_SUSPECT_NUM ) return false ;

	cltServer* pclServer = (cltServer*) pclClient ;
	if ( pclServer == NULL ) return false ;

	CMinisterHyungzo *pclminister = (CMinisterHyungzo *)pclServer->pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);
	if ( pclminister == NULL ) return false;
	SI16 PrisonTime = min(60, max(5, pclminister->m_siPrisonTime));


	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))  
	{
		// TargetID	의심되는 유저 personID
		// para1	오답
		// para2	정답

		SI32 siSuspectPersonID = pclServer->pclCM->GetPersonIDFromName(m_szSuspectName[siSlot]);

		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRISON, LOGCOMMAND_INDEX_CHECK_SUSPECTUSER_BYQUIZ, 
										0, NULL, NULL, siSuspectPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										(TCHAR*)szAnswer, m_szRequestTxt[siSlot]);
	}

	if ( !_tcscmp(m_szRequestTxt[siSlot],szAnswer) )
	{
		// 무혐의 처리시 의뢰자를 1분간 감옥에 보낸다.
		
		SI32 requestid = pclServer->pclCM->GetIDFromName(m_szRequestName[siSlot]);
		if (pclClient->pclCM->IsValidID(requestid))
		{
			if (pclClient->pclCM->CR[requestid]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
			{
				TCHAR* pText = GetTxtFromMgr(5087);
				PrisonUser(m_szRequestName[siSlot],0,NOSUSPICTION_PRISON_TIME,pText);

				if ( IsPC(requestid) )
				{
					pclServer->SendServerResponseMsg(0, SRVAL_AUTOHUNTING_FAIL_NOSUSPICION,  0, 0, requestid);
				}
			}

			DeletePerson(siSlot);

			return true ;
		}
		// 의뢰자가 로그아웃을 한 경우도 감옥에 보내기 위해 코드를 추가함. - by LEEKH 2007.10.11
		else
		{
			TCHAR* pText = GetTxtFromMgr(5087);
			PrisonUser(m_szRequestName[siSlot],0,NOSUSPICTION_PRISON_TIME,pText);

			DeletePerson(siSlot);

			return true ;
		}

	}
	else
	{
		m_siCheckNum[siSlot] ++ ;

		SI32 suspectid = pclServer->pclCM->GetIDFromName(m_szSuspectName[siSlot]);
		SI32 requestid = pclServer->pclCM->GetIDFromName(m_szRequestName[siSlot]);

		if ( m_siCheckNum[siSlot] == 3 && IsPC(suspectid) )
		{
			TCHAR* pText = GetTxtFromMgr(529);
			PrisonUser(m_szSuspectName[siSlot],m_szRequestPersonID[siSlot],PrisonTime,pText);

			// 경험치를 줄여준다.(level * 10)
			SI16 penalty = pclminister->m_siPrisonerPenalty ;
			SI32 deleteexp = pclServer->pclCM->CR[suspectid]->pclCI->clIP.GetLevel() * penalty;
			SI32 addexp = deleteexp / 10 ;
			pclServer->pclCM->CR[suspectid]->pclCI->clIP.DecreaseExp( deleteexp );
			// 클라이언트로 정보를 보낸다.
			cltGameMsgResponse_GetExp clinfo(suspectid,  -deleteexp,  0, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_GETEXP, sizeof(clinfo), (BYTE*)&clinfo);
			pclServer->pclCM->CR[suspectid]->SendNetMsg((sPacketHeader*)&clMsg);

			if ( IsPC(requestid) )
			{
				// 경험치를 늘여준다.
				cltCharServer* pclchar = (cltCharServer*)pclServer->pclCM->CR[requestid];
				pclchar->IncreaseExp(addexp);

				pclServer->SendServerResponseMsg(0, SRVAL_AUTOHUNTING_SUCCESS_PRISON, addexp, PrisonTime, requestid);
			}

			*siCheck = m_siCheckNum[siSlot];
			DeletePerson(siSlot);
		}
		else
			*siCheck = m_siCheckNum[siSlot];

		return false ;
	}

//	return false ;
}

void cltAutoHuntingManager::PrisonUser(TCHAR* name,SI32 requestid,SI32 time,TCHAR * reason)
{
	cltServer* pclServer = (cltServer*) pclClient ;
	if ( pclServer == NULL ) return ;

	if (name == NULL || _tcslen(name) <= 0 ) return ;
	if (requestid < 0 ) return ;

	cltGuiltyInfo pclGuiltyInfo ;
	pclGuiltyInfo.Init();
	pclGuiltyInfo.SetTotalPrisonSecond(time*60);
	pclGuiltyInfo.SetLeftPrisonSecond(time*60);
	pclGuiltyInfo.SetPrisonReason(reason);

	SI32 curdate = pclServer->pclTime->clDate.GetDateVary();
	sDBRequest_PrisonUser clMsg(0, curdate, name, requestid, &pclGuiltyInfo);
	pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	
}
