#ifndef _MAGICATTRIBUTE_H_
#define _MAGICATTRIBUTE_H_

#include "../MagicBaseInfo.h"

struct stMagicKindListObj
{
	TCHAR strMagicKindName[ MAX_MAGIC_NAME_LENGTH ];	
	MAGIC_GROUP	siMagicGoup;

	stMagicKindListObj()
	{
		Init();
	}

	void Init()
	{
		memset( this, 0, sizeof( stMagicKindListObj ) );
	}

	void Set( TCHAR *MagicKindName, MAGIC_GROUP group)
	{
		StringCchCopy( strMagicKindName, MAX_MAGIC_NAME_LENGTH, MagicKindName );
		siMagicGoup = group;
	}
};

class	CMagicAttribute
{
public:
	CMagicAttribute();
	~CMagicAttribute();
	
	void					ReadFileData();

	stMagicAttributeData	*GetMagicAttribute( SI32 si32MagicKind);

	BOOL					GetMagicStartEnd(SI32 index, SI32 &Start, SI32 &End);
	SI32					GetMagicType(SI32 index);
	SI32					GetMagicSkillType(SI32 index);
	SI32					GetMagicGroup(SI32 index);
	SI32					GetMagicNum();

	void					AddMagics();
	void					Add( TCHAR *MagicName, SI32 MagicKind , MAGIC_GROUP MagicGroup);

	SI32					FindRefFromKind(TCHAR *MagicKindName);
	SI32					FindRefFromName(TCHAR *MagicName, SI32 MagicType);
	bool					IsMatchAttackTye(SI32 id, SI32 magickind, SI32 attackatb);

	const TCHAR*			GetMagicKindString(SI32 index) const	{ return m_pMagicKindList[index].strMagicKindName;	}
	
private:

#ifdef _SAFE_MEMORY
	NSafeTArray<stMagicAttributeData, MAX_MAGIC_KIND_NUM>		m_stMagicAttributeData;
#else
	stMagicAttributeData	m_stMagicAttributeData[ MAX_MAGIC_KIND_NUM ];
#endif
	SI32					m_si32MagicNum;

	stMagicKindListObj		*m_pMagicKindList;

};

#endif
