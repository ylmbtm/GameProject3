#include "stdafx.h"
#include "CommonFunc.h"


UINT32 CommonFunc::GetProcessorNum()
{
	UINT32 dwNum = 0;
#ifdef WIN32
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	dwNum = sysInfo.dwNumberOfProcessors*2;
#else
	dwNum = sysconf(_SC_NPROCESSORS_CONF);
#endif

	return dwNum;
}

std::string CommonFunc::GetCurrentDir()
{
	char szPath[MAX_PATH_LEN];

#ifdef WIN32
	_getcwd(szPath, MAX_PATH_LEN);
#else
	getcwd(szPath, MAX_PATH_LEN);
#endif
	return std::string(szPath);
}

std::string CommonFunc::GetCurrentExeDir()
{
	char szPath[MAX_PATH_LEN];

#ifdef WIN32
	ZeroMemory(szPath, MAX_PATH_LEN);
	GetModuleFileName(NULL, szPath, MAX_PATH_LEN);
	char *p = strrchr(szPath, '\\');
	*p = 0;
#else
	
#endif
	return std::string(szPath);
}


BOOL CommonFunc::SetCurrentWorkPath(std::string strPath)
{
	if (strPath.empty())
	{
		strPath = GetCurrentExeDir();
	}

#ifdef WIN32
	SetCurrentDirectory(strPath.c_str());
#else

#endif

	return TRUE;
}

UINT32 CommonFunc::GetCurrTime()
{
	time_t t;    

	t=time(0);

	return (UINT32)t;
}

UINT32 CommonFunc::GetDayTime()
{
	time_t t;    
	t=time(0);
	tm* t_tm = localtime(&t);
	t_tm->tm_hour = 0;
	t_tm->tm_min = 0;
	t_tm->tm_sec = 0;
	t = mktime(t_tm);
	return (UINT32)t;
}

UINT32 CommonFunc::GetCurrDate()
{
	time_t t = time(0);

	struct tm *pTm = localtime(&t);

	pTm->tm_year += 1900;

	UINT32 dwCurDate = (pTm->tm_year<<16)|(pTm->tm_mon<<8)|(pTm->tm_mday);

	return dwCurDate;
}



UINT32 CommonFunc::GetTickCount()
{
#ifdef WIN32
	return ::GetTickCount();
#else

	UINT32 dwTickCount = 0;;
	struct timespec on;
	if(0 == clock_gettime(CLOCK_MONOTONIC,&on) )
	{
		dwTickCount = on.tv_sec*1000+ on.tv_nsec/1000000;
	}

	return dwTickCount;

#endif
}

UINT64 CommonFunc::GetTickCount64()
{
#if WINVER < 0x0501
	return GetTickCount();
#else
#ifdef WIN32
		return ::GetTickCount64();
#else
	UINT64 dwTickCount = 0;;
	struct timespec on;
	if(0 == clock_gettime(CLOCK_MONOTONIC,&on) )
	{
		dwTickCount = on.tv_sec*1000+ on.tv_nsec/1000000;
	}

	return dwTickCount;
#endif
#endif
}

BOOL CommonFunc::CreateDir( std::string &strDir )
{
	int nRet = 0;
#ifdef WIN32
	nRet = _mkdir(strDir.c_str());
#else
	nRet = mkdir(strDir.c_str(), S_IRWXU);
#endif

	if(nRet == 0)
	{
		return TRUE;
	}

	if(errno == EEXIST)
	{
		return TRUE;
	}

	return FALSE;
}

UINT32 CommonFunc::GetCurThreadID()
{
	UINT32 dwThreadID = 0;
#ifdef WIN32
	dwThreadID = ::GetCurrentThreadId();
#else
	dwThreadID = (UINT32)pthread_self();
#endif
	return dwThreadID;
}

UINT32 CommonFunc::GetFreePhysMemory()
{
	UINT32 dwFreeSize = 0;
#ifdef WIN32
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof (statex);

	GlobalMemoryStatusEx (&statex);

	dwFreeSize = (UINT32)(statex.ullAvailPhys/1024/1024);
#else
	UINT32 dwPageSize;
	UINT32 dwFreePages;
	dwPageSize = sysconf (_SC_PAGESIZE)/1024;
	dwFreePages = sysconf (_SC_AVPHYS_PAGES)/1024;
	dwFreeSize = dwFreePages * dwPageSize;
#endif

	return dwFreeSize;
}


INT32 CommonFunc::FloatToInt(FLOAT value)
{
	return 0;
}

UINT32 CommonFunc::GetRandNum(INT32 nType)
{
	if(nType >= 100||nType <0)
	{
		return 0;
	}

	static int nRandIndex[100] = {0};
	static UINT32 vtGlobalRankValue[10000];
	static bool bInit = false;

	if(bInit == false)
	{
		bInit = true;
		int nTempIndex;
		UINT32 nTemp;
		for(int j = 0; j < 10000; j++ )
		{
			vtGlobalRankValue[j] = j + 1;
		}

		for(int i = 0; i < 10000; i++ )
		{
			nTempIndex = rand() % (i + 1);
			if (nTempIndex != i)
			{
				nTemp = vtGlobalRankValue[i];
				vtGlobalRankValue[i] = vtGlobalRankValue[nTempIndex];
				vtGlobalRankValue[nTempIndex] = nTemp;
			}
		}
	}

	return  vtGlobalRankValue[nRandIndex[nType]++];
}
