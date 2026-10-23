#ifndef _NMAP_H_
#define _NMAP_H_

#include "NTCHARString.h"
#include "NArray.h"
#include "NHashTableString.h"

#include <assert.h>

template<int T_MAXKEYLEN, class DataType>
class NMap;

template<int T_MAXKEYLEN, class DataType>
class NMapEntry
{
	friend class NMap<T_MAXKEYLEN, DataType>;

public:
	NMapEntry()		{	}
	~NMapEntry()	{	}

protected:
	NTCHARString<T_MAXKEYLEN>		m_Key;
	DataType						m_Data;
};

template<int T_MAXKEYLEN, class DataType>
class NMap
{
public:
	NMap(int nStrBufferSize)	{	m_pszStringBuffer = new char[nStrBufferSize];			}
	~NMap()						{	if( m_pszStringBuffer)	delete[] m_pszStringBuffer;		}

	bool CreateMap( int MaxSize )
	{
		m_Datas.NewMem( MaxSize );
		m_KeyTable.CreateHashTableString( MaxSize, MaxSize, T_MAXKEYLEN );

		ClearAll();

		return true;
	}

	DataType& operator [] (TCHAR* pszKey)
	{
		int Index;
		bool bFind = m_KeyTable.Find( pszKey, Index );

		if( bFind )
		{
			return m_Datas[Index].m_Data;
		}
		else
		{
			int EmptyIndex = FindEmptyEntry();
			if( EmptyIndex >= 0 )
			{
				m_KeyTable.Insert( pszKey, EmptyIndex );
				m_Datas[EmptyIndex].m_Key = pszKey;
				return m_Datas[EmptyIndex].m_Data;
			}
			else
			{
				static DataType DumyData;
#ifdef _DEBUG
				assert( EmptyIndex >= 0 );
#endif
				return DumyData;
			}
		}
	}

	void ClearAll()
	{
		for( int i=0; i<m_Datas.GetSize(); i++ )
		{
			m_Datas[i].m_Key = "";
			EmptyEntry( m_Datas[i].m_Data );
		}
	}

	template<int T_MAXSIZE>
	bool ToString(NTCHARString<T_MAXSIZE>& kRet)
	{
		for( int i=0; i<m_Datas.GetSize(); i++ )
		{
			if( !IsEmptyEntry(m_Datas[i].m_Data) )
			{
				kRet += "\"";	kRet += m_Datas[i].m_Key;					kRet += "\""; 
				kRet += "=";
				kRet += "\"";	kRet += EntryToString(m_Datas[i].m_Data);	kRet += "\""; 
				kRet += ";";
			}	
		}

		return true;
	}

	// Key1=Value1;"Key2"="Value2";Key3 = Value3;Key4 = "Value 4"
	// ���� ���� �����͸� MAP���� ����
	virtual bool Parse( const TCHAR* pszString ) = 0;

protected:
	template<int T_MAXSTR>
	bool _Parse( const TCHAR* pszString, NTCHARString<T_MAXSTR>& kBuf )
	{
		if( pszString == NULL )			return false;

		int nPtr = 0;
		NTCHARString<T_MAXKEYLEN>	kKey;
		bool bQuotation = false;
		int  CurrentData = 0;		// 0:Key, 1:Data

		while ( pszString[nPtr] != '\0' )
		{
			if( _istspace( pszString[nPtr] ) )
			{
				if( bQuotation )		// ����ǥ �ȿ� �ִ� �͸� �����Ѵ�.
				{
					switch( CurrentData )
					{
					case 0:		kKey	+= pszString[nPtr];		break;
					case 1:		kBuf	+= pszString[nPtr];		break;
					}
				}
			}
			else if( pszString[nPtr] == '=' )
			{
				CurrentData = 1;	// ���Ĵ� ������ ó��
			}
			else if( pszString[nPtr] == '\"' )
			{
				bQuotation = !bQuotation;
			}
			else if( pszString[nPtr] == ';' )
			{
				(*this)[kKey] = EntryFromString(kBuf);

				CurrentData = 0;	// ���Ĵ� Ű ó��
				kKey	= "";		// �ӽ� Ű �ʱ�ȭ
				kBuf	= "";		// �ӽ� ������ �ʱ�ȭ
			}
			else
			{
				switch( CurrentData )
				{
				case 0:		kKey	+= pszString[nPtr];		break;
				case 1:		kBuf	+= pszString[nPtr];		break;
				}
			}
			nPtr++;
		}

		if( kKey.Length() > 0 && kBuf.Length() > 0 )
		{
			(*this)[kKey] = EntryFromString(kBuf);
		}

		return true;
	}

	virtual int FindEmptyEntry()
	{
		for( int i=0; i<m_Datas.GetSize(); i++ )
		{
			if( IsEmptyEntry(m_Datas[i].m_Data) )
				return i;
		}
		return -1;
	}

	virtual bool IsEmptyEntry( DataType& Data ) = 0;
	virtual void EmptyEntry( DataType& Data ) = 0;
	virtual DataType EntryFromString( const TCHAR* pszString ) = 0;
	virtual TCHAR* EntryToString( DataType& Data ) = 0;

protected:
	NSafeTMemArray< NMapEntry<T_MAXKEYLEN, DataType> >		m_Datas;
	NHashTableString<int>									m_KeyTable;
	TCHAR*													m_pszStringBuffer;
};

template<int T_MAXKEYLEN, int T_MAXDATALEN>
class NMapString : public NMap< T_MAXKEYLEN, NTCHARString<T_MAXDATALEN> >
{
public:
	NMapString() : NMap< T_MAXKEYLEN, NTCHARString<T_MAXDATALEN> >( T_MAXDATALEN )
	{
	}

public:
	bool Parse( const TCHAR* pszString )
	{
		NTCHARString<T_MAXDATALEN>	kValue;

		return _Parse( pszString, kValue );
	}

protected:
	virtual bool IsEmptyEntry( NTCHARString<T_MAXDATALEN>& Data )
	{
		return (Data.Length() == 0);
	}

	virtual void EmptyEntry( NTCHARString<T_MAXDATALEN>& Data )
	{
		Data = "";
	}

	virtual NTCHARString<T_MAXDATALEN> EntryFromString( const TCHAR* pszString )
	{
		NTCHARString<T_MAXDATALEN>	t;
		t = pszString;
		return t;
	}

	virtual TCHAR* EntryToString( NTCHARString<T_MAXDATALEN>& Data )
	{
		return Data;
	}
};

template<int T_MAXKEYLEN>
class NMapInteger : public NMap< T_MAXKEYLEN, SI32 >
{
public:
	NMapInteger() : NMap< T_MAXKEYLEN, SI32 >( 64 )
	{
	}

public:
	bool Parse( const TCHAR* pszString )
	{
		NTCHARString<64>	kValue;

		return _Parse( pszString, kValue );
	}

protected:
	virtual bool IsEmptyEntry( SI32& Data )
	{
		return (Data == 0);
	}

	virtual void EmptyEntry( SI32& Data )
	{
		Data = 0;
	}

	virtual SI32 EntryFromString( const TCHAR* pszString )
	{
		SI32 t;
		t = _ttoi( pszString );
		return t;
	}

	virtual TCHAR* EntryToString( SI32& Data )
	{
		if( m_pszStringBuffer == NULL )		return NULL;
		strcpy(m_pszStringBuffer, SI32ToString( Data ));
		return m_pszStringBuffer;
	}
};

#endif