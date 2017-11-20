#ifndef __COMMON_FUNCTION__
#define __COMMON_FUNCTION__

#define GET_BIT(X,Y) (((X) >> (Y-1)) & 1)
#define SET_BIT(X,Y) ((X) |= (1 << (Y)))
#define CLR_BIT(X,Y) ((X) &= (~(1<<Y)))

namespace CommonFunc
{
UINT32			GetProcessorNum();

std::string		GetCurrentDir();

std::string		GetCurrentExeDir();

BOOL			SetCurrentWorkPath(std::string strPath);

BOOL			CreateDir(std::string& strDir);

BOOL			GetDirFiles(const char* pszDir, char* pszFileType, std::vector<std::string>& vtFileList, BOOL bRecursion);

//获取当前的秒数
UINT64			GetCurrTime();

tm				GetCurrTmTime();

//获取当前0点的秒数
UINT64			GetDayStartTime();

//获取当前0点的秒数
UINT64			GetWeekStartTime();

//获取一个由年月日组合的一个唯一值
UINT32			GetCurrDate();

time_t			YearTimeToSec(INT32 nYear, INT32 nMonth, INT32 nDay, INT32 nHour, INT32 nMin, INT32 nSec);

UINT32			GetTickCount32();

UINT64			GetTickCount();

UINT32			GetCurThreadID();

UINT32			GetCurProcessID();

UINT32			GetFreePhysMemory();

INT32           GetRandNum(INT32 nType);

UINT32			GetLastError();

HANDLE          CreateShareMemory(std::string strName, INT32 nSize);

HANDLE			OpenShareMemory(std::string strName);

CHAR* 			GetShareMemory(HANDLE hShm);

BOOL 			ReleaseShareMemory(CHAR* pMem);

BOOL			CloseShareMemory(HANDLE hShm);

BOOL			DbgTrace(char* format, ...);
}


#endif /* __COMMON_FUNCTION__*/
