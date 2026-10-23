//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XStaticObject__
#define __XStaticObject__

#include "XInterlocked.h"

namespace XSystem
{
    namespace XTL
    {
        template<typename T>
        class StaticObject
        {
        private:
            static volatile unsigned int instanceCount_;
            static volatile T *instance_;

        public:
            StaticObject(void)
            {
                if (Threading::Interlocked::Increment(&instanceCount_) == 1)
                {
                    instance_ = new T;
                }
            }
            ~StaticObject(void)
            {
                if (Threading::Interlocked::Decrement(&instanceCount_) == 0)
                {
                    delete instance_;
                    instance_ = NULL;
                }
            }

            T *operator->()
            {
                return const_cast<T *>(instance_);
            }

            const T *operator->() const
            {
                return instance_;
            }

            static T *Instance(void)
            {
                return const_cast<T *>(instance_);
            }
        };

        template<typename T> volatile unsigned int StaticObject<T>::instanceCount_ = 0;
        template<typename T> volatile T *StaticObject<T>::instance_ = 0;
    }
}

#endif
