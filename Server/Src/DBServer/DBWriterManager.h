#ifndef _DB_WRITER_H_
#define _DB_WRITER_H_

#include "SharedMemory.h"
#include "..\ServerData\RoleData.h"
#include "Utility\CommonThreadFunc.h"
#include "DBManager.h"

Th_RetName _DBWriteThread( void *pParam );

class CDBWriterManager
{

public:
	CDBWriterManager();
	~CDBWriterManager();

public:
	BOOL Init();
	
	BOOL Uninit();

	void WriteWork();

	DataWriter<RoleDataObject> *m_pRoleDataWriter;

	BOOL IsStop();
public:

	THANDLE  m_hWorkThread;

	BOOL	 m_Stop;

	CDBManager m_DBManager;
	
};

#endif //_DB_WRITER_H_