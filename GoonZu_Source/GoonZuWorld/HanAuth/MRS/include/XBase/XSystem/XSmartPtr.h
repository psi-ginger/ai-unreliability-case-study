//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XSmartPtr__
#define __XSmartPtr__

#if defined(_MSC_VER) && _MSC_VER < 1300
    #include "XSmartPtrVC6.h"
#else
#include "XMPHeap.h"
#include "XInterlocked.h"
#include <algorithm>
#include <cassert>

namespace XSystem
{
    namespace XTL
    {
        namespace SmartPtrPolicies
        {
            namespace Storage
            {
                template<typename T>
                class DefaultStorage
                {
                public:
                    typedef T* StoredType;
                    typedef T const* ConstStoredType;
                    typedef T* PointerType;
                    typedef T const* ConstPointerType;
                    typedef T& ReferenceType;
                    typedef T const& ConstReferenceType;

                private:
                    StoredType pointee_;

                protected:
                    DefaultStorage(void) : pointee_(Default())
                    {
                    }
                    DefaultStorage(const DefaultStorage &)
                    {
                    }
                    template<typename U>
                    DefaultStorage(const DefaultStorage<U> &)
                    {
                    }
                    DefaultStorage(const StoredType &p) : pointee_(p)
                    {
                    }

                    void Swap(DefaultStorage &rhs)
                    {
                        std::swap(pointee_, rhs.pointee_);
                    }

                    PointerType GetPtr(void)
                    {
                        return pointee_;
                    }
                    ConstPointerType GetPtr(void) const
                    {
                        return pointee_;
                    }

                public:
                    // Accessors
                    friend inline PointerType GetRawPtr(const DefaultStorage &sp)
                    {
                        return sp.pointee_;
                    }
                    friend inline const StoredType &GetRawPtrRef(const DefaultStorage &sp)
                    {
                        return sp.pointee_;
                    }
                    friend inline StoredType &GetRawPtrRef(DefaultStorage &sp)
                    {
                        return sp.pointee_;
                    }

                    PointerType operator->() const
                    {
                        return pointee_;
                    }
                    ReferenceType operator*() const
                    {
                        return *pointee_;
                    }

                protected:
                    // Destroys the data stored
                    // (Destruction might be taken over by the OwnershipPolicy)
                    void Destroy(void)
                    {
                        delete pointee_;
                    }

                    // Default value to initialize the pointer
                    static StoredType Default(void)
                    {
                        return 0;
                    }
                };

                template<typename T>
                class ArrayStorage
                {
                public:
                    typedef T* StoredType;
                    typedef T const* ConstStoredType;
                    typedef T* PointerType;
                    typedef T const* ConstPointerType;
                    typedef T& ReferenceType;
                    typedef T const& ConstReferenceType;

                private:
                    StoredType pointee_;

                protected:
                    ArrayStorage(void) : pointee_(Default())
                    {
                    }
                    ArrayStorage(const ArrayStorage &)
                    {
                    }
                    template<typename U>
                    ArrayStorage(const ArrayStorage<U> &)
                    {
                    }
                    ArrayStorage(const StoredType &p) : pointee_(p)
                    {
                    }

                    void Swap(ArrayStorage &rhs)
                    {
                        std::swap(pointee_, rhs.pointee_);
                    }

                    PointerType GetPtr(void)
                    {
                        return pointee_;
                    }
                    ConstPointerType GetPtr(void) const
                    {
                        return pointee_;
                    }

                public:
                    // Accessors
                    friend inline PointerType GetRawPtr(const ArrayStorage &sp)
                    {
                        return sp.pointee_;
                    }
                    friend inline const StoredType &GetRawPtrRef(const ArrayStorage &sp)
                    {
                        return sp.pointee_;
                    }
                    friend inline StoredType &GetRawPtrRef(ArrayStorage &sp)
                    {
                        return sp.pointee_;
                    }

                    PointerType operator->() const
                    {
                        return pointee_;
                    }
                    ReferenceType operator*() const
                    {
                        return *pointee_;
                    }
                    ConstReferenceType operator[](int i) const
                    {
                        assert(i >= 0);
                        return GetPtr()[i];
                    }
                    ReferenceType operator[](int i)
                    {
                        assert(i >= 0);
                        return GetPtr()[i];
                    }

                protected:
                    // Destroys the data stored
                    // (Destruction might be taken over by the OwnershipPolicy)
                    void Destroy(void)
                    {
                        delete[] pointee_;
                    }

                    // Default value to initialize the pointer
                    static StoredType Default(void)
                    {
                        return 0;
                    }
                };
            }   // namespace Storage

            namespace Ownership
            {
                template<typename T>
                class RefCounted
                {
                private:
                    // Data
                    unsigned int *count_;

                protected:
                    RefCounted(void) : count_(new unsigned int)
                    {
                        assert(count_);
                        *count_ = 1;
                    }
                    RefCounted(const RefCounted &rhs) : count_(rhs.count_)
                    {
                    }
                    template<typename U>
                    RefCounted(const RefCounted<U> &rhs) : count_(reinterpret_cast<const RefCounted &>(rhs).count_)
                    {
                    }

                    T Clone(const T &Value)
                    {
                        ++*count_;

                        return Value;
                    }

                    bool Release(const T &)
                    {
                        if (--*count_ == 0)
                        {
                            delete count_;
                            count_ = NULL;

                            return true;
                        }

                        return false;
                    }

                    void Swap(RefCounted &rhs)
                    {
                        std::swap(count_, rhs.count_);
                    }
                };

                template<typename T>
                class RefCountedMT
                {
                private:
                    // Data
                    volatile unsigned int *count_;

                protected:
                    RefCountedMT(void) : count_(NULL)
                    {
                        count_ = static_cast<volatile unsigned int *>(MPHeap::Alloc(NULL, sizeof(unsigned int)));
                        if (count_ == NULL)
                        {
                            throw std::bad_alloc();
                        }
                        *count_ = 1;
                    }
                    RefCountedMT(const RefCountedMT &rhs) : count_(rhs.count_)
                    {
                    }
                    template<typename U>
                    RefCountedMT(const RefCountedMT<U> &rhs) : count_(reinterpret_cast<const RefCountedMT &>(rhs).count_)
                    {
                    }

                    T Clone(const T &Value)
                    {
                        Threading::Interlocked::Increment(count_);

                        return Value;
                    }

                    bool Release(const T &)
                    {
                        if (Threading::Interlocked::Decrement(count_) == 0)
                        {
                            MPHeap::Free(NULL, const_cast<unsigned int *>(count_));
                            count_ = NULL;

                            return true;
                        }

                        return false;
                    }

                    void Swap(RefCountedMT &rhs)
                    {
                        std::swap(count_, rhs.count_);
                    }
                };
            }   // namespace Ownership

            namespace Checking
            {
                template<typename T>
                class NoCheck
                {
                protected:
                    NoCheck(void)
                    {
                    }
                    template<typename U>
                    NoCheck(const NoCheck<U> &)
                    {
                    }
                    NoCheck(const NoCheck &)
                    {
                    }

                    static void OnDefault(const T &)
                    {
                    }
                    static void OnInit(const T &)
                    {
                    }
                    static void OnDereference(const T &)
                    {
                    }
                    static void Swap(NoCheck &)
                    {
                    }
                };

                // Checks the pointer before dereference
                template<typename T>
                class AssertCheck
                {
                protected:
                    AssertCheck(void)
                    {
                    }
                    template<typename U>
                    AssertCheck(const AssertCheck<U> &)
                    {
                    }
                    AssertCheck(const AssertCheck &)
                    {
                    }
                    template<typename U>
                    AssertCheck(const NoCheck<U> &)
                    {
                    }

                    static void OnDefault(const T &)
                    {
                    }
                    static void OnInit(const T &)
                    {
                    }
                    static void OnDereference(const T &value)
                    {
                        assert(value != 0);
                        (void)value;
                    }
                    static void Swap(AssertCheck &)
                    {
                    }
                };

                // Checks the pointer against zero upon initialization and before dereference
                // You can initialize an AssertCheckStrict with an AssertCheck
                template<typename T>
                class AssertCheckStrict
                {
                protected:
                    AssertCheckStrict(void)
                    {
                    }
                    template<typename U>
                    AssertCheckStrict(const AssertCheckStrict<U> &)
                    {
                    }
                    AssertCheckStrict(const AssertCheckStrict &)
                    {
                    }
                    template<typename U>
                    AssertCheckStrict(const AssertCheck<U> &)
                    {
                    }
                    template<typename U>
                    AssertCheckStrict(const NoCheck<U> &)
                    {
                    }

                    static void OnDefault(const T &value)
                    {
                        assert(value != 0);
                        (void)value;
                    }
                    static void OnInit(const T &value)
                    {
                        assert(value != 0);
                        (void)value;
                    }
                    static void OnDereference(const T &value)
                    {
                        assert(value != 0);
                        (void)value;
                    }
                    static void Swap(AssertCheckStrict &)
                    {
                    }
                };
            }   // namespace Checking
        }   // namespace SmartPtr

        // Default SmartPtr declaration
        template<typename T, template<typename> class OwnershipPolicyT = SmartPtrPolicies::Ownership::RefCountedMT,
                template<typename> class CheckingPolicyT = SmartPtrPolicies::Checking::AssertCheck,
                template<typename> class StoragePolicyT = SmartPtrPolicies::Storage::DefaultStorage> class SmartPtr;

        template<typename T, template<typename> class OwnershipPolicyT,
                template<typename> class CheckingPolicyT, template<typename> class StoragePolicyT>
        class SmartPtr : public StoragePolicyT<T>, public OwnershipPolicyT<typename StoragePolicyT<T>::PointerType>,
                        public CheckingPolicyT<typename StoragePolicyT<T>::StoredType>
        {
        private:
            typedef StoragePolicyT<T> StoragePolicyType;
            typedef OwnershipPolicyT<typename StoragePolicyT<T>::PointerType> OwnershipPolicyType;
            typedef CheckingPolicyT<typename StoragePolicyT<T>::StoredType> CheckingPolicyType;

        public:
            typedef typename StoragePolicyType::PointerType PointerType;
            typedef typename StoragePolicyType::ConstPointerType ConstPointerType;
            typedef typename StoragePolicyType::StoredType StoredType;
            typedef typename StoragePolicyType::ConstStoredType ConstStoredType;
            typedef typename StoragePolicyType::ReferenceType ReferenceType;
            typedef typename StoragePolicyType::ConstReferenceType ConstReferenceType;
            typedef const SmartPtr CopyArg;

            SmartPtr(void)
            {
                CheckingPolicyType::OnDefault(GetRawPtr(*this));
            }
            SmartPtr(const StoredType &p) : StoragePolicyType(p)
            {
                CheckingPolicyType::OnInit(GetRawPtr(*this));
            }
            SmartPtr(CopyArg &rhs) : StoragePolicyType(rhs), OwnershipPolicyType(rhs), CheckingPolicyType(rhs)
            {
                GetRawPtrRef(*this) = OwnershipPolicyType::Clone(GetRawPtrRef(rhs));
            }
            template<typename T1, template<typename> class OwnershipPolicyT1,
                    template<typename> class CheckingPolicyT1, template<typename> class StoragePolicyT1>
            SmartPtr(const SmartPtr<T1, OwnershipPolicyT1, CheckingPolicyT1, StoragePolicyT1> &rhs) : StoragePolicyType(rhs), OwnershipPolicyType(rhs), CheckingPolicyType(rhs)
            {
                GetRawPtrRef(*this) = OwnershipPolicyType::Clone(GetRawPtrRef(rhs));
            }
            template<typename T1, template<typename> class OwnershipPolicyT1,
                    template<typename> class CheckingPolicyT1, template<typename> class StoragePolicyT1>
            SmartPtr(SmartPtr<T1, OwnershipPolicyT1, CheckingPolicyT1, StoragePolicyT1> &rhs) : StoragePolicyType(rhs), OwnershipPolicyType(rhs), CheckingPolicyType(rhs)
            {
                GetRawPtrRef(*this) = OwnershipPolicyType::Clone(GetRawPtrRef(rhs));
            }
            ~SmartPtr(void)
            {
                if (OwnershipPolicyType::Release(GetRawPtr(*static_cast<StoragePolicyType *>(this))) == true)
                {
                    StoragePolicyType::Destroy();
                }
            }

            void Swap(SmartPtr &rhs)
            {
                OwnershipPolicyType::Swap(rhs);
                CheckingPolicyType::Swap(rhs);
                StoragePolicyType::Swap(rhs);
            }

            friend inline void Release(SmartPtr &sp, typename StoragePolicyType::StoredType &p)
            {
                p = GetRawPtrRef(sp);
                GetRawPtrRef(sp) = StoragePolicyType::Default();
            }

            friend inline void Reset(SmartPtr &sp, typename StoragePolicyType::StoredType p)
            {
                SmartPtr(p).Swap(sp);
            }

            operator PointerType() const
            {
                return GetRawPtr(*this);
            }
            SmartPtr &operator=(CopyArg &rhs)
            {
                SmartPtr temp(rhs);
                temp.Swap(*this);

                return *this;
            }
            template<typename T1, template<typename> class OwnershipPolicyT1,
                    template<typename> class CheckingPolicyT1, template<typename> class StoragePolicyT1>
            SmartPtr &operator=(const SmartPtr<T1, OwnershipPolicyT1, CheckingPolicyT1, StoragePolicyT1> &rhs)
            {
                SmartPtr temp(rhs);
                temp.Swap(*this);

                return *this;
            }
            template<typename T1, template<typename> class OwnershipPolicyT1,
                    template<typename> class CheckingPolicyT1, template<typename> class StoragePolicyT1>
            SmartPtr &operator=(SmartPtr<T1, OwnershipPolicyT1, CheckingPolicyT1, StoragePolicyT1> &rhs)
            {
                SmartPtr temp(rhs);
                temp.Swap(*this);

                return *this;
            }

            PointerType operator->()
            {
                CheckingPolicyType::OnDereference(GetRawPtrRef(*this));

                return StoragePolicyType::operator->();
            }
            ConstPointerType operator->() const
            {
                CheckingPolicyType::OnDereference(GetRawPtrRef(*this));

                return StoragePolicyType::operator->();
            }
            ReferenceType operator*()
            {
                CheckingPolicyType::OnDereference(GetRawPtrRef(*this));

                return StoragePolicyType::operator*();
            }
            ConstReferenceType operator*() const
            {
                CheckingPolicyType::OnDereference(GetRawPtrRef(*this));

                return StoragePolicyType::operator*();
            }

            bool operator!() const // Enables "if (!sp) ..."
            {
                return GetRawPtr(*this) == 0;
            }
        };
    }
}
#endif

#endif
