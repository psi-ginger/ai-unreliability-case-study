//---------------------------------
// 2004/2/25 김태곤
//---------------------------------
#ifndef _FATHER_H
#define _FATHER_H

#include <Directives.h>

#define ADD_FATHERSUPPLYER_ALREADY	1				// 이미 등록되어 있다. 
#define ADD_FATHERSUPPLYER_FULL		2				// 등록자가 꽉찼다. 
#define MAX_CHILD_LIST_IN_CLIENT_NUMBER		50		// 피후견자 최대 수

class cltFatherManager{
public:
	RECT DlgRect;
	bool bDlgRectInitSwitch;

	bool bUpdateFatherSwitch;
	bool bUpdateFatherApplyerListSwitch;
	bool bUpdateChildListSwitch;

	// 스승님 등록 신청자 명단. 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltFatherApplyerInfo, MAX_FATHER_APPLYER_LIST_NUMBER>	clApplyerList;
#else
	cltFatherApplyerInfo clApplyerList[MAX_FATHER_APPLYER_LIST_NUMBER];
#endif

	// 제자님 리스트. 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltChildInfo, MAX_CHILD_LIST_IN_CLIENT_NUMBER>	clChildList;
#else
	cltChildInfo clChildList[MAX_CHILD_LIST_IN_CLIENT_NUMBER];
#endif

	SI32 m_siSelectedIndex ;								// 선택한 리스트 데이타
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_FATHER_APPLYER_LIST_NUMBER>		m_siFatherApplyInx;
	NSafeTArray<SI32, MAX_CHILD_LIST_IN_CLIENT_NUMBER>		m_siChildListInx;
#else
	SI32 m_siFatherApplyInx[MAX_FATHER_APPLYER_LIST_NUMBER]; // 스승 신청자 리스트 순서에 따른 인덱스
	SI32 m_siChildListInx[MAX_CHILD_LIST_IN_CLIENT_NUMBER];	// 제자 리스트 소팅후 들어갈 리스트 데이타(위치)
#endif

	// 내림차순으로 정렬한다. 
	void ChildSort(SI32 sisortdata[]);

	cltFatherManager();
	~cltFatherManager();
	
	// 스승님을 보여준다. 
	void ShowFatherName(HWND hDlg);
	// 스승님 등록자 명단을 보여준다. 
	void ShowFatherList();

	// 스승님 등록 신청자를 추가한다. 
	bool AddApplyer(cltFatherApplyerInfo* pclinfo, SI32* preturnval);
	// 스승님 등록 신청자를 삭제 한다. 
	bool DeleteApplyer(SI32 personid);
	// 특정 사용자가 리스트에 존재하는가. 
	bool IsExist(SI32 personid);


	// 피후견자 리스트를 구한다 
	bool AddChildList(cltChildInfo* pclinfo);

	// 피후견자 리스트를 모두 삭제한다. 
	void DelChildList();

	// 피후견자 이름을 구한다. 
	TCHAR* GetChildName(SI32 personid);

	void Action(cltCharManager* pclCM);

};

#endif