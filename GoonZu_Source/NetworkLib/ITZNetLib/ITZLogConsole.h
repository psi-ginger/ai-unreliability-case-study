#include <commctrl.h>

class ITZConsole
{
public:
	ITZConsole();
	~ITZConsole();

	void				CreateConsole( HWND hWnd, HINSTANCE hInst, int x, int y, int width, int height, int maxline );

	void				Write( char *pBuf );

public:

	HWND				m_hList;
	
	SI32				m_siMaxLine;

	CRITICAL_SECTION	m_cs;

};

ITZConsole::ITZConsole()
{
	InitializeCriticalSection( &m_cs );
}

ITZConsole::~ITZConsole()
{
	DeleteCriticalSection( &m_cs );
}

void ITZConsole::CreateConsole( HWND hWnd, HINSTANCE hInst, int x, int y, int width, int height, int maxline )
{


	m_hList = CreateWindow( WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER, 
		x, y, width, height, hWnd, NULL, hInst, NULL );

	m_siMaxLine = maxline;	

	LVCOLUMN lvcol;
	LVITEM lvitem;

	lvcol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvcol.cx = width;
	lvcol.fmt = LVCFMT_LEFT;
	lvcol.pszText = "";
	lvcol.iSubItem = 0;

	ListView_InsertColumn( m_hList, 0, &lvcol );

	AllocConsole();
	//AllocConsole();

	SetConsoleMode( GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT );
}


void ITZConsole::Write( char *pBuf )
{

	EnterCriticalSection( &m_cs );

	
	int nCount = ListView_GetItemCount( m_hList );

	if( nCount == m_siMaxLine ) {

		ListView_DeleteItem( m_hList, 0 );
	}
	
	LVITEM lvitem;

	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = nCount;
	lvitem.iSubItem = 0;
	char buf[ 1024 ];
	wsprintf( buf, "Test Item %d", rand() );

	lvitem.pszText = buf;


	if( ListView_InsertItem( m_hList, &lvitem ) == -1 ) MessageBox( 0, "Failed", 0, 0 );

	ListView_Scroll( m_hList, 0, 20 );

	LeaveCriticalSection( &m_cs );
	
}



