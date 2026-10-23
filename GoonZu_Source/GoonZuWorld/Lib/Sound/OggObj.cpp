#include "OggObj.h"
#include "Directives.h"
#include <ddraw.h>

COggObj::COggObj()
{
    pDS					  = NULL;
    bInitialized		  = false;
    bReleaseDS			  = false;

	sound.bFileOpened     = false;
	sound.pDSB            = NULL;
	sound.bLoop           = false;

	memset( m_strFileName, NULL, sizeof( m_strFileName ) );

	m_lVolume = BASE_BACKGROUND_SOUND_VOLUME;
	
}

COggObj::~COggObj()
{
    if (bReleaseDS && pDS)
	{
        pDS->Release();
	}
}

bool COggObj::InitDirectSound( HWND hWnd )
{
	HRESULT hr;
	
	if ( FAILED( hr = DirectSoundCreate8( NULL, &pDS, NULL ) ) )
	{
		/*
		MessageBox(NULL, TEXT("create dsound error"), TEXT("Create DSound Error"), MB_OK|MB_TOPMOST );
		*/
        return bInitialized = false;
	}
	
	pDS->Initialize(NULL);
	
	//pDS->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );
	pDS->SetCooperativeLevel( hWnd, DSSCL_NORMAL  );
	
	bReleaseDS = true;

	m_hWnd = hWnd;

    return bInitialized = true;
}

bool COggObj::OpenOgg( TCHAR *strFileName )
{
    if ( ! bInitialized )
	{
		return false;
	}

	if ( IsPlaying() )
	{
		Stop();
		Close();
	
		sound.bFileOpened     = false;
		sound.pDSB            = NULL;
		sound.bLoop           = false;

		memset( m_strFileName, NULL, sizeof( m_strFileName ) );

		/*
		if (bReleaseDS && pDS)
		{
			pDS->Release();
		}

		InitDirectSound( m_hWnd );
		*/
	}

	if (bReleaseDS && pDS)
	{
		pDS->Release();
	}
	
	InitDirectSound( m_hWnd );
	
	LPDIRECTSOUNDBUFFER  pTemp;           // the temp buffer

    FILE    *f;

    f = _tfopen( strFileName, TEXT("rb") );

    if( ! f )
	{
		return false;
	}

    ov_open(f, &vf, NULL, 0);

    // ok now the tricky part

    // the vorbis_info struct keeps the most of the interesting format info
    vorbis_info *vi = ov_info( &vf,-1 );
	

    // set the wave format
	
	WAVEFORMATEX		wfm;

    memset(&wfm, 0, sizeof(wfm));

    wfm.cbSize          = sizeof(wfm);
    wfm.nChannels       = vi->channels;
    wfm.wBitsPerSample  = 16;                    // ogg vorbis is always 16 bit
    wfm.nSamplesPerSec  = vi->rate;
    wfm.nAvgBytesPerSec = wfm.nSamplesPerSec*wfm.nChannels*2;
    wfm.nBlockAlign     = 2*wfm.nChannels;
    wfm.wFormatTag      = 1;

	DWORD Size =  (DWORD)(ov_time_total( &vf, -1 ) * wfm.nSamplesPerSec*wfm.nChannels*2);

    // set up the buffer
    DSBUFFERDESC desc;

    desc.dwSize         = sizeof(desc);
    //desc.dwFlags        = 0;
	desc.dwFlags        = DSBCAPS_CTRLVOLUME;
	 
    desc.lpwfxFormat    = &wfm;
	desc.dwReserved     = 0;
	desc.dwBufferBytes  = (DWORD)Size;
    //desc.dwBufferBytes  = MAX_OGG_FILE_SIZE;


	//CREATE TEMPORARY BUFFER
    pDS->CreateSoundBuffer(&desc, &pTemp, NULL );

    // fill the buffer

	//DWORD   size = MAX_OGG_FILE_SIZE;
	DWORD   size = (DWORD)Size;
    DWORD   pos = 0;
    int     sec = 0;
    int     ret = 1;

    char    *buf;
    char    *buf2;

    pTemp->Lock(0, size, (LPVOID*)&buf, &size, NULL, NULL, DSBLOCK_ENTIREBUFFER);
    
    // now read in the bits
    while(ret && pos<size)
    {
        ret = ov_read(&vf, buf+pos, size-pos, 0, 2, 1, &sec);
        pos += ret;
    }

    pTemp->Unlock( buf, size, NULL, NULL );

	//CREATE BUFFER TO MATCH ACTUAL SIZE OF FILE
    desc.dwBufferBytes  = pos;


    if ( DS_OK != pDS->CreateSoundBuffer( &desc, (LPDIRECTSOUNDBUFFER*)&sound.pDSB, NULL ) )
	{
		MessageBox( NULL, TEXT("CreawteSoundBuffer Error"), TEXT("AeF"), MB_OK|MB_TOPMOST );
	}


	if(sound.pDSB == NULL)return false;

	sound.pDSB->Lock(0, pos, (LPVOID*)&buf2, &pos, NULL, NULL, DSBLOCK_ENTIREBUFFER);

	SetSoundVolume( m_lVolume );
	//sound.pDSB->SetVolume( m_lVolume );

    CopyMemory(buf2,buf,pos);


	sound.pDSB->Unlock( buf2, size, NULL, NULL );
	
	//DUMP THE TEMP BUFFER
	pTemp->Release();
	
	sound.nCurSection			= 0;
    sound.nLastSection			= 0;
	sound.actualSize			= pos;
	sound.bLoop					= false;	
	
	ov_clear(&vf);
	fclose(f);

	buf=NULL;

	StringCchCopy( m_strFileName, _MAX_PATH, strFileName );

		

    return sound.bFileOpened = true;
}

void COggObj::Close()
{
	if ( IsPlaying() )
	{
		Close();
	}

	sound.bFileOpened = false;

	if ( sound.pDSB )
	{
		sound.pDSB->Release();
	}
}


void COggObj::Play( int siVolume, bool loop )
{
    if ( ! bInitialized )
        return;

    if ( ! sound.bFileOpened )
		return;	

	if ( ! sound.pDSB )
		return;

	if ( IsPlaying() )
	{
		sound.pDSB->SetCurrentPosition(0);
		Stop();
	}

	SetSoundVolume( siVolume );

	sound.bLoop = loop;
	if ( loop )
	{
		sound.pDSB->Play( 0, 0, DSBPLAY_LOOPING );
	}
	else
	{
		sound.pDSB->Play( 0, 0, 0 );
	}
}

bool COggObj::IsPlaying()
{
	if ( ! sound.pDSB )
	{
		return false;
	}
	
	DWORD dwStatus;
	DWORD pos;

	sound.pDSB->GetStatus( &dwStatus );

	if(dwStatus & DSBSTATUS_PLAYING)
	{
		sound.pDSB->GetCurrentPosition( &pos,NULL );
		if( pos >= sound.actualSize )
		{
			if( ! sound.bLoop )
			{
				Stop();
				return false;
			}
			else
			{
				sound.pDSB->SetCurrentPosition(0);
				return true;
			}
		}
		return true;
	}
	else if(sound.bLoop)
	{
		Play( m_lVolume, true );
		return true;
	}

	return false;
}

void COggObj::Stop()
{
    if (!bInitialized)
        return;

    if (!sound.bFileOpened)
        return;

	if (!sound.pDSB)
		return;
	
    sound.pDSB->Stop();
	sound.bLoop=false;
}

long COggObj::GetSoundVolume()
{
	if ( m_lVolume != -1000 )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("Volume != -1000"), TEXT(""), MB_OK|MB_TOPMOST );
	//tsoi 	printf(TEXT("set"));
#endif
	}
	//return (m_lVolume / 10.0f) + 1;	
	return m_lVolume;
}	

void COggObj::SetSoundVolume( int siVolume )
{
	if(sound.pDSB == NULL) return;
	/*

	if ( siVolume != -1000 )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("Volume != -1000"), TEXT(""), MB_OK|MB_TOPMOST );
		//printf(TEXT("set"));
#endif
	}
*/
	m_lVolume = (long)siVolume;

	if ( DS_OK != sound.pDSB->SetVolume( m_lVolume ) )
	{
		MessageBox(NULL, TEXT("error"), TEXT("error"), MB_OK|MB_TOPMOST );
	}
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool COggObj::IsSameFile( TCHAR *strFileName )
//	Desc : 현재 버퍼에 있는 사운드 파일과 같은 파일인지 확인
//	Date : 2003. 07. 28
//	Update : 
//--------------------------------------------------------------------------------------------
bool COggObj::IsSameFile( TCHAR *strFileName )
{
	if ( m_strFileName == NULL )
	{
		return false;
	}
	
	if ( _tcscmp( m_strFileName, strFileName ) == 0 )
	{
		return true;
	}

	return false;
}
