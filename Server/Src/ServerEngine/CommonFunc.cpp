#include "stdafx.h"
#include "CommonFunc.h"

UINT32 CommonFunc::GetProcessorNum()
{
	UINT32 dwNum = 0;
#ifdef WIN32
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	dwNum = sysInfo.dwNumberOfProcessors;
#else
	dwNum = sysconf(_SC_NPROCESSORS_CONF);
#endif

	return dwNum;
}

std::string CommonFunc::GetCurrentWorkDir()
{
	char szPath[1024];

#ifdef WIN32
	_getcwd(szPath, 1024);
#else
	getcwd(szPath, 1024);
#endif
	return std::string(szPath);
}

std::string CommonFunc::GetCurrentExeDir()
{
	char szPath[1024] = {0};
#ifdef WIN32
	GetModuleFileName(NULL, szPath, 1024);
	char* p = strrchr(szPath, '\\');
#else
	readlink("/proc/self/exe", szPath, sizeof(szPath));
	char* p = strrchr(szPath, '/');
#endif
	*p = 0;
	return std::string(szPath);
}


BOOL CommonFunc::SetCurrentWorkDir(std::string strPath)
{
	if (strPath.empty())
	{
		strPath = GetCurrentExeDir();
	}

#ifdef WIN32
	SetCurrentDirectory(strPath.c_str());
#else
	chdir(strPath.c_str());
#endif
	return TRUE;
}

UINT64 CommonFunc::GetCurrTime()
{
	time_t t;

	t = time(0);

	return (UINT64)t;
}

tm CommonFunc::GetCurrTmTime()
{
	time_t rawtime;
	struct tm* timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	return *timeinfo;
}

UINT64 CommonFunc::GetDayBeginTime()
{
	time_t t;
	t = time(0);
	tm* t_tm = localtime(&t);
	t_tm->tm_hour = 0;
	t_tm->tm_min = 0;
	t_tm->tm_sec = 0;
	t = mktime(t_tm);
	return (UINT64)t;
}

UINT64 CommonFunc::GetWeekBeginTime()
{
	time_t t;
	t = time(0);
	tm* t_tm = localtime(&t);
	t_tm->tm_hour = 0;
	t_tm->tm_min = 0;
	t_tm->tm_sec = 0;
	t_tm->tm_wday = 0;
	t = mktime(t_tm);
	return (UINT64)t;
}

time_t CommonFunc::YearTimeToSec(INT32 nYear, INT32 nMonth, INT32 nDay, INT32 nHour, INT32 nMin, INT32 nSec)
{
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);

	tm newtm;
	newtm.tm_year = (nYear < 0) ? t_tm->tm_year : nYear;
	newtm.tm_mon = (nMonth < 0) ? t_tm->tm_mon : nMonth;
	newtm.tm_mday = (nDay < 0) ? t_tm->tm_mday : nDay;
	newtm.tm_hour = (nHour < 0) ? t_tm->tm_hour : nHour;
	newtm.tm_min = (nMin < 0) ? t_tm->tm_min : nMin;
	newtm.tm_sec = (nSec < 0) ? t_tm->tm_sec : nSec;
	return mktime(&newtm);;
}

std::string CommonFunc::TimeToString(time_t tTime)
{
	tm* t_tm = localtime(&tTime);
	if (t_tm == NULL)
	{
		return "";
	}

	char szTime[128] = { 0 };
	strftime(szTime, 128, "%Y-%m-%d %H:%M:%S", t_tm);
	return std::string(szTime);
}

UINT64 CommonFunc::GetTickCount()
{
#ifdef WIN32
	return ::GetTickCount64();
#else
	UINT64 uTickCount = 0;;
	struct timespec on;
	if(0 == clock_gettime(CLOCK_MONOTONIC, &on) )
	{
		uTickCount = on.tv_sec * 1000 + on.tv_nsec / 1000000;
	}

	return uTickCount;
#endif
}

BOOL CommonFunc::CreateDir( std::string& strDir )
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

BOOL CommonFunc::GetDirFiles(const char* pszDir, char* pszFileType, std::vector<std::string>& vtFileList, BOOL bRecursion)
{
	if (pszDir == NULL || pszFileType == NULL)
	{
		return FALSE;
	}

	char   szTem[1024] = { 0 };
	char   szDir[1024] = { 0 };
	strcpy(szTem, pszDir);
	if (szTem[strlen(szTem) - 1] != '\\' || szTem[strlen(szTem) - 1] != '/')
	{
		strcat(szTem, "/");
	}

	strcpy(szDir, szTem);
	strcat(szDir, pszFileType);

#ifdef WIN32
	struct _finddata_t  tFileInfo = { 0 };
	long long hFile = _findfirst(szDir, &tFileInfo);
	if (hFile == -1)
	{
		return FALSE;
	}

	do
	{
		if (strcmp(tFileInfo.name, ".") == 0 || strcmp(tFileInfo.name, "..") == 0)
		{
			continue;
		}

		if ((tFileInfo.attrib   &  _A_SUBDIR) && bRecursion)
		{
			char   szSub[1024] = { 0 };
			strcpy(szSub, pszDir);
			if (szSub[strlen(szSub) - 1] != '\\' || szSub[strlen(szSub) - 1] != '/')
			{
				strcat(szSub, "/");
			}
			strcat(szSub, tFileInfo.name);
			GetDirFiles(szSub, pszFileType, vtFileList, bRecursion);
		}
		else
		{
			vtFileList.push_back(std::string(szTem) + std::string(tFileInfo.name));
		}
	}
	while (_findnext(hFile, &tFileInfo) == 0);
	_findclose(hFile);

#else

	DIR* pDirInfo;
	struct dirent* tFileInfo;
	struct stat statbuf;
	if((pDirInfo = opendir(pszDir)) == NULL)
	{
		return FALSE;
	}

	while((tFileInfo = readdir(pDirInfo)) != NULL)
	{
		if (strcmp(".", tFileInfo->d_name) == 0 || strcmp("..", tFileInfo->d_name) == 0)
		{
			continue;
		}

		lstat(tFileInfo->d_name, &statbuf);
		if((S_IFDIR & statbuf.st_mode) && bRecursion)
		{
			GetDirFiles(tFileInfo->d_name, pszFileType, vtFileList, bRecursion);
		}
		else
		{
			vtFileList.push_back(std::string(szTem) + std::string(tFileInfo->d_name));
		}
	}

	closedir(pDirInfo);
#endif
	return TRUE;
}

BOOL CommonFunc::IsSameDay(UINT64 uTime)
{
#ifdef WIN32
	return ((uTime - _timezone) / 86400) == ((GetCurrTime() - _timezone) / 86400);
#else
	return ((uTime - timezone) / 86400) == ((GetCurrTime() - timezone) / 86400);
#endif

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

UINT32 CommonFunc::GetCurProcessID()
{
	UINT32 dwProcessID = 0;
#ifdef WIN32
	dwProcessID = ::GetCurrentProcessId();
#else
	dwProcessID = (UINT32)getpid();
#endif
	return dwProcessID;
}


VOID CommonFunc::Sleep(UINT32 dwMilliseconds)
{
#ifdef WIN32
	::Sleep(dwMilliseconds);
#else
	struct timespec req;
	req.tv_sec = 0;
	req.tv_nsec = dwMilliseconds * 1000000;
	nanosleep(&req, NULL);
#endif
	return;
}

UINT32 CommonFunc::GetFreePhysMemory()
{
	UINT32 dwFreeSize = 0;
#ifdef WIN32
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof (statex);

	GlobalMemoryStatusEx (&statex);

	dwFreeSize = (UINT32)(statex.ullAvailPhys / 1024 / 1024);
#else
	UINT32 dwPageSize;
	UINT32 dwFreePages;
	dwPageSize = sysconf (_SC_PAGESIZE) / 1024;
	dwFreePages = sysconf (_SC_AVPHYS_PAGES) / 1024;
	dwFreeSize = dwFreePages * dwPageSize;
#endif

	return dwFreeSize;
}

INT32 CommonFunc::GetRandNum(INT32 nType)
{
	if(nType >= 100 || nType < 0)
	{
		return 0;
	}

	static INT32 nRandIndex[100] = {0};
	static INT32 vtGlobalRankValue[10000];
	static BOOL  bInit = FALSE;

	if(bInit == FALSE)
	{
		bInit = TRUE;
		INT32  nTempIndex;
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

	return  vtGlobalRankValue[(nRandIndex[nType]++) % 10000];
}

UINT32 CommonFunc::GetLastError()
{
#ifdef WIN32
	return ::GetLastError();
#else
	return errno;
#endif
}

HANDLE CommonFunc::CreateShareMemory(UINT32 dwModuleID, INT32 nPage, INT32 nSize)
{
	HANDLE hShare = NULL;
#ifdef WIN32
	CHAR szMemName[128] = {0};
	snprintf(szMemName, 128, "SM_%d", dwModuleID << 16 | nPage);
	hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSize, szMemName);
	if (hShare != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(hShare);
			hShare = NULL;
		}
	}
#else
	hShare = shmget(dwModuleID << 16 | nPage, nSize, 0666 | IPC_CREAT | IPC_EXCL);
	if (hShare == -1)
	{
		hShare = NULL;
	}
#endif
	return hShare;
}

// HANDLE CommonFunc::CreateShareMemory(std::string strName, INT32 nSize)
// {
// 	HANDLE hShare = NULL;
// #ifdef WIN32
// 	hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSize, strName.c_str());
// 	if(hShare != NULL)
// 	{
// 		if(GetLastError() == ERROR_ALREADY_EXISTS)
// 		{
// 			CloseHandle(hShare);
// 			hShare = NULL;
// 		}
// 	}
// #else
// 	key_t key = ftok(strName.c_str(), 201);
// 	hShare = shmget(key, nSize, 0666 | IPC_CREAT | IPC_EXCL);
// 	if(hShare == -1)
// 	{
// 		hShare = NULL;
// 	}
// #endif
// 	return hShare;
// }

HANDLE CommonFunc::OpenShareMemory(UINT32 dwModuleID, INT32 nPage)
{
	HANDLE hShare = NULL;
#ifdef WIN32
	CHAR szMemName[128] = {0};
	snprintf(szMemName, 128, "SM_%d", dwModuleID << 16 | nPage);
	hShare = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, szMemName);
#else
	hShare = shmget(dwModuleID << 16 | nPage, 0, 0);
	if (hShare == -1)
	{
		return NULL;
	}
#endif
	return hShare;
}

// HANDLE CommonFunc::OpenShareMemory(std::string strName)
// {
// #ifdef WIN32
// 	HANDLE hShare = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, strName.c_str());
// #else
// 	key_t key = ftok(strName.c_str(), 201);
// 	HANDLE hShare = shmget(key, 0, 0);
// 	if (hShare == -1)
// 	{
// 		return NULL;
// 	}
// #endif
// 	return hShare;
// }



CHAR* CommonFunc::GetShareMemory(HANDLE hShm)
{
#ifdef WIN32
	CHAR* pdata = (CHAR*)MapViewOfFile(hShm, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
#else
	CHAR* pdata = (CHAR*)shmat(hShm, (void*)0, 0);
#endif
	return pdata;
}

BOOL CommonFunc::ReleaseShareMemory(CHAR* pMem)
{
#ifdef WIN32
	return UnmapViewOfFile(pMem);
#else
	return (0 == shmdt(pMem));
#endif
}

BOOL CommonFunc::CloseShareMemory(HANDLE hShm)
{
#ifdef WIN32
	return  CloseHandle(hShm);
#else
	return (0 == shmctl(hShm, IPC_RMID, 0));
#endif
}

BOOL CommonFunc::DbgTrace(char* format, ...)
{
#if (defined WIN32) && (defined _DEBUG)
	char szLog[1024] = { 0 };
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(szLog, 1023, format, argptr);
	va_end(argptr);
	OutputDebugString(szLog);
#endif

	return TRUE;
}

BOOL CommonFunc::KillProcess(UINT64 dwPid)
{
#ifdef WIN32
	HANDLE hPrc;
	if (0 == dwPid)
	{
		return FALSE;
	}

	hPrc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)dwPid);
	if (hPrc == NULL)
	{
		return TRUE;
	}

	if (!TerminateProcess(hPrc, 0))
	{
		CloseHandle(hPrc);
		return FALSE;
	}
	else
	{
		WaitForSingleObject(hPrc, 200);
	}
	CloseHandle(hPrc);
#else
	kill(dwPid, SIGKILL);
#endif
	return TRUE;
}

INT32 CommonFunc::Min(INT32 nValue1, INT32 nValue2)
{
	return (nValue1 < nValue2) ? nValue1 : nValue2;
}

BOOL CommonFunc::IsAlreadyRun(std::string strSignName)
{
#ifdef WIN32
	HANDLE hMutex = NULL;
	hMutex = CreateMutex(NULL, FALSE, strSignName.c_str());
	if (hMutex != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(hMutex);
			return TRUE;
		}
	}
	return FALSE;
#else
	INT32 fd;
	CHAR szbuf[32] = {0};

	std::string strLockFile = "/var/run/" + strSignName + ".pid";
	fd = open(strLockFile.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd < 0)
	{
		return TRUE;
	}

	struct flock fl;
	fl.l_type  = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len   = 0;

	if (fcntl(fd, F_SETLK, &fl) < 0)
	{
		close(fd);
		return TRUE;
	}

	ftruncate(fd, 0);

	snprintf(szbuf, 32, "%ld", (long)getpid());

	write(fd, szbuf, strlen(szbuf) + 1);

	return FALSE;
#endif
}
