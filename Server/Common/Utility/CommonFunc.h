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

	BOOL			CreateDir(std::string &strDir);

	UINT32			GetCurrTime();

	UINT32			GetDayTime();

	UINT32			GetCurrDate();

	UINT32			GetTickCount();

	UINT64			GetTickCount64();

	UINT32			GetCurThreadID();

	UINT32			GetFreePhysMemory();  

	INT32           FloatToInt(FLOAT value);

	UINT32          GetRandNum(INT32 nType);
}


#endif /* __COMMON_FUNCTION__*/
