#include "stdafx.h"
#include "DBStoredProcMgr.h"

CDBStoredProcedureMgr::CDBStoredProcedureMgr()
{

}

CDBStoredProcedureMgr::~CDBStoredProcedureMgr()
{

}


BOOL CDBStoredProcedureMgr::InitStoredProcedures()
{
	m_StoredProcedures.assign(MAX_DBServerDB_STATEMENTS, NULL);

	m_StoredProcedures[DB_INSERT_PLAYER_INFO] = new CDBStoredProcedure("CALL addrecord(?,?,?,?)", 4);
// 	m_StoredProcedures[DB_INSERT_PLAYER_INFO]->m_DBRecordSet.SetFieldNum(6);
// 	m_StoredProcedures[DB_INSERT_PLAYER_INFO]->m_DBRecordSet.SetFieldType(0, MYSQL_TYPE_FLOAT);


	m_StoredProcedures[DB_FIND_PLAYER_INFO] = new CDBStoredProcedure("CALL selectrecord(?)", 1);
// 	m_StoredProcedures[DB_FIND_PLAYER_INFO]->m_DBRecordSet.SetFieldNum(4);
// 	m_StoredProcedures[DB_FIND_PLAYER_INFO]->m_DBRecordSet.SetFieldType(0, MYSQL_TYPE_INT24);
// 	m_StoredProcedures[DB_FIND_PLAYER_INFO]->m_DBRecordSet.SetFieldType(1, MYSQL_TYPE_TINY);
// 	m_StoredProcedures[DB_FIND_PLAYER_INFO]->m_DBRecordSet.SetFieldType(2, MYSQL_TYPE_INT24);
// 	m_StoredProcedures[DB_FIND_PLAYER_INFO]->m_DBRecordSet.SetFieldType(3, MYSQL_TYPE_VAR_STRING);
	

	/*m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();*/

	return TRUE;
}

CDBStoredProcedure* CDBStoredProcedureMgr::GetStoredProcedure( DBStoredProcedureID SpID )
{
	if((SpID < 0)&&(SpID >= MAX_DBServerDB_STATEMENTS))
	{
		return NULL;
	}

	return m_StoredProcedures[SpID];
}

