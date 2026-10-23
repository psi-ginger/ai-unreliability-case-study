#pragma once

//------------------------------------------------------------------------------  
// NkRTTI::GetName
//------------------------------------------------------------------------------  
inline const char* NkRTTI::GetName() const 
{
	return m_pcName;
}

//------------------------------------------------------------------------------  
// NkRTTI::GetBaseRTTI
//------------------------------------------------------------------------------  
inline const NkRTTI* NkRTTI::GetBaseRTTI() const 
{
	return m_pkBaseRTTI;
}

//------------------------------------------------------------------------------  
// NkCaster::GetTypeName
//------------------------------------------------------------------------------  
inline const char* NkCaster::GetTypeName() const
{
	const NkRTTI* pkRTTI = GetRTTI();
	return (pkRTTI)? pkRTTI->GetName(): "";
}

//------------------------------------------------------------------------------  
// NkCaster::GetType
//------------------------------------------------------------------------------  
inline const NkCaster::TYPE NkCaster::GetType() const
{
	return GetRTTI();
}

//------------------------------------------------------------------------------  
// NkCaster::IsSameExactType
//------------------------------------------------------------------------------  
inline const bool NkCaster::IsSameExactType(const TYPE kType) const
{
	return GetType() == kType;
}

//------------------------------------------------------------------------------  
// NkCaster::IsSameType
//------------------------------------------------------------------------------  
inline const bool NkCaster::IsSameType(const TYPE kType) const
{
	const NkRTTI *pkRTTI = GetRTTI();
	while(pkRTTI)
	{		
		if(pkRTTI== kType) return true;
		pkRTTI = pkRTTI->GetBaseRTTI();
	};
	return false;
}

//------------------------------------------------------------------------------  
// NkRTTI::IsSameExactKind
//------------------------------------------------------------------------------  
template <class T>
inline bool NkCaster::IsSameExactKind()
{
	return T::IsExactKindOf(&T::ms_RTTI, this);
}

//------------------------------------------------------------------------------  
// NkRTTI::IsSameKind
//------------------------------------------------------------------------------  
template <class T> 
inline bool NkCaster::IsSameKind()
{
	return T::IsKindOf(&T::ms_RTTI, this);
}

//------------------------------------------------------------------------------  
// NkRTTI::StaticCast
//------------------------------------------------------------------------------  
template <class T>
inline T* NkCaster::StaticCast()
{
	return (T*)this;
}

//------------------------------------------------------------------------------  
// NkRTTI::VerifyStaticCast
//------------------------------------------------------------------------------  
template <class T>
inline T* NkCaster::VerifyStaticCast()
{
#ifdef NkDEBUG
	return ((T*) T::VerifyStaticCastDebug( &T::ms_RTTI, this));
#else
	return (T*)this;
#endif
}

//------------------------------------------------------------------------------  
// NkCaster::DynamicCast
//------------------------------------------------------------------------------  
template <class T>
inline T* NkCaster::DynamicCast()
{
	return ((T*) T::DynamicCast(&T::ms_RTTI, this));
}

//------------------------------------------------------------------------------  
// NkCaster::ConvertTo
//------------------------------------------------------------------------------  
template <class T>
inline T* NkCaster::ConvertTo()
{
	return DynamicCast<T>();
}

//------------------------------------------------------------------------------  