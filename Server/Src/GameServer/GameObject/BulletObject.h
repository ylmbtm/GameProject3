#ifndef __BULLET_OBJECT_H__
#define __BULLET_OBJECT_H__
#include "XMath.h"
#include "../Message/Msg_Move.pb.h"
#include "../StaticData/SkillStruct.h"

class CSceneObject;


class CBulletObject
{
public:
	CBulletObject(UINT64 uGuid, StBulletInfo* pBulletInfo, FLOAT fAngle);

	~CBulletObject();

public:
	BOOL OnUpdate( UINT64 uTick);

	BOOL SaveNewData(BulletNewNtf& Ntf);

	BOOL SetCastObject(CSceneObject* pObject);

	BOOL SetTargetObject(CSceneObject* pObject);

	BOOL IsFinished();
public:
	UINT64      m_uGuid;		//子弹GUID
	UINT64		m_uStartTick;	//开始时间
	UINT64		m_uLastTick;	//上一次时间
	Vector3D	m_Pos;			//当前位置
	FLOAT		m_vx, m_vz;		//当前速度向量
	BOOL		m_bFinished;	//是否完成
	StBulletInfo* m_pBulletInfo; //子弹信息
	CSceneObject* m_pCastObject;	//源对象
	CSceneObject* m_pTargetObject;  //目标对象
};

#endif //__BULLET_OBJECT_H__
