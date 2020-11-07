#include "stdafx.h"
#include "CommonConvert.h"

INT32 CommonConvert::StringToInt(char* pStr)
{
	if(pStr == NULL)
	{
		return 0;
	}

	return atoi(pStr);
}

INT64 CommonConvert::StringToInt64(char* pStr)
{
	if(pStr == NULL)
	{
		return 0;
	}

#ifdef WIN32
	return _atoi64(pStr);
#else
	return atoll(pStr);
#endif
}

INT64 CommonConvert::StringToInt64(const char* pStr)
{
	if(pStr == NULL)
	{
		return 0;
	}


#ifdef WIN32
	return _atoi64(pStr);
#else
	return atoll(pStr);
#endif
}

INT32 CommonConvert::StringToInt(const char* pStr)
{
	if(pStr == NULL)
	{
		return 0;
	}

	return atoi(pStr);
}

FLOAT  CommonConvert::StringToFloat(char* pStr)
{
	if(pStr == NULL)
	{
		return 0;
	}

	return (FLOAT)atof(pStr);
}

DOUBLE CommonConvert::StringToDouble(const char* pStr)
{
	if (pStr == NULL)
	{
		return 0;
	}

	return strtod(pStr, NULL);
}

std::string CommonConvert::DoubleToString(DOUBLE dValue)
{
	CHAR szValue[64] = { 0 };

	snprintf(szValue, 64, "%f", dValue);

	return std::string(szValue);
}

FLOAT  CommonConvert::StringToFloat(const char* pStr)
{
	if(pStr == NULL)
	{
		return 0;
	}

	return (FLOAT)atof(pStr);
}

std::string CommonConvert::IntToString(INT32 nValue)
{
	CHAR szValue[64] = {0};

	snprintf(szValue, 64, "%d", nValue);

	return std::string(szValue);
}

std::string CommonConvert::IntToString(INT64 uValue)
{
	CHAR szValue[64] = { 0 };

	snprintf(szValue, 64, "%lld", uValue);

	return std::string(szValue);
}

std::string CommonConvert::IntToString(UINT32 nValue)
{
	CHAR szValue[64] = { 0 };

	snprintf(szValue, 64, "%d", nValue);

	return std::string(szValue);
}

std::string CommonConvert::IntToString(UINT64 uValue)
{
	CHAR szValue[64] = { 0 };

	snprintf(szValue, 64, "%lld", uValue);

	return std::string(szValue);
}

BOOL CommonConvert::StringToPos(char* pStr, FLOAT& x, FLOAT& y, FLOAT& z)
{
	if(pStr == NULL)
	{
		return FALSE;
	}

	char szTempBuf[256] = { 0 };
	strncpy(szTempBuf, pStr, strlen(pStr));

	char* pPos = strchr(szTempBuf, ',');
	if(pPos == NULL)
	{
		return FALSE;
	}
	*pPos = 0;
	x = CommonConvert::StringToFloat(szTempBuf + 1);

	char* pOldPos = pPos + 1;
	pPos = strchr(pPos + 1, ',');
	if(pPos == NULL)
	{
		return FALSE;
	}
	*pPos = 0;
	y = CommonConvert::StringToFloat(pOldPos);

	pOldPos = pPos + 1;
	pPos = strchr(pPos + 1, ')');
	if(pPos == NULL)
	{
		return FALSE;
	}
	*pPos = 0;
	z = CommonConvert::StringToFloat(pOldPos);

	return TRUE;
}

BOOL CommonConvert::StringToBox(char* pStr, FLOAT& left, FLOAT& top, FLOAT& right, FLOAT& bottom)
{
	if (pStr == NULL)
	{
		return FALSE;
	}

	char szTempBuf[256] = { 0 };
	strncpy(szTempBuf, pStr, strlen(pStr));

	char* pPos = strchr(szTempBuf, ',');
	if (pPos == NULL)
	{
		return FALSE;
	}
	*pPos = 0;
	left = CommonConvert::StringToFloat(szTempBuf + 1);

	char* pOldPos = pPos + 1;
	pPos = strchr(pPos + 1, ',');
	if (pPos == NULL)
	{
		return FALSE;
	}
	*pPos = 0;
	top = CommonConvert::StringToFloat(pOldPos);

	pOldPos = pPos + 1;
	pPos = strchr(pPos + 1, ',');
	if (pPos == NULL)
	{
		return FALSE;
	}
	*pPos = 0;
	right = CommonConvert::StringToFloat(pOldPos);

	pOldPos = pPos + 1;
	pPos = strchr(pPos + 1, ')');
	if (pPos == NULL)
	{
		return FALSE;
	}
	*pPos = 0;
	bottom = CommonConvert::StringToFloat(pOldPos);

	return TRUE;
}

std::string CommonConvert::FloatToString(FLOAT fValue, INT32 nPrecision, BOOL bRound)
{
	CHAR szValue[64] = {0};

	if((bRound) && (nPrecision > 0) && (nPrecision < 6))
	{
		FLOAT fRoundValue = 5;

		for(int i = 0; i < nPrecision + 1; i++)
		{
			fRoundValue *= 0.1f;
		}

		fValue += fRoundValue;
	}

	snprintf(szValue, 64, "%f", fValue);

	CHAR* pChar = strchr(szValue, '.');
	if(pChar == NULL)
	{
		return std::string(szValue);
	}

	*(pChar + nPrecision + 1) = '\0';

	return std::string(szValue);
}

BOOL CommonConvert::SpliteString(std::string strSrc, std::string strDelim, std::vector<std::string>& vtStr)
{
	vtStr.clear();
	if (strDelim.empty())
	{
		vtStr.push_back(strSrc);
		return TRUE;
	}

	std::string::iterator subStart, subEnd;
	subStart = strSrc.begin();
	while (true)
	{
		subEnd = std::search(subStart, strSrc.end(), strDelim.begin(), strDelim.end());
		std::string temp(subStart, subEnd);
		if (!temp.empty())
		{
			vtStr.push_back(temp);
		}
		if (subEnd == strSrc.end())
		{
			break;
		}
		subStart = subEnd + strDelim.size();
	}

	return TRUE;
}

BOOL CommonConvert::ReplaceString(std::string& str, const std::string& pattern, const std::string& newpat)
{
	const size_t nsize = newpat.size();
	const size_t psize = pattern.size();

	for (size_t pos = str.find(pattern, 0); pos != std::string::npos; pos = str.find(pattern, pos + nsize))
	{
		str.replace(pos, psize, newpat);
	}

	return TRUE;
}

BOOL CommonConvert::StringToVector(const char* pStrValue, INT32 IntVector[], INT32 nSize, char cDelim)
{
	if (pStrValue == NULL)
	{
		return FALSE;
	}

	char szBuf[1024] = { 0 };
	strncpy(szBuf, pStrValue, 1024);

	char* pBeginPos = szBuf;
	char* pEndPos = strchr(pBeginPos, cDelim);

	if (pBeginPos == pEndPos)
	{
		pBeginPos += 1;
		pEndPos = strchr(pBeginPos, cDelim);
	}

	INT32 nIndex = 0;
	while (pEndPos != NULL)
	{
		//*pEndPos = 0;
		IntVector[nIndex++] = StringToInt(pBeginPos);
		if (nIndex >= nSize)
		{
			return TRUE;
		}

		pBeginPos = pEndPos + 1;
		pEndPos = strchr(pBeginPos, cDelim);
	}

	if (*pBeginPos != 0 && nIndex < nSize)
	{
		IntVector[nIndex++] = StringToInt(pBeginPos);
	}

	return TRUE;
}

BOOL CommonConvert::StringToVector(const char* pStrValue, FLOAT FloatVector[], INT32 nSize, char cDelim /*= ','*/)
{
	if (pStrValue == NULL)
	{
		return FALSE;
	}

	char szBuf[1024] = { 0 };
	strncpy(szBuf, pStrValue, 1024);

	char* pBeginPos = szBuf;
	char* pEndPos = strchr(pBeginPos, cDelim);

	if (pBeginPos == pEndPos)
	{
		pBeginPos += 1;
		pEndPos = strchr(pBeginPos, cDelim);
	}

	INT32 nIndex = 0;
	while (pEndPos != NULL)
	{
		FloatVector[nIndex++] = StringToFloat(pBeginPos);
		if (nIndex >= nSize)
		{
			return TRUE;
		}

		pBeginPos = pEndPos + 1;
		pEndPos = strchr(pBeginPos, cDelim);
	}

	if (*pBeginPos != 0 && nIndex < nSize)
	{
		FloatVector[nIndex++] = StringToFloat(pBeginPos);
	}

	return TRUE;
}

BOOL CommonConvert::SpliteString(std::string strSrc,  char cDelim, std::vector<std::string>& vtStr)
{
	vtStr.clear();

	std::string::size_type posStart, posEnd;
	posEnd = strSrc.find(cDelim);
	posStart = 0;
	while(std::string::npos != posEnd)
	{
		vtStr.emplace_back(strSrc.substr(posStart, posEnd - posStart));

		posStart = posEnd + 1;
		posEnd = strSrc.find(cDelim, posStart);
	}

	if(posStart != strSrc.length())
	{
		vtStr.emplace_back(strSrc.substr(posStart));
	}

	return TRUE;
}

/*
std::wstring CommonConvert::Utf8_To_Unicode( std::string strSrc )
{
#ifdef WIN32
	wchar_t wBuff[102400] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), -1, wBuff, 102400);
	std::wstring strRet = wBuff;
	return strRet;
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(strSrc.c_str());
#endif
}

std::string CommonConvert::Unicode_To_Uft8( std::wstring wstrValue )
{
#ifdef WIN32
	CHAR sBuff[102400] = { 0 };
	WideCharToMultiByte(CP_UTF8, 0, wstrValue.c_str(), -1, sBuff, 102400, NULL, NULL);
	std::string strRet = sBuff;
	return strRet;
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstrValue.c_str());

#endif
}

std::wstring CommonConvert::Ansi_To_Unicode( std::string strSrc )
{
	wchar_t wBuff[102400] = {0};
#ifdef WIN32
	MultiByteToWideChar(CP_ACP,  0, strSrc.c_str(), -1, wBuff, 102400);
	std::wstring strRet = wBuff;
	return strRet;
#else
	setlocale(LC_CTYPE, "chs");
	std::mbstowcs(wBuff, strSrc.c_str(), 102400);
#endif
}

std::string CommonConvert::Unicode_To_Ansi( std::wstring strValue )
{
	CHAR sBuff[102400] = { 0 };
#ifdef WIN32
	WideCharToMultiByte(CP_ACP, 0, strValue.c_str(), -1, sBuff, 102400, NULL, NULL);
	return std::string(sBuff);
#else
	setlocale(LC_CTYPE, "chs");
	std::wcstombs(sBuff, strValue.c_str(), 102400);
#endif
}

std::string CommonConvert::Utf8_To_Ansi( std::string strSrc )
{
	wchar_t wBuff[102400] = {0};
	CHAR sBuff[102400] = {0};
#ifdef WIN32
	MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), -1, wBuff, 102400);
	WideCharToMultiByte(CP_ACP, 0, wBuff, -1, sBuff, 102400, NULL, NULL);
	std::string strRet = sBuff;
	return strRet;
#else

#endif
}

std::string CommonConvert::Ansi_To_Uft8( std::string strSrc )
{
	wchar_t wBuff[102400] = {0};
	CHAR sBuff[102400] = {0};
#ifdef WIN32
	MultiByteToWideChar(CP_ACP,  0, strSrc.c_str(), -1, wBuff, 102400);
	WideCharToMultiByte(CP_UTF8, 0, wBuff, -1, sBuff, 102400, NULL, NULL);
	std::string strRet = sBuff;
	return strRet;
#else
#endif
}
*/

BOOL CommonConvert::IsTextUTF8(const char* str, UINT32 length)
{
	UINT32 i;
	UINT32 nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	UINT8 chr;
	BOOL bAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8
	for (i = 0; i < length; i++)
	{
		chr = *(str + i);
		if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
		{
			bAllAscii = FALSE;
		}
		if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
				{
					nBytes = 6;
				}
				else if (chr >= 0xF8)
				{
					nBytes = 5;
				}
				else if (chr >= 0xF0)
				{
					nBytes = 4;
				}
				else if (chr >= 0xE0)
				{
					nBytes = 3;
				}
				else if (chr >= 0xC0)
				{
					nBytes = 2;
				}
				else
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if ((chr & 0xC0) != 0x80)
			{
				return FALSE;
			}
			nBytes--;
		}
	}
	if (nBytes > 0) //违返规则
	{
		return FALSE;
	}
	if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
	{
		return TRUE;
	}
	return TRUE;
}


UINT32 CommonConvert::VersionToInt(const std::string& strVersion )
{
	INT32 nValue[3] = { 0 };
	StringToVector(strVersion.c_str(), nValue, 3, '.');
	return nValue[0] * 1000000 + nValue[1] * 1000 + nValue[2];
}

INT32 CommonConvert::CountSymbol(char* pStr, char cSymbol )
{
	if(pStr == NULL)
	{
		return 0;
	}

	INT32 nCount = 0;

	char* pTemp = pStr;
	while(*pTemp != '\0')
	{
		if(*pTemp == cSymbol)
		{
			nCount += 1;
		}

		pTemp += 1;
	}

	return nCount;
}

BOOL CommonConvert::HasSymbol(const char* pStr, const char* pszSymbol)
{
	if (pStr == NULL || pszSymbol == NULL)
	{
		return FALSE;
	}

	const char* pSym = pszSymbol;
	while (*pSym != '\0')
	{
		const char* pTemp = pStr;
		while (*pTemp != '\0')
		{
			if (*pTemp == *pSym)
			{
				return TRUE;
			}

			pTemp += 1;
		}

		pSym += 1;
	}

	return FALSE;
}

BOOL CommonConvert::StringTrim(std::string& strValue)
{
	if(!strValue.empty())
	{
		strValue.erase(0, strValue.find_first_not_of((" \n\r\t")));
		strValue.erase(strValue.find_last_not_of((" \n\r\t")) + 1);
	}
	return TRUE;
}

BOOL CommonConvert::StrCopy(char* pszDest, const char* pszSrc, INT32 nLen)
{
	if (pszDest == NULL || pszSrc == NULL)
	{
		return FALSE;
	}

	if (nLen <= 0)
	{
		return FALSE;
	}

	strncpy(pszDest, pszSrc, nLen - 1);

	if (strlen(pszSrc) >= nLen)
	{
		return FALSE;
	}

	return TRUE;
}
