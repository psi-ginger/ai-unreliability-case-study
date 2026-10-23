#pragma  once


#include "../../Client/Client.h"
#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;

#define NBuffstatus_MAX_Buffstatus	64

class CNBuffstatus : public CDialogController
{
public:
	CNBuffstatus();
	~CNBuffstatus();

	void Initialize();
	void Destroy();

	void Create();

	static void CALLBACK StaticBuffProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NBuffDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Action();
	bool	IsBuffShow();
	void	SetBuffTooltip();
	TCHAR*	MakeAddText( SI32 TextNum, SI32 Value );
	TCHAR*  MakeSetItemToolTip( cltCharCommon* _pclChar );
public:

	SI16			m_siImageNum;
	SI16			m_siImageXsize;
	SI16			m_siImageYSize;
	SI32			siClientScreenXsize;
	SI32			siClientScreenYsize;


	CImageStatic*	m_BuffStatic[NBuffstatus_MAX_Buffstatus] ;

	TSpr			m_BuffImg;
	bool			m_BuffShow;

	//CDialogController	*m_pDialog[ DIALOG_END ];



};