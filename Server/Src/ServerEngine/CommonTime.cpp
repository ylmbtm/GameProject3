#include "stdafx.h"
#include "CommonTime.h"

INT64 g_TimeAdd = 0;

UINT64 CommonFunc::GetCurrTime()
{
    time_t t = time(0);

    return (UINT64)t + g_TimeAdd;
}

BOOL CommonFunc::SetCurrTimeAdd(INT64 nTimeAdd)
{
    g_TimeAdd += nTimeAdd;

    return TRUE;
}

UINT64 CommonFunc::GetCurMsTime()
{
    auto time_now = std::chrono::system_clock::now();
    auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
    return duration_in_ms.count() + g_TimeAdd * 1000;
}

tm CommonFunc::GetCurrTmTime(UINT64 uTime)
{
    if (uTime == 0)
    {
        uTime = GetCurrTime();
    }

    time_t t = (time_t)uTime;
    tm _tm_time = *localtime(&t);

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

    //endtime -= (endtime - _timezone) % 86400;

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

UINT64 CommonFunc::GetMonthRemainTime(UINT64 uTime)
{
    if (uTime == 0)
    {
        uTime = GetCurrTime();
    }

    time_t t = (time_t)uTime;
    tm* t_tm = localtime(&t);

    tm newtm;
    newtm.tm_mday = 1;
    newtm.tm_hour = 0;
    newtm.tm_min = 0;
    newtm.tm_sec = 0;

    if (t_tm->tm_mon == 11)
    {
        newtm.tm_year = t_tm->tm_year + 1;
        newtm.tm_mon = 0;
    }
    else
    {
        newtm.tm_year = t_tm->tm_year;
        newtm.tm_mon = t_tm->tm_mon + 1;
    }

    return mktime(&newtm) - t;
}

INT32 CommonFunc::GetWeekDay()
{
    time_t t = (time_t)GetCurrTime();
    tm* t_tm = localtime(&t);
    return t_tm->tm_wday;
}

INT32 CommonFunc::GetMonthDay()
{
    time_t t = (time_t)GetCurrTime();
    tm* t_tm = localtime(&t);
    return t_tm->tm_mday;
}

INT32 CommonFunc::GetMonth(UINT64 uTime /*= 0*/)
{
    if (uTime == 0)
    {
        uTime = GetCurrTime();
    }

    time_t t = (time_t)uTime;
    tm* t_tm = localtime(&t);

    return (t_tm->tm_mon + 1);
}

INT32 CommonFunc::GetTimeZone()
{
#ifdef WIN32
    return _timezone / 3600;
#else
    return timezone / 3600;
#endif
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
    UINT64 DestWeekBegin = (UINT64)t - (t_tmDest.tm_wday == 0 ? 6 : t_tmDest.tm_wday - 1) * 86400 - t_tmDest.tm_hour * 3600 - t_tmDest.tm_min * 60 - t_tmDest.tm_sec;

    return SrcWeekBegin == DestWeekBegin;
}

BOOL CommonFunc::IsSameMonth(UINT64 uTime, UINT64 sTime)
{
    time_t t1 = uTime;
    tm t_tmSrc1 = *localtime(&t1);

    if (sTime == 0)
    {
        sTime = GetCurrTime();
    }

    time_t t2 = sTime;
    tm t_tmSrc2 = *localtime(&t2);

    return (t_tmSrc1.tm_mon == t_tmSrc2.tm_mon);
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
    if (uTimeSrc > uTimeDest)
    {
        std::swap(uTimeSrc, uTimeDest);
    }

    tm tm1 = *localtime(&(time_t)uTimeSrc);
    tm tm2 = *localtime(&(time_t)uTimeDest);

    uTimeSrc = tm1.tm_isdst > 0 ? uTimeSrc = uTimeSrc - 3600 : uTimeSrc;
    uTimeDest = tm2.tm_isdst > 0 ? uTimeDest = uTimeDest + 3600 : uTimeDest;

#ifdef WIN32
    return (INT32)((uTimeDest - _timezone) / 86400 - (uTimeSrc - _timezone) / 86400);
#else
    return (INT32)((uTimeDest - timezone) / 86400 - (uTimeSrc - timezone) / 86400);
#endif
}
