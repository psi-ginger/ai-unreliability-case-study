#ifndef _SOUNDOBJ_H_
#define _SOUNDOBJ_H_

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CSoundObj
//	Desc : CSoundObj 클래스
//		   사운드 각 파일별 객체
//		   주 기능은 wav 파일 load 와 wav 파일 play 기능
//	Date : 2003. 06. 24
//	Update : 2003. 06. 26 - m_bLoopStatus 추가
//							IsPlaying 함수 수정 ( alGetSourcei 함수 이용해서 알아 올 수 있음. )
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include "BaseDefineSoundLibInfo.h"

#include "alc.h"
#include "alut.h"

class CSoundObj  
{
public:
	CSoundObj();										// 생 성 자
	~CSoundObj();										// 소 멸 자

	void Initialize();									// 초 기 화
	void Destroy();										// 종료처리

	void GenerateBuffer();								// 버퍼 생성
	void DeleteBuffer();								// 버퍼 삭제

	void DeleteBufferAndSource();						// 소스와 버퍼 삭제 ( 삭제 순서는 소스 먼저임 )
	
	bool LoadWavFile( TCHAR *strFileName );				// Wav 파일 로딩
	//bool PlayWavFile( TCHAR *strFileName, bool bLoop, float fVolume = BASE_SOUND_VOLUME_RATE );									// Wav 파일 읽어서 play
	bool PlayPosWavFile( TCHAR *strFileName, bool bLoop, float xPos = 0.0f, float fVolume = BASE_SOUND_VOLUME_RATE );	// 위의 함수랑 틀린 점은 스피커 출력 옵션 좌우 앞뒤가 들어간다는 것임
	
	//void Play( bool bLoop, float fVolume = BASE_SOUND_VOLUME_RATE );							// load 된 wav 파일 play
	void PlayPos( bool bLoop, float xPos = 0.0f, float fVolume =BASE_SOUND_VOLUME_RATE );	// 사운드 출력 결정(좌우, 앞뒤)
	void ReWind();										// 이미 로드된 wav 파일을 다시 재생
	void Pause();										// 멈춤
	void Stop();										// play 되고 있다면, 중단 시킴

	bool IsPlaying();									// 사운드가 play 되고 있는지 확인
	bool IsSameFile( TCHAR *strFileName );				// 같은 파일인지 확인					

	
	
	
	float GetSoundVolume();								// 현재 사운드 크기를 얻어줌
	void SetSoundVolume( float fVolumeRate );			// 현재 사운드 크기 설정
	
	

#ifdef _DEBUG
	TCHAR *GetFileName();								// 현재 버퍼에 올라가 있는 파일의 이름을 얻어옮
#endif

private:
	TCHAR			m_strFileName[128];					// wav 파일 이름

	unsigned int	m_Source;
	unsigned int	m_Buffer;

	float			m_fSourcePos[3];					// 좌표 ( X, Y, Z ) X 는 좌측(-) 우측(+) 사운드 결정, Y 는 0.0, Z 는 원근( 1.0 기준으로 작으면 먼거리 크면 가까운 거리, Default = 1.0 )
	float			m_fSourceVel[3];					// 속도 -- 아직 정확히 구현하지 못했음. 후로 미룸. 기본 셑팅으로 0.0 으로 둠

	float			m_fSoundVolume;						// 사운드 크기
	
};

#endif