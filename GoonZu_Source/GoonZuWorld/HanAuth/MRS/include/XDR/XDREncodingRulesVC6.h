//
// XDR: neXt generation Data Representation
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Implementations of XDR Transfer Syntax.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XDREncodingRulesVC6__
#define __XDREncodingRulesVC6__

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
        // non-array
        inline bool Save(XStream::THandle hStream, IMessage *pData)
        {
            bool bSubMessageFlag = pData->GetSubMessageFlag();
            pData->SetSubMessageFlag(true);
            if (pData->Save(hStream) == false)
            {
                pData->SetSubMessageFlag(bSubMessageFlag);
                return false;
            }
            pData->SetSubMessageFlag(bSubMessageFlag);

            return true;
        }
        inline bool Load(XStream::THandle hStream, IMessage *pData)
        {
            bool bSubMessageFlag = pData->GetSubMessageFlag();
            pData->SetSubMessageFlag(true);
            if (pData->Load(hStream) == false)
            {
                pData->SetSubMessageFlag(bSubMessageFlag);
                return false;
            }
            pData->SetSubMessageFlag(bSubMessageFlag);

            return true;
        }
        inline int32 GetLength(IMessage *pData)
        {
            bool bSubMessageFlag = pData->GetSubMessageFlag();
            pData->SetSubMessageFlag(true);
            int32 nLength = pData->GetLength();
            pData->SetSubMessageFlag(bSubMessageFlag);

            return nLength;
        }

        // array
        inline bool Save(XStream::THandle hStream, IMessage *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                bool bSubMessageFlag = pData[i].GetSubMessageFlag();
                pData[i].SetSubMessageFlag(true);
                if (pData[i].Save(hStream) == false)
                {
                    pData[i].SetSubMessageFlag(bSubMessageFlag);
                    return false;
                }
                pData[i].SetSubMessageFlag(bSubMessageFlag);
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, IMessage *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                bool bSubMessageFlag = pData[i].GetSubMessageFlag();
                pData[i].SetSubMessageFlag(true);
                if (pData[i].Load(hStream) == false)
                {
                    pData[i].SetSubMessageFlag(bSubMessageFlag);
                    return false;
                }
                pData[i].SetSubMessageFlag(bSubMessageFlag);
            }

            return true;
        }
        inline int32 GetLength(IMessage *pData, int32 nArraySize)
        {
            int32 nLength = 0;

            for (int i = 0; i < nArraySize; ++i)
            {
                bool bSubMessageFlag = pData[i].GetSubMessageFlag();
                pData[i].SetSubMessageFlag(true);
                nLength += pData[i].GetLength();
                pData[i].SetSubMessageFlag(bSubMessageFlag);
            }

            return nLength;
        }

        // bool type, non-array
        inline bool Save(XStream::THandle hStream, const bool *pData)
        {
            bool Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(bool)) != sizeof(bool))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, bool *pData)
        {
            bool Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(bool)) != sizeof(bool))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const bool *pData)
        {
            return sizeof(bool);
        }

        // bool type, array
        inline bool Save(XStream::THandle hStream, const bool *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (XStream::Write(hStream, &pData[i], sizeof(bool)) != sizeof(bool))
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, bool *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (XStream::Read(hStream, &pData[i], sizeof(bool)) != sizeof(bool))
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const bool *pData, int32 nArraySize)
        {
            return sizeof(bool) * nArraySize;
        }

        // char type, non-array
        inline bool Save(XStream::THandle hStream, const char *pData)
        {
            char Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(char)) != sizeof(char))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, char *pData)
        {
            char Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(char)) != sizeof(char))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const char *pData)
        {
            return sizeof(char);
        }

        // char type, array
        inline bool Save(XStream::THandle hStream, const char *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, char *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const char *pData, int32 nArraySize)
        {
            return sizeof(char) * nArraySize;
        }

        // int16 type, non-array
        inline bool Save(XStream::THandle hStream, const int16 *pData)
        {
            int16 Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(int16)) != sizeof(int16))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, int16 *pData)
        {
            int16 Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(int16)) != sizeof(int16))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const int16 *pData)
        {
            return sizeof(int16);
        }

        // int16 type, array
        inline bool Save(XStream::THandle hStream, const int16 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, int16 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const int16 *pData, int32 nArraySize)
        {
            return sizeof(int16) * nArraySize;
        }

        // int32 type, non-array
        inline bool Save(XStream::THandle hStream, const int32 *pData)
        {
            int32 Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(int32)) != sizeof(int32))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, int32 *pData)
        {
            int32 Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(int32)) != sizeof(int32))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const int32 *pData)
        {
            return sizeof(int32);
        }

        // int32 type, array
        inline bool Save(XStream::THandle hStream, const int32 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, int32 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const int32 *pData, int32 nArraySize)
        {
            return sizeof(int32) * nArraySize;
        }

        // int64 type, non-array
        inline bool Save(XStream::THandle hStream, const int64 *pData)
        {
            int64 Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(int64)) != sizeof(int64))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, int64 *pData)
        {
            int64 Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(int64)) != sizeof(int64))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const int64 *pData)
        {
            return sizeof(int64);
        }

        // int64 type, array
        inline bool Save(XStream::THandle hStream, const int64 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, int64 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const int64 *pData, int32 nArraySize)
        {
            return sizeof(int64) * nArraySize;
        }

        // uchar type, non-array
        inline bool Save(XStream::THandle hStream, const uchar *pData)
        {
            uchar Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(uchar)) != sizeof(uchar))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, uchar *pData)
        {
            uchar Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(uchar)) != sizeof(uchar))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const uchar *pData)
        {
            return sizeof(uchar);
        }

        // uchar type, array
        inline bool Save(XStream::THandle hStream, const uchar *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, uchar *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const uchar *pData, int32 nArraySize)
        {
            return sizeof(uchar) * nArraySize;
        }

        // uint16 type, non-array
        inline bool Save(XStream::THandle hStream, const uint16 *pData)
        {
            uint16 Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(uint16)) != sizeof(uint16))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, uint16 *pData)
        {
            uint16 Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(uint16)) != sizeof(uint16))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const uint16 *pData)
        {
            return sizeof(uint16);
        }

        // uint16 type, array
        inline bool Save(XStream::THandle hStream, const uint16 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, uint16 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const uint16 *pData, int32 nArraySize)
        {
            return sizeof(uint16) * nArraySize;
        }

        // uint32 type, non-array
        inline bool Save(XStream::THandle hStream, const uint32 *pData)
        {
            uint32 Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(uint32)) != sizeof(uint32))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, uint32 *pData)
        {
            uint32 Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(uint32)) != sizeof(uint32))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const uint32 *pData)
        {
            return sizeof(uint32);
        }

        // uint32 type, array
        inline bool Save(XStream::THandle hStream, const uint32 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, uint32 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const uint32 *pData, int32 nArraySize)
        {
            return sizeof(uint32) * nArraySize;
        }

        // uint64 type, non-array
        inline bool Save(XStream::THandle hStream, const uint64 *pData)
        {
            uint64 Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(uint64)) != sizeof(uint64))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, uint64 *pData)
        {
            uint64 Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(uint64)) != sizeof(uint64))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const uint64 *pData)
        {
            return sizeof(uint64);
        }

        // uint64 type, array
        inline bool Save(XStream::THandle hStream, const uint64 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, uint64 *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const uint64 *pData, int32 nArraySize)
        {
            return sizeof(uint64) * nArraySize;
        }

        // float type, non-array
        inline bool Save(XStream::THandle hStream, const float *pData)
        {
            float Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(float)) != sizeof(float))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, float *pData)
        {
            float Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(float)) != sizeof(float))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const float *pData)
        {
            return sizeof(float);
        }

        // float type, array
        inline bool Save(XStream::THandle hStream, const float *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, float *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const float *pData, int32 nArraySize)
        {
            return sizeof(float) * nArraySize;
        }

        // double type, non-array
        inline bool Save(XStream::THandle hStream, const double *pData)
        {
            double Data = HostToNetwork(*pData);

            if (XStream::Write(hStream, &Data, sizeof(double)) != sizeof(double))
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, double *pData)
        {
            double Data = 0;
            if (XStream::Read(hStream, &Data, sizeof(double)) != sizeof(double))
            {
                return false;
            }
            *pData = NetworkToHost(Data);

            return true;
        }
        inline int32 GetLength(const double *pData)
        {
            return sizeof(double);
        }

        // double type, array
        inline bool Save(XStream::THandle hStream, const double *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Save(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, double *pData, int32 nArraySize)
        {
            for (int i = 0; i < nArraySize; ++i)
            {
                if (Load(hStream, &pData[i]) == false)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(const double *pData, int32 nArraySize)
        {
            return sizeof(double) * nArraySize;
        }

        // non-array
        inline bool Save(XStream::THandle hStream, std::string *pData)
        {
            int32 nLength = static_cast<int32>(pData->size());
            if (nLength > nMaxContainerSize)
            {
                assert(!"[Save()] std::string length is too long.");
                return false;
            }
            if (Save(hStream, &nLength) == false)
            {
                return false;
            }
            if (XStream::Write(hStream, pData->c_str(), nLength) != nLength)
            {
                return false;
            }

            return true;
        }
        inline bool Load(XStream::THandle hStream, std::string *pData)
        {
            int nLength = 0;
            if (Load(hStream, &nLength) == false)
            {
                return false;
            }
            if (nLength > nMaxContainerSize)
            {
                assert(!"[Load()] std::string length is too long.");
                return false;
            }
            if (nLength > 0)
            {
                pData->resize(nLength);

                std::string &strTemp = *pData;
                if (XStream::Read(hStream, &strTemp[0], nLength) != nLength)
                {
                    return false;
                }
            }

            return true;
        }
        inline int32 GetLength(std::string *pData)
        {
            return static_cast<int32>(sizeof(int32) + pData->size());
        }
    }   // namespace EncodingRules
}   // namespace XDR

#endif