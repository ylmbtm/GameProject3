#ifndef _WORLD_OBJECT_H_
#define _WORLD_OBJECT_H_
#include "Utility/Position.h"

class CWorldObject 
{
public:
	CWorldObject();

	~CWorldObject();


	CPosition&	GetPosition();

	VOID		SetPosition(FLOAT x, FLOAT y, FLOAT z);

public:

	CPosition  m_ObjectPos;
};

#endif //_WORLD_OBJECT_H_
