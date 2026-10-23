#include <NkMsgQueue.h>

#include "display_helper.h"

#define EXCUTE_NKMSGQUEUE(a) a; CDispHelper::Get().PrintSource(#a); CDispHelper::disp_msgqueue(&kMsgQueue); _getch();


void NkMsgQueueSample()
{

}