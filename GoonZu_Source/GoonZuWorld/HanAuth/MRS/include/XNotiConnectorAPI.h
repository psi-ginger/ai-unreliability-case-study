#pragma once

#ifdef XNOTICONNECTOR_EXPORTS
#define XNOTICONNECTOR_API __declspec(dllexport)
#else
#define XNOTICONNECTOR_API __declspec(dllimport)
#endif

namespace XNoti
{
	namespace Connector
	{
		enum XNotiNetType{ EnumNetType_Anycast, EnumNetType_Multicast };

		typedef struct _THandle
		{
		} *THandle;

		typedef bool (__stdcall *TXNotiMessageHandler)(XStream::Handle stream, void* additionalData, void* context);
		
		XNOTICONNECTOR_API THandle	__stdcall CreateHandle(void );
		XNOTICONNECTOR_API bool		__stdcall DestroyHandle(THandle xnotiConnector);
		XNOTICONNECTOR_API bool		__stdcall Initialize
			(
				THandle hXNotiConnector,
				char* xnotiServiceName,
				char* serviceName,
				char* serviceInstanceID,
				int netType = EnumNetType_Anycast,
				int intervalCheckServiceAvailabillity = 0,
				int intervalReportGameServiceInstanceInfo = 60
			);

		XNOTICONNECTOR_API bool		__stdcall RegistrationMassageHandler
			(
				THandle hConnector,
				int nMsgID,
				TXNotiMessageHandler fnMsgHandler,
				void* additionalData = NULL
			);

		//XNOTICONNECTOR_API bool		__stdcall UnregistrationMassageHandler
		//	(
		//		THandle hConnector,
		//		int nMsgID
		//	);

		XNOTICONNECTOR_API bool		__stdcall SendAnswerMessage
			(
				void* context,
				XDR::IMessage& xdrMessage
			);
	}
}