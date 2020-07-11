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
#include "MailManager.h"

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
		{
			CGameSvrMgr::GetInstancePtr()->BroadMsgToAll(MSG_PHP_GM_COMMAND_REQ, szMsgBuf, pNetPacket->m_pDataBuffer->GetBodyLenth());
			OnGmReloadTable(Params, pNetPacket->m_dwConnID);
		}
		break;
		case EWA_SEAL_ROLE:
		{
			OnGmSealRole(Params, pNetPacket->m_dwConnID);
		}
		break;
		case EWA_SINGLE_MAIL:           //单发邮件
		{
			OnGmSingleMail(Params, pNetPacket->m_dwConnID);
		}
		break;
		case EWA_GROUP_MAIL:           //群发邮件
		{
			OnGmGroupMail(Params, pNetPacket->m_dwConnID);
		}
		break;
		case EWA_GM_COMMAND:           //后台发的GM指令
		{
			OnGmCommand(Params, pNetPacket->m_dwConnID);
		}
		break;
		case EWA_PAY_CALLBACK:
		{
			SendWebResult(pNetPacket->m_dwConnID, EWR_SUCCESSED);
			CPayManager::GetInstancePtr()->OnGmPayCallBack(Params, pNetPacket->m_dwConnID);
		}
		break;
		default:
		{
			SendWebResult(pNetPacket->m_dwConnID, EWR_INVALID_ACT);
		}
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

void CWebCommandMgr::OnGmSingleMail(HttpParameter& hParams, UINT32 nConnID)
{
	ERROR_RETURN_NONE(nConnID != 0);
	SendWebResult(nConnID, EWR_SUCCESSED);
	UINT64 uRoleID = hParams.GetLongValue("roleid");
	std::string strRoleName = hParams.GetStrValue("rolename");
	std::string strTitle = hParams.GetStrValue("title");
	std::string strContent = hParams.GetStrValue("content");
	INT32 strLanguage = hParams.GetIntValue("language");

	UINT32 nItem[4] = { 0 };
	nItem[0] = hParams.GetIntValue("itemid1");
	nItem[1] = hParams.GetIntValue("itemid2");
	nItem[2] = hParams.GetIntValue("itemid3");
	nItem[3] = hParams.GetIntValue("itemid4");

	UINT32 nItemNum[4] = { 0 };
	nItemNum[0] = hParams.GetIntValue("itemid1");
	nItemNum[1] = hParams.GetIntValue("itemid2");
	nItemNum[2] = hParams.GetIntValue("itemid3");
	nItemNum[3] = hParams.GetIntValue("itemid4");

	std::vector<StMailItem> vtItems = { {nItem[0], nItemNum[0] }, {nItem[1], nItemNum[1]}, {nItem[2], nItemNum[2]}, {nItem[3], nItemNum[3]} };

	CMailManager::GetInstancePtr()->SendSingleMail(uRoleID, "GM", strTitle, strContent, vtItems);


	return;
}

void CWebCommandMgr::OnGmGroupMail(HttpParameter& hParams, UINT32 nConnID)
{
	ERROR_RETURN_NONE(nConnID != 0);
	SendWebResult(nConnID, EWR_SUCCESSED);
	UINT64 uRoleID = hParams.GetLongValue("roleid");
	std::string strRoleName = hParams.GetStrValue("rolename");
	std::string strTitle = hParams.GetStrValue("title");
	std::string strContent = hParams.GetStrValue("content");
	INT32 strLanguage = hParams.GetIntValue("language");
	INT32 nGroupID = hParams.GetIntValue("groupid");

	UINT32 nItem[4] = { 0 };
	nItem[0] = hParams.GetIntValue("itemid1");
	nItem[1] = hParams.GetIntValue("itemid2");
	nItem[2] = hParams.GetIntValue("itemid3");
	nItem[3] = hParams.GetIntValue("itemid4");

	UINT32 nItemNum[4] = { 0 };
	nItemNum[0] = hParams.GetIntValue("itemid1");
	nItemNum[1] = hParams.GetIntValue("itemid2");
	nItemNum[2] = hParams.GetIntValue("itemid3");
	nItemNum[3] = hParams.GetIntValue("itemid4");

	std::vector<StMailItem> vtItems = { {nItem[0], nItemNum[0] }, {nItem[1], nItemNum[1]}, {nItem[2], nItemNum[2]}, {nItem[3], nItemNum[3]} };

	CMailManager::GetInstancePtr()->SendGroupMail(nGroupID, "GM", strTitle, strContent, vtItems);
	return;
}

void CWebCommandMgr::OnGmCommand(HttpParameter& hParams, UINT32 nConnID)
{
	std::string strRoleName = hParams.GetStrValue("role_name");

	std::string strCmd = hParams.GetStrValue("gm_command");

	std::vector<std::string>  vtString;

	CommonConvert::SpliteString(strCmd, "%20", vtString);

	CPlayerObject* pPlayer = NULL;

	if (strRoleName != "@@")
	{
		UINT64 uRoleID = CSimpleManager::GetInstancePtr()->GetRoleIDByName(strRoleName);
		if (uRoleID == 0)
		{
			SendWebResult(nConnID, EWR_FAILURE);
			return;
		}

		pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(uRoleID);
	}


	return;
}

