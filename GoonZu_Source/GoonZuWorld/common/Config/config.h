//---------------------------------
// 2003/5/17 ±èÅÂ°ï
//---------------------------------

#ifndef _CONFIG_H
#define _CONFIG_H

#include <Directives.h>

class cltConfig{
public:
	TCHAR FileName[FILE_NAME_SIZE];

	 SI32 siGamma;
	 SI32 siGameSpeed;
	 SI32 siSoundVolume;
	 DWORD Reserved[43];

	 cltConfig(TCHAR* filename);
	 ~cltConfig();
	 
	 void SetGameSpeed(SI32 speed);
	 
	 BOOL GammaFunction(SI32 mount);
	 
	 void SetSoundVolume(SI32 volume);
	 void SetSoundVolume();
	 
	 BOOL Save();
	 BOOL Load();
	 
};

#endif