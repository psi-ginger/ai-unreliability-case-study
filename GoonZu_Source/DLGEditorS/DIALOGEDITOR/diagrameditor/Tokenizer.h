#ifndef _CTOKENIZER_H_9BBD5DD2_819A_40CE_92574222C14D
#define _CTOKENIZER_H_9BBD5DD2_819A_40CE_92574222C14D

///////////////////////////////////////////////////////////
// File :		Tokenizer.h
// Created :	06/18/04
//
#include "../DLGDefine.h"

class CTokenizer
{
public:
// Construction/destruction
	CTokenizer( CString strInput, const CString & strDelimiter = _T(DLG_ITEM_SPLITTER_STR) );

// Operations
	void Init( const CString & strInput, const CString & strDelimiter = _T(DLG_ITEM_SPLITTER_STR) );
	int GetSize(  ) const;
	void GetAt( int nIndex, CString & str ) const;
	void GetAt( int nIndex, int & var ) const;
	void GetAt( int nIndex, WORD & var ) const;
	void GetAt( int nIndex, double & var ) const;
	void GetAt( int nIndex, DWORD & var ) const;


// Attributes

private:
	CStringArray m_stra;


};
#endif //_CTOKENIZER_H_9BBD5DD2_819A_40CE_92574222C14D
