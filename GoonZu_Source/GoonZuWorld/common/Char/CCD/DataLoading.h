#ifndef _DATALOADING_H_
#define _DATALOADING_H_

#include "BaseDefinition.h"

struct stActionCode
{
	int cActionIndex;	
	int cFrameNum;
	int cTotalFrameNum;
	int cDirection;
	int cAttackFrameNum;
	TCHAR strActionCode[ MAX_ACTIONCODE_LENGTH ];
	TCHAR strActionName[ MAX_ITEM_NAME ];				// MAX_ITEM_NAME 과 관계 전혀 없음. 그냥 24 있고, 또 디파인 하기 그래서 그냥 했음.
};

struct stArmorCode
{
	TCHAR strArmorName[ MAX_ITEM_NAME ];
	TCHAR strArmorCode[ MAX_ITEMCODE_LENGTH ];
	unsigned int uiArmorIndex;
};

struct stHelmetCode
{
	TCHAR strHelmetName[ MAX_ITEM_NAME ];
	TCHAR strHelmetCode[ MAX_ITEMCODE_LENGTH ];
	unsigned int uiHelmetIndex;
};

struct stWeaponCode
{
	TCHAR strWeaponName[ MAX_ITEM_NAME ];
	TCHAR strWeaponCode[ MAX_ITEMCODE_LENGTH ];	
	int  cWeaponAction;
	TCHAR strWeaponActionCode[ MAX_ACTIONCODE_LENGTH ];
	unsigned int uiWeaponIndex;
};

struct stMantleCode
{
	TCHAR strMantleName[ MAX_ITEM_NAME ];
	TCHAR strMantleCode[ MAX_ITEMCODE_LENGTH ];
	unsigned int uiMantleIndex;
};

struct stHorseCode
{
	TCHAR strHorseName[ MAX_ITEM_NAME ];
	TCHAR strHorseCode[ MAX_ITEMCODE_LENGTH ];
	unsigned int uiHorseIndex;
};



class CDataLoading  
{
public:
	CDataLoading();
	~CDataLoading();

	void ReadActionCode();
	void SetAttackFrameNum();
	void ReadHelmetCode();
	void ReadArmorCode();
	void ReadWeaponCode();
	void ReadMantleCode();
	void ReadHorseCode();
	void ReadFileList();
	void LoadMinFileData();

	TCHAR	*GetHelmetName( char cIndex );
	TCHAR	*GetHelmetCode( char cIndex );
	unsigned int GetHelmetNum();

	TCHAR	*GetArmorName( char cIndex );
	TCHAR	*GetArmorCode( char cIndex );
	unsigned int GetArmorNum();

	TCHAR	*GetWeaponName( char cIndex );
	TCHAR	*GetWeaponCode( char cIndex );
	int		GetWeaponActionIndex( char cIndex );
	unsigned int GetWeaponNum();

	TCHAR	*GetMantleName( char cIndex );
	TCHAR	*GetMantleCode( char cIndex );
	unsigned int GetMantleNum();

	TCHAR	*GetHorseName( char cIndex );
	TCHAR	*GetHorseCode( char cIndex );
	unsigned int GetHorseNum();

	void	GetWeaponInfo( TCHAR *strWeapon, short &ssWeaponActionIndex );
	//stActionCode	m_pstAction[20];
	stActionCode	*m_pstAction;

	int		m_siImageFileNum;
	int		m_siMinFileNum;

	TCHAR	m_strImageFileNameList[ MAX_FILE_NUM ][ _MAX_PATH ];			// 파일 최대 이름은 128 byte

private:	
	//stActionCode	*m_pstAction;
	int				m_cActionNum;

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

	

};

#endif