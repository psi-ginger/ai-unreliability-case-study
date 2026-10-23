#ifndef _MAGICBASEOBJ_H_
#define _MAGICBASEOBJ_H_

#include "MagicBaseInfo.h"

#include "../../Common/SMLib/LibList/Obj.h"
#include "../../Client/Music/Music.h"

class	CLibList;
class	CMagicMgr;
class	CMagicMsgObj;

class CMagicBaseObj : public CObj
{
public:
	CMagicBaseObj( CMagicMgr *pMagicMgr);
	virtual ~CMagicBaseObj();

	void			Initialize();
	void			Destroy();

	SI32			GetStatus();
	void			SetStatus( SI32 si32Status );

	void			GetMagicAttribute( SI32 si32MagicKind );
	stMagicAttributeData	*GetMagicAttributePtr();

	// cltTSpr			*GetMagicSpr();
	TSpr			*GetMagicSpr();

	void			SetMagicSpr( SI32 gImageIndex );

	SI32			GetCurrentFrame();
	void			SetCurrentFrame( SI32 siCurrentFrame );

	SI32			GetCurrentMagicDelayFrame();

	SI32			GetMaxFrameNum();
	
	SI32			GetSkillLevel();

	stMagicHeader	*GetMagicHeader();
	void			SetKind(SI32 kind);
	SI32			GetKind()	{return m_siMagicKind;}
	virtual	bool	SetMagic( stMagicHeader *pstMagicHeader, SI32 si32SkillLevel);			// Set
	virtual	bool	Action();											// Action
	virtual	void	Draw();												// Draw

	virtual	bool	Push( BYTE *pData, SI32 si32Size );					// Push
	

private:

	SI32					m_si32CurrentFrame;
	SI32					m_si32MaxFrameNum;

	SI32					m_si32CurrentMagicDelayFrame;

	CMagicMgr				*m_pMagicMgr;							// Push, Pop 을 CMagicMgr 에서 관리하는데, 이 포인터를 찾아가기 위해서임.

	SI32					m_si32Status;							// 현재 상태( 마법을 쓰는 중이냐, 쉬는 중이냐, 마법이 끝났냐 등등 )
	stMagicHeader			*m_pMagicHeader;

	stMagicAttributeData	*m_pMagicAttributeData;					// 마법의 기본 속성(txt)을 가지고 있는 데이터

	TSpr					*m_pMagicSpr;							// 마법 이미지 포인터

	SI32					m_si32SkillLevel;
	SI32					m_siMagicKind;
};

#endif
