#if !defined(AFX_DIAGRAMBUTTON_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_)
#define AFX_DIAGRAMBUTTON_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_

// DiagramButton.h : header file
//
#include "DLGDefine.h"
#include "DiagramEditor/DiagramEntity.h"
#include "ButtonPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramButton window

class CDiagramButton : public CDiagramEntity
{
// Construction
public:
	CDiagramButton();
	~CDiagramButton();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 ) const;
	
	void SetPropertyDefault();	// 속성을 기본값으로 셋팅한다.
	void IncreaseUniqueIDNum();	// 고유 id 번호를 하나 증가 시킨다.

	private:
	CButtonPropertyDlg	m_dlg;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DIAGRAMBUTTON_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_)
