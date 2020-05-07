#include "stdafx.h"
#include "AccountManager.h"

CAccountObjectMgr::CAccountObjectMgr()
{
	m_IsRun			= FALSE;
	m_pThread		= NULL;
	m_bCrossChannel = FALSE;
	m_u64MaxID		= 0;
}

CAccountObjectMgr::~CAccountObjectMgr()
{
	m_IsRun			= FALSE;
	m_pThread		= NULL;
	m_bCrossChannel = FALSE;
}

BOOL CAccountObjectMgr::LoadCacheAccount()
{
	std::string strHost	= CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_ip");
	UINT32 nPort		= CConfigFile::GetInstancePtr()->GetIntValue("mysql_acc_svr_port");
	std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_user");
	std::string strPwd	= CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_pwd");
	std::string strDb	= CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_db_name");
	m_bCrossChannel		= CConfigFile::GetInstancePtr()->GetIntValue("account_cross_channel");

	if(!m_DBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
	{
		CLog::GetInstancePtr()->LogError("LoadCacheAccount Error: Can not open mysql database! Reason:%s", m_DBConnection.GetErrorMsg());
		return FALSE;
	}

	CppMySQLQuery QueryResult = m_DBConnection.querySQL("select * from account");
	CAccountObject* pTempObject = NULL;
	while(!QueryResult.eof())
	{
		pTempObject = AddAccountObject(QueryResult.getInt64Field("id"),
		                               QueryResult.getStringField("name"),
		                               QueryResult.getIntField("channel"));

		ERROR_RETURN_FALSE(pTempObject != NULL);

		pTempObject->m_strPassword	= QueryResult.getStringField("password");
		pTempObject->m_uCreateTime	= QueryResult.getInt64Field("create_time");
		pTempObject->m_uSealTime = QueryResult.getInt64Field("seal_end_time");
		pTempObject->m_dwLastSvrID = QueryResult.getIntField("lastsvrid");
		if(m_u64MaxID < (UINT64)QueryResult.getInt64Field("id"))
		{
			m_u64MaxID = (UINT64)QueryResult.getInt64Field("id");
		}

		QueryResult.nextRow();
	}

	return TRUE;
}

CAccountObject* CAccountObjectMgr::GetAccountObjectByID( UINT64 AccountID )
{
	return GetByKey(AccountID);
}

CAccountObject* CAccountObjectMgr::CreateAccountObject(const std::string& strName, const std::string& strPwd, UINT32 dwChannel)
{
	m_u64MaxID += 1;

	CAccountObject* pObj = InsertAlloc(m_u64MaxID);
	ERROR_RETURN_NULL(pObj != NULL);

	pObj->m_strName			= strName;
	pObj->m_strPassword		= strPwd;
	pObj->m_ID				= m_u64MaxID;
	pObj->m_dwChannel		= dwChannel;
	pObj->m_uCreateTime		= CommonFunc::GetCurrTime();

	if (m_bCrossChannel)
	{
		m_mapNameObj.insert(std::make_pair(strName, pObj));
	}
	else
	{
		m_mapNameObj.insert(std::make_pair(strName + CommonConvert::IntToString(dwChannel), pObj));
	}

	m_ArrChangedAccount.push(pObj);

	return pObj;
}

BOOL CAccountObjectMgr::ReleaseAccountObject(UINT64 AccountID )
{
	return Delete(AccountID);
}

BOOL CAccountObjectMgr::SealAccount(UINT64 uAccountID, const std::string& strName, UINT32 dwChannel, BOOL bSeal, UINT32 dwSealTime)
{
	CAccountObject* pAccObj = NULL;
	if (uAccountID == 0)
	{
		pAccObj = GetAccountObject(strName, dwChannel);
	}
	else
	{
		pAccObj = GetAccountObjectByID(uAccountID);
	}

	if (pAccObj == NULL)
	{
		return FALSE;
	}

	if (bSeal)
	{
		pAccObj->m_uSealTime = CommonFunc::GetCurrTime() + dwSealTime;
	}
	else
	{
		pAccObj->m_uSealTime = 0;
	}

	m_ArrChangedAccount.push(pAccObj);

	return TRUE;
}

BOOL CAccountObjectMgr::SetLastServer(UINT64 uAccountID, INT32 ServerID)
{
	ERROR_RETURN_FALSE(uAccountID != 0);

	CAccountObject* pAccObj = GetAccountObjectByID(uAccountID);
	ERROR_RETURN_FALSE(pAccObj != NULL);

	pAccObj->m_dwLastSvrID = ServerID;

	m_ArrChangedAccount.push(pAccObj);

	return TRUE;
}

CAccountObject* CAccountObjectMgr::AddAccountObject(UINT64 u64ID, const CHAR* pStrName, UINT32 dwChannel)
{
	CAccountObject* pObj = InsertAlloc(u64ID);
	ERROR_RETURN_NULL(pObj != NULL);

	pObj->m_strName		= pStrName;
	pObj->m_ID			= u64ID;
	pObj->m_dwChannel	= dwChannel;

	if (m_bCrossChannel)
	{
		m_mapNameObj.insert(std::make_pair(pObj->m_strName, pObj));
	}
	else
	{
		m_mapNameObj.insert(std::make_pair(pObj->m_strName + CommonConvert::IntToString(dwChannel), pObj));
	}

	return pObj;
}

BOOL CAccountObjectMgr::SaveAccountChange()
{
	while(m_IsRun)
	{
		CAccountObject* pAccount = NULL;

		CHAR szSql[SQL_BUFF_LEN] = { 0 };

		if (m_ArrChangedAccount.size() <= 0)
		{
			CommonFunc::Sleep(100);
			continue;
		}

		while(m_ArrChangedAccount.pop(pAccount) && (pAccount != NULL))
		{
			snprintf(szSql, SQL_BUFF_LEN, "replace into account(id, name, password, lastsvrid, channel, create_time, seal_end_time) values('%lld','%s','%s','%d', '%d', '%lld','%lld')",
			         pAccount->m_ID, pAccount->m_strName.c_str(), pAccount->m_strPassword.c_str(), pAccount->m_dwLastSvrID, pAccount->m_dwChannel, pAccount->m_uCreateTime, pAccount->m_uSealTime);

			if(m_DBConnection.execSQL(szSql) > 0)
			{
				continue;
			}

			CLog::GetInstancePtr()->LogError("CAccountMsgHandler::SaveAccountChange Failed, DB Lose Connection!");

			int nTimes = 0;
			while (!m_DBConnection.reconnect())
			{
				nTimes++;
				if (nTimes > 3)
				{
					break;
				}
				CommonFunc::Sleep(1000);
			}

			if(m_DBConnection.execSQL(szSql) < 0)
			{
				CLog::GetInstancePtr()->LogError("CAccountMsgHandler::SaveAccountChange Failed, execSQL Error!");
			}
		}
	}

	return TRUE;
}

BOOL CAccountObjectMgr::Init()
{
	ERROR_RETURN_FALSE(LoadCacheAccount());

	m_IsRun = TRUE;

	m_pThread = new std::thread(&CAccountObjectMgr::SaveAccountChange, this);

	ERROR_RETURN_FALSE(m_pThread != NULL);

	return TRUE;
}

BOOL CAccountObjectMgr::Uninit()
{
	m_IsRun = FALSE;

	m_pThread->join();

	delete m_pThread;

	m_mapNameObj.clear();

	m_DBConnection.close();

	Clear();

	return TRUE;
}

CAccountObject* CAccountObjectMgr::GetAccountObject(const std::string& name, UINT32 dwChannel )
{
	if (m_bCrossChannel)
	{
		auto itor = m_mapNameObj.find(name);
		if (itor != m_mapNameObj.end())
		{
			return itor->second;
		}
	}
	else
	{
		auto itor = m_mapNameObj.find(name + CommonConvert::IntToString(dwChannel));
		if (itor != m_mapNameObj.end())
		{
			return itor->second;
		}
	}

	return NULL;
}
