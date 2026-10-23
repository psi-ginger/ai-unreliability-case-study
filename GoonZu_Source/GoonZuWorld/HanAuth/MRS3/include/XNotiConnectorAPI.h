#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#ifdef XNOTICONNECTOR_EXPORTS
		#define XNOTICONNECTOR_API __declspec(dllexport)
	#else
		#define XNOTICONNECTOR_API __declspec(dllimport)
	#endif
#else
	#define XNOTICONNECTOR_API
#endif

namespace XNoti
{
	namespace Connector
	{
		typedef struct _THandle
		{
		} *THandle;

		typedef bool (__stdcall *TXNotiMessageHandler)(XStream::Handle stream, void* additionalData, void* context);
		
		XNOTICONNECTOR_API THandle	__stdcall CreateHandle(void );
		XNOTICONNECTOR_API bool		__stdcall DestroyHandle(THandle xnotiConnector);
		XNOTICONNECTOR_API bool		__stdcall Initialize
			(
				THandle xnotiConnector,
				char* xnotiServiceName,
				char* serviceName,
				char* serviceInstanceID,
				int intervalCheckServiceAvailabillity = 0,
				int intervalReportGameServiceInstanceInfo = 60
			);

		XNOTICONNECTOR_API bool		__stdcall RegistrationMassageHandler
			(
				THandle xnotiConnector,
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

		XNOTICONNECTOR_API bool		__stdcall SendToAnyService
			(
				THandle xnotiConnector,
				XDR::IMessage& xdrMessage
			);

		XNOTICONNECTOR_API bool		__stdcall SendToAllService
			(
				THandle xnotiConnector,
				XDR::IMessage& xdrMessage
			);
	}
}
