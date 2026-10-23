// DiagramRadiobutton.h : header file
//

#pragma once

#include "DiagramEditor/DiagramEntity.h"
#include "RadioButtonPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramRadiobutton window

class CDiagramRadiobutton : public CDiagramEntity
{
// Construction
public:
	CDiagramRadiobutton();
	~CDiagramRadiobutton();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 ) const;

	void SetPropertyDefault();	// 속성을 기본값으로 셋팅한다.
	void IncreaseUniqueIDNum();	// 고유 id 번호를 하나 증가 시킨다.


private:
	CRadioButtonPropertyDlg	m_dlg;

};
