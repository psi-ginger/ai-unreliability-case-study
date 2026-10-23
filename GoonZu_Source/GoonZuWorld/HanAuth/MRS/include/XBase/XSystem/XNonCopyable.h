//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XNonCopyable__
#define __XNonCopyable__

namespace XSystem
{
    namespace XTL
    {
        class NonCopyable
        {
        protected:
            NonCopyable(void)
            {
            }
            ~NonCopyable(void)
            {
            }

        private:
            NonCopyable(const NonCopyable &);
            const NonCopyable &operator=(const NonCopyable &);
        };
    }
}

#endif
