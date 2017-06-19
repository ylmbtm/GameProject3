#ifndef _WORLD_OBJECT_H_
#define _WORLD_OBJECT_H_
#include "Utility/Position.h"
#include "Object.h"

class CWorldObject : public CGObject
{
public:
	CWorldObject();

	~CWorldObject();

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper);

	CPosition&	GetPosition();

	VOID		SetPosition(FLOAT x, FLOAT y, FLOAT z);

public:

	CPosition  m_ObjectPos;
};

#endif //_WORLD_OBJECT_H_
