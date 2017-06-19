#ifndef _GLOBAL_LOG_H_
#define _GLOBAL_LOG_H_

#include "Utility/CritSec.h"

enum LogLevel
{
	Log_All,
	Log_Info,
	Log_Warnning,
	Log_Error,
	Log_None
};


class CLog
{
private:
	CLog(void);
	~CLog(void);

public:
	static CLog* GetInstancePtr();

	BOOL StartLog(std::string strPrefix, std::string strLogDir = "log");

	BOOL CloseLog();

	void AddLog(char* lpszFormat,...);

	void LogWarnning(char* lpszFormat,...);

	void LogError(char* lpszFormat,...);

	void LogInfo(char* lpszFormat,...);

	void SetLogLevel(int Level);

protected:
	CCritSec			m_CritSec;

	INT32				m_LogCount;

	FILE *m_pLogFile;

	INT32				m_LogLevel;
};


#endif