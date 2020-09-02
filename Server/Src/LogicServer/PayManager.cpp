#include "stdafx.h"
#include "PayManager.h"
#include "GameService.h"
#include "StaticData.h"

CPayManager::CPayManager()
{
}

CPayManager::~CPayManager()
{

}

CPayManager* CPayManager::GetInstancePtr()
{
	static CPayManager _StaticMgr;

	return &_StaticMgr;
}

BOOL CPayManager::Init()
{
	RegisterMessageHanler();

	return TRUE;
}

BOOL CPayManager::Uninit()
{
	return TRUE;
}

VOID CPayManager::RegisterMessageHanler()
{
}

BOOL CPayManager::Update()
{
	return TRUE;
}

void CPayManager::OnGmPayCallBack(HttpParameter& hParams, UINT32 nConnID)
{
	UINT64 uRoleID = hParams.GetLongValue("roleid");        //角色ID
	UINT32 nProductID = hParams.GetIntValue("productid");  //充值档位
	UINT32 nChannel = hParams.GetIntValue("channel");
	std::string strOrderID = hParams.GetStrValue("orderid");
	UINT64 uFinishTime = hParams.GetLongValue("finishtime");
	INT32 nMoney = hParams.GetIntValue("money");

	if (m_setFinishedOrder.find(strOrderID) != m_setFinishedOrder.end())
	{
		CLog::GetInstancePtr()->LogError("CPayManager::OnGmPayCallBack Error! Duplicated strOrderID:%s", strOrderID.c_str());
		return;
	}

	const StChargeInfo* pChargeInfo = CStaticData::GetInstancePtr()->GetChargeInfo(nProductID);
	if (pChargeInfo == NULL)
	{
		CLog::GetInstancePtr()->LogError("CPayManager::OnGmPayCallBack Error! Invalid ProductID:%d", nProductID);
		return;
	}

	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(uRoleID);
	if (pPlayer == NULL)
	{
		CLog::GetInstancePtr()->LogError("CPayManager::OnGmPayCallBack Error! Invalid uRoleID:%d", uRoleID);
		return;
	}









}

