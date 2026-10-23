#include "ITZHttpConnect.h"

#define MAX_MH_KIND					16
#define MAX_MH_WRITER				32
#define MAX_MH_DATE					32
#define MAX_MH_TEXT					128

#define MAX_MH_MESSAGE_NUMBER		20

struct MiniHomeMessage
{
	TCHAR	szKind[ MAX_MH_KIND ];
	TCHAR	szWriter[ MAX_MH_WRITER ];
	TCHAR	szDate[ MAX_MH_DATE ];
	TCHAR	szText[ MAX_MH_TEXT ];
};


class cltMiniHomeUpdates
{
public:
	cltMiniHomeUpdates();
	~cltMiniHomeUpdates();

	void				Request( TCHAR *szAccountID );
	void				CheckUpdate();

	SI32				GetMessageCount() { return m_siCount; };
	
	SI32				m_siCount;
	MiniHomeMessage		m_message[ MAX_MH_MESSAGE_NUMBER ];

private:
	void				setText( SI32 index, SI32 col, TCHAR *pSrcMsg, SI32 bp, SI32 ep );

	BOOL				m_bRequest;

public:
	ITZHttpConnect		m_httpconnect;
};
