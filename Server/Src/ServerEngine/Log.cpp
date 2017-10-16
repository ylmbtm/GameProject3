#include "stdafx.h"
#include "./Log.h"
#include "CommonFunc.h"

CLog::CLog(void)
{
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
#ifdef WIN32
	SetConsoleTitle(strPrefix.c_str());
#endif

	if(!CommonFunc::CreateDir(strLogDir))
	{
		return FALSE;
	}

	m_pLogFile = NULL;

	tm CurTime = CommonFunc::GetCurrTmTime();

	CHAR szFileName[512];

	sprintf(szFileName, "%s/%s-%02d%02d%02d-%02d%02d%02d.log",  strLogDir.c_str(), strPrefix.c_str(), CurTime.tm_year, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec);

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
	if(m_LogLevel > Log_Info)
	{
		return ;
	}

	if(m_pLogFile == NULL)
	{
		return ;
	}

	tm CurTime = CommonFunc::GetCurrTmTime();

	CHAR szLog[512];
	sprintf(szLog, "[%02d-%02d-%02d %02d:%02d:%02d][%04x] ", CurTime.tm_year % 100, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec, 0xffff & CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsnprintf(szLog + 26, 512 - 26,  lpszFormat, argList);
	va_end( argList );

	strncat(szLog, "\n", 10);

	m_CritSec.Lock();
	fputs(szLog, m_pLogFile);
	m_LogCount++;
	m_CritSec.Unlock();
	printf(szLog);



	if(m_LogCount >= 3)
	{
		fflush(m_pLogFile);

		m_LogCount = 0;
	}

	return ;
}

void CLog::LogError( char* lpszFormat, ... )
{
	if(m_LogLevel > Log_Error)
	{
		return ;
	}

	if(m_pLogFile == NULL)
	{
		return ;
	}

	CHAR szLog[512];
	tm CurTime = CommonFunc::GetCurrTmTime();
	sprintf(szLog, "[%02d-%02d-%02d %02d:%02d:%02d][%04x] ", CurTime.tm_year % 100, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec, 0xffff & CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsnprintf(szLog + 26, 512 - 26, lpszFormat, argList);
	va_end( argList );

	strncat(szLog, "\n", 10);

	m_CritSec.Lock();
	fputs(szLog, m_pLogFile);
	m_LogCount++;
	m_CritSec.Unlock();
	printf(szLog);



	if(m_LogCount >= 3)
	{
		fflush(m_pLogFile);

		m_LogCount = 0;
	}

	return ;
}

void CLog::LogInfo( char* lpszFormat, ... )
{
	if(m_LogLevel > Log_Info)
	{
		return ;
	}

	if(m_pLogFile == NULL)
	{
		return ;
	}

	CHAR szLog[512];

	tm CurTime = CommonFunc::GetCurrTmTime();
	sprintf(szLog, "[%02d-%02d-%02d %02d:%02d:%02d][%04x] ", CurTime.tm_year % 100, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec, 0xffff & CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsnprintf(szLog + 26, 512 - 26,  lpszFormat, argList);
	va_end( argList );

	strncat(szLog, "\n", 10);

	m_CritSec.Lock();
	fputs(szLog, m_pLogFile);
	m_LogCount++;
	m_CritSec.Unlock();
	printf(szLog);



	if(m_LogCount >= 3)
	{
		fflush(m_pLogFile);

		m_LogCount = 0;
	}

	return ;
}

void CLog::SetLogLevel( int Level )
{
	m_LogLevel = Level;

	return ;
}

void CLog::Flush()
{
	fflush(m_pLogFile);

	m_LogCount = 0;
}
