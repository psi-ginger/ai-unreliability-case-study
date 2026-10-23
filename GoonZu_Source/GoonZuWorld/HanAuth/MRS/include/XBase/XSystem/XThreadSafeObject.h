//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XThreadSafeObject__
#define __XThreadSafeObject__

#include "XSyncObject.h"
#include "XNonCopyable.h"

namespace XSystem
{
    namespace XTL
    {
#if (_MSC_VER >= 1301) || defined(_UNIX_)
        namespace Private
        {
            template<typename T>
            struct AddPointer
            {
                typedef T* PointerType;
                typedef T PointeeType;
            };
            template<typename T>
            struct AddPointer<T *>
            {
                typedef T* PointerType;
                typedef T PointeeType;
            };
        }

        template<typename T>
        class ThreadSafeObject : private NonCopyable
        {
        private:
            typename Private::AddPointer<T>::PointeeType obj_;
            Threading::CriticalSection lock_;

        public:
            ThreadSafeObject(void)
            {
            }
            ~ThreadSafeObject(void)
            {
            }

            typename Private::AddPointer<T>::PointerType Lock(void)
            {
                lock_.Enter();

                return &obj_;
            }
            void Unlock(void)
            {
                lock_.Leave();
            }
        };

        template<typename T>
        class RWThreadSafeObject : private NonCopyable
        {
        private:
            typename Private::AddPointer<T>::PointeeType obj_;
            Threading::RWCriticalSection lock_;

        public:
            RWThreadSafeObject(void)
            {
            }
            ~RWThreadSafeObject(void)
            {
            }

            typename Private::AddPointer<T>::PointerType ReadLock(void)
            {
                lock_.ReadLock();

                return &obj_;
            }
            void ReadUnlock(void)
            {
                lock_.ReadUnlock();
            }
            typename Private::AddPointer<T>::PointerType WriteLock(void)
            {
                lock_.WriteLock();

                return &obj_;
            }
            void WriteUnlock(void)
            {
                lock_.WriteUnlock();
            }
        };
#else
        template<typename T>
        class ThreadSafeObject : private NonCopyable
        {
        private:
            typename T obj_;
            Threading::CriticalSection lock_;

        public:
            ThreadSafeObject(void)
            {
            }
            ~ThreadSafeObject(void)
            {
            }

            typename T *Lock(void)
            {
                lock_.Enter();

                return &obj_;
            }
            void Unlock(void)
            {
                lock_.Leave();
            }
        };

        template<typename T>
        class RWThreadSafeObject : private NonCopyable
        {
        private:
            typename T obj_;
            Threading::RWCriticalSection lock_;

        private:
            RWThreadSafeObject(const RWThreadSafeObject &rhs);
            RWThreadSafeObject &operator=(const RWThreadSafeObject &rhs);

        public:
            RWThreadSafeObject(void)
            {
            }
            ~RWThreadSafeObject(void)
            {
            }

            typename T *ReadLock(void)
            {
                lock_.ReadLock();

                return &obj_;
            }
            void ReadUnlock(void)
            {
                lock_.ReadUnlock();
            }
            typename T *WriteLock(void)
            {
                lock_.WriteLock();

                return &obj_;
            }
            void WriteUnlock(void)
            {
                lock_.WriteUnlock();
            }
        };
#endif
    }
}

#endif
