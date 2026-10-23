#include "NPrivateChatDlgMgr.h"
#include "NPrivateChatDlg.h"

#include <CommonLogic.h>

#include "../../../lib/WebHTML/WebHTML.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "MsgType-System.h"

// 욕설 필터
#include "../../Client/AbuseFilter/AbuseFilter.h"

#include "../../Client/client.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"

extern	cltCommonLogic	*pclClient;

CNPrivateChatDlgMgr::CNPrivateChatDlgMgr()
{	
	for ( SI32 i = 0; i < MAX_REJECT_USER_NUM; ++i )
	{
		m_strRejectUserList[ i ][0] = NULL;
	}

	Initialize();
}

CNPrivateChatDlgMgr::~CNPrivateChatDlgMgr()
{
	Destroy();
}

void CNPrivateChatDlgMgr::Initialize()
{
	m_uiCurrentPrivateChatDlgNum = 0;
	m_siMinimizedSlot = 0;
	m_bNewMessage	= false;
	m_bAutoAnswerSwitch = false;
	ZeroMemory( m_szAutoAnswerMsg, MAX_CHAT_DATA_LENGTH );
	return;
}

void CNPrivateChatDlgMgr::Destroy()
{

	return;
}

void CNPrivateChatDlgMgr::SetMinimizeAllDialog(BOOL bMinimize)
{
	cltClient * pclclient = (cltClient*) pclClient;

	SI16 i = 0;

	m_bNewMessage = false;


	for( i = 0; i < MAX_PRIVATE_CHAT_DLG_NUM ; i++ )
	{
		if ( pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ] )
		{
			((CNPrivateChatDlg*) pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->Minimi(bMinimize);
		}		
	}	
	if(bMinimize == FALSE)
		m_siMinimizedSlot = 0;

	return;
}

BOOL CNPrivateChatDlgMgr::IsMinimi()
{
	cltClient * pclclient = (cltClient*) pclClient;

	SI16 i = 0;

	for( i = 0; i < MAX_PRIVATE_CHAT_DLG_NUM ; i++ )
	{
		if ( pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ] )
		{
			if(((CNPrivateChatDlg*) pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->isMini())
				return TRUE;
		}		
	}

	return FALSE;
}

SI32 CNPrivateChatDlgMgr::Push( BOOL bGameMaster, SI32 siCharUnique, TCHAR *strWhisperData )
{
	return 0;
}

SI32 CNPrivateChatDlgMgr::Push( BOOL bGameMaster, SI32 siCharUnique, TCHAR *strUserName, TCHAR *strWhisperData ,SI32 level ,TCHAR * szAccountID)
{
 	cltClient *pclclient = (cltClient *)pclClient;

	if( pclclient->bWatchModeSwitch )
	{
		return PRIVATE_CHAT_FAIL;
	}

	SI32 siFromID = cltCharCommon::CharUniqueInfo[siCharUnique];

	// 귓속말 창 개수가 부족한 경우 그냥 에러 리턴
	if ( m_uiCurrentPrivateChatDlgNum > MAX_PRIVATE_CHAT_DLG_NUM )
	{
		return PRIVATE_CHAT_EXCESS_CHAT_DLG_NUM;
	}

	if ( IsRejectUser( strUserName ) )
	{
		// g_PrivateChatDlg[ 0 ].m_pChatDataSendMgr->PushReject( WHISPER_CHAT_REJECT, siCharUnique );
		return PRIVATE_CHAT_FAIL;
	}

	// 현재 귓속말 창중에서 그 아이디랑 이야기 하는게 있는 경우 ( 창이 hide 된 경우도 체크함. )
	SI32 i = 0;
	for ( i = 0; i < MAX_PRIVATE_CHAT_DLG_NUM; ++i )
	{
		if ( pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ] )
		{
			if ( ((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->IsExist( siCharUnique, strUserName ) )
			{
				if ( strWhisperData == NULL )
				{
					return PRIVATE_CHAT_FAIL;
				}

				if ( ((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->m_strUserName != NULL )
				{
					if ( strUserName != NULL && _tcscmp( strUserName, ((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->m_strUserName ) != 0 )
					{
						TCHAR buffer[256] = TEXT("") ;
						MStrCpy( ((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->m_strUserName, strUserName, MAX_PLAYER_NAME );
						StringCchPrintf(buffer, 256, TEXT("         %s ( Lv.%d )"), strUserName, level );
						((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->SetTitle( buffer );
					}
				}

				if( false == ((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->IsShow() &&
					false == ((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->IsAccept() )
				{
					pclclient->CreateInterface( NMSGBOX0_DLG + i);

					TCHAR *pTitle = GetTxtFromMgr(5752);
					TCHAR *pText = GetTxtFromMgr(5746);
					TCHAR pBuffer[255];
					StringCchPrintf(pBuffer, 255, pText,strUserName);
					stMsgBox MsgBox;

					if( pclclient->m_pDialog[NMSGBOX0_DLG + i] != NULL )
					{
						MsgBox.Set( (CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ], pTitle, pBuffer, MSGBOX_TYPE_YESNO, i );
						((CNMsgBoxDlg*)pclclient->m_pDialog[NMSGBOX0_DLG + i])->SetMsgData((BYTE*)&MsgBox,(BYTE*)strUserName,0);
					}
				}

				if ( _tcslen( strWhisperData ) > 0 )
				{
					if ( pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ] != pclclient->m_pInterfaceMgr->GetFouceMgr() )
					{
						((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->SetTimerOnOff( true );
					}					

					((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->RecvChat( strUserName, strWhisperData, bGameMaster );
					
					((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->SaveChatLog(strUserName, (TCHAR*)pclClient->pclCM->CR[1]->GetName(), strWhisperData);

				}

				if(((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->isMini())
				{
					m_bNewMessage = true;
				}

				if ( m_bAutoAnswerSwitch )
				{
					if ( _tcslen(m_szAutoAnswerMsg) > 0 )
					{
						stWhisperChatDataSend WhisperSendData;
						WhisperSendData.siToID = siCharUnique;
						StringCchCopy( WhisperSendData.szToCharName, MAX_PLAYER_NAME, strUserName );
						StringCchCopy( WhisperSendData.strChatData, MAX_CHAT_DATA_LENGTH, m_szAutoAnswerMsg  );
						SendChat( &WhisperSendData , true );
					}
				}

				return PRIVATE_CHAT_SUCCESS;
			}
		}
	}

	// 현재 귀속말 창 중에는 그 아이디랑 이야기 하는게 없는 경우
	for ( i = 0; i < MAX_PRIVATE_CHAT_DLG_NUM; ++i )
	{
		if ( pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ] == NULL )
		{
			if ( szAccountID == NULL )
			{	
				pclclient->RequestSecretChatByCharUnique( siCharUnique, strWhisperData );
				return PRIVATE_CHAT_FAIL;
			}
		
			pclclient->CreateInterface( NPRIVATECHAT0_DLG + i );
			((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->Set( siCharUnique, strUserName, level );
			((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->MoveDialog(i * 5, i * 17);

			if ( strWhisperData[0] != NULL )	// 메시지가 있다면
			{
				//cyj 귓말 요청자가 운영자이면 무조건 귓말채널개통
				if (bGameMaster)
				{
					((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->Show(TRUE);

					cltGameMsgRequest_AcceptSecretChatChannel clinfo(true, siCharUnique, strUserName);
					cltMsg clMsg(GAMEMSG_REQUEST_ACCEPTSECRETCHATCHANNEL, sizeof(clinfo), (BYTE*)&clinfo);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				else
				{
					pclclient->CreateInterface( NMSGBOX0_DLG + i);

					((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->Show(FALSE);
					// 귓속말을 볼거냐는 메시지 박스 출력

					TCHAR *pTitle = GetTxtFromMgr(5752);
					TCHAR *pText = GetTxtFromMgr(5746);
					TCHAR pBuffer[255];
					StringCchPrintf(pBuffer, 255, pText,strUserName);
					stMsgBox MsgBox;

					MsgBox.Set( (CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ], pTitle, pBuffer, MSGBOX_TYPE_YESNO, i );

					((CNMsgBoxDlg*)pclclient->m_pDialog[NMSGBOX0_DLG + i])->SetMsgData((BYTE*)&MsgBox,(BYTE*)strUserName,0);
					
					if ( m_bAutoAnswerSwitch )
					{
						if ( _tcslen(m_szAutoAnswerMsg) > 0 )
						{
							stWhisperChatDataSend WhisperSendData;
							WhisperSendData.siToID = siCharUnique;
							StringCchCopy( WhisperSendData.szToCharName, MAX_PLAYER_NAME, strUserName );
							StringCchCopy( WhisperSendData.strChatData, MAX_CHAT_DATA_LENGTH, m_szAutoAnswerMsg  );
							SendChat( &WhisperSendData , true );
						}
					}
				}

			
			
				if ( _tcslen( strWhisperData ) > 0 )
				{
					((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->RecvChat( strUserName, strWhisperData, bGameMaster );
					((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->SaveChatLog(strUserName, (TCHAR*)pclClient->pclCM->CR[1]->GetName(), strWhisperData);


					if ( pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ] != pclclient->m_pInterfaceMgr->GetFouceMgr() )
					{
						((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->SetTimerOnOff( true );
					}					
				}
			}


			return PRIVATE_CHAT_SUCCESS;
		}
	}

	return PRIVATE_CHAT_FAIL;
}

// 이 함수를 한 프레임에 여러번 호출될수 있다. 내가 한 프레임동안 2명 이상에게 동시에 귓속말을 보낼수도 있으므로..
bool CNPrivateChatDlgMgr::Pop( stWhisperChatDataSend *pWhisperChatDataSend )
{
	if ( pWhisperChatDataSend == NULL )
	{
		return false;
	}

	deque< stWhisperChatDataSend >::size_type DequeSize;
	DequeSize = m_ChatDataDeque.size();

	if ( DequeSize < 1 )
	{
		return false;
	}

	deque< stWhisperChatDataSend >::iterator itr;
	
	itr = m_ChatDataDeque.begin();
	memcpy( pWhisperChatDataSend, &(*itr), sizeof( stWhisperChatDataSend ) );

	m_ChatDataDeque.pop_front();

	return true;
}

void CNPrivateChatDlgMgr::AddRejectUser( TCHAR *strNickName )
{
	if(  strNickName == NULL )
	{
		return;
	}

	for ( SI32 i = 0; i < MAX_REJECT_USER_NUM; ++i )
	{
		if ( m_strRejectUserList[ i ][ 0 ] != NULL )
		{
			if ( _tcscmp( m_strRejectUserList[ i ], strNickName ) == 0 )
			{
				return;
			}			
		}		
	}

	for ( i = 0; i < MAX_REJECT_USER_NUM; ++i )
	{
		if ( m_strRejectUserList[ i ][ 0 ] == NULL )
		{
			MStrCpy( m_strRejectUserList[ i ], strNickName, MAX_PLAYER_NAME );
			return;
		}		
	}

	return;
}

void CNPrivateChatDlgMgr::DelRejectUser( TCHAR *strNickName )
{
	if ( strNickName == NULL )
	{
		return;
	}

	for ( SI32 i = 0; i < MAX_REJECT_USER_NUM; ++i )
	{
		if ( m_strRejectUserList[ i ][ 0 ] != NULL )
		{			
			if ( _tcscmp( m_strRejectUserList[ i ], strNickName ) == 0 )
			{
				m_strRejectUserList[ i ][ 0 ] = NULL;
			}
		}
	}

	return;
}

bool CNPrivateChatDlgMgr::IsRejectUser( TCHAR *strNickName )
{
	for ( SI32 i = 0; i < MAX_REJECT_USER_NUM; ++i )
	{
		if ( m_strRejectUserList[ i ][ 0 ] != NULL )
		{			
			if ( _tcscmp( m_strRejectUserList[ i ], strNickName ) == 0 )
			{
				return true;
			}
		}
	}

	return false;
}

void CNPrivateChatDlgMgr::SendChat( stWhisperChatDataSend *pWhisperChatDataSend, bool bAutoAnswer )
{
	if ( m_bAutoAnswerSwitch == true && bAutoAnswer == false )
	{
		cltClient* pclclient = (cltClient*)pclClient;
		TCHAR* pTitle = GetTxtFromMgr( 10021 );
		TCHAR* pText = GetTxtFromMgr( 10020 );
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
	}
	else
	{
		m_ChatDataDeque.push_back( *pWhisperChatDataSend );
	}
}



bool CNPrivateChatDlgMgr::SetAcceptRefuseMsg(SI32 siCharUnique,TCHAR* strUserName ,TCHAR* buffer) //[영진]
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 i = 0;
	for ( i = 0; i < MAX_PRIVATE_CHAT_DLG_NUM; ++i )
	{
		if ( pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ] )
		{
			if ( ((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->IsExist( siCharUnique, strUserName ) )
			{
				((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->RecvChat(TEXT(""),buffer, FALSE);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CNPrivateChatDlgMgr::FaildPush(SI32 CharUnique)
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 i = 0;
	for ( i = 0; i < MAX_PRIVATE_CHAT_DLG_NUM; ++i )
	{
		if ( pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ] )
		{
			if ( ((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->IsExist(CharUnique,NULL) )
			{
				((CNPrivateChatDlg*)pclclient->m_pDialog[ NPRIVATECHAT0_DLG + i ])->ReturnFalseSend();
                				
				return;
			}
		}
	}
}

TCHAR* CNPrivateChatDlgMgr::GetAutoAnswerMsg()
{
	return (TCHAR*)m_szAutoAnswerMsg;
}
void CNPrivateChatDlgMgr::SetAutoAnswerMsg( TCHAR* msg )
{
	if ( msg )
	{
		StringCchCopy( m_szAutoAnswerMsg, MAX_CHAT_DATA_LENGTH, msg );
	}
}
bool CNPrivateChatDlgMgr::GetAutoAnswerSwitch()
{
	return m_bAutoAnswerSwitch;
}
void CNPrivateChatDlgMgr::SetAutoAnswerSwitch( bool autoanswer )
{
	m_bAutoAnswerSwitch = autoanswer;
}