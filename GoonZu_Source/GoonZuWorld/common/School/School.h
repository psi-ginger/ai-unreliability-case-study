/*
//---------------------------------
// 2004/2/10 김태곤
//---------------------------------
#include <Windows.h>
#include <Directives.h>

#define MAX_SCHOOL_AREA				40

#define SCHOOLAREA_SEOUL			1		// 서울.
#define SCHOOLAREA_KANGWON			2		// 강원. 
#define SCHOOLAREA_KYOUNGBOOK		3		// 경상북도 
#define SCHOOLAREA_KWANGJU			4		// 광주. 
#define SCHOOLAREA_DAIGU			5		// 대구
#define SCHOOLAREA_BOOSAN			6		// 부산. 
#define SCHOOLAREA_JOUNNAM			7		// 전라남도 
#define SCHOOLAREA_CHOONGBOOK		8		// 충청북도
#define SCHOOLAREA_KYOUNGKI			9		// 경기도 
#define SCHOOLAREA_KYOUNGNAM		10		// 경상남도 
#define SCHOOLAREA_CHINA			11		// 중국 
#define SCHOOLAREA_WOOLSAN			12		// 울산 
#define SCHOOLAREA_CHOONGNAM		13		// 충청남도
#define SCHOOLAREA_JOUNBOOK			14		// 전라북도 
#define SCHOOLAREA_INCHEON			15		// 인천 
#define SCHOOLAREA_ZEJU				16		// 제주도 
#define SCHOOLAREA_DAIJEON			17		// 대전 
#define SCHOOLAREA_WONJU			18		// 원주
#define SCHOOLAREA_CANADA			19		// 캐나다
#define SCHOOLAREA_CHEONAN			20		// 천안
#define SCHOOLAREA_SINGAPORE		21		// 싱가포르
#define SCHOOLAREA_NEWZEALAND		22		// 뉴질랜드
#define SCHOOLAREA_USA				23		// 미국
#define SCHOOLAREA_CHUNCHEON		24		// 춘천

#define MAX_SCHOOL_GRADE			10

#define SCHOOLGRADE_PRI				1		// 초등.
#define SCHOOLGRADE_MID				2		// 중등.
#define SCHOOLGRADE_HIG				3		// 고등.
#define SCHOOLGRADE_UNV				4		// 대
#define SCHOOLGRADE_SECON			5		// Secondary
#define SCHOOLGRADE_INTER			6		// InternationalSchool
#define SCHOOLGRADE_HIGH			7		// HighSchool

#define MAX_SCHOOL_NUMBER			1000

#define MAX_SCHOOL_NAME_SIZE		32

//-------------------------------
// 학교 지역 
//------------------------------
class cltSchoolArea{
public:
	char szName[32];


	cltSchoolArea()
	{
		strcpy(szName, "");
	}

	cltSchoolArea(char* pname)
	{
		strcpy(szName, pname);
	}

	void Set(cltSchoolArea* pclarea)
	{
		strcpy(szName, pclarea->szName);
	}
	
};

//-----------------------------
// 학교등급 
//-----------------------------
class cltSchoolGrade{
public:
	char szName[32];

	cltSchoolGrade()
	{
		strcpy(szName, "");
	}

	cltSchoolGrade(char* pname)
	{
		strcpy(szName, pname);
	}

	void Set(cltSchoolGrade* pclgrade)
	{
		strcpy(szName, pclgrade->szName);
	}
};

//-----------------------------
// 단위 학교 정보. 
//-----------------------------
class cltSchool{
public:
	SI16 siUnique;
	SI16 siAreaCode;				// 지역코드 
	SI16 siGradeCode;				// 등급코드 (초중고대)
	char szName[MAX_SCHOOL_NAME_SIZE];	// 학교 이름. 
	SI16 siFile;
	SI16 siIconIndex;					// 아이콘 이미지 인덱스. 


	cltSchool()
	{
		Init();
	}
	cltSchool(SI16 unique, SI16 areacode, SI16 gradecode, char* pname, SI16 file, SI16 icon)
	{
		siUnique	= unique;
		siAreaCode	= areacode;
		siGradeCode	= gradecode;
		MStrCpy( szName, pname, MAX_SCHOOL_NAME_SIZE );
		siFile		= file;
		siIconIndex	= icon;
	}

	void Init()
	{
		siUnique	= 0;
		siAreaCode	= 0;
		siGradeCode	= 0;
		strcpy(szName, "");
		siFile		= 0;
		siIconIndex = 0;
	}

	void Set(cltSchool* pclschool)
	{
		siUnique	= pclschool->siUnique;
		siAreaCode	= pclschool->siAreaCode;
		siGradeCode	= pclschool->siGradeCode;
		strcpy(szName, pclschool->szName);
		siFile		= pclschool->siFile;
		siIconIndex = pclschool->siIconIndex;
	}

	// 전체 이름을 구한다. 
	bool GetFullName(cltSchoolManager* pchoolmanager, char* pname);

};

//------------------------
// 학교 관리자. 
//------------------------
class cltSchoolManager{
public:

	HWND m_hDlg;					// 아이템 정보. 

	RECT DlgRect;
	bool bDlgRectInitSwitch;
	

	SI32			siCurrentSchoolUnique;
	bool			bShowUserNumSwitch;
	SI32			siUserNum;

	SI32			siSchoolIndex;
	cltSchool		clSchool[MAX_SCHOOL_NUMBER];
	cltSchoolArea	clSchoolArea[MAX_SCHOOL_AREA];
	cltSchoolGrade	clSchoolGrade[MAX_SCHOOL_GRADE];

	// 지역 코드.
	SI32			siCurrentSchoolArea;
	// 등급 코드 
	SI32			siCurrentSchoolGrade;

	cltSchoolManager();
	~cltSchoolManager();

	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd);
	static BOOL CALLBACK StaticDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	// 학교등급을 등록한다. 
	bool AddGrade(SI32 areacode, cltSchoolGrade* pclgrade);

	// 등급명을 근거로 등급 코드를 얻는다. 
	SI32 GetGradeCode(char* pname);

	// 학교지역을 등록한다. 
	bool AddArea(SI32 areacode, cltSchoolArea* pclarea);

	// 지역명을 근거로 지역 코드를 얻는다. 
	SI32 GetAreaCode(char* pname);

	// 학교를 등록한다. 
	bool Add(cltSchool* pclschool);

	//학교명단을 보여준다.  
	void ShowSchoolList();

	// 유니크를 근거로 ref를 구한다. 
	SI32 GetRefFromUnique(SI32 unique);

	// 학교 리스트를 등록한다. 
	void SetSchoolList();

	// 학교 로고를 출력한다. 
	void DrawLogo(SI32 schoolunique);


};
*/