#include "SoundObj.h"
#include "Directives.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CSoundObj::CSoundObj()
//	Desc : 생 성 자
//	Date : 2003. 06. 24
//	Update : 
//--------------------------------------------------------------------------------------------
CSoundObj::CSoundObj()
{
	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CSoundObj::~CSoundObj()
//	Desc : 소 멸 자
//	Date : 2003. 06. 24
//	Update : 
//--------------------------------------------------------------------------------------------
CSoundObj::~CSoundObj()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CSoundObj::Initialize()
//	Desc : 초 기 화
//	Date : 2003. 06. 24
//	Update : 
//--------------------------------------------------------------------------------------------
void CSoundObj::Initialize()
{
	ZeroMemory( m_strFileName, 64 );
	//ZeroMemory( m_strFileName, sizeof(&m_strFileName) );
	
	m_fSourcePos[ 0 ] = 0.0f;
	m_fSourcePos[ 1 ] = 0.0f;
	m_fSourcePos[ 2 ] = 1.0f;

	m_fSourceVel[ 0 ] = 0.0f;
	m_fSourceVel[ 1 ] = 0.0f;
	m_fSourceVel[ 2 ] = 0.0f;

	m_fSoundVolume = BASE_SOUND_VOLUME_RATE;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CSoundObj::Destroy()
//	Desc : 종료 처리
//	Date : 2003. 06. 24
//	Update : 
//--------------------------------------------------------------------------------------------
void CSoundObj::Destroy()
{
	DeleteBufferAndSource();
	
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CSoundObj::DeleteBufferAndSource()
//	Desc : 소스와 버퍼 삭제 ( 삭제 순서는 소스 먼저임 )
//	Date : 2003. 06. 24
//	Update : 
//--------------------------------------------------------------------------------------------
void CSoundObj::DeleteBufferAndSource()
{
	if ( IsPlaying() )
	{
		Stop();
	}

	alDeleteSources( 1, &m_Source );
	alDeleteBuffers( 1, &m_Buffer );

	Initialize();		//SOUND_STOP 하고 파일이름 NULL 로 설정하고...

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CSoundObj::LoadWavFile( TCHAR *strFileName )
//	Desc : wav 파일 로드
//	Date : 2003. 06. 30
//	Update : 
//--------------------------------------------------------------------------------------------
bool CSoundObj::LoadWavFile( TCHAR *strFileName )
{
	char cloop;
	
	// create buffer
	alGetError();							/* clear */

	alGenBuffers( 1, &m_Buffer );
	
	if(alGetError() != AL_NO_ERROR) 
	{
	   return false;
	}

	// create source
	alGetError();							/* clear */

	// load data into buffer
	ALsizei size, freq;
	ALenum format;
	ALvoid *data;

	int error;
	CHAR strDest[256];
	//후에 wav 파일 포맷을 바꿀 경우, 아래의 함수를 alutLoadWAVMemory 로 교체하면 됨

#ifndef _UNICODE
	StringCchCopy( strDest, 256, strFileName);
#else
	WideCharToMultiByte( CP_ACP, 0, strFileName, -1, strDest, 256 , NULL, NULL );
#endif
	
	alutLoadWAVFile( strDest , &format, &data, &size, &freq, &cloop );
	
	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		printf("alutLoadWAVFile error %s", strFileName );
		// Delete Buffers
		alDeleteBuffers( 1, &m_Buffer );
		return false;
	}

	alBufferData ( m_Buffer, format, data, size, freq);

	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		printf("alBufferData buffer error\n");
		alDeleteBuffers( 1, &m_Buffer );
		return false;
	}

	// Unload footsteps.wav
	alutUnloadWAV( format, data, size, freq );
	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		printf("alutUnloadWAV error\n");
		// Delete buffers
		alDeleteBuffers( 1, &m_Buffer );
		return false;
	}

	alGenSources( 1, &m_Source );
	
	if(alGetError() != AL_NO_ERROR)
	{
	   return false;
	}

	/*
	alSourcef( m_Source, AL_PITCH, 1.0f);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("alSourcef 0 AL_PITCH : %d\n", error);
	}

	alSourcef( m_Source, AL_GAIN, 1.0f);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("alSourcef 0 AL_GAIN : %d\n", error);
	}

	m_fSourcePos[0] = xPos;
	m_fSourcePos[1] = 0.0f;
	m_fSourcePos[2] = fNear;

	
	alSourcefv( m_Source, AL_POSITION, m_fSourcePos );
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("alSourcefv 0 AL_POSITION : %d\n", error);
	}
	
	alSourcefv( m_Source,AL_VELOCITY, m_fSourceVel );
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("alSourcefv 0 AL_VELOCITY : %d\n", error);
	}
	*/

	// setup source
	alSourcei( m_Source, AL_BUFFER, m_Buffer );
	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		printf("alSourcei AL_BUFFER 0 error\n");
		return false;
	}
	
	return true;
}

/*
//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CSoundObj::PlayWavFile(TCHAR *strFileName, bool bLoop)
//	Desc : Wav 파일 load 후에 출력
//	Date : 2003. 06. 25
//	Update : 
//--------------------------------------------------------------------------------------------
bool CSoundObj::PlayWavFile(TCHAR *strFileName, bool bLoop, float fVolume )
{
	if ( ! LoadWavFile ( strFileName ) )
	{
		return false;
	}

	if ( strFileName == NULL )
	{
		return false;
	}

	strcpy( m_strFileName, strFileName );
	
	Play( bLoop, fVolume );

	return true;
}
*/

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CSoundObj::PlayWavFile( TCHAR *strFileName, bool bLoop, float xPos, float yPos, float fNear = 1.0 )
//	Desc : 위의 함수랑 틀린 점은 스피커 출력 옵션 좌우 앞뒤가 들어간다는 것임.
//	Date : 2003. 06. 30
//	Update : ;	
//--------------------------------------------------------------------------------------------
bool CSoundObj::PlayPosWavFile( TCHAR *strFileName, bool bLoop, float xPos, float fVolume )
{
	if ( ! LoadWavFile ( strFileName ) )
	{
		return false;
	}

	if ( strFileName == NULL )
	{
		return false;
	}

	StringCchCopy( m_strFileName, 128, strFileName );
	
	PlayPos( bLoop, xPos, fVolume );

	return true;
}

/*
//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CSoundObj::Play( bool bLoop, int siVolume )
//	Desc : load 된 wav 파일 play
//	Date : 2003. 06. 25
//	Update : 
//--------------------------------------------------------------------------------------------
void CSoundObj::Play( bool bLoop, float fVolume )
{
	int error;

	alSourceStop( m_Source );

	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		printf("alSourceStop error 1 \n");
	}
	
	if ( bLoop )
	{
		alSourcei( m_Source, AL_LOOPING, AL_TRUE );
	}
	else
	{
		alSourcei( m_Source, AL_LOOPING, AL_FALSE );
	}

	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		printf(  TEXT("alSourcei AL_BUFFER 0 : error\n") );
	}

	SetSoundVolume( fVolume );
	alSourcef( m_Source, AL_REFERENCE_DISTANCE, m_fSoundVolume );

	alSourcePlay( m_Source );

	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		printf(  TEXT("alSourcePlay 0 error\n") );
	}	

	return;
}
*/

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CSoundObj::ReWind()
//	Desc : 사운드 출력 결정
//		   xPos : 좌 ( 음수 ), 우 ( 양수 ) fNear : 앞 ( 음수 ), 뒤( 양수 )
//	Date : 2003. 06. 30
//	Update : 2003. 07. 07 - 사운드 크기 조정 추가
//--------------------------------------------------------------------------------------------
void CSoundObj::PlayPos( bool bLoop, float xPos, float fVolume )
{
	int error;

	alSourceStop( m_Source );
	
	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		printf("alSourceStop error 1 \n");
	}
	
	if ( bLoop )
	{
		alSourcei( m_Source, AL_LOOPING, AL_TRUE );
	}
	else
	{
		alSourcei( m_Source, AL_LOOPING, AL_FALSE );
	}

	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		//printf(  TEXT("alSourcei AL_BUFFER 0 : error\n") );
	}
	
	/*
	playerPos[0] += distance * (float) sin(angle + playerAngle);
	playerPos[2] -= distance * (float) cos(angle + playerAngle);
	*/
	
	m_fSourcePos[ 0 ] = xPos;			// 좌 우 스피커 출력 좌표 설정
	m_fSourcePos[ 1 ] = 0.0f;
	m_fSourcePos[ 2 ] = 1.0f;

	
	
	alSourcefv( m_Source, AL_POSITION, m_fSourcePos );
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("alSourcefv 0 AL_POSITION : %d\n", error);
	}

	SetSoundVolume( fVolume );
	alSourcef( m_Source, AL_REFERENCE_DISTANCE, m_fSoundVolume );
	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		//printf(  TEXT("alSourcePlay 0 error\n") );
	}
	
	alSourcePlay( m_Source );

	if ( ( error = alGetError()) != AL_NO_ERROR )
	{
		//printf(  TEXT("alSourcePlay 0 error\n") );
	}

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CSoundObj::ReWind()
//	Desc : 이미 로드된 wav 파일을 다시 재생
//	Date : 2003. 06. 30
//	Update : 
//--------------------------------------------------------------------------------------------
void CSoundObj::ReWind()
{
	alSourceRewind( m_Source );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CSoundObj::Pause()
//	Desc : 멈춤
//	Date : 2003. 06. 30
//	Update : 
//--------------------------------------------------------------------------------------------
void CSoundObj::Pause()
{
	alSourcePause( m_Source );
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CSoundObj::Stop()
//	Desc : Play 되고 있다면, Stop 시킴
//	Date : 2003. 06. 25
//	Update : 
//--------------------------------------------------------------------------------------------
void CSoundObj::Stop()
{
	if ( IsPlaying() )
	{
		int error;

		alSourceStop( m_Source );

		if ((error = alGetError()) != AL_NO_ERROR)
		{
			//printf(  TEXT("Stop Error\n") );
		}
	}

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CSoundObj::IsSameFile( TCHAR *strFileName )
//	Desc : 현재 버퍼에 있는 사운드 파일과 같은 파일인지 확인
//	Date : 2003. 06. 25
//	Update : 
//--------------------------------------------------------------------------------------------
bool CSoundObj::IsSameFile( TCHAR *strFileName )
{
	if ( m_strFileName == NULL )
	{
		return false;
	}

	size_t length = 0;
	StringCchLength( m_strFileName, 128 , &length  );
	if ( length < 2 )
	{
		return false;
	}

	if ( _tcscmp( m_strFileName, strFileName ) == 0 )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CSoundObj::IsPlaying()
//	Desc : 현재 버퍼에 있는 사운드 파일이 플레이 중인지 확인
//	Date : 2003. 06. 25
//	Update : 2003. 06. 26 - IsPlaying 함수 수정 ( alGetSourcei 함수 이용해서 알아 올 수 있음. )
//--------------------------------------------------------------------------------------------
bool CSoundObj::IsPlaying()
{
	int iStatus = 0;

	alGetSourcei( m_Source, AL_SOURCE_STATE, &iStatus );

	if ( iStatus == AL_PLAYING )
	{
		return true;
	}
	
	return false;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - float CSoundObj::GetSoundVolume( int siVolumeRate )
//	Desc : 현재 사운드 비율을 얻어줌
//	Date : 2003. 07. 07
//	Update : 
//--------------------------------------------------------------------------------------------
float CSoundObj::GetSoundVolume()
{
	return m_fSoundVolume;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CSoundObj::SetSoundVolume( float fVolumeRate )
//	Desc : m_fSoundVolume 값 설정
//	Date : 2003. 07. 07
//	Update : 
//--------------------------------------------------------------------------------------------
void CSoundObj::SetSoundVolume( float fVolumeRate )
{	
	m_fSoundVolume = fVolumeRate;
	return;
}














//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 실제 release Mode 에서는 필요 없는 코드.
//	Date : 2003. 06. 30
//	Update : 
//--------------------------------------------------------------------------------------------
#ifdef _DEBUG
// 현재 버퍼에 올라가 있는 파일의 이름을 얻어옮
TCHAR *CSoundObj::GetFileName()
{
	return m_strFileName;
}
#endif