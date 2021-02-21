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
	time_t t = time(0);

	return (UINT64)t;
}

UINT64 CommonFunc::GetCurMsTime()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}

tm CommonFunc::GetCurrTmTime()
{
	time_t t = (time_t)GetCurrTime();

	struct tm _tm_time;
	_tm_time = *localtime(&t);

	return _tm_time;
}

UINT64 CommonFunc::GetDayBeginTime(UINT64 uTime)
{
	if (uTime == 0)
	{
		uTime = GetCurrTime();
	}
	time_t t = (time_t)uTime;
	tm* t_tm = localtime(&t);
	t_tm->tm_hour = 0;
	t_tm->tm_min = 0;
	t_tm->tm_sec = 0;
	t = mktime(t_tm);
	return (UINT64)t;
}

UINT64 CommonFunc::GetWeekBeginTime(UINT64 uTime)
{
	if (uTime == 0)
	{
		uTime = GetCurrTime();
	}

	time_t t = (time_t)uTime;
	tm* t_tm = localtime(&t);
	return (UINT64)t - (t_tm->tm_wday == 0 ? 6 : t_tm->tm_wday - 1) * 86400 - t_tm->tm_hour * 3600 - t_tm->tm_min * 60 - t_tm->tm_sec;
}


UINT64 CommonFunc::GetMonthBeginTime(UINT64 uTime)
{
	if (uTime == 0)
	{
		uTime = GetCurrTime();
	}
	time_t t = (time_t)uTime;
	tm* t_tm = localtime(&t);
	tm newtm;
	newtm.tm_year = t_tm->tm_year;
	newtm.tm_mon = t_tm->tm_mon;
	newtm.tm_mday = 1;
	newtm.tm_hour = 0;
	newtm.tm_min = 0;
	newtm.tm_sec = 0;

	return mktime(&newtm);
}

time_t CommonFunc::YearTimeToSec(INT32 nYear, INT32 nMonth, INT32 nDay, INT32 nHour, INT32 nMin, INT32 nSec)
{
	time_t t = (time_t)GetCurrTime();
	tm* t_tm = localtime(&t);

	tm newtm;
	newtm.tm_year = (nYear < 0) ? t_tm->tm_year : nYear - 1900;
	newtm.tm_mon = (nMonth < 0) ? t_tm->tm_mon : nMonth - 1;
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

time_t CommonFunc::DateStringToTime(std::string strDate)
{
	if (strDate.size() < 14)
	{
		return 0;
	}

	INT32 nYear;
	INT32 nMonth;
	INT32 nDay;
	INT32 nHour;
	INT32 nMinute;
	INT32 nSecond;

	INT32 nRet = sscanf(strDate.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
	if (nRet < 6)
	{
		return 0;
	}

	return YearTimeToSec(nYear, nMonth, nDay, nHour, nMinute, nSecond);
}

UINT64 CommonFunc::GetTickCount()
{
#ifdef WIN32
	return ::GetTickCount64();
#else
	UINT64 uTickCount = 0;
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

BOOL CommonFunc::IsSameWeek(UINT64 uTime)
{
	time_t t = GetCurrTime();
	tm t_tmSrc = *localtime(&t);
	UINT64 SrcWeekBegin = (UINT64)t - (t_tmSrc.tm_wday == 0 ? 6 : t_tmSrc.tm_wday - 1) * 86400 - t_tmSrc.tm_hour * 3600 - t_tmSrc.tm_min * 60 - t_tmSrc.tm_sec;

	t = uTime;
	tm t_tmDest = *localtime(&t);
	UINT64 SrcWeekDest = (UINT64)t - (t_tmDest.tm_wday == 0 ? 6 : t_tmDest.tm_wday - 1) * 86400 - t_tmDest.tm_hour * 3600 - t_tmDest.tm_min * 60 - t_tmDest.tm_sec;

	return (SrcWeekBegin - SrcWeekDest) / (86400 * 7) <= 0;
}

BOOL CommonFunc::IsSameMonth(UINT64 uTime)
{
	time_t t = uTime;
	tm t_tmSrc = *localtime(&t);

	time_t t_mon = GetCurrTime();
	tm t_tmDest = *localtime(&t_mon);

	return (t_tmSrc.tm_mon == t_tmDest.tm_mon);
}

INT32 CommonFunc::DiffWeeks(UINT64 uTimeSrc, UINT64 uTimeDest)
{
	time_t t = uTimeSrc;
	tm t_tmSrc = *localtime(&t);
	UINT64 SrcWeekBegin = (UINT64)t - (t_tmSrc.tm_wday == 0 ? 6 : t_tmSrc.tm_wday - 1) * 86400 - t_tmSrc.tm_hour * 3600 - t_tmSrc.tm_min * 60 - t_tmSrc.tm_sec;

	t = uTimeDest;
	tm t_tmDest = *localtime(&t);
	UINT64 SrcWeekDest = (UINT64)t - (t_tmDest.tm_wday == 0 ? 6 : t_tmDest.tm_wday - 1) * 86400 - t_tmDest.tm_hour * 3600 - t_tmDest.tm_min * 60 - t_tmDest.tm_sec;

	return uTimeSrc > uTimeDest ? (INT32)((SrcWeekBegin - SrcWeekDest) / (86400 * 7)) : (INT32)((SrcWeekDest - SrcWeekBegin) / (86400 * 7));
}

INT32 CommonFunc::DiffDays(UINT64 uTimeSrc, UINT64 uTimeDest)
{
#ifdef WIN32
	if (uTimeSrc > uTimeDest)
	{
		return (INT32)((uTimeSrc - _timezone) / 86400 - (uTimeDest - _timezone) / 86400);
	}

	return (INT32)((uTimeDest - _timezone) / 86400 - (uTimeSrc - _timezone) / 86400);
#else
	if (uTimeSrc > uTimeDest)
	{
		return (INT32)((uTimeSrc - timezone) / 86400 - (uTimeDest - timezone) / 86400);
	}

	return (INT32)((uTimeDest - timezone) / 86400 - (uTimeSrc - timezone) / 86400);
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

std::string CommonFunc::GetLastErrorStr(INT32 nError)
{
	std::string strErrorText;
#ifdef WIN32
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, nError,
	              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	strErrorText = (LPTSTR)lpMsgBuf;

	LocalFree(lpMsgBuf);
#else
	strErrorText = strerror(nError);
#endif

	return strErrorText;
}

//s:8m:8:p:16
HANDLE CommonFunc::CreateShareMemory(UINT32 dwModuleID, INT32 nPage, INT32 nSize)
{
	HANDLE hShare = NULL;
#ifdef WIN32
	CHAR szMemName[128] = {0};
	snprintf(szMemName, 128, "SM_%d", (dwModuleID << 16) | nPage);
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
	hShare = shmget((dwModuleID << 16) | nPage, nSize, 0666 | IPC_CREAT | IPC_EXCL);
	if (hShare == -1)
	{
		hShare = NULL;
	}
#endif
	return hShare;
}

//下面是用路径来创建建共享内存，可惜linux有缺陷
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

//s:10m:6:p:16
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

BOOL CommonFunc::IsProcessExist(UINT64 dwPid)
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
		return FALSE;
	}
	CloseHandle(hPrc);
#else
	if (kill(dwPid, 0) < 0)
	{
		return FALSE;
	}

	if (errno == ESRCH)
	{
		return FALSE;
	}
#endif
	return TRUE;
}

UINT32 CommonFunc::GetProcessID(std::string strProcName)
{
#ifdef WIN32
	UINT32 dwProcID = 0;
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 stProcessEntry;
	stProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(Snapshot, &stProcessEntry);
	while (bRet)
	{
		if(stricmp(strProcName.c_str(), stProcessEntry.szExeFile) == 0)
		{
			dwProcID = stProcessEntry.th32ProcessID;
			break;
		}

		bRet = Process32Next(Snapshot, &stProcessEntry);
	}

	CloseHandle(Snapshot);

	return dwProcID;
#else
	return 0;
#endif
}

BOOL CommonFunc::CreateProcess(std::string strProcName, std::string strCommandLine)
{
#ifdef WIN32
	STARTUPINFO stStartUpInfo;
	memset(&stStartUpInfo, 0, sizeof(stStartUpInfo));
	stStartUpInfo.cb = sizeof(stStartUpInfo);

	PROCESS_INFORMATION stProcessInfo;
	memset(&stProcessInfo, 0, sizeof(stProcessInfo));

	if (!CreateProcess(strProcName.c_str(), (LPSTR)strCommandLine.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &stStartUpInfo, &stProcessInfo))
	{
		return FALSE;
	}
#else
	std::string strExe = "./" + strProcName;
	if (execl(strExe.c_str(), strProcName.c_str(), strCommandLine.c_str(), (char*)0) < 0)
	{
		return FALSE;
	}

#endif

	return TRUE;
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

BOOL CommonFunc::PrintColorText(CHAR* pSzText, INT32 nColor)
{
#ifdef WIN32
	switch (nColor)
	{
		case 1:
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			printf(pSzText);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		break;
		case 2:
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			printf(pSzText);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		break;
		case 3:
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			printf(pSzText);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		break;
		default:
		{
			printf(pSzText);
		}
		break;
	}
#else
	switch (nColor)
	{
		case 1:
		{
			printf("\033[1;31;40m%s\033[0m", pSzText);
		}
		break;
		case 2:
		{
			printf("\033[1;33;40m%s\033[0m", pSzText);
		}
		break;
		case 3:
		{
			printf("\033[1;32;40m%s\033[0m", pSzText);
		}
		break;
		default:
		{
			printf(pSzText);
		}
		break;
	}
#endif

	return TRUE;
}

BOOL CommonFunc::GetBitValue(UINT64 nValue, INT32 nPos)
{
	return ((nValue >> (nPos - 1)) & 1) > 0;
}

BOOL CommonFunc::SetBitValue(UINT64& nValue, INT32 nPos, BOOL bValue)
{
	if (bValue)
	{
		nValue |= (UINT64)1 << (nPos - 1);
	}
	else
	{
		nValue &= ~((UINT64)1 << (nPos - 1));
	}

	return TRUE;
}
