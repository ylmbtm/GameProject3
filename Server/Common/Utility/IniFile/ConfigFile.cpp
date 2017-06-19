#include "stdafx.h"
#include "ConfigFile.h"


CConfigFile::CConfigFile( void )
{

}

CConfigFile::~CConfigFile( void )
{

}

inline static BOOL StringTrim(std::string& text)  
{  
	if(!text.empty())  
	{  
		text.erase(0, text.find_first_not_of((" \n\r\t")));  
		text.erase(text.find_last_not_of((" \n\r\t")) + 1);  
	}  
	return TRUE;  
}  

BOOL CConfigFile::Load( std::string strFileName )
{
	FILE *pFile = fopen(strFileName.c_str(),"r+");

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
		
		CHAR *pChar = strchr(szBuff,'=');
		if(pChar == NULL)
		{
			continue;
		}

		std::string strName;
		strName.assign(szBuff,pChar-szBuff);
		std::string strValue = pChar+1;

		StringTrim(strName);
		StringTrim(strValue);

		m_Values.insert(std::make_pair(strName, strValue));

	}while(!feof(pFile));
	
	fclose(pFile);


	return TRUE;
}


CConfigFile* CConfigFile::GetInstancePtr()
{
	static CConfigFile ConfigFile;

	return &ConfigFile;
}


