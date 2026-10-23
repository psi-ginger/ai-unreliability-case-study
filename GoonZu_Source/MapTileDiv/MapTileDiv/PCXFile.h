#ifndef _PCXFILE_H
#define _PCXFILE_H

// Standard PCX header
struct PCXHeader {
	char	ID;
	char	Version;
	char	Encoding;
	char	BitPerPixel;
	short	X1;
	short	Y1;
	short	X2;
	short	Y2;
	short	HRes;
	short	VRes;
	char	ClrMap[16*3];
	char	Reserved1;
	char	NumPlanes;
	short	BPL;
	short	Pal_t;
	char	Filler[58];
};


void LoadPcx( char *filename );



#endif
