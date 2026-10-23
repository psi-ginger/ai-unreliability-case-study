//
// XDR: neXt generation Data Representation
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Implementations of XDR Transfer Syntax.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XDREncodingRulesReference__
#define __XDREncodingRulesReference__

#include <map>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <XBase/XStream.h>

template<typename T>
inline T HostToNetwork(T src)
{
    register int i = 0;
    register int j = sizeof(T) - 1;
    T Result = src;

    while (i < j)
    {
        std::swap(reinterpret_cast<char *>(&Result)[i], reinterpret_cast<char *>(&Result)[j]);
        ++i;
        --j;
    }

    return Result;
}

template<typename T>
inline T NetworkToHost(T src)
{
    return HostToNetwork(src);
}

namespace XDR
{
    namespace EncodingRules
    {
        namespace Private
        {
            enum TTypeCategory
            {
                tcBasicType,
                tcSTLContainer,
                tcUserClass
            };
            template<typename T>
            struct TTypeTraits
            {
                enum {Result = tcUserClass};
                typedef T TType;

                inline static TType Create(void)
                {
                    return T();
                }
            };
            template<typename T>
            struct TTypeTraits<T *>
            {
                enum {Result = tcUserClass};
                typedef T* TType;

                inline static TType Create(void)
                {
                    return new T;
                }
            };
            template<>
            struct TTypeTraits<bool>
            {
                enum {Result = tcBasicType};
                typedef bool TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<>
            struct TTypeTraits<char>
            {
                enum {Result = tcBasicType};
                typedef char TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            //template<>
            //struct TTypeTraits<int8>
            //{
            //    enum {Result = tcBasicType};
            //    typedef int8 TType;

            //    inline static TType Create(void)
            //    {
            //        return TType();
            //    }
            //};
            template<>
            struct TTypeTraits<int16>
            {
                enum {Result = tcBasicType};
                typedef int16 TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<>
            struct TTypeTraits<int32>
            {
                enum {Result = tcBasicType};
                typedef int32 TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<>
            struct TTypeTraits<int64>
            {
                enum {Result = tcBasicType};
                typedef int64 TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<>
            struct TTypeTraits<double>
            {
                enum {Result = tcBasicType};
                typedef double TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<>
            struct TTypeTraits<float>
            {
                enum {Result = tcBasicType};
                typedef float TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<>
            struct TTypeTraits<uchar>
            {
                enum {Result = tcBasicType};
                typedef uchar TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            //template<>
            //struct TTypeTraits<uint8>
            //{
            //    enum {Result = tcBasicType};
            //    typedef uint8 TType;

            //    inline static TType Create(void)
            //    {
            //        return TType();
            //    }
            //};
            template<>
            struct TTypeTraits<uint16>
            {
                enum {Result = tcBasicType};
                typedef uint16 TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<>
            struct TTypeTraits<uint32>
            {
                enum {Result = tcBasicType};
                typedef uint32 TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<>
            struct TTypeTraits<uint64>
            {
                enum {Result = tcBasicType};
                typedef uint64 TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<>
            struct TTypeTraits<std::string>
            {
                enum {Result = tcSTLContainer};
                typedef std::string TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<typename T>
            struct TTypeTraits<std::list<T> >
            {
                enum {Result = tcSTLContainer};
                typedef std::list<T> TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<typename T>
            struct TTypeTraits<std::list<T> *>
            {
                enum {Result = tcSTLContainer};
                typedef std::list<T> TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<typename T>
            struct TTypeTraits<std::vector<T> >
            {
                enum {Result = tcSTLContainer};
                typedef std::vector<T> TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<typename T>
            struct TTypeTraits<std::vector<T> *>
            {
                enum {Result = tcSTLContainer};
                typedef std::vector<T> TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<typename TKey, typename T>
            struct TTypeTraits<std::map<TKey, T> >
            {
                enum {Result = tcSTLContainer};
                typedef std::map<TKey, T> TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
            template<typename TKey, typename T>
            struct TTypeTraits<std::map<TKey, T> *>
            {
                enum {Result = tcSTLContainer};
                typedef std::map<TKey, T> TType;

                inline static TType Create(void)
                {
                    return TType();
                }
            };
        }   // namespace Private

        // Implementation of XDR Basic Encoding Rules
        namespace BER
        {
            template<int nTypeCategory>
            struct TSerializer;

            template<>
            struct TSerializer<Private::tcBasicType>
            {
                // basic types
                template<typename T>
                inline static bool Save(XStream::THandle hStream, const T *pData)
                {
                    T Value = HostToNetwork(*pData);
                    if (XStream::Write(hStream, &Value, sizeof(T)) != sizeof(T))
                    {
                        return false;
                    }

                    return true;
                }
                template<typename T>
                inline static bool Load(XStream::THandle hStream, T *pData)
                {
                    T Value = 0;
                    if (XStream::Read(hStream, &Value, sizeof(T)) != sizeof(T))
                    {
                        return false;
                    }
                    *pData = NetworkToHost(Value);

                    return true;
                }
                template<typename T>
                inline static int32 GetLength(const T *pData)
                {
                    (void)pData;

                    return sizeof(T);
                }

                // array of basic types
                template<typename T>
                inline static bool Save(XStream::THandle hStream, const T *pData, int32 nArraySize)
                {
                    for (int32 i = 0; i < nArraySize; ++i)
                    {
                        if (Save(hStream, &pData[i]) == false)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                template<typename T>
                inline static bool Load(XStream::THandle hStream, T *pData, int32 nArraySize)
                {
                    for (int32 i = 0; i < nArraySize; ++i)
                    {
                        if (Load(hStream, &pData[i]) == false)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                template<typename T>
                inline static int32 GetLength(const T *pData, int32 nArraySize)
                {
                    (void)pData;

                    return sizeof(T) * nArraySize;
                }
            };

            template<>
            struct TSerializer<Private::tcSTLContainer>
            {
                // I/O Policy Selector
                template<typename T, int nTypeCategory>
                struct TIOPolicySelector;

                template<typename T>
                struct TIOPolicySelector<T, Private::tcBasicType>
                {
                    inline static bool Save(XStream::THandle hStream, const T &Item)
                    {
                        if (TSerializer<Private::TTypeTraits<T>::Result>::Save(hStream, &Item) == false)
                        {
                            return false;
                        }

                        return true;
                    }
                    inline static bool Load(XStream::THandle hStream, T &Item)
                    {
                        if (TSerializer<Private::TTypeTraits<T>::Result>::Load(hStream, &Item) == false)
                        {
                            return false;
                        }

                        return true;
                    }
                    inline static int32 GetLength(const T &Item)
                    {
                        return TSerializer<Private::TTypeTraits<T>::Result>::GetLength(&Item);
                    }
                };
                template<typename T>
                struct TIOPolicySelector<T, Private::tcSTLContainer>        // now, only string. ex) list<string>, vector<string>, ...
                {
                    inline static bool Save(XStream::THandle hStream, const T &Item)
                    {
                        if (TSerializer<Private::TTypeTraits<T>::Result>::Save(hStream, &Item) == false)
                        {
                            return false;
                        }

                        return true;
                    }
                    inline static bool Load(XStream::THandle hStream, T &Item)
                    {
                        if (TSerializer<Private::TTypeTraits<T>::Result>::Load(hStream, &Item) == false)
                        {
                            return false;
                        }

                        return true;
                    }
                    inline static int32 GetLength(const T &Item)
                    {
                        return TSerializer<Private::TTypeTraits<T>::Result>::GetLength(&Item);
                    }
                };
                template<typename T>
                struct TIOPolicySelector<T, Private::tcUserClass>
                {
                    inline static bool Save(XStream::THandle hStream, const T &Item)
                    {
                        if (TSerializer<Private::TTypeTraits<T>::Result>::Save(hStream, Item) == false)
                        {
                            return false;
                        }

                        return true;
                    }
                    inline static bool Load(XStream::THandle hStream, T &Item)
                    {
                        if (TSerializer<Private::TTypeTraits<T>::Result>::Load(hStream, Item) == false)
                        {
                            return false;
                        }

                        return true;
                    }
                    inline static int32 GetLength(const T &Item)
                    {
                        return TSerializer<Private::TTypeTraits<T>::Result>::GetLength(Item);
                    }
                };

                inline static bool Save(XStream::THandle hStream, const std::string *pContainer)
                {
                    int32 nSize = static_cast<int32>(pContainer->size());
                    if (nSize > nMaxContainerSize)
                    {
                        assert(!"[TSerializer<Private::tcSTLContainer>::Save()] std::string length is too long.");
                        return false;
                    }

                    if (TSerializer<Private::TTypeTraits<int32>::Result>::Save(hStream, &nSize) == false)
                    {
                        return false;
                    }

                    if (nSize > 0)
                    {
                        std::string &strTemp = const_cast<std::string &>(*pContainer);
                        if (XStream::Write(hStream, &strTemp[0], nSize) != nSize)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                inline static bool Load(XStream::THandle hStream, std::string *pContainer)
                {
                    int32 nSize = 0;
                    if (TSerializer<Private::TTypeTraits<int32>::Result>::Load(hStream, &nSize) == false)
                    {
                        return false;
                    }

                    if (nSize > nMaxContainerSize)
                    {
                        assert(!"[TSerializer<Private::tcSTLContainer>::Load()] std::string length is too long.");
                        return false;
                    }

                    if (nSize > 0)
                    {
                        pContainer->resize(nSize);

                        std::string &strTemp = *pContainer;
                        if (XStream::Read(hStream, &strTemp[0], nSize) != nSize)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                inline static int32 GetLength(const std::string *pContainer)
                {
                    int32 nSize = static_cast<int32>(pContainer->size());

                    return TSerializer<Private::TTypeTraits<int32>::Result>::GetLength(&nSize) + nSize;
                }

                // list
                template<typename T>
                inline static bool Save(XStream::THandle hStream, const std::list<T> *pContainer)
                {
                    int32 nSize = static_cast<int32>(pContainer->size());
                    if (nSize > nMaxContainerSize)
                    {
                        assert(!"[TSerializer<Private::tcSTLContainer>::Save()] std::list size is too big.");
                        return false;
                    }

                    if (TSerializer<Private::TTypeTraits<int32>::Result>::Save(hStream, &nSize) == false)
                    {
                        return false;
                    }

                    for (typename std::list<T>::const_iterator it = pContainer->begin(); it != pContainer->end(); ++it)
                    {
                        if (TIOPolicySelector<typename Private::TTypeTraits<T>::TType, Private::TTypeTraits<T>::Result>::Save(hStream, *it) == false)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                template<typename T>
                inline static bool Load(XStream::THandle hStream, std::list<T> *pContainer)
                {
                    int32 nSize = 0;
                    if (TSerializer<Private::TTypeTraits<int32>::Result>::Load(hStream, &nSize) == false)
                    {
                        return false;
                    }

                    if (nSize > nMaxContainerSize)
                    {
                        assert(!"[TSerializer<Private::tcSTLContainer>::Load()] std::list size is too big.");
                        return false;
                    }

                    for (int32 i = 0; i < nSize; ++i)
                    {
                        typename Private::TTypeTraits<T>::TType Item = Private::TTypeTraits<T>::Create();
                        if (TIOPolicySelector<typename Private::TTypeTraits<T>::TType, Private::TTypeTraits<T>::Result>::Load(hStream, Item) == false)
                        {
                            return false;
                        }
                        pContainer->push_back(Item);
                    }

                    return true;
                }
                template<typename T>
                inline static int32 GetLength(const std::list<T> *pContainer)
                {
                    int32 nLength = TSerializer<Private::TTypeTraits<int32>::Result>::GetLength(&nLength);

                    for (typename std::list<T>::const_iterator it = pContainer->begin(); it != pContainer->end(); ++it)
                    {
                        nLength += TIOPolicySelector<typename Private::TTypeTraits<T>::TType, Private::TTypeTraits<T>::Result>::GetLength(*it);
                    }

                    return nLength;
                }

                // vector
                template<typename T>
                inline static bool Save(XStream::THandle hStream, const std::vector<T> *pContainer)
                {
                    int32 nSize = static_cast<int32>(pContainer->size());
                    if (nSize > nMaxContainerSize)
                    {
                        assert(!"[TSerializer<Private::tcSTLContainer>::Save()] std::vector size is too big.");
                        return false;
                    }

                    if (TSerializer<Private::TTypeTraits<int32>::Result>::Save(hStream, &nSize) == false)
                    {
                        return false;
                    }

                    for (typename std::vector<T>::const_iterator it = pContainer->begin(); it != pContainer->end(); ++it)
                    {
                        if (TIOPolicySelector<typename Private::TTypeTraits<T>::TType, Private::TTypeTraits<T>::Result>::Save(hStream, *it) == false)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                template<typename T>
                inline static bool Load(XStream::THandle hStream, std::vector<T> *pContainer)
                {
                    int32 nSize = 0;
                    if (TSerializer<Private::TTypeTraits<int32>::Result>::Load(hStream, &nSize) == false)
                    {
                        return false;
                    }

                    if (nSize > nMaxContainerSize)
                    {
                        assert(!"[TSerializer<Private::tcSTLContainer>::Load()] std::vector size is too big.");
                        return false;
                    }

                    pContainer->reserve(nSize);

                    for (int32 i = 0; i < nSize; ++i)
                    {
                        typename Private::TTypeTraits<T>::TType Item = Private::TTypeTraits<T>::Create();
                        if (TIOPolicySelector<typename Private::TTypeTraits<T>::TType, Private::TTypeTraits<T>::Result>::Load(hStream, Item) == false)
                        {
                            return false;
                        }
                        pContainer->push_back(Item);
                    }

                    return true;
                }
                template<typename T>
                inline static int32 GetLength(const std::vector<T> *pContainer)
                {
                    int32 nLength = TSerializer<Private::TTypeTraits<int32>::Result>::GetLength(&nLength);

                    for (typename std::vector<T>::const_iterator it = pContainer->begin(); it != pContainer->end(); ++it)
                    {
                        nLength += TIOPolicySelector<typename Private::TTypeTraits<T>::TType, Private::TTypeTraits<T>::Result>::GetLength(*it);
                    }

                    return nLength;
                }

                // map
                template<typename TKey, typename T>
                inline static bool Save(XStream::THandle hStream, const std::map<TKey, T> *pContainer)
                {
                    int32 nSize = static_cast<int32>(pContainer->size());
                    if (nSize > nMaxContainerSize)
                    {
                        assert(!"[TSerializer<Private::tcSTLContainer>::Save()] std::map size is too big.");
                        return false;
                    }

                    if (TSerializer<Private::TTypeTraits<int32>::Result>::Save(hStream, &nSize) == false)
                    {
                        return false;
                    }

                    for (typename std::map<TKey, T>::const_iterator it = pContainer->begin(); it != pContainer->end(); ++it)
                    {
                        if (TIOPolicySelector<typename Private::TTypeTraits<TKey>::TType, Private::TTypeTraits<TKey>::Result>::Save(hStream, it->first) == false)
                        {
                            return false;
                        }
                        if (TIOPolicySelector<typename Private::TTypeTraits<T>::TType, Private::TTypeTraits<T>::Result>::Save(hStream, it->second) == false)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                template<typename TKey, typename T>
                inline static bool Load(XStream::THandle hStream, std::map<TKey, T> *pContainer)
                {
                    int32 nSize = 0;
                    if (TSerializer<Private::TTypeTraits<int32>::Result>::Load(hStream, &nSize) == false)
                    {
                        return false;
                    }

                    if (nSize > nMaxContainerSize)
                    {
                        assert(!"[TSerializer<Private::tcSTLContainer>::Load()] std::map size is too big.");
                        return false;
                    }

                    for (int i = 0; i < nSize; ++i)
                    {
                        typename Private::TTypeTraits<TKey>::TType KeyItem = Private::TTypeTraits<TKey>::Create();
                        if (TIOPolicySelector<typename Private::TTypeTraits<TKey>::TType, Private::TTypeTraits<TKey>::Result>::Load(hStream, KeyItem) == false)
                        {
                            return false;
                        }
                        typename Private::TTypeTraits<T>::TType Item = Private::TTypeTraits<T>::Create();
                        if (TIOPolicySelector<typename Private::TTypeTraits<T>::TType, Private::TTypeTraits<T>::Result>::Load(hStream, Item) == false)
                        {
                            return false;
                        }
                        if (pContainer->insert(typename std::map<TKey, T>::value_type(KeyItem, Item)).second == false)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                template<typename TKey, typename T>
                inline static int32 GetLength(const std::map<TKey, T> *pContainer)
                {
                    int32 nLength = TSerializer<Private::TTypeTraits<int32>::Result>::GetLength(&nLength);

                    for (typename std::map<TKey, T>::const_iterator it = pContainer->begin(); it != pContainer->end(); ++it)
                    {
                        nLength += TIOPolicySelector<typename Private::TTypeTraits<TKey>::TType, Private::TTypeTraits<TKey>::Result>::GetLength(it->first);
                        nLength += TIOPolicySelector<typename Private::TTypeTraits<T>::TType, Private::TTypeTraits<T>::Result>::GetLength(it->second);
                    }

                    return nLength;
                }
            };

            template<>
            struct TSerializer<Private::tcUserClass>
            {
                // user defined messages
                inline static bool Save(XStream::THandle hStream, const IMessage *pData)
                {
                    bool bSubMessageFlag = const_cast<IMessage *>(pData)->GetSubMessageFlag();
                    const_cast<IMessage *>(pData)->SetSubMessageFlag(true);
                    if (const_cast<IMessage *>(pData)->Save(hStream) == false)
                    {
                        const_cast<IMessage *>(pData)->SetSubMessageFlag(bSubMessageFlag);
                        return false;
                    }
                    const_cast<IMessage *>(pData)->SetSubMessageFlag(bSubMessageFlag);

                    return true;
                }
                inline static bool Load(XStream::THandle hStream, IMessage *pData)
                {
                    bool bSubMessageFlag = const_cast<IMessage *>(pData)->GetSubMessageFlag();
                    const_cast<IMessage *>(pData)->SetSubMessageFlag(true);
                    if (pData->Load(hStream) == false)
                    {
                        const_cast<IMessage *>(pData)->SetSubMessageFlag(bSubMessageFlag);
                        return false;
                    }
                    const_cast<IMessage *>(pData)->SetSubMessageFlag(bSubMessageFlag);

                    return true;
                }
                inline static int32 GetLength(const IMessage *pData)
                {
                    bool bSubMessageFlag = const_cast<IMessage *>(pData)->GetSubMessageFlag();
                    const_cast<IMessage *>(pData)->SetSubMessageFlag(true);
                    int32 nLength = const_cast<IMessage *>(pData)->GetLength();
                    const_cast<IMessage *>(pData)->SetSubMessageFlag(bSubMessageFlag);

                    return nLength;
                }
                inline static bool Save(XStream::THandle hStream, const IMessage &Data)
                {
                    return Save(hStream, &Data);
                }
                inline static bool Load(XStream::THandle hStream, IMessage &Data)
                {
                    return Load(hStream, &Data);
                }
                inline static int32 GetLength(const IMessage &Data)
                {
                    return GetLength(&Data);
                }

                // array of user defined messages
                inline static bool Save(XStream::THandle hStream, const IMessage *pData, int32 nArraySize)
                {
                    for (int32 i = 0; i < nArraySize; ++i)
                    {
                        if (Save(hStream, &pData[i]) == false)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                inline static bool Load(XStream::THandle hStream, IMessage *pData, int32 nArraySize)
                {
                    for (int32 i = 0; i < nArraySize; ++i)
                    {
                        if (Load(hStream, &pData[i]) == false)
                        {
                            return false;
                        }
                    }

                    return true;
                }
                inline static int32 GetLength(const IMessage *pData, int32 nArraySize)
                {
                    int32 nLength = 0;

                    for (int32 i = 0; i < nArraySize; ++i)
                    {
                        nLength += GetLength(&pData[i]);
                    }

                    return nLength;
                }
            };
        }   // namespace BER

        template<typename T>
        struct TSerializer : public BER::TSerializer<Private::TTypeTraits<T>::Result>
        {
        };

        // non-array
        template<typename T>
        bool Save(XStream::THandle hStream, const T *pData)
        {
            return TSerializer<T>::Save(hStream, pData);
        }
        template<typename T>
        bool Load(XStream::THandle hStream, T *pData)
        {
            return TSerializer<T>::Load(hStream, pData);
        }
        template<typename T>
        int32 GetLength(const T *pData)
        {
            return TSerializer<T>::GetLength(pData);
        }

        // array
        template<typename T>
        bool Save(XStream::THandle hStream, const T *pData, int32 nArraySize)
        {
            return TSerializer<T>::Save(hStream, pData, nArraySize);
        }
        template<typename T>
        bool Load(XStream::THandle hStream, T *pData, int32 nArraySize)
        {
            return TSerializer<T>::Load(hStream, pData, nArraySize);
        }
        template<typename T>
        int32 GetLength(const T *pData, int32 nArraySize)
        {
            return TSerializer<T>::GetLength(pData, nArraySize);
        }
    }   // namespace EncodingRules
}   // namespace XDR

#endif
