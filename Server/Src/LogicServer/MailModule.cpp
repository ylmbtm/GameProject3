#include "stdafx.h"
#include "MailModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../ConfigData/ConfigData.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"

CMailModule::CMailModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CMailModule::~CMailModule()
{

}

BOOL CMailModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CMailModule::OnDestroy()
{
	for(auto itor = m_mapMailData.begin(); itor != m_mapMailData.end(); itor++)
	{
		itor->second->release();
	}

	m_mapMailData.clear();

	return TRUE;
}

BOOL CMailModule::OnLogin()
{
	return TRUE;
}

BOOL CMailModule::OnLogout()
{
	return TRUE;
}

BOOL CMailModule::OnNewDay()
{
	return TRUE;
}

BOOL CMailModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBMailData& MailData = Ack.maildata();
	for(int i = 0; i < MailData.maillist_size(); i++)
	{
		MailDataObject* pObject = g_pMailDataObjectPool->NewObject(FALSE);


		m_mapMailData.insert(std::make_pair(pObject->m_uGuid, pObject));
	}

	return TRUE;
}

BOOL CMailModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}



BOOL CMailModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{

	return TRUE;
}

BOOL CMailModule::AddMail(MailDataObject* pMail)
{
	m_mapMailData.insert(std::make_pair(pMail->m_uGuid, pMail));

	m_setChange.insert(pMail->m_uGuid);

	return TRUE;
}

BOOL CMailModule::DeleteMail(UINT64 uGuid)
{
	auto itor = m_mapMailData.find(uGuid);
	if(itor == m_mapMailData.end())
	{
		return FALSE;
	}

	MailDataObject* pObject = itor->second;

	pObject->destroy();

	m_mapMailData.erase(itor);

	m_setRemove.insert(uGuid);

	return TRUE;
}

BOOL CMailModule::SendMail(std::string strSender, std::string strTitle, std::string strContent)
{
	MailDataObject* pMailObject = g_pMailDataObjectPool->NewObject(TRUE);
	pMailObject->lock();
	pMailObject->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pMailObject->m_uRoleID = m_pOwnPlayer->GetObjectID();
	strncpy(pMailObject->m_szSender, strSender.c_str(), min(ROLE_NAME_LEN, strSender.size()));
	pMailObject->m_uTime = CommonFunc::GetCurrTime();
	pMailObject->unlock();
	return AddMail(pMailObject);
}

MailDataObject* CMailModule::GetMailByGuid(UINT64 uGuid)
{
	auto itor = m_mapMailData.find(uGuid);
	if(itor != m_mapMailData.end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL CMailModule::NotifyChange()
{
	if (m_setChange.size() <= 0 && m_setRemove.size() <= 0)
	{
		return TRUE;
	}

	MailChangeNty Nty;
	for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
	{
		MailDataObject* pObject = GetMailByGuid(*itor);
		ERROR_CONTINUE_EX(pObject != NULL);

		MailItem* pItem = Nty.add_changelist();
	}

	for(auto itor = m_setRemove.begin(); itor != m_setRemove.end(); itor++)
	{
		Nty.add_removelist(*itor);
	}

	m_pOwnPlayer->SendMsgProtoBuf(MSG_MAIL_CHANGE_NTY, Nty);

	m_setChange.clear();
	m_setRemove.clear();

	return TRUE;
}
