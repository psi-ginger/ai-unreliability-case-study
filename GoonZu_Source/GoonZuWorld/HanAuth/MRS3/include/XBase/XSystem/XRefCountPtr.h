//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XRefCountPtr__
#define __XRefCountPtr__

#include <utility>

namespace XSystem
{
    namespace XTL
    {
        template<typename T>
        class RefCountPtr
        {
        private:
            typedef T* StoredType;
            typedef T* PointerType;
            typedef T const* ConstPointerType;
            typedef T& ReferenceType;
            typedef T const& ConstReferenceType;

        private:
            StoredType pointee_;

        public:
            RefCountPtr(void) : pointee_(NULL)
            {
            }
            RefCountPtr(const StoredType &p) : pointee_(p)
            {
                if (pointee_ != NULL)
                {
                    pointee_->AddRef();
                }
            }
#if (_MSC_VER >= 1301) || defined(_UNIX_)
            RefCountPtr(const RefCountPtr &rhs) : pointee_(rhs.pointee_)
            {
                if (pointee_ != NULL)
                {
                    pointee_->AddRef();
                }
            }
            template<typename U>
            RefCountPtr(RefCountPtr<U> &rhs) : pointee_(rhs.GetRawPtr())
            {
                if (pointee_ != NULL)
                {
                    pointee_->AddRef();
                }
            }
#endif
            template<typename U>
            RefCountPtr(const RefCountPtr<U> &rhs) : pointee_(const_cast<T *>(rhs.GetRawPtr()))
            {
                if (pointee_ != NULL)
                {
                    pointee_->AddRef();
                }
            }
            ~RefCountPtr(void)
            {
                if (pointee_ != NULL)
                {
                    pointee_->Release();
                }
            }

            void Swap(RefCountPtr &rhs)
            {
                std::swap(pointee_, rhs.pointee_);
            }

            PointerType GetRawPtr(void)
            {
                return pointee_;
            }
            ConstPointerType GetRawPtr(void) const
            {
                return pointee_;
            }

#if (_MSC_VER >= 1301) || defined(_UNIX_)
            RefCountPtr &operator=(const RefCountPtr &rhs)
            {
                RefCountPtr temp(rhs);
                temp.Swap(*this);

                return *this;
            }
            template<typename U>
            RefCountPtr &operator=(RefCountPtr<U> &rhs)
            {
                RefCountPtr temp(rhs);
                temp.Swap(*this);

                return *this;
            }
#endif
            template<typename U>
            RefCountPtr &operator=(const RefCountPtr<U> &rhs)
            {
                RefCountPtr temp(rhs);
                temp.Swap(*this);

                return *this;
            }

            PointerType operator->()
            {
                return GetRawPtr();
            }
            ConstPointerType operator->() const
            {
                return GetRawPtr();
            }

            ReferenceType operator*()
            {
                return *GetRawPtr();
            }
            ConstReferenceType operator*() const
            {
                return *GetRawPtr();
            }
        };
    }
}

#endif
