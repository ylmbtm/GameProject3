#ifndef _DB_WRITER_H_
#define _DB_WRITER_H_

#include "SharedMemory.h"
#include "..\ServerData\RoleData.h"
#include "Utility\CommonThreadFunc.h"
#include "DBManager.h"
#include "..\ServerData\GlobalData.h"
#include "..\ServerData\BagData.h"
#include "..\ServerData\CopyData.h"

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
	DataWriter<GlobalDataObject> *m_pGlobalDataWriter;
	DataWriter<BagDataObject> *m_pBagDataWriter;
	DataWriter<CopyDataObject> *m_pCopyDataWriter;
	BOOL IsStop();
public:

	THANDLE  m_hWorkThread;

	BOOL	 m_Stop;

	CDBManager m_DBManager;
	
};

#endif //_DB_WRITER_H_