@echo off
::
:: 이 배치 파일은 프로젝트의 헤더 파일을 SDK의 Include폴더로 복사한다.
:: 또한 복사된 파일들을 속성을 읽기 전용으로 만든다.
::

:: FIND THE SDK DIRECTORY 
:: Prefer the most local sdk dir, 
:: so order these checks such that local dirs set the variable last.
set SDK_INCLUDE=not_found
if exist ..\..\SDK\NkCore       set SDK_INCLUDE=..\..\SDK\NkCore\Include
if exist ..\..\..\SDK\NkCore    set SDK_INCLUDE=..\..\..\SDK\NkCore\Include
if (%SDK_INCLUDE%)==(not_found) goto :ERROR_NO_SDK

echo (새로 추가된 파일이나 수정된 파일만 복사)

:: COPY STANDARD FILES
if exist ..\*Nk*.h      xcopy /D /F /I /R /Y ..\*Nk*.h      %SDK_INCLUDE%
if exist ..\*Nk*.inl    xcopy /D /F /I /R /Y ..\*Nk*.inl    %SDK_INCLUDE%

:: COPY USER SPECIFIED FILES
:==LOOP
if (%~1)==() goto LOOP_END
if exist "%~1" xcopy /D /F /I /R /Y "%~1" %SDK_INCLUDE%
shift
goto LOOP
:==LOOP_END

:: MARK FILES READ ONLY
if exist %SDK_INCLUDE% attrib +R %SDK_INCLUDE%\*

exit /b

:==ERROR_NO_SDK
echo %0: 치명적인 오류: SDK 폴더를 찾을 수 없습니다.
exit 1

