#include "MagicAttribute.h"

#include "../../../Client/client.h"

#include "Char\CharManager\CharManager.h"

#include "../NLib/NDataTypes.h"
#include "../NLib/NDataLoader.h"

extern cltCommonLogic* pclClient;

CMagicAttribute::CMagicAttribute()
{
	m_si32MagicNum = 0;
#ifdef _SAFE_MEMORY
	m_stMagicAttributeData.ZeroMem();
#else
	memset ( m_stMagicAttributeData, 0, MAX_MAGIC_KIND_NUM * sizeof( stMagicAttributeData ) );
#endif

	m_pMagicKindList = new stMagicKindListObj[MAX_MAGIC_KIND_NUM];

	memset( m_pMagicKindList, 0, sizeof( stMagicKindListObj ) * MAX_MAGIC_KIND_NUM );

	//ReadFileData();
}

CMagicAttribute::~CMagicAttribute()
{
	if( m_pMagicKindList )
	{
		delete [] m_pMagicKindList;
		m_pMagicKindList = NULL;
	}
}

stMagicAttributeData *CMagicAttribute::GetMagicAttribute( SI32 si32MagicKind )
{
	return &m_stMagicAttributeData[ si32MagicKind ];
}

BOOL CMagicAttribute::GetMagicStartEnd(SI32 index, SI32 &Start, SI32 &End)
{
	switch(index)
	{
	case MAGIC_TYPE_SWORD:		Start = MAGIC_SWORD_START;		End = MAGIC_SWORD_LAST; 		break;
	case MAGIC_TYPE_SPEAR:		Start = MAGIC_SPEAR_START;		End = MAGIC_SPEAR_LAST; 		break;
	case MAGIC_TYPE_AXE:		Start = MAGIC_AXE_START;		End = MAGIC_AXE_LAST;			break;
	case MAGIC_TYPE_STAFF:
		if( pclClient->IsCountrySwitch(Switch_HealthAddChange) )	{	Start = NEW_MAGIC_STAFF_START;	End = NEW_MAGIC_STAFF_LAST;		}
		else														{	Start = ORG_MAGIC_STAFF_START;	End = ORG_MAGIC_STAFF_LAST;		}

		break;
	case MAGIC_TYPE_BOW:		Start = MAGIC_BOW_START;		End = MAGIC_BOW_LAST;			break;
	case MAGIC_TYPE_GUN:		Start = MAGIC_GUN_START;		End = MAGIC_GUN_LAST;			break;
	case MAGIC_TYPE_MON:		Start = MAGIC_MON_START;		End = MAGIC_MON_LAST;			break;
	case MAGIC_TYPE_SOULGUARD:	Start = MAGIC_SOULGUARD_START;	End = MAGIC_SOULGUARD_LAST;		break;
	case MAGIC_TYPE_SKILL_BOOK:	Start = MAGIC_SKILL_BOOK_START;	End = MAGIC_SKILL_BOOK_LAST;	break;
		
	default:
		return FALSE;
	}

	return TRUE;
}

SI32 CMagicAttribute::GetMagicType(SI32 index)
{

	// 범위를 벗어나면 ERROR
	if(index < MAGIC_INDEX_START || index > MAGIC_INDEX_LAST) return -1;

	SI32 Start, End;
	Start = MAGIC_SWORD_START;	End = MAGIC_SWORD_LAST;
	if(index >= Start && index <= End)	return MAGIC_TYPE_SWORD;

	Start = MAGIC_SPEAR_START;	End = MAGIC_SPEAR_LAST;
	if(index < Start) return -1;
	if(index >= Start && index <= End)	return MAGIC_TYPE_SPEAR;

	Start = MAGIC_AXE_START;	End = MAGIC_AXE_LAST;
	if(index < Start) return -1;
	if(index >= Start && index <= End)	return MAGIC_TYPE_AXE;

	if( pclClient->IsCountrySwitch(Switch_HealthAddChange) )	{	Start = ORG_MAGIC_STAFF_START;	End = ORG_MAGIC_STAFF_LAST;	}
	else														{	Start = NEW_MAGIC_STAFF_START;	End = NEW_MAGIC_STAFF_LAST;	}

	if(index < Start) return -1;
	if(index >= Start && index <= End)	return MAGIC_TYPE_STAFF;

	Start = MAGIC_BOW_START;	End = MAGIC_BOW_LAST;
	if(index < Start) return -1;
	if(index >= Start && index <= End)	return MAGIC_TYPE_BOW;

	Start = MAGIC_GUN_START;	End = MAGIC_GUN_LAST;
	if(index < Start) return -1;
	if(index >= Start && index <= End)	return MAGIC_TYPE_GUN;

	Start = MAGIC_MON_START;	End = MAGIC_MON_LAST;
	if(index < Start) return -1;
	if(index >= Start && index <= End)	return MAGIC_TYPE_MON;

	Start = MAGIC_SOULGUARD_START;	End = MAGIC_SOULGUARD_LAST;
	if(index < Start) return -1;
	if(index >= Start && index <= End)	return MAGIC_TYPE_SOULGUARD;

	Start = MAGIC_SKILL_BOOK_START;	End = MAGIC_SKILL_BOOK_LAST;
	if(index < Start) return -1;
	if(index >= Start && index <= End)	return MAGIC_TYPE_SKILL_BOOK;

	return -1;
}


SI32 CMagicAttribute::GetMagicSkillType(SI32 index)
{
	SI32 magictype = GetMagicType(index);
	switch(magictype)
	{
	case MAGIC_TYPE_SWORD:		return SKILL_USESWORD1;	break;	
	case MAGIC_TYPE_SPEAR:		return SKILL_USESPEAR1;	break;	
	case MAGIC_TYPE_AXE:		return SKILL_USEAXE1;	break;	
	case MAGIC_TYPE_STAFF:		return SKILL_USESTAFF1;	break;	
	case MAGIC_TYPE_BOW:		return SKILL_USEBOW1;	break;	
	case MAGIC_TYPE_GUN:		return SKILL_USEGUN1;	break;
	case MAGIC_TYPE_MON:		return 0;				break;
	case MAGIC_TYPE_SOULGUARD:	return 0;				break;
	case MAGIC_TYPE_SKILL_BOOK:	return SKILL_SIKLLBOOK;	break;
		
	}

	return -1;
}
SI32 CMagicAttribute::GetMagicGroup(SI32 index)
{
	// 범위를 벗어나면 ERROR
	if(index < MAGIC_INDEX_START || index > MAGIC_INDEX_LAST) return -1;

	return m_pMagicKindList[index].siMagicGoup;
}

SI32 CMagicAttribute::GetMagicNum()
{
	return m_si32MagicNum;
}

void CMagicAttribute::Add( TCHAR *MagicKindName, SI32 MagicKind, MAGIC_GROUP MagicGroup)
{
	m_pMagicKindList[ MagicKind ].Set( MagicKindName, MagicGroup);
}

SI32 CMagicAttribute::FindRefFromName(TCHAR *MagicName, SI32 MagicType)
{
	if(MagicName == NULL)		return -1;
	if(MagicType<MAGIC_TYPE_START || MagicType>MAGIC_TYPE_LAST) return -1;

	SI32 Start, End;
	switch(MagicType)
	{
	case MAGIC_TYPE_SWORD:		Start = MAGIC_SWORD_START;		End = MAGIC_SWORD_LAST; 		break;
	case MAGIC_TYPE_SPEAR:		Start = MAGIC_SPEAR_START;		End = MAGIC_SPEAR_LAST; 		break;
	case MAGIC_TYPE_AXE:		Start = MAGIC_AXE_START;		End = MAGIC_AXE_LAST;			break;
	case MAGIC_TYPE_STAFF:
		if( pclClient->IsCountrySwitch(Switch_HealthAddChange) )	{	Start = NEW_MAGIC_STAFF_START;	End = NEW_MAGIC_STAFF_LAST;		}
		else														{	Start = ORG_MAGIC_STAFF_START;	End = ORG_MAGIC_STAFF_LAST;		}
		break;
	case MAGIC_TYPE_BOW:		Start = MAGIC_BOW_START;		End = MAGIC_BOW_LAST;			break;
	case MAGIC_TYPE_GUN:		Start = MAGIC_GUN_START;		End = MAGIC_GUN_LAST;			break;
	case MAGIC_TYPE_MON:		Start = MAGIC_MON_START;		End = MAGIC_MON_LAST;			break;
	case MAGIC_TYPE_SOULGUARD:	Start = MAGIC_SOULGUARD_START;	End = MAGIC_SOULGUARD_LAST;		break;
	case MAGIC_TYPE_SKILL_BOOK:	Start = MAGIC_SKILL_BOOK_START;	End = MAGIC_SKILL_BOOK_LAST;	break;
		
	default:		return -1;
	}
	for ( SI32 i = Start; i <= End; ++i )
	{
		if ( _tcsncmp( m_stMagicAttributeData[i].strMagicName, MagicName, 64 ) == 0 )
		{
			return i;
		}
	}
	return -1;
}

// 해당 Magic이 ATTACK TYPE와 일치하는지를 검사한다.
bool CMagicAttribute::IsMatchAttackTye(SI32 id, SI32 magickind, SI32 attackatb) 
{
	SI32 magictype = GetMagicType(magickind);

	// NPC는 무기타입에 상관없이 수행한다.
	bool bSuccess = false;
	if(IsPC(pclClient->pclCM->CR[id]->GetCharUnique()))	 bSuccess = false;
	else												 bSuccess = true;	

	// 기술에 해당하는 무기타입을 알아낸다. 
	switch(magictype)
	{
	case MAGIC_TYPE_SWORD:	if(attackatb  == ATTACKTYPE_SWORD)	bSuccess = true;	break;	
	case MAGIC_TYPE_SPEAR:	if(attackatb  == ATTACKTYPE_SPEAR)	bSuccess = true;	break;	
	case MAGIC_TYPE_AXE:	if(attackatb  == ATTACKTYPE_AXE)	bSuccess = true;	break;	
	case MAGIC_TYPE_STAFF:	if(attackatb  == ATTACKTYPE_STAFF	|| attackatb == ATTACKTYPE_STAFF1	|| attackatb == ATTACKTYPE_STAFF2 || attackatb == ATTACKTYPE_NEW_STAFF2) bSuccess = true;	break;
	case MAGIC_TYPE_BOW:	if(attackatb  == ATTACKTYPE_BOW		|| attackatb  == ATTACKTYPE_BOW1	|| attackatb  == ATTACKTYPE_NEW_BOW1)	bSuccess = true;	break;	
	case MAGIC_TYPE_GUN:	if(attackatb  == ATTACKTYPE_GUN		|| attackatb  == ATTACKTYPE_GUN1	|| attackatb  == ATTACKTYPE_NEW_GUN1)	bSuccess = true;	break;
	case MAGIC_TYPE_MON:										bSuccess = true;	break;
	case MAGIC_TYPE_SOULGUARD:									bSuccess = true;	break;
	case MAGIC_TYPE_SKILL_BOOK:									bSuccess = true;	break;
		
	}		

	return bSuccess;

}

SI32 CMagicAttribute::FindRefFromKind( TCHAR *MagicKindName)
{
	for ( SI32 i = 1; i <MAX_MAGIC_KIND_NUM; ++i )
	{
		if ( _tcscmp( m_pMagicKindList[ i ].strMagicKindName, MagicKindName) == 0 )
		{
			return i;
		}
	}

	return -1;
}

// JAYCEE
void CMagicAttribute::ReadFileData()
{
	TCHAR *FileNameList[] = 
	{ 
			TEXT("Magic/Magic_Sword.txt"), 
			TEXT("Magic/Magic_Spear.txt"), 
			TEXT("Magic/Magic_Axe.txt"), 
			TEXT("Magic/Magic_Staff.txt"), 
			TEXT("Magic/Magic_Bow.txt"), 
			TEXT("Magic/Magic_Gun.txt"),
			TEXT("Magic/Magic_Mon.txt"),
			TEXT("Magic/Magic_soulguard.txt"),
			TEXT("Magic/magic_skillbook.txt"),
			TEXT("Magic/magic_skillaidbook.txt")
			
	};

	TCHAR *FileNameListKorea[] = 
	{
			TEXT("Magic/Magic_SwordK.txt"), 
			TEXT("Magic/Magic_SpearK.txt"), 
			TEXT("Magic/Magic_AxeK.txt"), 
			TEXT("Magic/Magic_StaffK.txt"), 
			TEXT("Magic/Magic_BowK.txt"), 
			TEXT("Magic/Magic_GunK.txt"),
			TEXT("Magic/Magic_MonK.txt"),
			TEXT("Magic/Magic_soulguardk.txt"),
			TEXT("Magic/magic_skillbookK.txt"),
			TEXT("Magic/magic_skillaidbookK.txt")
	};

	TCHAR *FileNameListEnglish[] = 
	{ 
			TEXT("Magic/Magic_SwordE.txt"), 
			TEXT("Magic/Magic_SpearE.txt"), 
			TEXT("Magic/Magic_AxeE.txt"), 
			TEXT("Magic/Magic_StaffE.txt"), 
			TEXT("Magic/Magic_BowE.txt"), 
			TEXT("Magic/Magic_GunE.txt"),
			TEXT("Magic/Magic_MonE.txt"),
			TEXT("Magic/Magic_soulguarde.txt"),
			TEXT("Magic/magic_skillbookE.txt"),
			TEXT("Magic/magic_skillaidbookE.txt")
	};

	TCHAR *FileNameListChina[] = 
	{ 
			TEXT("Magic/Magic_SwordC.txt"), 
			TEXT("Magic/Magic_SpearC.txt"), 
			TEXT("Magic/Magic_AxeC.txt"), 
			TEXT("Magic/Magic_StaffC.txt"), 
			TEXT("Magic/Magic_BowC.txt"), 
			TEXT("Magic/Magic_GunC.txt"),
			TEXT("Magic/Magic_MonC.txt"),
			TEXT("Magic/Magic_soulguardC.txt"),
			TEXT("Magic/magic_skillbookC.txt"),
			TEXT("Magic/magic_skillaidbookC.txt")
	};

	TCHAR *FileNameListJapan[] = 
	{ 
			TEXT("Magic/Magic_SwordJ.txt"), 
			TEXT("Magic/Magic_SpearJ.txt"), 
			TEXT("Magic/Magic_AxeJ.txt"), 
			TEXT("Magic/Magic_StaffJ.txt"), 
			TEXT("Magic/Magic_BowJ.txt"), 
			TEXT("Magic/Magic_GunJ.txt"),
			TEXT("Magic/Magic_MonJ.txt"),
			TEXT("Magic/Magic_soulguardJ.txt"),
			TEXT("Magic/magic_skillbookJ.txt"),
			TEXT("Magic/magic_skillaidbookJ.txt")
	};

	TCHAR *FileNameListTaiwan[] = 
	{ 
			TEXT("Magic/Magic_SwordT.txt"), 
			TEXT("Magic/Magic_SpearT.txt"), 
			TEXT("Magic/Magic_AxeT.txt"), 
			TEXT("Magic/Magic_StaffT.txt"), 
			TEXT("Magic/Magic_BowT.txt"), 
			TEXT("Magic/Magic_GunT.txt"),
			TEXT("Magic/Magic_MonT.txt"),
			TEXT("Magic/Magic_soulguardT.txt"),
			TEXT("Magic/magic_skillbookT.txt"),
			TEXT("Magic/magic_skillaidbookT.txt")
	};

	TCHAR *FileNameListUSA[] = 
	{ 
			TEXT("Magic/Magic_SwordU.txt"), 
			TEXT("Magic/Magic_SpearU.txt"), 
			TEXT("Magic/Magic_AxeU.txt"), 
			TEXT("Magic/Magic_StaffU.txt"), 
			TEXT("Magic/Magic_BowU.txt"), 
			TEXT("Magic/Magic_GunU.txt"),
			TEXT("Magic/Magic_MonU.txt"),
			TEXT("Magic/Magic_soulguardU.txt"),
			TEXT("Magic/magic_skillbookU.txt"),
			TEXT("Magic/magic_skillaidbookU.txt")
	};

	TCHAR *FileNameListNHNChina[] = 
	{ 
			TEXT("Magic/Magic_SwordC.txt"), 
			TEXT("Magic/Magic_SpearC.txt"), 
			TEXT("Magic/Magic_AxeC.txt"), 
			TEXT("Magic/Magic_StaffC.txt"), 
			TEXT("Magic/Magic_BowC.txt"), 
			TEXT("Magic/Magic_GunC.txt"),
			TEXT("Magic/Magic_MonC.txt"),
			TEXT("Magic/Magic_soulguardC.txt"),
			TEXT("Magic/magic_skillbookC.txt"),
			TEXT("Magic/magic_skillaidbookC.txt")
	};

	TCHAR *FileNameListEUROPE[] = 
	{ 
			TEXT("Magic/Magic_SwordG.txt"), 
			TEXT("Magic/Magic_SpearG.txt"), 
			TEXT("Magic/Magic_AxeG.txt"), 
			TEXT("Magic/Magic_StaffG.txt"), 
			TEXT("Magic/Magic_BowG.txt"), 
			TEXT("Magic/Magic_GunG.txt"),
			TEXT("Magic/Magic_MonG.txt"),
			TEXT("Magic/Magic_soulguardG.txt"),
			TEXT("Magic/magic_skillbookG.txt"),
			TEXT("Magic/magic_skillaidbookG.txt")
	};


	BOOL bLoadFile = FALSE;
	NDataLoader	dataloader;

	for(int i = 0; i < 10; ++i)
	{
		//KHY - 0810 - 텍스트 리소스 암호화.
		if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
		{
			switch (pclClient->siServiceArea)
			{
			case ConstServiceArea_Korea :
				bLoadFile = dataloader.LoadDataFromCryptFile( FileNameListKorea[i] );
				break;
			case ConstServiceArea_China :
				bLoadFile = dataloader.LoadDataFromCryptFile( FileNameListChina[i] );
				break;
			case ConstServiceArea_Japan : 
				bLoadFile = dataloader.LoadDataFromCryptFile( FileNameListJapan[i] );
				break;
			case ConstServiceArea_English : 
				bLoadFile = dataloader.LoadDataFromCryptFile( FileNameListEnglish[i] );
				break;
			case ConstServiceArea_Taiwan : 
				bLoadFile = dataloader.LoadDataFromCryptFile( FileNameListTaiwan[i] );
				break;
			case ConstServiceArea_USA  : 
				bLoadFile = dataloader.LoadDataFromCryptFile( FileNameListUSA[i] );
				break;
			case ConstServiceArea_NHNChina : 
				bLoadFile = dataloader.LoadDataFromCryptFile( FileNameListNHNChina[i] );
				break;
			case ConstServiceArea_EUROPE : 
				bLoadFile = dataloader.LoadDataFromCryptFile( FileNameListEUROPE[i] );
				break;
			default:
				bLoadFile = dataloader.LoadDataFromCryptFile( FileNameList[i] );
				break;
			}

		}
		else
		{
			switch (pclClient->siServiceArea)
			{
			case ConstServiceArea_Korea :
				bLoadFile = dataloader.LoadDataFromFile( FileNameListKorea[i] );
				break;
			case ConstServiceArea_China :
				bLoadFile = dataloader.LoadDataFromFile( FileNameListChina[i] );
				break;
			case ConstServiceArea_Japan : 
				bLoadFile = dataloader.LoadDataFromFile( FileNameListJapan[i] );
				break;
			case ConstServiceArea_English : 
				bLoadFile = dataloader.LoadDataFromFile( FileNameListEnglish[i] );
				break;
			case ConstServiceArea_Taiwan : 
				bLoadFile = dataloader.LoadDataFromFile( FileNameListTaiwan[i] );
				break;
			case ConstServiceArea_USA  : 
				bLoadFile = dataloader.LoadDataFromFile( FileNameListUSA[i] );
				break;
			case ConstServiceArea_NHNChina : 
				bLoadFile = dataloader.LoadDataFromFile( FileNameListNHNChina[i] );
				break;
			case ConstServiceArea_EUROPE : 
				bLoadFile = dataloader.LoadDataFromFile( FileNameListEUROPE[i] );
				break;

			default:
				bLoadFile = dataloader.LoadDataFromFile( FileNameList[i] );
				break;
			}
		}	

		if (!bLoadFile)
		{
			if (pclClient->GameMode != GAMEMODE_SERVER)
				MessageBox(NULL, TEXT("MagicData Load Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
			return;
		}

		TCHAR* pszAttribute[MAGIC_ATTRIBUTE_NUM] = 
		{ 
			TEXT("MAGIC_ATTRIBUTE_NONE"), 
			TEXT("MAGIC_ATTRIBUTE_FIRE"), 
			TEXT("MAGIC_ATTRIBUTE_WATER"), 
			TEXT("MAGIC_ATTRIBUTE_EARTH"), 
			TEXT("MAGIC_ATTRIBUTE_WIND"), 
		};

		TCHAR* pszAddMagic[ADDMAGIC_NUM] = 
		{ 
			TEXT("ADDMAGIC_NONE"), 
			TEXT("ADDMAGIC_CURSE_POISON"), 
			TEXT("ADDMAGIC_CURSE_ATTACK"), 
			TEXT("ADDMAGIC_CURSE_DEFENSE"), 
			TEXT("ADDMAGIC_CURSE_DODGE"), 
			TEXT("ADDMAGIC_CURSE_HIT"), 
			TEXT("ADDMAGIC_CURSE_ICE"), 
			
		};

		TCHAR	MagicKindName[ MAX_MAGIC_NAME_LENGTH ]	= TEXT("");
		TCHAR	MagicName[ MAX_MAGIC_NAME_LENGTH ]		= TEXT("");
		SI32 	MagicUserUsing;
		SI32 	MagicCastType;
		SI32 	MagicDelay;
		SI32 	MagicSpendMana;
		SI32 	MagicYIndent;
		SI32 	MagicMaxDrawFrame;
		SI32 	MagicDirection;
		SI32 	MagicMinDamage;
		SI32 	MagicMaxDamage;
		SI32 	MagicRadius;
		SI32 	MagicDistance;
		SI32 	MagicHitIDExist;
		SI32 	MagicMaxActionFrame;
		SI32 	MagicNextFrameDelay;
		SI32 	MagicSkillSpendMana;
		SI32 	MagicSkillIncreaseDamage;
		SI32 	MagicSkillLevel;
		TCHAR	MagicOrganKeyword[ MAX_MAGIC_NAME_LENGTH ] =TEXT("");
		SI32	MagicOrganDamage;
		SI32	MagicExplainNum;
		TCHAR	szAttribute[ 32 ] = TEXT("");
		TCHAR	szAddMagic[ 32 ]  = TEXT("");
		SI32	siDuration;
		SI32	siValue;

		NDataTypeInfo pDataTypeInfo[] =
		{
				NDATA_MBSTRING,	MagicKindName,				64,
				NDATA_MBSTRING,	MagicName,					64,
				NDATA_INT32,	&MagicUserUsing,			4,
				NDATA_INT32,	&MagicCastType,				4,
				NDATA_INT32,	&MagicDelay,				4,
				NDATA_INT32,	&MagicSpendMana,			4,
				NDATA_INT32,	&MagicYIndent,				4,
				NDATA_INT32,	&MagicMaxDrawFrame,			4,
				NDATA_INT32,	&MagicDirection,			4,
				NDATA_INT32,	&MagicMinDamage,			4,
				NDATA_INT32,	&MagicMaxDamage,			4,
				NDATA_INT32,	&MagicRadius,				4,
				NDATA_INT32,	&MagicDistance,				4,
				NDATA_INT32,	&MagicHitIDExist,			4,
				NDATA_INT32,	&MagicMaxActionFrame,		4,
				NDATA_INT32,	&MagicNextFrameDelay,		4,
				NDATA_INT32,	&MagicSkillSpendMana,		4,
				NDATA_INT32,	&MagicSkillIncreaseDamage,	4,
				NDATA_INT32,	&MagicSkillLevel,			4,
				NDATA_MBSTRING,	MagicOrganKeyword,			64,
				NDATA_INT32,	&MagicOrganDamage,			4,
				NDATA_INT32,	&MagicExplainNum,			4,
				NDATA_MBSTRING,	&szAttribute,				32,
				NDATA_MBSTRING,	&szAddMagic,				32,
				NDATA_INT32,	&siDuration,				4,
				NDATA_INT32,	&siValue,					4,
				0,0,0
		};
		dataloader.RegDataType( pDataTypeInfo );

		while( !dataloader.IsEndOfData() ) 
		{
			if (dataloader.ReadData())	
			{
				if (MagicKindName[0] != NULL)
				{
					SI32 ListIndex = FindRefFromKind( MagicKindName );

					// List 에 0 index는 사용하지 않는다.
					if ( ListIndex <= 0 )	continue;

					StringCchCopy( m_stMagicAttributeData[ ListIndex ].strMagicKindName, MAX_MAGIC_NAME_LENGTH, MagicKindName );
					StringCchCopy( m_stMagicAttributeData[ ListIndex ].strMagicName, MAX_MAGIC_NAME_LENGTH, MagicName );
					m_stMagicAttributeData[ ListIndex ].si16MagicUserUsing				= MagicUserUsing;
					m_stMagicAttributeData[ ListIndex ].si32MagicDelay					= MagicDelay;
					m_stMagicAttributeData[ ListIndex ].si32MagicSpendMana				= MagicSpendMana;
					m_stMagicAttributeData[ ListIndex ].si32MagicYIndent				= MagicYIndent;
					m_stMagicAttributeData[ ListIndex ].si32MagicMaxDrawFrame			= MagicMaxDrawFrame;
					m_stMagicAttributeData[ ListIndex ].si32MagicDirection				= MagicDirection;
					m_stMagicAttributeData[ ListIndex ].si32MagicMinDamage				= MagicMinDamage;
					m_stMagicAttributeData[ ListIndex ].si32MagicMaxDamage				= MagicMaxDamage;
					m_stMagicAttributeData[ ListIndex ].si32MagicRadius					= MagicRadius;
					m_stMagicAttributeData[ ListIndex ].si32MagicDistance				= MagicDistance;
					m_stMagicAttributeData[ ListIndex ].si32MagicHitIDExist				= MagicHitIDExist;
					m_stMagicAttributeData[ ListIndex ].si32MagicMaxActionFrame			= MagicMaxActionFrame;
					m_stMagicAttributeData[ ListIndex ].si32MagicNextFrameDelay			= MagicNextFrameDelay;
					m_stMagicAttributeData[ ListIndex ].si16MagicSkillSpendMana			= MagicSkillSpendMana;
					m_stMagicAttributeData[ ListIndex ].si16MagicSkillIncreaseDamage	= MagicSkillIncreaseDamage;
					m_stMagicAttributeData[ ListIndex ].si16MagicSkillLevel				= MagicSkillLevel;
					m_stMagicAttributeData[ ListIndex ].si16MagicOrganDamage			= MagicOrganDamage;
					m_stMagicAttributeData[ ListIndex ].si08MagicCastType				= MagicCastType;
					m_stMagicAttributeData[ ListIndex ].si32MagicExplainNum				= MagicExplainNum;

					m_stMagicAttributeData[ ListIndex ].siAttribute = MAGIC_ATTRIBUTE_NONE;
					for( SI32 iCount = MAGIC_ATTRIBUTE_FIRE; iCount < MAGIC_ATTRIBUTE_NUM; ++iCount )
					{
						if( 0 == _tcscmp( szAttribute, pszAttribute[iCount] ) )
						{
							m_stMagicAttributeData[ ListIndex ].siAttribute = iCount;
							break;
						}
					}

					m_stMagicAttributeData[ ListIndex ].siAddMagic = ADDMAGIC_NONE;
					for( SI32 iCount = ADDMAGIC_CURSE_POISON; iCount < ADDMAGIC_NUM; ++iCount )
					{
						if( 0 == _tcscmp( szAddMagic, pszAddMagic[iCount] ) )
						{
							m_stMagicAttributeData[ ListIndex ].siAddMagic = iCount;
							break;
						}
					}

					m_stMagicAttributeData[ ListIndex ].dwAddMagicDuration	= siDuration * 50;
					m_stMagicAttributeData[ ListIndex ].siAddMagicValue		= siValue;
					

					// Organ 을 설정한다.
					SI16 OrganType = 0;
					TCHAR OrganFullName[ MAX_MAGIC_NAME_LENGTH ] =TEXT("");
					TCHAR *pOrganName = MagicOrganKeyword;

					TCHAR *pText;
					if(     _tcscmp(pOrganName, TEXT("ORG1"))==0)	
					{
						pText = GetTxtFromMgr(5194);
						OrganType = HEALTH_ORGAN_TYPE_ORG1;
						//StringCchCopy(OrganFullName, MAX_MAGIC_NAME_LENGTH, TEXT("소화기관"));	
						StringCchCopy(OrganFullName, MAX_MAGIC_NAME_LENGTH, pText);	
					}
					else if(_tcscmp(pOrganName, TEXT("ORG2"))==0)	
					{
						pText = GetTxtFromMgr(5195);
						OrganType = HEALTH_ORGAN_TYPE_ORG2;
						//StringCchCopy(OrganFullName, MAX_MAGIC_NAME_LENGTH, TEXT("순환기관"));	
						StringCchCopy(OrganFullName, MAX_MAGIC_NAME_LENGTH, pText);	
					}	
					else if(_tcscmp(pOrganName, TEXT("ORG3"))==0)	
					{
						pText = GetTxtFromMgr(5196);
						OrganType = HEALTH_ORGAN_TYPE_ORG3;
						//StringCchCopy(OrganFullName, MAX_MAGIC_NAME_LENGTH, TEXT("호흡기관"));	
						StringCchCopy(OrganFullName, MAX_MAGIC_NAME_LENGTH, pText);	
					}	
					else if(_tcscmp(pOrganName, TEXT("ORG4"))==0)	
					{
						pText = GetTxtFromMgr(5197);
						OrganType = HEALTH_ORGAN_TYPE_ORG4;
						//StringCchCopy(OrganFullName, MAX_MAGIC_NAME_LENGTH, TEXT("신경기관"));	
						StringCchCopy(OrganFullName, MAX_MAGIC_NAME_LENGTH, pText);	
					}	
					else
					{
						if (pclClient->GameMode != GAMEMODE_SERVER)
							MessageBox(NULL, FileNameList[i], pOrganName, MB_OK|MB_TOPMOST);
					}

					StringCchCopy(m_stMagicAttributeData[ ListIndex ].strMagicOrganName, MAX_MAGIC_NAME_LENGTH,OrganFullName);
					m_stMagicAttributeData[ ListIndex ].si16MagicOrganKind = OrganType;

					++m_si32MagicNum;
				}
			}
		}
	}

	return;
}

// 게임에서 사용하면 모든 마법을 등록한다. JAYCEE
void CMagicAttribute::AddMagics()
{
	// SWORD TYPE
	Add( TEXT("MAGIC_SWORD_STRIKING"),		MAGIC_SWORD_STRIKING,		MAGIC_GROUP_BLESSDAMAGE);
	Add( TEXT("MAGIC_SWORD_AURABLADE"),		MAGIC_SWORD_AURABLADE,		MAGIC_GROUP_AURABLADE);
	Add( TEXT("MAGIC_SWORD_POISON"),		MAGIC_SWORD_POISON,			MAGIC_GROUP_POISON);
	Add( TEXT("MAGIC_SWORD_SWORDBOMB"),		MAGIC_SWORD_SWORDBOMB,		MAGIC_GROUP_SWORDBOMB);

	// SPEAR TYPE
	Add( TEXT("MAGIC_SPEAR_DEATHBLOW"),		MAGIC_SPEAR_DEATHBLOW,		MAGIC_GROUP_BLESSCIRICAL);
	Add( TEXT("MAGIC_SPEAR_POISON"),		MAGIC_SPEAR_POISON,			MAGIC_GROUP_POISON);
	Add( TEXT("MAGIC_SPEAR_CURSECRITICAL"),	MAGIC_SPEAR_CURSECRITICAL,	MAGIC_GROUP_CURSECRITICAL);
	Add( TEXT("MAGIC_SPEAR_UPPERDRAGON"),	MAGIC_SPEAR_UPPERDRAGON,	MAGIC_GROUP_UPPERDRGON);

	// BOW TYPE
	Add( TEXT("MAGIC_BOW_HITPOINT"),		MAGIC_BOW_HITPOINT,			MAGIC_GROUP_BLESSHITRATE);
	Add( TEXT("MAGIC_BOW_TRIPLEDEATH"),		MAGIC_BOW_TRIPLEDEATH,		MAGIC_GROUP_TRIPLEDEATH);
	Add( TEXT("MAGIC_BOW_ICE"),				MAGIC_BOW_ICE,				MAGIC_GROUP_ICE);
	Add( TEXT("MAGIC_BOW_TRIMETEOR"),		MAGIC_BOW_TRIMETEOR,		MAGIC_GROUP_TRIMETEOR);

	// AXE TYPE
	Add( TEXT("MAGIC_AXE_HITPOINT"),		MAGIC_AXE_HITPOINT,			MAGIC_GROUP_BLESSHITRATE);
	Add( TEXT("MAGIC_AXE_POISON"),			MAGIC_AXE_POISON,			MAGIC_GROUP_POISON);
	Add( TEXT("MAGIC_AXE_DRAIN"),			MAGIC_AXE_DRAIN,			MAGIC_GROUP_DRAIN);
	Add( TEXT("MAGIC_AXE_METEOR"),			MAGIC_AXE_METEOR,			MAGIC_GROUP_METEOR);

	// GUN TYPE
	Add( TEXT("MAGIC_GUN_WIDEVISION"),		MAGIC_GUN_WIDEVISION,		MAGIC_GROUP_BLESSRANGE);
	Add( TEXT("MAGIC_GUN_CANNON"),			MAGIC_GUN_CANNON,			MAGIC_GROUP_BLESSATTACKINTERVAL);
	Add( TEXT("MAGIC_GUN_ICE"),				MAGIC_GUN_ICE,				MAGIC_GROUP_ICE);
	Add( TEXT("MAGIC_GUN_PALMWIND"),		MAGIC_GUN_PALMWIND,			MAGIC_GROUP_PALMWIND);

	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		// STAFF TYPE
		Add( TEXT("MAGIC_STAFF_THUNDER"),		NEW_MAGIC_STAFF_THUNDER,		MAGIC_GROUP_THUNDER);
		Add( TEXT("MAGIC_STAFF_FIREBALL"),		NEW_MAGIC_STAFF_FIREBALL,		MAGIC_GROUP_FIREBALL);
		Add( TEXT("MAGIC_STAFF_FLAME"),			NEW_MAGIC_STAFF_FLAME,			MAGIC_GROUP_FLAME);
		Add( TEXT("MAGIC_STAFF_BIGWIND"),		NEW_MAGIC_STAFF_BIGWIND,		MAGIC_GROUP_BIGWIND);
		Add( TEXT("MAGIC_STAFF_WHIRLPOOL"),		NEW_MAGIC_STAFF_WHIRLPOOL,		MAGIC_GROUP_WHIRLPOOL);
		Add( TEXT("MAGIC_STAFF_POISON"),		NEW_MAGIC_STAFF_POISON,			MAGIC_GROUP_POISON);
		Add( TEXT("MAGIC_STAFF_ICE"),			NEW_MAGIC_STAFF_ICE,			MAGIC_GROUP_ICE);
		Add( TEXT("MAGIC_STAFF_QUICKMANA"),		NEW_MAGIC_STAFF_QUICKMANA,		MAGIC_GROUP_BLESSMPRECOVERYTIME);
		Add( TEXT("MAGIC_STAFF_GAIN"),			NEW_MAGIC_STAFF_GAIN,			MAGIC_GROUP_BLESSHP);
	}
	else
	{
		// STAFF TYPE
		Add( TEXT("MAGIC_STAFF_THUNDER"),		ORG_MAGIC_STAFF_THUNDER,		MAGIC_GROUP_THUNDER);
		Add( TEXT("MAGIC_STAFF_FIREBALL"),		ORG_MAGIC_STAFF_FIREBALL,		MAGIC_GROUP_FIREBALL);
		Add( TEXT("MAGIC_STAFF_FLAME"),			ORG_MAGIC_STAFF_FLAME,			MAGIC_GROUP_FLAME);
		Add( TEXT("MAGIC_STAFF_BIGWIND"),		ORG_MAGIC_STAFF_BIGWIND,		MAGIC_GROUP_BIGWIND);
		Add( TEXT("MAGIC_STAFF_WHIRLPOOL"),		ORG_MAGIC_STAFF_WHIRLPOOL,		MAGIC_GROUP_WHIRLPOOL);
		Add( TEXT("MAGIC_STAFF_POISON"),		ORG_MAGIC_STAFF_POISON,			MAGIC_GROUP_POISON);
		Add( TEXT("MAGIC_STAFF_ICE"),			ORG_MAGIC_STAFF_ICE,			MAGIC_GROUP_ICE);
		Add( TEXT("MAGIC_STAFF_QUICKMANA"),		ORG_MAGIC_STAFF_QUICKMANA,		MAGIC_GROUP_BLESSMPRECOVERYTIME);
		Add( TEXT("MAGIC_STAFF_GAIN"),			ORG_MAGIC_STAFF_GAIN,			MAGIC_GROUP_BLESSHP);
	}

	//MAGIC_TYPE_MON
	Add( TEXT("MAGIC_MON_FIREBALL"),		MAGIC_MON_FIREBALL,			MAGIC_GROUP_FIREBALL);
	Add( TEXT("MAGIC_MON_BIGWIND"),			MAGIC_MON_BIGWIND,			MAGIC_GROUP_BIGWIND);
	Add( TEXT("MAGIC_MON_LIGHTINGFIST"),	MAGIC_MON_LIGHTINGFIST,		MAGIC_GROUP_LIGHTINGFIST);
	Add( TEXT("MAGIC_MON_THUNDER"),			MAGIC_MON_THUNDER,			MAGIC_GROUP_THUNDER);
	Add( TEXT("MAGIC_MON_THUNDER1"),		MAGIC_MON_THUNDER1,			MAGIC_GROUP_THUNDER1);
	Add( TEXT("MAGIC_MON_WHIRLPOOL"),		MAGIC_MON_WHIRLPOOL,		MAGIC_GROUP_WHIRLPOOL);
	Add( TEXT("MAGIC_MON_FLAME"),			MAGIC_MON_FLAME,			MAGIC_GROUP_FLAME);
	Add( TEXT("MAGIC_MON_POISONFLAME"),		MAGIC_MON_POISONFLAME,		MAGIC_GROUP_POISONFLAME);
	Add( TEXT("MAGIC_MON_ROCKSLIDE"),		MAGIC_MON_ROCKSLIDE,		MAGIC_GROUP_ROCKSLIDE);
	Add( TEXT("MAGIC_MON_HEAL"),			MAGIC_MON_HEAL,				MAGIC_GROUP_HEAL);
	Add( TEXT("MAGIC_MON_POISON"),			MAGIC_MON_POISON,			MAGIC_GROUP_POISON);
	Add( TEXT("MAGIC_MON_ICE"),				MAGIC_MON_ICE,				MAGIC_GROUP_ICE);
	Add( TEXT("MAGIC_MON_METEOR"),			MAGIC_MON_METEOR,			MAGIC_GROUP_METEOR);
	Add( TEXT("MAGIC_MON_DRAIN"),			MAGIC_MON_DRAIN,			MAGIC_GROUP_DRAIN);
	Add( TEXT("MAGIC_MON_DROPMONEY"),		MAGIC_MON_DROPMONEY,		MAGIC_GROUP_DROPMONEY);
	Add( TEXT("MAGIC_MON_BLACKINK"),		MAGIC_MON_BLACKINK,			MAGIC_GROUP_BLACKINK);
	Add( TEXT("MAGIC_MON_DROPSNOWMAN"),		MAGIC_MON_DROPSNOWMAN,		MAGIC_GROUP_DROPSNOWMAN);
	Add( TEXT("MAGIC_MON_DROPFORK"),		MAGIC_MON_DROPFORK,			MAGIC_GROUP_DROPFORK);
	Add( TEXT("MAGIC_MON_CURSEATTACK"),		MAGIC_MON_CURSEATTACK,		MAGIC_GROUP_CURSEATTACK);
	Add( TEXT("MAGIC_MON_CURSECRITICAL"),	MAGIC_MON_CURSECRITICAL,	MAGIC_GROUP_CURSECRITICAL);
	Add( TEXT("MAGIC_MON_CURSEDEFENSE"),	MAGIC_MON_CURSEDEFENSE,		MAGIC_GROUP_CURSEDEFENSE);
	Add( TEXT("MAGIC_MON_CURSEMOVESPEED"),	MAGIC_MON_CURSEMOVESPEED,	MAGIC_GROUP_CURSEMOVESPEED);
	Add( TEXT("MAGIC_MON_THROWKNIFE"),		MAGIC_MON_THROWKNIFE,		MAGIC_GROUP_THROWKNIFE);

	// 임시 
	Add( TEXT("MAGIC_MON_LASERBEAM"),		MAGIC_MON_LASERBEAM,		MAGIC_GROUP_LASERBEAM);
	Add( TEXT("MAGIC_MON_TRIMETEOR"),		MAGIC_MON_TRIMETEOR,		MAGIC_GROUP_TRIMETEOR);
	Add( TEXT("MAGIC_MON_FIREGROUND"),		MAGIC_MON_FIREGROUND,		MAGIC_GROUP_FIREGOUND);
	Add( TEXT("MAGIC_MON_SWORDBOMB"),		MAGIC_MON_SWORDBOMB,		MAGIC_GROUP_SWORDBOMB);
	Add( TEXT("MAGIC_MON_FREEZEEXPLOSION"),	MAGIC_MON_FREEZEEXPLOSION,	MAGIC_GROUP_FREEZEEXPLOSION);

	Add( TEXT("MAGIC_MON_UPPERDRAGON"),		MAGIC_MON_UPPERDRAGON,		MAGIC_GROUP_UPPERDRGON);
	Add( TEXT("MAGIC_MON_PALMWIND"),		MAGIC_MON_PALMWIND,			MAGIC_GROUP_PALMWIND);
	Add( TEXT("MAGIC_MON_SUMMONDRAGON"),	MAGIC_MON_SUMMONDRAGON,		MAGIC_GROUP_SUMMONDRAGON);

	Add( TEXT("MAGIC_MON_FIREBURN"),		MAGIC_MON_FIREBURN,			MAGIC_GROUP_FIREBURN);

	Add( TEXT("MAGIC_MON_SKELLGHOST"),		MAGIC_MON_SKELLGHOST,		MAGIC_GROUP_SKELLGHOST);
	Add( TEXT("MAGIC_MON_SWORDMETEOR"),		MAGIC_MON_SWORDMETEOR,		MAGIC_GROUP_SWORDMETEOR);

	Add( TEXT("MAGIC_MON_FROZEN"),			MAGIC_MON_FROZEN,			MAGIC_GROUP_FROZEN);

	Add( TEXT("MAGIC_MON_TWINSPEAR"),		MAGIC_MON_TWINSPEAR,		MAGIC_GROUP_TWINSPEAR);
	Add( TEXT("MAGIC_MON_FIREWHIRLWIND"),	MAGIC_MON_FIREWHIRLWIND,	MAGIC_GROUP_FIREWHIRLWIND);
	Add( TEXT("MAGIC_MON_NORTHWIND"),		MAGIC_MON_NORTHWIND,		MAGIC_GROUP_NORTHWIND);

	Add( TEXT("MAGIC_MON_FIRESTORM"),		MAGIC_MON_FIRESTORM,		MAGIC_GROUP_FIRESTORM);
	Add( TEXT("MAGIC_MON_EARTHQUAKE"),		MAGIC_MON_EARTHQUAKE,		MAGIC_GROUP_EARTHQUAKE);
	Add( TEXT("MAGIC_MON_SPEAR_DRAGON"),	MAGIC_MON_SPEAR_DRAGON,		MAGIC_GROUP_SPEAR_DRAGON);

	Add( TEXT("MAGIC_MON_GARGOYLE"),		MAGIC_MON_GARGOYLE,			MAGIC_GROUP_GARGOYLE);
	Add( TEXT("MAGIC_MON_CERBERUS"),		MAGIC_MON_CERBERUS,			MAGIC_GROUP_CERBERUS);

	Add( TEXT("MAGIC_MON_FROZENSORCERER"),		MAGIC_MON_FROZENSORCERER,		MAGIC_GROUP_FROZENSORCERER );
	Add( TEXT("MAGIC_MON_MARIONETTE"),			MAGIC_MON_MARIONETTE,			MAGIC_GROUP_MARIONETTE );
	Add( TEXT("MAGIC_MON_FROKEN"),				MAGIC_MON_FROKEN,				MAGIC_GROUP_FROKEN );
	Add( TEXT("MAGIC_MON_ICEHOUND"),			MAGIC_MON_ICEHOUND,				MAGIC_GROUP_ICEHOUND );
	Add( TEXT("MAGIC_MON_FROZENGOLEM"),			MAGIC_MON_FROZENGOLEM,			MAGIC_GROUP_FROZENGOLEM );
	Add( TEXT("MAGIC_MON_FROZENKNIGHT"),		MAGIC_MON_FROZENKNIGHT,			MAGIC_GROUP_FROZENKNIGHT );
	Add( TEXT("MAGIC_MON_FROZENQUEEN"),			MAGIC_MON_FROZENQUEEN,			MAGIC_GROUP_FROZENQUEEN );
	Add( TEXT("MAGIC_MON_GOTPA"),				MAGIC_MON_GOTPA,				MAGIC_GROUP_GOTPA );
	Add( TEXT("MAGIC_MON_NINJAPANDA"),			MAGIC_MON_NINJAPANDA,			MAGIC_GROUP_NINJAPANDA );
	Add( TEXT("MAGIC_MON_RACCOONDOG"),			MAGIC_MON_RACCOONDOG,			MAGIC_GROUP_RACCOONDOG );
	Add( TEXT("MAGIC_MON_BOSSMONKEY"),			MAGIC_MON_BOSSMONKEY,			MAGIC_GROUP_BOSSMONKEY );
	Add( TEXT("MAGIC_MON_TOADPOISON"),			MAGIC_MON_TOADPOISON,			MAGIC_GROUP_TOADPOISON );
	Add( TEXT("MAGIC_MON_BLOODBAT"),			MAGIC_MON_BLOODBAT,				MAGIC_GROUP_BLOODBAT );
	Add( TEXT("MAGIC_MON_WEREWOLF"),			MAGIC_MON_WEREWOLF,				MAGIC_GROUP_WEREWOLF );
	Add( TEXT("MAGIC_MON_PRIEST"),				MAGIC_MON_PRIEST,				MAGIC_GROUP_PRIEST );
	Add( TEXT("MAGIC_MON_DEVILOUS"),			MAGIC_MON_DEVILOUS,				MAGIC_GROUP_DEVILOUS );
	Add( TEXT("MAGIC_MON_GANGSI"),				MAGIC_MON_GANGSI,				MAGIC_GROUP_GANGSI );
	Add( TEXT("MAGIC_MON_DRACULA"),				MAGIC_MON_DRACULA,				MAGIC_GROUP_DRACULA );
	Add( TEXT("MAGIC_MON_HARPY"),				MAGIC_MON_HARPY,				MAGIC_GROUP_HARPY );
	Add( TEXT("MAGIC_MON_MANDRAGORA"),			MAGIC_MON_MANDRAGORA,			MAGIC_GROUP_MANDRAGORA );
	Add( TEXT("MAGIC_MON_GIRAFFE"),				MAGIC_MON_GIRAFFE,				MAGIC_GROUP_GIRAFFE );
	Add( TEXT("MAGIC_MON_PAPAGOBLIN"),			MAGIC_MON_PAPAGOBLIN,			MAGIC_GROUP_PAPAGOBLIN );
	Add( TEXT("MAGIC_MON_PIRATESOLDIER"),		MAGIC_MON_PIRATESOLDIER,		MAGIC_GROUP_PIRATESOLDIER );
	Add( TEXT("MAGIC_MON_PIRATEARCHER"),		MAGIC_MON_PIRATEARCHER,			MAGIC_GROUP_PIRATEARCHER );
	Add( TEXT("MAGIC_MON_BLACKBOSS"),	    	MAGIC_MON_BLACKBOSS,			MAGIC_GROUP_BLACKBOSS );
	Add( TEXT("MAGIC_MON_EMPERORCAVALRY"),		MAGIC_MON_EMPERORCAVALRY,		MAGIC_GROUP_EMPERORCAVALRY );
	Add( TEXT("MAGIC_MON_EMPERORMASTER"),		MAGIC_MON_EMPERORMASTER,    	MAGIC_GROUP_EMPERORMASTER );
	Add( TEXT("MAGIC_MON_GREATWALLSOLDIER"),	MAGIC_MON_GREATWALLSOLDIER, 	MAGIC_GROUP_GREATWALLSOLDIER );
	Add( TEXT("MAGIC_MON_GREATWALLMAGICIAN"),	MAGIC_MON_GREATWALLMAGICIAN,    MAGIC_GROUP_GREATWALLMAGICIAN );
	Add( TEXT("MAGIC_MON_EMPERORWARRIOR"),		MAGIC_MON_EMPERORWARRIOR,   	MAGIC_GROUP_EMPERORWARRIOR );
	Add( TEXT("MAGIC_MON_GREATWALLSPIDER"),		MAGIC_MON_GREATWALLSPIDER,  	MAGIC_GROUP_GREATWALLSPIDER );
	Add( TEXT("MAGIC_MON_GREATWALLTWINHEAD"),	MAGIC_MON_GREATWALLTWINHEAD,    MAGIC_GROUP_GREATWALLTWINHEAD );
	Add( TEXT("MAGIC_MON_BEARTANG"),			MAGIC_MON_BEARTANG,				MAGIC_GROUP_BEARTANG );
	Add( TEXT("MAGIC_MON_SUPERBEARTANG"),		MAGIC_MON_SUPERBEARTANG,    	MAGIC_GROUP_SUPERBEARTANG );
	Add( TEXT("MAGIC_MON_SFROZENGOLEM"),		MAGIC_MON_SFROZENGOLEM,			MAGIC_GROUP_SFROZENGOLEM );
	Add( TEXT("MAGIC_MON_SFROZENQUEEN"),		MAGIC_MON_SFROZENQUEEN,			MAGIC_GROUP_SFROZENQUEEN );
	Add( TEXT("MAGIC_MON_CATPET"),				MAGIC_MON_CATPET,				MAGIC_GROUP_CATPET );
	Add( TEXT("MAGIC_MON_COSMOSRABBIT"),		MAGIC_MON_COSMOSRABBIT,			MAGIC_GROUP_COSMOSRABBIT );
	Add( TEXT("MAGIC_MON_RUDOLPHPET"),			MAGIC_MON_RUDOLPHPET,			MAGIC_GROUP_RUDOLPHPET);
	Add( TEXT("MAGIC_MON_DROPGREATWALLASURA"),	MAGIC_MON_DROPGREATWALLASURA,	MAGIC_GROUP_DROPGREATWALLASURA);
	Add( TEXT("MAGIC_MON_AMBERSAURUS"),			MAGIC_MON_AMBERSAURUS,			MAGIC_GROUP_AMBERSAURUS );
	Add( TEXT("MAGIC_MON_ROLLINGSTEGO"),		MAGIC_MON_ROLLINGSTEGO,			MAGIC_GROUP_ROLLINGSTEGO );
	Add( TEXT("MAGIC_MON_SUMMONDRAGON_STUN"),	MAGIC_MON_SUMMONDRAGON_STUN,	MAGIC_GROUP_SUMMONDRAGON_STUN );
	Add( TEXT("MAGIC_MON_DRAGONFIRE"),			MAGIC_MON_DRAGONFIRE,			MAGIC_GROUP_DRAGONFIRE );
	Add( TEXT("MAGIC_MON_CLOWNBALL"),			MAGIC_MON_CLOWNBALL,			MAGIC_GROUP_CLOWNBALL );
	Add( TEXT("MAGIC_MON_BOUQUET"),				MAGIC_MON_BOUQUET,				MAGIC_GROUP_BOUQUET );

	// MAGIC_TYPE_SOULGUARD
	Add( TEXT("MAGIC_SOULGUARD_FIRE"),			MAGIC_SOULGUARD_FIRE,			MAGIC_GROUP_FIREBALL);
	Add( TEXT("MAGIC_SOULGUARD_PUMKIN"),		MAGIC_SOULGUARD_PUMKIN,			MAGIC_GROUP_SKELLGHOST);	// 소울 가드 펌킨이 사용 하는 마법

	// MAGIC_TYPE_SKILL_BOOK
	Add( TEXT("MAGIC_SKILL_BOOK_FLAMEBUSTER"),	MAGIC_SKILL_BOOK_FLAMEBUSTER,	MAGIC_GROUP_SKILL_BOOK_FLAMEBUSTER	);
	Add( TEXT("MAGIC_SKILL_BOOK_FLAMEWHIRL"	),	MAGIC_SKILL_BOOK_FLAMEWHIRL,	MAGIC_GROUP_SKILL_BOOK_FLAMEWHIRL	);	
	Add( TEXT("MAGIC_SKILL_BOOK_ICERAIN"),		MAGIC_SKILL_BOOK_ICERAIN,		MAGIC_GROUP_SKILL_BOOK_ICERAIN		);
	Add( TEXT("MAGIC_SKILL_BOOK_WETFOG"),		MAGIC_SKILL_BOOK_WETFOG,		MAGIC_GROUP_SKILL_BOOK_WETFOG		);	
	Add( TEXT("MAGIC_SKILL_BOOK_CRASH"),		MAGIC_SKILL_BOOK_CRASH,			MAGIC_GROUP_SKILL_BOOK_CRASH		);
	Add( TEXT("MAGIC_SKILL_BOOK_SLING"),		MAGIC_SKILL_BOOK_SLING,			MAGIC_GROUP_SKILL_BOOK_SLING		);	
	Add( TEXT("MAGIC_SKILL_BOOK_TORNADO"),		MAGIC_SKILL_BOOK_TORNADO,		MAGIC_GROUP_SKILL_BOOK_TORNADO		);
	Add( TEXT("MAGIC_SKILL_BOOK_WINDCUTER"),	MAGIC_SKILL_BOOK_WINDCUTER,		MAGIC_GROUP_SKILL_BOOK_WINDCUTER	);	

	Add( TEXT("MAGIC_SKILL_BOOK_BUFTIMEINCREASE1"	 	),	MAGIC_SKILL_BOOK_BUFTIMEINCREASE1,		MAGIC_GROUP_SKILL_BOOK_SUB	);
	Add( TEXT("MAGIC_SKILL_BOOK_BUFTIMEINCREASE2"		),	MAGIC_SKILL_BOOK_BUFTIMEINCREASE2,		MAGIC_GROUP_SKILL_BOOK_SUB	);	
	Add( TEXT("MAGIC_SKILL_BOOK_HITCOUNTINCREASE"		),	MAGIC_SKILL_BOOK_HITCOUNTINCREASE,		MAGIC_GROUP_SKILL_BOOK_SUB	);
	Add( TEXT("MAGIC_SKILL_BOOK_FREEZINGTIMEINCREASE"	),	MAGIC_SKILL_BOOK_FREEZINGTIMEINCREASE,	MAGIC_GROUP_SKILL_BOOK_SUB	);	
	Add( TEXT("MAGIC_SKILL_BOOK_ADDMAGIC"				),	MAGIC_SKILL_BOOK_ADDMAGIC,				MAGIC_GROUP_SKILL_BOOK_SUB	);
	Add( TEXT("MAGIC_SKILL_BOOK_ATTINCREASE"			),	MAGIC_SKILL_BOOK_ATTINCREASE,			MAGIC_GROUP_SKILL_BOOK_SUB	);	
	Add( TEXT("MAGIC_SKILL_BOOK_ATTINCREASE_CANE"		),	MAGIC_SKILL_BOOK_ATTINCREASE_CANE,		MAGIC_GROUP_SKILL_BOOK_SUB	);
	Add( TEXT("MAGIC_SKILL_BOOK_RANGEINCREASE"			),	MAGIC_SKILL_BOOK_RANGEINCREASE,			MAGIC_GROUP_SKILL_BOOK_SUB	);	
	Add( TEXT("MAGIC_SKILL_BOOK_COOLTIMEDECREASE"		),	MAGIC_SKILL_BOOK_COOLTIMEDECREASE,		MAGIC_GROUP_SKILL_BOOK_SUB	);
	Add( TEXT("MAGIC_SKILL_BOOK_MPDECREASE"				),	MAGIC_SKILL_BOOK_MPDECREASE,			MAGIC_GROUP_SKILL_BOOK_SUB	);	
	Add( TEXT("MAGIC_SKILL_BOOK_ORGDECREASE"			),	MAGIC_SKILL_BOOK_ORGDECREASE,			MAGIC_GROUP_SKILL_BOOK_SUB	);
}