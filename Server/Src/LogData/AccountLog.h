#ifndef __LOG_DATA_LOGIN_H__
#define __LOG_DATA_LOGIN_H__


struct Log_AccountCreate
{
	UINT64		m_uAccountID;	//账号ID
	UINT32      m_dwChannel;	//渠道
	UINT64 		m_OpTime;		//操作时间
	std::string m_strIdfa;		//IDFA
	std::string m_strAndroidID;	//AndroidID
	std::string m_strMacAddr;	//Mac Addr

	BOOL GetLogText(char* pBuff)
	{

		return TRUE;
	}
};

struct Log_AccountLogin
{
	UINT64		m_uAccountID;	//账号ID
	UINT32      m_dwChannel;	//渠道
	UINT64 		m_OpTime;		//操作时间
	std::string m_strIdfa;		//IDFA
	std::string m_strAndroidID;	//AndroidID
	std::string m_strMacAddr;	//Mac Addr

	BOOL GetLogText(char* pBuff)
	{

		return TRUE;
	}
};


#endif //__LOG_DATA_LOGIN_H__