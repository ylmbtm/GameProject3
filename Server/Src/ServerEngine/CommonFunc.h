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
UINT32			GetCurrTime();

tm				GetCurrTmTime();

//获取当前0点的秒数
UINT32			GetDayStartTime();

//获取当前0点的秒数
UINT32			GetWeekStartTime();

//获取一个由年月日组合的一个唯一值
UINT32			GetCurrDate();

UINT32			GetTickCount();

UINT64			GetTickCount64();

UINT32			GetCurThreadID();

UINT32			GetFreePhysMemory();

INT32           FloatToInt(FLOAT value);

INT32           GetRandNum(INT32 nType);

UINT32			GetLastError();

HANDLE          CreateShareMemory(std::string strName, INT32 nSize);

HANDLE			OpenShareMemory(std::string strName);

CHAR* 			GetShareMemory(HANDLE hShm);

BOOL 			ReleaseShareMemory(CHAR* pMem);

BOOL			CloseShareMemory(HANDLE hShm);

}


#endif /* __COMMON_FUNCTION__*/
