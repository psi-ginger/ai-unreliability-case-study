#ifndef _IDNUMTABLE_H
#define _IDNUMTABLE_H

#include "NHashTable.h"

class cltIDNumTable
{
public:
	cltIDNumTable();
	~cltIDNumTable();

	bool	AddIDNum( int idnum, int charid );
	bool	DelIDNum( int idnum );
	int		GetCharIDFromIDNum( int idnum );

	NHashTable< int, int >		m_hashtable;

};




#endif