/* ==========================================================================
	클래스 :		CNBossMarketDetailDlg

	작성일 :		05/03/28
	작성자 :		정용래

	용  법 :		

	변경사항 :		없음
	
	차후작업 :		cpp 참고

   ========================================================================*/
//#if defined(_LEON_NBOSSMARKET_) && defined(_DEBUG)

#pragma once

#include <Directives.h>

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

class CEdit;
class CButton;
class CImageStatic;

#define MAX_FILE_FULLPATH	256

class CNBossMarketDetailDlg : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 80,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 80,
		PREV_INDEX = 0,
		NEXT_INDEX = 1,
		
		MAX_SCENE_NUM = 10,
	};

	CNBossMarketDetailDlg();
	~CNBossMarketDetailDlg();

	void Create();

	static	void CALLBACK StaticNBossMarketDetailDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NBossMarketDetailDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	bool IsShow();
	void Show();
	void Hide();

	// WM_TIMER 일 경우
	//void Action();
	bool CheckValidation();						// 현재 상태의 무결성 체크
	void UpdateControls( SI16 in_psiIndex );	// 각 컨트롤 업데이트

	void SetTextInfo( SI16 *in_pTextNum, SI16 in_siMaxIndex );							// 읽을 Text 번호 순서 배열 셋팅
	void SetImageInfo( TCHAR* in_pImageFullPath, SI16 in_siMaxIndex, SI16 *in_pImageFont = NULL);  // 읽을 Image 번호 순서 배열 셋팅
	void SetImageInfoWithFiles( TCHAR** in_pImageFullPath, SI16 in_siMaxIndex );  // 읽을 Image 번호 순서 배열 셋팅
	void SetMaxScene( SI16 in_siMaxScene );							// 최대 장면 수 셋팅
	void SetCurControl(  SI16 in_siCurIndex );						// 현재 장면 설정 ( Image + Edit )

	void SetPortrait( TCHAR* in_pImageFullPath, SI16 in_siIndex = 0);	// 초상화 설정

private:	
	bool					m_bMustDelete;		// m_psiImageIndex를 new로 할당 했는지 여부
	bool					m_bShow;
	bool					m_bFileNameMode;	// true : 그림 파일이 여러개 , false : 한 파일에 FontIndex만 달라짐
	SI16					m_siCurSceneIndex;	// 현재 장면 번호
	SI16					m_siMaxScene;		// 이미지와 Text의 개수가 같다, 한쌍을 Scene이라 하자!
	SI16*					m_psiTextIndex;		// Scene에 따른 텍스트 번호 인덱스
	SI16*					m_psiImageIndex;	// Scene에 따른 텍스트 번호 인덱스
	TCHAR**					m_ppImageFileName;	// 그림 파일이 여러장일때 

	CImageStatic			*m_pPortraitImage;
	CImageStatic			*m_pDetailImage;
	CEdit					*m_pExplainEdit;
	CButton					*m_pPrevButton;
	CButton					*m_pNextButton;
	CButton					*m_pCloseButton;
   
};

//#endif	//#if defined(_LEON_NBOSSMARKET_) && defined(_DEBUG)

