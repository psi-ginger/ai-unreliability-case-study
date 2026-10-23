//--------------------------------------------------------------------------
// Name: HQSync CLASS
// Desc: 프로세스간의 Win32 Sync  
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Name: HQSync
// Desc: 동기 함수
//--------------------------------------------------------------------------
class HQSync
{
public:
	virtual inline void Lock()   {return;};
	virtual inline void Unlock() {return;};
	virtual inline ~HQSync()     {return;};
};


//--------------------------------------------------------------------------
// Name: UCriticalSection
// Desc: 동기화 객체
//--------------------------------------------------------------------------
class HQCriticalSection : public HQSync
{
public:
	inline  HQCriticalSection()  {InitializeCriticalSection(&CriticalSection);return;};
	inline  void Lock()          {EnterCriticalSection(&CriticalSection);return;};
	inline  void Unlock()        {LeaveCriticalSection(&CriticalSection);return;};
	inline ~HQCriticalSection()  {DeleteCriticalSection(&CriticalSection);return;};
private:
	CRITICAL_SECTION CriticalSection;
	
};

//--------------------------------------------------------------------------
// Name: HQLock
// Desc: 동기화 실행을 위한 객체 
//--------------------------------------------------------------------------
class HQLock
{
public:
	inline  HQLock(HQSync *pSO) {pSync=pSO;pSync->Lock();return;};
	inline ~HQLock()            {pSync->Unlock();return;};
private:
  	HQSync  *pSync;
};
