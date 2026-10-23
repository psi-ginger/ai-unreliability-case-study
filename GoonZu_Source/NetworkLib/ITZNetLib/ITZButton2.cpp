#include "ITZButton2.h"


ITZButton::ITZButton()
{	
	m_pMgr = NULL;
	m_hWnd = NULL;
	
	m_state = BUTTON_STATE_NONE;
	m_index = -1;
	m_bChecked = FALSE;


}

ITZButton::~ITZButton()
{

}

BOOL ITZButton::PtInButton( int x, int y )
{
	if( m_x <= x && x < m_x + m_width &&
		m_y <= y && y < m_y + m_height ) {

		return TRUE;
	}

	return FALSE;
}

void ITZButton::OnDraw()
{

	if( m_hWnd == NULL ) return;

	HDC hdc = GetDC( m_hWnd );

	OnDraw( hdc );

	ReleaseDC( m_hWnd, hdc );
}

ITZButtonMgr::ITZButtonMgr()
{
	m_siMaxSize = 0;

	m_ppButton = NULL;
	m_pActiveButton = NULL;	

	m_hWnd = NULL;
}



ITZButtonMgr::~ITZButtonMgr()
{
	delete [] m_ppButton;
}


void ITZButtonMgr::CreateButtonMgr( HWND hWnd, int maxSize )
{

	m_hWnd = hWnd;

	m_pActiveButton = NULL;

	delete [] m_ppButton;

	m_ppButton = new ITZButton*[ maxSize ];

	for( int i = 0; i < maxSize; ++i ) {
		m_ppButton[ i ] = NULL;
	}
	
	m_siMaxSize = maxSize;

}

void ITZButtonMgr::DeleteAll()
{
	if( m_ppButton == NULL ) return;

	for( int i = 0; i < m_siMaxSize; ++i ) {
		m_ppButton[ i ] = NULL;
	}

	m_pActiveButton = NULL;
}

BOOL ITZButtonMgr::RegButton( int index, ITZButton *pButton )
{
	if( m_ppButton == NULL ) return FALSE;

	if( index < 0 || index >= m_siMaxSize ) return FALSE;

	pButton->m_pMgr = this;

	pButton->SetState( BUTTON_STATE_ENABLED );

	pButton->m_index = index;

	pButton->m_bChecked = FALSE;

	m_ppButton[ index ] = pButton;

	return TRUE;
}

ITZButton* ITZButtonMgr::GetButton( int index )
{
	return m_ppButton[ index ];
}

void ITZButtonMgr::OnMouseMove( int x, int y )
{

	if( m_pActiveButton ) {

		m_pActiveButton->OnMouseMove( x, y );

		if( m_pActiveButton->PtInButton( x, y ) ) { 

			SetCursor( LoadCursor( NULL, IDC_HAND ) );

			return;

		} else {

			m_pActiveButton->SetState( BUTTON_STATE_ENABLED );

			m_pActiveButton->OnDraw();

			m_pActiveButton = NULL;
		}

	}

	for( int i = 0; i < m_siMaxSize; ++i ) {
		
		if( m_ppButton[ i ] == NULL ) continue;

		if( m_ppButton[ i ]->PtInButton( x, y ) ) {

			SetCursor( LoadCursor( NULL, IDC_HAND ) );

			m_ppButton[ i ]->OnFocus();

			if( m_ppButton[ i ]->GetState() == BUTTON_STATE_ENABLED ) {

				m_ppButton[ i ]->SetState( BUTTON_STATE_FOCUSED );

				m_ppButton[ i ]->OnDraw();

				m_pActiveButton = m_ppButton[ i ];

			}

			break;
		}

	}

}

int ITZButtonMgr::OnLButtonDown( int x, int y )
{

	if( m_pActiveButton == NULL ) return -1;

	if( m_pActiveButton->PtInButton( x, y ) ) {

		m_pActiveButton->OnLButtonDown( x, y );

		SetCursor( LoadCursor( NULL, IDC_HAND ) );

		m_pActiveButton->SetState( BUTTON_STATE_PUSHED );

		m_pActiveButton->m_bChecked = !m_pActiveButton->m_bChecked;

		m_pActiveButton->OnDraw();

		return m_pActiveButton->m_index;

	} else {

		m_pActiveButton->OnLButtonUp( x, y );

		m_pActiveButton->SetState( BUTTON_STATE_ENABLED );

		m_pActiveButton->OnDraw();

		m_pActiveButton = NULL;
	}

	return -1;

}

int ITZButtonMgr::OnLButtonUp( int x, int y )
{

	if( m_pActiveButton == NULL ) return -1;

	if( m_pActiveButton->PtInButton( x, y ) ) {

		m_pActiveButton->OnLButtonUp( x, y );

		SetCursor( LoadCursor( NULL, IDC_HAND ) );

		m_pActiveButton->SetState( BUTTON_STATE_FOCUSED );

		m_pActiveButton->OnDraw();

		return m_pActiveButton->m_index;

	} else {

		m_pActiveButton->OnLButtonUp( x, y );

		m_pActiveButton->SetState( BUTTON_STATE_ENABLED );

		m_pActiveButton->OnDraw();

		m_pActiveButton = NULL;
	}

	return -1;
}

void ITZButtonMgr::OnDraw( HDC hdc )
{

	for( int i = 0; i < m_siMaxSize; ++i ) {

		if( m_ppButton[ i ] == NULL ) continue;

		if( m_ppButton[ i ]->m_hWnd == NULL ) {
			m_ppButton[ i ]->m_hWnd = m_hWnd;
		}

		m_ppButton[ i ]->OnDraw( hdc );
	}

}


