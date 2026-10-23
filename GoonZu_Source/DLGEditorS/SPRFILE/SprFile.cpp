#include "stdafx.h"
#include "SprFile.h"


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

void cltTSpr::ClearImageData()
{
    if(Image)
	{
		delete [] Image;
		Image = NULL;
		clHeader.siVersion		= NSPR_VERSION;
		clHeader.siSprType		= SPRTYPE_SPR;
		clHeader.siXsize			= 0;
		clHeader.siYsize			= 0;	
		clHeader.siHRes			= 0;
		clHeader.siVRes			= 0;

		clHeader.siTotalLength	= 0;
		clHeader.siFontNum		= 0;
	}
}

// spr파일을 불러온다 
BOOL cltTSpr::LoadSpr( char* szfilename )
{
	FILE *fp;

	if(szfilename == NULL || strcmp(szfilename, "") == 0)
	{
		return FALSE;
	}
		
	fp = fopen(szfilename, "rb");

	if( !fp )
	{
		CString strTmp = szfilename;
		strTmp += " 파일이 없습니다..! 프로그램의 셋팅을 확인하고 다시 시작하세요!!";
		AfxMessageBox(strTmp);
		return FALSE;
	}
	
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
