#include <Graphic.h>


//=============================================================================
// IMAGE
//=============================================================================
void cltGrp::PutImg8T(int x, int y, int xsize, int ysize, unsigned char* image)
{
	int i, j, k, index=0;
	UI08* vidptr = Screen8 + x + y * SurfaceWidth;

	for( i = 0; i < ysize; ++i )
	{
		j = 0;

		while( j < xsize )
		{
			if( image[ index ] == COLOR_TRANS )
			{
				++index;

				for( k = 0; k < image[ index ]; ++k )
				{
					vidptr[ j + k ] = COLOR_TRANS;
				}

				j += image[ index++ ];
			}
			else 
			{
				vidptr[ j ]=image[ index++ ];
				j++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

void cltGrp::PutImg8CT(int x, int y, int xsize, int ysize, unsigned char* image)
{
	int i, j, k, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;

	UI08* vidptr=Screen8+x+y*SurfaceWidth;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;

	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j < xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;

				for(k=0;k < image[index];k++)
				{
					if(i>=up_skip && j+k>=left_skip && j+k<xsize-right_skip)
					    vidptr[j+k] = COLOR_TRANS;
				}
				j+=image[index];
				index++;
			}
			else 
			{
				if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				     vidptr[j] = image[index];

				j++;
				index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}


void cltGrp::PutImg8T(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable)
{
	int i, j, k, index=0;

	UI08 *vidptr = Screen8 + y * SurfaceWidth + x;

	 for(i=0;i<ysize;i++)
	 {
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;

			   for(k=0;k<image[index];k++)
				  vidptr[j+k]=COLOR_TRANS;

			   j+=image[index];
			   index++;
			}
			else
			{
			   vidptr[j]=ctable[image[index++]];
			   j++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}


//=============================================================================
// SRPITE DOT
//=============================================================================

void cltGrp::PutSprDot8T(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable)
{
	int i, j,index=0;
	UI08 *vidptr=Screen8 + y * SurfaceWidth + x;

	 for( i = 0; i < ysize; ++i )
	 {
		j = 0;
		while( j < xsize )
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else
			{
				if ( ( j + i ) % 2 )
				{
			       vidptr[ j ]=ctable[ image[ index ] ];
				}

			   index++;
			   j++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}


void cltGrp::PutSprDot8CT(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;

	 for(i=0;i<ysize-down_skip;i++)
	 {
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else
			{
			   if((j+i)%2==0 && i>=up_skip && j>=left_skip && j<xsize-right_skip)
				  vidptr[j]=ctable[image[index]];


			   j++;index++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}

void cltGrp::PutSprDot8RT(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable)
{
	int i, j,index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	 for(i=0;i<ysize;i++)
	 {
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else
			{
				if((j+i)%2)
				   vidptr[xsize-j-1]=ctable[image[index]];

				index++;
			    j++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}



void cltGrp::PutSprDot8CRT(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable)
{
	int i, j, index=0;
	int vary;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	 if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)right_skip=ClipX1-x;
	if(x+xsize>ClipX2)left_skip=x+xsize-ClipX2-1;

	 for(i=0;i<ysize-down_skip;i++)
	 {
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else
			{
				vary=xsize-j-1;
			   if((j+i)%2==0 && i>=up_skip && j>=left_skip && j<xsize-right_skip)
			      vidptr[vary]=ctable[image[index]];

			   j++;index++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}


//=============================================================================
// SPRITE
//=============================================================================
void cltGrp::PutSpr8T(int x, int y, int xsize, int ysize, unsigned char *image)
{
	int i, j,index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	 for(i = 0;i < ysize;i++)
	 {
		j = 0;

		while(j < xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else
			{
			   vidptr[j] = image[index++];
			   j++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}


void cltGrp::PutSpr8RT(int x, int y, int xsize, int ysize, unsigned char *image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	 for(i=0;i<ysize;i++)
	 {
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else
			{
			   vidptr[xsize-j-1]= image[index++];
			   j++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}


void cltGrp::PutSpr8CT(int x, int y, int xsize, int ysize, unsigned char *image)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;

	 for(i=0;i<ysize-down_skip;i++){
		j=0;
		while(j<xsize){
			if(image[index]==COLOR_TRANS){
			   index++;
			   j+=image[index];
			   index++;
			}
			else{
			   if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				  vidptr[j] = image[index];
			   j++;index++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}

void cltGrp::PutSpr8CRT(int x, int y, int xsize, int ysize, unsigned char *image)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	 if(x+xsize-1<ClipX1)return ;
	 if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	 if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)right_skip=ClipX1-x;
	if(x+xsize>ClipX2)left_skip=x+xsize-ClipX2-1;

	 for(i=0;i<ysize-down_skip;i++){
		j=0;
		while(j<xsize){
			if(image[index]==COLOR_TRANS){
			   index++;
			   j+=image[index];
			   index++;
			}
			else{
			   if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
			   vidptr[xsize-j-1]=image[index];

			   j++;index++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}

//=============================================================================
// SPRITE TABLE
//=============================================================================
void cltGrp::PutSpr8T(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable)
{
	int i, j,index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	 for(i=0;i<ysize;i++)
	 {
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else
			{
			   vidptr[j]=ctable[ image[index++] ];
			   j++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}


void cltGrp::PutSpr8CT(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;

	 for(i=0;i<ysize-down_skip;i++){
		j=0;
		while(j<xsize){
			if(image[index]==COLOR_TRANS){
			   index++;
			   j+=image[index];
			   index++;
			}
			else{
			   if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				  vidptr[j]=ctable[image[index]];
			   j++;index++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}

void cltGrp::PutSpr8RT(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char* ctable)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;
	 
	for(i=0;i<ysize;i++)
	 {
		j=0;
		while(j<xsize){
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else{
			   vidptr[xsize-j-1]=ctable[image[index]];
			   j++;  index++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}

void cltGrp::PutSpr8CRT(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char* ctable)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+ x;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)right_skip=ClipX1-x;
	if(x+xsize>ClipX2)left_skip=x+xsize-ClipX2-1;

	 for(i=0;i<ysize-down_skip;i++){
		j=0;
		while(j<xsize){
			if(image[index]==COLOR_TRANS){
			   index++;
			   j+=image[index];
			   index++;
			}
			else{
			   if(i>=up_skip && j>=left_skip && j<xsize-right_skip){
					  vidptr[xsize-j-1]=ctable[image[index]];
			   }

			   j++;index++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}

//=============================================================================
// SPRITE SHADOE
//=============================================================================
//// 그림자를 찍어준다.
void cltGrp::PutSprShadow8T(int x, int y, int xsize, int ysize, unsigned char* image)
{
	int i, j, index=0;
    UI08* vidptr=(UI08*)Screen8+y*SurfaceWidth+x;

	for(i=0;i<ysize;i++)
	{
		j=0;
		
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vidptr[j] = DarkTable[ ShadowAlpha[image[index] ] ][ vidptr[j] ];

				index++;
				j++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}


void cltGrp::PutSprShadow8CT(int x, int y, int xsize, int ysize, unsigned char* image)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08* vidptr=(UI08*)Screen8+y*SurfaceWidth+x;	

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;

	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				{
				    vidptr[j] = DarkTable[ ShadowAlpha[image[index] ] ][ vidptr[j] ];
				}

				index++;
				j++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}



//// 그림자를 찍어준다.
void cltGrp::PutSprShadow8RT(int x, int y, int xsize, int ysize, unsigned char* image)
{
	int i, j, index=0;
    UI08* vidptr=(UI08*)Screen8+y*SurfaceWidth+x;
	SHORT temp;

	for(i=0;i<ysize;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				temp=xsize-j-1;
				vidptr[temp] = DarkTable[ ShadowAlpha[image[index] ] ][ vidptr[temp] ];


				index++;
				j++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}


void cltGrp::PutSprShadow8CRT(int x, int y, int xsize, int ysize, unsigned char* image)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08* vidptr=(UI08*)Screen8+y*SurfaceWidth+x;
	int temp;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	 if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)right_skip=ClipX1-x;
	if(x+xsize>ClipX2)left_skip=x+xsize-ClipX2-1;

	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
	while(j<xsize)
	{
		if(image[index]==COLOR_TRANS)
		{
			index++;
			j+=image[index];
			index++;
		}
		else
		{
			temp=xsize-j-1;
			if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
			{
				vidptr[temp] = DarkTable[ ShadowAlpha[image[index] ] ][ vidptr[temp] ];
				
			}
              
			j++; index++;
		}
	}
	vidptr+=SurfaceWidth;
	}
}


//=============================================================================
// SPRITE LIGHT 
//=============================================================================
void cltGrp::PutSprLight8T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;

	for(i=0;i<ysize;i++)
	{
		j=0;
	while(j<xsize)
	{
		if(image[index]==COLOR_TRANS)
		{
			index++;
			j+=image[index];
			index++;
		}
		else
		{
			vidptr[j]=Palette16[ (PaletteRGB[ vidptr[j] ][0]>>1) + (PaletteRGB[ image[index] ][0]>>1)]
				               [ (PaletteRGB[ vidptr[j] ][1]>>1) + (PaletteRGB[ image[index] ][1]>>1)]
				               [ (PaletteRGB[ vidptr[j] ][2]>>1) + (PaletteRGB[ image[index] ][2]>>1)];
			
			j++; index++;
		}
	}
	vidptr+=SurfaceWidth;
	}
}


void cltGrp::PutSprLight8CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)

{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;

	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;

	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
	while(j<xsize)
	{
		if(image[index]==COLOR_TRANS)
		{
			index++;
			j+=image[index];
			index++;
		}
		else
		{
			if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
			{
				vidptr[j]=Palette16[ (PaletteRGB[ vidptr[j] ][0]>>1) + (PaletteRGB[ image[index] ][0]>>1)]
					               [ (PaletteRGB[ vidptr[j] ][1]>>1) + (PaletteRGB[ image[index] ][1]>>1)]
								   [ (PaletteRGB[ vidptr[j] ][2]>>1) + (PaletteRGB[ image[index] ][2]>>1)];

			}
                
			j++; index++;
		}
	}
	vidptr+=SurfaceWidth;
	}
}


void cltGrp::PutSprLight8RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;

	for(i=0;i<ysize;i++)
	{
		j=0;
	while(j<xsize)
	{
		if(image[index]==COLOR_TRANS)
		{
			index++;
			j+=image[index];
			index++;
		}
		else
		{
			vary=xsize-j-1;

				vidptr[vary]=Palette16[ (PaletteRGB[ vidptr[vary] ][0]>>1) + (PaletteRGB[ image[index] ][0]>>1)]
					                  [ (PaletteRGB[ vidptr[vary] ][1]>>1) + (PaletteRGB[ image[index] ][1]>>1)]
							  	      [ (PaletteRGB[ vidptr[vary] ][2]>>1) + (PaletteRGB[ image[index] ][2]>>1)];

			j++; index++;
		}
	}
	vidptr+=SurfaceWidth;
	}
}



void cltGrp::PutSprLight8CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)

{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;

	 if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)right_skip=ClipX1-x;
	if(x+xsize>ClipX2)left_skip=x+xsize-ClipX2-1;

	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
	while(j<xsize)
	{
		if(image[index]==COLOR_TRANS)
		{
			index++;
			j+=image[index];
			index++;
		}
		else
		{
			vary=xsize-j-1;

			if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
			{

				vidptr[vary]=Palette16[ (PaletteRGB[ vidptr[vary] ][0]>>1) + (PaletteRGB[ image[index] ][0]>>1)]
					                  [ (PaletteRGB[ vidptr[vary] ][1]>>1) + (PaletteRGB[ image[index] ][1]>>1)]
							   	      [ (PaletteRGB[ vidptr[vary] ][2]>>1) + (PaletteRGB[ image[index] ][2]>>1)];

			}
                
			j++; index++;
		}
	}
	vidptr+=SurfaceWidth;
	}
}

//=============================================================================
// SPRITE LIGHT1 
//=============================================================================
void cltGrp::PutSprLight18T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;

	for(i=0;i<ysize;i++)
	{
		j=0;
	while(j<xsize)
	{
		if(image[index]==COLOR_TRANS)
		{
			index++;
			j+=image[index];
			index++;
		}
		else
		{
			vidptr[j]=Palette16[ LightEffect1[PaletteRGB[ vidptr[j] ][0]][PaletteRGB[ image[index] ][0]] ]
				               [ LightEffect1[PaletteRGB[ vidptr[j] ][1]][PaletteRGB[ image[index] ][1]] ]
							   [ LightEffect1[PaletteRGB[ vidptr[j] ][2]][PaletteRGB[ image[index] ][2]] ];
                
			j++; index++;
		}
	}
	vidptr+=SurfaceWidth;
	}
}


void cltGrp::PutSprLight18CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index = 0;
	int up_skip = 0, down_skip = 0;
	int left_skip = 0, right_skip = 0;
	UI08 *vidptr = Screen8 + y * SurfaceWidth + x;

	if(x + xsize - 1 < ClipX1) return;
	if(y + ysize - 1 < ClipY1) return;

	if(y         < ClipY1) up_skip    = ClipY1 - y;
	if(y + ysize > ClipY2) down_skip  = y + ysize - ClipY2 - 1;
	if(x         < ClipX1) left_skip  = ClipX1 - x;
	if(x + xsize > ClipX2) right_skip = x + xsize - ClipX2 - 1;

	down_skip = ysize - down_skip;
	for(i = 0; i < up_skip; i++)
	{
		j = 0;
		while(j < xsize)
		{
			if(image[index] == COLOR_TRANS)
			{
				index++;
				j += image[index] - 1;
			}

			index++;
			j++;
		}

		vidptr += SurfaceWidth;
	}
	right_skip = xsize - right_skip;
	for(i = up_skip; i < down_skip; i++)
	{
		j = 0;
		while(j < xsize)
		{
			if(image[index] == COLOR_TRANS)
			{
				index++;
				j += image[index];
				index++;
			}
			else
			{
				if(j >= left_skip && j < right_skip)
				{
					vidptr[j] = Palette16[LightEffect1[PaletteRGB[vidptr[j]][0]][PaletteRGB[image[index]][0]]]
								  	     [LightEffect1[PaletteRGB[vidptr[j]][1]][PaletteRGB[image[index]][1]]]
									     [LightEffect1[PaletteRGB[vidptr[j]][2]][PaletteRGB[image[index]][2]]];
				}
                
				j++; index++;
			}
		}
		vidptr += SurfaceWidth;
	}
}


void cltGrp::PutSprLight18RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;

	for(i=0;i<ysize;i++)
	{
		j=0;
	while(j<xsize)
	{
		if(image[index]==COLOR_TRANS)
		{
			index++;
			j+=image[index];
			index++;
		}
		else
		{
			vary=xsize-j-1;

			vidptr[vary]=Palette16[ LightEffect1[PaletteRGB[ vidptr[vary] ][0]][PaletteRGB[ image[index] ][0]] ]
				                  [ LightEffect1[PaletteRGB[ vidptr[vary] ][1]][PaletteRGB[ image[index] ][1]] ]
							      [ LightEffect1[PaletteRGB[ vidptr[vary] ][2]][PaletteRGB[ image[index] ][2]] ];
                
			j++; index++;
		}
	}
	vidptr+=SurfaceWidth;
	}
}

void cltGrp::PutSprLight18CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)

{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;

	 if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)right_skip=ClipX1-x;
	if(x+xsize>ClipX2)left_skip=x+xsize-ClipX2-1;

	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
	while(j<xsize)
	{
		if(image[index]==COLOR_TRANS)
		{
			index++;
			j+=image[index];
			index++;
		}
		else
		{
			vary=xsize-j-1;

			if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
			{
				vidptr[vary]=Palette16[LightEffect1[PaletteRGB[ vidptr[vary] ][0]][PaletteRGB[ image[index] ][0]]]
					                  [LightEffect1[PaletteRGB[ vidptr[vary] ][1]][PaletteRGB[ image[index] ][1]]]
								      [LightEffect1[PaletteRGB[ vidptr[vary] ][2]][PaletteRGB[ image[index] ][2]]];
			}
                
			j++; index++;
		}
	}
	vidptr+=SurfaceWidth;
	}
}

//=============================================================================
// SPRITE ADD
//=============================================================================
void cltGrp::PutSprAdd8T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	
	for(i=0;i<ysize;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vidptr[j]=AddTable[vidptr[j]][image[index]];
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

void cltGrp::PutSprAdd8CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	
	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	
	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;
	
	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				{
					vidptr[j]=AddTable[vidptr[j]][image[index]];
				}
                
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

void cltGrp::PutSprAdd8RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;
	
	for(i=0;i<ysize;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vary=xsize-j-1;
				vidptr[vary]=AddTable[vidptr[vary]][image[index]];
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}


void cltGrp::PutSprAdd8CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)

{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;
	
	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	
	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)right_skip=ClipX1-x;
	if(x+xsize>ClipX2)left_skip=x+xsize-ClipX2-1;
	
	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vary=xsize-j-1;
				
				if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				{
					vidptr[vary]=AddTable[vidptr[vary]][image[index]];
				}
                
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

//=============================================================================
// SPRITE SCREEN
//=============================================================================
void cltGrp::PutSprScreen8T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	
	for(i=0;i<ysize;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vidptr[j]=ScreenTable[vidptr[j]][image[index]];
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

void cltGrp::PutSprScreen8CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	
	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	
	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;
	
	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				{
					vidptr[j]=ScreenTable[vidptr[j]][image[index]];
				}
                
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

void cltGrp::PutSprScreen8RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;
	
	for(i=0;i<ysize;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vary=xsize-j-1;
				vidptr[vary]=ScreenTable[vidptr[vary]][image[index]];
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}


void cltGrp::PutSprScreen8CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)

{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;
	
	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	
	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)right_skip=ClipX1-x;
	if(x+xsize>ClipX2)left_skip=x+xsize-ClipX2-1;
	
	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vary=xsize-j-1;
				
				if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				{
					vidptr[vary]=ScreenTable[vidptr[vary]][image[index]];
				}
                
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

//=============================================================================
// SPRITE MULTIPLY
//=============================================================================
void cltGrp::PutSprMultiply8T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	
	for(i=0;i<ysize;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vidptr[j]=MultiplyTable[vidptr[j]][image[index]];
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

void cltGrp::PutSprMultiply8CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	
	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	
	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;
	
	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				{
					vidptr[j]=MultiplyTable[vidptr[j]][image[index]];
				}
                
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

void cltGrp::PutSprMultiply8RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;
	
	for(i=0;i<ysize;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vary=xsize-j-1;
				vidptr[vary]=MultiplyTable[vidptr[vary]][image[index]];
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}


void cltGrp::PutSprMultiply8CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)

{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;
	SHORT vary;
	
	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	
	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)right_skip=ClipX1-x;
	if(x+xsize>ClipX2)left_skip=x+xsize-ClipX2-1;
	
	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				vary=xsize-j-1;
				
				if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				{
					vidptr[vary]=MultiplyTable[vidptr[vary]][image[index]];
				}
                
				j++; index++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}


//=============================================================================
// MPR 
//=============================================================================
void cltGrp::PutSpr8M(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j,index=0;
	UI08 *vidptr	=	Screen8+y*SurfaceWidth + x;

	 for(i = 0;i < ysize;i++)
	 {
		j = 0;

		while(j < xsize)
		{
			// 투명색이면, 
			if(image[index] == COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			// 유효색의 카운터이면, 
			else
			{
				memcpy(&vidptr[j], &image[index+1], image[index]);
				
				j		+= image[index];
				index	+= image[index];
				index++;
			}
		}

		vidptr += SurfaceWidth;
	 }
}


void cltGrp::PutSpr8CM(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{

	int i, j, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;
	SI32 xindent, yindent;
	SI32 xend;
	SI32 xcpysize;
	SI32 abandomsize;

	SI32 startx = 0 , starty = 0 ;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	// xindent를 확인한다. (왼쪽으로부터 몇 칸을 건너뛰는가.) 
	if(x < ClipX1)	xindent = ClipX1 - x;
	else			xindent	= 0;

	// yindent를 확인한다. (위로부터 몇 줄을 건너뛰는가.) 
	if(y < ClipY1)	yindent = ClipY1 - y;
	else			yindent	= 0;

	// xend를 조정한다.
	if(x + xsize - 1 > ClipX2)
	{
		xend = ClipX2 - x + 1;
	}
	else
	{
		xend = xsize;
	}

	// ysize를 재조정한다.
	if(y + ysize - 1 > ClipY2)
	{
		ysize = ClipY2 - y + 1;
	}


	 for(i = 0;i < ysize;i++)
	 {
		j = 0;

		while(j < xsize)
		{
			// 투명색이면, 
			if(image[index] == COLOR_TRANS)
			{
			   index++;
			   j	=	(j + image[index]);
			   index++;
			}
			// 유효색의 카운터이면, 
			else
			{
				if(i >= yindent)
				{
					if(j < xend)
					{
						if(j + image[index] - 1 >= xend)
						{
							xcpysize = xend - j;
						}
						else
						{
							xcpysize = image[index];
						}
					
						// ClipX1 이전에서부터 출력한다면, 
						if(j < xindent)
						{
							// 잘려진 사이즈. 
							abandomsize = xindent-j;

							if(xcpysize - abandomsize > 0)
							{
								memcpy(&vidptr[xindent], &image[index+1 + abandomsize], xcpysize - abandomsize);
							}
						}
						// ClipX1이후로 출력한다면, 
						else
						{
							memcpy(&vidptr[j], &image[index+1], xcpysize);
						}
					}
				}
				
				j		+= image[index];
				index	+= image[index];
				index++;
			}
		}

		vidptr += SurfaceWidth;
	 }

}

//=============================================================================
// MPR SCREEN
//=============================================================================

void cltGrp::PutSprScreen8M(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, k, index=0;
	UI08 *vidptr	=	Screen8+y*SurfaceWidth + x;

	 for(i = 0;i < ysize;i++)
	 {
		j = 0;

		while(j < xsize)
		{
			// 투명색이면, 
			if(image[index] == COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			// 유효색의 카운터이면, 
			else
			{
				int Count    = image[index];
				int vidIndex = j;
				int ImgIndex = index+1;
				for(k = 0; k <Count; ++k)
				{	
					vidptr[vidIndex] =	ScreenTable[vidptr[vidIndex]][image[ImgIndex]];
					++vidIndex; ++ImgIndex;
				}

				//memcpy(&vidptr[j], &image[index+1], image[index]);
				
				j		+= image[index];
				index	+= image[index];
				index++;
			}
		}

		vidptr += SurfaceWidth;
	 }
}

void cltGrp::PutSprScreen8CM(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{

	int i, j, k, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;
	SI32 xindent, yindent;
	SI32 xend;
	SI32 xcpysize;
	SI32 abandomsize;

	SI32 startx = 0 , starty = 0 ;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	// xindent를 확인한다. (왼쪽으로부터 몇 칸을 건너뛰는가.) 
	if(x < ClipX1)	xindent = ClipX1 - x;
	else			xindent	= 0;

	// yindent를 확인한다. (위로부터 몇 줄을 건너뛰는가.) 
	if(y < ClipY1)	yindent = ClipY1 - y;
	else			yindent	= 0;

	// xend를 조정한다.
	if(x + xsize - 1 > ClipX2)
	{
		xend = ClipX2 - x + 1;
	}
	else
	{
		xend = xsize;
	}

	// ysize를 재조정한다.
	if(y + ysize - 1 > ClipY2)
	{
		ysize = ClipY2 - y + 1;
	}


	 for(i = 0;i < ysize;i++)
	 {
		j = 0;

		while(j < xsize)
		{
			// 투명색이면, 
			if(image[index] == COLOR_TRANS)
			{
			   index++;
			   j	=	(j + image[index]);
			   index++;
			}
			// 유효색의 카운터이면, 
			else
			{
				if(i >= yindent)
				{
					if(j < xend)
					{
						if(j + image[index] - 1 >= xend)
						{
							xcpysize = xend - j;
						}
						else
						{
							xcpysize = image[index];
						}
					
						// ClipX1 이전에서부터 출력한다면, 
						if(j < xindent)
						{
							// 잘려진 사이즈. 
							abandomsize = xindent-j;
							if(xcpysize - abandomsize > 0)
							{
								int Count = xcpysize - abandomsize;
								int vidIndex = xindent;
							int ImgIndex = index+1 + abandomsize;
								for(k = 0; k <Count; ++k)
								{	
									vidptr[vidIndex] =	ScreenTable[vidptr[vidIndex]][image[ImgIndex]];
									++vidIndex; ++ImgIndex;
								}
								//memcpy(&vidptr[xindent], &image[index+1 + abandomsize], xcpysize - abandomsize);
							}
						}
						// ClipX1이후로 출력한다면, 
						else
						{
							int Count = xcpysize;
							int vidIndex = j;
							int ImgIndex = index+1;
							for(k = 0; k <Count; ++k)
							{	
								vidptr[vidIndex] =	ScreenTable[vidptr[vidIndex]][image[ImgIndex]];
								++vidIndex; ++ImgIndex;
							}
							//memcpy(&vidptr[j], &image[index+1], xcpysize);
						}
					}
				}
				
				j		+= image[index];
				index	+= image[index];
				index++;
			}
		}

		vidptr += SurfaceWidth;
	 }

}

void cltGrp::PutSprMultiply8M(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, k, index=0;
	UI08 *vidptr	=	Screen8+y*SurfaceWidth + x;

	 for(i = 0;i < ysize;i++)
	 {
		j = 0;

		while(j < xsize)
		{
			// 투명색이면, 
			if(image[index] == COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			// 유효색의 카운터이면, 
			else
			{
				int Count    = image[index];
				int vidIndex = j;
				int ImgIndex = index+1;
				for(k = 0; k <Count; ++k)
				{	
					vidptr[vidIndex] =	MultiplyTable[vidptr[vidIndex]][image[ImgIndex]];
					++vidIndex; ++ImgIndex;
				}

				//memcpy(&vidptr[j], &image[index+1], image[index]);
				
				j		+= image[index];
				index	+= image[index];
				index++;
			}
		}

		vidptr += SurfaceWidth;
	 }

	return;
}

void cltGrp::PutSprMultiply8CM(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, k, index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;
	SI32 xindent, yindent;
	SI32 xend;
	SI32 xcpysize;
	SI32 abandomsize;

	SI32 startx = 0 , starty = 0 ;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;


	// xindent를 확인한다. (왼쪽으로부터 몇 칸을 건너뛰는가.) 
	if(x < ClipX1)	xindent = ClipX1 - x;
	else			xindent	= 0;

	// yindent를 확인한다. (위로부터 몇 줄을 건너뛰는가.) 
	if(y < ClipY1)	yindent = ClipY1 - y;
	else			yindent	= 0;

	// xend를 조정한다.
	if(x + xsize - 1 > ClipX2)
	{
		xend = ClipX2 - x + 1;
	}
	else
	{
		xend = xsize;
	}

	// ysize를 재조정한다.
	if(y + ysize - 1 > ClipY2)
	{
		ysize = ClipY2 - y + 1;
	}


	 for(i = 0;i < ysize;i++)
	 {
		j = 0;

		while(j < xsize)
		{
			// 투명색이면, 
			if(image[index] == COLOR_TRANS)
			{
			   index++;
			   j	=	(j + image[index]);
			   index++;
			}
			// 유효색의 카운터이면, 
			else
			{
				if(i >= yindent)
				{
					if(j < xend)
					{
						if(j + image[index] - 1 >= xend)
						{
							xcpysize = xend - j;
						}
						else
						{
							xcpysize = image[index];
						}
					
						// ClipX1 이전에서부터 출력한다면, 
						if(j < xindent)
						{
							// 잘려진 사이즈. 
							abandomsize = xindent-j;
							if(xcpysize - abandomsize > 0)
							{
								int Count = xcpysize - abandomsize;
								int vidIndex = xindent;
							int ImgIndex = index+1 + abandomsize;
								for(k = 0; k <Count; ++k)
								{	
									vidptr[vidIndex] =	MultiplyTable[vidptr[vidIndex]][image[ImgIndex]];
									++vidIndex; ++ImgIndex;
								}
								//memcpy(&vidptr[xindent], &image[index+1 + abandomsize], xcpysize - abandomsize);
							}
						}
						// ClipX1이후로 출력한다면, 
						else
						{
							int Count = xcpysize;
							int vidIndex = j;
							int ImgIndex = index+1;
							for(k = 0; k <Count; ++k)
							{	
								vidptr[vidIndex] =	MultiplyTable[vidptr[vidIndex]][image[ImgIndex]];
								++vidIndex; ++ImgIndex;
							}
							//memcpy(&vidptr[j], &image[index+1], xcpysize);
						}
					}
				}
				
				j		+= image[index];
				index	+= image[index];
				index++;
			}
		}

		vidptr += SurfaceWidth;
	 }

	return;
}