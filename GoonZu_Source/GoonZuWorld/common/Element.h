#ifndef __CELEMENT__
#define __CELEMENT__

// 집합의 원소가 되는 클래스의 부모 클래스 
class CElement
{
private:
	int				m_key;							//  원소의 설정된 키값 
	DWORD			m_uniqID;						//  원소의 유일한 아이디 값 
	bool			m_bUsed;						//	현재 이 원소의 사용 여부 	
	
public:
	CElement();
	virtual ~CElement();

public:	
	void			setKey(int key);
	int				getKey();

	void			setUsed(bool which);
	bool			getUsed();

	void			setUniqID(DWORD uniqID);
	DWORD			getUniqID();
};

#endif