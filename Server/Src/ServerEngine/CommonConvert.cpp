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

	sprintf(szValue, "%d", nValue);

	return std::string(szValue);
}

BOOL CommonConvert::StringToPos(char* pStr, FLOAT& x, FLOAT& y, FLOAT& z)
{
	if(pStr == NULL)
	{
		return FALSE;
	}

	char* pPos = strchr(pStr, ',');
	if(pPos == NULL)
	{
		return FALSE;
	}
	*pPos = 0;
	x = CommonConvert::StringToFloat(pStr + 1);

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

	sprintf(szValue, "%f", fValue);

	CHAR* pChar = strchr(szValue, '.');
	if(pChar == NULL)
	{
		return std::string(szValue);
	}

	*(pChar + nPrecision + 1) = '\0';

	return std::string(szValue);
}

std::wstring CommonConvert::Utf8ToUnicode( std::string strValue )
{
	std::wstring strResult;


	return strResult;
}

std::string CommonConvert::UnicodeToUft8( std::wstring wstrValue )
{
	std::string strResult;


	return strResult;
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
