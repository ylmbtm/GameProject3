#include "stdafx.h"
#include "SceneObject.h"
#include "../GameService.h"
#include "../Message/Msg_Copy.pb.h"
#include "BuffObject.h"
#include "Log.h"

CSceneObject::CSceneObject(UINT64 uGuid, UINT32 dwActorID, UINT32 dwObjType, UINT32 dwCamp, std::string strName)
{
	m_dwProxyConnID = 0;
	m_dwClientConnID = 0;
	m_dwObjState = 0;
	m_bEnter = FALSE;

	memset(m_Propertys, 0, sizeof(m_Propertys));

	m_uGuid = uGuid;
	m_dwActorID = dwActorID;
	m_dwObjType = dwObjType;
	m_dwCamp = dwCamp;
	m_strName = strName;

	m_bDataChange = FALSE;

	m_SkillObject.SetHostObject(this);
}

CSceneObject::~CSceneObject()
{

}

//BOOL CSceneObject::StartSkill()
//{
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

//return TRUE;
//}

BOOL CSceneObject::SetConnectID(UINT32 dwProxyID, UINT32 dwClientID)
{
	m_dwProxyConnID = dwProxyID;
	m_dwClientConnID = dwClientID;

	return TRUE;
}

// BOOL CPlayerObject::AttackEvent()
// {
// 	UINT32  SkillID;
// 	UINT32  targets[];
//
//
// }


BOOL CSceneObject::SendMsgProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata)
{
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwProxyConnID, dwMsgID, GetObjectGUID(), m_dwClientConnID, pdata);
}

BOOL CSceneObject::SendMsgRawData(UINT32 dwMsgID, const char* pdata, UINT32 dwLen)
{
	return ServiceBase::GetInstancePtr()->SendMsgRawData(m_dwProxyConnID, dwMsgID, GetObjectGUID(), m_dwClientConnID, pdata, dwLen);
}

BOOL CSceneObject::OnUpdate( UINT32 dwTick )
{
	UpdateBuff(dwTick);

	return TRUE;
}

UINT32 CSceneObject::GetHp()
{
	return m_Propertys[HP];
}

UINT32 CSceneObject::GetMp()
{
	return m_Propertys[MP];
}

VOID CSceneObject::AddHp( UINT32 dwValue )
{
	m_Propertys[HP] += dwValue;
}

VOID CSceneObject::SubHp( UINT32 dwValue )
{
	m_Propertys[HP] -= dwValue;

	if(m_Propertys[HP] < 0)
	{
		m_Propertys[HP] = 0;
	}
}

VOID CSceneObject::AddMp( UINT32 dwValue )
{
	m_Propertys[MP] += dwValue;
}

VOID CSceneObject::SubMp( UINT32 dwValue )
{
	m_Propertys[MP] -= dwValue;

	if(m_Propertys[MP] < 0)
	{
		m_Propertys[MP] = 0;
	}
}

UINT64 CSceneObject::GetObjectGUID()
{
	return m_uGuid;
}

UINT32 CSceneObject::GetActorID()
{
	return m_dwActorID;
}

UINT32 CSceneObject::GetObjType()
{
	return m_dwObjType;
}

UINT32 CSceneObject::GetCamp()
{
	return m_dwCamp;
}

BOOL CSceneObject::IsConnected()
{
	return (m_dwClientConnID != 0) && (m_dwProxyConnID != 0);
}

BOOL CSceneObject::IsEnterCopy()
{
	return m_bEnter;
}

VOID CSceneObject::SetEnterCopy()
{
	m_bEnter = TRUE;
}

BOOL CSceneObject::SaveNewObject( ObjectNewNty& Nty )
{
	NewItem* pItem = Nty.add_newlist();
	pItem->set_objectguid(m_uGuid);
	pItem->set_objtype(m_dwObjType);
	pItem->set_actorid(m_dwActorID);
	pItem->set_name(m_strName);
	pItem->set_level(m_dwLevel);
	pItem->set_summonid(m_uSummonerID);
	pItem->set_controlerid(m_uControlerID);
	pItem->set_hostguid(m_uHostGuid);
	pItem->set_x(m_x);
	pItem->set_x(m_y);
	pItem->set_z(m_z);
	pItem->set_ft(m_ft);;
	pItem->set_hp(m_Propertys[HP]);
	pItem->set_mp(m_Propertys[MP]);
	pItem->set_hpmax(m_Propertys[HP_MAX]);
	pItem->set_mpmax(m_Propertys[MP_MAX]);

	return TRUE;
}

BOOL CSceneObject::IsDie()
{
	if(m_Propertys[HP] <= 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CSceneObject::SetPos(FLOAT x, FLOAT y,  FLOAT z, FLOAT ft)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_ft = ft;
	return TRUE;
}

UINT32 CSceneObject::GetLastSkillTime(UINT32 dwSkillID)
{
	auto itor = m_mapSkillTime.find(dwSkillID);
	if(itor != m_mapSkillTime.end())
	{
		return itor->second;
	}

	return 0;
}

BOOL CSceneObject::SetLastSkillTime(UINT32 dwSkillID, UINT32 dwTime)
{
	m_mapSkillTime.insert(std::make_pair(dwSkillID, dwTime));

	return TRUE;
}

BOOL CSceneObject::SaveBattleResult(ResultPlayer* pResult)
{
	pResult->set_objectid(m_uGuid);
	pResult->set_actorid(m_dwActorID);
	pResult->set_result(m_dwResult);
	pResult->set_damage(m_dwDamage);

	return TRUE;
}

BOOL CSceneObject::AddBuff(UINT32 dwBuffID)
{
	CBuffObject* pBuffObject = new CBuffObject(this, dwBuffID);

	ERROR_RETURN_FALSE(pBuffObject != NULL);

	pBuffObject->OnAddBuff();

	m_mapBuff.insert(std::make_pair(dwBuffID, pBuffObject));

	return TRUE;
}

BOOL CSceneObject::UpdateBuff(UINT32 dwTick)
{
	for(std::map<UINT32, CBuffObject*>::iterator itor = m_mapBuff.begin(); itor != m_mapBuff.end();)
	{
		CBuffObject* pBuffObject = itor->second;
		ERROR_CONTINUE_EX(pBuffObject != NULL);

		pBuffObject->OnUpdate(dwTick);

		if(pBuffObject->IsOver())
		{
			delete pBuffObject;

			itor = m_mapBuff.erase(itor);
		}
		else
		{
			itor++;
		}
	}


	return TRUE;
}

BOOL CSceneObject::StartSkill(const ActionReqItem& Item)
{
	ERROR_RETURN_FALSE(m_pScene != NULL);

	m_x = Item.hostx();
	m_y = Item.hosty();
	m_z = Item.hostz();
	m_ft = Item.hostft();

	ActionNtyItem* pSvrItem = m_pScene->m_ObjectActionNty.add_actionlist();
	ERROR_RETURN_TRUE(pSvrItem != NULL);
	pSvrItem->set_objectguid(Item.objectguid());
	pSvrItem->set_objstatue(m_dwObjState);			//实例状态
	pSvrItem->set_actorid(m_dwActorID);				//类型ID
	pSvrItem->set_controlerid(m_uControlerID);		//AI控制人的GUID
	pSvrItem->set_actionid(Item.actionid());
	pSvrItem->set_actiontime(0);
	pSvrItem->set_x(m_x);
	pSvrItem->set_y(m_y);
	pSvrItem->set_z(m_z);
	pSvrItem->set_ft(m_ft);
	pSvrItem->set_hp(GetHp());
	pSvrItem->set_mp(GetMp());
	pSvrItem->set_hpmax(1000);
	pSvrItem->set_mpmax(1000);

	m_bDataChange = FALSE;

	return TRUE;
}

BOOL CSceneObject::StartAction(const ActionReqItem& Item)
{
	ERROR_RETURN_FALSE(m_pScene != NULL);

	m_x = Item.hostx();
	m_y = Item.hosty();
	m_z = Item.hostz();
	m_ft = Item.hostft();

	m_bDataChange = TRUE;

	return TRUE;
}
