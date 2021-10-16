#ifndef _GLOBAL_LOG_H_
#define _GLOBAL_LOG_H_

enum LogLevel
{
    Log_HiInfo,
    Log_Error,
    Log_Warn,
    Log_Info,
    Log_None
};


class CLog
{
private:
    CLog(void);
    ~CLog(void);

public:
    static CLog* GetInstancePtr();

    BOOL Start(std::string strPrefix, std::string strLogDir = "log");

    BOOL Close();

    void LogHiInfo(char* lpszFormat, ...);

    void LogWarn(char* lpszFormat, ...);

    void LogError(char* lpszFormat, ...);

    void LogInfo(char* lpszFormat, ...);

    void SetLogLevel(int Level);

    void SetTitle(char* lpszFormat, ...);

    void CheckAndCreate();

protected:
    std::mutex          m_WriteMutex;

    INT32               m_LogCount;

    FILE*               m_pLogFile;

    INT32               m_LogLevel;

    std::string         m_strPrefix;
    std::string         m_strLogDir;
};

#define LOG_FUNCTION_LINE  CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);

#define ERROR_RETURN_TRUE(P) \
    if((P) == FALSE)\
    {\
        CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
        return TRUE;    \
    }


#define ERROR_RETURN_FALSE(P) \
    if((P) == FALSE)\
    {\
        CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
        return FALSE;   \
    }

#define ERROR_RETURN_NULL(P) \
    if((P) == FALSE)\
{\
    CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
    return NULL;    \
}

#define ERROR_RETURN_NONE(P) \
    if((P) == FALSE)\
{\
    CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
    return ;    \
}

#define ERROR_RETURN_VALUE(P, V) \
    if((P) == FALSE)\
{\
    CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
    return V;   \
}

#define ERROR_CONTINUE_EX(P) \
    if((P) == FALSE)\
{\
    CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
    continue; \
}

#define ERROR_TO_CONTINUE(P) \
    if((P) == FALSE)\
{\
    CLog::GetInstancePtr()->LogError("Error : File:%s, Func: %s Line:%d", __FILE__ , __FUNCTION__, __LINE__);\
    continue; \
}

#define PARSE_FROM_PACKET(TYPE, VALUE)   TYPE VALUE; \
ERROR_RETURN_TRUE(VALUE.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth()));

#endif