//
// XDR: neXt generation Data Representation
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Definitions of XDR primitive data types and XDR::IMessage interface.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XDRBase__
#define __XDRBase__

#include <XBase/XStream.h>
#include <vector>
#include <cassert>
#ifdef _UNIX_
    #include <stdint.h>
#endif

typedef char int8;
typedef short int16;
typedef int int32;
#ifdef _UNIX_
    typedef int64_t int64;
#else
    typedef __int64 int64;
#endif
typedef unsigned char uchar;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
#ifdef _UNIX_
    typedef uint64_t uint64;
#else
    typedef unsigned __int64 uint64;
#endif

const int32 nInvalidMessageID = 0xFFFFFFFF;
#define INVALID_MESSAGE_ID nInvalidMessageID
#define __messageid(__ClassName__) __ClassName__::tagMsgID

namespace XDR
{
    const int nMaxContainerSize = 1024 * 100;

    class IMessage
    {
    public:
        virtual ~IMessage(void) {}

        virtual void Clear(void) = 0;
        virtual int GetID(void) const = 0;
        virtual const char *GetName(void) const = 0;
        virtual int GetLength(void) = 0;
        virtual bool Save(XStream::THandle hStream) = 0;
        virtual bool Load(XStream::THandle hStream) = 0;

        virtual void SetSubMessageFlag(bool bSubMessageFlag) = 0;
        virtual bool GetSubMessageFlag(void) const = 0;
    };

    template<typename ResultType, ResultType DefaultValue = ResultType()>
    class Dispatcher
    {
    public:
        typedef ResultType (__stdcall *TCallback)(XStream::THandle, void *, void *);

    private:
        typedef std::pair<TCallback, void *> TFunctor;
        typedef std::vector<std::pair<int, TFunctor> > TMessageMap;

    private:
        TMessageMap m_MsgMap;
        TFunctor m_DefaultCallback;

    public:
        bool AddHandler(int nMsgID, const TCallback &Callback, void *pData)
        {
            for (TMessageMap::const_iterator it = m_MsgMap.begin(); it != m_MsgMap.end(); ++it)
            {
                if ((*it).first == nMsgID)
                {
                    assert(!"nMsgID is already exists.");
                    return false;
                }
            }

            m_MsgMap.push_back(std::pair<int, TFunctor>(nMsgID, TFunctor(Callback, pData)));

            return true;
        }

        void SetDefaultHandler(const TCallback &Callback, void *pData)
        {
            m_DefaultCallback.first = Callback;
            m_DefaultCallback.second = pData;
        }

        ResultType DoDispatch(XStream::THandle hStream, void *pAdditionalData)
        {
            int nMsgID = 0;
            if (XStream::Peek(hStream, &nMsgID, sizeof(int)) != sizeof(int))
            {
                assert(!"Invalid stream. Stream length is too short.");
                return DefaultValue;
            }
            nMsgID = NetworkToHost(nMsgID);

            for (TMessageMap::const_iterator it = m_MsgMap.begin(); it != m_MsgMap.end(); ++it)
            {
                if ((*it).first == nMsgID)
                {
                    const TFunctor &Functor = (*it).second;
                    return (*Functor.first)(hStream, Functor.second, pAdditionalData);
                }
            }

            if (m_DefaultCallback.first != NULL)
            {
                return (*m_DefaultCallback.first)(hStream, m_DefaultCallback.second, pAdditionalData);
            }

            return DefaultValue;
        }
    };
}

#endif
