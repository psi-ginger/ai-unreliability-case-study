// XThreadPool.h
/*!
\file		XThreadPool.h

- Required header: <XBase/XSystem/XThreadPool.h>
*/

#ifndef __XThreadPool_Old__
#define __XThreadPool_Old__

#include "XSystemBase.h"
#include <Windows.h>

/*!
\brief					새로운 thread pool을 생성한다.

\param dwSize			[in] 생성할 thread 갯수

\return					생성된 thread pool의 handle

\sa						XDestroyThreadPool, XGetThreadPoolSize, XQueueWorkItem, XBindCompletionCallback
*/
XSYSTEM_API HANDLE __stdcall XCreateThreadPool(DWORD dwSize);

/*!
\brief					thread pool을 제거한다.

\param dwSize			[in] 제거할 thread pool의 handle

\return					리턴값이 TRUE이면 함수가 성공적으로 수행되었음을 의미한다.

\sa						XCreateThreadPool, XGetThreadPoolSize, XQueueWorkItem, XBindCompletionCallback
*/
XSYSTEM_API BOOL __stdcall XDestroyThreadPool(HANDLE hThreadPool);

/*!
\brief					thread pool의 크기를 반환한다.

\param dwSize			[in] 크기를 얻어올 thread pool의 handle

\return					해당 thread pool의 크기 (0 미만일 경우 오류)

\sa						XCreateThreadPool, XDestroyThreadPool, XQueueWorkItem, XBindCompletionCallback
*/
XSYSTEM_API int __stdcall XGetThreadPoolSize(HANDLE hThreadPool);

/*!
\brief					실행을 위해 function을 thread pool에 queuing 한다.

\param hThreadPool		[in] function이 실행될 thread pool의 handle
\param pFunction		[in] 실행할 function
\param pParam			[in] pFunction 실행시 함께 전달할 인자

\return					리턴값이 TRUE이면 thread pool에 성공적으로 queuing되었음을 의미한다.

\sa						XCreateThreadPool, XDestroyThreadPool, XGetThreadPoolSize, XBindCompletionCallback
*/
XSYSTEM_API BOOL __stdcall XQueueWorkItem(HANDLE hThreadPool, LPTHREAD_START_ROUTINE pFunction, PVOID pParam);

/*!
\brief					실행을 위해 function을 thread pool에 queuing 한다.

\param hThreadPool		[in] function이 실행될 thread pool의 handle
\param pFunction		[in] 실행할 function
\param dwErrorCode		[in] pFunction 실행시 함께 전달할 첫번째 인자
\param dwNumberOfBytesTransfered
						[in] pFunction 실행시 함께 전달할 두번째 인자
\param pOverlapped		[in] pFunction 실행시 함께 전달할 세번째 인자

\return					리턴값이 TRUE이면 thread pool에 성공적으로 queuing되었음을 의미한다.

\sa						XCreateThreadPool, XDestroyThreadPool, XGetThreadPoolSize, XBindCompletionCallback
*/
XSYSTEM_API BOOL __stdcall XQueueWorkItem(HANDLE hThreadPool, LPOVERLAPPED_COMPLETION_ROUTINE pFunction, DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped);

/*!
\brief					thread pool의 I/O completion port에 file handle을 bind한다.

\param hThreadPool		[in] bind할 thread pool의 handle

\param hHandle			[in] bind할 file handle

\param pfnCompletionCallback
						[in] file에 request된 I/O 작업이 완료될 경우 호출될 callback function. \n
						(callback function에 대한 자세한 정보는 MSDN의 FileIOCompletionRoutine 항목 참고)

\return					리턴값이 TRUE이면 thread pool에 성공적으로 queuing되었음을 의미한다.

\remarks				thread pool의 I/O completion port에 file handle을 bind한다.
						bind된 file에 request된 I/O 작업이 완료될 경우 thread pool의 worker thread에 의해 등록된 
						callback function이 호출된다.

\sa						XCreateThreadPool, XDestroyThreadPool, XGetThreadPoolSize, XBindCompletionCallback
*/
XSYSTEM_API BOOL __stdcall XBindCompletionCallback(HANDLE hThreadPool, HANDLE hHandle, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionCallback);

#endif //__XThreadPool__
