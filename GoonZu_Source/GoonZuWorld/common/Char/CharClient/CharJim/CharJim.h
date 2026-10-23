#ifndef _CHARJIM_H_
#define _CHARJIM_H_

#include <Directives.h>
#include "TSpr.h"


#define DELAYFRAME_NUM		2		// 2 Frame 마다 바뀐 이미지 출력

enum { JIM_STATUS, NOT_JIM_STATUS };

class CCharJim
{
public:
	CCharJim( TSpr * );
	~CCharJim();

	void	Set( bool bSet, bool bMyJim );
	void	Action();
	void	Draw( SI32 siCenterXPos, SI32 siCenterYPos );
	BOOL	IsJimStatus();	

private:	
	TSpr	*m_pJimXSPR;
	bool	m_bMyJim;
	char	m_cCurrentFrame;
	
	char	m_cStatus;	
	SI16	m_siDelayFrame;
};

#endif