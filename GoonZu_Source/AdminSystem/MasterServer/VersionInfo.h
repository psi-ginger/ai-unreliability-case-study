#ifndef _VERSIONINFO_H
#define _VERSIONINFO_H

#include "itzbase.h"

class VersionInfo
{
public:
	VersionInfo();
	~VersionInfo();

public:
	void				ReadFromFile();
	void				printInfo();


private:
	void				_setPatch( char *buf1, char *buf2 );

public:
	UI32				m_uiVersionCount;
	UI32				m_uiVersionList[ 2048 ];

};


#endif