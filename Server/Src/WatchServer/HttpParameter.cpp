
#include "stdafx.h"
#include "HttpParameter.h"
#include "../ServerEngine/CommonConvert.h"


HttpParameter::HttpParameter(const std::string& strParam)
{
	ParseStringToMap(strParam);
}


HttpParameter::~HttpParameter(void)
{
}

void HttpParameter::ParseStringToMap(const std::string& strParam)
{
	if(strParam.length() <= 0)
	{
		return;
	}
	std::string str = strParam.substr(1);
	std::vector<std::string> strVector;
	CommonConvert::SpliteString(str, "&", strVector);
	std::vector<std::string>::iterator itend = strVector.end();
	for(std::vector<std::string>::iterator it = strVector.begin(); it != itend; it++)
	{
		std::vector<std::string> strVectorSub;
		CommonConvert::SpliteString(*it, "=", strVectorSub);
		if((strVectorSub.begin() + 1) != strVectorSub.end())
		{
			m_ParameterMap.insert(std::make_pair(strVectorSub[0], strVectorSub[1]));
		}
	}
}

bool HttpParameter::HasKey(const std::string& strKey) const
{
	auto itor = m_ParameterMap.find(strKey);
	if(itor != m_ParameterMap.end())
	{
		return true;
	}

	return false;
}

bool HttpParameter::GetIntValue(const std::string& strKey, INT32& intValue) const
{
	auto itor = m_ParameterMap.find(strKey);
	if(itor != m_ParameterMap.end())
	{
		intValue = CommonConvert::StringToInt(itor->second.c_str());
		return true;
	}
	return false;
}

bool HttpParameter::GetStrValue(const std::string& strKey, std::string& strValue) const
{
	auto it = m_ParameterMap.find(strKey);
	if(it != m_ParameterMap.end())
	{
		strValue = (it->second);
		return true;
	}
	return false;
}

bool HttpParameter::GetLongValue(const std::string& strKey, INT64& longValue) const
{
	auto it = m_ParameterMap.find(strKey);
	if(it != m_ParameterMap.end())
	{
		longValue = CommonConvert::StringToInt64(it->second.c_str());
		return true;
	}
	return false;
}

bool HttpParameter::GetFloatValue(const std::string& strKey, FLOAT& floatValue) const
{
	auto it = m_ParameterMap.find(strKey);
	if(it != m_ParameterMap.end())
	{
		floatValue = CommonConvert::StringToFloat(it->second.c_str());
		return true;
	}
	return false;
}

