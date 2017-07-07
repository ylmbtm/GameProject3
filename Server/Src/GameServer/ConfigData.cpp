#include <stdafx.h>
#include "ConfigData.h"

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
	m_vtDataFuncList.push_back(DataFuncNode("sss", &CConfigData::ReadRoleType));
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


BOOL CConfigData::ReadRoleType(CppSQLite3Query &QueryData)
{
	return TRUE;
}

BOOL CConfigData::ReadCopyBase(CppSQLite3Query &QueryData)
{
	while(QueryData.eof())
	{
		StCopyBase stValue;
		stValue.dwTypeID = QueryData.getIntField("id");

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

		m_mapLocalString.insert(std::make_pair(stValue.dwID, stValue));

		QueryData.nextRow();
	}

	return TRUE;
}
