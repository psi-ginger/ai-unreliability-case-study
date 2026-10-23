#pragma once
//[성웅] 라쿤이벤트 군주 이벤트로 변경

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"

enum
{
	FirstMapIndex = 0 ,	//맵인덱스 1
	SecondMapIndex,		//맵인덱스 2
	ThirdMapIndex,		//맵인덱스 3
};
enum
{
	FirstRaccoonTime = 0 ,	//라쿤타임 1
	SecondRaccoonTime,		//라쿤타임 2
	ThirdRaccoonTime,		//라쿤타임 3
};
#define MAX_DATA_MAP_NUM	20			// 리소스에 넣을수 있는 맵 정보는 20개 까지
#define MAX_DATA_MAPCOMBOBOX_NUM	9	// 맵콤보박스 갯수
#define MAX_DATA_TIME_NUM	24			// 리소스에 넣을수 있는 시간은 24개 까지 
#define MAX_MAP_NUM	3					// 가지고 있을 맵의 종류
// 리소스에서 읽어 들인 정보를 가지고 있을 클래스
class CNGoonzuRaccoonEventDlg : public CDialogController
{
public:

	InterfaceFactory	m_InterfaceFactory_;

	CNGoonzuRaccoonEventDlg();
	~CNGoonzuRaccoonEventDlg();

	void init();
	void Create();
	static void CALLBACK StaticCallBackDialogNGoonzuRaccoonEventDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNGoonzuRaccoonEventDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void AddHuntmaps();
	void AddCombo(TCHAR* maptypename);
	void Action()	;

	void Select_FiledComboBox(SI32 siComboBoxID, SI32 siRaccoonTimeIndex, SI32 siMapIndexArray)	;
	void Select_TimeComboBox(SI32 siComboBoxID, SI32 siRaccoonTimeIndex)	;
	void Select_MonsterNumComboBox(SI32 siComboBoxID, SI32 siRaccoonTimeIndex)	;

	void SetComboX(cGoonzuRaccoonEventData* pData,SI32 siRaccoonTime)	;	// 데이터와 몇번째 라쿤인지도 넣어줘야 한다.
	void SetComboBox_Cursel( SI32 siComboBoxID , TCHAR* pText)	;
	void SetComboBox_Cursel( SI32 siComboBoxID , SI32 siNumber )	;

	void EnableComboBox_Map(SI32 siComboBoxID,SI32 siMonsterNum)	;	// 맵콤보 박스를 enable시키기 위해서 

	void BeforeCheckData()	;			// 데이터를 날리기 전에 날리는 데이터가 들어있는지 확인 한다 , 빈데이터라면 보낼 필요가 없다 

	bool CheckSetMapData( SI32 siMapname )	;			// 적절한 맵데이터인지 확인
private:
	RocData GoonzuRaccoonLoadData[MAX_DATA_MAP_NUM]	;
	cGoonzuRaccoonEventData GoonzuRaccoonEventData[MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT]	; //서버에 보낼 데이터. 콤보박스를클릭하면 해당 데이터를 이 클래스에 넣어서 보낸다 .
};