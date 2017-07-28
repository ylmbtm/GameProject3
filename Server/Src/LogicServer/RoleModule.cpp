#include "stdafx.h"
#include "RoleModule.h"
#include "Utility/Log/Log.h"
#include "../ServerData/ServerDefine.h"
#include "../ConfigData/ConfigData.h"
#include "Utility/CommonFunc.h"
#include "DataPool.h"

CRoleModule::CRoleModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CRoleModule::~CRoleModule()
{

}

BOOL CRoleModule::OnCreate(UINT64 u64RoleID)
{
    ERROR_RETURN_FALSE(m_pRoleDataObject != NULL);
    m_pRoleDataObject->lock();
    m_pRoleDataObject->m_Level = 1;
	m_pRoleDataObject->m_CityCopyID = 6;
    m_pRoleDataObject->unlock();

	return TRUE;
}

BOOL CRoleModule::InitBaseData( UINT64 u64RoleID, std::string Name, UINT32 dwActorID, UINT64 u64AccountID, UINT32 dwChannel )
{
	m_pRoleDataObject = g_pRoleDataObjectPool->newOjbect(TRUE);
	m_pRoleDataObject->lock();
	m_pRoleDataObject->m_u64ID = u64RoleID;
	m_pRoleDataObject->m_u64AccountID = u64AccountID;
	strcpy_s(m_pRoleDataObject->m_szName, 255, Name.c_str());
	m_pRoleDataObject->m_nLangID = 0;
	m_pRoleDataObject->m_ActorID = dwActorID;
	m_pRoleDataObject->unlock();
	return TRUE;
}

BOOL CRoleModule::OnDestroy()
{
	m_pRoleDataObject->release();

	m_pRoleDataObject = NULL;

	return TRUE;
}

BOOL CRoleModule::OnLogin()
{
	return TRUE;
}

BOOL CRoleModule::OnLogout()
{
	return TRUE;
}

BOOL CRoleModule::OnNewDay()
{
	return TRUE;
}

BOOL CRoleModule::ReadFromLoginAck( DBRoleLoginAck &Ack )
{
    m_pRoleDataObject = g_pRoleDataObjectPool->newOjbect(FALSE);
    m_pRoleDataObject->lock();
    m_pRoleDataObject->m_u64ID = Ack.roledata().roleid();
    m_pRoleDataObject->m_u64AccountID = Ack.roledata().accountid();
    strcpy_s(m_pRoleDataObject->m_szName, 255, Ack.roledata().name().c_str());
    m_pRoleDataObject->m_nLangID = Ack.roledata().langid();
    m_pRoleDataObject->m_ActorID = Ack.roledata().actorid();
    m_pRoleDataObject->m_Level = Ack.roledata().level();
    m_pRoleDataObject->m_Exp = Ack.roledata().exp();
    m_pRoleDataObject->m_VipLvl = Ack.roledata().viplvl();
    m_pRoleDataObject->m_VipExp = Ack.roledata().vipexp();
    m_pRoleDataObject->m_nLangID = Ack.roledata().langid();
    m_pRoleDataObject->unlock();


    return TRUE;
}

BOOL CRoleModule::CostMoney(UINT32 dwMoneyID, INT32 nMoneyNum)
{
	if ((dwMoneyID <= 0) || (dwMoneyID > MAX_MONEY_NUM)) 
	{
		CLog::GetInstancePtr()->LogError("CostMoney Error: Inavlid dwMoneyID :%d", dwMoneyID);
		return FALSE;
	}

	if (dwMoneyID <= 0) 
	{
		CLog::GetInstancePtr()->LogError("CostMoney Error : Invalid nMoneyNum :%d", nMoneyNum);
		return FALSE;
	}

	if (m_pRoleDataObject->m_Money[dwMoneyID-1] < nMoneyNum) 
	{
		CLog::GetInstancePtr()->LogError("CostMoney Error : Not Enough Money :%d", nMoneyNum);
		return FALSE;
	}

	m_pRoleDataObject->m_Money[dwMoneyID-1] -= nMoneyNum;

	return TRUE;
}

BOOL CRoleModule::CheckMoneyEnough(UINT32 dwMoneyID, INT32 nMoneyNum)
{
	if ((dwMoneyID <= 0) || (dwMoneyID > MAX_MONEY_NUM)) 
	{
		CLog::GetInstancePtr()->LogError("CheckMoneyEnough Error: Inavlid dwMoneyID :%d", dwMoneyID);
		return FALSE;
	}

	if (nMoneyNum <= 0) 
	{
		CLog::GetInstancePtr()->LogError("CheckMoneyEnough Error : Invalid nMoneyNum :%d", nMoneyNum);
		return FALSE;
	}

	if (m_pRoleDataObject->m_Money[dwMoneyID-1] >= nMoneyNum) 
	{
		return TRUE;
	}

	return FALSE;
}

UINT64 CRoleModule::GetMoney(UINT32 dwMoneyID)
{
	if ((dwMoneyID <= 0) || (dwMoneyID >= MAX_MONEY_NUM)) 
	{
		CLog::GetInstancePtr()->LogError("GetMoney Error: Inavlid dwMoneyID :%d", dwMoneyID);
		return 0;
	}

	return m_pRoleDataObject->m_Money[dwMoneyID-1];
}

UINT64 CRoleModule::AddMoney(UINT32 dwMoneyID, INT32 nMoneyNum)
{
	if (nMoneyNum <= 0) 
	{
		CLog::GetInstancePtr()->LogError("AddMoney Error: Inavlid nMoneyNum :%d", nMoneyNum);
		return m_pRoleDataObject->m_Money[dwMoneyID-1];
	}

	if ((dwMoneyID <= 0) || (dwMoneyID > MAX_MONEY_NUM)) 
	{
		CLog::GetInstancePtr()->LogError("AddMoney Error: Inavlid dwMoneyID :%d", dwMoneyID);
		return 0;
	}

	m_pRoleDataObject->m_Money[dwMoneyID-1] += nMoneyNum;
	if ( m_pRoleDataObject->m_Money[dwMoneyID-1] > CConfigData::GetInstancePtr()->GetMoneyMaxValue(dwMoneyID))
	{
		m_pRoleDataObject->m_Money[dwMoneyID-1] = CConfigData::GetInstancePtr()->GetMoneyMaxValue(dwMoneyID);
	}

	return  m_pRoleDataObject->m_Money[dwMoneyID-1];
}

BOOL CRoleModule::CostAction(UINT32 dwActionID, INT32 nActionNum)
{
	if ((dwActionID <= 0) || (dwActionID >= MAX_ACTION_NUM))
	{
		CLog::GetInstancePtr()->LogError("CostAction Error: Inavlid dwActionID :%d", dwActionID);
		return FALSE;
	}

	if (nActionNum <= 0) 
	{
		CLog::GetInstancePtr()->LogError("CostAction Error: Inavlid nActionNum :%d", nActionNum);
		return FALSE;
	}

	if (m_pRoleDataObject->m_Action[dwActionID-1] < nActionNum)
	{
		return FALSE;
	}

	m_pRoleDataObject->m_Action[dwActionID-1] -= nActionNum;

	if (m_pRoleDataObject->m_Action[dwActionID-1] < CConfigData::GetInstancePtr()->GetActoinMaxValue(dwActionID) )
	{
		if (m_pRoleDataObject->m_Actime[dwActionID-1] <= 0) 
		{
			m_pRoleDataObject->m_Actime[dwActionID-1]= CommonFunc::GetCurrTime();
		}
	} 
	else 
	{
		m_pRoleDataObject->m_Actime[dwActionID-1] = 0;
	}
	return TRUE;
}

BOOL CRoleModule::CheckActionEnough(UINT32 dwActionID, INT32 nActionNum)
{
	if ((dwActionID <= 0) || (dwActionID > MAX_ACTION_NUM)) 
	{
		CLog::GetInstancePtr()->LogError("CheckActionEnough Error: Inavlid dwActionID :%d", dwActionID);
			return FALSE;
	}

	if (nActionNum <= 0)
	{
		CLog::GetInstancePtr()->LogError("CheckActionEnough Error: Inavlid nActionNum :%d", nActionNum);
		return FALSE;
	}

	if (m_pRoleDataObject->m_Action[dwActionID-1] >= nActionNum) 
	{
		return TRUE;
	}

	UpdateAction(dwActionID); 

	if (m_pRoleDataObject->m_Action[dwActionID-1] < nActionNum) 
	{
		return FALSE;
	}

	return TRUE;
}

UINT64 CRoleModule::GetAction(UINT32 dwActionID)
{
	if ((dwActionID <= 0) || (dwActionID > MAX_ACTION_NUM)) 
	{
		CLog::GetInstancePtr()->LogError("GetAction Error: Inavlid dwActionID :%d", dwActionID);
		return 0;
	}

	UpdateAction(dwActionID);

	return m_pRoleDataObject->m_Action[dwActionID-1];
}

UINT64 CRoleModule::AddAction(UINT32 dwActionID, INT32 nActionNum)
{
	if ((dwActionID <= 0) || (dwActionID > MAX_ACTION_NUM)) 
	{
		CLog::GetInstancePtr()->LogError("AddAction Error: Inavlid dwActionID :%d", dwActionID);
		return 0;
	}

	UpdateAction(dwActionID);

	m_pRoleDataObject->m_Action[dwActionID-1] += nActionNum;

	if (m_pRoleDataObject->m_Action[dwActionID-1] >= CConfigData::GetInstancePtr()->GetActoinMaxValue(dwActionID)) 
	{
		m_pRoleDataObject->m_Actime[dwActionID-1] = 0;
	}

	return m_pRoleDataObject->m_Action[dwActionID-1];
}

BOOL CRoleModule::UpdateAction(UINT32 dwActionID)
{
	if (m_pRoleDataObject->m_Action[dwActionID-1] >= CConfigData::GetInstancePtr()->GetActoinMaxValue(dwActionID))
	{
		if (m_pRoleDataObject->m_Actime[dwActionID-1] > 0) 
		{
			CLog::GetInstancePtr()->LogError("UpdateAction error  StartTime is not 0");
		}
		m_pRoleDataObject->m_Actime[dwActionID-1] = 0;
		return FALSE;
	}

	if (m_pRoleDataObject->m_Actime[dwActionID-1] <= 0) 
	{
		CLog::GetInstancePtr()->LogError("UpdateAction error  action not max, but starttime is 0");
	}

	UINT32 dwTimeElapse = CommonFunc::GetCurrTime() - m_pRoleDataObject->m_Actime[dwActionID-1];

	if (dwTimeElapse < CConfigData::GetInstancePtr()->GetActoinUnitTime(dwActionID)) 
	{
		return FALSE;
	}

	UINT32 dwActionNum = int(dwTimeElapse) / CConfigData::GetInstancePtr()->GetActoinUnitTime(dwActionID);
	m_pRoleDataObject->m_Action[dwActionID-1] += dwActionNum;

	if (m_pRoleDataObject->m_Action[dwActionID-1] >= CConfigData::GetInstancePtr()->GetActoinMaxValue(dwActionID)) 
	{
		m_pRoleDataObject->m_Action[dwActionID-1] = CConfigData::GetInstancePtr()->GetActoinMaxValue(dwActionID);
		m_pRoleDataObject->m_Actime[dwActionID-1] = 0;
	} 
	else 
	{
		m_pRoleDataObject->m_Actime[dwActionID-1] = m_pRoleDataObject->m_Actime[dwActionID-1] + dwActionNum * CConfigData::GetInstancePtr()->GetActoinUnitTime(dwActionID);
	}

	return TRUE;
}
