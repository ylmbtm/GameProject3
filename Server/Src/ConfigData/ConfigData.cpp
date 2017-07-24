#include <stdafx.h>
#include "ConfigData.h"
#include "Utility\CommonConvert.h"

CConfigData::CConfigData()
{

}

CConfigData::~CConfigData()
{

}

CConfigData* CConfigData::GetInstancePtr()
{
	static CConfigData _StaticMgr;

	return &_StaticMgr;
}

BOOL CConfigData::InitDataReader()
{
	m_vtDataFuncList.push_back(DataFuncNode("Data_Actor", &CConfigData::ReadActor));
	return TRUE;
}

BOOL CConfigData::ReadConfigData(std::string strDbFile)
{
	try
	{
		m_DBConnection.open(strDbFile.c_str());
	}
	catch(CppSQLite3Exception& e)  
	{  
		printf("%s",e.errorMessage());  
		return FALSE;
	}

	char szSql[1024]  = {0};
	for(std::vector<DataFuncNode>::iterator itor = m_vtDataFuncList.begin(); itor != m_vtDataFuncList.end(); itor++)
	{
		DataFuncNode dataNode = (*itor);
		sprintf(szSql, "select * from %s;",dataNode.m_strTbName);
		CppSQLite3Query Tabledatas = m_DBConnection.execQuery(szSql);
		(this->*dataNode.m_pDataFunc)(Tabledatas);
	}

	m_DBConnection.close();

	return TRUE;
}


BOOL CConfigData::ReadConstantValue(CppSQLite3Query &QueryData)
{
	while(QueryData.eof())
	{
		std::string strName = QueryData.getStringField("name");
		std::string strValue = QueryData.getStringField("value");

		if(strName == "xxxxx")
		{
			m_ConstantValue.xxxx = CommonConvert::StringToInt(strValue.c_str());
		}

		QueryData.nextRow();
	}

	return TRUE;
}

UINT64 CConfigData::GetMoneyMaxValue(UINT32 dwMoneyID)
{
	return 0;
}

UINT64 CConfigData::GetActoinMaxValue(UINT32 dwActionID)
{
	return 0;
}

UINT32 CConfigData::GetActoinUnitTime(UINT32 dwActionID)
{
	return 0;
}

BOOL CConfigData::ReadActor(CppSQLite3Query &QueryData)
{
	while(QueryData.eof())
	{
		StActor stValue;
		stValue.dwID = QueryData.getIntField("Id");
		m_mapActor.insert(std::make_pair(stValue.dwID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

StActor* CConfigData::GetActorInfo(UINT32 dwActorID)
{
	std::map<UINT32, StActor>::iterator itor = m_mapActor.find(dwActorID);
	if(itor != m_mapActor.end())
	{
		return &itor->second;
	}

	return NULL;
}

BOOL CConfigData::ReadCopyBase(CppSQLite3Query &QueryData)
{
	while(QueryData.eof())
	{
		StCopyBase stValue;
		stValue.dwTypeID = QueryData.getIntField("Id");
		/*   stValue.dwBattleTimes = QueryData.getIntField("Id"); 
		stValue.dwCostActID = QueryData.getIntField("Id");
		stValue.dwCostActNum = QueryData.getIntField("Id");*/
        stValue.dwLogicType = QueryData.getIntField("Id");

		m_mapCopybase.insert(std::make_pair(stValue.dwTypeID, stValue));
		
		QueryData.nextRow();
	}

	return TRUE;
}

StCopyBase* CConfigData::GetCopyBaseInfo(UINT32 dwCopyType)
{
	std::map<UINT32, StCopyBase>::iterator itor = m_mapCopybase.find(dwCopyType);
	if(itor != m_mapCopybase.end())
	{
		return &itor->second;
	}

	return NULL;
}

BOOL CConfigData::ReadLanguage(CppSQLite3Query &QueryData)
{
	while(QueryData.eof())
	{
		StLocalString stValue;
		stValue.dwID = QueryData.getIntField("id");
        stValue.Language[0] = QueryData.getIntField("lang0");
        stValue.Language[1] = QueryData.getIntField("lang1");
        stValue.Language[2] = QueryData.getIntField("lang2");
        stValue.Language[3] = QueryData.getIntField("lang3");
        stValue.Language[4] = QueryData.getIntField("lang4");
        stValue.Language[5] = QueryData.getIntField("lang5");
        stValue.Language[6] = QueryData.getIntField("lang6");
        stValue.Language[7] = QueryData.getIntField("lang7");
        stValue.Language[8] = QueryData.getIntField("lang8");
        stValue.Language[9] = QueryData.getIntField("lang9");
        stValue.Language[10] = QueryData.getIntField("lang10");
        stValue.Language[11] = QueryData.getIntField("lang11");
        stValue.Language[12] = QueryData.getIntField("lang12");
        stValue.Language[13] = QueryData.getIntField("lang13");
        stValue.Language[14] = QueryData.getIntField("lang14");
		m_mapLocalString.insert(std::make_pair(stValue.dwID, stValue));
		QueryData.nextRow();
	}

	return TRUE;
}

std::string& CConfigData::GetLanguageText( UINT32 dwID, UINT32 dwLang )
{
    if((dwLang < 0)||(dwLang > 14))
    {
        return m_strNull;
    }

    std::map<UINT32, StLocalString>::iterator itor = m_mapLocalString.find(dwID);
    if(itor != m_mapLocalString.end())
    {
        return itor->second.Language[dwLang];
    }

    return m_strNull;
}
