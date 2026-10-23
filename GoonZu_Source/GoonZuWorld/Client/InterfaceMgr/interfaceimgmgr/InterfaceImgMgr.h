#ifndef _INTERFACEIMGMGR_H_
#define _INTERFACEIMGMGR_H_

#include "../ControllerBaseDefinition.h"
#include "../../../Common/SMLib/LibList/LibListMgr.h"

class CInterfaceImgMgr : public CLibListMgr
{
public:
	CInterfaceImgMgr();
	~CInterfaceImgMgr();

	void CreateMaxEmptyObj();

	TSpr* LoadInterfaceImg( TCHAR *FileName );

	void Action();

private:


};

#endif