#ifndef _DB_WRITER_H_
#define _DB_WRITER_H_

#include "SharedMemory.h"
#include "CommonThreadFunc.h"
#include "DBManager.h"
#include "../ServerData/GlobalData.h"
#include "../ServerData/BagData.h"
#include "../ServerData/CopyData.h"
#include "../ServerData/EquipData.h"
#include "../ServerData/PetData.h"
#include "../ServerData/PartnerData.h"
#include "../ServerData/GuildData.h"
#include "../ServerData/TaskData.h"
#include "../ServerData/MountData.h"
#include "../ServerData/MailData.h"
#include "../ServerData/RoleData.h"

Th_RetName _DBWriteThread( void* pParam );

class CDBWriterManager
{

public:
	CDBWriterManager();
	~CDBWriterManager();

public:
	BOOL Init();

	BOOL Uninit();

	void WriteWork();

	BOOL IsStop();
public:
	DataWriter<RoleDataObject>* m_pRoleDataWriter;
	DataWriter<GlobalDataObject>* m_pGlobalDataWriter;
	DataWriter<BagDataObject>* m_pBagDataWriter;
	DataWriter<CopyDataObject>* m_pCopyDataWriter;
	DataWriter<EquipDataObject>* m_pEquipDataWriter;
	DataWriter<PetDataObject>* m_pPetDataWriter;
	DataWriter<PartnerDataObject>* m_pPartnerDataWriter;
	DataWriter<GuildDataObject>* m_pGuildDataWriter;
	DataWriter<MemberDataObject>* m_pMemberDataWriter;
	DataWriter<TaskDataObject>* m_pTaskDataWriter;
	DataWriter<MountDataObject>* m_pMountDataWriter;
	DataWriter<MailDataObject>* m_pMailDataWriter;
	DataWriter<GroupMailDataObject>* m_pGroupMailDataWriter;
	THANDLE  m_hWorkThread;

	BOOL	 m_Stop;

	CDBManager m_DBManager;

};

#endif //_DB_WRITER_H_