#ifndef _XSPRMGR_H_
#define _XSPRMGR_H_

#define	MAX_LOADED_FILE_NUM		500
enum { LOAD_FAIL, SUCCESS, FILE_EXIST };

class CXSPRObj;

class CXSPRMgr  
{
public:
	CXSPRMgr();
	~CXSPRMgr();

	void Initialize();
	void Destroy();

	CXSPRObj *GetFirst();
	void SetFirst( CXSPRObj *pNode );

	CXSPRObj *GetLast();
	void SetLast( CXSPRObj *pNode );

	char LoadXSPR( char *strFileName );

	void PushBack( CXSPRObj *pNode );
	void PopFront();
	void MoveToBack( CXSPRObj *pNode );

	CXSPRObj* IsSameFileExist( char *strFileName );

	char PutSprT( char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );
	char PutSprRT( char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos );

	short GetDirection( char *strFileName, char cDirection, short *pssAnimationNum );
	short GetTotalTileNum( char *strFileName );
private:
	CXSPRObj *m_pFirst;
	CXSPRObj *m_pLast;

	unsigned short	m_usImageNum;
};

#endif