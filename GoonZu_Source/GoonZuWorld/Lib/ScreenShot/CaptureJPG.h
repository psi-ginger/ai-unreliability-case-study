
//=============================================================================
//
//	CaptureJPG.h
//
//	Copyright 2003 Goonzu
//
//=============================================================================

#ifndef __CAPTUREJPG_H__
#define __CAPTUREJPG_H__

#define MAX_CAPTUREJPG_WINDOW	100

class CaptureJPG
{
private:
	HDC		m_hMemDC,m_hScrDC;
	HBITMAP m_hOldBitmap;
	HBITMAP	m_hBitmap;   // 윈도우 크기의 비트맵
	HWND	m_hWnd;
	RECT	m_ScRT;

	int		m_ScreenWidth;		// Primary Screen 크기
	int		m_ScreenHeight;		// Primary Screen 크기

	int		m_WindowWidth;
	int		m_WindowHeight;


	void	*m_lpImageData;		// Image DATA

	TCHAR	m_WorkPath[MAX_PATH];	// 파일을 저장할 Working Folder
	TCHAR	m_FileName[MAX_PATH];	// 저장될 File 이름 
	TCHAR	m_FullPath[MAX_PATH];	// 저장될 File의 FullPath 이름 
public:
	//RECT	m_RT[MAX_CAPTUREJPG_WINDOW];
	//int		m_RTCount;
	RECT	m_WindowRect;
public:
	CaptureJPG(HWND hWnd = NULL);
	~CaptureJPG();

	void	CaptureToJPG(TCHAR *Path=NULL);	// PRIMARY SCREEEN만 해당 경로에 JPG 파일로 저장한다.
	TCHAR   *GetFileName();
	HWND	GetWindowHandle()	{return m_hWnd;	};

private:
	BOOL	InitCapture();					// Capture의 성능을 향상하기 위해서 미리 초기화 해둔다.
	BOOL	CloseCapture();					// 변수들을 해제한다.
	BOOL	SaveAsJPEG();					// 설정된값으로 JPG파일로 저장한다.
	void	CollectWindows();
	void	MakeFullPathName(TCHAR *Path);	// 저장될 FullPathName을 설정한다.
};


#endif