#include "stdafx.h"
#include "./Log.h"
#include "Utility/CommonFunc.h"

CLog::CLog(void)
{
}

CLog::~CLog(void)
{
	CloseLog();
}

void CLog::AddLog( char* lpszFormat,... )
{
	if(m_pLogFile == NULL)
	{
		return ;
	}
	
	CHAR szLog[512];
	memset(szLog, 0 , sizeof(0));

	time_t _time;
	time(&_time);
	tm *pTime = localtime(&_time);

	sprintf(szLog,"[%02d-%02d-%02d %02d:%02d:%02d][%04x] ", pTime->tm_year%100, pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec, 0xffff&CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsprintf(szLog+26, lpszFormat, argList);
	va_end( argList );

	strcat(szLog,"\n");
 

	CAutoLock Lock(&m_CritSec);
	fputs(szLog, m_pLogFile);
	printf(szLog);

	m_LogCount++;

	if(m_LogCount >= 10)
	{
		fflush(m_pLogFile);

		m_LogCount = 0;
	}

	
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

	time_t _time;
	time(&_time);

	tm *pTime = localtime(&_time);

	CHAR szFileName[256];

	sprintf(szFileName,"%s/%s-%02d%02d%02d-%02d%02d%02d.log",  strLogDir.c_str(),strPrefix.c_str(), pTime->tm_year, pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);

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

void CLog::LogWarnning( char* lpszFormat,... )
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

	time_t _time;
	time(&_time);
	tm *pTime = localtime(&_time);

	sprintf(szLog,"[%02d-%02d-%02d %02d:%02d:%02d][%04x] ", pTime->tm_year%100, pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec, 0xffff&CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsprintf(szLog+26, lpszFormat, argList);
	va_end( argList );

	strcat(szLog,"\n");


	CAutoLock Lock(&m_CritSec);
	fputs(szLog, m_pLogFile);
	printf(szLog);

	m_LogCount++;

	if(m_LogCount >= 10)
	{
		fflush(m_pLogFile);

		m_LogCount = 0;
	}

	return ;
}

void CLog::LogError( char* lpszFormat,... )
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

	time_t _time;
	time(&_time);
	tm *pTime = localtime(&_time);

	sprintf(szLog,"[%02d-%02d-%02d %02d:%02d:%02d][%04x] ", pTime->tm_year%100, pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec, 0xffff&CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsprintf(szLog+26, lpszFormat, argList);
	va_end( argList );

	strcat(szLog,"\n");


	CAutoLock Lock(&m_CritSec);
	fputs(szLog, m_pLogFile);
	printf(szLog);

	m_LogCount++;

	if(m_LogCount >= 10)
	{
		fflush(m_pLogFile);

		m_LogCount = 0;
	}

	return ;
}

void CLog::LogInfo( char* lpszFormat,... )
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

	time_t _time;
	time(&_time);
	tm *pTime = localtime(&_time);

	sprintf(szLog,"[%02d-%02d-%02d %02d:%02d:%02d][%04x] ", pTime->tm_year%100, pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec, 0xffff&CommonFunc::GetCurThreadID());

	va_list argList;
	va_start( argList, lpszFormat );
	vsprintf(szLog+26, lpszFormat, argList);
	va_end( argList );

	strcat(szLog,"\n");


	CAutoLock Lock(&m_CritSec);
	fputs(szLog, m_pLogFile);
	printf(szLog);

	m_LogCount++;

	if(m_LogCount >= 10)
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
