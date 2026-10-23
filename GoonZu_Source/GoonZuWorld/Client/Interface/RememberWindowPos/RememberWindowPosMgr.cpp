#include "RememberWindowPosMgr.h"
#include "RememberWindowPosObj.h"
#include "..\..\..\Common\SMLib\LibList\LibList.h"

CRememberWindowPosMgr::CRememberWindowPosMgr() : CLibListMgr()
{	
	m_bSupportMode = false;

	CreateMaxEmptyObj(MAX_REMEMBERWINDOWPOS_OBJ_NUM);
	ReadWindowPosData();
}

CRememberWindowPosMgr::~CRememberWindowPosMgr()
{
	WriteWindowPosData();
}

void CRememberWindowPosMgr::CreateMaxEmptyObj( int si32MaxObjNum )
{
	CRememberWindowPosObj *pTemp = NULL;

	for ( SI32 i = 0; i < si32MaxObjNum; ++i )
	{
		pTemp = new CRememberWindowPosObj();
		PushBackToEmptyList( pTemp );
	}

	return;
}

void CRememberWindowPosMgr::ReadWindowPosData()
{
	HWND hDesktopWnd = GetDesktopWindow();

	RECT DesktopRect;
	GetWindowRect( hDesktopWnd, &DesktopRect );

	m_siDesktopRight = DesktopRect.right;
	m_siDesktopBottom = DesktopRect.bottom;

	FILE *fp = _tfopen( TEXT("Config/WindowPos.dat"), TEXT("rt") );

	UI32 uiWindowKind;
	SI32 siLeftPos;
	SI32 siTopPos;

	SI32 siRightPos;
	SI32 siBottomPos;


	if ( fp == NULL )
	{
		return;
	}

	SI32 siDesktopRight;
	SI32 siDesktopBottom;

	_ftscanf( fp, TEXT("%d %d %d"), &m_siScriptVersion, &siDesktopRight, &siDesktopBottom );

	if ( m_siScriptVersion != SCRIPT_VERSION || m_siDesktopRight != siDesktopRight || m_siDesktopBottom != siDesktopBottom )
	{
		m_bSupportMode = false;
		fclose( fp );

		return;
	}
	
	CRememberWindowPosObj *pTemp = NULL;
	while( ! feof( fp ) )
	{
		pTemp = (CRememberWindowPosObj *)PopFrontToEmptyListAndPushBackToExistList();
		
		_ftscanf( fp, TEXT("%u %d %d %d %d"), &uiWindowKind, &siLeftPos, &siTopPos, &siRightPos, &siBottomPos );
		pTemp->ChangePos( uiWindowKind, siLeftPos, siTopPos, siRightPos, siBottomPos );
	}

	fclose( fp );

	return;
}

void CRememberWindowPosMgr::WriteWindowPosData()
{
	/*
	HWND hDesktopWnd = GetDesktopWindow();

	RECT DesktopRect;
	GetWindowRect( hDesktopWnd, &DesktopRect );

	m_siDesktopRight = DesktopRect.right;
	m_siDesktopBottom = DesktopRect.bottom;

	FILE *fp = _tfopen( TEXT("Config/WindowPos.dat"), TEXT("wt") );

	if ( fp == NULL )
	{
		return;
	}

	UI32 uiWindowKind;
	SI32 siLeftPos;
	SI32 siTopPos;

	SI32 siRightPos;
	SI32 siBottomPos;

	_ftprintf( fp, TEXT("%d %d %d\n"), SCRIPT_VERSION, m_siDesktopRight, m_siDesktopBottom );

	CRememberWindowPosObj *pTemp = (CRememberWindowPosObj *)GetExistList()->GetFirst();

	while( pTemp )
	{
		pTemp->GetRememberWindowPosData( &uiWindowKind, &siLeftPos, &siTopPos, &siRightPos, &siBottomPos );

		_ftprintf( fp, TEXT("%u %d %d %d %d\n"), uiWindowKind, siLeftPos, siTopPos, siRightPos, siBottomPos );
		pTemp = (CRememberWindowPosObj *)pTemp->GetNext();
	}

	fclose( fp );

	return;
	*/
}

bool CRememberWindowPosMgr::IsExist( UI32 uiWindowKind )
{
	CRememberWindowPosObj *pTemp = (CRememberWindowPosObj *)GetExistList()->GetFirst();

	UI32 WindowKind = 0;

	while( pTemp )
	{
		WindowKind = pTemp->GetRememberWindowKind();
		
		if ( uiWindowKind == WindowKind )
		{			
			return true;
		}

		pTemp = (CRememberWindowPosObj *)pTemp->GetNext();
	}
	
	return false;
}

bool CRememberWindowPosMgr::GetWindowPosData( UI32 uiWindowKind, SI32 *psiLeft, SI32 *psiTop, SI32 *psiRight, SI32 *psiBottom )
{
	CRememberWindowPosObj *pTemp = (CRememberWindowPosObj *)GetExistList()->GetFirst();

	UI32 WindowKind = 0;

	while( pTemp )
	{
		WindowKind = pTemp->GetRememberWindowKind();
		
		if ( uiWindowKind == WindowKind )
		{	
			pTemp->GetRememberWindowPosData( &uiWindowKind, psiLeft, psiTop, psiRight, psiBottom );			
			return true;
		}

		pTemp = (CRememberWindowPosObj *)pTemp->GetNext();
	}
	
	return false;
}


void CRememberWindowPosMgr::AddWindowPosData( UI32 uiWindowKind, SI32 siLeft, SI32 siTop, SI32 siRight, SI32 siBottom )
{
	CRememberWindowPosObj *pTemp = (CRememberWindowPosObj *)PopFrontToEmptyListAndPushBackToExistList();	
	pTemp->ChangePos( uiWindowKind, siLeft, siTop, siRight, siBottom );

	return;
}

void CRememberWindowPosMgr::WindowPosChange( UI32 uiWindowKind, SI32 siLeft, SI32 siTop, SI32 siRight, SI32 siBottom )
{
	CRememberWindowPosObj *pTemp = (CRememberWindowPosObj *)GetExistList()->GetFirst();

	UI32 WindowKind = 0;

	while( pTemp )
	{
		WindowKind = pTemp->GetRememberWindowKind();
		
		if ( uiWindowKind == WindowKind )
		{	
			pTemp->ChangePos( uiWindowKind, siLeft, siTop, siRight, siBottom );
			return;
		}

		pTemp = (CRememberWindowPosObj *)pTemp->GetNext();
	}
	
	return;
}
