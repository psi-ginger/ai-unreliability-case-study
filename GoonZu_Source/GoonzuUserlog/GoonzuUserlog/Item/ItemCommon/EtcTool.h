#ifndef _ETCTOOL_H
#define _ETCTOOL_H

#include <Windows.h>
#include <Directives.h>

#include <Stdio.h>

void MsgBox(char * szCaption, char*szFormat,... );


// 소팅을 위해 데이터를 추가한다. 
bool SortAdd(SI32* pdst, SI32* pdata,  SI32* pmindata, 	 SI32 data1,   SI32 data2,  SI32 maxnum);

#endif
