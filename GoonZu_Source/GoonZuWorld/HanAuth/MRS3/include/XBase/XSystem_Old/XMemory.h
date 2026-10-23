// XMemory.h
/*!
\file					XMemory.h

\remarks				XSystem Library에서 제공하는 Memory Pool은 아래와 같은 특징을 가진다.
						- Memory Pool을 제공함으로써 메모리 할당, 해제 작업의 효율성을 높인다.
						- Memory Pool로부터 할당된 메모리는 XMemory 객체로 관리된다.
						- XMemory는 reference counting 기능을 기본 제공한다.
						- XMemory에 대한 복사는 shallow copy를 기본으로하며, 이를 통한 메모리 할당 및 복사의 overhead를 줄인다.

- Required header: <XBase/XSystem/XMemory.h>
*/

#ifndef __XMemory_Old__
#define __XMemory_Old__

#include "XSystemBase.h"
#include <Windows.h>

class XMemory;

/*!
\brief					새로운 메모리를 할당한다.

\param dwSize			[in] 할당할 메모리 크기

\return					새로 할당된 XMemory 객체

\sa						XMemory, XMemoryFree, XMemoryRealloc, XMemoryClone, XMemorySize
*/
XSYSTEM_API XMemory __stdcall XMemoryAlloc(DWORD dwSize);

/*!
\brief					할당된 메모리를 해제한다.

\param rMem				[in] 해제할 XMemory 객체

\return					This function does not return a value.
*/
XSYSTEM_API void __stdcall XMemoryFree(XMemory& rMem);

/*!
\brief					할당된 메모리의 크기를 변경한다.

\param rMem				[in] 메모리 크기를 변경할 XMemory 객체

\param dwNewSize		[in] 변경될 메모리 크기

\return					메모리 크기가 변경된 XMemory 객체
*/
XSYSTEM_API XMemory __stdcall XMemoryRealloc(XMemory& rMem, DWORD dwNewSize);

/*!
\brief					할당된 메모리를 복제한다.

\param rMem				[in] 복제할 소스 XMemory 객체

\return					복사된 XMemory 객체

\remarks				이때 복사는 소스 XMemory 객체에 할당된 메모리 블럭과 동일한 크기의 메모리 블럭이 
						Memory Pool로부터 할당되고 메모리 블럭의 내용이 복사되는 deep copy를 의미한다.
*/
XSYSTEM_API XMemory __stdcall XMemoryClone(XMemory& rMem);

/*!
\brief					할당된 메모리의 크기를 반환한다.

\param rMem				[in] 할당된 메모리의 크기를 얻을 XMemory 객체

\return					할당된 메모리 크기
*/
XSYSTEM_API DWORD __stdcall XMemorySize(XMemory& rMem);


struct MEMORY_BLOCK;
typedef MEMORY_BLOCK* PMEMORY;


/*!
	\brief				Memory Pool에서 할당된 메모리를 관리하는 class
	\remarks			XMemoryAlloc 함수를 이용하여 Memory Pool로부터 할당된 메모리는 XMemory 객체로 관리된다. \n
						\n
						XSystem Library에서 제공하는 Memory Pool은 아래와 같은 특징을 가진다.
						- Memory Pool을 제공함으로써 메모리 할당, 해제 작업의 효율성을 높인다.
						- Memory Pool로부터 할당된 메모리는 XMemory 객체로 관리된다.
						- XMemory는 reference counting 기능을 기본 제공한다.
						- XMemory에 대한 복사는 shallow copy를 기본으로하며, 이를 통한 메모리 할당 및 복사의 overhead를 줄인다.

	- Required header: <XBase/XSystem/XMemory.h>
*/
class XSYSTEM_API XMemory
{
public:
	/*!
	\brief					defualt constructor

	\remarks				NULL 포인터를 가지는 XMemory 객체를 생성한다.
	*/
	XMemory();

	/*!
	\brief					copy constructor

	\param	rhs				[in] 복사 생성할 소스 XMemory 객체

	\remarks				특정 XMemory 객체를 복사 생성한다. \n
							이때 복사는 단순히 reference count만 증가시키는 shallow copy를 의미하며, 
							생성된 XMemory 객체와 소스 XMemory 객체는 동일한 메모리 블럭을 가리키게 된다.
	*/
    XMemory(const XMemory &rhs);

	/*!
	\brief					destructor

	\remarks				XMemory가 할당된 메모리 블럭을 가지는 경우 XMemoryFree를 사용하여 해제 작업을 수행한다.

	\sa						XMemoryFree, XMemoryAlloc
	*/
	~XMemory();

	/*!
	\brief					할당된 메모리 블럭을 const void* 타입으로 반환한다.

	\param					This function has no parameters.

	\return					할당된 메모리 블럭에 대한 const void* 타입

	\sa						XMemorySize, XMemoryAlloc, XMemoryFree
	*/
	const void* GetData() const;

	/*!
	\brief					할당된 메모리 블럭을 void* 타입으로 반환한다.

	\param					This function has no parameters.

	\return					할당된 메모리 블럭에 대한 void* 타입

	\sa						operator const void*, XMemorySize, XMemoryAlloc, XMemoryFree
	*/
	void* GetData();

	/*!
	\brief					할당된 메모리의 크기를 반환한다.

	\param					This function has no parameters.

	\return					할당된 메모리의 크기

	\sa						operator void*, XMemorySize, XMemoryAlloc, XMemoryFree
	*/
	DWORD GetSize() const;

	/*!
	\brief					메모리 블럭이 할당되지 않은 XMemory 객체인지 검사한다.

	\param					This function has no parameters.

	\return					BOOL
							- TRUE: 메모리 블럭이 할당되지 않은 XMemory 객체, NULL 포인터를 가리키고 있다.
							- FALSE: 할당된 메모리를 가지고 있는 XMemory 객체

	\sa						XMemoryAlloc, XMemoryFree
	*/
	BOOL IsEmpty() const;


	/*!
	\brief					XMemory 객체에 대한 assign operator

	\param rMem				[in] assign할 right-hand side의 XMemory 객체

	\return					assign된 left-hand side의 XMemory 객체

	\remarks				XMemory 객체에 대한 assign operator는 right-hand side의 XMemory 객체의 메모리 블럭을 
							left-hand side 객체의 메모리 블럭으로 복사하는 동작을 의미한다. 
							이때 복사는 단순히 reference count만 증가시키는 shallow copy를 의미하며, 
							left-hand side의 XMemory 객체와 right-hand side의 XMemory 객체는 동일한 메모리 블럭을 가리키게 된다. \n
							만약 assign operator가 호출된 시점에 left-hand side의 XMemory 객체가 이미 할당된 메모리 블럭을 
							가지고 있는 경우 가지고 있던 메모리 블럭을 XMemoryFree 함수로 해제한 후 right-hand side의 
							XMemory 객체의 메모리 블럭을 shallow copy 한다.

	\sa						XMemoryClone, XMemoryAlloc, XMemoryFree
	*/
	XMemory& operator = (const XMemory& rMem);

	/*!
	\brief					동일한 메모리 블럭을 가리키는 XMemory인지 검사한다.

	\param rMem				[in] 비교할 XMemory 객체

	\return					BOOL
							- TRUE: 동일한 메모리 블럭을 가리키고 있다.
							- FALSE: 서로 다른 메모리 블럭을 가리키고 있다.

	\sa						operator ==, XMemoryAlloc, XMemoryFree
	*/
	BOOL operator == (const XMemory& rMem) const;

	/*!
	\brief					동일한 메모리 블럭을 가리키는 XMemory가 아닌지 검사한다.

	\param rMem				[in] 비교할 XMemory 객체

	\return					BOOL
							- TRUE: 서로 다른 메모리 블럭을 가리키고 있다.
							- FALSE: 동일한 메모리 블럭을 가리키고 있다.

	\sa						operator ==, XMemoryAlloc, XMemoryFree
	*/
	BOOL operator != (const XMemory& rMem) const;


	/*!
	\brief					할당된 메모리 블럭을 const void* 자료형으로 access하기 위한 casting operator

	\param					This function has no parameters.

	\return					const void* 자료형으로 casting된 메모리 블럭

	\sa						operator void*, XMemoryAlloc, XMemoryFree
	*/
	operator const void*() const;

	/*!
	\brief					할당된 메모리 블럭을 void* 자료형으로 access하기 위한 casting operator

	\param					This function has no parameters.

	\return					void* 자료형으로 casting된 메모리 블럭

	\sa						operator const void*, XMemoryAlloc, XMemoryFree
	*/
	operator void*();

private:
	explicit XMemory(PMEMORY pMem);

	BOOL Alloc(DWORD dwSize);
	BOOL Realloc(DWORD dwNewSize);
	void Free();
	XMemory Clone();

	inline static PMEMORY InternalAlloc(DWORD dwSize);

	friend XSYSTEM_API XMemory __stdcall XMemoryAlloc(DWORD dwSize);
	friend XSYSTEM_API void __stdcall XMemoryFree(XMemory& rMem);
	friend XSYSTEM_API XMemory __stdcall XMemoryRealloc(XMemory& rMem, DWORD dwNewSize);
	friend XSYSTEM_API XMemory __stdcall XMemoryClone(XMemory& rMem);

private:
	PMEMORY m_pMem;
};

inline BOOL operator == (const void* lhs, const XMemory& rhs)
{
	return (rhs == lhs);
}

#endif //__XMemory__
