#ifndef _SERVERSTATISTICS_H
#define _SERVERSTATISTICS_H

class cltUserNum
{
public:
	cltUserNum();
	~cltUserNum();

public:
	BYTE					byteDay;
	BYTE					byteHour;
	BYTE					byteMin;

	FILETIME				filetime;
	SI32					siUserNum;
};

class cltUserNumList
{
public:
	cltUserNumList();
	~cltUserNumList();

public:
	cltUserNum				clUserNum[ 50 ];



};

#endif
