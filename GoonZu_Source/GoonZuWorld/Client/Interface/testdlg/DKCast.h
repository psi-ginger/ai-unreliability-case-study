#define ICM_ICAST_MSG		(WM_USER + 10)
#define CONNECT_SERVER		(WM_USER + 20)
#define CUR_TITLE			(WM_USER + 30)
#define ICY_NAME			(WM_USER + 40)
#define ICY_GENRE			(WM_USER + 50)
#define ICY_URL				(WM_USER + 60)
#define ICY_PUB				(WM_USER + 70)
#define ICY_BR				(WM_USER + 80)
#define CONNECT_ERROR		(WM_USER + 100)

class CRecvMp3;

class CDKCast
{
	public:
		CDKCast();
		CDKCast(HWND hWnd);
		virtual ~CDKCast();
		
		void SetHWND(HWND hWnd);
		BOOL Play(LPCTSTR pszAddr, int nPort);
		BOOL Play(LPCTSTR pszPls);
		BOOL Stop();
		void SetVolume(int nVolume);

	private:
		CRecvMp3	*m_pCRecvMp3;
		BOOL		m_bPlayerSw;
};
