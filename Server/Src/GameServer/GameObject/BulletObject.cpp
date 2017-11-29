#include "stdafx.h"
#include "BulletObject.h"
#include "SceneObject.h"

CBulletObject::CBulletObject(UINT64 uGuid, UINT32 dwID, FLOAT fAngle, FLOAT AttackFix, FLOAT AttackMuti, CSceneObject* pSrcObject)
{
	m_pSourceObject = pSrcObject;
	m_pTargetObject = NULL;
	m_bFinished = FALSE;
	m_vx = 0;
	m_vz = 0;
}

CBulletObject::~CBulletObject()
{
	m_pSourceObject = NULL;
	m_pTargetObject = NULL;
	m_bFinished = FALSE;
	m_vx = 0;
	m_vz = 0;
}

BOOL CBulletObject::OnUpdate(UINT64 uTick)
{
	m_Pos.m_x += m_vx * (uTick - m_uLastTick) / 1000;

	m_Pos.m_z += m_vz * (uTick - m_uLastTick) / 1000;

	if ((uTick - m_uStartTick) > m_uLifeTick)
	{
		m_bFinished = TRUE;
	}

	return TRUE;
}

BOOL CBulletObject::SaveNewData(BulletNewNtf& Ntf)
{
	BulletItem* pItem = Ntf.add_itemlist();

	pItem->set_objectguid(m_uGuid);
	pItem->set_bulletid(m_dwID);
	pItem->set_x(m_Pos.m_x);
	pItem->set_y(m_Pos.m_y);
	pItem->set_z(m_Pos.m_z);
	pItem->set_vx(m_vx);
	pItem->set_vz(m_vz);
	pItem->set_speed(m_vx);
	pItem->set_accspeed(m_vz);
	pItem->set_accspeed(m_vz);

	return TRUE;
}

BOOL CBulletObject::IsFinished()
{
	return m_bFinished;
}

