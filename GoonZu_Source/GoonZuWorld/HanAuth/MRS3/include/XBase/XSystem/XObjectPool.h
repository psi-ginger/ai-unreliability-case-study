//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XObjectPool__
#define __XObjectPool__

#include "XRawPtrPool.h"
#include "XNonCopyable.h"

namespace XSystem
{
    namespace XTL
    {
        template<typename T>
        class ObjectPool : private NonCopyable
        {
        private:
            RawPtrPool<sizeof(T)> pool_;

        public:
            ObjectPool(void)
            {
            }
            ~ObjectPool(void)
            {
            }

            T *Alloc(void)
            {
                T *result = NULL;
                void *p = pool_.Alloc();
                try
                {
                    result = new(p) T;
                }
                catch (...)
                {
                    pool_.Free(p);
                    throw;
                }

                return result;
            }
            void Free(T *p)
            {
                if (p == NULL)
                {
                    return;
                }

                p->~T();
                pool_.Free(p);
            }
        };
    }
}

#endif
