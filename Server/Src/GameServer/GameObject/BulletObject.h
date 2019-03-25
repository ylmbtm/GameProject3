#ifndef __BULLET_OBJECT_H__
#define __BULLET_OBJECT_H__
#include "XMath.h"
#include "../Message/Msg_Move.pb.h"

class CSceneObject;


class CBulletObject
{
public:
	CBulletObject(UINT64 uGuid, UINT32 dwID, UINT32 dwType, FLOAT fAngle);

	~CBulletObject();

public:
	BOOL OnUpdate( UINT64 uTick);

	BOOL SaveNewData(BulletNewNtf& Ntf);

	BOOL SetCastObject(CSceneObject* pObject);

	BOOL SetTargetObject(CSceneObject* pObject);

	BOOL IsFinished();
public:
	UINT64      m_uGuid;		//子弹GUID
	UINT32		m_dwType;		//子弹类型
	UINT32		m_dwID;			//子弹ID
	UINT64		m_uStartTick;	//开始时间
	UINT64		m_uLastTick;	//上一次时间
	Vector3D	m_Pos;			//当前位置
	FLOAT		m_vx, m_vz;		//当前速度向量
	UINT64      m_uLifeTick;	//生命期
	BOOL		m_bFinished;	//是否完成
	FLOAT		m_fSpeed;		//整度
	FLOAT		m_fAccSpeed;	//加速度
	CSceneObject* m_pCastObject;	//源对象
	CSceneObject* m_pTargetObject;  //目标对象
};

#endif //__BULLET_OBJECT_H__
