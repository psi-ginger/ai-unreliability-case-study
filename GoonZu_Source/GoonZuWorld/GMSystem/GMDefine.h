//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 파일 : GMDefine.h
//
// 설명 : GMTool전용 메시지 모음
//
// 날짜 : 2008.01.23
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// 기본정보 페이지 버튼 메시지
enum UPDATA_ID_BASICINFO
{
	UPDATA_ID_BASICINFO_NONE = 0,			// 아무것도 아님
	UPDATA_ID_BASICINFO_STATUS_RESET = 1,	// 스테이터스 리셋	
	UPDATA_ID_BASICINFO_MONEY_ADD,			// 사용자 머니 추가
	UPDATA_ID_BASICINFO_MONEY_SUB,			// 사용자 머니 차감
	UPDATA_ID_BASICINFO_BANKMONEY_ADD,		// 계좌 머니 추가
	UPDATA_ID_BASICINFO_BANKMONEY_SUB,		// 게좌 머니 차감
	UPDATA_ID_BASICINFO_ACU_ADD,			// 아큐 추가
	UPDATA_ID_BASICINFO_ACU_SUB,			// 아큐 차감
	UPDATA_ID_BASICINFO_STOCK_ADD,			// 주식 추가
	UPDATA_ID_BASICINFO_STOCK_SUB,			// 주식 차감
	UPDATA_ID_BASICINFO_BUF_MODIFY,			// 버프 수정
};

enum UPDATA_ID_CHARINFO
{
	UPDATA_ID_CHARINFO_REQUEST = 100,		// 캐릭터 정보 요청
};

// 스킬 페이지 버튼 메시지
enum UPDATA_ID_SKILL
{
	UPDATA_ID_SKILL_NONE = 0 //아무것도 아님
};

// 소지품 페이지 버튼 메시지
enum UPDATA_ID_INVENTORY
{
	UPDATA_ID_INVENTORY_NONE = 0,			// 아무것도 아님
	UPDATA_ID_INVENTORY_ITEM_ADD = 1		// 아이템을 넣어준다
};

// 부동산 페이지 버튼 메시지
enum UPDATA_ID_REALESTATE
{
	UPDATA_ID_REALESTATE_NONE = 0 //아무것도 아님
};

// 소환수 페이지 버튼 메시지
enum UPDATA_ID_SUMMON
{
	UPDATA_ID_SUMMON_NONE = 0 //아무것도 아님
};

// 동물(탈것) 페이지 버튼 메시지
enum UPDATA_ID_ANIMAL
{
	UPDATA_ID_ANIMAL_NONE = 0 //아무것도 아님
};

// 업데이트 페이지 메시지
enum UPDATA_MSG
{
	UPDATA_ERROR_MSG_NOT_DEFINE_ID = 0,		// 정의되지 않은 명령(버튼)을 수행했음
	UPDATA_ERROR_MSG_DO_NOT_RUN,			// 실행할 수 없음
	UPDATA_ERROR_MSG_NO_PERSONID,			// 존재하지 않는 케릭터
	UPDATA_ERROR_MSG_ONLY_CONNECT_USER,		// 접속한 케릭터에 대해서만 명령을 실행할 수 있다
	UPDATA_ERROR_MSG_ONLY_DISCONNECT_USER,	// 접속하지 않은 케릭터에 대해서만 명령을 실행할 수 있다
	UPDATA_ERROR_MSG_WRONG_MONEY,			// 돈 설정이 잘못 되어있다
	UPDATA_ERROR_MSG_WRONG_VILLAGE_UNIQUE,	// 마을 유니크 설정이 잘못 되었다
	UPDATA_ERROR_MSG_WRONG_STOCK_AMOUNT,	// 주식 수 설정이 잘못 되었다
	UPDATA_ERROR_MSG_WRONG_BUF_NUMBER,		// 버프 번호(Type) 설정이 잘못 되어있다
	UPDATA_ERROR_MSG_WRONG_BUF_SLOT,		// 버프 슬롯(배열 요소값) 설정이 잘못 되어있다
	UPDATA_ERROR_MSG_WRONG_BUF_TIME,		// 버프 시간설정이 잘못 되어있다

	UPDATA_ERROR_MSG_WRONG_ITEM_NAME,		// 아이템 이름이 잘못 되었다
	UPDATA_ERROR_MSG_WRONG_ITEM_NUM,		// 아이템 갯수가 잘못 되었다
	UPDATA_ERROR_MSG_WRONG_INVENTORY_SLOT,	// 인벤토리 슬롯 번호가 잘못 되었다
	UPDATA_ERROR_MSG_DO_NOT_MAKE_ITEM,		// 아이템을 생성할 수 없다
	// 에러메시지 추가시 이 메시지 위에 추가
	UPDATA_ERROR_MSG_END					// 에러메시지 끝 번호
};

const SI32 Const_UpData_Success = UPDATA_ERROR_MSG_END + 1;// 업데이트 작업이 성공 했다는 메시지

const SI32 Const_Max_Account_Num = 50;	// GMTool로 접속할수 있는 최대 접속자 수