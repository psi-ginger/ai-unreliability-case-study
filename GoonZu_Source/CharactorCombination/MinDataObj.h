#ifndef _MINDATAOBJ_H_
#define _MINDATAOBJ_H_

#include "BaseDefinition.h"
#include <stdio.h>

class CMinDataObj  
{
public:
	CMinDataObj();
	~CMinDataObj();

	void Initialize();
	void Destroy();

	void LoadMinData( char *strFileName );

	CMinDataObj	*GetPre();
	void SetPre( CMinDataObj *pPre );

	CMinDataObj	*GetNext();
	void SetNext( CMinDataObj *pNext );

	char* GetFileName();
	void SetFileName( char * );

	char GetSex();
	void SetSex( char cSex );

	char GetFrameNum();
	void SetFrameNum( char cFrameNum );

	char GetTotalFrameNum();
	void SetTotalFrameNum( char cTotalFrameNum );

	char GetDirectionNum();
	void SetDierctionNum( char cDirectionNum );

	char *GetActionCode();
	void SetActionCode( char *pActionCode );

	char GetActionIndex();
	void SetActionIndex( char cActionIndex );

	char GetDelayTime();

	int *GetDrawingSequence( char cDirection );
	void CopySequence( int **pDrawingSequence);
	void SetDrawingSequence( int *pDrawingSequence, char cDirection );

	void SequenceStadingInLine( int *pDrawingSequence );

	bool IsSameMinFile( char *strFileName );


	

private:
	CMinDataObj	*m_pPre;
	CMinDataObj	*m_pNext;

	char	m_strFileName[ MAX_PATH ];

	char	m_cNation;
	int		m_cSex;
	int		m_cFrameNum;
	int		m_cTotalFrameNum;
	int		m_cDirectionNum;
	char	m_strActionCode[ MAX_ACTIONCODE_LENGTH ];
	int		m_cActionIndex;
	int		m_cDelayTime;

	int		m_siDrawingSequence[ 5 ][ MAX_DRAWING_SEQUENCE_NUM ];

	

};

#endif