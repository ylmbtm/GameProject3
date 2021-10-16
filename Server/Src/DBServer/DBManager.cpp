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
	INT32 nPort		= CConfigFile::GetInstancePtr()->GetIntValue("mysql_game_svr_port");
	std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_user");
	std::string strPwd	= CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_pwd");
	std::string strDb	= CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_db_name");

	if(!m_DBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
	{
		CLog::GetInstancePtr()->LogError("CDBManager::Init Error: Can not open mysql database! Reason:%s", m_DBConnection.GetErrorMsg());
        CLog::GetInstancePtr()->LogError("CDBManager::Init Error: Host:[%s]-User:[%s]-Pwd:[%s]-DBName:[%s]", strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str());
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

	snprintf(szSql, SQL_BUFF_LEN, "select * from player where accountid = %lld", u64AccountID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
    if (m_DBConnection.GetErrorNo() != 0)
    {
        CLog::GetInstancePtr()->LogError("CDBManager::GetRoleList Error :%s", m_DBConnection.GetErrorMsg());
        return FALSE;
    }

	Ack.set_accountid(u64AccountID);

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
    if (m_DBConnection.GetErrorNo() != 0)
    {
        CLog::GetInstancePtr()->LogError("CDBManager::GetRoleData Error :%s", m_DBConnection.GetErrorMsg());
        return FALSE;
    }

	if (QueryRes.eof())
	{
		return FALSE;
	}

	DBRoleData* pData = Ack.mutable_roledata();
	pData->set_roleid(QueryRes.getInt64Field("id", 0));
	pData->set_name(QueryRes.getStringField("name"));
	pData->set_carrerid(QueryRes.getIntField("carrerid", 0));
	pData->set_level(QueryRes.getIntField("level", 0));
	pData->set_exp(QueryRes.getInt64Field("exp", 0));
	pData->set_accountid(QueryRes.getInt64Field("accountid", 0));
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
	pData->set_logofftime(QueryRes.getInt64Field("logofftime", 0));
	pData->set_guildid(QueryRes.getInt64Field("guildid", 0));
	pData->set_channel(QueryRes.getIntField("channel", 0));
	pData->set_onlinetime(QueryRes.getIntField("onlinetime", 0));
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
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from task where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBTaskData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_taskdata();
		}
		DBTaskItem* pItem = pData->add_tasklist();
		pItem->set_taskid(QueryRes.getIntField("id", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_status(QueryRes.getIntField("task_status", 0));
		pItem->set_progress(QueryRes.getIntField("progress", 0));

		QueryRes.nextRow();
	}
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
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from activity where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBActivityData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_activitydata();
		}

		DBActivityItem* pItem = pData->add_activitylist();
		INT32 nLen = QueryRes.getIntField("data_len", 0);
		pItem->set_activityid(QueryRes.getIntField("id", 0));
		pItem->set_activitytype(QueryRes.getIntField("type", 0));
		pItem->set_roleid(QueryRes.getIntField("roleid", 0));
		pItem->set_jointime(QueryRes.getIntField("join_time", 0));
		pItem->set_datalen(nLen);


		if (nLen > 0)
		{
			INT32 nLen2 = 0;
			const unsigned char* pData = QueryRes.getBlobField("data", nLen2);
			pItem->set_data(pData, nLen);
		}

		QueryRes.nextRow();
	}

	return TRUE;
}

BOOL CDBManager::GetMailData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from mail where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBMailData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_maildata();
		}

		DBMailItem* pItem = pData->add_maillist();
		pItem->set_guid(QueryRes.getInt64Field("id", 0));
		pItem->set_sender(QueryRes.getStringField("sendername", 0));
		pItem->set_senderid(QueryRes.getInt64Field("senderid", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_groupid(QueryRes.getInt64Field("groupid", 0));
		pItem->set_time(QueryRes.getInt64Field("mail_time", 0));
		pItem->set_title(QueryRes.getStringField("title", 0));
		pItem->set_content(QueryRes.getStringField("content", 0));
		pItem->set_mailtype(QueryRes.getIntField("mailtype", 0));
		pItem->set_status(QueryRes.getIntField("mailstatus", 0));

		INT32 nLen = 0;
		const unsigned char* pData = QueryRes.getBlobField("itemdata", nLen);
		pItem->set_items(pData, nLen);
		QueryRes.nextRow();
	}
	return TRUE;
}

BOOL CDBManager::GetCounterData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from counter where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBCounterData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_counterdata();
		}
		DBCounterItem* pItem = pData->add_counterlist();
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_counterid(QueryRes.getIntField("id", 0));
		pItem->set_index(QueryRes.getIntField("cindex", 0));
		pItem->set_value(QueryRes.getInt64Field("value", 0));
		pItem->set_time(QueryRes.getInt64Field("time", 0));

		QueryRes.nextRow();
	}

	return TRUE;
}

BOOL CDBManager::GetFriendData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };
	snprintf(szSql, SQL_BUFF_LEN, "select * from relationship where roleid = %lld", u64ID);
	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBFriendData* pData = NULL;

	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_frienddata();
		}
		auto pItem = pData->add_friendlist();
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_friendid(QueryRes.getInt64Field("other_id", 0));
		QueryRes.nextRow();
	}
	return TRUE;
}

BOOL CDBManager::GetSkillData(UINT64 u64ID, DBRoleLoginAck& Ack)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	snprintf(szSql, SQL_BUFF_LEN, "select * from skill where roleid = %lld", u64ID);

	CppMySQLQuery  QueryRes = m_DBConnection.querySQL(szSql);
	DBSkillData* pData = NULL;
	while (!QueryRes.eof())
	{
		if (pData == NULL)
		{
			pData = Ack.mutable_skilldata();
		}
		DBSkillItem* pItem = pData->add_skilllist();
		pItem->set_skillid(QueryRes.getIntField("id", 0));
		pItem->set_roleid(QueryRes.getInt64Field("roleid", 0));
		pItem->set_level(QueryRes.getIntField("level", 0));
		pItem->set_keypos(QueryRes.getIntField("key_pos", 0));
		QueryRes.nextRow();
	}
	return TRUE;
}

