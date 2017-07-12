#ifndef _CHAR_OBJECT_H_
#define _CHAR_OBJECT_H_
#include "WorldObject.h"

class CCharObject : public CWorldObject
{
public:
	CCharObject();

	~CCharObject();

	char			m_szObjectName[1024];
};

#endif //_CHAR_OBJECT_H_
