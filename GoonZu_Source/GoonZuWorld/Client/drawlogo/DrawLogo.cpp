#include "../Lib/NGraphics/Graphics.h"
#include "../Client/DrawLogo/DrawLogo.h"
#include "../common/mouse.h"

#include "../Client/Client.h"
#include "../CommonLogic/CommonLogic.h"
#include "../Client/StatusBar/ChatStatusBar/ChatStatusBar.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

extern NGraphics	g_graphics;
extern SI32		gsiResolution;
extern _InputDevice					IpD;

#define KeyState(KEY) (GetAsyncKeyState(KEY) & 0x8000)

CDrawLogo::CDrawLogo() 
{ 
//	m_DD = NULL;
	CoInitialize(NULL);     
} 

CDrawLogo::~CDrawLogo() 
{ 
	Release(); 
} 

void CDrawLogo::PlayMovie(TCHAR* Name)
{ 
	CoInitialize(NULL); 
	TCHAR hr; 
//	m_DD = pDD; 
	IMultiMediaStream *pMMStream; 
	hr = OpenMMStream(Name, &pMMStream);//, m_DD); 
	if (hr==TRUE)
	{ 
		RenderStreamToSurface(pMMStream);
		pMMStream->Release(); 
	} 
	CoUninitialize(); 
} 

BOOL CDrawLogo::OpenMMStream(TCHAR * pszFileName, IMultiMediaStream **ppMMStream)//, NGraphics DDraw)
{ 
	*ppMMStream = NULL; 
	IAMMultiMediaStream *pAMStream; 
	if(FAILED(CoCreateInstance(CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER, 
		IID_IAMMultiMediaStream, (void **)&pAMStream))) 
	{
		pAMStream->Release();
		MessageBox(NULL, TEXT("초기화 실패1"), TEXT("로고실패"), MB_OK|MB_TOPMOST );
		return FALSE;
	} 
	if(FAILED(pAMStream->Initialize(STREAMTYPE_READ, 0, NULL))) 
	{
		pAMStream->Release();
		MessageBox(NULL, TEXT("초기화 실패2"), TEXT("로고실패"), MB_OK|MB_TOPMOST );
		return FALSE;
	} 
	if(FAILED(pAMStream->AddMediaStream(g_graphics.GetDirectDraw(), &MSPID_PrimaryVideo, 0, NULL))) 
	{
		pAMStream->Release();
		MessageBox(NULL, TEXT("미디어 스트림 연결실패"), TEXT("로고실패"), MB_OK|MB_TOPMOST );
		return FALSE;
	} 
	if(FAILED(pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, 
		AMMSF_ADDDEFAULTRENDERER, NULL))) 
	{
		pAMStream->Release();
		MessageBox(NULL, TEXT("오디오 연결실패"), TEXT("로고실패"), MB_OK|MB_TOPMOST );
		return FALSE;
	} 
	WCHAR       wPath[MAX_PATH]; 

#ifndef _UNICODE
	MultiByteToWideChar(CP_ACP, 0, pszFileName, -1, wPath, sizeof(wPath)/sizeof(wPath[0])); 
#else
	StringCchCopy(wPath, sizeof(wPath)/sizeof(wPath[0]), pszFileName);
#endif

	if(pAMStream->OpenFile(wPath, 0)!=S_OK) 
	{ 
		pAMStream->Release(); 
		MessageBox(NULL, TEXT("파일열기 실패"), TEXT("로고 실패"), MB_OK|MB_TOPMOST );
		return FALSE;
	} 
	*ppMMStream = pAMStream; 
	pAMStream->AddRef(); 
	return TRUE; 
} 

BOOL CDrawLogo::RenderStreamToSurface(IMultiMediaStream *pMMStream) 
{ 
	cltClient* pclclient = (cltClient*)pclClient ;

	IMediaStream *pPrimaryVidStream = NULL; 
	IDirectDrawMediaStream *pDDStream = NULL; 
	IDirectDrawSurface *pSurface = NULL; 
	IDirectDrawStreamSample *pSample = NULL; 
	RECT rect; 

	if(FAILED(pMMStream->GetMediaStream(MSPID_PrimaryVideo, &pPrimaryVidStream))) 
	{
		MessageBox(NULL, TEXT("GetMediaStream() 실패"), TEXT("로고실패"), MB_OK|MB_TOPMOST );
		return FALSE; 
	}
	if(FAILED(pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, (void**)&pDDStream))) 
	{
		MessageBox(NULL, TEXT("QueryInterface() 실패"), TEXT("로고실패"), MB_OK|MB_TOPMOST );
		return FALSE; 
	}
	if(FAILED(pDDStream->CreateSample(NULL, NULL, DDSFF_PROGRESSIVERENDER, &pSample))) 
	{
		MessageBox(NULL, TEXT("CreateSample() 실패"), TEXT("로고실패"), MB_OK|MB_TOPMOST );
		return FALSE; 
	}
	if(FAILED(pSample->GetSurface(&pSurface, &rect))) 
	{
		MessageBox(NULL, TEXT("GetSurface() 실패"), TEXT("로고실패"), MB_OK|MB_TOPMOST );
		return FALSE; 
	}
	if(FAILED(pMMStream->SetState(STREAMSTATE_RUN))) 
	{
		MessageBox(NULL, TEXT("SetState() 실패"), TEXT("로고실패"), MB_OK|MB_TOPMOST );
		return FALSE; 
	}

	SI32 x=0, y=0;
	RECT rect2;
	RECT rect3;
	if(gsiResolution == 1) // 800 X 600
	{
		if(g_SoundOnOffDlg.m_bFullScreenMode == FALSE)	// 원도우
		{
			x = 244;
			y = 230;
			//SetRect(&rect3, 0, 0, g_graphics.m_width, g_graphics.m_height); 
			GetWindowRect(pclClient->GetHwnd(),&rect3);
			RECT rt = {0, 0, 0, 0} ;
//			GetClientRect(pclclient->g_ChatStatusBar->GetDlgHwnd(),&rt);
			rect3.bottom -= rt.bottom ;
		}
		else
		{
			x = 244;
			y = 215;
			SetRect(&rect3, 0, 0, g_graphics.m_width, g_graphics.m_height); 
		}
	}
	else if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
	{
		if(g_SoundOnOffDlg.m_bFullScreenMode == FALSE)	// 원도우
		{
			x = 326;
			y = 278;
			//SetRect(&rect3, 0, 0, g_graphics.m_width, g_graphics.m_height); 
			GetWindowRect(pclClient->GetHwnd(),&rect3);
			RECT rt = {0, 0, 0, 0};
//			GetClientRect(pclclient->g_ChatStatusBar->GetDlgHwnd(),&rt);
			rect3.bottom -= rt.bottom ;
		}
		else
		{
			x = 326;
			y = 260;
			//SetRect(&rect3, 0, 0, g_graphics.m_width, g_graphics.m_height); 
			SetRect(&rect3, 0, 0, g_graphics.m_width, g_graphics.m_height); 
		}
	}

	SetRect(&rect2, x  ,  y , 
					rect.right + x, rect.bottom + y); 

	DDBLTFX ddbltfx;

	ZeroMemory( &ddbltfx, sizeof(ddbltfx) );

	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0;
	
	g_graphics.GetBackBuffer()->Blt( &rect3, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
	g_graphics.GetConvertBuffer()->Blt( &rect3, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
	g_graphics.GetFrontBuffer()->Blt( &rect3, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

	bool loop = true;
	while (loop)
	{ 
		if(KeyState(VK_LBUTTON))
		{
			break;
		}

		if (pSample->Update(0, NULL, NULL, 0) != S_OK)
		{ 
			break; 
		} 

		g_graphics.GetFrontBuffer()->Blt(&rect2, (LPDIRECTDRAWSURFACE7)pSurface, &rect, DDBLT_WAIT, NULL); 
	} 
	pMMStream->SetState(STREAMSTATE_STOP);

	pSurface->Release(); 
	pSample->Release(); 
	pDDStream->Release(); 
	pPrimaryVidStream->Release(); 

	return TRUE; 
}

void CDrawLogo::Release() 
{ 
///	m_DD = NULL;
	CoUninitialize();   
} 




