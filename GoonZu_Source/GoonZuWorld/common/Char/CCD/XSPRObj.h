#ifndef _XSPROBJ_H_
#define _XSPROBJ_H_

#include "BaseDefinition.h"

#include "TSpr.h"

/*
#include "..\..\SMLib\LibList\Obj.h"

class CXSPRObj : public CObj
{
public:
	CXSPRObj();
	~CXSPRObj();

	void Initialize();
	void Destroy();

	bool LoadXSPR( TCHAR *FileName );
	bool LoadXSPR( TCHAR *FileName, char cSex, char cPart, short ssActionIndex );

	bool IsSameFile( TCHAR *strFileName );
	bool IsSameFile( TCHAR *strFileName, char cSex, char cPart, short ssActionIndex );

	cltTSpr* PutSprT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );
	cltTSpr* PutSprRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );

	cltTSpr* PutSprScreenT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );
	cltTSpr* PutSprScreenRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );

	short GetDirectionStartIndexAndDirectionAnimationNum( char cDirection, short *pssAnimationNum );
	short GetTotalTileNum();

	unsigned int GetImageFileSize();

	void SetInfo( char cSex, char cPart, short ssActionIndex );

	TCHAR* GetSimilarFileName( char cSex, char cPart, short ssActionIndex );

	bool Action( UI32 uiCurrentFrame );

private:
	cltTSpr m_SprObj;

	TCHAR m_strFileName[ _MAX_PATH ];
	TCHAR m_strChangedFileName[ _MAX_PATH ];
	TCHAR m_ssEachDirectionAnimationNum;

	short m_ssActionIndex;
	char m_cPart;
	char m_cSex;

	UI32 m_uiLastDrawFrame;
	

	unsigned int m_uiImageFileSize;
};
*/

#include "..\..\SMLib\LibList\Obj.h"

class CXSPRObj : public CObj
{
public:
	CXSPRObj();
	~CXSPRObj();

	void Initialize();
	void Destroy();

	bool LoadXSPR( TCHAR *FileName );
	
	TSpr* PutSprT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );
	TSpr* PutSprRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );

	TSpr* PutSprScreenT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );
	TSpr* PutSprScreenRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos = 0 );


	short GetDirectionStartIndexAndDirectionAnimationNum( char cDirection, short *pssAnimationNum );
	short GetTotalTileNum();

	unsigned int GetImageFileSize();

	bool Action( UI32 uiCurrentFrame );

	TCHAR* GetFileName()
	{
		return m_strFileName;
	}

private:
	TSpr m_SprObj;

	TCHAR m_strFileName[ _MAX_PATH ];
	TCHAR m_strChangedFileName[ _MAX_PATH ];
	char m_ssEachDirectionAnimationNum;

	UI32 m_uiLastDrawFrame;

	unsigned int m_uiImageFileSize;
};

#endif