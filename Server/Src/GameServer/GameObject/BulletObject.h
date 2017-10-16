#ifndef __BULLET_OBJECT_H__
#define __BULLET_OBJECT_H__

#include "../ServerData/ServerDefine.h"

class CSceneObject;


class CBulletObject
{
public:
	CBulletObject(CSceneObject* pObject);

	~CBulletObject();

public:
	BOOL OnUpdate( UINT32 dwTick);

public:
	UINT32  m_dwStartTick; //开始时间

	CSceneObject* m_pSceneObject;
};

#endif //__BULLET_OBJECT_H__
