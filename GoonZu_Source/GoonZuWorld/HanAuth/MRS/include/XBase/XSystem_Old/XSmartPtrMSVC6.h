// XSmartPtrMSVC6.h

#ifndef __XSMARTPTRMSVC6_Old_H__
#define __XSMARTPTRMSVC6_Old_H__

#include "XMSVC6Helpers.h"

#include <utility>
#include <cassert>


namespace XSmartPtrPolicies
{
    namespace Storage
    {
        template<class T>
        class CDefaultStorage
        {
        public:
            typedef T* TStoredType;
            typedef T const* TConstStoredType;
            typedef T* TPointerType;
            typedef T const* TConstPointerType;
            typedef T& TReferenceType;
            typedef T const& TConstReferenceType;

        //private:
		public:
            TStoredType m_pPointee;

        protected:
            CDefaultStorage(void) : m_pPointee(Default())
            {
            }
            template<class U>
            CDefaultStorage(const CDefaultStorage<U> &)
            {
            }
            CDefaultStorage(const CDefaultStorage &)
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

		struct CDefaultStorageWrapper
		{
			template <class T>
			struct In
			{
				typedef CDefaultStorage<T> type;
			};
		};

        template<class T>
        class CArrayStorage
        {
        public:
            typedef T* TStoredType;
            typedef T const* TConstStoredType;
            typedef T* TPointerType;
            typedef T const* TConstPointerType;
            typedef T& TReferenceType;
            typedef T const& TConstReferenceType;

        //private:
		public:
            TStoredType m_pPointee;

        protected:
            CArrayStorage(void) : m_pPointee(Default())
            {
            }
            template<class U>
            CArrayStorage(const CArrayStorage<U> &)
            {
            }
            CArrayStorage(const CArrayStorage &)
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
		
		struct CArrayStorageWrapper
		{
			template <class T>
			struct In
			{
				typedef CArrayStorage<T> type;
			};
		};

    }   // namespace Storage

    namespace Ownership
    {
        template<class T>
        class CRefCounted
        {
        private:
            // Data
            unsigned long *m_pCount;

        protected:
            CRefCounted(void) 
            {
                m_pCount = new unsigned int;
				//m_pCount = XSmallObject::operator new (sizeof(unsigned long));
                assert(m_pCount);
                *m_pCount = 1;
            }
            template<typename U>
            CRefCounted(const CRefCounted<U> &rhs) : m_pCount(reinterpret_cast<const CRefCounted &>(rhs).m_pCount)
            {
            }
            CRefCounted(const CRefCounted &rhs) : m_pCount(rhs.m_pCount)
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
					//XSmallObject::operator delete(m_pCount, sizeof(unsigned long));

                    return true;
                }

                return false;
            }

            void Swap(CRefCounted &rhs)
            {
                std::swap(m_pCount, rhs.m_pCount);
            }
        };

		struct CRefCountedWrapper
		{
			template<class T>
			struct In
			{
				typedef CRefCounted<T> type;
			};
		};

        template<class T>
        class CRefCountedMT
        {
        private:
            // Data
            volatile long *m_pCount;

        protected:
            CRefCountedMT(void) 
            {
                m_pCount = static_cast<volatile long *>(new long);
				//m_pCount = static_cast<volatile long *>(XSmallObject::operator new (sizeof(unsigned long)));
                assert(m_pCount);
                *m_pCount = 1;
            }
            template<typename U>
            CRefCountedMT(const CRefCountedMT<U> &rhs) : m_pCount(reinterpret_cast<const CRefCountedMT &>(rhs).m_pCount)
            {
            }
            CRefCountedMT(const CRefCountedMT &rhs) : m_pCount(rhs.m_pCount)
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
					delete const_cast<long *>(m_pCount);
					//XSmallObject::operator delete (const_cast<long *>(m_pCount), sizeof(unsigned long));

                    return true;
                }

                return false;
            }

            void Swap(CRefCountedMT &rhs)
            {
                std::swap(m_pCount, rhs.m_pCount);
            }
        };

		struct CRefCountedMTWrapper
		{
			template <class T>
			struct In
			{
				typedef CRefCountedMT<T> type;
			};
		};

    }   // namespace Ownership

    namespace Checking
    {
        template<class T>
        class CNoCheck
        {
        protected:
            CNoCheck(void)
            {
            }
            template<class U>
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

		struct CNoCheckWrapper
		{
			template <class T>
			struct In
			{
				typedef CNoCheck<T> type;
			};
		};

        // Checks the pointer before dereference
        template<class T>
        class CAssertCheck
        {
        protected:
            CAssertCheck(void)
            {
            }
            template<class U>
            CAssertCheck(const CAssertCheck<U> &)
            {
            }
            CAssertCheck(const CAssertCheck &)
            {
            }
            template<class U>
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

		struct CAssertCheckWrapper
		{
			template <class T>
			struct In
			{
				typedef CAssertCheck<T> type;
			};
		};

        // Checks the pointer against zero upon initialization and before dereference
        // You can initialize an CAssertCheckStrict with an CAssertCheck
        template<class T>
        class CAssertCheckStrict
        {
        protected:
            CAssertCheckStrict(void)
            {
            }
            template<class U>
            CAssertCheckStrict(const CAssertCheckStrict<U> &)
            {
            }
            CAssertCheckStrict(const CAssertCheckStrict &)
            {
            }
            template<class U>
            CAssertCheckStrict(const CAssertCheck<U> &)
            {
            }
            template<class U>
            CNoCheck(const CNoCheck<U> &)
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

		struct CAssertCheckStrictWrapper
		{
			template <class T>
			struct In
			{
				typedef CAssertCheckStrict<T> type;
			};
		};

    }   // namespace Checking
}   // namespace SmartPtr

// Default SmartPtr declaration
template
<
	typename T,
	class TOwnershipPolicy = XSmartPtrPolicies::Ownership::CRefCountedMTWrapper,
    class TCheckingPolicy = XSmartPtrPolicies::Checking::CAssertCheckWrapper,
    class TStoragePolicy = XSmartPtrPolicies::Storage::CDefaultStorageWrapper
>
class XSmartPtr;


namespace XMSVC6
{
	template
	<
		class T,
		class TOwnershipPolicy,
		class TCheckingPolicy,
		class TStoragePolicy
	>
	struct CSmartPtrImpl
	{
		struct Dummy{};
		typedef typename ApplyInnerType<TStoragePolicy, T>::type TempType;
		typedef ::XMSVC6::Private::VC_Base_Workaround<TempType, Dummy> sttype;
		// VC 6.0 will emit an "Error C2516. : is not a legal base class"
		// if one tries to use TempType as base class for SmartPtr.
		// Don't know why the compiler is happy with this workaround
		typedef sttype::LeftBase TStorage;

		typedef TStorage::TPointerType TPointerType;
		typedef TStorage::TConstPointerType TConstPointerType;
		typedef TStorage::TStoredType TStoredType;
		typedef TStorage::TConstStoredType TConstStoredType;
		typedef TStorage::TReferenceType TReferenceType;
		typedef TStorage::TConstReferenceType TConstReferenceType;

		typedef typename ApplyInnerType<TOwnershipPolicy, TPointerType>::type Temp2Type;
		typedef typename ApplyInnerType<TCheckingPolicy, TStoredType>::type Temp3Type;

		typedef ::XMSVC6::Private::VC_Base_Workaround<Temp2Type, Dummy> owtype;
		typedef owtype::LeftBase TOwnership;

		typedef ::XMSVC6::Private::VC_Base_Workaround<Temp3Type, Dummy> chtype;
		typedef chtype::LeftBase TChecking;
	};
}

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
template
<
	typename T,
	class TOwnershipPolicy,
    class TCheckingPolicy,
    class TStoragePolicy
>
class XSmartPtr
	: public XMSVC6::CSmartPtrImpl<T, TOwnershipPolicy, TCheckingPolicy, TStoragePolicy>::TStorage
    , public XMSVC6::CSmartPtrImpl<T, TOwnershipPolicy, TCheckingPolicy, TStoragePolicy>::TOwnership
    , public XMSVC6::CSmartPtrImpl<T, TOwnershipPolicy, TCheckingPolicy, TStoragePolicy>::TChecking
{
private:
	typedef typename XMSVC6::CSmartPtrImpl<T, TOwnershipPolicy, TCheckingPolicy, TStoragePolicy>::TStorage TStoragePolicyType;
	typedef typename XMSVC6::CSmartPtrImpl<T, TOwnershipPolicy, TCheckingPolicy, TStoragePolicy>::TOwnership TOwnershipPolicyType;
	typedef typename XMSVC6::CSmartPtrImpl<T, TOwnershipPolicy, TCheckingPolicy, TStoragePolicy>::TChecking TCheckingPolicyType;

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
			TCheckingPolicyType::OnDefault(m_pPointee);
    }
    XSmartPtr(const TStoredType &p) : TStoragePolicyType(p)
    {
			TCheckingPolicyType::OnInit(m_pPointee); 
    }

    template
	<
		typename T1,
		class TOwnershipPolicy1,
		class TCheckingPolicy1,
		class TStoragePolicy1
	>
    XSmartPtr(const XSmartPtr<T1, TOwnershipPolicy1, TCheckingPolicy1, TStoragePolicy1> &rhs) : TStoragePolicyType(rhs), TOwnershipPolicyType(rhs), TCheckingPolicyType(rhs)
    {
        m_pPointee = TOwnershipPolicyType::Clone(rhs.m_pPointee);
    }

    template
	<
		typename T1,
		class TOwnershipPolicy1,
		class TCheckingPolicy1,
		class TStoragePolicy1
	>
    XSmartPtr(XSmartPtr<T1, TOwnershipPolicy1, TCheckingPolicy1, TStoragePolicy1> &rhs) : TStoragePolicyType(rhs), TOwnershipPolicyType(rhs), TCheckingPolicyType(rhs)
    {
        m_pPointee = TOwnershipPolicyType::Clone(rhs.m_pPointee);
    }

    XSmartPtr(TCopyArg &rhs) : TStoragePolicyType(rhs), TOwnershipPolicyType(rhs), TCheckingPolicyType(rhs)
    {
        m_pPointee = TOwnershipPolicyType::Clone(rhs.m_pPointee);
    }
    ~XSmartPtr(void)
    {
        if (TOwnershipPolicyType::Release(m_pPointee) == true)
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
        p = sp.m_pPointee;
        sp.m_pPointee = TStoragePolicyType::Default();
    }

    friend inline void Reset(XSmartPtr &sp, typename TStoragePolicyType::TStoredType p)
    {
        XSmartPtr(p).Swap(sp);
    }

    operator TPointerType() const
    {
        return m_pPointee;
    }

/*
    template
	<
		typename T1,
		class TOwnershipPolicy1,
		class TCheckingPolicy1,
		class TStoragePolicy1
	>
    XSmartPtr &operator=(const XSmartPtr<T1, TOwnershipPolicy1, TCheckingPolicy1, TStoragePolicy1> &rhs)
    {
        XSmartPtr Temp(rhs);
        Temp.Swap(*this);

        return *this;
    }

    template
	<
		typename T1,
		class TOwnershipPolicy1,
		class TCheckingPolicy1,
		class TStoragePolicy1
	>
    XSmartPtr &operator=(XSmartPtr<T1, TOwnershipPolicy1, TCheckingPolicy1, TStoragePolicy1> &rhs)
    {
        XSmartPtr Temp(rhs);
        Temp.Swap(*this);

        return *this;
    }
*/

    XSmartPtr &operator=(TCopyArg &rhs)
    {
        XSmartPtr Temp(rhs);
        Temp.Swap(*this);

        return *this;
    }

    TPointerType operator->()
    {
        TCheckingPolicyType::OnDereference(m_pPointee);

        return TStoragePolicyType::operator->();
    }
    TConstPointerType operator->() const
    {
        TCheckingPolicyType::OnDereference(m_pPointee);

        return TStoragePolicyType::operator->();
    }
    TReferenceType operator*()
    {
        TCheckingPolicyType::OnDereference(m_pPointee);
		
        return TStoragePolicyType::operator*();
    }
    TConstReferenceType operator*() const
    {
        TCheckingPolicyType::OnDereference(m_pPointee);

        return TStoragePolicyType::operator*();
    }

    bool operator!() const // Enables "if (!sp) ..."
    {
        return m_pPointee == 0;
    }
};

#endif //__XSMARTPTRMSVC6_H__
