#ifndef _SOUNDMGR_H_
#define _SOUNDMGR_H_

#include <Windows.h>

#include <stdio.h>
#include <Conio.h>
#include "BaseDefineSoundLibInfo.h"

/*
#ifndef OPENAL
#define OPENAL
#include <initguid.h>
#include "eax.h"
#endif
*/
//MAX_REFERENCE_DISTANCE : 4131

#pragma comment( lib, "dsound" )

#pragma comment( lib, "ALut" )
#pragma comment( lib, "OpenAL32" )

#pragma comment( lib, "ogg_static" )
#pragma comment( lib, "vorbis_static" )
#pragma comment( lib, "vorbisfile_static" )

class CSoundObj;
class COggObj;

class CSoundMgr  
{
public:
	enum { FILE_NOT_EXIST };								// 에러 코드 정의
	CSoundMgr();											// 생 성 자
	~CSoundMgr();											// 소 멸 자
	
	void Initialize();										// 초 기 화
	void Destroy();											// 종료처리

	bool Create( HWND hWnd );								// 초기화까지 완전히 다 끝나고 객체 생성 까지 끝내기 위해서

	bool SoundDeviceInitialize();							// 사운드 장치 초기화
	int  IncrementEnv();

	
	bool PlayFile( char *strFileName, bool bLoop = false, bool bForceSoundPlay = false, float xPos = 0.0f, float fVolume = BASE_SOUND_VOLUME_RATE, bool bSameSoundPlaying = false );	// 위의 함수랑 틀린 점은 좌표가 추가 되어 있다는 것임. ( xPos : 좌 ( 음수 ), 우 ( 양수 ) ) , 볼륨의 크기( 100 이면 최대 사운드 크기 ), 같은 사운드 다른 버퍼에서 또 생성해서 출력 유무	
	bool PlayBackGroundFile( char *strFileName, bool bLoop = false, bool bForceSoundPlay = false );

	bool GetSoundVolume( char* strFileName, float *pfOutSoundVolume );	// 특정 파일의 현재 사운드 크기를 얻어 옮
	bool GetBackGroundSoundVolume( char *strFileName, long *pOutSoundVolume );	// 위와 같음 ( 배경 음악용 )

	char IsFileAlreadyExist( char *strFileName );			// 기존의 객체에 있는 것인지 확인하는 것.
	char IsBackGroundFileAlreadyExist( char *strFileName );	// 위랑 같음 ( 배경 음악용 )

	bool IsBackGroundFilePlaying();							// 배경음악이 출력중이냐?
	
	void SoundStop( char index );							// 하나의 사운드를 중지 시킨다.
	void AllSoundStop();									// 모든 사운드를 중지 시킨다.	
	void BackGroundSoundStop();								// 현재 음악 사운드 중지 시킨다.
	

	void EffectSoundOn( bool bEffectSoundOn );
	void BackGroundSoundOn( bool bBackGroundSoundOn );

	void SetSoundVolume( float soundvolumepercent  );
	void SetBackGroundSoundVolume( int soundvolume );

	
#ifdef _DEBUG
	// 테스트용 함수들
	bool IsPlaying( int );									// 리스트에 있는 몇 번째 객체가 사운드 출력중인지를 물음 ( 파라미터는 입력값임 )
	void PrintAllFileName();								// 리스트에 있는 모든 wav 파일 이름 출력
#endif



private:	
	HANDLE			m_hSrch;								// 파일 존재 유무 체크 하기 위한 핸들
	WIN32_FIND_DATA m_wfd;

	char			m_cSoundIndexLoop;						// MAX_SOUND_BUFFER_NUM 개 만큼 사운드 넣고 나서는 안 쓰는 파일도 가지고 있으면서, 최근에꺼는 안 지우는 방식으로 해야 되므로... 증가 시키는 방식이 제일 좋을듯... MAX_SOUND_BUFFER_NUM 값과 같아 지면 0으로 다시 만들어서 원형큐 같이 돌게... 최선의 선택...
	char			m_cBackGroundSoundIndexLoop;			// 위와 같음 ( 배경 음악용 )
	
	bool			m_bEffectSoundOn;
	bool			m_bBackGroundSoundOn;


	float			m_fListenerPos[3];
	float			m_fListenerVelocity[3];
	float			m_fListenerOrientation[6];
	
	COggObj			*m_pOggObjList;							// 배경 사운드 객체 리스트
	CSoundObj		*m_pSoundObjList;						// 사운드 객체 리스트

	float m_srEffSoundVolumeLate;
	int m_siBGMSoundVolume;
	
	CRITICAL_SECTION m_SoundListCrit;						// 사운드 로딩할려고 하는데, 또 로딩하는 뭐 그런 중복 되는 경우를 막기 위해서... 일반적으로 리스트에 다 해주는 것이지...
};

#endif