#ifndef _CHARCUT_H_
#define _CHARCUT_H_

#include <Directives.h>
#include "TSpr.h"

enum { CUT_HORIZONTAL, CUT_VERTICAL, CUT_DIAGONAL };
enum { CUT_STATUS, NOT_CUT_STATUS };
#define MAX_HITTYPE_NUM 6

class CCharCut
{
public:
	CCharCut();
	~CCharCut();

	void	SetAction(SI32 mode);
	void	Action();
	void	Draw( SI32 siCenterXPos, SI32 siCenterYPos );

private:	
	TSpr	*m_pCutXSPR;
	char	m_cCurrentFrame;
	
	char	m_cCutStatus;
	char	m_cCutKind;

	SI32	m_siMode ;
};

#endif