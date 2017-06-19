#include "stdafx.h"
#include "PlayerObject.h"

CPlayerObject::CPlayerObject()
{
}

CPlayerObject::~CPlayerObject()
{

}

BOOL CPlayerObject::DispatchPacket(NetPacket *pNetPack)
{
	
	return TRUE;
}

UINT32 CPlayerObject::GetConnectID()
{
	return m_dwConnID;
}

VOID CPlayerObject::SetConnectID( UINT32 dwConnID )
{
	m_dwConnID = dwConnID;

	return ;
}

BOOL CPlayerObject::StartSkill()
{
	//UINT32 SkillID;
	//UINT32 targets[1];
	//BOOL bKill;

	//需要检

	/*
	1. 技能是否存在。
	2. 技能CD是否可以施放。
	3. 技能是否可以打中指定的目标.
	4. 玩家是否增加buff.


	//技能分类。
	1. 立即攻击技有, 只有CD, 方向， 
	2. 增加buff的技能， 就增加buff.
	3. 位移技能， 需要带位移长度。
	4. 波次技能。 就是每一帆的收集攻击的对象.就是分成两部分。
	5. 时长技能。 放出去就有位移，由客户端自己算打中谁，服务器验证一下。就转发。



	//实现设想两种。
	1. 一放一个技能，就new一个技能对象。
	在线这个技能对象里就update，不断的update，由技能本身去改变状态，寻找攻击目标.


	2. 服务器不管过程。
	由客户端去驱动攻击状态。
	*/


	/*
	buff类型, buff的特效就不用管了。
	1. 增加属性, 加上就上， 减掉就取消。
	2. 修改状态, 
	3. 每秒造成伤害
	4. 攻击时增加伤害
	5. 持线伤害

	玩家需要对应的状态
	1. 无敌。
	2. 不能移动。
	*/

	return TRUE;
}

// BOOL CPlayerObject::AttackEvent()
// {
// 	UINT32  SkillID;
// 	UINT32  targets[];
// 
// 
// }

BOOL CPlayerObject::IsChanged()
{

	return TRUE;
}

BOOL CPlayerObject::ClearChangeFlag()
{

	return TRUE;
}

UINT32 CPlayerObject::WriteToBuffer( UINT32 dwChangeFlag, UINT32 dwDest )
{
	UINT32 dwSize = 0;
	dwSize += CCharObject::WriteToBuffer(dwChangeFlag, dwDest);
	
	if(dwChangeFlag == UPDATE_FLAG_CHANGE)
	{
		
	}
	else if(dwChangeFlag == UPDATE_FLAG_CREATE)
	{
		
	}

	return dwSize;
}

UINT32 CPlayerObject::ReadFromBuffer(UINT32 dwChangeFlag)
{
	UINT32 dwSize = 0;
	dwSize += CCharObject::ReadFromBuffer(dwChangeFlag);

	if(dwChangeFlag == UPDATE_FLAG_CHANGE)
	{

	}
	else if(dwChangeFlag == UPDATE_FLAG_CREATE)
	{
	
	}


	return dwSize;
}

BOOL CPlayerObject::OnUpdate( UINT32 dwTick )
{

	return TRUE;
}



BOOL CPlayerObjectMgr::AddPlayer( CPlayerObject *pObject )
{
	insert(std::make_pair(pObject->GetObjectID(), pObject));

	return TRUE;
}

CPlayerObject* CPlayerObjectMgr::GetPlayer( UINT64 ObjectID )
{
	iterator itor  = find(ObjectID);
	if(itor != end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL CPlayerObjectMgr::RemovePlayer( UINT64 ObjectID )
{
	if(GetPlayer(ObjectID) == NULL)
	{
		ASSERT_FAIELD;
	}

	erase(ObjectID);

	return TRUE;
}

BOOL CUpdateObjectMgr::AddUpdateObject( CWorldObject *pObject )
{
	insert(pObject);

	return TRUE;
}

CWorldObject* CUpdateObjectMgr::GetFisrtOjbect()
{
	iterator itor = begin();
	if(itor == end())
	{
		return NULL;
	}

	CWorldObject *pWorldObject = *itor;

	erase(itor);

	return pWorldObject;
}

BOOL CUpdateObjectMgr::RemoveUpdateObject( CWorldObject *pObject )
{
	iterator itor = find(pObject);
	if(itor == end())
	{
		return FALSE;
	}

	erase(itor);

	return TRUE;
}


