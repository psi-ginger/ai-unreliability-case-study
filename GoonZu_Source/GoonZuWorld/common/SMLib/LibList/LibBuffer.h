#ifndef _LIBBUFFER_H_
#define _LIBBUFFER_H_

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CObj::CObj( SI32 siMaxBufferSize )
//	Desc : 주로 Socket 통신에서 Recv Buffer 또는 Send Buffer가
//		   이 클래스를 상속받아서 사용하면 편리함
//	Date : 2003. 11. 15
//	Update : 
//--------------------------------------------------------------------------------------------

class CLibBuffer
{
public:
	CLibBuffer();
	virtual ~CLibBuffer();

	void Initialize();
	void Destroy();


private:

};


#endif