#ifndef __LOG_DATA_LOGIN_H__
#define __LOG_DATA_LOGIN_H__

struct Log_AccountLogin
{
	UINT64		m_uAccountID;	//账号ID
	UINT32      m_dwChannel;
	UINT64 		m_OpTime;      //职业ID

	BOOL GetLogSql(char* pBuff)
	{

		return TRUE;
	}
};


#endif //__LOG_DATA_LOGIN_H__