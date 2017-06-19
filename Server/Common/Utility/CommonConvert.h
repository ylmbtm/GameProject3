#ifndef __COMMON_CONVERT__
#define __COMMON_CONVERT__

namespace CommonConvert
{
	INT32 StringToInt(char *pStr);

	FLOAT StringToFloat(char *pStr);
	
	std::string IntToString(INT32 nValue);
	
	//浮点到字符串， nPrecision 保留的最大小数的位数， bRound 是否四舍五入
	std::string FloatToString(FLOAT fValue, INT32 nPrecision = -1, BOOL bRound = FALSE);

	std::wstring Utf8ToUnicode(std::string strValue);

	std::string  UnicodeToUft8(std::wstring wstrValue);
}


#endif /* __COMMON_CONVERT__ */
