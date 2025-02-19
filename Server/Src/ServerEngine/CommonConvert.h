#ifndef __COMMON_CONVERT__
#define __COMMON_CONVERT__

namespace CommonConvert
{
INT32 StringToInt(const char* pStr);

INT64 StringToInt64(const char* pStr);

FLOAT StringToFloat(const char* pStr);

DOUBLE StringToDouble(const char* pStr);

std::string DoubleToString(DOUBLE dValue);

std::string IntToString(UINT32 nValue);

std::string IntToString(INT32 nValue);

std::string IntToString(UINT64 nValue);

std::string IntToString(INT64 nValue);

std::string FloatToString(FLOAT fValue, INT32 nPrecision = -1, BOOL bRound = FALSE);

BOOL  StringToPos(const char* pStr, FLOAT& x, FLOAT& y, FLOAT& z);

BOOL  StringToBox(const char* pStr, FLOAT& left, FLOAT& top, FLOAT& right, FLOAT& bottom);

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
std::string Ansi_To_Uft8(std::string strValue);

BOOL IsTextUTF8(const char* str, INT32 nLength);

INT32 GetValidUtf8Length(char* pStr, INT32 nLen);

std::string TruncateUtf8(char* pStr, INT32 nLen);

BOOL SpliteString(std::string strSrc,  std::string strDelim, std::vector<std::string>& vtStr);

BOOL SpliteStringByMuti(std::string strSrc, std::string strDelim, std::vector<std::string>& vtStr);

BOOL SpliteStringByBlank(std::string strSrc, std::vector<std::string>& vtStr);

BOOL SpliteString(std::string strSrc, char cDelim, std::vector<std::string>& vtStr);

BOOL ReplaceString(std::string& strSrc, const std::string& pattern, const std::string& newpat);

BOOL StringToVector(const char* pStrValue, std::vector<INT32>& vtInt, char cDelim = ',');

BOOL StringToVector(const char* pStrValue, INT32 IntVector[], INT32 nSize, char cDelim = ',');

BOOL StringToVector(const char* pStrValue, FLOAT FloatVector[], INT32 nSize, char cDelim = ',');

INT32 VersionToInt(const std::string& strVersion);

INT32 CountSymbol(const char* pStr, char cSymbol);

BOOL HasSymbol(const char* pStr, const char* pszSymbol);

BOOL StringTrim(std::string& strValue);

BOOL StrCopy(char* pszDest, const char* pszSrc, INT32 nLen);

BOOL EscapeString(char* pszDest, INT32 nLen);

BYTE FromHex(const BYTE& x);

std::string UrlDecode(const std::string& strIn);
}


#endif /* __COMMON_CONVERT__ */
