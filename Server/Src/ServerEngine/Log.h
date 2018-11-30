#ifndef _GLOBAL_LOG_H_
#define _GLOBAL_LOG_H_

#include "CritSec.h"

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

	void LogWarnning(char* lpszFormat, ...);

	void LogError(char* lpszFormat, ...);

	void LogInfo(char* lpszFormat, ...);

	void SetLogLevel(int Level);

	void SetTitle(char* lpszFormat, ...);

	void Flush();

protected:
	CCritSec			m_CritSec;

	INT32				m_LogCount;

	FILE*				m_pLogFile;

	INT32				m_LogLevel;

	std::string         m_strPrefix;
};

#define LOG_ERROR  CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);

#define ERROR_RETURN_TRUE(P) \
    if(P == FALSE)\
    {\
        CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
        ASSERT_FAIELD; \
        return TRUE;	\
    }


#define ERROR_RETURN_FALSE(P) \
    if(P == FALSE)\
    {\
        CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
        ASSERT_FAIELD; \
        return TRUE;	\
    }

#define ERROR_RETURN_NULL(P) \
	if(P == FALSE)\
{\
	CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
	ASSERT_FAIELD; \
	return NULL;	\
}

#define ERROR_RETURN_NONE(P) \
	if(P == FALSE)\
{\
	CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
	ASSERT_FAIELD; \
	return ;	\
}



#define ERROR_RETURN_CODE(P, Code) \
	if(P == FALSE)\
{\
	CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
	ASSERT_FAIELD; \
	return Code;	\
}


#define ERROR_CONTINUE_EX(P) \
    if(P == FALSE)\
{\
	CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
    continue; \
}


#endif