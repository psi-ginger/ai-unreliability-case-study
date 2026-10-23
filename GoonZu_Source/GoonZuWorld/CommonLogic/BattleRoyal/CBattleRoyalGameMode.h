//
//
//  @ File Name : CBattleRoyalGameMode.h
//  @ Date : 2009-09-25
//  @ Author : �ڱ���
//
//

#if !defined(_CBATTLEROYALGAMEMODE_H)
#define _CBATTLEROYALGAMEMODE_H

#include "../CommonLogic.h"
#include "./CBattleRoyalPersonChangedData.h"
#include "./CBattleRoyalStadium.h"

enum 
{
	BATTLEROYALGAMEMODE_START = 0,
	//==================================================================
	BATTLEROYALGAMEMODE_NORMAL,					// �Ϲ�
	BATTLEROYALGAMEMODE_RESTRICTTOUSEHEALITEM,	// "���� ������ ��� ����" ���			
	BATTLEROYALGAMEMODE_ONEKILL,				// "HP ����" ���
	BATTLEROYALGAMEMODE_POWERUPONEUSER,			// "�� ���� �Ŀ� ��" ���
	//==================================================================
	BATTLEROYALGAMEMODE_END,
	BATTLEROYALGAMEMODE_COUNT = BATTLEROYALGAMEMODE_END -1
};

class CBattleRoyalGameMode {

public:
	CBattleRoyalGameMode();
	~CBattleRoyalGameMode();
	BOOL	Apply();					// GameMode ���� ��Ű��
	BOOL	RestrictToUseHealItem();	// "���� ������ ��� ����" ���
	BOOL	OneKill();					// "OneKill" ���
	BOOL	PowerupOneUser();			// "�� ���� �Ŀ� ��" ���
	BOOL	DetermineMode();			// ��� ����
	void	SetGamers(NkVector<CBattleRoyalPersonChangedData>& rclPersonChangedData, SI32	siCount
					 , NkVector<SI32>& rclMobData);
	BOOL	ChangeVisibleMode();		// ������ �����ش�.
	BOOL	ChangeInvisibleMode(cltCharServer* pclChar);		// ������ �����ش�.
	BOOL	ChangeCharBasicInfo();		// ���� �� ������ �ʿ��� �⺻���� ĳ���� ���� ����
	BOOL	RestoreCharInfo(cltCharServer* pclChar);		// ���� ����, ���� ��Ż �� ����� ĳ���� ������ �ǵ������´�.
	BOOL	TransformChar();			// ������ ������ ����
	BOOL	TransformChar(cltCharServer* pclChar);			// Ư�� ������ ������ ����
	BOOL	RestoreTrancsformChar(cltCharServer* pclChar);	// ������ ������ ������ ���� ���·� 
	SI32	GetMode();
	void	SetMode(SI32 siMode);
	void	SetGameTime(SI32 siGameTime);
	BOOL	SetGMGameMode(SI32 siMode);
	BOOL	TakeinHorse(cltCharServer* pclChar);
protected:
	const static int LIMIT_COUNTERCHANGEPOWERUPCHAR = 3;
	SI32 m_siCountChangePowerupChar;
		
	NkVector<CBattleRoyalPersonChangedData>*	m_pclPersonChangedData;	// ����� ���� ���� ����Ʈ
	NkVector<SI32>*	m_pclMobData;	// �� charid
	SI32 m_siPersonDataCount;

	SI32 m_siMode;
	SI32 m_siGameTime;
	SI32 m_siGmGameMode;
	// �� kind��
	SI32 m_siKindBlackarmySword;
	SI32 m_siKindBlackarmyRifle;
	SI32 m_siKindBlackarmyKatar;
};

#endif  //_CBATTLEROYALGAMEMODE_H
