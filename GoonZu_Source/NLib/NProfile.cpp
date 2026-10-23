/***************************************************************************************************
**
** profile.cpp
**
** Real-Time Hierarchical Profiling for Game Programming Gems 3
**
** by Greg Hjelstrom & Byon Garrabrant
**
***************************************************************************************************/
#include <windows.h>
#include "./NProfile.h"


BOOL	bProfileCalculateON = FALSE;

inline void Profile_Get_Ticks(_int64 * ticks)
{
	__asm
	{
		push edx;
		push ecx;
		mov ecx,ticks;
		_emit 0Fh
			_emit 31h
			mov [ecx],eax;
		mov [ecx+4],edx;
		pop ecx;
		pop edx;
	}
}

inline float Profile_Get_Tick_Rate(void)
{
	static float _CPUFrequency = -1.0f;

	if (_CPUFrequency == -1.0f) {
		__int64 curr_rate = 0;
		::QueryPerformanceFrequency ((LARGE_INTEGER *)&curr_rate);
		_CPUFrequency = (float)curr_rate;
	} 

	return _CPUFrequency;
}

/***************************************************************************************************
**
** NProfileNode
**
***************************************************************************************************/

/***********************************************************************************************
* INPUT:                                                                                      *
* name - pointer to a static string which is the name of this profile node                    *
* parent - parent pointer                                                                     *
*                                                                                             *
* WARNINGS:                                                                                   *
* The name is assumed to be a static pointer, only the pointer is stored and compared for     *
* efficiency reasons.                                                                         *
*=============================================================================================*/
NProfileNode::NProfileNode( const char * name, NProfileNode * parent ) :
Name( name ),
TotalCalls( 0 ),
TotalTime( 0 ),
StartTime( 0 ),
RecursionCounter( 0 ),
Parent( parent ),
Child( NULL ),
Sibling( NULL )
{
	Reset();
}


NProfileNode::~NProfileNode( void )
{
	delete Child;
	delete Sibling;
}


/***********************************************************************************************
* INPUT:                                                                                      *
* name - static string pointer to the name of the node we are searching for                   *
*                                                                                             *
* WARNINGS:                                                                                   *
* All profile names are assumed to be static strings so this function uses pointer compares   *
* to find the named node.                                                                     *
*=============================================================================================*/
NProfileNode * NProfileNode::Get_Sub_Node( const char * name )
{
	// Try to find this sub node
	NProfileNode * child = Child;
	while ( child ) {
		if ( child->Name == name ) {
			return child;
		}
		child = child->Sibling;
	}

	// We didn't find it, so add it
	NProfileNode * node = new NProfileNode( name, this );
	node->Sibling = Child;
	Child = node;
	return node;
}


void	NProfileNode::Reset( void )
{
	TotalCalls = 0;
	TotalTime = 0.0f;

	if ( Child ) {
		Child->Reset();
	}
	if ( Sibling ) {
		Sibling->Reset();
	}
}


void	NProfileNode::Call( void )
{
	TotalCalls++;
	if (RecursionCounter++ == 0) {
		Profile_Get_Ticks(&StartTime);
	}
}


bool	NProfileNode::Return( void )
{
	if ( --RecursionCounter == 0 && TotalCalls != 0 ) { 
		__int64 time;
		Profile_Get_Ticks(&time);
		time-=StartTime;
		TotalTime += (float)time / Profile_Get_Tick_Rate();
	}
	return ( RecursionCounter == 0 );
}


/***************************************************************************************************
**
** NProfileIterator
**
***************************************************************************************************/
NProfileIterator::NProfileIterator( NProfileNode * start )
{
	CurrentParent = start;
	CurrentChild = CurrentParent->Get_Child();
}


void	NProfileIterator::First(void)
{
	CurrentChild = CurrentParent->Get_Child();
}


void	NProfileIterator::Next(void)
{
	CurrentChild = CurrentChild->Get_Sibling();
}


bool	NProfileIterator::Is_Done(void)
{
	return CurrentChild == NULL;
}


void	NProfileIterator::Enter_Child( int index )
{
	CurrentChild = CurrentParent->Get_Child();
	while ( (CurrentChild != NULL) && (index != 0) ) {
		index--;
		CurrentChild = CurrentChild->Get_Sibling();
	}

	if ( CurrentChild != NULL ) {
		CurrentParent = CurrentChild;
		CurrentChild = CurrentParent->Get_Child();
	}
}


void	NProfileIterator::Enter_Parent( void )
{
	if ( CurrentParent->Get_Parent() != NULL ) {
		CurrentParent = CurrentParent->Get_Parent();
	}
	CurrentChild = CurrentParent->Get_Child();
}


/***************************************************************************************************
**
** NProfileSystem
**
***************************************************************************************************/

NProfileNode	NProfileSystem::Root( "Root", NULL );
NProfileNode *	NProfileSystem::CurrentNode = &NProfileSystem::Root;
int				NProfileSystem::FrameCounter = 0;
__int64			NProfileSystem::ResetTime = 0;


/***********************************************************************************************
* NProfileSystem::Start_Profile -- Begin a named profile                                    *
*                                                                                             *
* Steps one level deeper into the tree, if a child already exists with the specified name     *
* then it accumulates the profiling; otherwise a new child node is added to the profile tree. *
*                                                                                             *
* INPUT:                                                                                      *
* name - name of this profiling record                                                        *
*                                                                                             *
* WARNINGS:                                                                                   *
* The string used is assumed to be a static string; pointer compares are used throughout      *
* the profiling code for efficiency.                                                          *
*=============================================================================================*/
void	NProfileSystem::Start_Profile( const char * name )
{
	if (name != CurrentNode->Get_Name()) {
		CurrentNode = CurrentNode->Get_Sub_Node( name );
	} 

	CurrentNode->Call();
}


/***********************************************************************************************
* NProfileSystem::Stop_Profile -- Stop timing and record the results.                       *
*=============================================================================================*/
void	NProfileSystem::Stop_Profile( void )
{
	// Return will indicate whether we should back up to our parent (we may
	// be profiling a recursive function)
	if (CurrentNode->Return()) {
		CurrentNode = CurrentNode->Get_Parent();
	}
}


/***********************************************************************************************
* NProfileSystem::Reset -- Reset the contents of the profiling system                       *
*                                                                                             *
*    This resets everything except for the tree structure.  All of the timing data is reset.  *
*=============================================================================================*/
void	NProfileSystem::Reset( void )
{ 
	Root.Reset(); 
	FrameCounter = 0;
	Profile_Get_Ticks(&ResetTime);
}


/***********************************************************************************************
* NProfileSystem::Increment_Frame_Counter -- Increment the frame counter                    *
*=============================================================================================*/
void NProfileSystem::Increment_Frame_Counter( void )
{
	FrameCounter++;
}


/***********************************************************************************************
* NProfileSystem::Get_Time_Since_Reset -- returns the elapsed time since last reset         *
*=============================================================================================*/
float NProfileSystem::Get_Time_Since_Reset( void )
{
	__int64 time;
	Profile_Get_Ticks(&time);
	time -= ResetTime;
	return (float)time / Profile_Get_Tick_Rate();
}


