#ifndef __LOG_STRUCT_H__
#define __LOG_STRUCT_H__


enum ELogType
{
	ELT_ACCOUNT_CREATE = 1,
	ELT_ACCOUNT_LOGIN,
	ELT_ROLE_CREATE,
	ELT_ROLE_LOGIN,
	ELT_ROLE_EXP
};

//角色日志
struct Log_BaseData
{
	ELogType    m_LogType;  //日志类型
	UINT64		m_uID;	    //角色ID或账号ID
	UINT64 		m_uOpTime;  //日志发生时间
	UINT32      m_dwChannel;//渠道

	//以下两条仅角色日志有效
	UINT32      m_nLeve;    //角色等级
	UINT32      m_nVipLevel;//角色VIP等级
};

struct Log_AccountCreate : public Log_BaseData
{
	UINT32      m_dwVersion;   //客户端版本号
	UINT32      m_dwIpAddr;    //登录IP
	CHAR        m_szIdfa[64];  //客户端idfa
	CHAR        m_szImei[64];  //手机的MEI
	CHAR        m_szModel[20]; //手机的机型
	CHAR        m_szUuid[50];  //手机的uuid;
	CHAR        m_szOpenID[32];//第三方平台ID

	Log_AccountCreate()
	{
		m_LogType = ELT_ACCOUNT_CREATE;
	}

	BOOL GetLogSql(char* pBuff)
	{
		snprintf(pBuff, 2048, "insert into account_create(accountid, channel, version, optime, ip, uuid, idfa, imei, imodel) values(%lld, %u, %d, %lld, %u,'%s', '%s','%s', '%s')",
		         m_uID, m_dwChannel, m_dwVersion, m_uOpTime, m_dwIpAddr, m_szUuid, m_szIdfa, m_szImei, m_szModel);
		return TRUE;
	}
};

struct Log_AccountLogin : public Log_BaseData
{
	UINT32      m_dwVersion;  //客户端版本号
	UINT32      m_dwIpAddr;   //登录IP
	CHAR        m_szIdfa[64]; //客户端idfa
	CHAR        m_szImei[64]; //手机的MEI
	CHAR        m_szModel[20];//手机的机型
	CHAR        m_szUuid[50]; //手机的uuid;

	Log_AccountLogin()
	{
		m_LogType = ELT_ACCOUNT_LOGIN;
	}

	BOOL GetLogSql(char* pBuff)
	{
		snprintf(pBuff, 2048, "insert into account_login(accountid, channel, version, optime, ip, uuid, idfa, imei, imodel) values(%lld, %u, %d, %lld, %u,'%s', '%s','%s', '%s')",
		         m_uID, m_dwChannel, m_dwVersion, m_uOpTime, m_dwIpAddr, m_szUuid, m_szIdfa, m_szImei, m_szModel);
		return TRUE;
	}
};

struct Log_RoleCreate : public Log_BaseData
{
	UINT64 m_uAccountID;
	UINT32 m_dwAreaID;
	CHAR   m_szRoleName[64];
	CHAR   m_szIdfa[64]; //客户端idfa

	Log_RoleCreate()
	{
		m_LogType = ELT_ROLE_CREATE;
	}

	BOOL GetLogSql(char* pBuff)
	{
		snprintf(pBuff, 2048, "insert into role_create(roleid, accountid, areaid, channel, optime, rolename, idfa) values(%lld, %lld,%d, %d, %lld,'%s', '%s')",
		         m_uID, m_uAccountID, m_dwAreaID, m_dwChannel, m_uOpTime, m_szRoleName, m_szIdfa);
		return TRUE;
	}
};

struct Log_RoleLogin : public Log_BaseData
{
	UINT64 m_uAccountID;
	UINT32 m_dwAreaID;
	CHAR   m_szRoleName[64];
	CHAR   m_szIdfa[64]; //客户端idfa

	Log_RoleLogin()
	{
		m_LogType = ELT_ROLE_LOGIN;
	}

	BOOL GetLogSql(char* pBuff)
	{
		snprintf(pBuff, 2048, "insert into role_login(roleid, accountid, areaid, channel, optime, rolename, idfa) values(%lld, %lld, %d, %d, %lld, '%s', '%s')",
		         m_uID, m_uAccountID, m_dwAreaID, m_dwChannel, m_uOpTime, m_szRoleName, m_szIdfa);
		return TRUE;
	}
};

struct Log_RoleExp : public Log_BaseData
{
	Log_RoleExp()
	{
		m_LogType = ELT_ROLE_EXP;
	}

	BOOL GetLogSql(char* pBuff)
	{
		return TRUE;
	}
};

#endif //__LOG_STRUCT_H__