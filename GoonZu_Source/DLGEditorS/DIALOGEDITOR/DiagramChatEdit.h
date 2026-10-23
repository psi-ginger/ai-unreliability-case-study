#pragma once
// DiagramEdit.h : header file
//

#include "DiagramEditor/DiagramEntity.h"
//#include "EditPropertyDlg.h"
#include "ChatEditPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramChatEdit window

class CDiagramChatEdit : public CDiagramEntity
{
// Construction
public:
	CDiagramChatEdit();
	~CDiagramChatEdit();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 ) const;

	void SetPropertyDefault();	// 속성을 기본값으로 셋팅한다.
	void IncreaseUniqueIDNum();	// 고유 id 번호를 하나 증가 시킨다.

private:
	CChatEditPropertyDlg	m_dlg;

};

