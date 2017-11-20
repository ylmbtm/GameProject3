#include "stdafx.h"
#include "BulletObject.h"
#include "SceneObject.h"

CBulletObject::CBulletObject(CSceneObject* pObject)
{
	m_pSourceObject = pObject;

	m_x		= 0;
	m_y		= 0;
	m_z		= 0;
	m_vx	= 0;
	m_vz	= 0;
}

CBulletObject::~CBulletObject()
{
	m_pSourceObject = NULL;
	m_pTargetObject = NULL;
}

BOOL CBulletObject::OnUpdate(UINT64 uTick)
{
	m_x = m_vx * (uTick - m_uLastTick) / 1000;

	m_z = m_vz * (uTick - m_uLastTick) / 1000;

	return TRUE;
}

