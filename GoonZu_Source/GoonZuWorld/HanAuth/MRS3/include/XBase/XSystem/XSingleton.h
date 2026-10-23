//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XSingleton__
#define __XSingleton__

#include "XSyncObject.h"
#include <stdexcept>

namespace XSystem
{
    namespace XTL
    {
        template<typename T>
        class Singleton
        {
        private:
            static volatile T *instance_;
            static bool destroyed_;
            static Threading::CriticalSection lock_;

        public:
            inline static T &Instance(void)
            {
                if (instance_ == NULL)
                {
                    MakeInstance();
                }

                return const_cast<T &>(*instance_);
            }

        private:
            Singleton(void)
            {
            }

            static void MakeInstance(void)
            {
                Threading::ScopedLock<Threading::CriticalSection> lock(lock_);

                if (instance_ == NULL)
                {
                    if (destroyed_ == true)
                    {
                        OnDeadReference();
                    }

                    instance_ = new T;
                    if (NULL == instance_)
                    {
                        throw std::bad_alloc();
                    }

                    atexit(DestroySingleton);
                }
            }
            static void DestroySingleton(void)
            {
                delete instance_;
                instance_ = NULL;

                destroyed_ = true;
            }
            static void OnDeadReference(void)
            {
                throw std::logic_error("[XSystem::XTL::Singleton] Dead reference detected.");
            }
        };

        template<typename T> volatile T *Singleton<T>::instance_ = NULL;
        template<typename T> bool Singleton<T>::destroyed_ = false;
        template<typename T> Threading::CriticalSection Singleton<T>::lock_;

        template<typename T>
        struct SingletonStatic
        {
        private:
            class InternalObjectCreator
            {
            public:
                InternalObjectCreator(void)
                {
                    SingletonStatic<T>::Instance();
                }
                inline void DoNothing(void) const
                {
                }
            };

            static InternalObjectCreator objectCreator_;

        public:
            inline static T &Instance(void)
            {
                static T obj;
                objectCreator_.DoNothing();

                return obj;
            }

        private:
            SingletonStatic(void)
            {
            }
        };

        template<typename T> typename SingletonStatic<T>::InternalObjectCreator SingletonStatic<T>::objectCreator_;
    }
}

#endif
