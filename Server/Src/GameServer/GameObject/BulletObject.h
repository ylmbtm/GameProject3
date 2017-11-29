#ifndef __BULLET_OBJECT_H__
#define __BULLET_OBJECT_H__
#include "XMath.h"
#include "../ServerData/ServerDefine.h"
#include "../Message/Msg_Move.pb.h"

class CSceneObject;


class CBulletObject
{
public:
	CBulletObject(UINT64 uGuid, UINT32 dwID, FLOAT fAngle, FLOAT AttackFix, FLOAT AttackMuti, CSceneObject* pSrcObject);

	~CBulletObject();

public:
	BOOL OnUpdate( UINT64 uTick);

	BOOL SaveNewData(BulletNewNtf& Ntf);

	BOOL IsFinished();
public:
	UINT64      m_uGuid;		//子弹GUID
	UINT32		m_dwID;			//子弹ID
	UINT64		m_uStartTick;	//开始时间
	UINT64		m_uLastTick;	//上一次时间
	Vector3D	m_Pos;			//当前位置
	FLOAT		m_vx, m_vz;		//当前向量
	UINT64      m_uLifeTick;	//生命期
	BOOL		m_bFinished;	//是否完成
	CSceneObject* m_pSourceObject;	//源对象
	CSceneObject* m_pTargetObject;  //目标对象
};

#endif //__BULLET_OBJECT_H__
