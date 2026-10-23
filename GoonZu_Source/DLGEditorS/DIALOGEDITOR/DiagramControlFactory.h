#ifndef _DIAGRAMCONTROLFACTORY_H_
#define _DIAGRAMCONTROLFACTORY_H_

#include "DiagramEditor\DiagramEntity.h"

class CDiagramControlFactory {

public:
	static CDiagramEntity* CreateFromString( const CString& str );

};

#endif // _DIAGRAMCONTROLFACTORY_H_