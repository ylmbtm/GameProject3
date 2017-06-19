#ifndef _OBJECT_POS_
#define _OBJECT_POS_

class CPosition
{
public:
	CPosition(void)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	~CPosition(void)
	{

	}

public:

	FLOAT x, y, z;

};

#endif //_OBJECT_POS_