#pragma once
#include <string>
#include "Sqlite/CppSQLite3.h"
#include <vector>

#define BEGIN_LOAD_DATA \
	BOOL ProcessTable(std::string strTableName, CppSQLite3Query &TableData) \
	{

#define END_LOAD_DATA \
	return TRUE;\
	}

#define LOAD_DB_DATA(tbname, func) \
	if(strTableName == #tbname) \
	{ \
	while(!TableData.eof()) \
		{\
		func(TableData);\
		\
		TableData.nextRow();\
		}\
	}


class CStaticDataMgr
{
public:
	CStaticDataMgr(void);
	virtual ~CStaticDataMgr(void);

	BOOL	Load(std::string strDbDir);

	BOOL	Release();

public:
	BOOL	ProcessDBFile(std::string strDbFile);

	BOOL	GetFileList(std::string strDbDir, std::vector<std::string> &vtFileList);

protected:
	CppSQLite3DB	m_DBConnection;

public:
	BEGIN_LOAD_DATA
		//LOAD_DB_DATA(t_accountinfo, LoadAccountData)
	END_LOAD_DATA

public:
	//例子
	//BOOL	LoadAccountData(CppSQLite3Query &QueryData);


};
