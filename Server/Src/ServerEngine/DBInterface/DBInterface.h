#ifndef __DB_INTERFACE_H__
#define __DB_INTERFACE_H__

class CDBStoredProcedure;

struct IDBInterface
{
	virtual BOOL Execute(std::string sql) = 0;
	virtual BOOL Query(std::string sql) = 0;
	virtual BOOL Execute(CDBStoredProcedure* pSpc) = 0;
	virtual BOOL Query(CDBStoredProcedure* pSpc) = 0;
};

/*
m_DBConnection.Init();

m_DBConnection.Connect("127.0.0.1","root","123456", "db_log", 3306);
m_DBProceduceMgr.InitStoredProcedures();
CDBStoredProcedure *pProcedure = NULL;

pProcedure = m_DBProceduceMgr.GetStoredProcedure(DB_INSERT_PLAYER_INFO);
pProcedure->set_int8(0, 10);
pProcedure->set_string(1, "test", 5);
pProcedure->set_int32(2, 10);
pProcedure->set_int32(3, 10);
m_DBConnection.Execute(pProcedure);

while(pProcedure->m_DBRecordSet.MoveNext())
{
	int nValue = pProcedure->m_DBRecordSet.get_int32((size_t)0);
	printf("%d", nValue);
}

pProcedure->m_DBRecordSet.ClearRecordSet();

//pProcedure->m_DBRecordSet.MoveNext(0);



/*
pProcedure = m_DBProceduceMgr.GetStoredProcedure(DB_FIND_PLAYER_INFO);
pProcedure->set_int32(0, 2);
m_DBConnection.Query(pProcedure);


int n = pProcedure->m_DBRecordSet.GetRowCount();

while(pProcedure->m_DBRecordSet.MoveNext())
{
	int nValue = pProcedure->m_DBRecordSet.get_int32("id");
	nValue = pProcedure->m_DBRecordSet.get_int32("fb");
	nValue = pProcedure->m_DBRecordSet.get_int32("fi");
	char * p = pProcedure->m_DBRecordSet.get_string("fc");

	printf(p);
}
*/

#endif //__DB_INTERFACE_H__