#include "stdafx.h"
#include "DBManager.h"
#include "CommonFunc.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../ServerData/ServerDefine.h"
#include "Log.h"


CDBManager::CDBManager(void)
{
}

CDBManager::~CDBManager(void)
{
}

BOOL CDBManager::Init()
{
	std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_ip");
	UINT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_game_svr_port");
	std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_user");
	std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_pwd");
	std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_db_name");

	if(!m_DBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
	{
		CLog::GetInstancePtr()->LogError("CDBManager::Init Error: Can not open database!!!");
		return FALSE;
	}
	return TRUE;
}

BOOL CDBManager::Uninit()
{
	m_DBConnection.close();

	return TRUE;
}

BOOL CDBManager::GetRoleList(UINT64 u64AccountID, RoleListAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN];

	snprintf(szSql, SQL_BUFF_LEN, "select * from player where account_id = %lld", u64AccountID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);

	while(!QueryRes.eof())
	{
		RoleItem* pNode = Ack.add_rolelist();
		pNode->set_roleid(QueryRes.getInt64Field("id", 0));
		pNode->set_name(QueryRes.getStringField("name"));
		pNode->set_carrer(QueryRes.getIntField("carrerid", 0));
		pNode->set_level(QueryRes.getIntField("level", 0));
		QueryRes.nextRow();
	}

	return TRUE;
}

BOOL CDBManager::GetRoleData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN];

	snprintf(szSql, SQL_BUFF_LEN, "select * from player where id = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);

	if(!QueryRes.eof())
	{
		DBRoleData* pData = Ack.mutable_roledata();
		pData->set_roleid(QueryRes.getInt64Field("id", 0));
		pData->set_name(QueryRes.getStringField("name"));
		pData->set_carrerid(QueryRes.getIntField("carrerid", 0));
		pData->set_level(QueryRes.getIntField("level", 0));
		pData->set_exp(QueryRes.getInt64Field("exp", 0));
		pData->set_accountid(QueryRes.getInt64Field("account_id", 0));
		pData->set_citycopyid(QueryRes.getIntField("citycopyid", 0));
		pData->add_action(QueryRes.getInt64Field("action1", 0));
		pData->add_action(QueryRes.getInt64Field("action2", 0));
		pData->add_action(QueryRes.getInt64Field("action3", 0));
		pData->add_action(QueryRes.getInt64Field("action4", 0));
		pData->add_actime(QueryRes.getInt64Field("actime1", 0));
		pData->add_actime(QueryRes.getInt64Field("actime2", 0));
		pData->add_actime(QueryRes.getInt64Field("actime3", 0));
		pData->add_actime(QueryRes.getInt64Field("actime4", 0));
		pData->add_money(QueryRes.getInt64Field("money1", 0));
		pData->add_money(QueryRes.getInt64Field("money2", 0));
		pData->add_money(QueryRes.getInt64Field("money3", 0));
		pData->add_money(QueryRes.getInt64Field("money4", 0));
		pData->add_money(QueryRes.getInt64Field("money5", 0));
		pData->add_money(QueryRes.getInt64Field("money6", 0));
		pData->add_money(QueryRes.getInt64Field("money7", 0));
		pData->add_money(QueryRes.getInt64Field("money8", 0));
		pData->set_createtime(QueryRes.getInt64Field("createtime", 0));
		pData->set_logontime(QueryRes.getInt64Field("logontime", 0));
		pData->set_logofftime(QueryRes.getIntField("logofftime", 0));
	}

	return TRUE;
}

BOOL CDBManager::GetBagData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetCopyData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetEquipData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetPetData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetPartnerData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetTaskData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetMountData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetActivtyData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetMailData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetCounterData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetFriendData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

