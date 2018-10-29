#include "stdafx.h"
#include "Log.h"

CLog::CLog(void)
{
	m_LogLevel = 0;
}

CLog::~CLog(void)
{
	CloseLog();
}

CLog* CLog::GetInstancePtr()
{
	static CLog Log;

	return &Log;
}

BOOL CLog::StartLog(std::string strPrefix, std::string strLogDir)
{
	if(!CommonFunc::CreateDir(strLogDir))
	{
		return FALSE;
	}
	m_strPrefix = strPrefix;
#ifdef WIN32
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS);
#endif
	m_pLogFile = NULL;

	tm CurTime = CommonFunc::GetCurrTmTime();

	CHAR szFileName[512];

	snprintf(szFileName, 512, "%s/%s-%02d%02d%02d-%02d%02d%02d.log",  strLogDir.c_str(), strPrefix.c_str(), CurTime.tm_year, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec);

	m_pLogFile = fopen(szFileName, "w+");

	if(m_pLogFile == NULL)
	{
		return FALSE;
	}

	m_LogCount = 0;

	return TRUE;
}

BOOL CLog::CloseLog()
{
	if(m_pLogFile == NULL)
	{
		return FALSE;
	}

	fflush(m_pLogFile);

	fclose(m_pLogFile);

	m_pLogFile = NULL;

	return TRUE;
}

void CLog::LogWarnning( char* lpszFormat, ... )
{
	if(m_LogLevel >= Log_Info)
	{
		return ;
	}

	if(m_pLogFile == NULL)
	{
		return ;
	}

	tm CurTime = CommonFunc::GetCurrTmTime();

	CHAR szLog[512];
	snprintf(szLog, 512, "[%s][%02d-%02d-%02d %02d:%02d:%02d][%04x] ", m_strPrefix.c_str(), CurTime.tm_year % 100, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec, 0xffff & CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsnprintf(szLog + 28 + m_strPrefix.size(), 512 - 28 - m_strPrefix.size(),  lpszFormat, argList);
	va_end( argList );

	strncat(szLog, "\n", 10);

	m_CritSec.Lock();
	fputs(szLog, m_pLogFile);
	m_LogCount++;
	printf(szLog);
	m_CritSec.Unlock();

	return ;
}

void CLog::LogError( char* lpszFormat, ... )
{
	if(m_LogLevel >= Log_Error)
	{
		return ;
	}

	if(m_pLogFile == NULL)
	{
		return ;
	}

	CHAR szLog[512];
	tm CurTime = CommonFunc::GetCurrTmTime();
	snprintf(szLog, 512, "[%s][%02d-%02d-%02d %02d:%02d:%02d][%04x] ", m_strPrefix.c_str(), CurTime.tm_year % 100, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec, 0xffff & CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsnprintf(szLog + 28 + m_strPrefix.size(), 512 - 28 - m_strPrefix.size(), lpszFormat, argList);
	va_end( argList );

	strncat(szLog, "\n", 10);

	m_CritSec.Lock();
	fputs(szLog, m_pLogFile);
	m_LogCount++;
	printf(szLog);
	m_CritSec.Unlock();

	return ;
}

void CLog::LogInfo( char* lpszFormat, ... )
{
	if(m_LogLevel >= Log_Info)
	{
		return ;
	}

	if(m_pLogFile == NULL)
	{
		return ;
	}

	CHAR szLog[512];

	tm CurTime = CommonFunc::GetCurrTmTime();
	snprintf(szLog, 512, "[%s][%02d-%02d-%02d %02d:%02d:%02d][%04x] ", m_strPrefix.c_str(), CurTime.tm_year % 100, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec, 0xffff & CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsnprintf(szLog + 28 + m_strPrefix.size(), 512 - 28 - m_strPrefix.size(),  lpszFormat, argList);
	va_end( argList );

	strncat(szLog, "\n", 10);

	m_CritSec.Lock();
	fputs(szLog, m_pLogFile);
	m_LogCount++;
	printf(szLog);
	m_CritSec.Unlock();


	return ;
}

void CLog::SetLogLevel( int Level )
{
	m_LogLevel = Level;

	return ;
}

void CLog::SetTitle(char* lpszFormat, ...)
{
#ifdef WIN32
	CHAR szLog[512] = {0};
	snprintf(szLog, 512, "%s: ", m_strPrefix.c_str());

	UINT32 nSize = (UINT32)strlen(szLog);

	va_list argList;
	va_start(argList, lpszFormat);
	vsnprintf(szLog + nSize, 512 - nSize, lpszFormat, argList);
	va_end(argList);

	SetConsoleTitle(szLog);
#endif
	return;
}

void CLog::Flush()
{
	if (m_LogCount > 0)
	{
		fflush(m_pLogFile);

		m_LogCount = 0;
	}
}
