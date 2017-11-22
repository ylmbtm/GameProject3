#ifndef __BULLET_OBJECT_H__
#define __BULLET_OBJECT_H__

#include "../ServerData/ServerDefine.h"
#include "XMath.h"
class CSceneObject;


class CBulletObject
{
public:
	CBulletObject(CSceneObject* pObject);

	~CBulletObject();

public:
	BOOL OnUpdate( UINT64 uTick);

public:
	UINT32		m_dwID;		  //子弹ID
	UINT64		m_uStartTick; //开始时间
	UINT64		m_uLastTick;
	Vector3D	m_Pos;
	FLOAT		m_vx, m_vz;
	CSceneObject* m_pSourceObject;	//源对象
	CSceneObject* m_pTargetObject;  //目标对象
};

#endif //__BULLET_OBJECT_H__
