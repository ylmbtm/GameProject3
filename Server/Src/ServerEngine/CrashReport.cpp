#include "stdafx.h"

#ifdef WIN32
#pragma warning(disable: 4996)
#include <windows.h>

#include <Dbghelp.h>

#include "crashReport.h"
#include "time.h"

#pragma auto_inline (off)
#pragma comment( lib, "DbgHelp" )


void NewInvalidParamHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
    throw 1;
}

void NewPureCallHandler(void)
{
    throw 1;
}

LPTOP_LEVEL_EXCEPTION_FILTER g_preFilter;

std::string g_AppName;


_invalid_parameter_handler g_preInvalidParamHandler;

_purecall_handler g_prePureHandler;


long   __stdcall  CrashCallBack(_EXCEPTION_POINTERS* pExInfo)
{
    struct tm* pTime;
    time_t ctTime;
    time(&ctTime);
    pTime = localtime( &ctTime );
    TCHAR tem[256];
    memset(tem, 0, 256);
    snprintf(tem, 256, ("%s-%d-%d-%d_%d-%d-%d.dmp"), g_AppName.c_str(),
             1900 + pTime->tm_year, 1 + pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);

    HANDLE hFile = ::CreateFile( tem, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if( hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION einfo;
        einfo.ThreadId = ::GetCurrentThreadId();
        einfo.ExceptionPointers = pExInfo;
        einfo.ClientPointers = FALSE;
        ::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, &einfo, NULL, NULL);
        ::CloseHandle(hFile);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

void  SetCrashReport(std::string strAppName)
{
    g_AppName = strAppName;

    g_preFilter = SetUnhandledExceptionFilter(CrashCallBack);

    g_preInvalidParamHandler = _set_invalid_parameter_handler(NewInvalidParamHandler);

    g_prePureHandler = _set_purecall_handler(NewPureCallHandler);
}

void UnSetCrashReport()
{
    SetUnhandledExceptionFilter(g_preFilter);

    return;
}


#else
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

std::string g_AppName;

void exceptionalStack(int signal)
{
    time_t nowtime;
    time(&nowtime);
    pid_t pid = getpid();
    char cmd[256];
    snprintf(cmd, 256, "gstack %d > core_%s_log_%d.core", pid, g_AppName.c_str(), nowtime);
    system(cmd);
    exit(-1);
}

void  SetCrashReport(std::string strAppName)
{
    return;
    g_AppName = strAppName;
    /*捕获异常信息 start*/
    signal(SIGABRT, &exceptionalStack); //异常终止(abort)
    signal(SIGBUS, &exceptionalStack); //硬件故障
    signal(SIGFPE, &exceptionalStack); //算术异常
    signal(SIGILL, &exceptionalStack); //非法硬件指令
    signal(SIGIOT, &exceptionalStack); //硬件故障
    signal(SIGQUIT, &exceptionalStack); //终端退出符
    signal(SIGSEGV, &exceptionalStack); //无效存储访问
    signal(SIGSYS, &exceptionalStack); //无效系统调用
    signal(SIGTRAP, &exceptionalStack); //硬件故障
    signal(SIGXCPU, &exceptionalStack); //超过CPU限制(setrlimit)
    signal(SIGXFSZ, &exceptionalStack); //超过文件长度限制(setrlimit)
    /*捕获异常信息 end*/

    setvbuf(stdout, NULL, _IONBF, 0);
    return ;
}

void UnSetCrashReport()
{
    return;
}

#endif