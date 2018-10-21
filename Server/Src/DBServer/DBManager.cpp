#include "stdafx.h"
#include "DBManager.h"
#include "../Message/Msg_RetCode.pb.h"


CDBManager::CDBManager(void)
{
}

CDBManager::~CDBManager(void)
{
}

BOOL CDBManager::Init()
{
	std::string strHost	= CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_ip");
	UINT32 nPort		= CConfigFile::GetInstancePtr()->GetIntValue("mysql_game_svr_port");
	std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_user");
	std::string strPwd	= CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_pwd");
	std::string strDb	= CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_db_name");

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
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

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
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

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
		pData->set_createtime(QueryRes.getInt64Field("createtime", 0));
		pData->set_logontime(QueryRes.getInt64Field("logontime", 0));
		pData->set_logofftime(QueryRes.getIntField("logofftime", 0));
	}

	return TRUE;
}

BOOL CDBManager::GetBagData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from bag where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBBagData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_bagdata();
		}
		DBBagItem* pItem = pData->add_itemlist();
		pItem->set_guid(QueryRes.getInt64Field("guid", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_itemguid(QueryRes.getInt64Field("itemguid", 0));
		pItem->set_itemid(QueryRes.getIntField("itemid", 0));
		pItem->set_count(QueryRes.getInt64Field("count", 0));
		QueryRes.nextRow();
	}

	return TRUE;
}

BOOL CDBManager::GetCopyData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from copy where roleid = %lld", u64ID);
	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBCopyData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_copydata();
		}
		DBCopyItem* pItem = pData->add_copylist();
		pItem->set_copyid(QueryRes.getIntField("copyid", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_starnum(QueryRes.getIntField("star", 0));
		pItem->set_battlecnt(QueryRes.getIntField("battlecnt", 0));
		pItem->set_resetcnt(QueryRes.getIntField("resetcnt", 0));
		pItem->set_battletime(QueryRes.getInt64Field("battletime", 0));
		pItem->set_resettime(QueryRes.getInt64Field("resettime", 0));
		QueryRes.nextRow();
	}

	snprintf(szSql, SQL_BUFF_LEN, "select * from chapter where roleid = %lld", u64ID);
	QueryRes = m_DBConnection.querySQL(szSql);
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_copydata();
		}
		DBChapterItem* pItem = pData->add_chapterlist();
		pItem->set_copytype(QueryRes.getIntField("copytype", 0));
		pItem->set_chapterid(QueryRes.getIntField("chapterid", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_staraward(QueryRes.getIntField("staraward", 0));
		pItem->set_sceneaward(QueryRes.getIntField("sceneaward", 0));
		QueryRes.nextRow();
	}

	return TRUE;
}

BOOL CDBManager::GetEquipData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from equip where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBEquipData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_equipdata();
		}
		DBEquipItem* pItem = pData->add_equiplist();
		pItem->set_guid(QueryRes.getInt64Field("guid", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_equipid(QueryRes.getIntField("equipid", 0));
		pItem->set_isusing(QueryRes.getIntField("isuse", 0));
		pItem->set_refinelevel(QueryRes.getIntField("refinelvl", 0));
		pItem->set_starlevel(QueryRes.getIntField("starlvl", 0));
		pItem->set_strengthlvl(QueryRes.getIntField("strengthlvl", 0));
		QueryRes.nextRow();
	}

	return TRUE;
}

BOOL CDBManager::GetGemData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from gem where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBGemData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_gemdata();
		}
		DBGemItem* pItem = pData->add_gemlist();
		pItem->set_guid(QueryRes.getInt64Field("guid", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_gemid(QueryRes.getIntField("gemid", 0));
		pItem->set_pos(QueryRes.getIntField("pos", 0));
		pItem->set_refinelevel(QueryRes.getIntField("refinelvl", 0));
		pItem->set_starlevel(QueryRes.getIntField("starlvl", 0));
		pItem->set_strengthlvl(QueryRes.getIntField("strengthlvl", 0));
		QueryRes.nextRow();
	}

	return TRUE;
}

BOOL CDBManager::GetPetData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from pet where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBPetData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_petdata();
		}
		DBPetItem* pItem = pData->add_petlist();
		pItem->set_guid(QueryRes.getInt64Field("guid", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_petid(QueryRes.getIntField("petid", 0));
		pItem->set_refinelevel(QueryRes.getIntField("refinelvl", 0));
		pItem->set_strengthlvl(QueryRes.getIntField("strengthlvl", 0));
		pItem->set_starlevel(QueryRes.getIntField("starlvl", 0));
		pItem->set_isusing(QueryRes.getIntField("isuse", 0));
		QueryRes.nextRow();
	}

	return TRUE;
}

BOOL CDBManager::GetPartnerData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from partner where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBPartnerData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_partnerdata();
		}
		DBPartnerItem* pItem = pData->add_partnerlist();
		pItem->set_guid(QueryRes.getInt64Field("guid", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_partnerid(QueryRes.getIntField("partnerid", 0));
		pItem->set_refinelevel(QueryRes.getIntField("refinelvl", 0));
		pItem->set_strengthlvl(QueryRes.getIntField("strengthlvl", 0));
		pItem->set_starlevel(QueryRes.getIntField("starlvl", 0));
		pItem->set_setpos(QueryRes.getIntField("setpos", 0));
		QueryRes.nextRow();
	}

	return TRUE;
}

BOOL CDBManager::GetTaskData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CDBManager::GetMountData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from mount where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBMountData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_mountdata();
		}
		DBMountItem* pItem = pData->add_mountlist();
		pItem->set_guid(QueryRes.getInt64Field("guid", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_mountid(QueryRes.getIntField("mountid", 0));
		pItem->set_refinelevel(QueryRes.getIntField("refinelvl", 0));
		pItem->set_strengthlvl(QueryRes.getIntField("strengthlvl", 0));
		pItem->set_starlevel(QueryRes.getIntField("starlvl", 0));
		pItem->set_isusing(QueryRes.getIntField("isuse", 0));
		QueryRes.nextRow();
	}
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

