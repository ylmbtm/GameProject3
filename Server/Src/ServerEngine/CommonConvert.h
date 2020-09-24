#ifndef __COMMON_CONVERT__
#define __COMMON_CONVERT__

namespace CommonConvert
{
INT32 StringToInt(char* pStr);

INT32 StringToInt(const char* pStr);

INT64 StringToInt64(char* pStr);

INT64 StringToInt64(const char* pStr);

FLOAT StringToFloat(char* pStr);

FLOAT StringToFloat(const char* pStr);

DOUBLE StringToDouble(const char* pStr);

std::string DoubleToString(DOUBLE dValue);

std::string IntToString(UINT32 nValue);

std::string IntToString(INT32 nValue);

std::string IntToString(UINT64 nValue);

std::string IntToString(INT64 nValue);

BOOL  StringToPos(char* pStr, FLOAT& x, FLOAT& y, FLOAT& z);

BOOL  StringToBox(char* pStr, FLOAT& left, FLOAT& top, FLOAT& right, FLOAT& bottom);

//浮点到字符串， nPrecision 保留的最大小数的位数， bRound 是否四舍五入
std::string FloatToString(FLOAT fValue, INT32 nPrecision = -1, BOOL bRound = FALSE);

// std::wstring Utf8_To_Unicode(std::string strValue);
//
// std::string  Unicode_To_Uft8(std::wstring wstrValue);
//
// std::wstring Ansi_To_Unicode(std::string strValue);
//
// std::string Unicode_To_Ansi(std::wstring strValue);
//
// std::string Utf8_To_Ansi(std::string strValue);
//
// std::string Ansi_To_Uft8(std::string wstrValue);
//
BOOL IsTextUTF8(const char* str, UINT32 length);

BOOL SpliteString(std::string strSrc,  std::string strDelim, std::vector<std::string>& vtStr);

BOOL SpliteString(std::string strSrc, char cDelim, std::vector<std::string>& vtStr);

BOOL ReplaceString(std::string& str, const std::string& pattern, const std::string& newpat);

BOOL StringToVector(const char* pStrValue, INT32 IntVector[], INT32 nSize, char cDelim = ',');

BOOL StringToVector(const char* pStrValue, FLOAT FloatVector[], INT32 nSize, char cDelim = ',');

UINT32 VersionToInt(const std::string& strVersion);

INT32 CountSymbol(char* pStr, char cSymbol);

BOOL HasSymbol(const char* pStr, const char* pszSymbol);

BOOL StringTrim(std::string& strValue);

BOOL StrCopy(char* pszDest, const char* pszSrc, INT32 nLen);

}


#endif /* __COMMON_CONVERT__ */
