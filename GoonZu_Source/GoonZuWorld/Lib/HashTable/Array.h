// ============================================================================
// Data Structures For Game Programmers
// Ron Penton
// Array.h
// This is the basic 1D array class.
// ============================================================================
#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdio.h>

template<class Datatype>
class Array
{

// -------------------------------------------------------
// Name:        Array::m_array
// Description: This is a pointer to the array.
// -------------------------------------------------------
    Datatype* m_Array;


// -------------------------------------------------------
// Name:        Array::m_size
// Description: the current size of the array.
// -------------------------------------------------------
    int m_Size;
	
public:

// -------------------------------------------------------
// Name:        Array
// Description: Array의 메모리를 할당한다.
// Arguments:   size - Array의 크기
// -------------------------------------------------------
    Array( int size )
    {
        // 배열을 위한 메모리를 할당한다.
        m_Array = new Datatype[size];

        // Array의 size를 설정한다.
        m_Size = size;
    }


// -------------------------------------------------------
// Name:        ~Array
// Description: Array를 해제한다.
// -------------------------------------------------------
    ~Array()
    {
		// Array가 할당되어 있으면 제거한다.
        if( m_Array != 0 )
            delete[] m_Array;

        // Array Pointer를 초기화한다.
        m_Array = 0;
    }

// -------------------------------------------------------
//  Name:         operator[]
//  Description:  주어진 index로 Item에 접근하도록 한다.
//  Arguments:    index - 배열의 Index
//  Return Value: index에서 배열이 갖고 있는 Item의 Reference
// -------------------------------------------------------
    Datatype& operator[] ( int index )
    {
        return m_Array[index];
    }



// -------------------------------------------------------
//  Name:         Size
//  Description:  Array의 
//  Arguments:    None.
//  Return Value: Array의 크기 
// -------------------------------------------------------
    int Size()
    {
        return m_Size;
    }


// -------------------------------------------------------
//  Name:         operator DataType*
//  Description:  Array의 Pointer를 conver하는 Operator
//  Arguments:    None.
//  Return Value: Array 의 Pointer
// -------------------------------------------------------
    operator Datatype* ()
    {
        return m_Array;
    }


// -------------------------------------------------------
//  Name:         WriteFile
//  Description:  Array 을 저장한다.
//  Arguments:    The Filename
//  Return Value: true on success, false on failure
// -------------------------------------------------------
    bool WriteFile( const char* pFileName )
    {
        FILE* fpOutFile = 0;
        int written = 0;

        // 파일을 연다.
        fpOutFile = _tfopen( pFileName, TEXT("wb") );

        // 파일 열기에 실패하면 돌아간다.
        if( fpOutFile == 0 )	return false;

        // 파일에 Array를 저장한다.
        written = fwrite( m_Array, sizeof( Datatype ), m_Size, fpOutFile );
        fclose( fpOutFile );

		// 쓰기에 성공하지 못하면 false
        if( written != m_Size )	return false;

        // 성공
        return true;
    }
    

// -------------------------------------------------------
//  Name:         ReadFile
//  Description:  Array을 읽어온다.
//  Arguments:    The Filename
//  Return Value: true on success, false on failure
// -------------------------------------------------------
    bool ReadFile( const char* pFileName )
    {
        FILE* fpInFile = 0;
        int read = 0;

        // 파일을 연다.
        fpInFile = _tfopen( pFileName, TEXT("rb") );

        // 파일 열기에 실패하면 돌아간다.
        if( fpInFile == 0 )		return false;

        // 파일에 Array를 읽어온다.
        read = fread( m_Array, sizeof( Datatype ), m_Size, fpInFile );
        fclose( fpInFile );

		// 읽기에 성공하지 못하면 false	
        if( read != m_Size )	return false;

        // 성공
        return true;
    }


};




#endif