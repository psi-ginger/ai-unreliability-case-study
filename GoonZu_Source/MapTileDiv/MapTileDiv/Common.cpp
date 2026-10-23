#include "stdafx.h"
#include "common.h"

cltTSpr::cltTSpr()
{
	Image = NULL;
}

cltTSpr::~cltTSpr()
{
	if(Image)
	{
		delete [] Image;
		Image = NULL;
	}
}

// spr파일을 불러온다 
BOOL cltTSpr::LoadSpr( char* szfilename )
{
	FILE *fp;

	fp = fopen(szfilename, "rb");

	if( !fp ) return FALSE;
	
	//-------------------------------
	// 헤더를 읽는다. 
	//-------------------------------
	fread( &clHeader, sizeof(cltTSprHeader), 1, fp );
	
	//----------------------------------
	// 필요한 양의 이미지 버퍼를 만든다.
	//----------------------------------
	if( Image ) delete [] Image;
	Image = new unsigned char[ clHeader.siTotalLength ];
	
	//-----------------------------------
	// 이미지를 불러온다.
	//-----------------------------------
	fread( Image, clHeader.siTotalLength, 1, fp );

	// 여기 까지는 기존 spr포멧을 읽어 오는것과 동일..


	//--------------------------------------------
	// 버전이 400인경우 16bit로컬팔레트를 불러온다 
	//--------------------------------------------

	// 현재 그래픽 모드( 565 or 555 )에 따라 적절한 팔레트를 불러온다 
	// spr파일에 565팔레트( 2 * 256 = 512 byte ), 555팔레트( 2 * 256 = 512 byte ) 순서로 저장되어있다

	// 스프라이트 툴 같은데서는 하나의 모드를 정해서 읽어오면 된다. 
	// 저장할때는 물론 565, 555 팔레트 둘다 저장해야된다 
	
	// (여기서는 565모드로 정해서 읽어오는 것으로 했다)
	int mode = 1;

	if( clHeader.siVersion == 400 ) {

		if( mode == 1 ) {
			// 565 모드이면,
			fread( pal, 512, 1, fp);

		} else {
			// 555 모드이면, 565팔레트 스킵하고, 555팔레트를 읽는다 
			fseek( fp, 512, SEEK_CUR );
			fread( pal, 512, 1, fp);
		}

	}
	
	fclose(fp);

    return TRUE;
}

XSpr::XSpr()
{
	m_pBody = NULL;
	m_pImageTag = NULL;
	m_pImage = NULL;
	m_pPal = NULL;
}

XSpr::~XSpr()
{
	if( m_pBody ) delete [] m_pBody;

	m_pImageTag = NULL;
	m_pImage = NULL;
	m_pPal = NULL;
}

