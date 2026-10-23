#ifndef _MSGTYPEBANK_H
#define _MSGTYPEBANK_H

class cltGameMsgRequest_CreateBankAccount
{
public:
	SI32 siVillageUnique;

	cltGameMsgRequest_CreateBankAccount(SI32 villageunique)
	{
		siVillageUnique = villageunique;
	}
};


class cltGameMsgRequest_CloseBankAccount
{
public:
	SI32 siVillageUnique;

	cltGameMsgRequest_CloseBankAccount(SI32 villageunique)
	{
		siVillageUnique = villageunique;
	}
};

class cltGameMsgRequest_GMoney
{
public :
	GMONEY m_money;

	cltGameMsgRequest_GMoney (GMONEY money)
	{
		m_money = money;
	}


};

#endif
