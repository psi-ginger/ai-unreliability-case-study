#include "stdafx.h"
#include "DiagramControlFactory.h"

#include "DiagramButton.h"
#include "DiagramEdit.h"
#include "DiagramStatic.h"
#include "DiagramCheckbox.h"
#include "DiagramRadiobutton.h"
#include "DiagramCombobox.h"
#include "DiagramListbox.h"
#include "DiagramImageStatic.h"
#include "DiagramOutLine.h"
#include "DiagramChatEdit.h"

#include "DiagramGroupbox.h"
#include "DiagramListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramControlFactory

CDiagramEntity* CDiagramControlFactory::CreateFromString( const CString& str )
{
	CDiagramEntity* obj;

	obj = CDiagramButton::CreateFromString( str );
	if( !obj )
		obj = CDiagramEdit::CreateFromString( str );
	if( !obj )
		obj = CDiagramStatic::CreateFromString( str );
	if( !obj )
		obj = CDiagramCheckbox::CreateFromString( str );
	if( !obj )
		obj = CDiagramRadiobutton::CreateFromString( str );
	if( !obj )
		obj = CDiagramCombobox::CreateFromString( str );
	if( !obj )
		obj = CDiagramListbox::CreateFromString( str );
	if( !obj )
		obj = CDiagramListView::CreateFromString( str );
	//if( !obj )
	//	obj = CDiagramGroupbox::CreateFromString( str );
	/**
	leon add
	* new object added!!
	*/
	if( !obj )
		obj = CDiagramImageStatic::CreateFromString( str );
	if( !obj )
		obj = CDiagramOutLine::CreateFromString( str );
	if( !obj )
		obj = CDiagramChatEdit::CreateFromString( str );

	return obj;
}