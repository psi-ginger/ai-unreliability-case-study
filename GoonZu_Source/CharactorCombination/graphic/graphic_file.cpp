#include <graphic.h>
#include "EtcTool.h"

//-------------------------------------------------------------------------------------------
// 설명 : cltTSpr 화일의 이미지를 저장하기 위해 만든 메모리를 해제해 준다.
//-------------------------------------------------------------------------------------------
BOOL cltGrp::FreeSpr(cltTSpr& xspr)
{
	if(xspr.Image==NULL)
	{
		return FALSE;
	}

	delete [] xspr.Image;

	xspr.Image=NULL;

	return TRUE;
}

BOOL cltGrp::LoadSpr(char* szfilename, cltTSpr* pspr)
{
	int version;
	FILE *fp;

	fp=fopen(szfilename, "rb");

	if(!fp)
	{
		MsgBox("NoFile", "FileName:[%s]", szfilename);
		return FALSE;
	}

	cltTSprHeader clHeader;
	
	//-------------------------------
	// 헤더를 읽는다. 
	//-------------------------------
	fread(&pspr->clHeader, sizeof(cltTSprHeader), 1, fp);
	
	//----------------------------------
	// 필요한 양의 이미지 버퍼를 만든다.
	//----------------------------------
	if( pspr->Image ) delete [] pspr->Image;
	pspr->Image = new unsigned char[pspr->clHeader.siTotalLength];
	
	if(!pspr->Image)
	{
		MsgBox("NotEnoughMemory", "Size:[%d]", pspr->clHeader.siTotalLength);
		fclose( fp );
		return FALSE;
	}
	
	//-----------------------------------
	// 이미지를 불러온다.
	//-----------------------------------
	fread(pspr->Image, pspr->clHeader.siTotalLength, 1, fp);

	//-----------------------------------
	// 로컬팔레트를 불러온다 
	//-----------------------------------
	if( pspr->clHeader.siVersion == 400 ) {
		if( g_graphics.m_rgbmode == RGB16_555 ) fseek( fp, 512, SEEK_CUR );
		fread(pspr->pal, 512, 1, fp);
	}
	
	fclose(fp);

    return TRUE;
}

BOOL cltGrp::LoadMprToXpr(char* szfilename, cltTSpr* pspr)
{
	unsigned char buffer[ 768 * 1024 ];

	int version;
	FILE *fp;

	fp=fopen(szfilename, "rb");

	if(!fp)
	{
		MsgBox("NoFile", "FileName:[%s]", szfilename);
		return FALSE;
	}

	
	fread(&version, 4, 1, fp);

	if(version == TSPR_VERSION)
	{
		cltTSprHeader clHeader;

		fseek(fp, 0, SEEK_SET);
		
		//-------------------------------
		// 헤더를 읽는다. 
		//-------------------------------
		fread(&pspr->clHeader, sizeof(cltTSprHeader), 1, fp);
				
		
		//-----------------------------------
		// 이미지를 불러온다.
		//-----------------------------------
		fread(buffer, pspr->clHeader.siTotalLength, 1, fp);
		
		fclose(fp);

		if( pspr->Image ) delete [] pspr->Image;
		pspr->Image = new unsigned char[pspr->clHeader.siTotalLength << 1];

		if(!pspr->Image)
		{
			MsgBox("NotEnoughMemory", "Size:[%d]", pspr->clHeader.siTotalLength << 1);
			return FALSE;
		}


		int i, j;

		UI08 *psrc = buffer;
		RGB16 *pdest = (RGB16*)pspr->Image;
		RGB16 *pColorTable = (RGB16 *)g_graphics.m_pCurrentColorTablePtr;

		for( i = 0; i < pspr->clHeader.siTotalLength; ++i ) {

			if( psrc[ i ] == COLOR_TRANS ) {

				pdest[ i ] = RGB16_TRANS;

				++i;

				pdest[ i ] = (RGB16)psrc[ i ];

			} else {

				pdest[ i ] = (RGB16)psrc[ i ];

				for( j = 1; j <= psrc[ i ]; ++j ) {

					pdest[ i + j ] = pColorTable[ psrc[ i + j ] ];

					if( pdest[ i + j ] == RGB16_TRANS ) pdest[ i + j ]++;
				}

				i += j - 1;
			}
			
		}

	}
	
    return TRUE;

}


//-------------------------------------------------------------------------------------------
// 설명 : cltTSpr 화일을 메모리 상으로 불러온다.
//-------------------------------------------------------------------------------------------

BOOL cltGrp::EncodeSpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey)
{
	int i, j;
	UI08 color;
	int whitenumber = 0;
	int length = 0;

	
	for(i=0;i < ysize;i++)
	{
		 for(j=0;j<xsize;j++)
		 {
			 color=src[j+i*xsize];
			 
			 if(color==colorkey)
			 {
				 whitenumber++;
				 // 가로 축의 끝에서 나오면,
				 if(j==(xsize-1))
				 {
					 dest[length] = COLOR_TRANS;
					 length++;
					 dest[length]=whitenumber;
					 length++;
					 whitenumber=0;
				 }
				 // 투명색이 255개가 되면, 
				 else if(whitenumber>=255)
				 {
					 dest[length]=COLOR_TRANS;
					 length++;
					 dest[length]=whitenumber;
					 length++;
					 whitenumber=0;
				 }
				 
			 }
			 else
			 {
				 if(whitenumber)
				 {
					 dest[length]=COLOR_TRANS;
					 length++;
					 dest[length]=whitenumber;
					 length++;
					 whitenumber=0;
				 }
				 dest[length]=color;
				 length++;
			 }
		 }
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
// 설명 : 데이타를 cltTSpr 화일로 저장한다.
// filename : 화일 이름
// spr      : 스프라이트에 대한 정보.
// image    : 저장할 이미지.
//-------------------------------------------------------------------------------------------
BOOL cltGrp::SaveSpr(CHAR* filename, cltTSpr& spr, UI08 *image)
{
	SI32 i, j, k, l;
	SI32 length;
	SI32 xcount;
	SI32 ycount;
	SI32 whitenumber=0;
	UI08 *start;
	UI08 color;
	UI08 *temp;	 
	FILE *fp;
	
	spr.clHeader.siSprType = SPRTYPE_SPR;
	spr.clHeader.siTotalLength = 0;
	xcount = spr.clHeader.siHRes / spr.clHeader.siXsize;
	ycount = spr.clHeader.siVRes / spr.clHeader.siYsize;
	
	temp = new unsigned char [spr.clHeader.siHRes * spr.clHeader.siVRes * 2];
	
	
	if(!temp)return FALSE;
	
	for( i = 0; i < ycount; i++)
	{
		for( j = 0; j < xcount; j++)
		{
			spr.clHeader.clImgInfo[j + i * xcount].siStartPos  = spr.clHeader.siTotalLength;
			length=0;
			
			start = &image[j*spr.clHeader.siXsize+i*spr.clHeader.siYsize*spr.clHeader.siHRes];
			
			for(k=0;k<spr.clHeader.siYsize;k++)
				for(l=0;l<spr.clHeader.siXsize;l++)
				{
					color = start[l+k*spr.clHeader.siHRes];
					
					if(color==COLOR_TRANS)
					{
						whitenumber++;
						// 가로 축의 끝에서 나오면,
						if(l==(spr.clHeader.siXsize-1))
						{
							temp[spr.clHeader.siTotalLength+length]=COLOR_TRANS;
							length++;
							temp[spr.clHeader.siTotalLength+length]=whitenumber;
							length++;
							whitenumber=0;
						}
						// 투명색이 255개가 되면, 
						else if(whitenumber>=255)
						{
							temp[spr.clHeader.siTotalLength+length]=COLOR_TRANS;
							length++;
							temp[spr.clHeader.siTotalLength+length]=whitenumber;
							length++;
							whitenumber=0;
						}
						
					}
					else
					{
						if(whitenumber)
						{
							temp[spr.clHeader.siTotalLength+length]=COLOR_TRANS;
							length++;
							temp[spr.clHeader.siTotalLength+length]=whitenumber;
							length++;
							whitenumber=0;
						}
						temp[spr.clHeader.siTotalLength+length]=color;
						length++;
					}
				}
				spr.clHeader.clImgInfo[j+i*xcount].siLength = length;
				spr.clHeader.siTotalLength +=length;
				
		}
	}

	fp=fopen(filename, "wb");

	if(!fp)
		return 0;

	fwrite(&spr.clHeader,  sizeof(cltTSprHeader),		1, fp);
	fwrite(temp,			spr.clHeader.siTotalLength, 1, fp);
	fclose(fp);

	return TRUE;
}


void cltGrp::record(int count, unsigned char color, FILE* fp)
{
   if(count>1 || color>=192)
   {
	   fputc(192+count, fp);
	   fputc(color, fp);
   }
   else
   {
	   fputc(color, fp);
   }
}

int cltGrp::SavePCX(PCX & pcx, char *filename)
{
	FILE *fp;
    int i, j;

	fp=fopen(filename,"wb");
	if(fp==NULL) return 0;

	pcx.Head.maker= 10;
	pcx.Head.ver  =  5;
	pcx.Head.encode= 1;
	pcx.Head.bpp   = 8;
	pcx.Head.xmin  = 0;
	pcx.Head.ymin  = 0;

	for(i=0;i<48;i++)
	  pcx.Head.palette[i]=0;

	pcx.Head.vmode   = 0;
	pcx.Head.nplanes = 1;
	pcx.Head.palette_type = 0;

	for(i=0;i<54;i++)
	  pcx.Head.unused[i]=0;

	fwrite(&pcx.Head,sizeof(_head),1,fp);	

   unsigned char da1, color, count=0;
   int xsize=pcx.Head.hres;
   int ysize=pcx.Head.vres;
   int color_switch=FALSE;
   int write_switch=FALSE;

   for(i=0;i<ysize;i++)
	  for(j=0;j<xsize;j++)
	  {
		 da1=pcx.Image[j+i*xsize];

		 if(color_switch==FALSE)
		 {
			color=da1;
			count=1;
			color_switch=TRUE;
			if(j==(xsize-1))
			{
			  record(count, color, fp);
			  color_switch=FALSE;
			}
		 }
		 else if(color==da1)
		 {
			count++;
			if(j==(xsize-1)){
			  record(count, color, fp);
			  color_switch=FALSE;
			}
			else if(count>=63){
			  record(count, color, fp);
			  color_switch=FALSE;
			}
		 }
		 else 
		 {
		   record(count, color, fp);
		   color=da1;
		   count=1;
		   color_switch=TRUE;
		   if(j==(xsize-1))
		   {
			  record(count, color, fp);
			  color_switch=FALSE;
			}
		 }
	  }

	  pcx.Tail.identification=12; // always;

	  for(i=0;i<256;i++)
	{
		pcx.Tail.palette[i*3]   = PalEntries[i].peRed;
		pcx.Tail.palette[i*3+1] = PalEntries[i].peGreen;
		pcx.Tail.palette[i*3+2] = PalEntries[i].peBlue;
	}
	  
	fwrite(&pcx.Tail,sizeof(_tail),1,fp);

	fclose(fp);
	return 1;
}

BOOL cltGrp::EncodeMpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey)
{
	int i, j;
	UI08 color;
	int whitenumber = 0;
	int length = 0;
	int colornumber=0;
	int colornumberpos = -1;
	
	
	for(i=0;i < ysize;i++)
	{
		for(j=0;j<xsize;j++)
		{
			color=src[j+i*xsize];
			
			if(color==colorkey)
			{
				whitenumber++;

				if ( colornumberpos > -1 )
				{
					dest[ colornumberpos ] = colornumber;
					colornumber = 0;
					colornumberpos = -1;
				}
				
				// 가로 축의 끝에서 나오면,
				if(j==(xsize-1))
				{
					dest[length] = COLOR_TRANS;
					length++;
					dest[length]=whitenumber;
					length++;
					whitenumber=0;
				}
				// 투명색이 253개가 되면, 
				else if(whitenumber>=253)
				{
					dest[length]=COLOR_TRANS;
					length++;
					dest[length]=whitenumber;
					length++;
					whitenumber=0;
				}
				
			}
			else
			{
				++colornumber;

				if ( colornumberpos < 0 )
				{
					if(whitenumber)
					{
						dest[length]=COLOR_TRANS;
						length++;
						dest[length]=whitenumber;
						length++;
						whitenumber=0;
					}

					colornumberpos = length;
					++length;
				}

				// 가로 축의 끝에서 나오면,
				if( j == ( xsize - 1 ) )
				{
					dest[ colornumberpos ] = colornumber;
					dest[length] = color;
					++length;
					
					colornumber = 0;
					colornumberpos = -1;
				}
				// 색이 253개가 되면, 
				else if( colornumber>=253 )
				{
					dest[ colornumberpos ] = colornumber;
					dest[length] = color;
					++length;
					
					colornumber = 0;
					colornumberpos = -1;
				}
				else
				{
					dest[length]=color;
					++length;
				}
			}
		}
	}
	
	return TRUE;

}

BOOL cltGrp::SaveMpr(CHAR* filename, cltTSpr& spr, UI08 *image)
{
	int i, j, k, l;
	int length;
	int xcount;
	int ycount;
	int whitenumber=0;
	//int colornumber=0;
	//int colornumberpos = -1;
	UI08 *start;
	UI08 color;
	UI08 *temp;	 
	FILE *fp;
	
	spr.clHeader.siSprType = SPRTYPE_TLE;

	spr.clHeader.siTotalLength = 0;
	xcount = spr.clHeader.siHRes / spr.clHeader.siXsize;
	ycount = spr.clHeader.siVRes / spr.clHeader.siYsize;
	
	temp = new unsigned char [spr.clHeader.siHRes * spr.clHeader.siVRes * 2];
	
	if(!temp)return FALSE;
	
	for( i = 0; i < ycount; i++)
	{
		for( j = 0; j < xcount; j++)
		{
			spr.clHeader.clImgInfo[j + i * xcount].siStartPos  = spr.clHeader.siTotalLength;
			length=0;
			int colornumber=0;
			int colornumberpos = -1;
			
			start = &image[ j*spr.clHeader.siXsize + i*spr.clHeader.siYsize*spr.clHeader.siHRes ];
			
			for(k=0;k<spr.clHeader.siYsize;k++)
			{
				for(l=0;l<spr.clHeader.siXsize;l++)
				{
					color = start[l+k*spr.clHeader.siHRes];
					
					if(color==COLOR_TRANS)
					{
						++whitenumber;

						if ( colornumber )
						{
							temp[ colornumberpos ] = colornumber;
							colornumber = 0;
							colornumberpos = -1;
						}

						// 가로 축의 끝에서 나오면,
						if(l==(spr.clHeader.siXsize-1))
						{
							temp[spr.clHeader.siTotalLength+length]=COLOR_TRANS;
							++length;
							temp[spr.clHeader.siTotalLength+length]=whitenumber;
							++length;
							whitenumber=0;
						}
						// 투명색이 253개가 되면, 
						else if(whitenumber>=253)
						{
							temp[spr.clHeader.siTotalLength+length]=COLOR_TRANS;
							++length;
							temp[spr.clHeader.siTotalLength+length]=whitenumber;
							++length;
							whitenumber=0;
						}
						
					}
					else
					{
						++colornumber;

						if ( colornumberpos < 0 )
						{
							if(whitenumber)
							{
								temp[spr.clHeader.siTotalLength+length]=COLOR_TRANS;
								++length;
								temp[spr.clHeader.siTotalLength+length]=whitenumber;
								++length;
								whitenumber=0;
							}

							colornumberpos = spr.clHeader.siTotalLength+length;
							++length;
						}
						
						// 가로 축의 끝에서 나오면,
						if(l==(spr.clHeader.siXsize-1))
						{
							temp[ colornumberpos ] = colornumber;
							temp[spr.clHeader.siTotalLength+length]=color;
							++length;
							
							colornumber = 0;
							colornumberpos = -1;
						}
						// 색이 253개가 되면, 
						else if( colornumber>=253 )
						{
							temp[ colornumberpos ] = colornumber;
							temp[spr.clHeader.siTotalLength+length]=color;
							++length;
							
							colornumber = 0;
							colornumberpos = -1;
						}
						else
						{
							temp[spr.clHeader.siTotalLength+length]=color;
							++length;
						}
						
					}
				}
			}

			spr.clHeader.clImgInfo[j+i*xcount].siLength = length;
			spr.clHeader.siTotalLength +=length;
		}
	}
	
	fp=fopen(filename, "wb");
	
	if( !fp )
	{
		return 0;
	}
	
	fwrite(&spr.clHeader,	sizeof(cltTSprHeader),		1, fp);
	fwrite(temp,			spr.clHeader.siTotalLength, 1, fp);
	fclose(fp);
	
	if ( temp )
	{
		delete [] temp;
		temp = NULL;
	}
	
	return TRUE; 
}