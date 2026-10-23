#ifndef _IMAGESTATIC_H_
#define _IMAGESTATIC_H_

#include "../ControllerObj.h"

class CControllerMgr;

class CImageStatic : public CControllerObj  
{
public:
	CImageStatic( CControllerMgr *pParent );
	~CImageStatic();
	
	void Create( SI16 ControllerID, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight );

	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );

	void ClearImageStaticPos();
	void SetImageStaticPos( int XPos, int YPos );
	void GetImageStaticPos( POINT &Point );

	SI32 GetZDelta()
	{
		return m_siZDelta;
	}

	// [광명]필요해서
	void SetUseToolTip( TCHAR *ToolTipMsg, bool bColor = true );
	void SetUseToolTip( bool bSet )		
	{
		m_bUseToolTip = bSet;		
	}
	
	// [진성] 툴팁 정보필요. 2008-6-9
	const TCHAR* GetToolTipMsg(void)
	{
		return m_strToolTipMsg;
	}

private:
	bool	m_bLMPressed;
	bool	m_bRMPressed;
	bool	m_bUseToolTip;
	bool	m_bUseToolTipColor;

	POINT	m_ptImageStaticPos;

	SI32	m_siZDelta;
	
	TCHAR *m_strToolTipMsg;

};

#endif