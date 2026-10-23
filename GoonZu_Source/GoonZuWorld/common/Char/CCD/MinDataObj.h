#ifndef _MINDATAOBJ_H_
#define _MINDATAOBJ_H_

#include "BaseDefinition.h"
#include <stdlib.h>
#include <stdio.h>

class CMinDataObj  
{
public:
	CMinDataObj();
	~CMinDataObj();

	void Initialize();
	void Destroy();

	void LoadMinData( TCHAR *strFileName );

	CMinDataObj	*GetPre();
	void SetPre( CMinDataObj *pPre );

	CMinDataObj	*GetNext();
	void SetNext( CMinDataObj *pNext );

	TCHAR* GetFileName();
	void SetFileName( TCHAR * );

	char GetCharacterKind();
	void SetCharacterKind( int CharacterKind );

	char GetFrameNum();
	void SetFrameNum( char cFrameNum );

	char GetTotalFrameNum();
	void SetTotalFrameNum( char cTotalFrameNum );

	char GetDirectionNum();
	void SetDierctionNum( char cDirectionNum );

	
	char GetActionIndex();
	void SetActionIndex( char cActionIndex );

	char GetDelayTimeIndex();

	int *GetDrawingSequence( char cDirection );
	void CopySequence( int **pDrawingSequence);
	void SetDrawingSequence( int *pDrawingSequence, char cDirection );

	bool IsSameMinFile( TCHAR *strFileName );

	void SequenceStadingInLine( int *Sequence );

private:
	CMinDataObj	*m_pPre;
	CMinDataObj	*m_pNext;

	TCHAR	m_strFileName[ _MAX_PATH ];

	char	m_cNation;
	int		m_siCharacterKind;
	int		m_cFrameNum;
	int		m_cTotalFrameNum;
	int		m_cDirectionNum;
	TCHAR	m_strActionCode[ MAX_ACTIONCODE_LENGTH ];
	int		m_cActionIndex;
	int		m_cDelayTimeIndex;

	int		m_siDrawingSequence[ 5 ][ MAX_DRAWING_SEQUENCE_NUM ];

	

};

#endif