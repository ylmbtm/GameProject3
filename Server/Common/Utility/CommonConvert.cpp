#include "stdafx.h"
#include "CommonConvert.h"


INT32 CommonConvert::StringToInt(char *pStr)
{
	if(pStr == NULL)
	{
		return 0;
	}

	return atoi(pStr);
}

FLOAT  CommonConvert::StringToFloat(char *pStr)
{
	if(pStr == NULL)
	{
		return 0;
	}

	return (FLOAT)atof(pStr);
}

std::string CommonConvert::IntToString(INT32 nValue)
{
	CHAR szValue[64]={0};

	sprintf(szValue, "%d", nValue);

	return std::string(szValue);
}

std::string CommonConvert::FloatToString(FLOAT fValue, INT32 nPrecision, BOOL bRound)
{
	CHAR szValue[64]={0};

	if((bRound)&&(nPrecision>0)&&(nPrecision<6))
	{
		FLOAT fRoundValue = 5;

		for(int i = 0; i < nPrecision+1; i++)
		{
			fRoundValue *= 0.1f;
		}

		fValue += fRoundValue;
	}
	
	sprintf(szValue, "%f", fValue);

	CHAR *pChar = strchr(szValue,'.');
	if(pChar == NULL)
	{
		return std::string(szValue);
	}

	*(pChar+nPrecision+1) = '\0';

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
