#include "RememberwindowPosObj.h"

CRememberWindowPosObj::CRememberWindowPosObj()
{	
	Initialize();
}

CRememberWindowPosObj::~CRememberWindowPosObj()
{
	Destroy();
}

void CRememberWindowPosObj::Initialize()
{	

	m_uiWindowKind = 0;

	m_siLeftPos = 0;
	m_siTopPos = 0;

	m_siRightPos = 0;
	m_siBottomPos = 0;

	return;
}

void CRememberWindowPosObj::Destroy()
{
	return;
}

bool CRememberWindowPosObj::IsAvailablePos( SI32 siLeft, SI32 siTop, SI32 siWidth, SI32 siHeight )
{
	/*
	HWND hDesktopWnd = GetDesktopWindow();

	RECT DesktopRect;
	GetWindowRect( hDesktopWnd, &DesktopRect );

	if ( DesktopRect.left + 10 > siLeft + siWidth )
	{
		siLeft = 10 - siWidth;
	}	

	if ( DesktopRect.top + 10 > siTop )
	{
		siTop = 10;
	}

	if ( DesktopRect.right - 10 < siLeft )
	{
		siLeft = DesktopRect.right - 10;
	}

	if ( DesktopRect.bottom - 10 < siTop )
	{
		siTop = DesktopRect.bottom - 10;
	}
	*/

	return false;
}

void CRememberWindowPosObj::ChangePos( UI32 uiWindowKind, SI32 siLeft, SI32 siTop, SI32 siRight, SI32 siBottom )
{
	m_uiWindowKind = uiWindowKind;

	m_siLeftPos = siLeft;
	m_siTopPos = siTop;

	m_siRightPos = siRight;
	m_siBottomPos = siBottom;

	HWND hDesktopWnd = GetDesktopWindow();

	RECT DesktopRect;
	GetWindowRect( hDesktopWnd, &DesktopRect );

	if ( DesktopRect.left + 10 > m_siRightPos )
	{
		m_siLeftPos += DesktopRect.left + 10 - m_siRightPos;
		m_siRightPos = DesktopRect.left + 10;
	}	

	if ( DesktopRect.top + 10 > m_siTopPos )
	{
		m_siBottomPos = 10 + m_siBottomPos - m_siTopPos;
		m_siTopPos = 10;		
	}

	if ( DesktopRect.right - 10 < m_siLeftPos )
	{
		m_siRightPos += m_siLeftPos - ( DesktopRect.right - 10 );
		m_siLeftPos = DesktopRect.right - 10;
	}

	if ( DesktopRect.bottom - 10 < m_siTopPos )
	{
		m_siBottomPos = DesktopRect.bottom - 10 + m_siBottomPos - m_siTopPos;
		m_siTopPos = DesktopRect.bottom - 10;		
	}

	return;
}

UI32 CRememberWindowPosObj::GetRememberWindowKind()
{
	return m_uiWindowKind;
}

void CRememberWindowPosObj::GetRememberWindowPosData( UI32 *pWindowKind, SI32 *pLeftPos, SI32 *pTopPos, SI32 *pRightPos, SI32 *pBottomPos )
{
	*pWindowKind = m_uiWindowKind;

	*pLeftPos = m_siLeftPos;
	*pTopPos = m_siTopPos;

	*pRightPos = m_siRightPos;
	*pBottomPos = m_siBottomPos;

	return;
}
