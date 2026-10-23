#include "HelperManager.h"

#include "Char/CharManager/CharManager.h"
#include "Char/CharServer/Char-Server.h"
#include "../Server.h"
#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Person.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../CommonLogic/MsgType-System.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"

extern cltCommonLogic* pclClient;

// ============================================================================================
// cltHelperManager 클레스.
// ============================================================================================

cltHelperManager::cltHelperManager()
{
	ZeroMemory( this, sizeof(*this) );
}

cltHelperManager::~cltHelperManager()
{

}

void cltHelperManager::Init( void )
{

}

void cltHelperManager::Action( void )
{
	cltServer* pclserver = (cltServer*)pclClient;
	
	static bool DailyQuestionInit = false;
	if( 1 == pclserver->sTime.wHour && false == DailyQuestionInit) 
	{
		DailyQuestionInit = true;
		
		sDBRequest_Question_Daily_Count_Del clMsg;
		cltServer *pclServer = (cltServer *)pclClient;
		pclServer->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );	
	}
	else if( 1 != pclserver->sTime.wHour && true == DailyQuestionInit) 
	{
		DailyQuestionInit = false;
	}
}

void cltHelperManager::CheckQuestion( cltCharServer* _pclChar )
{
	if ( NULL == _pclChar )	return;

	// 한 캐릭이 하나의 질문이 가능.
	for( SI32 Count = 0; Count < MAX_QUESTION_NUM; ++Count )
	{
		 if( _pclChar->pclCI->GetPersonID() == m_cltHelperInfo[Count].GetQuestionPersonID() )
		{
			// 이미 질문을 했으면 꺼져~.
			cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ALREADY_QUESTION, 1 );
			cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
			_pclChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
			return;
		}
	}

	if( DAILY_QUESTION_NUM <= _pclChar->m_uiDailyQuestionCount )
	{
		cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_LIMIT_COUNT_QUESTION, 0 );
		cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
		_pclChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
		return;
	}

	// 이미 질문을 안 했으면 해~.
	cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ALREADY_QUESTION, 0 );
	cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
	_pclChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
}
 
void cltHelperManager::AddQuestion( cltQuestionInfo* _pcltQuestionInfo, SI32 id )
{
//	if( false == PlusQuestion() ) return;

	cltCharServer* pclchar = pclClient->pclCM->GetCharServer(id);
	if ( NULL == pclchar )	return;

	// 하루 이용 가능 횟수 검사
	if( DAILY_QUESTION_NUM <= pclchar->m_uiDailyQuestionCount )
	{ 
		cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_LIMIT_COUNT_QUESTION, 0 );
		cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
		pclchar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
		return;
	}

	SI32 siPersonID = _pcltQuestionInfo->GetQuestionPersonID();

	SI32 EmptyQuestionIndex = -1;
	// 한 캐릭이 하나의 질문이 가능.
	for( SI32 Count = 0; Count < MAX_QUESTION_NUM; ++Count )
	{
		if( (-1 == EmptyQuestionIndex) && (0 == m_cltHelperInfo[Count].GetQuestionPersonID()) )
		{
			EmptyQuestionIndex = Count;
		}
		else if( siPersonID == m_cltHelperInfo[Count].GetQuestionPersonID() )
		{
			// 이미 질문을 했으면 꺼져~.
			cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ALREADY_QUESTION, 0 );
			cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
			pclchar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
			return;
		}
	}

	// 비어있는 곳에 질문 정보를 쉐팅.
	if( -1 != EmptyQuestionIndex )
	{
		memcpy( &m_cltHelperInfo[EmptyQuestionIndex], _pcltQuestionInfo, sizeof(m_cltHelperInfo[Count]) );
		
		// 질문등록 성공을 클라에 알림.
		cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_QUESTION_ADD_SUCCESS, 0 );
		cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
		pclchar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
		
		// 하루 이용횟수를 db에 저장.
		sDBRequest_Question_Daily_Count_Set clMsg( pclchar->GetCharID() , pclchar->pclCI->GetPersonID(), pclchar->m_uiDailyQuestionCount + 1 );
		cltServer *pclServer = (cltServer *)pclClient;
		pclServer->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );	
	}
	else
	{
		// 질문등록 실패를 클라에 알림.
		cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_QUESTION_FULL, 0 );
		cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
		pclchar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
	}
}

void cltHelperManager::DelQuestion( SI32 _siPersonID )
{
	// 질문 삭제.
	for( SI32 Count = 0; Count < MAX_QUESTION_NUM; ++Count )
	{
		if( _siPersonID == m_cltHelperInfo[Count].GetQuestionPersonID() )
		{
			m_cltHelperInfo[Count].Clear();
			return;
		}
	}
}

void cltHelperManager::DelQuestion( cltCharServer* _pclchar, bool ReturnMsg )
{
//	if( false == MinusQuestion() ) return;
	if( NULL  == _pclchar		 ) return;

	// 질문 삭제.
	for( SI32 Count = 0; Count < MAX_QUESTION_NUM; ++Count )
	{
		if( _pclchar->pclCI->GetPersonID() == m_cltHelperInfo[Count].GetQuestionPersonID() )
		{
			m_cltHelperInfo[Count].Clear();

			if( ReturnMsg )
			{
				cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_QUESTION_DEL_SUCCESS, 0 );
				cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
				_pclchar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
			}
						
			return;
		}
	}
}


bool cltHelperManager::PlusQuestion( void )
{
	if( MAX_QUESTION_NUM > m_siQuestionNum )
	{
		++m_siQuestionNum;
		return true;
	}
	else
	{
#ifdef _DEBUG
		//MessageBox( NULL, "더이상 질문 추가하면 안되.", "알림", 0 );
#endif
		return false;
	}
}

bool cltHelperManager::MinusQuestion( void )
{
	if( 0 < m_siQuestionNum )
	{
		--m_siQuestionNum;
		return true;
	}
	else
	{
#ifdef _DEBUG
		//MessageBox( NULL, "삭제할 질문이 없어.", "알림", 0 );
#endif
		return false;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 클라 -> 서버  메시지 함수.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cltHelperManager::AddAnswer( cltCharServer* _pclAnswerChar, SI32 _siQuestionPersonID )
{
	if( NULL  == _pclAnswerChar ) return;
	
	// 같은 캐릭이면 질문을 삭제한다.
	if( _pclAnswerChar->pclCI->GetPersonID() == _siQuestionPersonID )	return;
	
	SI32 QuestionIndex = -1;
	
	// 한 캐릭이 하나의 질문이 가능.
	for( SI32 Count = 0; Count < MAX_QUESTION_NUM; ++Count )
	{
		if( _siQuestionPersonID == m_cltHelperInfo[Count].GetQuestionPersonID() )
		{
			if( m_cltHelperInfo[Count].GetAnswerPersonID() )
			{
				// 답변을 하려는 사람이 이미 답변 중이면.
				cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ALREADY_ANSWER_OTHER, 0 );
				cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
				_pclAnswerChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
				return;
			}
			else
			{
				// 질문 인덱스 번호 저장.
				QuestionIndex = Count;
			}
		}

		if( _pclAnswerChar->pclCI->GetPersonID() == m_cltHelperInfo[Count].GetAnswerPersonID() )
		{
			// 답변을 하려는 사람이 이미 답변 중이면.
			cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ALREADY_ANSWER_ME, 0 );
			cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
			_pclAnswerChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
			return;
		}
	}

	bool bAlreadyQuestioinDel = false;
	if( -1 == QuestionIndex )
	{
		bAlreadyQuestioinDel = true;
	}

	SI32 siQuestionID = pclClient->pclCM->GetIDFromPersonID( _siQuestionPersonID );
	cltCharServer* _pclQuestionChar = pclClient->pclCM->GetCharServer( siQuestionID );
	if( NULL == _pclQuestionChar )	
	{
		bAlreadyQuestioinDel = true;

		DelQuestion( _siQuestionPersonID );
	}

	if(bAlreadyQuestioinDel)
	{
		// 질문이 삭제 되었거나 등록 되있지 않음. 
		cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ALREADY_DEL_QUESTION, 0 );
		cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
		_pclAnswerChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
		return;
	}
	

	// 도움주는 캐릭을 등록.
	m_cltHelperInfo[QuestionIndex].SetAnswerPersonID( _pclAnswerChar->pclCI->GetPersonID() );
	
	// 도움주는 캐릭으로 등록 되었으므로 클라에게 알려주고 답변창을 띄우게 한다.
	cltGameMsgResponse_Add_Answer clinfo( _pclQuestionChar->pclCI->clBI.GetName(), m_cltHelperInfo[QuestionIndex].GetQuestionTile(), _siQuestionPersonID );
	cltMsg clMsg(GAMEMSG_RESPONSE_ADD_ANSWER, sizeof(clinfo), (BYTE*)&clinfo);
	_pclAnswerChar->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltHelperManager::AddAnswerComplete( cltCharServer* _pclAnswerChar, SI32 _siQuestionPersonID, TCHAR* _pszQuestionTitle, TCHAR* _pszQuestionContents )
{
	SI32 siQuestionID = pclClient->pclCM->GetIDFromPersonID( _siQuestionPersonID );
	cltCharServer* _pclQuestionChar = pclClient->pclCM->GetCharServer( siQuestionID );
	if( NULL == _pclQuestionChar )	
	{
		//질문자가 없으면 질문 삭제.
		DelQuestion( _siQuestionPersonID );

		// 질문이 삭제 되었거나 등록 되있지 않음. 
		cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ALREADY_DEL_QUESTION, 0 );
		cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
		_pclAnswerChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
		return;
	}

	SI32 EmptyQuestionIndex = -1;
	// 한 캐릭이 하나의 질문이 가능.
	for( SI32 Count = 0; Count < MAX_QUESTION_NUM; ++Count )
	{
		if( _siQuestionPersonID == m_cltHelperInfo[Count].GetQuestionPersonID() )
		{
			EmptyQuestionIndex = Count;
		}
	}

	if( -1 == EmptyQuestionIndex )	
	{
		// 질문이 삭제 되었거나 등록 되있지 않음. 
		cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ALREADY_DEL_QUESTION, 0 );
		cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
		_pclAnswerChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 	
		return;
	}
	 
	// 답변을 질문자에게 보내준다.
	cltGameMsgResponse_Add_Answer_Complete clinfo( _pclAnswerChar->pclCI->GetPersonID(), (TCHAR*)_pclAnswerChar->pclCI->clBI.GetName(), _pszQuestionTitle, _pszQuestionContents );
	cltMsg clMsg(GAMEMSG_RESPONSE_ADD_ANSWER_COMPLETE, sizeof(clinfo), (BYTE*)&clinfo);
	_pclQuestionChar->SendNetMsg((sPacketHeader*)&clMsg);

	if( NULL  == _pclAnswerChar ) return;
	// 답변을 보냈다고 답변자에게 알림.
	cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ANSWER_SEND_SUCCESS, 0 );
	cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
	_pclAnswerChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
}

void cltHelperManager::AddAnswerCompleteResult( bool _bResult, SI32 _siAnswerCharPersonID, SI32 _siQuestionCharID )
{
	// 만족
	if( _bResult )	
	{
		cltCharServer* pclQuestionChar = pclClient->pclCM->GetCharServer( _siQuestionCharID );
		if( NULL == pclQuestionChar )	return;

		DelQuestion( pclQuestionChar );
	
		SI32 siAnswerCharID = pclClient->pclCM->GetIDFromPersonID( _siAnswerCharPersonID );
		cltCharServer* pclAnswerChar = pclClient->pclCM->GetCharServer( siAnswerCharID );
		if( NULL == pclAnswerChar )	return;

		SI32 siItemUnique = rand()%2 ? 2570 : 2590;
		SI32 siItemNum	  = 1;

		if( pclAnswerChar->IsBlankInv(1, INVMODE_ALL) )
		{
			pclAnswerChar->GiveItem( siItemUnique, siItemNum, INPUTITEMPERSON_OPTION_HELPER, 0 );

			cltGameMsgResponse_Add_Answer_Complete_Result clinfo( true, siItemUnique, siItemNum, (TCHAR*)pclQuestionChar->pclCI->clBI.GetName() );
			cltMsg clMsg(GAMEMSG_RESPONSE_ADD_ANSWER_COMPLETE_RESULT, sizeof(clinfo), (BYTE*)&clinfo);
			pclAnswerChar->SendNetMsg((sPacketHeader*)&clMsg);
		}		
		else
		{
			((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_GACHA_FAIL_FULLINVEN, 100, 0, pclAnswerChar->GetID());
		}
	}
	//불만족
	else
	{
		cltCharServer* pclQuestionChar = pclClient->pclCM->GetCharServer( _siQuestionCharID );
		if( NULL == pclQuestionChar )	return;

		SI32 siAnswerCharID = pclClient->pclCM->GetIDFromPersonID( _siAnswerCharPersonID );
		cltCharServer* pclAnswerChar = pclClient->pclCM->GetCharServer( siAnswerCharID );
		if( NULL == pclAnswerChar )		return;

		DelAnswer( pclAnswerChar );			
		
		cltGameMsgResponse_Add_Answer_Complete_Result clinfo( false, 0, 0, (TCHAR*)pclQuestionChar->pclCI->clBI.GetName() );
		cltMsg clMsg(GAMEMSG_RESPONSE_ADD_ANSWER_COMPLETE_RESULT, sizeof(clinfo), (BYTE*)&clinfo);
		pclAnswerChar->SendNetMsg((sPacketHeader*)&clMsg);
	}
}


void cltHelperManager::DelAnswer( cltCharServer* _pclAnswerChar )
{
	// 한 캐릭이 하나의 질문이 가능.
	for( SI32 Count = 0; Count < MAX_QUESTION_NUM; ++Count )
	{
		if( _pclAnswerChar->pclCI->GetPersonID() == m_cltHelperInfo[Count].GetAnswerPersonID() )
		{
			m_cltHelperInfo[Count].SetAnswerPersonID( 0 );	
			return;
		}
	}
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 서버 -> 클라 메시지 함수.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void cltHelperManager::SendMsgQuestionTileList( SI32 _id )
{
	cltServer* pclserver = (cltServer*)pclClient;

	// 질문 관련 리스트 전체를 보낸다.
	static bool		m_bAleadyAnswer[ SENDMSG_LIST_NUM ];								// 답변자가 있는지 없는지.
	static SI32		m_siQuestionPersonID[ SENDMSG_LIST_NUM ];							// 질문자 personID
	static TCHAR	m_szQuestionTitle[ SENDMSG_LIST_NUM ][ TITLE_LETTER_NUM ];			// 제목
	static TCHAR	m_szName[ SENDMSG_LIST_NUM ][ MAX_PLAYER_NAME ];					// 질문자 이름.

	ZeroMemory(m_bAleadyAnswer,			sizeof(m_bAleadyAnswer));
	ZeroMemory(m_siQuestionPersonID,	sizeof(m_siQuestionPersonID));
	ZeroMemory(m_szQuestionTitle,		sizeof(m_szQuestionTitle));
	ZeroMemory(m_szName,				sizeof(m_szName));

	SI32			siSendMsgCount = 0;													// 한번에 보내는 질문 리스트 수 카운트.
	cltCharServer*	pclchar		   = NULL;
	for( SI32 Count = 0; Count < MAX_QUESTION_NUM; ++Count )
	{
		// 정보 누적.
		if( m_cltHelperInfo[Count].GetQuestionPersonID() )
		{
			m_bAleadyAnswer[ siSendMsgCount ]      = m_cltHelperInfo[Count].GetAnswerPersonID() ? true : false;
			m_siQuestionPersonID[ siSendMsgCount ] = m_cltHelperInfo[Count].GetQuestionPersonID();
			
			memcpy(m_szQuestionTitle[ siSendMsgCount ], m_cltHelperInfo[Count].GetQuestionTile(), sizeof(m_szQuestionTitle[ siSendMsgCount ]) );
			
			pclchar = pclClient->pclCM->GetCharServer( pclClient->pclCM->GetIDFromPersonID( m_siQuestionPersonID[ siSendMsgCount ]) );
			if ( pclchar )	
				memcpy( m_szName[siSendMsgCount], pclchar->pclCI->clBI.GetName(), sizeof(m_szName[siSendMsgCount]) );

			++siSendMsgCount;
		}
		 
		// 메시지가 20개가 차거나 마지막 메시지가 되면.
		if( (0 == (Count+1)%SENDMSG_LIST_NUM) || ((MAX_QUESTION_NUM - 1) == Count) || (siSendMsgCount == SENDMSG_LIST_NUM) )
		{
			cltGameMsgResponse_Question_List_Get clinfo( m_siQuestionPersonID, (TCHAR**)m_szQuestionTitle, (TCHAR**)m_szName, m_bAleadyAnswer );
			cltMsg clMsg(GAMEMSG_RESPONSE_QUESTION_LIST_GET, sizeof(clinfo), (BYTE*)&clinfo);
			pclserver->pclCM->CR[_id]->SendNetMsg((sPacketHeader*)&clMsg);

			ZeroMemory( m_siQuestionPersonID,	sizeof(m_siQuestionPersonID) );
			ZeroMemory( m_szQuestionTitle,		sizeof(m_szQuestionTitle) );
			
			siSendMsgCount = 0;	
		}
	}
}

void cltHelperManager::SendMsgQuestionContents( SI32 _siPersonID, cltCharServer* _pclAnswerChar )
{
	if( NULL  == _pclAnswerChar ) return;

	cltServer* pclserver = (cltServer*)pclClient;

	// 질문자로 질문 내용을 찾아 해당 캐릭에게 질문 내용을 알려줌.
	for( SI32 Count = 0; Count < MAX_QUESTION_NUM; ++Count )
	{
		if( _siPersonID == m_cltHelperInfo[Count].GetQuestionPersonID() )
		{
			cltGameMsgResponse_Question_Contents_Get clinfo( m_cltHelperInfo[Count].GetQuestionContents() );
			cltMsg clMsg(GAMEMSG_RESPONSE_QUESTION_CONTENTS_GET, sizeof(clinfo), (BYTE*)&clinfo);
			_pclAnswerChar->SendNetMsg((sPacketHeader*)&clMsg);

			return;
		}
	}

	// 질문이 지워졌다.
	cltServerResponseValue clreturn( 0, SRVAL_VILLAGE_HELPER, SRVAL_VILLAGE_HELPER_ALREADY_DEL_QUESTION, 0 );
	cltMsg clRtnMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
	_pclAnswerChar->SendNetMsg( (sPacketHeader*)&clRtnMsg ); 
}

