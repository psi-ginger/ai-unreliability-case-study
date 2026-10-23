#ifndef _CRUSESFILTERMGR_H_
#define _CRUSESFILTERMGR_H_

#include "../Common/SMLib/LibList/LibListMgr.h"

class CCrusesFilterMgr : public CLibListMgr
{
public:
	CCrusesFilterMgr();
	~CCrusesFilterMgr();

	void Init();

	void ReadTxt( TCHAR *FileName );

	bool FindCruses( TCHAR *TextData );

	void TrimChar( TCHAR *InTextData, TCHAR *OutTextData, TCHAR ch );

private:


	
};

#endif