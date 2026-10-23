	#ifndef _DIAGRAMPROPERTYDLG_H_
#define _DIAGRAMPROPERTYDLG_H_

#include "DiagramEntity.h"

#include "../../FileNameTokenizer.h"
#include "../../SprFile/SprFile.h"
#include "../../MyDefine/MyDefine.h"

#include "../StdGrfx.h"

class CDiagramEntity;
class CDiagramPropertyDlg : public CDialog
{
public:
	/**
	leon add
	* picture property double buffering
	*/
	void			MakeMemDC( CDC* dc, int tileImageStartFont = 0, int tileImageNum = 1);
	void			DrawImageOnMemDC(CDC* dc, int tileImageStartFont = 0, int tileImageNum = 1,  DWORD col_dest = TRANSPARENT_COLOR, DWORD col_src = TRANSPARENT_COLOR, DWORD col_exclusive_src = 0xffffffff, BOOL bexclusive = false );
	void			BitBltDS(CDC* dest, CDC* src, int dest_width, int dest_height, int src_width, int src_height, DWORD mode=SRCCOPY, int src_x=0, int src_y=0, int dest_x=0, int dest_y=0);

	bool			DrawTileImg( CDC* dest, SI16 PutSprOption, /*SI16 PutSprParam1,*/ SI16 PutSprParam2, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 TileImgNum, bool IsRowTile = true );

	void			DrawMiniPicture(CDC* in_pDC, int in_TileCount );
	void			DeleteAllBufferDC();
	void			DeleteAllBmp();
	void			ClearAllOldBmp();

	CDC*			GetBufferDC(int index);

	// font 선택 다이얼로그 상자 열기
	void			ShowFontDialog();

	CString			GetFontName();
	int				GetFontColor();
	int				GetFontSize();

	// 각 컨트롤 공통 작업 관련 함수들
	virtual void	SetValues() = 0;

	// 파일 이름을 분석해서 .. 각 컨트롤 속성 일부(타일 방식, 타일 개수)
	// 등을 분석 해온다.
	bool			ParsingFileFullPath(CString fullPath);
	CString			GetTileImageNum();
	CString			GetTileMode();

public:

	CDiagramPropertyDlg( UINT res, CWnd* parent );
	~CDiagramPropertyDlg();

	BOOL Create( UINT nIDTemplate, CWnd* pParentWnd );

	void			SetEntity( CDiagramEntity* entity );
	CDiagramEntity*	GetEntity() const;

	virtual CWnd*	GetRedrawWnd();
	virtual void	SetRedrawWnd( CWnd* redrawWnd );
	virtual void	Redraw();

	/**
	leon add
	* 프로퍼티 대화상자에서 입력 받은 파일을 간략하게 표현
	*/
	CString m_picFullPath;
	CString m_oldPicFullPath;

	CDC* m_pbufferDC[MAX_CONTROL_BUFFER_LENGTH];
	CBitmap* m_pmemBmp[MAX_CONTROL_BUFFER_LENGTH];
	CBitmap* m_pOldBmp[MAX_CONTROL_BUFFER_LENGTH];

	cltTSpr m_spr;

	// 컨트롤 프로퍼티에 폰트 속성 추가
	LOGFONT m_logFont;
	CString		m_fontName;
	int		m_fontSize;
	COLORREF m_fontColor;

	// FileName Parsing 후 저장 멤버변수
	// 파일 이름을 분석해서 .. 각 컨트롤 속성 일부(타일 방식, 타일 개수)
	// 등을 분석 해온다.
	CFileNameTok m_fileNameInfo;

	bool m_bError;	// 속성값 오류 발생 !! => 팝업창 안닫기 위해

private:
	CDiagramEntity*	m_entity;
	CWnd*			m_redrawWnd;

protected:
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

#endif // _DIAGRAMPROPERTYDLG_H_