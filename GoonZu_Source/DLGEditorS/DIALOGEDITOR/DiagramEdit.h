#if !defined(AFX_DIAGRAMEDIT_H__A34BFDFD_1CBE_463B_94EF_907D9123D614__INCLUDED_)
#define AFX_DIAGRAMEDIT_H__A34BFDFD_1CBE_463B_94EF_907D9123D614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagramEdit.h : header file
//

#include "DiagramEditor/DiagramEntity.h"
//#include "EditPropertyDlg.h"
#include "EditBoxPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramEdit window

class CDiagramEdit : public CDiagramEntity
{
// Construction
public:
	CDiagramEdit();
	~CDiagramEdit();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 ) const;

	void SetPropertyDefault();	// 속성을 기본값으로 셋팅한다.
	void IncreaseUniqueIDNum();	// 고유 id 번호를 하나 증가 시킨다.

private:
	CEditBoxPropertyDlg	m_dlg;

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DIAGRAMEDIT_H__A34BFDFD_1CBE_463B_94EF_907D9123D614__INCLUDED_)
