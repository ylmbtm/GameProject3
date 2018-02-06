#include "stdafx.h"
#include "CommandLine.h"

CCommandLine::CCommandLine(INT32 argc, CHAR* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		CHAR* pChar = strchr(argv[i], '=');
		if (pChar == NULL)
		{
			continue;
		}

		std::string strName;
		strName.assign(argv[i], pChar - argv[i]);
		std::string strValue = pChar + 1;

		CommonConvert::StringTrim(strName);
		CommonConvert::StringTrim(strValue);

		m_ParameterMap.insert(std::make_pair(strName, strValue));
	}
}

CCommandLine::~CCommandLine()
{
	m_ParameterMap.clear();
}

INT32 CCommandLine::GetIntValue(const std::string& strKey) const
{
	auto itor = m_ParameterMap.find(strKey);
	if(itor != m_ParameterMap.end())
	{
		return CommonConvert::StringToInt(itor->second.c_str());
	}
	return 0;
}

std::string CCommandLine::GetStrValue(const std::string& strKey) const
{
	auto it = m_ParameterMap.find(strKey);
	if(it != m_ParameterMap.end())
	{
		return it->second;
	}
	return "";
}

INT64 CCommandLine::GetLongValue(const std::string& strKey) const
{
	auto it = m_ParameterMap.find(strKey);
	if(it != m_ParameterMap.end())
	{
		return CommonConvert::StringToInt64(it->second.c_str());
	}

	return 0;
}

FLOAT CCommandLine::GetFloatValue(const std::string& strKey) const
{
	auto it = m_ParameterMap.find(strKey);
	if(it != m_ParameterMap.end())
	{
		return CommonConvert::StringToFloat(it->second.c_str());
	}
	return 0.0f;
}




