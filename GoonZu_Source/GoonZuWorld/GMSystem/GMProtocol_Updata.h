#include "ITZPerson.h"
#include "ITZNetworkThread.h"
#include "GMSectionData.h"


struct sGMRequest_UpdateData;
class cltCharServer;


class CGMProtocol_Updata
{
public:
	// 케릭터 기본정보에 관련된 데이터 변환
	SI16	BasicInfoUpdata( sGMRequest_UpdateData* pRecvMsg , SI32 siButtonID , cltCharServer* pclChar );
	// 스킬에 관련된 데이터 변환
	SI16	SkillInfoUpdata( sGMRequest_UpdateData* pRecvMsg , SI32 siButtonID , cltCharServer* pclChar );
	// 소지품에 관련된 데이터 변환
	SI16	InventoryUpdata( sGMRequest_UpdateData* pRecvMsg , SI32 siButtonID , cltCharServer* pclChar );
	// 부동산에 관련된 데이터 변환
	SI16	RealestateUpdata( sGMRequest_UpdateData* pRecvMsg , SI32 siButtonID , cltCharServer* pclChar );
	// 소환수에 관련된 데이터 변환
	SI16	SummonUpdata( sGMRequest_UpdateData* pRecvMsg , SI32 siButtonID , cltCharServer* pclChar );
	// 동물에 관련된 데이터 변환
	SI16	AnimalUpdata( sGMRequest_UpdateData* pRecvMsg , SI32 siButtonID , cltCharServer* pclChar );

};