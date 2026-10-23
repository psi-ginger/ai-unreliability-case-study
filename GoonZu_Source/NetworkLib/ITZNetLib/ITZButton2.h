#ifndef _ITZBUTTON_H
#define _ITZBUTTON_H

#define BUTTON_STATE_NONE			0
#define BUTTON_STATE_ENABLED		1
#define BUTTON_STATE_PUSHED			2
#define BUTTON_STATE_FOCUSED		3
#define BUTTON_STATE_DISABLED		4

#include "itzbase.h"

#ifndef IDC_HAND
#define IDC_HAND					0
#endif


class ITZButtonMgr;

class ITZButton
{
friend class ITZButtonMgr;
public:
	ITZButton();
	virtual ~ITZButton();

public:
	void				SetState( int state ) { m_state = state ; };
	int					GetState() { return m_state; };

	BOOL				IsChecked() { return m_bChecked; };
	void				SetChecked( BOOL bChk ) { m_bChecked = bChk; };

	void				SetPosition( int x, int y ) { m_x = x; m_y = y; };
	void				SetPosition( int x, int y, int width, int height ) { 
							m_x = x; m_y =y; m_width = width; m_height = height; };

	virtual	BOOL		PtInButton( int x, int y );

	virtual void		Run() {};
	
	virtual void		OnDraw( HDC hdc ) {};									// 상태에 따른 출력
	virtual void		OnDraw();

	virtual void		OnFocus() {};											// 포커스
	virtual void		OnMouseMove( int x, int y ) {};							// 마우스 이동 
	virtual void		OnLButtonDown( int x, int y ) {};						// 마우스 왼쪽 버튼 다운
	virtual void		OnLButtonUp( int x, int y ) {};							// 마우스 왼쪽 버튼 업 

protected:
	ITZButtonMgr*		m_pMgr;

	HWND				m_hWnd;

	int					m_state;
	int					m_index;

	BOOL				m_bChecked;

public:
	int					m_x;
	int					m_y;
	int					m_width;
	int					m_height;

};


class ITZButtonMgr
{
friend class ITZButton;
public:
	ITZButtonMgr();
	~ITZButtonMgr();

public:

	void			CreateButtonMgr( HWND hwnd, int maxSize );

	BOOL			RegButton( int index, ITZButton *pButton );
	ITZButton*		GetButton( int index );

	void			DeleteAll();

	void			OnMouseMove( int x, int y );
	int				OnLButtonDown( int x, int y );
	int				OnLButtonUp( int x, int y );

	void			OnDraw( HDC hdc );

public:
	ITZButton**		m_ppButton;

	ITZButton*		m_pActiveButton;
	
	int				m_siMaxSize;

	POINT			m_point;

public:
	HWND			m_hWnd;
};

#endif
