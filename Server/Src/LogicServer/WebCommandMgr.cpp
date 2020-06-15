#include "stdafx.h"
#include "WebCommandMgr.h"
#include "GameService.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "PacketHeader.h"
#include "PlayerManager.h"
#include "MsgHandlerManager.h"
#include "HttpParameter.h"
#include "BagModule.h"
#include "StaticData.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Game_Define.pb.h"
#include "SimpleManager.h"
#include "PayManager.h"

CWebCommandMgr::CWebCommandMgr()
{

}

CWebCommandMgr::~CWebCommandMgr()
{

}

CWebCommandMgr* CWebCommandMgr::GetInstancePtr()
{
	static CWebCommandMgr _StaticMgr;

	return &_StaticMgr;
}

BOOL CWebCommandMgr::Init()
{
	return TRUE;
}

BOOL CWebCommandMgr::Uninit()
{
	return TRUE;
}

BOOL CWebCommandMgr::SendWebResult(UINT32 nConnID, EWebResult eResult)
{
	std::string strResult = CommonConvert::IntToString((INT64)eResult);

	ServiceBase::GetInstancePtr()->SendMsgRawData(nConnID, MSG_PHP_GM_COMMAND_ACK, 0, 0, strResult.c_str(), (UINT32)strResult.size());

	return TRUE;
}

BOOL CWebCommandMgr::DispatchPacket(NetPacket* pNetPacket)
{
	if (MSG_PHP_GM_COMMAND_REQ == pNetPacket->m_dwMsgID)
	{
		OnMsgGmCommandReq(pNetPacket);

		return TRUE;
	}

	return FALSE;
}

BOOL CLogicMsgHandler::ProcessGMCommand(UINT64 u64ID, std::vector<std::string>& vtParam)
{
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(u64ID);
	ERROR_RETURN_TRUE(pPlayer != NULL);
	ERROR_RETURN_TRUE(vtParam.size() >= 1);

	if (std::strcmp(vtParam[0].c_str(), "@@additem") == 0)
	{
		ERROR_RETURN_TRUE(vtParam.size() >= 3);
		CBagModule* pBag = (CBagModule*)pPlayer->GetModuleByType(MT_BAG);
		ERROR_RETURN_TRUE(pBag != NULL);
		pBag->AddItem(CommonConvert::StringToInt(vtParam[1].c_str()), CommonConvert::StringToInt(vtParam[2].c_str()));
	}

	return TRUE;
}


BOOL CWebCommandMgr::OnMsgGmCommandReq(NetPacket* pNetPacket)
{
	CHAR szMsgBuf[1024] = { 0 };
	strncpy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	HttpParameter Params;
	Params.ParseStringToMap(szMsgBuf);
	std::string strAction = Params.GetStrValue("Action");
	CLog::GetInstancePtr()->LogInfo("Web Action :%s", strAction.c_str());

	EWebAction eWebAction = (EWebAction)CommonConvert::StringToInt(strAction.c_str());
	switch (eWebAction)
	{
		case EWA_RELOAD_TABLE:
			CGameSvrMgr::GetInstancePtr()->BroadMsgToAll(MSG_PHP_GM_COMMAND_REQ, szMsgBuf, pNetPacket->m_pDataBuffer->GetBodyLenth());
			OnGmReloadTable(Params, pNetPacket->m_dwConnID);
			break;
		case EWA_SEAL_ROLE:
			OnGmSealRole(Params, pNetPacket->m_dwConnID);
			break;
		case EWA_PAY_CALLBACK:
			SendWebResult(pNetPacket->m_dwConnID, EWR_SUCCESSED);
			CPayManager::GetInstancePtr()->OnGmPayCallBack(Params, pNetPacket->m_dwConnID);
		default:
			SendWebResult(pNetPacket->m_dwConnID, EWR_INVALID_ACT);
			break;
	}

	return TRUE;
}

void CWebCommandMgr::OnGmReloadTable(HttpParameter& hParams, UINT32 nConnID)
{
	ERROR_RETURN_NONE(nConnID != 0);

	std::string strName = hParams.GetStrValue("TableName");

	CStaticData::GetInstancePtr()->ReloadConfigData(strName);

	SendWebResult(nConnID, EWR_SUCCESSED);

	return;
}

void CWebCommandMgr::OnGmSealRole(HttpParameter& hParams, UINT32 nConnID)
{
	ERROR_RETURN_NONE(nConnID != 0);

	UINT64 uRoleID = hParams.GetLongValue("roleid");
	std::string strRoleName = hParams.GetStrValue("rolename");
	BOOL bSeal = hParams.GetIntValue("seal");
	UINT64 uTime = hParams.GetLongValue("sealtime");

	if (uRoleID <= 0)
	{
		uRoleID = CSimpleManager::GetInstancePtr()->GetRoleIDByName(strRoleName);
	}

	if (uRoleID <= 0)
	{
		SendWebResult(nConnID, EWR_FAILURE);
		CLog::GetInstancePtr()->LogError("CWebCommandMgr::OnGmSealRole Invalid roleid and Invalid rolename");
		return;
	}

	return;
}

