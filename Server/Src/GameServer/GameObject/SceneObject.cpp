#include "stdafx.h"
#include "SceneObject.h"
#include "../GameService.h"
#include "../Message/Msg_Copy.pb.h"
#include "BuffObject.h"
#include "../Message/Msg_Move.pb.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../ConfigData/ConfigData.h"

CSceneObject::CSceneObject(UINT64 uGuid, CScene* pScene)
{
	m_uGuid = uGuid;
	m_pScene = pScene;

	m_dwProxyConnID		= 0;
	m_dwClientConnID	= 0;
	m_dwObjectStatus	= 0;
	m_bEnter			= FALSE;
	m_dwActorID			= 0;
	m_dwObjType			= 0;
	m_dwCamp			= 0;
	m_bDataChange		= FALSE;
	m_uLastMoveTick		= 0;
	memset(m_Equips, 0, sizeof(m_Equips));
	memset(m_Propertys, 0, sizeof(m_Propertys));
	m_SkillObject.SetCastObject(this);
}

CSceneObject::~CSceneObject()
{
	m_pScene = NULL;
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
	m_bDataChange = TRUE;
}

VOID CSceneObject::SubHp( UINT32 dwValue )
{
	m_Propertys[HP] -= dwValue;

	if(m_Propertys[HP] < 0)
	{
		m_Propertys[HP] = 0;
	}

	m_bDataChange = TRUE;
}

VOID CSceneObject::AddMp( UINT32 dwValue )
{
	m_Propertys[MP] += dwValue;
	m_bDataChange = TRUE;
}

VOID CSceneObject::SubMp( UINT32 dwValue )
{
	m_Propertys[MP] -= dwValue;

	if(m_Propertys[MP] < 0)
	{
		m_Propertys[MP] = 0;
	}

	m_bDataChange = TRUE;
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
	pItem->set_buffstate(m_dwBuffStatus);
	pItem->set_objectstate(m_dwObjectStatus);
	pItem->set_name(m_strName);
	pItem->set_level(m_dwLevel);
	pItem->set_summonid(m_uSummonerID);
	pItem->set_controlerid(m_uControlerID);
	pItem->set_hostguid(m_uHostGuid);
	pItem->set_x(m_Pos.m_x);
	pItem->set_y(m_Pos.m_y);
	pItem->set_z(m_Pos.m_z);
	pItem->set_ft(m_ft);
	pItem->set_hp(m_Propertys[HP]);
	pItem->set_mp(m_Propertys[MP]);
	pItem->set_hpmax(m_Propertys[HP_MAX]);
	pItem->set_mpmax(m_Propertys[MP_MAX]);
	pItem->set_camp(m_dwCamp);
	for (int i = 0; i < EQUIP_MAX_NUM; i++)
	{
		pItem->add_equips(m_Equips[i]);
	}

	m_bDataChange = FALSE;
	return TRUE;
}

BOOL CSceneObject::SaveUpdateData(ObjectActionNty& Nty)
{
	ActionNtyItem* pItem = Nty.add_actionlist();
	pItem->set_objectguid(m_uGuid);
	pItem->set_actorid(m_dwActorID);
	pItem->set_actionid(m_dwActionID);
	pItem->set_buffstate(m_dwBuffStatus);
	pItem->set_objectstate(m_dwObjectStatus);
	pItem->set_level(m_dwLevel);
	pItem->set_controlerid(m_uControlerID);
	pItem->set_hostx(m_Pos.m_x);
	pItem->set_hosty(m_Pos.m_y);
	pItem->set_hostz(m_Pos.m_z);
	pItem->set_hostft(m_ft);
	pItem->set_hp(m_Propertys[HP]);
	pItem->set_mp(m_Propertys[MP]);
	pItem->set_hpmax(m_Propertys[HP_MAX]);
	pItem->set_mpmax(m_Propertys[MP_MAX]);
	pItem->set_camp(m_dwCamp);
	for (int i = 0; i < EQUIP_MAX_NUM; i++)
	{
		pItem->add_equips(m_Equips[i]);
	}
	m_bDataChange = FALSE;
	return TRUE;
}

BOOL CSceneObject::IsDead()
{
	return (m_dwObjectStatus & EOS_DEAD) > 0;
}

BOOL CSceneObject::SetDead(BOOL bDead)
{
	if (bDead)
	{
		m_dwObjectStatus |= EOS_DEAD;
	}
	else
	{
		m_dwObjectStatus &= ~EOS_DEAD;
	}

	return TRUE;
}

BOOL CSceneObject::SetPos(FLOAT x, FLOAT y,  FLOAT z, FLOAT ft)
{
	m_Pos.m_x = x;
	m_Pos.m_y = y;
	m_Pos.m_z = z;
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


BOOL CSceneObject::GetAffectTargets(std::vector<CSceneObject*>& vtTargets)
{
	vtTargets.clear();



	return TRUE;
}

BOOL CSceneObject::IsInCircle(float radius, float height, Vector3D hitPoint)
{
	StActorInfo* pActorInfo = CConfigData::GetInstancePtr()->GetActorInfo(m_dwActorID);
	ERROR_RETURN_FALSE(pActorInfo != NULL);

	FLOAT fdistance = m_Pos.Distance2D(hitPoint);
	if (fdistance > radius  + pActorInfo->fRadius)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSceneObject::IsInBox(float length, float width, Vector3D hitPoint, FLOAT ft)
{
	float radius = 1.0f;

	CPoint2D A(-width / 2, -length / 2), B(-width / 2, length / 2), C(width / 2, length / 2), D(width / 2, -length / 2);

	A.Rotate(ft * DEG_TO_RAD);
	B.Rotate(ft * DEG_TO_RAD);
	C.Rotate(ft * DEG_TO_RAD);
	D.Rotate(ft * DEG_TO_RAD);

	CPoint2D rolept(m_Pos.m_x, m_Pos.m_z);

	if (rolept.DistanceToSegment(A, B) < radius)
	{
		return TRUE;
	}

	if (rolept.DistanceToSegment(B, C) < radius)
	{
		return TRUE;
	}

	if (rolept.DistanceToSegment(C, D) < radius)
	{
		return TRUE;
	}

	if (rolept.DistanceToSegment(D, A) < radius)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CSceneObject::IsInSphere(float radius, Vector3D hitPoint)
{
	Vector3D dir = m_Pos - hitPoint;
	if (dir.SquaredLength() > radius * radius)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSceneObject::IsInSector(float radius, float hAngle, Vector3D hitPoint, Vector3D hitDir)
{
	float maxDis = radius;// +target.Radius;
	if (m_Pos.Distance2D(hitPoint) > maxDis)
	{
		return FALSE;
	}

	Vector3D vtDir = m_Pos - hitPoint;

	if (vtDir.AngleBetween2D(hitDir) > hAngle / 2)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CSceneObject::SaveBattleResult(ResultPlayer* pResult)
{
	pResult->set_objectid(m_uGuid);
	pResult->set_actorid(m_dwActorID);
// 	pResult->set_result(m_dwResult);
// 	pResult->set_damage(m_dwDamage);

	return TRUE;
}

BOOL CSceneObject::ChangeEquip(INT32 nPos, UINT32 dwEquipID)
{
	ERROR_RETURN_FALSE(nPos > 0);
	m_Equips[nPos - 1] = dwEquipID;
	m_bDataChange = TRUE;
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


BOOL CSceneObject::StartSkill(UINT32 dwSkillID)
{
	SkillCastReq Req;
	Req.set_skillid(dwSkillID);
	Req.set_objectguid(m_uGuid);
	Req.set_hostx(m_Pos.m_x);
	Req.set_hosty(m_Pos.m_y);
	Req.set_hostz(m_Pos.m_z);

	m_pScene->BroadMessage(MSG_SKILL_CAST_NTF, Req);

	m_SkillObject.StartSkill(dwSkillID);

	return TRUE;

}

UINT32 CSceneObject::ProcessSkill(const SkillCastReq& Req)
{
	m_pScene->BroadMessage(MSG_SKILL_CAST_NTF, Req);

	return MRC_SUCCESSED;

	ERROR_RETURN_CODE(m_pScene != NULL, MRC_UNKNOW_ERROR);

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

	//3. 扣除放技能需要的东西
	if (GetMp() < pSkillInfo->CostMp)
	{
		//	return MRC_SKILL_CD_ERROR;
	}

	m_SkillObject.StopSkill(); //停止当前的技能

	//4. 技能是否可以打中指定的目标.(带有目标的技能要检查目标是否合法)
	for (int i = 0; i < Req.targetobjects_size(); i++)
	{
		CSceneObject* pTempObject = m_pScene->GetSceneObject(Req.targetobjects(i));
		ERROR_RETURN_CODE(pTempObject != NULL, MRC_INVALID_TARGET_ID);
		m_SkillObject.AddTargetObject(pTempObject);
	}

	m_pScene->BroadMessage(MSG_SKILL_CAST_NTF, Req);

	UINT32 dwRetCode = m_SkillObject.StartSkill(Req.skillid());
	if (MRC_SUCCESSED != dwRetCode)
	{
		return dwRetCode;
	}

	SetLastSkillTick(Req.skillid(), uCurTick);

	m_Pos.m_x		= Req.hostx();
	m_Pos.m_y		= Req.hosty();
	m_Pos.m_z		= Req.hostz();
	m_ft			= Req.hostft();



	return MRC_SUCCESSED;
}

UINT32 CSceneObject::ProcessAction(const ActionReqItem& Item)
{
	ERROR_RETURN_CODE(m_pScene != NULL, MRC_SUCCESSED);

	m_Pos.m_x		= Item.hostx();
	m_Pos.m_y		= Item.hosty();
	m_Pos.m_z		= Item.hostz();
	m_ft			= Item.hostft();
	m_dwActionID	= Item.actionid();
	m_bDataChange	= TRUE;

	return MRC_SUCCESSED;
}
