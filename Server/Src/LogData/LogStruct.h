#ifndef __LOG_STRUCT_H__
#define __LOG_STRUCT_H__


enum ELogType
{
	ELT_LOG_TYPE_NONE = 0,
	ELT_ACCOUNT_CREATE = 1,
	ELT_ACCOUNT_LOGIN,
	ELT_ROLE_CREATE,
	ELT_ROLE_LOGIN,
	ELT_ROLE_LOGOUT, //离线
	ELT_ROLE_EXP,
	ELT_ROLE_DIAMOND,
	ELT_ROLE_GOLD,
	ELT_ROLE_LEVEL,
	ELT_ROLE_CHAT, //聊天
};

//角色日志
struct Log_BaseData
{
	ELogType    m_LogType   = ELT_LOG_TYPE_NONE; //日志类型
	UINT64		m_uID       = 0;	//角色ID或账号ID
	UINT64 		m_uOpTime   = 0;  //日志发生时间
	UINT32      m_dwChannel = 0;//渠道
	UINT32      m_dwAreaID  = 0; //区服ID

	//以下两条仅角色日志有效
	UINT32      m_nLeve     = 0;    //角色等级
	UINT32      m_nVipLevel = 0;//角色VIP等级
	UINT32      m_nAddWay   = 0;  //
};

struct Log_AccountCreate : public Log_BaseData
{
	UINT32      m_dwVersion     = 0;   //客户端版本号
	UINT32      m_dwIpAddr      = 0;    //登录IP
	CHAR        m_szIdfa[64]    = { 0 };  //客户端idfa
	CHAR        m_szImei[64]    = { 0 };  //手机的MEI
	CHAR        m_szModel[64]   = { 0 }; //手机的机型
	CHAR        m_szUuid[64]    = { 0 };  //手机的uuid;
	CHAR        m_szOpenID[64]  = { 0 };//第三方平台ID

	Log_AccountCreate()
	{
		m_LogType = ELT_ACCOUNT_CREATE;
	}

	BOOL GetLogSql(char* pBuff)
	{
		snprintf(pBuff, 2048, "insert into account_create(accountid, channel, version, optime, ip, uuid, idfa, imei, imodel, openid) values(%lld, %u, %d, '%s', '%s','%s','%s','%s', '%s', '%s')",
		         m_uID, m_dwChannel, m_dwVersion, CommonFunc::TimeToString(m_uOpTime).c_str(), CommonSocket::IpAddrIntToStr(m_dwIpAddr).c_str(), m_szUuid, m_szIdfa, m_szImei, m_szModel, m_szOpenID);
		return TRUE;
	}
};

struct Log_AccountLogin : public Log_BaseData
{
	UINT32      m_dwVersion = 0;  //客户端版本号
	UINT32      m_dwIpAddr = 0;   //登录IP
	CHAR        m_szIdfa[64] = { 0 }; //客户端idfa
	CHAR        m_szImei[64] = { 0 }; //手机的MEI
	CHAR        m_szModel[64] = { 0 };//手机的机型
	CHAR        m_szUuid[64] = { 0 }; //手机的uuid;
	CHAR        m_szOpenID[64] = { 0 };//第三方平台ID
	Log_AccountLogin()
	{
		m_LogType = ELT_ACCOUNT_LOGIN;
	}

	BOOL GetLogSql(char* pBuff)
	{

		snprintf(pBuff, 2048, "insert into account_login(accountid, channel, version, optime, ip, uuid, idfa, imei, imodel, openid) values(%lld, %u, %d, '%s', '%s','%s', '%s','%s', '%s', '%s')",
		         m_uID, m_dwChannel, m_dwVersion, CommonFunc::TimeToString(m_uOpTime).c_str(), CommonSocket::IpAddrIntToStr(m_dwIpAddr).c_str(), m_szUuid, m_szIdfa, m_szImei, m_szModel, m_szOpenID);
		return TRUE;
	}
};

struct Log_RoleCreate : public Log_BaseData
{
	UINT64 m_uAccountID = 0;
	CHAR   m_szRoleName[64] = { 0 };
	CHAR   m_szIdfa[64] = { 0 }; //客户端idfa

	Log_RoleCreate()
	{
		m_LogType = ELT_ROLE_CREATE;
	}

	BOOL GetLogSql(char* pBuff)
	{
		snprintf(pBuff, 2048, "insert into role_create(roleid, accountid, areaid, channel, optime, rolename, idfa) values(%lld, %lld, %d, %d, '%s','%s', '%s')",
		         m_uID, m_uAccountID, m_dwAreaID, m_dwChannel, CommonFunc::TimeToString(m_uOpTime).c_str(), m_szRoleName, m_szIdfa);
		return TRUE;
	}
};

struct Log_RoleLogin : public Log_BaseData
{
	UINT64 m_uAccountID = 0;
	CHAR   m_szRoleName[64] = { 0 };
	CHAR   m_szIdfa[64] = { 0 }; //客户端idfa

	Log_RoleLogin()
	{
		m_LogType = ELT_ROLE_LOGIN;
	}

	BOOL GetLogSql(char* pBuff)
	{
		snprintf(pBuff, 2048, "insert into role_login(roleid, accountid, areaid, channel, level, viplevel, optime, rolename, idfa) values(%lld, %lld, %d, %d, %d, %d, '%s', '%s', '%s')",
		         m_uID, m_uAccountID, m_dwAreaID, m_dwChannel, m_nLeve, m_nVipLevel, CommonFunc::TimeToString(m_uOpTime).c_str(), m_szRoleName, m_szIdfa);
		return TRUE;
	}
};

struct Log_RoleLogout : public Log_BaseData
{
	UINT64 m_uAccountID = 0;
	CHAR   m_szRoleName[64] = { 0 };
	CHAR   m_szIdfa[64] = { 0 }; //客户端idfa

	Log_RoleLogout()
	{
		m_LogType = ELT_ROLE_LOGOUT;
	}

	BOOL GetLogSql(char* pBuff)
	{
		snprintf(pBuff, 2048, "insert into role_logout(roleid, accountid, areaid, channel, level, viplevel, optime, rolename, idfa) values(%lld, %lld, %d, %d, %d, %d, '%s', '%s', '%s')",
		         m_uID, m_uAccountID, m_dwAreaID, m_dwChannel, m_nLeve, m_nVipLevel, CommonFunc::TimeToString(m_uOpTime).c_str(), m_szRoleName, m_szIdfa);
		return TRUE;
	}
};

//经验获取
struct Log_RoleExp : public Log_BaseData
{
	UINT64 m_uPreValue;
	UINT64 m_uAfterValue;

	Log_RoleExp()
	{
		m_LogType = ELT_ROLE_EXP;
	}

	BOOL GetLogSql(char* pBuff)
	{
		return TRUE;
	}
};

//钻石获取(充值币)
struct Log_RoleDiamond : public Log_BaseData
{
	UINT64 m_uPreValue;
	UINT64 m_uAfterValue;

	Log_RoleDiamond()
	{
		m_LogType = ELT_ROLE_DIAMOND;
	}

	BOOL GetLogSql(char* pBuff)
	{
		return TRUE;
	}
};

//金币获取(游戏币)
struct Log_RoleGold : public Log_BaseData
{
	UINT64 m_uPreValue;
	UINT64 m_uAfterValue;

	Log_RoleGold()
	{
		m_LogType = ELT_ROLE_GOLD;
	}

	BOOL GetLogSql(char* pBuff)
	{
		return TRUE;
	}
};

//聊天日志
struct Log_RoleChat : public Log_BaseData
{
	CHAR   m_szText[256] = { 0 };
	CHAR   m_szSrcName[64] = { 0 };
	CHAR   m_szTargetName[64] = { 0 };
	UINT64 m_uTargetID = 0;
	UINT32 m_nTargetVip = 0;
	INT32  m_nChatChl = 0;

	Log_RoleChat()
	{
		m_LogType = ELT_ROLE_CHAT;
	}

	BOOL GetLogSql(char* pBuff)
	{
		//CHAR szTemp[512] = { 0 };
		//mysql_escape_string(szTemp, m_szText, strlen(m_szText));

		snprintf(pBuff, 2048, "insert into role_chat(roleid,rolename, areaid, channel, optime, level, viplevel, chatchl, text, targetid, targetvip, targetname) values(%lld, '%s', %d ,%d, '%s', %d, %d,%d, '%s', %lld, %ld, '%s')",
		         m_uID, m_szSrcName, m_dwAreaID, m_dwChannel, CommonFunc::TimeToString(m_uOpTime).c_str(), m_nLeve, m_nVipLevel, m_nChatChl, m_szText, m_uTargetID, m_nTargetVip, m_szTargetName);
		return TRUE;
	}
};
//金币获取(游戏币)
struct Log_RoleLevel : public Log_BaseData
{
	UINT64 m_uPreValue;
	UINT64 m_uAfterValue;

	Log_RoleLevel()
	{
		m_LogType = ELT_ROLE_LEVEL;
	}

	BOOL GetLogSql(char* pBuff)
	{
		return TRUE;
	}
};

#endif //__LOG_STRUCT_H__