#include "stdafx.h"
#include "CommonFunc.h"

INT32 CommonFunc::GetProcessorNum()
{
    INT32 nNum = 0;
#ifdef WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    nNum = sysInfo.dwNumberOfProcessors;
#else
    nNum = sysconf(_SC_NPROCESSORS_CONF);
#endif

    return nNum;
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
    if (pszDir == NULL || pszFileType == NULL || NULL == strrchr(pszFileType, '.'))
    {
        return FALSE;
    }

    char   szTem[1024] = { 0 };
    char   szDir[1024] = { 0 };
    strcpy(szTem, pszDir);
    if (szTem[strlen(szTem) - 1] != '\\' && szTem[strlen(szTem) - 1] != '/')
    {
        strcat(szTem, "/");
    }

    strcpy(szDir, szTem);
#ifdef WIN32
    strcat(szDir, "*.*");
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
            if (strcmp(pszFileType, "*.*") != 0)
            {
                char* szSuffix = strrchr(tFileInfo.name, '.');
                if (szSuffix == NULL || strcmp(szSuffix, pszFileType + 1) != 0)
                {
                    continue;
                }
            }

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

        char szTempDir[1024] = {0};
        strcpy(szTempDir, pszDir);
        strcat(szTempDir, tFileInfo->d_name);
        lstat(szTempDir, &statbuf);
        if((S_IFDIR & statbuf.st_mode) && bRecursion)
        {
            char   szSub[1024] = { 0 };
            strcpy(szSub, pszDir);
            if (szSub[strlen(szSub) - 1] != '\\' || szSub[strlen(szSub) - 1] != '/')
            {
                strcat(szSub, "/");
            }
            strcat(szSub, tFileInfo->d_name);
            GetDirFiles(szSub, pszFileType, vtFileList, bRecursion);
        }
        else
        {
            if (strcmp(pszFileType, "*.*") != 0)
            {
                char* szSuffix = strrchr(tFileInfo->d_name, '.');
                if (szSuffix == NULL || strcmp(szSuffix, pszFileType + 1) != 0)
                {
                    continue;
                }
            }
            vtFileList.push_back(std::string(szTem) + std::string(tFileInfo->d_name));
        }
    }

    closedir(pDirInfo);
#endif
    return TRUE;
}

BOOL CommonFunc::GetSubDirNames(const char* pszDir, const char* pszBegin, std::vector<std::string>& vtDirList, BOOL bRecursion)
{
    if (pszDir == NULL)
    {
        return FALSE;
    }

    char   szTem[1024] = { 0 };
    char   szDir[1024] = { 0 };
    strcpy(szTem, pszDir);
    if (szTem[strlen(szTem) - 1] != '\\' && szTem[strlen(szTem) - 1] != '/')
    {
        strcat(szTem, "/");
    }


    strcpy(szDir, szTem);

#ifdef WIN32
    strcat(szDir, "*.*");
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

        if (tFileInfo.attrib   &  _A_SUBDIR)
        {
            std::string strDirName = tFileInfo.name;

            if (strDirName.substr(0, strlen(pszBegin)) == std::string(pszBegin))
            {
                vtDirList.push_back(strDirName);
            }

            if ( bRecursion)
            {
                char   szSub[1024] = { 0 };
                strcpy(szSub, pszDir);
                if (szSub[strlen(szSub) - 1] != '\\' || szSub[strlen(szSub) - 1] != '/')
                {
                    strcat(szSub, "/");
                }
                strcat(szSub, tFileInfo.name);
                GetSubDirNames(szSub, pszBegin, vtDirList, bRecursion);
            }
        }
    }
    while (_findnext(hFile, &tFileInfo) == 0);
    _findclose(hFile);

#else

    DIR* pDirInfo;
    struct dirent* tFileInfo;
    struct stat statbuf;
    if ((pDirInfo = opendir(pszDir)) == NULL)
    {
        return FALSE;
    }

    while ((tFileInfo = readdir(pDirInfo)) != NULL)
    {
        if (strcmp(".", tFileInfo->d_name) == 0 || strcmp("..", tFileInfo->d_name) == 0)
        {
            continue;
        }

        char szTempDir[1024] = {0};
        strcpy(szTempDir, pszDir);
        strcat(szTempDir, tFileInfo->d_name);
        lstat(szTempDir, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            std::string strDirName = tFileInfo->d_name;
            if (strDirName.substr(0, strlen(pszBegin)) == std::string(pszBegin))
            {
                vtDirList.push_back(strDirName);
            }

            if (bRecursion)
            {
                char   szSub[1024] = { 0 };
                strcpy(szSub, pszDir);
                if (szSub[strlen(szSub) - 1] != '\\' || szSub[strlen(szSub) - 1] != '/')
                {
                    strcat(szSub, "/");
                }
                strcat(szSub, tFileInfo->d_name);
                GetSubDirNames(szSub, pszBegin, vtDirList, bRecursion);
            }
        }
    }

    closedir(pDirInfo);
#endif
    return TRUE;
}

INT32 CommonFunc::GetCurThreadID()
{
    INT32 nThreadID = 0;
#ifdef WIN32
    nThreadID = ::GetCurrentThreadId();
#else
    nThreadID = (INT32)pthread_self();
#endif
    return nThreadID;
}

INT32 CommonFunc::GetCurProcessID()
{
    INT32 nProcessID = 0;
#ifdef WIN32
    nProcessID = ::GetCurrentProcessId();
#else
    nProcessID = (INT32)getpid();
#endif
    return nProcessID;
}


VOID CommonFunc::Sleep(INT32 nMilliseconds)
{
#ifdef WIN32
    ::Sleep(nMilliseconds);
#else
    struct timespec req;
    req.tv_sec = nMilliseconds / 1000;
    req.tv_nsec = nMilliseconds % 1000 * 1000000;
    nanosleep(&req, NULL);
#endif
    return;
}

INT32 CommonFunc::GetFreePhysMemory()
{
    INT32 nFreeSize = 0;
#ifdef WIN32
    MEMORYSTATUSEX statex;

    statex.dwLength = sizeof (statex);

    GlobalMemoryStatusEx (&statex);

    nFreeSize = (INT32)(statex.ullAvailPhys / 1024 / 1024);
#else
    INT32 nPageSize;
    INT32 nFreePages;
    nPageSize = sysconf (_SC_PAGESIZE) / 1024;
    nFreePages = sysconf (_SC_AVPHYS_PAGES) / 1024;
    nFreeSize = nFreePages * nPageSize;
#endif

    return nFreeSize;
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
        INT32 nTempIndex;
        INT32 nTemp;
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

INT32 CommonFunc::GetLastError()
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
HANDLE CommonFunc::CreateShareMemory(INT32 nModuleID, INT32 nPage, INT32 nSize)
{
    HANDLE hShare = NULL;
#ifdef WIN32
    CHAR szMemName[128] = {0};
    snprintf(szMemName, 128, "SM_%d", (nModuleID << 16) | nPage);
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
    hShare = shmget((nModuleID << 16) | nPage, nSize, 0666 | IPC_CREAT | IPC_EXCL);
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
//  HANDLE hShare = NULL;
// #ifdef WIN32
//  hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSize, strName.c_str());
//  if(hShare != NULL)
//  {
//      if(GetLastError() == ERROR_ALREADY_EXISTS)
//      {
//          CloseHandle(hShare);
//          hShare = NULL;
//      }
//  }
// #else
//  key_t key = ftok(strName.c_str(), 201);
//  hShare = shmget(key, nSize, 0666 | IPC_CREAT | IPC_EXCL);
//  if(hShare == -1)
//  {
//      hShare = NULL;
//  }
// #endif
//  return hShare;
// }

//s:10m:6:p:16
HANDLE CommonFunc::OpenShareMemory(INT32 nModuleID, INT32 nPage)
{
    HANDLE hShare = NULL;
#ifdef WIN32
    CHAR szMemName[128] = {0};
    snprintf(szMemName, 128, "SM_%d", nModuleID << 16 | nPage);
    hShare = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, szMemName);
#else
    hShare = shmget(nModuleID << 16 | nPage, 0, 0);
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
//  HANDLE hShare = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, strName.c_str());
// #else
//  key_t key = ftok(strName.c_str(), 201);
//  HANDLE hShare = shmget(key, 0, 0);
//  if (hShare == -1)
//  {
//      return NULL;
//  }
// #endif
//  return hShare;
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


BOOL CommonFunc::KillProcess(INT32 nPid)
{
#ifdef WIN32
    HANDLE hPrc;
    if (0 == nPid)
    {
        return FALSE;
    }

    hPrc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)nPid);
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
    kill(nPid, SIGKILL);
#endif
    return TRUE;
}

BOOL CommonFunc::IsProcessExist(INT32 nPid)
{
#ifdef WIN32
    HANDLE hPrc;
    if (0 == nPid)
    {
        return FALSE;
    }

    DWORD nExitCode = 0;
    hPrc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)nPid);
    if (hPrc == NULL)
    {
        return FALSE;
    }

    //! 进程关闭后, 句柄不一定为空, 需要获取进程退出码进一步判断
    GetExitCodeProcess(hPrc, &nExitCode);
    if (nExitCode != STILL_ACTIVE)
    {
        return FALSE;
    }
    CloseHandle(hPrc);
#else
    if (kill(nPid, 0) < 0)
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

INT32 CommonFunc::GetProcessID(const char* pszProcName)
{
#ifdef WIN32
    INT32 nProcID = 0;
    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 stProcessEntry;
    stProcessEntry.dwSize = sizeof(PROCESSENTRY32);
    BOOL bRet = Process32First(Snapshot, &stProcessEntry);
    while (bRet)
    {
        if(stricmp(pszProcName, stProcessEntry.szExeFile) == 0)
        {
            nProcID = stProcessEntry.th32ProcessID;
            break;
        }

        bRet = Process32Next(Snapshot, &stProcessEntry);
    }

    CloseHandle(Snapshot);

    return nProcID;
#else
    FILE* fp;
    char buf[100];
    char cmd[200] = { '\0' };
    pid_t pid = 0;
    sprintf(cmd, "pidof %s", pszProcName);

    if ((fp = popen(cmd, "r")) != NULL)
    {
        if (fgets(buf, 255, fp) != NULL)
        {
            pid = atoi(buf);
        }
    }

    pclose(fp);
    return pid;
#endif
}

BOOL CommonFunc::StartProcess(const char* pszProcName, const char* pszCommandLine, const char*  pszWorkPath)
{
#ifdef WIN32
    STARTUPINFO stStartUpInfo;
    memset(&stStartUpInfo, 0, sizeof(stStartUpInfo));
    stStartUpInfo.cb = sizeof(stStartUpInfo);

    PROCESS_INFORMATION stProcessInfo;
    memset(&stProcessInfo, 0, sizeof(stProcessInfo));

    if (!CreateProcess(pszProcName, (LPSTR)pszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, pszWorkPath, &stStartUpInfo, &stProcessInfo))
    {
        return FALSE;
    }
#else
    const char* pDot = strrchr(pszProcName, '.');
    if (pDot == NULL)
    {
        const char* p = strrchr(pszProcName, '/');
        if (execl(pszProcName, p + 1, pszCommandLine, (char*)0) < 0)
        {
            return FALSE;
        }
    }
    else if(strcmp(pDot, ".sh") == 0)
    {
        if (execl("/bin/sh", "sh", pszProcName, (char*)0) < 0)
        {
            return FALSE;
        }
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
