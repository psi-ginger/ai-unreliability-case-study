#ifndef __XString_Old__
#define __XString_Old__

#include <string>

#pragma warning(disable:4512)
namespace InternalBasicString
{
    template<typename T>
    class XTypeSelect;

    template<>
    class XTypeSelect<char>
    {
    public:
        const bool bUnicode;
        typedef std::basic_string<char> TStdStringType;

    private:
        wchar_t *m_szBuffer;

    public:
        XTypeSelect(void) : bUnicode(false)
        {
            m_szBuffer = NULL;
        }
        ~XTypeSelect(void)
        {
            delete[] m_szBuffer;
        }

        const wchar_t *ConvertToW(const wchar_t *szBuffer, int nLen)
        {
            (void)nLen;

            return szBuffer;
        }
        const wchar_t *ConvertToW(const char *szBuffer, int nLen)
        {
            if (m_szBuffer != NULL)
            {
                delete[] m_szBuffer;
                m_szBuffer = NULL;
            }
            m_szBuffer = new wchar_t[nLen + 1];
            MultiByteToWideChar(CP_ACP, 0, szBuffer, -1, m_szBuffer, nLen + 1);

            return m_szBuffer;
        }

        const char *ConvertToA(const wchar_t *szBuffer, int nLen)
        {
            (void)szBuffer;
            (void)nLen;

            return 0;
        }
        const char *ConvertToA(const char *szBuffer, int nLen)
        {
            (void)nLen;

            return szBuffer;
        }
    };

    template<>
    class XTypeSelect<wchar_t>
    {
    public:
        const bool bUnicode;
        typedef std::basic_string<wchar_t> TStdStringType;

    private:
        char *m_szBuffer;

    public:
        XTypeSelect(void) : bUnicode(true)
        {
            m_szBuffer = NULL;
        }
        ~XTypeSelect(void)
        {
            delete[] m_szBuffer;
        }

        const wchar_t *ConvertToW(const wchar_t *szBuffer, int nLen)
        {
            (void)nLen;

            return szBuffer;
        }
        const wchar_t *ConvertToW(const char *szBuffer, int nLen)
        {
            (void)szBuffer;
            (void)nLen;

            return 0;
        }

        const char *ConvertToA(const wchar_t *szBuffer, int nLen)
        {
            if (m_szBuffer != NULL)
            {
                delete[] m_szBuffer;
                m_szBuffer = NULL;
            }
            m_szBuffer = new char[(nLen + 1) * sizeof(wchar_t)];            // must include NULL chars
            WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, szBuffer, -1, m_szBuffer, (nLen + 1) * sizeof(wchar_t), NULL, NULL);

            return m_szBuffer;
        }
        const char *ConvertToA(const char *szBuffer, int nLen)
        {
            (void)nLen;

            return szBuffer;
        }
    };
}   // namespace InternalBasicString
#pragma warning(default:4512)

template<typename T>
class XBasicString
{
protected:
    typedef typename InternalBasicString::XTypeSelect<T>::TStdStringType TStdStringType;

    TStdStringType m_str;
    mutable InternalBasicString::XTypeSelect<T> m_Convert;

public:
    XBasicString(void)
    {
    }
    XBasicString(const XBasicString &rhs) : m_str(rhs.m_str)
    {
    }
    XBasicString(const XBasicString &rhs, int nOffset, int nCount) : m_str(rhs.m_str, nOffset, nCount)
    {
    }
    XBasicString(const T *szPtr) : m_str(szPtr)
    {
    }
    XBasicString(const T *szPtr, int nCount) : m_str(szPtr, nCount)
    {
    }
    XBasicString(int nCount, T ch) : m_str(nCount, ch)
    {
    }
    ~XBasicString(void)
    {
    }

    int Capacity(void) const
    {
        // return current length of allocated storage
        return static_cast<int>(m_str.capacity());
    }
    void Reserve(int nNewCapacity = 0)
    {
        m_str.reserve(nNewCapacity);
    }
    int Length(void) const
    {
        return static_cast<int>(m_str.length());
    }
    bool Empty(void) const
    {
        return m_str.empty();
    }

    void Resize(int nNewSize)
    {
        // determine new length, padding with null elements as needed
        Resize(nNewSize, T());
    }
    void Resize(int nNewSize, T ch)
    {
        // determine new length, padding with ch elements as needed
        m_str.resize(nNewSize, ch);
    }

    // Insert
    XBasicString &Insert(int nOffset, const XBasicString &rhs)
    {
        // insert rhs at nOffset
        m_str.insert(nOffset, rhs.m_str);

        return *this;
    }
    XBasicString &Insert(int nOffset, const XBasicString &rhs, int nRightOffset, int nRightCount)
    {
        // insert rhs [nRightOffset, nRightOffset + nRightCount) at nOffset
        m_str.insert(nOffset, rhs.m_str, nRightOffset, nRightCount);

        return *this;
    }
    XBasicString &Insert(int nOffset, const T *szPtr, int nCount)
    {
        // insert [szPtr, szPtr + nCount) at nOffset
        m_str.insert(nOffset, szPtr, nCount);

        return *this;
    }
    XBasicString &Insert(int nOffset, const T *szPtr)
    {
        // insert [szPtr, <null>) at nOffset
        m_str.insert(nOffset, szPtr);

        return *this;
    }
    XBasicString &Insert(int nOffset, int nCount, T ch)
    {
        // insert nCount * ch at nOffset
        m_str.insert(nOffset, nCount, ch);

        return *this;
    }

    // Erase
    XBasicString &Erase(int nOffset, int nCount)
    {
        m_str.erase(nOffset, nCount);

        return *this;
    }
    void Clear(void)
    {
        // erase all
        m_str.clear();
    }

    // Replace
    XBasicString &Replace(int nOffset, int nCount, const XBasicString &rhs)
    {
        // replace [nOffset, nOffset + nCount) with rhs
        m_str.replace(nOffset, nCount, rhs.m_str);

        return *this;
    }
    XBasicString &Replace(int nOffset, int nCount, const XBasicString &rhs, int nRightOffset, int nRightCount)
    {
        // replace [nOffset, nOffset + nCount) with rhs [nRightOffset, nRightOffset + nRightCount)
        m_str.replace(nOffset, nCount, rhs.m_str, nRightOffset, nRightCount);

        return *this;
    }
    XBasicString &Replace(int nOffset, int nCount, const T *szPtr, int nRightCount)
    {
        // replace [nOffset, nOffset + nCount) with [szPtr, szPtr + nRightCount)
        m_str.replace(nOffset, nCount, szPtr, nRightCount);

        return *this;
    }
    XBasicString &Replace(int nOffset, int nCount, const T *szPtr)
    {
        // replace [nOffset, nOffset + nCount) with [szPtr, <null>)
        m_str.replace(nOffset, nCount, szPtr);

        return *this;
    }
    XBasicString &Replace(int nOffset, int nCount, int nCharCount, T ch)
    {
        // replace [nOffset, nOffset + nCount) with nCharCount * ch
        m_str.replace(nOffset, nCount, nCharCount, ch);

        return *this;
    }

    // Copy
    int Copy(T *szPtr, int nCount, int nOffset = 0) const
    {
        return static_cast<int>(m_str.copy(szPtr, nCount, nOffset));
    }

    // Swap
    void Swap(XBasicString &rhs)
    {
        m_str.swap(rhs.m_str);
    }

    // Find
    int Find(const XBasicString &rhs, int nOffset = 0) const
    {
        // look for rhs beginning at or after nOffset
        return static_cast<int>(m_str.find(rhs, nOffset));
    }
    int Find(const T *szPtr, int nOffset, int nCount) const
    {
        // look for [szPtr, szPtr + nCount) beginning at or after nOffset
        return static_cast<int>(m_str.find(szPtr, nOffset, nCount));
    }
    int Find(const T *szPtr, int nOffset = 0) const
    {
        // look for [szPtr, <null>) beginning at or after nOffset
        return static_cast<int>(m_str.find(szPtr, nOffset));
    }
    int Find(T ch, int nOffset) const
    {
        // look for ch at or after nOffset
        return static_cast<int>(m_str.find(ch, nOffset));
    }

    // Compare
    int Compare(const XBasicString &rhs) const
    {
        // compare [0, Length) with rhs
        return m_str.compare(rhs.m_str);
    }
    int Compare(int nOffset, int nCount, const XBasicString &rhs) const
    {
        // compare [nOffset, nOffset + nCount) with rhs
        return m_str.compare(nOffset, nCount, rhs.m_str);
    }
    int Compare(int nOffset, int nCount, const XBasicString &rhs, int nRightOffset, int nRightCount) const
    {
        // compare [nOffset, nOffset + nCount) with rhs [nRightOffset, nRightOffset + nRightCount)
        return m_str.compare(nOffset, nCount, rhs.m_str, nRightOffset, nRightCount);
    }
    int Compare(const T *szPtr) const
    {
        // compare [0, Length) with [szPtr, <null>)
        return m_str.compare(szPtr);
    }
    int Compare(int nOffset, int nCount, const T *szPtr) const
    {
        // compare [nOffset, nOffset + nCount) with [szPtr, <null>)
        return m_str.compare(nOffset, nCount, szPtr);
    }
    int Compare(int nOffset, int nCount, const T *szPtr, int nRightCount) const
    {
        // compare [nOffset, nOffset + nCount) with [szPtr, szPtr + nRightCount)
        return m_str.compare(nOffset, nCount, szPtr, nRightCount);
    }

    T &At(int nIndex)
    {
        return m_str.at(nIndex);
    }
    const T &At(int nIndex) const
    {
        return m_str.at(nIndex);
    }

    const T *c_str(void) const
    {
        return m_str.c_str();
    }

    XBasicString &operator=(const XBasicString &rhs)         // assign rhs
    {
        m_str = rhs.m_str;

        return *this;
    }
    XBasicString &operator=(const T *szPtr)                 // assign [szPtr, <null>)
    {
        m_str = szPtr;

        return *this;
    }
    XBasicString &operator=(T ch)                           // assign 1 * ch
    {
        m_str = ch;

        return *this;
    }

    XBasicString &operator+=(const XBasicString &rhs)        // append rhs
    {
        m_str += rhs.m_str;

        return *this;
    }
    XBasicString &operator+=(const T *szPtr)                // append [szPtr, <null>)
    {
        m_str += szPtr;

        return *this;
    }
    XBasicString &operator+=(T ch)                          // append 1 * ch
    {
        m_str += ch;

        return *this;
    }

    T &operator[](int nOffset)
    {
        return m_str._Myptr()[nOffset];
    }
    const T &operator[](int nOffset) const
    {
        return m_str._Myptr()[nOffset];
    }

    operator const TStdStringType &() const
    {
        return m_str;
    }

    operator const char *() const
    {
        return m_Convert.ConvertToA(m_str.c_str(), static_cast<int>(m_str.length()));
    }
    operator const wchar_t *() const
    {
        return m_Convert.ConvertToW(m_str.c_str(), static_cast<int>(m_str.length()));
    }
};

typedef XBasicString<wchar_t> XStringW;
typedef XBasicString<char> XStringA;

#if defined _UNICODE || defined UNICODE
typedef XStringW XString;
#else
typedef XStringA XString;
#endif

// operator +
template<typename T>
inline XBasicString<T> operator+(const XBasicString<T> &lhs, const XBasicString<T> &rhs)
{
    // return string + string
    return XBasicString<T>(lhs) += rhs;
}

template<typename T>
inline XBasicString<T> operator+(const T *lhs, const XBasicString<T> &rhs)
{
    // return NTCS(Null-Terminate-Character-String) + string
    return XBasicString<T>(lhs) += rhs;
}

template<typename T>
inline XBasicString<T> operator+(const T lhs, const XBasicString<T> &rhs)
{
    // return character + string
    return XBasicString<T>(1, lhs) += rhs;
}

template<typename T>
inline XBasicString<T> operator+(const XBasicString<T> &lhs, const T *rhs)
{
    // return string + NTCS
    return XBasicString<T>(lhs) += rhs;
}

template<typename T>
inline XBasicString<T> operator+(const XBasicString<T> &lhs, const T rhs)
{
    // return string + character
    return XBasicString<T>(lhs) += rhs;
}

// operator ==
template<typename T>
inline bool operator==(const XBasicString<T> &lhs, const XBasicString<T> &rhs)
{
    // test for string equality
    return lhs.Compare(rhs) == 0;
}

template<typename T>
inline bool operator==(const T *lhs, const XBasicString<T> &rhs)
{
    // test for NTCS vs. string equality
    return rhs.Compare(lhs) == 0;
}

template<typename T>
inline bool operator==(const XBasicString<T> &lhs, const T *rhs)
{
    // test for string vs. NTCS equality
    return lhs.Compare(rhs) == 0;
}

// operator !=
template<typename T>
inline bool operator!=(const XBasicString<T> &lhs, const XBasicString<T> &rhs)
{
    // test for string inequality
    return (lhs == rhs) == false;
}

template<typename T>
inline bool operator!=(const T *lhs, const XBasicString<T> &rhs)
{
    // test for NTCS vs. string inequality
    return (lhs == rhs) == false;
}

template<typename T>
inline bool operator!=(const XBasicString<T> &lhs, const T *rhs)
{
    // test for string vs. NTCS inequality
    return (lhs == rhs) == false;
}

// operator <
template<typename T>
inline bool operator<(const XBasicString<T> &lhs, const XBasicString<T> &rhs)
{
    // test if string < string
    return lhs.Compare(rhs) < 0;
}

template<typename T>
inline bool operator<(const T *lhs, const XBasicString<T> &rhs)
{
    // test if NTCS < string
    return rhs.Compare(lhs) > 0;
}

template<typename T>
inline bool operator<(const XBasicString<T> &lhs, const T *rhs)
{
    // test if string < NTCS
    return lhs.Compare(rhs) < 0;
}

// operator >
template<typename T>
inline bool operator>(const XBasicString<T> &lhs, const XBasicString<T> &rhs)
{
    // test if string > string
    return rhs < lhs;
}

template<typename T>
inline bool operator>(const T *lhs, const XBasicString<T> &rhs)
{
    // test if NTCS > string
    return rhs < lhs;
}

template<typename T>
inline bool operator>(const XBasicString<T> &lhs, const T *rhs)
{
    // test if string > NTCS
    return rhs < lhs;
}

// operator <=
template<typename T>
inline bool operator<=(const XBasicString<T> &lhs, const XBasicString<T> &rhs)
{
    // test if string <= string
    return (rhs < lhs) == false;
}

template<typename T>
inline bool operator<=(const T *lhs, const XBasicString<T> &rhs)
{
    // test if NTCS <= string
    return (rhs < lhs) == false;
}

template<typename T>
inline bool operator<=(const XBasicString<T> &lhs, const T *rhs)
{
    // test if string <= NTCS
    return (rhs < lhs) == false;
}

// operator >=
template<typename T>
inline bool operator>=(const XBasicString<T> &lhs, const XBasicString<T> &rhs)
{
    // test if string >= string
    return (lhs < rhs) == false;
}

template<typename T>
inline bool operator>=(const T *lhs, const XBasicString<T> &rhs)
{
    // test if NTCS >= string
    return (lhs < rhs) == false;
}

template<typename T>
inline bool operator>=(const XBasicString<T> &lhs, const T *rhs)
{
    // test if string >= NTCS
    return (lhs < rhs) == false;
}

#endif
