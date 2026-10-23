// XSmartPtr.h

#ifndef __XSMARTPTR_Old_H__
#define __XSMARTPTR_Old_H__

#if defined(_MSC_VER) && _MSC_VER < 1300
	#include "XSmartPtrMSVC6.h"
#else

#include <utility>
#include <cassert>


namespace XSmartPtrPolicies
{
    namespace Storage
    {
        template<typename T>
        class CDefaultStorage
        {
        public:
            typedef T* TStoredType;
            typedef T const* TConstStoredType;
            typedef T* TPointerType;
            typedef T const* TConstPointerType;
            typedef T& TReferenceType;
            typedef T const& TConstReferenceType;

        private:
            TStoredType m_pPointee;

        protected:
            CDefaultStorage(void) : m_pPointee(Default())
            {
            }
            CDefaultStorage(const CDefaultStorage &)
            {
            }
            template<typename U>
            CDefaultStorage(const CDefaultStorage<U> &)
            {
            }
            CDefaultStorage(const TStoredType &p) : m_pPointee(p)
            {
            }

            void Swap(CDefaultStorage &rhs)
            {
                std::swap(m_pPointee, rhs.m_pPointee);
            }

            TPointerType GetPtr(void)
            {
                return m_pPointee;
            }
            TConstPointerType GetPtr(void) const
            {
                return m_pPointee;
            }

        public:
            // Accessors
            friend inline TPointerType GetRawPtr(const CDefaultStorage &sp)
            {
                return sp.m_pPointee;
            }
            friend inline const TStoredType &GetRawPtrRef(const CDefaultStorage &sp)
            {
                return sp.m_pPointee;
            }
            friend inline TStoredType &GetRawPtrRef(CDefaultStorage &sp)
            {
                return sp.m_pPointee;
            }

            TPointerType operator->() const
            {
                return m_pPointee;
            }
            TReferenceType operator*() const
            {
                return *m_pPointee;
            }

        protected:
            // Destroys the data stored
            // (Destruction might be taken over by the OwnershipPolicy)
            void Destroy(void)
            {
                delete m_pPointee;
            }

            // Default value to initialize the pointer
            static TStoredType Default(void)
            {
                return 0;
            }
        };

        template<typename T>
        class CArrayStorage
        {
        public:
            typedef T* TStoredType;
            typedef T const* TConstStoredType;
            typedef T* TPointerType;
            typedef T const* TConstPointerType;
            typedef T& TReferenceType;
            typedef T const& TConstReferenceType;

        private:
            TStoredType m_pPointee;

        protected:
            CArrayStorage(void) : m_pPointee(Default())
            {
            }
            CArrayStorage(const CArrayStorage &)
            {
            }
            template<typename U>
            CArrayStorage(const CArrayStorage<U> &)
            {
            }
            CArrayStorage(const TStoredType &p) : m_pPointee(p)
            {
            }

            void Swap(CArrayStorage &rhs)
            {
                std::swap(m_pPointee, rhs.m_pPointee);
            }

            TPointerType GetPtr(void)
            {
                return m_pPointee;
            }
            TConstPointerType GetPtr(void) const
            {
                return m_pPointee;
            }

        public:
            // Accessors
            friend inline TPointerType GetRawPtr(const CArrayStorage &sp)
            {
                return sp.m_pPointee;
            }
            friend inline const TStoredType &GetRawPtrRef(const CArrayStorage &sp)
            {
                return sp.m_pPointee;
            }
            friend inline TStoredType &GetRawPtrRef(CArrayStorage &sp)
            {
                return sp.m_pPointee;
            }

            TPointerType operator->() const
            {
                return m_pPointee;
            }
            TReferenceType operator*() const
            {
                return *m_pPointee;
            }
            TConstReferenceType operator[](int i) const
            {
                assert(i >= 0);
                return GetPtr()[i];
            }
            TReferenceType operator[](int i)
            {
                assert(i >= 0);
                return GetPtr()[i];
            }

        protected:
            // Destroys the data stored
            // (Destruction might be taken over by the OwnershipPolicy)
            void Destroy(void)
            {
                delete[] m_pPointee;
            }

            // Default value to initialize the pointer
            static TStoredType Default(void)
            {
                return 0;
            }
        };
    }   // namespace Storage

    namespace Ownership
    {
        template<typename T>
        class CRefCounted
        {
        private:
            // Data
            unsigned int *m_pCount;

        protected:
            CRefCounted(void) 
            {
                m_pCount = new unsigned int;
				//m_pCount = XSmallObject::operator new (sizeof(unsigned int));
                assert(m_pCount);
                *m_pCount = 1;
            }
            CRefCounted(const CRefCounted &rhs) : m_pCount(rhs.m_pCount)
            {
            }
            template<typename U>
            CRefCounted(const CRefCounted<U> &rhs) : m_pCount(reinterpret_cast<const CRefCounted &>(rhs).m_pCount)
            {
            }

            T Clone(const T &Value)
            {
                ++*m_pCount;

                return Value;
            }

            bool Release(const T &)
            {
                if (--*m_pCount == 0)
                {
                    delete m_pCount;
					//XSmallObject::operator delete(m_pCount, sizeof(unsigned int));

                    return true;
                }

                return false;
            }

            void Swap(CRefCounted &rhs)
            {
                std::swap(m_pCount, rhs.m_pCount);
            }
        };

        template<typename T>
        class CRefCountedMT
        {
        private:
            // Data
            volatile LONG *m_pCount;

        protected:
            CRefCountedMT(void) 
            {
                m_pCount = new LONG;
				//m_pCount = static_cast<volatile LONG *>(XSmallObject::operator new (sizeof(LONG)));
                assert(m_pCount);
                *m_pCount = 1;
            }
            CRefCountedMT(const CRefCountedMT &rhs) : m_pCount(rhs.m_pCount)
            {
            }
            template<typename U>
            CRefCountedMT(const CRefCountedMT<U> &rhs) : m_pCount(reinterpret_cast<const CRefCountedMT &>(rhs).m_pCount)
            {
            }

            T Clone(const T &Value)
            {
                InterlockedIncrement(m_pCount);

                return Value;
            }

            bool Release(const T &)
            {
                if (InterlockedDecrement(m_pCount) == 0)
                {
                    delete m_pCount;
					//XSmallObject::operator delete (const_cast<LONG *>(m_pCount), sizeof(LONG));

                    return true;
                }

                return false;
            }

            void Swap(CRefCountedMT &rhs)
            {
                std::swap(m_pCount, rhs.m_pCount);
            }
        };
    }   // namespace Ownership

    namespace Checking
    {
        template<typename T>
        class CNoCheck
        {
        protected:
            CNoCheck(void)
            {
            }
            template<typename U>
            CNoCheck(const CNoCheck<U> &)
            {
            }
            CNoCheck(const CNoCheck &)
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
            static void Swap(CNoCheck &)
            {
            }
        };

        // Checks the pointer before dereference
        template<typename T>
        class CAssertCheck
        {
        protected:
            CAssertCheck(void)
            {
            }
            template<typename U>
            CAssertCheck(const CAssertCheck<U> &)
            {
            }
            CAssertCheck(const CAssertCheck &)
            {
            }
            template<typename U>
            CAssertCheck(const CNoCheck<U> &)
            {
            }

            static void OnDefault(const T &)
            {
            }
            static void OnInit(const T &)
            {
            }
            static void OnDereference(const T &AValue)
            {
                assert(AValue != 0);
                (void)AValue;
            }
            static void Swap(CAssertCheck &)
            {
            }
        };

        // Checks the pointer against zero upon initialization and before dereference
        // You can initialize an CAssertCheckStrict with an CAssertCheck
        template<typename T>
        class CAssertCheckStrict
        {
        protected:
            CAssertCheckStrict(void)
            {
            }
            template<typename U>
            CAssertCheckStrict(const CAssertCheckStrict<U> &)
            {
            }
            CAssertCheckStrict(const CAssertCheckStrict &)
            {
            }
            template<typename U>
            CAssertCheckStrict(const CAssertCheck<U> &)
            {
            }
            template<typename U>
            void CNoCheck(const CNoCheck<U> &)
            {
            }

            static void OnDefault(const T &AValue)
            {
                assert(AValue != 0);
                (void)AValue;
            }
            static void OnInit(const T &AValue)
            {
                assert(AValue != 0);
                (void)AValue;
            }
            static void OnDereference(const T &AValue)
            {
                assert(AValue != 0);
                (void)AValue;
            }
            static void Swap(CAssertCheckStrict &)
            {
            }
        };
    }   // namespace Checking
}   // namespace SmartPtr

// Default SmartPtr declaration
template<typename T, template<typename> class TOwnershipPolicy = XSmartPtrPolicies::Ownership::CRefCountedMT,
        template<typename> class TCheckingPolicy = XSmartPtrPolicies::Checking::CAssertCheck,
        template<typename> class TStoragePolicy = XSmartPtrPolicies::Storage::CDefaultStorage> class XSmartPtr;

/*!
	\brief				XSmartPtr 단위정책
						- TStoragePolicy
							- DefaultStorage: pointer 해제시 delete 연산자 사용(기본정책)
							- ArrayStorage: pointer 해제시 delete 대신 delete [] 연산자 사용
						.
						- TOwnershipPolicy
							- RefCounted: single-thread 지원
							- RefCountedMT: multi-thread 지원(기본정책)
						.
						- TCheckingPolicy
							- NoCheck: pointer에 대한 valid 검사안함
							- AssertCheck: object의 dereferencing시 assert를 이용한 pointer에 대한 valid 검사(기본정책)
							- AssertCheckStrict: XSmartPtr 초기화시 assert를 이용한 pointer에 대한 valid 검사
						.

	- Required header: <XBase/XSystem/XSmartPtr.h>
*/
template<typename T, template<typename> class TOwnershipPolicy,
        template<typename> class TCheckingPolicy, template<typename> class TStoragePolicy>
class XSmartPtr : public TStoragePolicy<T>, public TOwnershipPolicy<typename TStoragePolicy<T>::TPointerType>,
                public TCheckingPolicy<typename TStoragePolicy<T>::TStoredType>
{
private:
    typedef TStoragePolicy<T> TStoragePolicyType;
    typedef TOwnershipPolicy<typename TStoragePolicy<T>::TPointerType> TOwnershipPolicyType;
    typedef TCheckingPolicy<typename TStoragePolicy<T>::TStoredType> TCheckingPolicyType;

public:
    typedef typename TStoragePolicyType::TPointerType TPointerType;
    typedef typename TStoragePolicyType::TConstPointerType TConstPointerType;
    typedef typename TStoragePolicyType::TStoredType TStoredType;
    typedef typename TStoragePolicyType::TConstStoredType TConstStoredType;
    typedef typename TStoragePolicyType::TReferenceType TReferenceType;
    typedef typename TStoragePolicyType::TConstReferenceType TConstReferenceType;
    typedef const XSmartPtr TCopyArg;

    XSmartPtr(void)
    {
        TCheckingPolicyType::OnDefault(GetRawPtr(*this));
    }
    XSmartPtr(const TStoredType &p) : TStoragePolicyType(p)
    {
        TCheckingPolicyType::OnInit(GetRawPtr(*this));
    }
    XSmartPtr(TCopyArg &rhs) : TStoragePolicyType(rhs), TOwnershipPolicyType(rhs), TCheckingPolicyType(rhs)
    {
        GetRawPtrRef(*this) = TOwnershipPolicyType::Clone(GetRawPtrRef(rhs));
    }
    template<typename T1, template<typename> class TOwnershipPolicy1,
            template<typename> class TCheckingPolicy1, template<typename> class TStoragePolicy1>
    XSmartPtr(const XSmartPtr<T1, TOwnershipPolicy1, TCheckingPolicy1, TStoragePolicy1> &rhs) : TStoragePolicyType(rhs), TOwnershipPolicyType(rhs), TCheckingPolicyType(rhs)
    {
        GetRawPtrRef(*this) = TOwnershipPolicyType::Clone(GetRawPtrRef(rhs));
    }
    template<typename T1, template<typename> class TOwnershipPolicy1,
            template<typename> class TCheckingPolicy1, template<typename> class TStoragePolicy1>
    XSmartPtr(XSmartPtr<T1, TOwnershipPolicy1, TCheckingPolicy1, TStoragePolicy1> &rhs) : TStoragePolicyType(rhs), TOwnershipPolicyType(rhs), TCheckingPolicyType(rhs)
    {
        GetRawPtrRef(*this) = TOwnershipPolicyType::Clone(GetRawPtrRef(rhs));
    }
    ~XSmartPtr(void)
    {
        if (TOwnershipPolicyType::Release(GetRawPtr(*static_cast<TStoragePolicyType *>(this))) == true)
        {
            TStoragePolicyType::Destroy();
        }
    }

    void Swap(XSmartPtr &rhs)
    {
        TOwnershipPolicyType::Swap(rhs);
        TCheckingPolicyType::Swap(rhs);
        TStoragePolicyType::Swap(rhs);
    }

    friend inline void Release(XSmartPtr &sp, typename TStoragePolicyType::TStoredType &p)
    {
        p = GetRawPtrRef(sp);
        GetRawPtrRef(sp) = TStoragePolicyType::Default();
    }

    friend inline void Reset(XSmartPtr &sp, typename TStoragePolicyType::TStoredType p)
    {
        XSmartPtr(p).Swap(sp);
    }

    operator TPointerType() const
    {
        return GetRawPtr(*this);
    }
    XSmartPtr &operator=(TCopyArg &rhs)
    {
        XSmartPtr Temp(rhs);
        Temp.Swap(*this);

        return *this;
    }
    template<typename T1, template<typename> class TOwnershipPolicy1,
            template<typename> class TCheckingPolicy1, template<typename> class TStoragePolicy1>
    XSmartPtr &operator=(const XSmartPtr<T1, TOwnershipPolicy1, TCheckingPolicy1, TStoragePolicy1> &rhs)
    {
        XSmartPtr Temp(rhs);
        Temp.Swap(*this);

        return *this;
    }
    template<typename T1, template<typename> class TOwnershipPolicy1,
            template<typename> class TCheckingPolicy1, template<typename> class TStoragePolicy1>
    XSmartPtr &operator=(XSmartPtr<T1, TOwnershipPolicy1, TCheckingPolicy1, TStoragePolicy1> &rhs)
    {
        XSmartPtr Temp(rhs);
        Temp.Swap(*this);

        return *this;
    }

    TPointerType operator->()
    {
        TCheckingPolicyType::OnDereference(GetRawPtrRef(*this));

        return TStoragePolicyType::operator->();
    }
    TConstPointerType operator->() const
    {
        TCheckingPolicyType::OnDereference(GetRawPtrRef(*this));

        return TStoragePolicyType::operator->();
    }
    TReferenceType operator*()
    {
        TCheckingPolicyType::OnDereference(GetRawPtrRef(*this));

        return TStoragePolicyType::operator*();
    }
    TConstReferenceType operator*() const
    {
        TCheckingPolicyType::OnDereference(GetRawPtrRef(*this));

        return TStoragePolicyType::operator*();
    }

    bool operator!() const // Enables "if (!sp) ..."
    {
        return GetRawPtr(*this) == 0;
    }
};

#endif


#endif //__XSMARTPTR_H__
