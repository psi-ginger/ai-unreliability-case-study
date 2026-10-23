/* ==========================================================================
	클래스 :		CNHintDlg

	작성일 :		05/04/24
	작성자 :		김태양

	목  적 :		.
	                

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/
#pragma once

#include <Directives.h>
#include <stdio.h>

#include "../../Resource.h"
#include "../../InterfaceMgr/DialogController.h"

#define     MAX_HINTCOMBO_TYPE		20
#define     MAX_HINTLIST_TYPE		20


#define 	FAMELEVEL_GET	         1    //명성등급의 획득방법
#define 	FISHING_READY	         2     //낚시를 하기 전에 필요한 준비물
#define 	FISHING_WISEANGLER	     3     //현명한 낚시군이 되는 비결
#define 	FISHING_PLACE	         4     //좋은 낚시터는?
#define 	WHAT_REPORT	             5     //사용자 신고란 무엇입니까?
#define 	WHATIS_AUTO_REPORT	     6     //자동 프로그램이란 무엇입니까?
#define 	HOWCANFIND_REPORT	     7     //어떻게 자동프로그램 사용자를 가려낼수 있습니까?
#define 	WHAT_DO_REPORT	         8     //신고를 하면 어떻게 됩니까?
#define 	FIND_AUTO_REPORT	     9     //자동 프로그램 사용자로 판명되면?
#define 	IF_FALSE_ACCUSATIOR	     10    //허위 신고일때는?
#define 	ETC_ACCUSATION_REPORT    11    //기타 신고가 불가능한 경우  
#define 	HOK_WHO	                 12    //혹부리영감은 누구입니까?
#define 	HOK_WHATJOB	             13    //혹부리영감은 무슨일을 합니까?
#define 	HOK_CREATE	             14    //호품. 명품 만들기
#define 	HOK_UPGRADE	             15    //물품 능력치 올리기 
#define 	HOK_FAILURE	             16    //혹부리 영감이 실패하면?
#define 	PERSONALMARKET_OPEN	     17    //개인 상점 개설
#define 	PERSONALMARKET_USE	     18    //개인 상점 이용법
#define 	PERSONALMARKET_FIND	     19    //개인 상점에서 판매되고 있는 물품 검색	법
#define 	SWORD	                 20    //검을 사용하십니까?
#define 	SPEAR	                 21    //창을 사용하십니까?
#define 	BOW	                     22    //활을 사용하십니까?
#define 	AXE	                     23    //도끼를 사용하십니까?
#define 	GUN	                     24    //총을 사용하십니까?
#define 	STAFF	                 25    //지팡이를 사용하십니까?
#define 	WHAT_DIVIDEND	         26    //배당이란 무엇인가요?
#define 	STOCK_WHAT	             27    //주주배당이란?
#define 	RESIDENTS_DIVIDEND	     28    //주민배당이란?
#define 	DECISION_DIVIDEND	     29    //주민 배당과 주주배당의 결정
#define 	WHAT_CANNOTKILL	         30    //불가사리란 무엇입니까?
#define 	WHERE_CANNOTKILL	     31    //불가사리는 어디서 볼수 있습니까?
#define 	HOWCANGET_CANNOTKILL     32    //불가사리는 어떻게 잡을수 있습니까?
#define 	REWARD_CANNOTKILL	     33    //불가사리 이벤트를 성공하면 어떤 보상이 있습니까?
#define 	SUMMONSWHATIS	         34    //소환수는 무엇입니까?
#define 	SUMMONSLIFE	             35    //소환수의 수명은?
#define 	SUMMONSHOWCANIGET        36    //소환수는 어떻게 얻습니까?
#define 	SUMMONSWELLBEING	     37    //소환수를 잘 키우는 방법은?
#define 	SUMMONSEVOLUTION	     38    //소환수는 어떻게 진화합니까?
#define 	SUMMONSUPGRADE	         39    //소환수는 어떻게 강해집니까?
#define 	SUMMONSDIE	             40    //소환수가 전투에서 죽으면?
#define     WHO_TEACHER              41    //스승님은 누구입니까?
#define     WANT_STUDENT             42    //제자가 되고 싶은데요?
#define     CHANGE_TEACHER           43    //스승님을 바꾸고 싶은데요?
#define     NOCONNECT_STUDENT        44    //제자가 오랫동안 접속을 하지 않으면?
#define     WANT_TEACHER             45    //스승이 되고 싶은데요?  
#define     WHAT_GOOD_TEACHER        46    //스승이 되면 무엇이 좋은가요?
#define 	HOZO_WHAT	             47    //호조 판서는 누구입니까?
#define 	HOZO_RECOMMAND	         48    //추천인 포상금 결정이란?
#define 	HOZO_MONEY	             49    //신규 회원 장착자금이란?
#define 	HOZO_POPULATION	         50    //각 마을 주민 수 결정이란?
//#define 	HOWCANWIN_GOGURYO	     51    //고구려 유적 이벤트란 무엇입니까?
//#define     OPEN_GOGURYO	         52    //고구려 유적 보호 이벤트는 어떻게 발동됩니까?
//#define 	SUCESS_GOGURYO           53    //고구려 유적 보호 이벤트를 성공시키는 방법은?
//#define     REWARD_GOGURYO	         54    //고구려 유적 보호 이벤트의 성공 보상은?
#define     VILLAGE_LEVEL            55    //마을 레벨이란 무엇입니까?
#define     WHAT_GOOD_VILLAGE        56    //마을 레벨이 높으면 무엇이 좋습니까?
#define     HOW_VILLAGE_LEVEL        57    //마을 레벨은 어떻게 올립니까?      
#define 	DOWN_TOWNLEVEL	         58    //마을 레벨이 떨어지면?
#define 	PARTY_WHAT	             59    //파티 사냥이란?
#define 	PARTY_MUSTKNOW	         60    //파티 사냥, 이것만은 알아 두자 
#define 	HORSEWHAT	             61    //말은 무엇입니까? 
#define 	HORSEWHATGOOD	         62    //말을 키우면 무엇이 좋습니까?
#define 	HORSEAGE	             63    //말이 수명을 다하면?
#define 	HORSEHOW	             64    //말은 어떻게 얻습니까?
#define 	HORSEFREE	             65    //말 풀어주기?
#define 	HORSEHOWBREED	         66    //말을 잘 키우는 방법은?
#define 	HORSEITEM	             67    //말에게 아이템 옮기는 법
#define 	RECOMMAND_WHAT	         68    //추천인 제도란 무엇인가요?
#define 	RECOMMAND_GOOD	         69    //추천을 받으면 무엇이 좋은가요?
#define 	RECOMMAND_WARNING	     70    //추천할 시에 주의할 점
#define 	STOCK_SUBSCRIPTION	     71    //군주에서 주식이란?
#define 	STOCK_INCREASE	         72    //주식 공모에 참여해보자
#define 	STOCKHOLDER_DIVIDEND	 73    //주식 증자에 참여해보자
#define     WHAT_CAPITAL             74    //자본금이란 무엇인가요?
#define     HOW_CAPITAL              75    //자본금은 어떻게 생기는가요?
#define     WHAT_PROFIT              76    //수익금이란 무엇인가요?
#define 	WAR_JOINING	             77    //왜구 전쟁에 참가해보자
#define 	WAR_BONUSTIME	         78    //보너스 타임
#define 	WHATGOOD_GOODPRODUCT     79    //명품이란 무엇입니까?
#define 	HOWCANGET_GOODPRODUCT    80    //명품을 어떻게 얻을수 있습니까?
#define     WHAT_GOODPRODUCT         81    //명품을 얻게 되면 무엇이 좋습니까?
#define     WHAT_FAME                82    // 명성 등급의 정의	
#define     WHAT_ENCHANT                83    //인챈트란


class CStatic;
class CButton;
class CComboBox;
class CList;
class CEdit;

class NHintDlgExplain : public CDialogController
{
public:
	NHintDlgExplain();
	~NHintDlgExplain();

	//void AddComboType(TCHAR *szName,TCHAR *szFile);

	void Create();

	static void CALLBACK NHintDlgExplainStaticProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NHintDlgExplainProc( UINT nEvent, int nControlID, CControllerObj* pControl);

	SI32 GetExplainType(TCHAR* pExplainType);
	void ShowExplainHint(SI32 siHintType);

	void Show(TCHAR * szFileName ) ;
	void Set(TCHAR * szFileName ) ;

	void ReadListFile();
	void ShowList(TCHAR * szFileName, SI16 txtSize);

private:
	//HWND			m_hWnd;
	//HINSTANCE		m_hInst;

	//HWND	m_hDlg;	
	//RECT	m_DlgRect;
	//HBITMAP m_hHitBitmap;
	BOOL	m_DlgRectInitSwitch;

	TCHAR	m_szBmpName[MAX_HINTCOMBO_TYPE][128] ;
	TCHAR	m_szComboType[MAX_HINTCOMBO_TYPE][256] ;

	TCHAR	m_szListType[MAX_HINTLIST_TYPE][128] ;
	TCHAR	m_szSubject[MAX_HINTLIST_TYPE][128] ;

	//CStatic     	*m_pNHintStatic1; 
	CStatic     	*m_pNHintStatic2;
    CStatic     	*m_pNHintStatic3;
	CComboBox       *m_pNHintCombo;
	CList     	    *m_pNHintList;
	CButton     	*m_pNHintButton;
	CEdit     	    *m_pNHintEdit;

};

