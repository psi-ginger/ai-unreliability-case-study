#ifndef _REMEMBERWINDOWPOSMGR_H_
#define _REMEMBERWINDOWPOSMGR_H_

#include "RememberWindowPosBaseInfo.h"

#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"

class CRememberWindowPosMgr : CLibListMgr
{
public:
	CRememberWindowPosMgr();
	~CRememberWindowPosMgr();

	void	CreateMaxEmptyObj( int si32MaxObjNum );

	void	ReadWindowPosData();
	void	WriteWindowPosData();

	bool	IsExist( UI32 uiWindowKind );
	bool	GetWindowPosData( UI32 uiWindowKind, SI32 *psiLeft, SI32 *psiTop, SI32 *psiRight, SI32 *psiBottom );



	void	AddWindowPosData( UI32 uiWindowKind, SI32 siLeft, SI32 siTop, SI32 siRight, SI32 siBottom );
	void	WindowPosChange( UI32 uiWindowKind, SI32 siLeft, SI32 siTop, SI32 siRight, SI32 siBottom );
	
private:

	SI32	m_siDesktopRight;
	SI32	m_siDesktopBottom;
	SI32	m_siScriptVersion;

	bool	m_bSupportMode;
	

};

#endif