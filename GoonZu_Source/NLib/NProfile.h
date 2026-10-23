/***************************************************************************************************
**
** profile.h
**
** Real-Time Hierarchical Profiling for Game Programming Gems 3
**
** by Greg Hjelstrom & Byon Garrabrant
**
***************************************************************************************************/

#ifndef __GEM3_PROFILE_H__
#define __GEM3_PROFILE_H__

/*
** A node in the Profile Hierarchy Tree
*/
class	NProfileNode {

public:
	NProfileNode( const char * name, NProfileNode * parent );
	~NProfileNode( void );

	NProfileNode *	Get_Sub_Node( const char * name );

	NProfileNode *	Get_Parent( void )			{ return Parent; }
	NProfileNode *	Get_Sibling( void )			{ return Sibling; }
	NProfileNode *	Get_Child( void )			{ return Child; }

	void			Reset( void );
	void			Call( void );
	bool			Return( void );

	const char *	Get_Name( void )			{ return Name; }
	int				Get_Total_Calls( void )		{ return TotalCalls; }
	float			Get_Total_Time( void )		{ return TotalTime; }

protected:

	const char *	Name;
	int				TotalCalls;
	float			TotalTime;
	__int64			StartTime;
	int				RecursionCounter;

	NProfileNode *	Parent;
	NProfileNode *	Child;
	NProfileNode *	Sibling;
};

/*
** An iterator to navigate through the tree
*/
class NProfileIterator
{
public:
	// Access all the children of the current parent
	void			First(void);
	void			Next(void);
	bool			Is_Done(void);

	void			Enter_Child( int index );		// Make the given child the new parent
	void			Enter_Largest_Child( void );	// Make the largest child the new parent
	void			Enter_Parent( void );			// Make the current parent's parent the new parent

	// Access the current child
	const char *	Get_Current_Name( void )				{ return CurrentChild->Get_Name(); }
	int				Get_Current_Total_Calls( void )			{ return CurrentChild->Get_Total_Calls(); }
	float			Get_Current_Total_Time( void )			{ return CurrentChild->Get_Total_Time(); }
	void			Current_Reset( void )					{ CurrentChild->Reset(); }

	// Access the current parent
	const char *	Get_Current_Parent_Name( void )			{ return CurrentParent->Get_Name(); }
	int				Get_Current_Parent_Total_Calls( void )	{ return CurrentParent->Get_Total_Calls(); }
	float			Get_Current_Parent_Total_Time( void )	{ return CurrentParent->Get_Total_Time(); }
	void			Current_Parent_Reset( void )			{ CurrentParent->Reset(); }

protected:

	NProfileNode *	CurrentParent;
	NProfileNode *	CurrentChild;

	NProfileIterator( NProfileNode * start );
	friend	class		NProfileSystem;
};


/*
** The Manager for the Profile system
*/
class	NProfileSystem {
public:
	static	void				Start_Profile( const char * name );
	static	void				Stop_Profile( void );

	static	void				Reset( void );
	static	void				Increment_Frame_Counter( void );
	static	int					Get_Frame_Count_Since_Reset( void )		{ return FrameCounter; }
	static	float				Get_Time_Since_Reset( void );

	static	NProfileIterator *	Get_Iterator( void )	{ return new NProfileIterator( &Root ); }
	static	void				Release_Iterator( NProfileIterator * iterator ) { delete iterator; }

private:
	static	NProfileNode		Root;
	static	NProfileNode *		CurrentNode;
	static	int					FrameCounter;
	static	__int64				ResetTime;
};


/*
** ProfileSampleClass is a simple way to profile a function's scope
** Use the PROFILE macro at the start of scope to time
*/
class	NProfileSample 
{
public:
	NProfileSample( const char * name )
	{ 
		NProfileSystem::Start_Profile( name ); 
	}

	~NProfileSample( void )					
	{ 
		NProfileSystem::Stop_Profile(); 
	}
};

//=============================================================================
//	
//=============================================================================
extern	BOOL	bProfileCalculateON;

#define	PROFILE( name )			if(bProfileCalculateON == TRUE) NProfileSample __profile( name )
#define	PROFILE_START( name )	if(bProfileCalculateON == TRUE) NProfileSystem::Start_Profile( name ) 		
#define	PROFILE_STOP()			if(bProfileCalculateON == TRUE) NProfileSystem::Stop_Profile()		
#define	PROFILE_RESET()			if(bProfileCalculateON == TRUE) NProfileSystem::Reset()
#define	PROFILE_INC_FRAME()		if(bProfileCalculateON == TRUE) NProfileSystem::Increment_Frame_Counter()
#define	PROFILE_CALC_START()	{ NProfileSystem::Reset(); bProfileCalculateON = TRUE;  }
#define	PROFILE_CALC_END()		{ NProfileSystem::Reset(); bProfileCalculateON = FALSE; }

//=============================================================================

#endif