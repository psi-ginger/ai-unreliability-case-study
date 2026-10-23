//
// XSystem
// Copyright (C) NHN Corporation 1999-2005. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __XSmartPtrVC6__
#define __XSmartPtrVC6__

#include "XMSVC6Helpers.h"
#include "XMPHeap.h"
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
                template<class T>
                class DefaultStorage
                {
                public:
                    typedef T* StoredType;
                    typedef T const* ConstStoredType;
                    typedef T* PointerType;
                    typedef T const* ConstPointerType;
                    typedef T& ReferenceType;
                    typedef T const& ConstReferenceType;

                public:
                    StoredType pointee_;

                protected:
                    DefaultStorage(void) : pointee_(Default())
                    {
                    }
                    template<class U>
                    DefaultStorage(const DefaultStorage<U> &)
                    {
                    }
                    DefaultStorage(const DefaultStorage &)
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

                struct DefaultStorageWrapper
                {
                    template<class T>
                    struct In
                    {
                        typedef DefaultStorage<T> type;
                    };
                };

                template<class T>
                class ArrayStorage
                {
                public:
                    typedef T* StoredType;
                    typedef T const* ConstStoredType;
                    typedef T* PointerType;
                    typedef T const* ConstPointerType;
                    typedef T& ReferenceType;
                    typedef T const& ConstReferenceType;

                //private:
                public:
                    StoredType pointee_;

                protected:
                    ArrayStorage(void) : pointee_(Default())
                    {
                    }
                    template<class U>
                    ArrayStorage(const ArrayStorage<U> &)
                    {
                    }
                    ArrayStorage(const ArrayStorage &)
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
                
                struct ArrayStorageWrapper
                {
                    template<class T>
                    struct In
                    {
                        typedef ArrayStorage<T> type;
                    };
                };

            }   // namespace Storage

            namespace Ownership
            {
                template<class T>
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
                    template<typename U>
                    RefCounted(const RefCounted<U> &rhs) : count_(reinterpret_cast<const RefCounted &>(rhs).count_)
                    {
                    }
                    RefCounted(const RefCounted &rhs) : count_(rhs.count_)
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

                struct RefCountedWrapper
                {
                    template<class T>
                    struct In
                    {
                        typedef RefCounted<T> type;
                    };
                };

                template<class T>
                class RefCountedMT
                {
                private:
                    // Data
                    volatile unsigned int *count_;

                protected:
                    RefCountedMT(void) : count_(new unsigned int)
                    {
                        assert(count_);
                        *count_ = 1;
                    }
                    template<typename U>
                    RefCountedMT(const RefCountedMT<U> &rhs) : count_(reinterpret_cast<const RefCountedMT &>(rhs).count_)
                    {
                    }
                    RefCountedMT(const RefCountedMT &rhs) : count_(rhs.count_)
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
                            delete const_cast<unsigned int *>(count_);
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

                struct RefCountedMTWrapper
                {
                    template<class T>
                    struct In
                    {
                        typedef RefCountedMT<T> type;
                    };
                };

            }   // namespace Ownership

            namespace Checking
            {
                template<class T>
                class NoCheck
                {
                protected:
                    NoCheck(void)
                    {
                    }
                    template<class U>
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

                struct NoCheckWrapper
                {
                    template<class T>
                    struct In
                    {
                        typedef NoCheck<T> type;
                    };
                };

                // Checks the pointer before dereference
                template<class T>
                class AssertCheck
                {
                protected:
                    AssertCheck(void)
                    {
                    }
                    template<class U>
                    AssertCheck(const AssertCheck<U> &)
                    {
                    }
                    AssertCheck(const AssertCheck &)
                    {
                    }
                    template<class U>
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

                struct AssertCheckWrapper
                {
                    template <class T>
                    struct In
                    {
                        typedef AssertCheck<T> type;
                    };
                };

                // Checks the pointer against zero upon initialization and before dereference
                // You can initialize an AssertCheckStrict with an AssertCheck
                template<class T>
                class AssertCheckStrict
                {
                protected:
                    AssertCheckStrict(void)
                    {
                    }
                    template<class U>
                    AssertCheckStrict(const AssertCheckStrict<U> &)
                    {
                    }
                    AssertCheckStrict(const AssertCheckStrict &)
                    {
                    }
                    template<class U>
                    AssertCheckStrict(const AssertCheck<U> &)
                    {
                    }
                    template<class U>
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

                struct AssertCheckStrictWrapper
                {
                    template <class T>
                    struct In
                    {
                        typedef AssertCheckStrict<T> type;
                    };
                };

            }   // namespace Checking
        }   // namespace SmartPtr

        // Default SmartPtr declaration
        template
        <
            typename T,
            class OwnershipPolicyT = SmartPtrPolicies::Ownership::RefCountedMTWrapper,
            class CheckingPolicyT = SmartPtrPolicies::Checking::AssertCheckWrapper,
            class StoragePolicyT = SmartPtrPolicies::Storage::DefaultStorageWrapper
        >
        class SmartPtr;


        namespace XMSVC6
        {
            template
            <
                class T,
                class OwnershipPolicyT,
                class CheckingPolicyT,
                class StoragePolicyT
            >
            struct SmartPtrImpl
            {
                struct Dummy{};
                typedef typename ApplyInnerType<StoragePolicyT, T>::type TempType;
                typedef XMSVC6::Private::VC_Base_Workaround<TempType, Dummy> sttype;
                // VC 6.0 will emit an "Error C2516. : is not a legal base class"
                // if one tries to use TempType as base class for SmartPtr.
                // Don't know why the compiler is happy with this workaround
                typedef sttype::LeftBase Storage;

                typedef Storage::PointerType PointerType;
                typedef Storage::ConstPointerType ConstPointerType;
                typedef Storage::StoredType StoredType;
                typedef Storage::ConstStoredType ConstStoredType;
                typedef Storage::ReferenceType ReferenceType;
                typedef Storage::ConstReferenceType ConstReferenceType;

                typedef typename ApplyInnerType<OwnershipPolicyT, PointerType>::type Temp2Type;
                typedef typename ApplyInnerType<CheckingPolicyT, StoredType>::type Temp3Type;

                typedef XMSVC6::Private::VC_Base_Workaround<Temp2Type, Dummy> owtype;
                typedef owtype::LeftBase TOwnership;

                typedef XMSVC6::Private::VC_Base_Workaround<Temp3Type, Dummy> chtype;
                typedef chtype::LeftBase TChecking;
            };
        }

        template
        <
            typename T,
            class OwnershipPolicyT,
            class CheckingPolicyT,
            class StoragePolicyT
        >
        class SmartPtr
            : public XMSVC6::SmartPtrImpl<T, OwnershipPolicyT, CheckingPolicyT, StoragePolicyT>::Storage
            , public XMSVC6::SmartPtrImpl<T, OwnershipPolicyT, CheckingPolicyT, StoragePolicyT>::TOwnership
            , public XMSVC6::SmartPtrImpl<T, OwnershipPolicyT, CheckingPolicyT, StoragePolicyT>::TChecking
        {
        private:
            typedef typename XMSVC6::SmartPtrImpl<T, OwnershipPolicyT, CheckingPolicyT, StoragePolicyT>::Storage StoragePolicyType;
            typedef typename XMSVC6::SmartPtrImpl<T, OwnershipPolicyT, CheckingPolicyT, StoragePolicyT>::TOwnership OwnershipPolicyType;
            typedef typename XMSVC6::SmartPtrImpl<T, OwnershipPolicyT, CheckingPolicyT, StoragePolicyT>::TChecking CheckingPolicyType;

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
                    CheckingPolicyType::OnDefault(pointee_);
            }
            SmartPtr(const StoredType &p) : StoragePolicyType(p)
            {
                    CheckingPolicyType::OnInit(pointee_); 
            }

            template
            <
                typename T1,
                class OwnershipPolicyT1,
                class CheckingPolicyT1,
                class StoragePolicyT1
            >
            SmartPtr(const SmartPtr<T1, OwnershipPolicyT1, CheckingPolicyT1, StoragePolicyT1> &rhs) : StoragePolicyType(rhs), OwnershipPolicyType(rhs), CheckingPolicyType(rhs)
            {
                pointee_ = OwnershipPolicyType::Clone(rhs.pointee_);
            }

            template
            <
                typename T1,
                class OwnershipPolicyT1,
                class CheckingPolicyT1,
                class StoragePolicyT1
            >
            SmartPtr(SmartPtr<T1, OwnershipPolicyT1, CheckingPolicyT1, StoragePolicyT1> &rhs) : StoragePolicyType(rhs), OwnershipPolicyType(rhs), CheckingPolicyType(rhs)
            {
                pointee_ = OwnershipPolicyType::Clone(rhs.pointee_);
            }

            SmartPtr(CopyArg &rhs) : StoragePolicyType(rhs), OwnershipPolicyType(rhs), CheckingPolicyType(rhs)
            {
                pointee_ = OwnershipPolicyType::Clone(rhs.pointee_);
            }
            ~SmartPtr(void)
            {
                if (OwnershipPolicyType::Release(pointee_) == true)
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
                p = sp.pointee_;
                sp.pointee_ = StoragePolicyType::Default();
            }

            friend inline void Reset(SmartPtr &sp, typename StoragePolicyType::StoredType p)
            {
                SmartPtr(p).Swap(sp);
            }

            operator PointerType() const
            {
                return pointee_;
            }

        /*
            template
            <
                typename T1,
                class OwnershipPolicyT1,
                class CheckingPolicyT1,
                class StoragePolicyT1
            >
            SmartPtr &operator=(const SmartPtr<T1, OwnershipPolicyT1, CheckingPolicyT1, StoragePolicyT1> &rhs)
            {
                SmartPtr temp(rhs);
                temp.Swap(*this);

                return *this;
            }

            template
            <
                typename T1,
                class OwnershipPolicyT1,
                class CheckingPolicyT1,
                class StoragePolicyT1
            >
            SmartPtr &operator=(SmartPtr<T1, OwnershipPolicyT1, CheckingPolicyT1, StoragePolicyT1> &rhs)
            {
                SmartPtr temp(rhs);
                temp.Swap(*this);

                return *this;
            }
        */

            SmartPtr &operator=(CopyArg &rhs)
            {
                SmartPtr temp(rhs);
                temp.Swap(*this);

                return *this;
            }

            PointerType operator->()
            {
                CheckingPolicyType::OnDereference(pointee_);

                return StoragePolicyType::operator->();
            }
            ConstPointerType operator->() const
            {
                CheckingPolicyType::OnDereference(pointee_);

                return StoragePolicyType::operator->();
            }
            ReferenceType operator*()
            {
                CheckingPolicyType::OnDereference(pointee_);
                
                return StoragePolicyType::operator*();
            }
            ConstReferenceType operator*() const
            {
                CheckingPolicyType::OnDereference(pointee_);

                return StoragePolicyType::operator*();
            }

            bool operator!() const // Enables "if (!sp) ..."
            {
                return pointee_ == 0;
            }
        };
    }
}

#endif
