
#pragma once
// DiagramStatic.h : header file
//

#include "DiagramEditor/DiagramEntity.h"
#include "OutLinePropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramOutLine window

class CDiagramOutLine : public CDiagramEntity
{
// Construction
public:
	CDiagramOutLine();
	~CDiagramOutLine();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 );

	void SetPropertyDefault();	// 속성을 기본값으로 셋팅한다.
	void IncreaseUniqueIDNum();	// 고유 id 번호를 하나 증가 시킨다.

private:
	COutLinePropertyDlg m_dlg;

};

/////////////////////////////////////////////////////////////////////////////

