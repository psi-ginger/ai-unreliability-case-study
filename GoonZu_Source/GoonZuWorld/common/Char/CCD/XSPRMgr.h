#ifndef _XSPRMGR_H_
#define _XSPRMGR_H_

enum { LOAD_FAIL, SUCCESS, FILE_EXIST };

#include "../../../Lib/Graphic/Graphic.h"
#include <string>
#include <map>
#include "TSpr.h"
#include "BaseDefinition.h"

using namespace std;


/*
#include "..\..\SMLib\LibList\LibListMgr.h"

class CXSPRObj;

class CXSPRMgr : public CLibListMgr
{
public:
	CXSPRMgr();
	~CXSPRMgr();

	void Initialize();
	void Destroy();

	void CreateEmptyObj( SI32 siEmptyObj );

	char LoadXSPR( TCHAR *strFileName, SI16 CharacterKind, char cPart, short ssActionIndex );

	CXSPRObj* IsSameFileExist( TCHAR *strFileName, SI16 CharacterKind, char cPart, short ssActionIndex );

	cltTSpr* PutSprT( SI16 CharacterKind, char cPart, short ssActionIndex, char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );
	cltTSpr* PutSprRT(  SI16 CharacterKind, char cPart, short ssActionIndex, char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );

	cltTSpr* PutSprScreenT( SI16 CharacterKind, char cPart, short ssActionIndex, char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );
	cltTSpr* PutSprScreenRT( SI16 CharacterKind, char cPart, short ssActionIndex, char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );

	short GetDirection( TCHAR *strFileName, char cDirection, short *pssAnimationNum );
	short GetTotalTileNum( TCHAR *strFileName );

	int GetLoadedImageNum();
	int GetLoadedImageDataSize( unsigned int *pFileSize );

	void LoadSimilarXSPR( TCHAR *strFileName, SI16 CharacterKind, char cPart, short ssActionIndex );

	void Action();

private:

};
*/

#include "..\..\SMLib\LibList\LibListMgr.h"

class CXSPRObj;

class CXSPRMgr : public CLibListMgr
{
public:
	CXSPRMgr();
	~CXSPRMgr();

	void Initialize();
	void Destroy();

	void CreateEmptyObj( SI32 siMaxEmptyObj );

	char LoadXSPR( TCHAR *strFileName, SI16 CharacterKind, char cPart, short ssActionIndex );

	CXSPRObj* IsSameFileExist( const TCHAR *strFileName );

	TSpr* PutSprT( short ssActionIndex, TCHAR *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );
	TSpr* PutSprRT(  short ssActionIndex, TCHAR *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );

	TSpr* PutSprScreenT( short ssActionIndex, TCHAR *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );
	TSpr* PutSprScreenRT( short ssActionIndex, TCHAR *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );


	short GetDirection( TCHAR *strFileName, char cDirection, short *pssAnimationNum );
	short GetTotalTileNum( TCHAR *strFileName );

	int GetLoadedImageNum();
	int GetLoadedImageDataSize( unsigned int *pFileSize );

	void LoadSimilarXSPR( TCHAR *strFileName, short ssActionIndex );

	void Action();

private:
	map<tstring,CXSPRObj*> m_XSPRMap;

};


#endif