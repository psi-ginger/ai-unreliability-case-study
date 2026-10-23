//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XRawPtrPool__
#define __XRawPtrPool__

#include "XSyncObject.h"
#include "XNonCopyable.h"
#include <vector>

namespace XSystem
{
    namespace XTL
    {
        template<size_t BlockSizeT>
        class RawPtrPool : private NonCopyable
        {
        private:
            std::vector<void *> pool_;
            Threading::CriticalSection lock_;

        public:
            RawPtrPool(void)
            {
            }
            ~RawPtrPool(void)
            {
                try
                {
                    Threading::ScopedLock<Threading::CriticalSection> lock(lock_);

                    for (std::vector<void *>::iterator it = pool_.begin(); it != pool_.end(); ++it)
                    {
                        free(*it);
                    }
                    pool_.clear();
                }
                catch (...)
                {
                }
            }

            void *Alloc(void)
            {
                void *p = NULL;

                Threading::ScopedLock<Threading::CriticalSection> lock(lock_);
                if (pool_.empty() == true)
                {
                    p = malloc(BlockSizeT);
                    if (p == NULL)
                    {
                        throw std::bad_alloc();
                    }
                }
                else
                {
                    p = pool_.back();
                    pool_.pop_back();
                }

                return p;
            }
            void Free(void *p)
            {
                if (p == NULL)
                {
                    return;
                }

#ifdef _DEBUG
                memset(p, 0xCC, BlockSizeT);
#endif
                Threading::ScopedLock<Threading::CriticalSection> lock(lock_);
                pool_.push_back(p);
            }
        };
    }
}

#endif
