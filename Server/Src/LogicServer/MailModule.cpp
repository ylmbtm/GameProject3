#include "stdafx.h"
#include "MailModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../ConfigData/ConfigData.h"
#include "Log.h"

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
		MailDataObject *pObject = g_pMailDataObjectPool->NewOjbect(FALSE);


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