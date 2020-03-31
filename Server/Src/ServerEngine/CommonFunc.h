#ifndef __COMMON_FUNCTION__
#define __COMMON_FUNCTION__

#define GET_BIT(X,Y) (((X) >> (Y-1)) & 1)
#define SET_BIT(X,Y) ((X) |= (1 << (Y)))
#define CLR_BIT(X,Y) ((X) &= (~(1<<Y)))

namespace CommonFunc
{
UINT32			GetProcessorNum();

std::string		GetCurrentWorkDir();

BOOL			SetCurrentWorkDir(std::string strPath);

std::string		GetCurrentExeDir();

BOOL			CreateDir(std::string& strDir);

BOOL			GetDirFiles(const char* pszDir, char* pszFileType, std::vector<std::string>& vtFileList, BOOL bRecursion);

BOOL			IsSameDay(UINT64 uTime);

UINT64			GetCurrTime(); //获取当前的秒数

tm				GetCurrTmTime();

UINT64			GetDayBeginTime(); //获取当天起点的秒数

UINT64			GetWeekBeginTime(); //获取当周起点的秒数

time_t			YearTimeToSec(INT32 nYear, INT32 nMonth, INT32 nDay, INT32 nHour, INT32 nMin, INT32 nSec);

std::string     TimeToString(time_t tTime);

UINT64			GetTickCount();

UINT32			GetCurThreadID();

UINT32			GetCurProcessID();

VOID			Sleep(UINT32 dwMilliseconds);

UINT32			GetFreePhysMemory();

INT32           GetRandNum(INT32 nType);

UINT32			GetLastError();

// HANDLE       CreateShareMemory(std::string strName, INT32 nSize);
//
// HANDLE		OpenShareMemory(std::string strName);

HANDLE          CreateShareMemory(UINT32 dwModuleID, INT32 nPage, INT32 nSize);

HANDLE			OpenShareMemory(UINT32 dwModuleID, INT32 nPage);

CHAR* 			GetShareMemory(HANDLE hShm);

BOOL 			ReleaseShareMemory(CHAR* pMem);

BOOL			CloseShareMemory(HANDLE hShm);

BOOL			DbgTrace(char* format, ...);

BOOL			KillProcess(UINT64 dwPid);

INT32			Min(INT32 nValue1, INT32 nValue2);

BOOL            IsAlreadyRun(std::string strSignName);
}


#endif /* __COMMON_FUNCTION__*/
