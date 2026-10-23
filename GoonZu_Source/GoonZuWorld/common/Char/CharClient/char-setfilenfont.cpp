
#include "Char\CharClient\Char-Client.h"

#include "..\..\Map\Map.h"


void cltCharClient::SetFileNFont()
{

	SI32 realimagexsize, realimageysize;

	// by LEEKH 2007.12.24 - TRANSFORM
	RealImageXPercent = GetKindInfo(true)->GetXsizePercent();
	RealImageYPercent = GetKindInfo(true)->GetYsizePercent();

	realimagexsize	= ImageXsize * RealImageXPercent/100;
	realimageysize	= ImageYsize * RealImageYPercent/100;
	
	RealImageStartX	= (ImageXsize-realimagexsize)/2;
	RealImageEndX	= RealImageStartX+realimagexsize;
	
	RealImageEndY	= ImageYsize - (GetKindInfo(true)->GetBottomIndent());
	RealImageStartY	= RealImageEndY-realimageysize;
}