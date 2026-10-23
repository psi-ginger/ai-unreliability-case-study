#ifndef _OGGOBJ_H_
#define _OGGOBJ_H_

#include	"BaseDefineSoundLibInfo.h"
#include    <windows.h>                 // from your fave include dir ;)
#include    <mmreg.h>

#include    <dsound.h>                  // from the directx 8 sdk

#include    "codec.h"					// from the vorbis sdk
#include    "vorbisfile.h"				  // also :)

#define     MAX_OGG_FILE_SIZE   35000000        // buffer length

class COggObj  
{
public:
	COggObj();
	~COggObj();

	bool InitDirectSound( HWND hWnd );// initialize dsound ..
    inline void UseDirectSound( LPDIRECTSOUND8 _pDS ) { pDS = _pDS; }// .. or use already initialized
    bool OpenOgg( TCHAR *strFileName ); // this opens an oggvorbis for playing    
	void Close(); //close specific file
    void Play( int siVolume, bool loop = false ); // play it again sam
    void Stop(); // stop it
    bool IsPlaying();//
	
	long GetSoundVolume();								// 현재 사운드 크기를 얻어줌
	void SetSoundVolume( int siVolume );

	bool IsSameFile( TCHAR *strFileName );				// 같은 파일인지 확인					

	

private:
	TCHAR		m_strFileName[ _MAX_PATH ];

	bool        bInitialized;           // initialized?

    bool        bReleaseDS;             // release ds by ourselves?

	LONG		m_lVolume;

    LPDIRECTSOUND8 pDS;                    // the directsound 8 object

	struct soundFile
	{
		LPDIRECTSOUNDBUFFER8  pDSB;           // the buffer
	    bool        bFileOpened;            // have we opened an ogg yet?
	    bool        bLoop;                  // loop?
		int         nLastSection,           // which half of the buffer are/were
	                nCurSection;            // we playing?
		unsigned int actualSize;			// actual size of buffer
	};

	soundFile	sound;

    OggVorbis_File vf;                     // for the vorbisfile interface
											//tf is temporary for checking file size
	HWND		m_hWnd;

};

#endif