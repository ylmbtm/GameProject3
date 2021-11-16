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
#include "MailModule.h"

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

BOOL CWebCommandMgr::SendWebResult(INT32 nConnID, EWebResult eResult)
{
	std::string strResult = CommonConvert::IntToString((INT64)eResult);

	ServiceBase::GetInstancePtr()->SendMsgRawData(nConnID, MSG_PHP_GM_COMMAND_ACK, 0, 0, strResult.c_str(), (UINT32)strResult.size());

	return TRUE;
}

BOOL CWebCommandMgr::DispatchPacket(NetPacket* pNetPacket)
{
	if (MSG_PHP_GM_COMMAND_REQ == pNetPacket->m_nMsgID)
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
	CommonConvert::StrCopy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth() + 1);

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
			OnGmReloadTable(Params, pNetPacket->m_nConnID);
		}
		break;
		case EWA_SEAL_ROLE:
		{
			OnGmSealRole(Params, pNetPacket->m_nConnID);
		}
		break;
		case EWA_SINGLE_MAIL:           //单发邮件
		{
			OnGmSingleMail(Params, pNetPacket->m_nConnID);
		}
		break;
		case EWA_GROUP_MAIL:           //群发邮件
		{
			OnGmGroupMail(Params, pNetPacket->m_nConnID);
		}
		break;
		case EWA_DELTE_MAIL:           //删除邮件
		{
			OnGmDeleteMail(Params, pNetPacket->m_nConnID);
		}
		break;
		case EWA_GM_COMMAND:           //后台发的GM指令
		{
			OnGmCommand(Params, pNetPacket->m_nConnID);
		}
		break;
		case EWA_PAY_CALLBACK:
		{
			SendWebResult(pNetPacket->m_nConnID, EWR_SUCCESSED);
			CPayManager::GetInstancePtr()->OnGmPayCallBack(Params, pNetPacket->m_nConnID);
		}
		break;
		default:
		{
			SendWebResult(pNetPacket->m_nConnID, EWR_INVALID_ACT);
		}
		break;
	}

	return TRUE;
}

void CWebCommandMgr::OnGmReloadTable(HttpParameter& hParams, INT32 nConnID)
{
	ERROR_RETURN_NONE(nConnID != 0);

	std::string strName = hParams.GetStrValue("TableName");

	std::vector<std::string> vtTbNames;

	CommonConvert::SpliteString(strName, '|', vtTbNames);

	for (int i = 0; i < vtTbNames.size(); i++)
	{
		CStaticData::GetInstancePtr()->ReloadConfigData(vtTbNames.at(i));
	}

	SendWebResult(nConnID, EWR_SUCCESSED);

	return;
}

void CWebCommandMgr::OnGmSealRole(HttpParameter& hParams, INT32 nConnID)
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

void CWebCommandMgr::OnGmSingleMail(HttpParameter& hParams, INT32 nConnID)
{
	ERROR_RETURN_NONE(nConnID != 0);
	SendWebResult(nConnID, EWR_SUCCESSED);
	std::string strSenderName = hParams.GetStrValue("sender");
	UINT64 uRoleID = hParams.GetLongValue("receiver_id");
	std::string strRoleName = hParams.GetStrValue("reciver_name");
	std::string strTitle = hParams.GetStrValue("mail_title");
	std::string strContent = hParams.GetStrValue("mail_content");
	INT32 strLanguage = hParams.GetIntValue("language");

	std::vector<StMailItem> vtItems;
	INT32 nItem[2] = {0};
	CommonConvert::StringToVector(hParams.GetStrValue("itemid1").c_str(), nItem, 2);
	if (nItem[0] != 0)
	{
		vtItems.push_back(StMailItem(nItem[0], nItem[1]));
	}

	memset(nItem, 0, sizeof(nItem));
	CommonConvert::StringToVector(hParams.GetStrValue("itemid2").c_str(), nItem, 2);
	if (nItem[0] != 0)
	{
		vtItems.push_back(StMailItem(nItem[0], nItem[1]));
	}

	memset(nItem, 0, sizeof(nItem));
	CommonConvert::StringToVector(hParams.GetStrValue("itemid3").c_str(), nItem, 2);
	if (nItem[0] != 0)
	{
		vtItems.push_back(StMailItem(nItem[0], nItem[1]));
	}

	memset(nItem, 0, sizeof(nItem));
	CommonConvert::StringToVector(hParams.GetStrValue("itemid4").c_str(), nItem, 2);
	if (nItem[0] != 0)
	{
		vtItems.push_back(StMailItem(nItem[0], nItem[1]));
	}

	if (uRoleID == 0)
	{
		uRoleID = CSimpleManager::GetInstancePtr()->GetRoleIDByName(strRoleName);
	}
	else
	{
		CSimpleInfo* pSimpleInfo = CSimpleManager::GetInstancePtr()->GetSimpleInfoByID(uRoleID);
		if (pSimpleInfo == NULL)
		{
			uRoleID = CSimpleManager::GetInstancePtr()->GetRoleIDByName(strRoleName);
		}
	}

	CMailManager::GetInstancePtr()->SendSingleMail(uRoleID, EMT_CUSTOM, strContent, vtItems, strSenderName, strTitle);

	return;
}

void CWebCommandMgr::OnGmGroupMail(HttpParameter& hParams, INT32 nConnID)
{
	ERROR_RETURN_NONE(nConnID != 0);
	SendWebResult(nConnID, EWR_SUCCESSED);
	std::string strSenderName = hParams.GetStrValue("sender");
	std::string strTitle = hParams.GetStrValue("mail_title");
	std::string strContent = hParams.GetStrValue("mail_content");
	INT32 nLanguage = hParams.GetIntValue("language");
	INT32 nRecvGroup = hParams.GetIntValue("reciver_group"); //1:全部玩家; 2:在线玩家


	std::vector<StMailItem> vtItems;
	INT32 nItem[2] = { 0 };
	CommonConvert::StringToVector(hParams.GetStrValue("itemid1").c_str(), nItem, 2);
	if (nItem[0] != 0)
	{
		vtItems.push_back(StMailItem(nItem[0], nItem[1]));
	}

	memset(nItem, 0, sizeof(nItem));
	CommonConvert::StringToVector(hParams.GetStrValue("itemid2").c_str(), nItem, 2);
	if (nItem[0] != 0)
	{
		vtItems.push_back(StMailItem(nItem[0], nItem[1]));
	}

	memset(nItem, 0, sizeof(nItem));
	CommonConvert::StringToVector(hParams.GetStrValue("itemid3").c_str(), nItem, 2);
	if (nItem[0] != 0)
	{
		vtItems.push_back(StMailItem(nItem[0], nItem[1]));
	}

	memset(nItem, 0, sizeof(nItem));
	CommonConvert::StringToVector(hParams.GetStrValue("itemid4").c_str(), nItem, 2);
	if (nItem[0] != 0)
	{
		vtItems.push_back(StMailItem(nItem[0], nItem[1]));
	}

	CMailManager::GetInstancePtr()->SendGroupMail(strSenderName, strTitle, strContent, vtItems, nRecvGroup);

	return;
}

void CWebCommandMgr::OnGmDeleteMail(HttpParameter& hParams, INT32 nConnID)
{
	ERROR_RETURN_NONE(nConnID != 0);
	SendWebResult(nConnID, EWR_SUCCESSED);
	UINT64 uRoleID = hParams.GetLongValue("roleid");
	INT32 nMailType = hParams.GetIntValue("mailtype");
	UINT64 uMailGuid = hParams.GetLongValue("mailguid");

	if (nMailType == 1) //群发邮件
	{
		//先删除群邮件表里的
		CMailManager::GetInstancePtr()->DeleteGroupMail(uMailGuid);
	}
	else  if (nMailType == 2) //单人邮件
	{
		CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(uRoleID);
		ERROR_RETURN_NONE(pPlayer != NULL);

		CMailModule* pMailModule = (CMailModule*)pPlayer->GetModuleByType(MT_MAIL);
		ERROR_RETURN_NONE(pMailModule != NULL);

		pMailModule->DeleteMail(uMailGuid);
	}

	return;
}

void CWebCommandMgr::OnGmCommand(HttpParameter& hParams, INT32 nConnID)
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

