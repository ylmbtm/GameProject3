#include "stdafx.h"
#include "SceneObject.h"
#include "../GameService.h"
#include "../Message/Msg_Copy.pb.h"
#include "BuffObject.h"
#include "Log.h"
#include "../Message/Msg_Move.pb.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "CommonFunc.h"
#include "../ConfigData/ConfigData.h"

CSceneObject::CSceneObject(UINT64 uGuid, UINT32 dwActorID, UINT32 dwObjType, UINT32 dwCamp, std::string strName)
{
	m_dwProxyConnID = 0;
	m_dwClientConnID = 0;
	m_dwObjectState = 0;
	m_bEnter = FALSE;

	memset(m_Propertys, 0, sizeof(m_Propertys));

	m_uGuid = uGuid;
	m_dwActorID = dwActorID;
	m_dwObjType = dwObjType;
	m_dwCamp = dwCamp;
	m_strName = strName;

	m_bDataChange = FALSE;

	m_SkillObject.SetCastObject(this);
}

CSceneObject::~CSceneObject()
{

}


BOOL CSceneObject::SetConnectID(UINT32 dwProxyID, UINT32 dwClientID)
{
	m_dwProxyConnID = dwProxyID;
	m_dwClientConnID = dwClientID;

	return TRUE;
}

BOOL CSceneObject::SendMsgProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata)
{
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwProxyConnID, dwMsgID, GetObjectGUID(), m_dwClientConnID, pdata);
}

BOOL CSceneObject::SendMsgRawData(UINT32 dwMsgID, const char* pdata, UINT32 dwLen)
{
	return ServiceBase::GetInstancePtr()->SendMsgRawData(m_dwProxyConnID, dwMsgID, GetObjectGUID(), m_dwClientConnID, pdata, dwLen);
}

BOOL CSceneObject::OnUpdate( UINT64 uTick )
{
	UpdateBuff(uTick);

	m_SkillObject.OnUpdate(uTick);

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

BOOL CSceneObject::IsChanged()
{
	return m_bDataChange;
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

BOOL CSceneObject::SaveNewData( ObjectNewNty& Nty )
{
	NewItem* pItem = Nty.add_newlist();
	pItem->set_objectguid(m_uGuid);
	pItem->set_objtype(m_dwObjType);
	pItem->set_actorid(m_dwActorID);
	pItem->set_buffstate(m_dwBuffState);
	pItem->set_objectstate(m_dwObjectState);
	pItem->set_name(m_strName);
	pItem->set_level(m_dwLevel);
	pItem->set_summonid(m_uSummonerID);
	pItem->set_controlerid(m_uControlerID);
	pItem->set_hostguid(m_uHostGuid);
	pItem->set_x(m_x);
	pItem->set_x(m_y);
	pItem->set_z(m_z);
	pItem->set_ft(m_ft);
	pItem->set_hp(m_Propertys[HP]);
	pItem->set_mp(m_Propertys[MP]);
	pItem->set_hpmax(m_Propertys[HP_MAX]);
	pItem->set_mpmax(m_Propertys[MP_MAX]);
	m_bDataChange = FALSE;
	return TRUE;
}

BOOL CSceneObject::SaveUpdateData(ObjectActionNty& Nty)
{
	ActionNtyItem* pItem = Nty.add_actionlist();
	pItem->set_objectguid(m_uGuid);
	pItem->set_actorid(m_dwActorID);
	pItem->set_actionid(m_dwActionID);
	pItem->set_buffstate(m_dwBuffState);
	pItem->set_objectstate(m_dwObjectState);
	pItem->set_level(m_dwLevel);
	pItem->set_controlerid(m_uControlerID);
	pItem->set_hostx(m_x);
	pItem->set_hosty(m_y);
	pItem->set_hostz(m_z);
	pItem->set_hostft(m_ft);
	pItem->set_hp(m_Propertys[HP]);
	pItem->set_mp(m_Propertys[MP]);
	pItem->set_hpmax(m_Propertys[HP_MAX]);
	pItem->set_mpmax(m_Propertys[MP_MAX]);
	m_bDataChange = FALSE;
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

UINT64 CSceneObject::GetLastSkillTick(UINT32 dwSkillID)
{
	auto itor = m_mapSkillTime.find(dwSkillID);
	if(itor != m_mapSkillTime.end())
	{
		return itor->second;
	}

	return 0;
}

BOOL CSceneObject::SetLastSkillTick(UINT32 dwSkillID, UINT64 dwTime)
{
	m_mapSkillTime.insert(std::make_pair(dwSkillID, dwTime));

	return TRUE;
}

BOOL CSceneObject::StartSkill(UINT32 dwSkillID)
{
	m_SkillObject.StartSkill(dwSkillID);

	return TRUE;

}

std::vector<CSceneObject*>& CSceneObject::GetAffectTargets()
{
	m_vtTargets.clear();




	return m_vtTargets;
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

BOOL CSceneObject::UpdateBuff(UINT64 uTick)
{
	for(std::map<UINT32, CBuffObject*>::iterator itor = m_mapBuff.begin(); itor != m_mapBuff.end();)
	{
		CBuffObject* pBuffObject = itor->second;
		ERROR_CONTINUE_EX(pBuffObject != NULL);

		pBuffObject->OnUpdate(uTick);

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

UINT32 CSceneObject::ProcessSkill(const SkillCastReq& Req)
{
	ERROR_RETURN_CODE(m_pScene != NULL, MRC_SUCCESSED);

	//1. 技能是否存在
	StSkillInfo* pSkillInfo = CConfigData::GetInstancePtr()->GetSkillInfo(Req.skillid(), 0);
	ERROR_RETURN_CODE(pSkillInfo != NULL, MRC_INVALID_SKILL_ID);

	//2. 技能CD是否可以施放。
	UINT64 uCurTick = CommonFunc::GetTickCount();
	UINT64 uLastTick = GetLastSkillTick(Req.skillid());
	if (uCurTick - uLastTick < pSkillInfo->CD * 1000)
	{
		return MRC_SKILL_CD_ERROR;
	}

	//3. 技能是否可以打中指定的目标.(带有目标的技能要检查目标是否合法)
	for (int i = 0; i < Req.targetobjects_size(); i++)
	{

	}

	m_pScene->BroadMessage(MSG_SKILL_CAST_NTF, Req);

	UINT32 dwRetCode = m_SkillObject.StartSkill(Req.skillid());
	if (MRC_SUCCESSED != dwRetCode)
	{
		return dwRetCode;
	}

	SetLastSkillTick(Req.skillid(), uCurTick);

	m_x		= Req.hostx();
	m_y		= Req.hosty();
	m_z		= Req.hostz();
	m_ft	= Req.hostft();



	return MRC_SUCCESSED;
}

UINT32 CSceneObject::ProcessAction(const ActionReqItem& Item)
{
	ERROR_RETURN_CODE(m_pScene != NULL, MRC_SUCCESSED);

	m_x				= Item.hostx();
	m_y				= Item.hosty();
	m_z				= Item.hostz();
	m_ft			= Item.hostft();
	m_dwActionID	= Item.actionid();
	m_bDataChange	= TRUE;

	return MRC_SUCCESSED;
}
