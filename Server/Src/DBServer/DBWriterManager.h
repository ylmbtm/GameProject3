#ifndef _DB_WRITER_MANAGER_H_
#define _DB_WRITER_MANAGER_H_

#include "DBInterface/DBConnection.h"
#include "GlobalData.h"
#include "BagData.h"
#include "CopyData.h"
#include "EquipData.h"
#include "PetData.h"
#include "PartnerData.h"
#include "GuildData.h"
#include "TaskData.h"
#include "MountData.h"
#include "MailData.h"
#include "RoleData.h"
#include "ActivityData.h"
#include "CounterData.h"
#include "FriendData.h"
#include "GemData.h"
#include "SkillData.h"
#include "PayData.h"
#include "SealData.h"

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

    BOOL Update();

    void DBWriteThread();
public:
    std::vector<DataWriterBase*> m_vtDataWriters;

    BOOL            m_Stop;
    std::thread*    m_pWorkThread;
    CDBConnection   m_DBConnection;
    INT32           m_nErrorCount;
    INT32           m_nSaveTime;
    INT32           m_nSharePageSize;
};

#endif //_DB_WRITER_MANAGER_H_