#ifndef _DB_WRITER_MANAGER_H_
#define _DB_WRITER_MANAGER_H_

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
#include "../ServerData/ActivityData.h"
#include "../ServerData/CounterData.h"
#include "../ServerData/FriendData.h"
#include "../ServerData/GemData.h"
#include "../ServerData/SkillData.h"
#include "DBInterface/DBConnection.h"

class CDBWriterManager
{

public:
	CDBWriterManager();
	~CDBWriterManager();

public:
	BOOL Init();

	BOOL Uninit();

	BOOL WriteDataToDB();

	BOOL IsStop();

	void DBWriteThread();
public:
	std::vector<DataWriterBase*> m_vtDataWriters;

	BOOL			m_Stop;
	std::thread*	m_pWorkThread;
	CDBConnection   m_DBConnection;
};

#endif //_DB_WRITER_MANAGER_H_