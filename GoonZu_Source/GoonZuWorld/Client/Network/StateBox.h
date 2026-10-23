#ifndef _STATEBOX_H
#define _STATEBOX_H

#include "itzbase.h"

enum {
		USERSTATE_DISCONNECTED = 0,
		USERSTATE_FRONT_CONNECTING,			// 프론트 서버로 접속 시도중 
		USERSTATE_FRONT_CONNECTED,			// 프론트 서버 접속 성공, 아이디 입력 중, 인증 대기중
		USERSTATE_FRONT_AUTHOK,				// 인증 성공
		USERSTATE_FRONT_NOTICEOK,			// 공지사항 열람
		USERSTATE_FRONT_SERVERSELECTING,	// 서버 선택 중
		USERSTATE_GAME_CONNECTING,			// 게임 서버로 접속 시도중
		USERSTATE_GAME_CONNECTED,			// 게임 서버 접속 성공, 인증 대기중
		USERSTATE_GAME_AUTHOK,				// 인증 성공
		USERSTATE_GAME_CHARSELECTING,		// 캐릭터 선택 중
		USERSTATE_GAME_PLAYING				// 게임 플레이 중
};


class cltStateBox
{
public:
	cltStateBox() { m_uiState = 0; };
	~cltStateBox() {};

	void			SetState( UI32 state ) { InterlockedExchange( (LPLONG)&m_uiState, state ); };
	UI32			GetState() { return m_uiState; };

public:

	UI32			m_uiState;
};


#endif