#include "stdafx.h"
#include "ConfigFile.h"

CConfigFile::CConfigFile( void )
{

}

CConfigFile::~CConfigFile( void )
{

}

BOOL CConfigFile::Load( std::string strFileName )
{
	FILE* pFile = fopen(strFileName.c_str(), "r+");

	if(pFile == NULL)
	{
		return FALSE;
	}

	CHAR szBuff[256] = {0};

	do
	{
		fgets(szBuff, 256, pFile);

		if(szBuff[0] == ';')
		{
			continue;
		}

		CHAR* pChar = strchr(szBuff, '=');
		if(pChar == NULL)
		{
			continue;
		}

		std::string strName;
		strName.assign(szBuff, pChar - szBuff);
		std::string strValue = pChar + 1;

		CommonConvert::StringTrim(strName);
		CommonConvert::StringTrim(strValue);

		m_Values.insert(std::make_pair(strName, strValue));

	}
	while(!feof(pFile));

	fclose(pFile);


	return TRUE;
}


CConfigFile* CConfigFile::GetInstancePtr()
{
	static CConfigFile ConfigFile;

	return &ConfigFile;
}

std::string CConfigFile::GetStringValue( std::string strName )
{
	std::map<std::string, std::string>::iterator itor = m_Values.find(strName);
	if(itor != m_Values.end())
	{
		return itor->second;
	}

	CLog::GetInstancePtr()->LogError("无效的配制选项: [%s]", strName.c_str());

	return "";
}

INT32 CConfigFile::GetIntValue( std::string VarName )
{
	return atoi(GetStringValue(VarName).c_str());
}

FLOAT CConfigFile::GetFloatValue( std::string VarName )
{
	return (float)atof(GetStringValue(VarName).c_str());
}

DOUBLE CConfigFile::GetDoubleValue( std::string VarName )
{
	return atof(GetStringValue(VarName).c_str());
}


