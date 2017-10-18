#include "stdafx.h"
#include "CommonFunc.h"

UINT32 CommonFunc::GetProcessorNum()
{
	UINT32 dwNum = 0;
#ifdef WIN32
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	dwNum = sysInfo.dwNumberOfProcessors * 2;
#else
	dwNum = sysconf(_SC_NPROCESSORS_CONF);
#endif

	return dwNum;
}

std::string CommonFunc::GetCurrentDir()
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
	char szPath[1024];

#ifdef WIN32
	ZeroMemory(szPath, 1024);
	GetModuleFileName(NULL, szPath, 1024);
	char* p = strrchr(szPath, '\\');
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

	t = time(0);

	return (UINT32)t;
}

tm CommonFunc::GetCurrTmTime()
{
	time_t rawtime;
	struct tm* timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	return *timeinfo;
}

UINT32 CommonFunc::GetDayStartTime()
{
	time_t t;
	t = time(0);
	tm* t_tm = localtime(&t);
	t_tm->tm_hour = 0;
	t_tm->tm_min = 0;
	t_tm->tm_sec = 0;
	t = mktime(t_tm);
	return (UINT32)t;
}

UINT32 CommonFunc::GetWeekStartTime()
{
	time_t t;
	t = time(0);
	tm* t_tm = localtime(&t);
	t_tm->tm_hour = 0;
	t_tm->tm_min = 0;
	t_tm->tm_sec = 0;
	t_tm->tm_wday = 0;
	t = mktime(t_tm);
	return (UINT32)t;
}

UINT32 CommonFunc::GetCurrDate()
{
	time_t t = time(0);

	struct tm* pTm = localtime(&t);

	pTm->tm_year += 1900;

	UINT32 dwCurDate = (pTm->tm_year << 16) | (pTm->tm_mon << 8) | (pTm->tm_mday);

	return dwCurDate;
}



UINT32 CommonFunc::GetTickCount()
{
#ifdef WIN32
	return ::GetTickCount();
#else

	UINT32 dwTickCount = 0;;
	struct timespec on;
	if(0 == clock_gettime(CLOCK_MONOTONIC, &on) )
	{
		dwTickCount = on.tv_sec * 1000 + on.tv_nsec / 1000000;
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
	if(0 == clock_gettime(CLOCK_MONOTONIC, &on) )
	{
		dwTickCount = on.tv_sec * 1000 + on.tv_nsec / 1000000;
	}

	return dwTickCount;
#endif
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


INT32 CommonFunc::FloatToInt(FLOAT value)
{
	return 0;
}

INT32 CommonFunc::GetRandNum(INT32 nType)
{
	if(nType >= 100 || nType < 0)
	{
		return 0;
	}

	static INT32 nRandIndex[100] = {0};
	static INT32 vtGlobalRankValue[10000];
	static BOOL  bInit = false;

	if(bInit == false)
	{
		bInit = true;
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

HANDLE CommonFunc::CreateShareMemory(std::string strName, INT32 nSize)
{
#ifdef WIN32
	HANDLE hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSize, strName.c_str());
	if(hShare != NULL)
	{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(hShare);
			hShare = NULL;
		}
	}
#else
	key_t key = ftok(strName.c_str(), 201);
	HANDLE hShare = shmget(key, nSize, 0666 | IPC_CREAT | IPC_EXCL);
	if(hShare == -1)
	{
		hShare = NULL;
	}
#endif
	return hShare;
}

HANDLE CommonFunc::OpenShareMemory(std::string strName)
{
#ifdef WIN32
	HANDLE hShare = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, strName.c_str());
#else
	key_t key = ftok(strName.c_str(), 201);
	HANDLE hShare = shmget(key, 0, 0);
#endif
	return hShare;
}

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
