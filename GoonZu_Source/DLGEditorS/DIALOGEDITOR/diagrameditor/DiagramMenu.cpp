#include "stdafx.h"
#include "DiagramMenu.h"
#include "DiagramEditor.h"

CDiagramMenu::CDiagramMenu()
/* ============================================================
	Function :		CDiagramMenu::CDiagramMenu
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Always heap allocate

   ============================================================*/
{
}

CDiagramMenu::~CDiagramMenu()
/* ============================================================
	Function :		CDiagramMenu::~CDiagramMenu
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Note that the editor will delete attached 
					instances automatically

   ============================================================*/
{

	if( m_menu.m_hMenu != NULL )
		m_menu.DestroyMenu();

}

CMenu* CDiagramMenu::GetPopupMenu( CDiagramEditor* editor )
/* ============================================================
	Function :		CDiagramMenu::GetPopupMenu
	Description :	Gets a menu pointer to the desired popup 
					menu.
	Access :		Public

	Return :		CMenu*					-	A pointer to 
												the popup menu
	Parameters :	CDiagramEditor* editor	-	The editor 
												calling for a 
												menu.
					
	Usage :			Call to get the popup menu for the editor.

   ============================================================*/
{

	if( m_menu.m_hMenu == NULL )
	{
		if( m_menu.CreatePopupMenu() )
		{

			m_menu.AppendMenu( MF_STRING, ID_EDIT_CUT, _T( "Cut" ) );
			m_menu.AppendMenu( MF_STRING, ID_EDIT_COPY, _T( "Copy" ) );
			m_menu.AppendMenu( MF_STRING, ID_EDIT_PASTE, _T( "Paste" ) );

			m_menu.AppendMenu( MF_SEPARATOR );

			m_menu.AppendMenu( MF_STRING, ID_EDIT_GROUP, _T( "Group" ) );
			m_menu.AppendMenu( MF_STRING, ID_EDIT_UNGROUP, _T( "Ungroup" ) );

			/**
			leon add
			* Dialog Property 추가
			*/
			m_menu.AppendMenu( MF_STRING, ID_DLG_PROPERTY, _T( "Properties" ) );			
		}
	}

	UINT cut = MF_GRAYED;
	UINT copy = MF_GRAYED;
	UINT paste = MF_GRAYED;
	UINT group = MF_GRAYED;
	if( editor->IsAnyObjectSelected() )
	{
		cut = 0;
		copy = 0;
	}

	if( editor->GetSelectCount() > 1 )
		group = 0;

	if( editor->GetDiagramEntityContainer() && editor->GetDiagramEntityContainer()->ObjectsInPaste() )
		paste = 0;

	/**
	leon add
	* Edit 기능 사용 안함
	*/
	cut = MF_GRAYED;
	copy = MF_GRAYED;
	paste = MF_GRAYED;

	m_menu.EnableMenuItem( ID_EDIT_CUT, MF_BYCOMMAND | cut );
	m_menu.EnableMenuItem( ID_EDIT_COPY, MF_BYCOMMAND | copy );
	m_menu.EnableMenuItem( ID_EDIT_PASTE, MF_BYCOMMAND | paste );
	m_menu.EnableMenuItem( ID_EDIT_GROUP, MF_BYCOMMAND | group );
	m_menu.EnableMenuItem( ID_EDIT_UNGROUP, MF_BYCOMMAND | group );

	return &m_menu;

}