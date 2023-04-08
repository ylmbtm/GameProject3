#ifndef __COMMON_TIME__
#define __COMMON_TIME__

namespace CommonFunc
{
BOOL            IsSameDay(UINT64 uTime);

BOOL            IsSameWeek(UINT64 uTime);

BOOL            IsSameMonth(UINT64 uTime, UINT64 sTime = 0);

INT32           DiffWeeks(UINT64 uTimeSrc, UINT64 uTimeDest);

INT32           DiffDays(UINT64 uTimeSrc, UINT64 uTimeDest);

UINT64          GetCurrTime(); //获取当前的秒数

BOOL            SetCurrTimeAdd(INT64 nTimeAdd);

UINT64          GetCurMsTime(); //获取当前的毫秒数

tm              GetCurrTmTime(UINT64 uTime = 0);

UINT64          GetDayBeginTime(UINT64 uTime = 0); //获取当天起点的秒数

UINT64          GetWeekBeginTime(UINT64 uTime = 0); //获取当周起点的秒数

UINT64          GetMonthBeginTime(UINT64 uTime = 0);//获取当月起点的秒数

UINT64          GetMonthRemainTime(UINT64 uTime = 0);   //获取当月剩余的秒数

INT32           GetWeekDay();       //获得周几

INT32           GetMonthDay();

INT32           GetMonth(UINT64 uTime = 0);

INT32           GetTimeZone();

time_t          YearTimeToSec(INT32 nYear, INT32 nMonth, INT32 nDay, INT32 nHour, INT32 nMin, INT32 nSec);

std::string     TimeToString(time_t tTime);

time_t          DateStringToTime(std::string strDate);

UINT64          GetTickCount();
}


#endif /* __COMMON_TIME__*/
