#ifndef _NBILLSERVER_H
#define _NBILLSERVER_H

#include "ITZPerson.h"

class cGame : public ITZPerson
{
public:
	cGame() {};
	~cGame() {};

public:
	virtual void Init() { printf( "GameServer Connected\n" ); };
	virtual void Close() { printf( "GameServer Disconnected\n" ); };

};

#endif