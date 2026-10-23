#ifndef _XSPROBJ_H_
#define _XSPROBJ_H_

#include "graphic.h"

class CXSPRObj
{
public:
	CXSPRObj();
	~CXSPRObj();

	void Initialize();
	void Destroy();

	CXSPRObj *GetPre();
	void SetPre( CXSPRObj *pNode );

	CXSPRObj *GetNext();
	void SetNext( CXSPRObj *pNode );

	bool LoadXSPR( char *FileName );

	bool IsSameFile( char *strFileName );

	void PutSprT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );
	void PutSprRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );

	short GetDirectionStartIndexAndDirectionAnimationNum( char cDirection, short *pssAnimationNum );
	short GetTotalTileNum();

private:
	CXSPRObj *m_pPre;
	CXSPRObj *m_pNext;

	cltTSpr m_SprObj;

	char m_strFileName[ _MAX_PATH ];
	char m_strChangedFileName[ _MAX_PATH ];
	char m_ssEachDirectionAnimationNum;
};

#endif
