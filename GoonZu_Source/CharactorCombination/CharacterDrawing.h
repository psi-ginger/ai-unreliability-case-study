#ifndef _CHARACTERDRAWING_H_
#define _CHARACTERDRAWING_H_


#include <stdio.h>
#include <direct.h>
#include "./Graphic.h"

#include "BaseDefinition.h"

class CMinDataMgr;

struct stActionCode
{
	int cActionIndex;
	char strActionCode[ MAX_ACTIONCODE_LENGTH ];
	char strActionName[ MAX_ITEM_NAME ];				// MAX_ITEM_NAME 과 관계 전혀 없음. 그냥 24 있고, 또 디파인 하기 그래서 그냥 했음.
	int cFrameNum;
	int cTotalFrameNum;
	int cDirection;
};

struct stArmorCode
{
	char strArmorName[ MAX_ITEM_NAME ];
	char strArmorCode[ MAX_ITEMCODE_LENGTH ];
	unsigned int uiArmorIndex;
};

struct stHelmetCode
{
	char strHelmetName[ MAX_ITEM_NAME ];
	char strHelmetCode[ MAX_ITEMCODE_LENGTH ];
	unsigned int uiHelmetIndex;
};

struct stWeaponCode
{
	char strWeaponName[ MAX_ITEM_NAME ];
	char strWeaponCode[ MAX_ITEMCODE_LENGTH ];	
	int cWeaponAction;
	char strWeaponActionCode[ MAX_ACTIONCODE_LENGTH ];
	unsigned int uiWeaponIndex;
};

struct stMantleCode
{
	char strMantleName[ MAX_ITEM_NAME ];
	char strMantleCode[ MAX_ITEMCODE_LENGTH ];
	unsigned int uiMantleIndex;
};

struct stHorseCode
{
	char strHorseName[ MAX_ITEM_NAME ];
	char strHorseCode[ MAX_ITEMCODE_LENGTH ];
	unsigned int uiHorseIndex;
};

struct stHorseHeadCode
{
	char strHorseHeadName[ MAX_ITEM_NAME ];
	char strHorseHeadCode[ MAX_ITEMCODE_LENGTH ];
	unsigned int uiHorseHeadIndex;
};

class CCharacterDrawing  
{
public:
	CCharacterDrawing();
	~CCharacterDrawing();

	void	ReadActionCode();
	void	ReadHelmetCode();
	void	ReadArmorCode();
	void	ReadWeaponCode();
	void	ReadMantleCode();
	void	ReadHorseCode();
	void	ReadHorseHeadCode();
	void	ReadMinFile();
	void	ReadFileList();

	void	LoadMinFileData();

	void	Initialize();
	void	Destroy();

	

	char	GetCurrentDirection();
	void	SetCurrentDirection( char cCurrentDirection, int *pDrawSequence );

	short	GetAnimationStartIndex();
	void	SetAnimationStartIndex( short ssAnimationStartIndex );

	short	GetAnimationEndIndex();
	void	SetAnimationEndIndex( short ssAnimationEndIndex );

	short	GetCurrentAnimationIndex();
	void	SetCurrentAnimationIndex( short ssCurrentAnimationIndex );

	char	*GetActionName( int cIndex );
	char	*GetActionCode( int cIndex );
	char	GetActionNum();

	char	*GetHelmetName( int cIndex );
	char	*GetHelmetCode( int cIndex );
	unsigned int GetHelmetNum();

	char	*GetArmorName( int cIndex );
	char	*GetArmorCode( int cIndex );
	unsigned int GetArmorNum();

	char	*GetWeaponName( int cIndex );
	char	*GetWeaponCode( int cIndex );
	char	GetWeaponActionIndex( int cIndex );
	unsigned int GetWeaponNum();

	char	*GetMantleName( int cIndex );
	char	*GetMantleCode( int cIndex );
	unsigned int GetMantleNum();

	char	*GetHorseName( int cIndex );
	char	*GetHorseCode( int cIndex );
	unsigned int GetHorseNum();

	char	*GetHorseHeadName( int cIndex );
	char	*GetHorseHeadCode( int cIndex );
	unsigned int GetHorseHeadNum();

		

	void	LoadImageDataFromList( char *strHelmet, char *strArmor, char *strWeapon, char *strMantle, char *strHorse, char *strHorseHead );

	void	LoadItemCodeData( char cSex, char *strHelmet, char *strArmor, char *strWeapon, char *strMantle, char *strHorse, char *strHorseHead );
	void	DrawCharacter( char *strAction, char cDirection, short ssAnimationIndex, short ssBottomX, short ssBottomY );

	
	// 아래의 2개는 툴을 위한 것임
	void	GetDrawingInfo( char *strFilePath );	
	void	DrawCharacter( int *pDrawSequence, char cActionIndex, char cDirection, short ssBottomX, short ssBottomY );

	void	DrawPreFrame();
	void	DrawNextFrame();

	void	SaveMinFile( int siDelayTime );
	void	ReloadMinFileList();
	void	LoadMinFile( char *strFileName, int **ppSequence, char *pActionIndex, char *pSex, char *pDelayTime );

	char	*FileName( int siIndex );

	
	
private:

	CMinDataMgr	*m_pMinDataMgr;

	short	m_ssAnimationNum;
	char	m_cCurrentDirection;
	
	short	m_ssAnimationStartIndex;
	short	m_ssAnimationEndIndex;

	short	m_ssCurrentAnimationIndex;

	char	m_cCurrentActionIndex;

	bool	m_bReverse;

	int		m_siDrawingSequence[ 5 ][ MAX_DRAWING_SEQUENCE_NUM ];								// 앞의 5는 다섯 방향을 뜻하고, 뒤의 5는 찍는 순서임( 머리, 몸통, 왼손, 오른손, 망토, 말 )

	
	
	char	m_cSex;														// 남자 = 1, 여자 = 2 구분
	bool	m_bIsTakeHorse;
	bool	m_bIsTakeMantle;
	bool	m_bIsTakeEquipment;
	bool	m_bIsTakeHorseHead;
	char	m_strSex[ 4 ];												// 남자(TP1), 여자(TP2)
	char	m_strTempSex[ 4 ];												// 남자(TP1), 여자(TP2)
	char	m_strHelmet[ MAX_ITEMCODE_LENGTH ];							// 헬멧 아이템 코드
	char	m_strArmor[ MAX_ITEMCODE_LENGTH ];							// 갑옷 아이템 코드
	char	m_strWeapon[ MAX_ITEMCODE_LENGTH ];							// 무기 아이템 코드
	char	m_strMantle[ MAX_ITEMCODE_LENGTH ];							// 망토 종류 코드
	char	m_strEquipment[ MAX_ITEMCODE_LENGTH ];						// 장비 아이템 코드
	char	m_strHorse[ MAX_ITEMCODE_LENGTH ];							// 말 종류 코드
	char	m_strHorseHead[ MAX_ITEMCODE_LENGTH ];							// 말머리 종류 코드

	char	m_strFileName[ 5 ][ MAX_DRAWING_SEQUENCE_NUM ][ MAX_PATH ];						// 전체 완성된 파일명( 5방향과 각 방향별 출력 순서에 따라서 출력 파일명을 가지고 있음 )

	char	m_strImageFileNameList[ MAX_FILE_NUM ][ MAX_PATH ];			// 파일 최대 이름은 128 byte
	
	int		m_siImageFileNum;
	int		m_siMinFileNum;

	stActionCode	*m_pstAction;
	int		m_cActionNum;

	stArmorCode		*m_pstArmor;
	unsigned int	m_uiArmorNum;

	stHelmetCode	*m_pstHelmet;
	unsigned int	m_uiHelmetNum;

	stWeaponCode	*m_pstWeapon;
	unsigned int	m_uiWeaponNum;

	stMantleCode	*m_pstMantle;
	unsigned int	m_uiMantleNum;

	stHorseCode		*m_pstHorse;
	unsigned int	m_uiHorseNum;

	stHorseHeadCode	*m_pstHorseHead;
	unsigned int	m_uiHorseHeadNum;

	short	i, j;

	

};

#endif