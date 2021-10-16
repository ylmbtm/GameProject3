#include "stdafx.h"
#include "SceneObject.h"
#include "../GameService.h"
#include "../Message/Msg_Copy.pb.h"
#include "BuffObject.h"
#include "../Message/Msg_Move.pb.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "StaticData.h"
#include "../Message/Game_Define.pb.h"

CSceneObject::CSceneObject(UINT64 uGuid, CScene* pScene)
{
	m_uGuid = uGuid;

	m_pScene = pScene;

	Reset();
}

CSceneObject::~CSceneObject()
{
	Reset();

	ClearBuff();

	m_vtNormals.clear();

	m_vtSpecials.clear();
}


BOOL CSceneObject::SetConnectID(UINT32 dwProxyID, UINT32 dwClientID)
{
	m_dwProxyConnID = dwProxyID;
	m_dwClientConnID = dwClientID;
	return TRUE;
}

BOOL CSceneObject::SendMsgProtoBuf(INT32 nMsgID, const google::protobuf::Message& pdata)
{
	if (m_dwProxyConnID == 0)
	{
		CLog::GetInstancePtr()->LogError("Error SendMsgProtoBuf MessageID:%d", nMsgID);
		return FALSE;
	}

	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwProxyConnID, nMsgID, GetObjectGUID(), m_dwClientConnID, pdata);
}

BOOL CSceneObject::SendMsgRawData(INT32 nMsgID, const char* pdata, UINT32 dwLen)
{
	if (m_dwProxyConnID == 0)
	{
		CLog::GetInstancePtr()->LogError("Error SendMsgRawData MessageID:%d", nMsgID);
		return FALSE;
	}

	return ServiceBase::GetInstancePtr()->SendMsgRawData(m_dwProxyConnID, nMsgID, GetObjectGUID(), m_dwClientConnID, pdata, dwLen);
}

BOOL CSceneObject::OnUpdate( UINT64 uTick )
{
	UpdatePosition(uTick);

	UpdateBuff(uTick);

	m_SkillObject.OnUpdate(uTick);

	return TRUE;
}

UINT32 CSceneObject::GetHp()
{
	return m_Propertys[EA_HP];
}

UINT32 CSceneObject::GetMp()
{
	return m_Propertys[EA_MP];
}

VOID CSceneObject::ChangeHp( INT32 nValue )
{
	m_Propertys[EA_HP] += nValue;
	if (m_Propertys[EA_HP] < 0)
	{
		m_Propertys[EA_HP] = 0;
	}
	m_ChangeFlag.bBase = 1;
}


VOID CSceneObject::ChangeMp( INT32 nValue )
{
	m_Propertys[EA_MP] += nValue;
	if (m_Propertys[EA_MP] < 0)
	{
		m_Propertys[EA_MP] = 0;
	}
	m_ChangeFlag.bBase = 1;
}



BOOL CSceneObject::IsChanged()
{
	return m_ChangeFlag.dwValue != 0;
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

UINT64 CSceneObject::GetControllerID()
{
	return m_uControlerID;
}

VOID CSceneObject::SetControllerID(UINT64 uID)
{
	m_uControlerID = uID;
	m_ChangeFlag.bControl = 1;
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

VOID CSceneObject::SetActionID(UINT32 dwActionID)
{
	if (dwActionID != m_dwActionID)
	{
		m_dwActionID = dwActionID;
		m_ChangeFlag.bAction = 1;
	}

	return;
}

UINT32 CSceneObject::GetActionID()
{
	return m_dwActionID;
}

BOOL CSceneObject::SaveNewData( ObjectNewNty& Nty )
{
	NewItem* pItem = Nty.add_newlist();
	pItem->set_objectguid(m_uGuid);
	pItem->set_actionid(m_dwActionID);
	pItem->set_objtype(m_dwObjType);
	pItem->set_actorid(m_dwActorID);
	pItem->set_objectstatus(m_dwStatus);
	pItem->set_name(m_strName);
	pItem->set_level(m_dwLevel);
	pItem->set_summonid(m_uSummonerID);
	pItem->set_controlerid(m_uControlerID);
	pItem->set_hostguid(m_uHostGuid);
	pItem->set_x(m_Pos.m_x);
	pItem->set_y(m_Pos.m_y);
	pItem->set_z(m_Pos.m_z);
	pItem->set_ft(m_ft);
	pItem->set_hp(m_Propertys[EA_HP]);
	pItem->set_mp(m_Propertys[EA_MP]);
	pItem->set_hpmax(m_Propertys[EA_HP_MAX]);
	pItem->set_mpmax(m_Propertys[EA_MP_MAX]);
	pItem->set_speed(m_Propertys[EA_SPEED]);
	pItem->set_camp(m_dwCamp);
	if (m_bRiding)
	{
		pItem->set_mountid(m_dwMountID);
	}
	else
	{
		pItem->set_mountid(0 - m_dwMountID);
	}

	for (int i = 0; i < EQUIP_MAX_NUM; i++)
	{
		pItem->add_equips(m_Equips[i]);
	}

	for (int i = 0; i < m_vtNormals.size(); i++)
	{
		SkillItem* pSkillitem = pItem->add_skills();
		pSkillitem->set_keypos(m_vtNormals.at(i).nKeyPos);
		pSkillitem->set_level(m_vtNormals.at(i).nLevel);
		pSkillitem->set_skillid(m_vtNormals.at(i).dwSkillID);
	}

	for (int i = 0; i < m_vtSpecials.size(); i++)
	{
		SkillItem* pSkillitem = pItem->add_skills();
		pSkillitem->set_keypos(m_vtSpecials.at(i).nKeyPos);
		pSkillitem->set_level(m_vtSpecials.at(i).nLevel);
		pSkillitem->set_skillid(m_vtSpecials.at(i).dwSkillID);
	}

	m_ChangeFlag.dwValue = 0;

	return TRUE;
}

BOOL CSceneObject::SaveUpdateData(ObjectActionNty& Nty)
{
	ActionNtyItem* pItem = Nty.add_actionlist();
	pItem->set_objectguid(m_uGuid);
	pItem->set_hp(m_Propertys[EA_HP]);
	pItem->set_mp(m_Propertys[EA_MP]);
	if (m_ChangeFlag.bBase)
	{
		pItem->set_hpmax(m_Propertys[EA_HP_MAX]);
		pItem->set_mpmax(m_Propertys[EA_MP_MAX]);
		pItem->set_speed(m_Propertys[EA_SPEED]);
		pItem->set_hostx(m_Pos.m_x);
		pItem->set_hosty(m_Pos.m_y);
		pItem->set_hostz(m_Pos.m_z);
		pItem->set_hostft(m_ft);
		pItem->set_objectstatus(m_dwStatus);
	}

	if (m_ChangeFlag.bEquip)
	{
		for (int i = 0; i < EQUIP_MAX_NUM; i++)
		{
			pItem->add_equips(m_Equips[i]);
		}
	}

	if (m_ChangeFlag.bActor)
	{
		pItem->set_actorid(m_dwActorID);
	}

	if (m_ChangeFlag.bBuff)
	{
	}

	if (m_ChangeFlag.bAction)
	{
		pItem->set_actionid(m_dwActionID);
	}

	if (m_ChangeFlag.bLevel)
	{
		pItem->set_level(m_dwLevel);
	}

	if (m_ChangeFlag.bControl)
	{
		pItem->set_controlerid(m_uControlerID);
	}

	if (m_ChangeFlag.bCamp)
	{
		pItem->set_camp(m_dwCamp);
	}

	if (m_ChangeFlag.bMount)
	{
		if (m_bRiding)
		{
			pItem->set_mountid(m_dwMountID);
		}
		else
		{
			pItem->set_mountid(0 - m_dwMountID);
		}

	}

	m_ChangeFlag.dwValue = 0;

	return TRUE;
}

BOOL CSceneObject::Reset()
{
	m_dwProxyConnID     = 0;
	m_dwClientConnID    = 0;
	m_dwStatus          = 0;
	m_bEnter            = FALSE;
	m_dwActorID         = 0;
	m_dwObjType         = OT_NONE;
	m_dwCamp            = 0;
	m_ChangeFlag.dwValue = 0;
	m_uLastMoveTick     = 0;
	m_dwActionID        = AT_IDLE;
	m_pActorInfo        = NULL;
	m_bRiding           = FALSE;
	m_bRobot            = FALSE;
	m_dwMountID         = 0;
	m_nBattleResult     = ECR_NONE;
	m_bIsMonsCheck      = FALSE;
	m_bIsCampCheck      = TRUE;
	m_uHostGuid         = 0;
	m_uControlerID      = 0;
	m_uSummonerID       = 0;
	m_uPetGuid          = 0;
	m_uPartnerGuid      = 0;

	memset(m_Equips, 0, sizeof(m_Equips));

	memset(m_Propertys, 0, sizeof(m_Propertys));

	m_SkillObject.SetCastObject(this);

	return TRUE;
}

BOOL CSceneObject::IsDead()
{
	return (m_dwStatus & EOS_DEAD) > 0;
}

BOOL CSceneObject::SetDead(BOOL bDead)
{
	if (bDead)
	{
		m_dwStatus |= EOS_DEAD;
	}
	else
	{
		m_dwStatus &= ~EOS_DEAD;
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

Vector3D& CSceneObject::GetPos()
{
	return m_Pos;
}

FLOAT CSceneObject::GetFaceTo()
{
	return m_ft;
}

UINT64 CSceneObject::GetLastSkillTick(UINT32 dwSkillID)
{
	for (int i = 0; i < m_vtSpecials.size(); i++)
	{
		if (m_vtSpecials.at(i).dwSkillID == dwSkillID)
		{
			return m_vtSpecials.at(i).uLastTime;
		}
	}

	for (int i = 0; i < m_vtNormals.size(); i++)
	{
		if (m_vtNormals.at(i).dwSkillID == dwSkillID)
		{
			return m_vtNormals.at(i).uLastTime;
		}
	}

	return 0;
}

BOOL CSceneObject::SetLastSkillTick(UINT32 dwSkillID, UINT64 uTime)
{
	for (int i = 0; i < m_vtNormals.size(); i++)
	{
		if (m_vtNormals.at(i).dwSkillID == dwSkillID)
		{
			St_SkillData& tData = m_vtNormals.at(i);
			tData.uLastTime = uTime;
			return TRUE;
		}
	}

	for (int i = 0; i < m_vtSpecials.size(); i++)
	{
		if (m_vtSpecials.at(i).dwSkillID == dwSkillID)
		{
			St_SkillData& tData = m_vtSpecials.at(i);
			tData.uLastTime = uTime;
			return TRUE;
		}
	}

	return FALSE;
}

St_SkillData* CSceneObject::GetSkillData(UINT32 dwSkillID)
{
	for (int i = 0; i < m_vtNormals.size(); i++)
	{
		if (m_vtNormals.at(i).dwSkillID == dwSkillID)
		{
			return &m_vtNormals[i];
		}
	}

	for (int i = 0; i < m_vtSpecials.size(); i++)
	{
		if (m_vtSpecials.at(i).dwSkillID == dwSkillID)
		{
			return &m_vtSpecials[i];
		}
	}

	return NULL;
}

BOOL CSceneObject::IsInCircle(Vector3D hitPoint, float radius, float height)
{
	StActorInfo* pActorInfo = CStaticData::GetInstancePtr()->GetActorInfo(m_dwActorID);
	ERROR_RETURN_FALSE(pActorInfo != NULL);

	FLOAT fdistance = m_Pos.Distance2D(hitPoint);
	if (fdistance > radius  + pActorInfo->fRadius)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSceneObject::IsInSquare(Vector3D hitPoint, FLOAT hitDegree, float length, float width)
{
	float radius = 1.0f; //玩家自身的半径

	CPoint2D A(-width / 2, -length / 2), B(-width / 2, length / 2), C(width / 2, length / 2), D(width / 2, -length / 2);

	A.Rotate(hitDegree * DEG_TO_RAD);
	B.Rotate(hitDegree * DEG_TO_RAD);
	C.Rotate(hitDegree * DEG_TO_RAD);
	D.Rotate(hitDegree * DEG_TO_RAD);

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

BOOL CSceneObject::IsInSector(Vector3D hitPoint, float hitDegree, float radius, float hAngle)
{
	float maxDis = radius;// +target.Radius;
	if (m_Pos.Distance2D(hitPoint) > maxDis)
	{
		return FALSE;
	}

	Vector3D vtDir = m_Pos - hitPoint;
	float fAttack = vtDir.ToDegreesAngle();
	float fDiff = fAttack - hitDegree;
	if (fDiff > hAngle / 2)
	{
		return FALSE;
	}

	return TRUE;
}

VOID CSceneObject::SetBattleResult(ECopyResult nBattleResult)
{
	m_nBattleResult = nBattleResult;
}

ECopyResult CSceneObject::GetBattleResult()
{
	return m_nBattleResult;
}

BOOL CSceneObject::UpdatePosition(UINT64 uTick)
{
	if(m_dwActionID == AT_RUN)
	{

	}

	if (m_dwActionID == AT_WALK)
	{

	}


	return TRUE;
}

BOOL CSceneObject::Revive()
{
	SetDead(FALSE);

	SetActionID(AT_IDLE);

	m_Propertys[EA_HP] = m_Propertys[EA_HP_MAX];

	m_Propertys[EA_MP] = m_Propertys[EA_MP_MAX];

	return TRUE;
}

INT32 CSceneObject::GetShip(CSceneObject* pTarget)
{
	if (pTarget->GetCamp() == 0)
	{
		return ES_NEUTRAL;
	}

	if (pTarget->GetCamp() != GetCamp())
	{
		return ES_ENEMY;
	}

	return ES_FRIEND;
}



BOOL CSceneObject::SaveBattleRecord(ResultPlayer* pResult)
{
	pResult->set_objectid(m_uGuid);
	pResult->set_actorid(m_dwActorID);
	pResult->set_result(m_nBattleResult);
	return TRUE;
}

BOOL CSceneObject::ChangeEquip(INT32 nPos, UINT32 dwEquipID)
{
	ERROR_RETURN_FALSE(nPos > 0);
	m_Equips[nPos - 1] = dwEquipID;
	m_ChangeFlag.bEquip = 1;
	return TRUE;
}

BOOL CSceneObject::ChangeMount(UINT32 dwMountID)
{
	m_dwMountID = dwMountID;
	m_ChangeFlag.bMount = 1;
	return TRUE;
}

BOOL CSceneObject::SetRiding(BOOL bRiding)
{
	if (m_dwMountID <= 0)
	{
		m_bRiding = FALSE;
	}
	else
	{
		m_bRiding = bRiding;
	}

	m_ChangeFlag.bMount = 1;

	return TRUE;
}

FLOAT CSceneObject::GetCurSpeed()
{
	if (m_pActorInfo == NULL)
	{
		m_pActorInfo = CStaticData::GetInstancePtr()->GetActorInfo(m_dwActorID);
	}

	return m_pActorInfo == NULL ? 0.0f : m_pActorInfo->fDefSpeed * m_Propertys[EA_SPEED] / 10000.0f;
}

BOOL CSceneObject::IsRobot()
{
	return m_bRobot;
}

BOOL CSceneObject::AddBuff(UINT32 dwBuffID)
{
	CBuffObject* pBuffObject = new CBuffObject(this, dwBuffID);

	ERROR_RETURN_FALSE(pBuffObject != NULL);

	pBuffObject->OnAddBuff();

	m_mapBuff.insert(std::make_pair(dwBuffID, pBuffObject));

	return TRUE;
}

BOOL CSceneObject::RemoveBuff(UINT32 dwBuffID)
{
	std::map<UINT32, CBuffObject*>::iterator itor = m_mapBuff.find(dwBuffID);
	ERROR_RETURN_FALSE(itor != m_mapBuff.end());

	CBuffObject* pBuffObject = itor->second;
	ERROR_RETURN_FALSE(pBuffObject != NULL);

	pBuffObject->OnRemoveBuff();

	pBuffObject->SetOver();

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


BOOL CSceneObject::ClearBuff()
{
	for (std::map<UINT32, CBuffObject*>::iterator itor = m_mapBuff.begin(); itor != m_mapBuff.end();)
	{
		CBuffObject* pBuffObject = itor->second;
		ERROR_CONTINUE_EX(pBuffObject != NULL);
		delete pBuffObject;
	}

	m_mapBuff.clear();

	return TRUE;
}

INT32 CSceneObject::GetSkillLevel(UINT32 dwSkillID)
{
	St_SkillData* pData  = GetSkillData(dwSkillID);

	ERROR_RETURN_VALUE(pData != NULL, 0);

	return pData->nLevel;
}

BOOL CSceneObject::InitSkills(const google::protobuf::RepeatedPtrField< ::SkillItem >& vtSkills)
{
	m_vtNormals.clear();
	m_vtSpecials.clear();
	for (int i = 0; i < vtSkills.size(); i++)
	{
		const SkillItem& tItem = vtSkills.Get(i);

		St_SkillData tData;
		tData.nKeyPos = tItem.keypos();
		tData.nLevel = tItem.level();
		tData.uLastTime = 0;
		tData.dwSkillID = tItem.skillid();

		if (tData.nKeyPos == 1)
		{
			m_vtNormals.emplace_back(tData);
		}
		else
		{
			m_vtSpecials.emplace_back(tData);
		}
	}

	if (m_vtNormals.size() <= 0 || m_vtNormals[0].dwSkillID <= 0)
	{
		return FALSE;
	}

	StComboSkillInfo* pComboInfo = CStaticData::GetInstancePtr()->GetComboSkillInfo(m_vtNormals[0].dwSkillID);
	if (pComboInfo == NULL)
	{
		//表示没有连招
		return TRUE;
	}

	for (int i = 0; i < pComboInfo->vtComboSkill.size(); i++)
	{
		St_SkillData tData;
		tData.nKeyPos = m_vtNormals[0].nKeyPos;
		tData.nLevel = m_vtNormals[0].nLevel;
		tData.uLastTime = 0;
		tData.dwSkillID = pComboInfo->vtComboSkill.at(i);
		m_vtNormals.emplace_back(tData);
	}

	return TRUE;
}

BOOL CSceneObject::InitSkills()
{
	UINT32 dwActorID = GetActorID();
	if (dwActorID == 0)
	{
		return FALSE;
	}

	StActorSkillInfo* pActorSkillInfo = CStaticData::GetInstancePtr()->GetActorSkillInfo(dwActorID);
	ERROR_RETURN_FALSE(pActorSkillInfo != NULL);

	m_vtNormals.clear();
	m_vtSpecials.clear();

	St_SkillData tData;
	tData.nKeyPos = 1;
	tData.nLevel = 1;
	tData.uLastTime = 0;
	tData.dwSkillID = pActorSkillInfo->NormalID;
	m_vtNormals.emplace_back(tData);

	for (int i = 0; i < 5; i++)
	{
		if (pActorSkillInfo->Specials[i] <= 0)
		{
			break;
		}

		St_SkillData tData;
		tData.nKeyPos = i + 2;
		tData.nLevel = 1;
		tData.uLastTime = 0;
		tData.dwSkillID = pActorSkillInfo->Specials[i];
		m_vtSpecials.emplace_back(tData);
	}

	if (m_vtNormals.size() <= 0 || m_vtNormals[0].dwSkillID <= 0)
	{
		return FALSE;
	}

	StComboSkillInfo* pComboInfo = CStaticData::GetInstancePtr()->GetComboSkillInfo(m_vtNormals[0].dwSkillID);
	if (pComboInfo == NULL)
	{
		//表示没有连招
		return TRUE;
	}

	for (int i = 0; i < pComboInfo->vtComboSkill.size(); i++)
	{
		St_SkillData tData;
		tData.nKeyPos = m_vtNormals[0].nKeyPos;
		tData.nLevel = m_vtNormals[0].nLevel;
		tData.uLastTime = 0;
		tData.dwSkillID = pComboInfo->vtComboSkill.at(i);
		m_vtNormals.emplace_back(tData);
	}

	return TRUE;
}

UINT32 CSceneObject::GetNextComboSkill(UINT32 dwSkillID)
{
	for (int i = 0; i < m_vtNormals.size(); i++)
	{
		if (m_vtNormals[i].dwSkillID == dwSkillID)
		{
			if (i >= m_vtNormals.size() - 1)
			{
				return m_vtNormals[0].dwSkillID;
			}
			else
			{
				return m_vtNormals[i + 1].dwSkillID;
			}
		}
	}

	return  0;
}

BOOL CSceneObject::CheckSkillCD(UINT32 dwSkillID, UINT64 uCD)
{
	UINT64 uCurTick = CommonFunc::GetTickCount();
	UINT64 uLastTick = GetLastSkillTick(dwSkillID);
	if (uCurTick - uLastTick < uCD)
	{
		return FALSE;
	}

	SetLastSkillTick(dwSkillID, uCurTick);
	return TRUE;
}

UINT32 CSceneObject::ProcessSkill(const SkillCastReq& Req)
{
	ERROR_RETURN_VALUE(m_pScene != NULL, MRC_UNKNOW_ERROR);

	//取技能等级
	INT32 nLevel = GetSkillLevel(Req.skillid());
	ERROR_RETURN_VALUE(nLevel > 0, MRC_INVALID_SKILL_ID);

	StSkillInfo* pSkillInfo = CStaticData::GetInstancePtr()->GetSkillInfo(Req.skillid(), nLevel);
	ERROR_RETURN_VALUE(pSkillInfo != NULL, MRC_INVALID_SKILL_ID);

	//3. 扣除放技能需要的东西
	//if (GetMp() < pSkillInfo->CostMp)
	//{
	//	return MRC_SKILL_CD_ERROR;
	//}

	//是否是普通的连击技能
	if (m_vtNormals.size() > 0 && Req.skillid() == m_vtNormals[0].dwSkillID)
	{
		//如果是普通的连击技能
		if(m_SkillObject.GetSkillStatus() == ESS_RUNNING)
		{
			m_SkillObject.SetComboSkill(TRUE);
			return MRC_SUCCESSED;
		}
	}

	m_SkillObject.SetComboSkill(FALSE);

	//技能CD是否可以施放。
	if (!CheckSkillCD(Req.skillid(), pSkillInfo->CD))
	{
		return MRC_SKILL_CD_ERROR;
	}

	m_SkillObject.StopSkill(); //停止当前的技能

	//技能是否可以打中指定的目标.(带有目标的技能要检查目标是否合法)
	if (Req.targetobjects_size() > 0)
	{
		for (int i = 0; i < Req.targetobjects_size(); i++)
		{
			CSceneObject* pTempObject = m_pScene->GetSceneObject(Req.targetobjects(i));
			if (pTempObject == NULL || pTempObject->IsDead())
			{
				continue;
			}
			m_SkillObject.AddTargetObject(pTempObject);
		}

		if (m_SkillObject.GetTargetNum() <= 0)
		{
			return MRC_INVALID_TARGET_ID;
		}
	}

	m_pScene->BroadMessage(MSG_SKILL_CAST_NTF, Req);

	if (!m_SkillObject.StartSkill(Req.skillid(), nLevel))
	{
		return MRC_UNKNOW_ERROR;
	}

	m_Pos.m_x       = Req.hostx();
	m_Pos.m_y       = Req.hosty();
	m_Pos.m_z       = Req.hostz();
	m_ft            = Req.hostft();
	m_dwActionID    = AT_SKILL;

	return MRC_SUCCESSED;
}

UINT32 CSceneObject::ProcessAction(const ActionReqItem& Item)
{
	ERROR_RETURN_VALUE(m_dwActionID != AT_DEAD, MRC_SKILL_DEAD_OBJ);
	ERROR_RETURN_VALUE(m_pScene != NULL, MRC_SUCCESSED);

	m_Pos.m_x		= Item.hostx();
	m_Pos.m_y		= Item.hosty();
	m_Pos.m_z		= Item.hostz();
	m_ft			= Item.hostft();

	m_ChangeFlag.bBase = 1;

	if (Item.actionid() != m_dwActionID)
	{
		m_dwActionID = Item.actionid();
		m_ChangeFlag.bAction = 1;
	}

	return MRC_SUCCESSED;
}
