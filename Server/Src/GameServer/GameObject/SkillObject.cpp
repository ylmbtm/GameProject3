#include "stdafx.h"
#include "SkillObject.h"
#include "SceneObject.h"
#include "StaticStruct.h"
#include "StaticData.h"
#include "../Message/Game_Define.pb.h"
#include "StaticStruct.h"
#include "../Scene.h"
#include "../Message/Msg_ID.pb.h"

CSkillObject::CSkillObject()
{
    m_pCastObject = NULL;
    ResetSkill();
}

CSkillObject::~CSkillObject()
{
    m_pCastObject   = NULL;
    ResetSkill();
}


BOOL CSkillObject::OnUpdate( UINT64 uTick )
{
    if (m_SkillStatus != ESS_RUNNING)
    {
        return TRUE;
    }

    if (m_dwSkillID == 0 || m_pSkillInfo == NULL || m_pSkillEventInfo == NULL)
    {
        return TRUE;
    }

    UINT64 uElaspedTick = uTick - m_dwStartTick;

    if (m_dwEventIndex < m_pSkillEventInfo->vtEvents.size())
    {
        if (uElaspedTick >= m_pSkillEventInfo->vtEvents[m_dwEventIndex].TrigerTime)
        {
            ProcessSkillEvent(m_pSkillEventInfo->vtEvents[m_dwEventIndex]);

            m_dwEventIndex += 1;
        }
    }

    if (uElaspedTick > m_pSkillEventInfo->uDuration)
    {
        //响应技能结束
        OnSkillComplete();
        return TRUE;
    }

    return TRUE;
}

BOOL CSkillObject::OnSkillComplete()
{
    m_SkillStatus = ESS_FINISHED;
    //如果当前是
    if (m_bComboSkill)
    {
        m_bComboSkill = FALSE;
        UINT32 dwSkillID = m_pCastObject->GetNextComboSkill(m_dwSkillID);
        if (dwSkillID != 0)
        {
            SkillCastReq Req;
            Req.set_objectguid(m_pCastObject->GetObjectGUID());
            Req.set_skillid(dwSkillID);
            Req.set_hostx(m_pCastObject->GetPos().m_x);
            Req.set_hosty(m_pCastObject->GetPos().m_y);
            Req.set_hostz(m_pCastObject->GetPos().m_z);
            Req.set_hostft(m_pCastObject->GetFaceTo());
            m_pCastObject->GetScene()->BroadMessage(MSG_SKILL_CAST_NTF, Req);
            StartSkill(dwSkillID, m_pSkillInfo->Level);
        }

        return TRUE;
    }
    return TRUE;
}

BOOL CSkillObject::StartSkill(UINT32 dwSkillID, INT32 nLevel)
{
    m_dwSkillID = dwSkillID;

    m_dwStartTick = CommonFunc::GetTickCount();

    m_dwEventIndex = 0;

    m_pSkillInfo = CStaticData::GetInstancePtr()->GetSkillInfo(dwSkillID, nLevel);
    ERROR_RETURN_FALSE(m_pSkillInfo != NULL);

    m_pSkillEventInfo = CStaticData::GetInstancePtr()->GetSkillEventInfo(dwSkillID);
    ERROR_RETURN_FALSE(m_pSkillEventInfo != NULL);

    if (ESCT_TYPE_INSTANT == m_pSkillEventInfo->dwCastType)
    {
        m_vtTargets.clear();
    }
    else if(m_vtTargets.size() > 0)
    {
        SetCalcTargets(FALSE);
    }

    m_SkillStatus = ESS_RUNNING;

    //计算攻击目标
    //1.直接带有目标， 2.需要自己计算目标
    //2.只给自己加buff的技能
    //3.只给目标加buff的技能
    //4.加血的技能
    //5.位移技能
    //6.波次技能(闪电链)
    //7.产生子弹的技能


    OnUpdate(m_dwStartTick);

    return TRUE;
}


BOOL CSkillObject::StopSkill()
{
    ResetSkill();

    return TRUE;
}

BOOL CSkillObject::ResetSkill()
{
    m_dwStartTick       = 0;
    m_dwSkillID         = 0;
    m_pSkillInfo        = NULL;
    m_pSkillEventInfo   = NULL;
    m_dwEventIndex      = 0;
    m_SkillStatus       = ESS_INIT;
    m_bCalcTargets      = TRUE;
    m_vtTargets.clear();
    return TRUE;
}

BOOL CSkillObject::SetCastObject(CSceneObject* pObject)
{
    m_pCastObject = pObject;

    return TRUE;
}

CSceneObject* CSkillObject::GetCastObject()
{
    return m_pCastObject;
}

BOOL CSkillObject::AddTargetObject(CSceneObject* pObject)
{
    m_vtTargets.push_back(pObject);

    return TRUE;
}

INT32 CSkillObject::GetTargetNum()
{
    return (INT32)m_vtTargets.size();
}

BOOL CSkillObject::AttackTarget(CSceneObject* pTarget, UINT32 HitActionID, UINT32 HitEffectID, FLOAT HitDistance)
{
    ERROR_RETURN_FALSE(m_pCastObject != NULL);
    ERROR_RETURN_FALSE(m_pSkillInfo != NULL);
    ERROR_RETURN_FALSE(pTarget != NULL);

    CScene* pScene = m_pCastObject->GetScene();
    ERROR_RETURN_FALSE(pScene != NULL);

    INT32 dwRandValue = CommonFunc::GetRandNum(1);
    //先判断是否命中
    if (dwRandValue > (8000 + m_pCastObject->m_Propertys[EA_HIT_RATE] - pTarget->m_Propertys[EA_DODGE]) && dwRandValue > 5000)
    {
        //未命中
        pScene->AddHitEffect(m_pCastObject->GetObjectGUID(), pTarget->GetObjectGUID(), 0, FALSE, HitActionID, HitEffectID, HitDistance);
        return TRUE;
    }

    //判断是否爆击
    dwRandValue = CommonFunc::GetRandNum(1);
    BOOL bCriticalHit = FALSE;
    if (dwRandValue < (m_pCastObject->m_Propertys[EA_CRIT_HIT] - pTarget->m_Propertys[EA_CRIT_DEF]) || dwRandValue < 100)
    {
        bCriticalHit = TRUE;
    }

    //最终伤害加成
    INT32 nFinalAdd = m_pCastObject->m_Propertys[EA_MORE_HURT] - pTarget->m_Propertys[EA_LESS_HURT];
    if (nFinalAdd < 0)
    {
        nFinalAdd = 0;
    }

    //伤害随机
    INT32 nFightRand = 9000 + CommonFunc::GetRandNum(1) % 2000;

    INT32 nDefendValue = (m_pSkillInfo->SkillType == ESTMAGIC) ? pTarget->m_Propertys[EA_MAGIC_DEF] : pTarget->m_Propertys[EA_PHYSIC_DEF];

    INT32 nHurt = m_pCastObject->m_Propertys[EA_ATTACK] - nDefendValue;
    if (nHurt < 0)
    {
        nHurt = 1;
    }

    nHurt = nHurt * nFightRand / 10000;
    nHurt += nHurt * nFinalAdd / 10000;
    if (bCriticalHit)
    {
        nHurt = nHurt * 3 / 2;
    }

    pTarget->ChangeHp(-nHurt);

    pScene->AddHitEffect(m_pCastObject->GetObjectGUID(), pTarget->GetObjectGUID(), -nHurt, bCriticalHit, HitActionID, HitEffectID, HitDistance);

    return TRUE;
}

BOOL CSkillObject::CalcTargetObjects(StSkillEvent& SkillEvent)
{
    if (!m_bCalcTargets)
    {
        return TRUE;
    }

    ERROR_RETURN_FALSE(m_pCastObject != NULL);
    CScene* pScene = m_pCastObject->GetScene();
    ERROR_RETURN_FALSE(pScene != NULL);
    UINT64 uExcludeID = 0;
    if (!m_pSkillInfo->HitMyself)
    {
        uExcludeID = m_pCastObject->GetObjectGUID();
    }

    return pScene->SelectTargets(m_vtTargets, uExcludeID, m_pCastObject->GetCamp(), m_pSkillInfo->HitShipType, m_pCastObject->GetPos(), m_pCastObject->GetFaceTo(), SkillEvent.RangeType, SkillEvent.RangeParams);
}

BOOL CSkillObject::ProcessSkillEvent(StSkillEvent& SkillEvent)
{
    CalcTargetObjects(SkillEvent);

    if (SkillEvent.SelfBuffID != 0)
    {
        m_pCastObject->AddBuff(SkillEvent.SelfBuffID);
    }

    for (auto itor = m_vtTargets.begin(); itor != m_vtTargets.end(); itor++)
    {
        CSceneObject* pTempObject = *itor;

        if (pTempObject == m_pCastObject)
        {
            continue;
        }

        if (SkillEvent.TargetBuffID != 0)
        {
            pTempObject->AddBuff(SkillEvent.TargetBuffID);
        }

        AttackTarget(pTempObject, SkillEvent.HitActionID, SkillEvent.HitEffect, SkillEvent.HitDistance);
    }

    if (SkillEvent.vtBullets.size() > 0)
    {
        BulletNewNtf Ntf;
        for (INT32 nIndex = 0; nIndex < SkillEvent.vtBullets.size(); nIndex++)
        {
            StBulletObject& bulletObject = SkillEvent.vtBullets.at(nIndex);

            CBulletObject* pBulletObject = CreateBullet(bulletObject);
            ERROR_RETURN_FALSE(pBulletObject != NULL);

            pBulletObject->SaveNewData(Ntf);
        }

        CScene* pScene = m_pCastObject->GetScene();
        ERROR_RETURN_NULL(pScene != NULL);
        pScene->BroadMessage(MSG_BULLET_NEW_NTF, Ntf);
    }

    return TRUE;
}

ESkillStatus CSkillObject::GetSkillStatus()
{
    return m_SkillStatus;
}

BOOL CSkillObject::SetComboSkill(BOOL bCombo)
{
    m_bComboSkill = bCombo;

    return TRUE;
}

UINT32 CSkillObject::GetSkillID()
{
    return m_dwSkillID;
}

VOID CSkillObject::SetCalcTargets(BOOL bCalc)
{
    m_bCalcTargets = bCalc;
}

CBulletObject* CSkillObject::CreateBullet(StBulletObject& stBullet)
{
    CScene* pScene = m_pCastObject->GetScene();
    ERROR_RETURN_NULL(pScene != NULL);

    StBulletInfo* pBulletInfo = CStaticData::GetInstancePtr()->GetBulletInfo(stBullet.BulletID);
    ERROR_RETURN_NULL(pBulletInfo != NULL);

    CBulletObject* pBulletObject = pScene->CreateBullet(stBullet.BulletID, pBulletInfo, this, m_pCastObject->GetPos());
    ERROR_RETURN_NULL(pBulletObject != NULL);

    //创建方向型的子弹
    switch (pBulletInfo->BulletType)
    {
        case EBT_CHASE:         //追踪型飞弹:
        {
            pBulletObject->SetTargetObject(m_vtTargets.at(0));
        }
        break;

        case EBT_FIXDIRECTION:  //固定方向型飞弹:
        {
            pBulletObject->SetAngle(m_pCastObject->m_ft + stBullet.fAngle);
        }
        break;
        case EBT_FIXTARGETPOS:  //固定目标点飞弹
        {
            //pBulletObject->SetTargetPos();
        }
        break;
        case EBT_POINT:         //固定点飞弹
        {
        }
        break;
        case EBT_LINK:          //连接飞弹
        {
        }
        break;
        case EBT_ANNULAR:       //环形飞弹
        {
        }
        break;
        case EBT_BACK:          //回旋飞弹
        {
        }
        break;
        case EBT_EXTRACT:       //抽取飞弹
        {
        }
        break;
        case EBT_BOUNDCE:       //弹跳飞弹
        {
        }
        break;
        default:
        {

        }
    }

    return pBulletObject;
}

