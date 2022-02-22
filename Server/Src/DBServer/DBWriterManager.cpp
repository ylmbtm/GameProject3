#include "stdafx.h"
#include "DBWriterManager.h"

CDBWriterManager::CDBWriterManager()
{
    m_Stop = FALSE;
    m_nErrorCount = 0;
    m_nSaveTime = 30;
}

CDBWriterManager::~CDBWriterManager()
{
    m_Stop = FALSE;
    m_nErrorCount = 0;
    m_nSaveTime = 30;
}

BOOL CDBWriterManager::Init()
{
    m_nSharePageSize = CConfigFile::GetInstancePtr()->GetIntValue("share_page_size");
    if (m_nSharePageSize <= 1)
    {
        m_nSharePageSize = 1024;
    }

    m_nSaveTime = CConfigFile::GetInstancePtr()->GetIntValue("db_save_time");
    if (m_nSaveTime <= 0)
    {
        m_nSaveTime = 30;
    }

    INT32 nAreaID = CConfigFile::GetInstancePtr()->GetIntValue("areaid");
    ERROR_RETURN_FALSE(nAreaID > 0);

    m_vtDataWriters.assign(ESD_END, NULL);
    m_vtDataWriters[ESD_ROLE]           = new DataWriter<RoleDataObject>(ESD_ROLE, m_nSharePageSize);
    m_vtDataWriters[ESD_GLOBAL]         = new DataWriter<GlobalDataObject>(ESD_GLOBAL, m_nSharePageSize);
    m_vtDataWriters[ESD_BAG]            = new DataWriter<BagDataObject>(ESD_BAG, m_nSharePageSize);
    m_vtDataWriters[ESD_COPY]           = new DataWriter<CopyDataObject>(ESD_COPY, m_nSharePageSize);
    m_vtDataWriters[ESD_CHAPTER]        = new DataWriter<ChapterDataObject>(ESD_CHAPTER, m_nSharePageSize);
    m_vtDataWriters[ESD_EQUIP]          = new DataWriter<EquipDataObject>(ESD_EQUIP, m_nSharePageSize);
    m_vtDataWriters[ESD_GEM]            = new DataWriter<GemDataObject>(ESD_GEM, m_nSharePageSize);
    m_vtDataWriters[ESD_PET]            = new DataWriter<PetDataObject>(ESD_PET, m_nSharePageSize);
    m_vtDataWriters[ESD_PARTNER]        = new DataWriter<PartnerDataObject>(ESD_PARTNER, m_nSharePageSize);
    m_vtDataWriters[ESD_GUILD]          = new DataWriter<GuildDataObject>(ESD_GUILD, m_nSharePageSize);
    m_vtDataWriters[ESD_GUILD_MEMBER]   = new DataWriter<MemberDataObject>(ESD_GUILD_MEMBER, m_nSharePageSize);
    m_vtDataWriters[ESD_TASK]           = new DataWriter<TaskDataObject>(ESD_TASK, m_nSharePageSize);
    m_vtDataWriters[ESD_MOUNT]          = new DataWriter<MountDataObject>(ESD_MOUNT, m_nSharePageSize);
    m_vtDataWriters[ESD_MAIL]           = new DataWriter<MailDataObject>(ESD_MAIL, m_nSharePageSize);
    m_vtDataWriters[ESD_OFFDATA]        = new DataWriter<OffDataObject>(ESD_OFFDATA, m_nSharePageSize);
    m_vtDataWriters[ESD_GROUP_MAIL]     = new DataWriter<GroupMailDataObject>(ESD_GROUP_MAIL, m_nSharePageSize);
    m_vtDataWriters[ESD_ACTIVITY]       = new DataWriter<ActivityDataObject>(ESD_ACTIVITY, m_nSharePageSize);
    m_vtDataWriters[ESD_COUNTER]        = new DataWriter<CounterDataObject>(ESD_COUNTER, m_nSharePageSize);
    m_vtDataWriters[ESD_FRIEND]         = new DataWriter<FriendDataObject>(ESD_FRIEND, m_nSharePageSize);
    m_vtDataWriters[ESD_SKILL]          = new DataWriter<SkillDataObject>(ESD_SKILL, m_nSharePageSize);
    m_vtDataWriters[ESD_PAYMENT]        = new DataWriter<PayDataObject>(ESD_PAYMENT, m_nSharePageSize);
    m_vtDataWriters[ESD_SEAL_ROLE]      = new DataWriter<SealDataObject>(ESD_SEAL_ROLE, m_nSharePageSize);

    for (int i = ESD_BEGIN + 1; i < ESD_END; i++)
    {
        if (m_vtDataWriters[i] == NULL)
        {
            CLog::GetInstancePtr()->LogError("CDBWriterManager::Init Error: ModuleID:[%d] Is NULL!", i);
            return FALSE;
        }
    }

    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_ip");
    INT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_game_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_user");
    std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_pwd");
    std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_db_name");

    m_DBConnection.SetConnectParam(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort);

    m_pWorkThread = new std::thread(&CDBWriterManager::DBWriteThread, this);

    return TRUE;
}

BOOL CDBWriterManager::Uninit()
{
    m_Stop = TRUE;

    if (m_pWorkThread != NULL)
    {
        m_pWorkThread->join();

        delete m_pWorkThread;

        m_pWorkThread = NULL;
    }

    for (int i = ESD_BEGIN + 1; i < ESD_END; i++)
    {
        ERROR_TO_CONTINUE(m_vtDataWriters[i] != NULL);

        delete m_vtDataWriters[i];

        m_vtDataWriters[i] = NULL;
    }

    m_vtDataWriters.clear();

    return TRUE;
}

BOOL CDBWriterManager::WriteDataToDB()
{
    BOOL bHasWrite = FALSE;
    INT32 nErrorCount = 0;
    for (int i = ESD_BEGIN + 1; i < ESD_END; i++)
    {
        ERROR_TO_CONTINUE(m_vtDataWriters[i] != NULL);

        if (m_vtDataWriters[i]->SaveModifyToDB(&m_DBConnection))
        {
            bHasWrite = TRUE;
        }

        nErrorCount += m_vtDataWriters[i]->GetErrorCount();
    }

    m_nErrorCount = nErrorCount;

    return bHasWrite;
}

BOOL CDBWriterManager::IsStop()
{
    return m_Stop;
}

BOOL CDBWriterManager::Update()
{
    static INT32 nLastErrorCount = 0;

    if (nLastErrorCount == m_nErrorCount)
    {
        return TRUE;
    }

    nLastErrorCount = m_nErrorCount;

    //Msg_DbErrorCountNty Nty;

    //Nty.set_errorcount(m_nCurErrorCount);

    //ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_DB_WRITE_ERROR_NTY, 0, 0, Nty);

    return TRUE;
}

void CDBWriterManager::DBWriteThread()
{
    if (!m_DBConnection.Init())
    {
        return ;
    }

    while (TRUE)
    {
        if (!m_DBConnection.Ping())
        {
            if (!m_DBConnection.Reconnect())
            {
                CommonFunc::Sleep(1000);
                continue;
            }
        }

        if (IsStop())
        {
            CLog::GetInstancePtr()->LogError("开始退出将所有己修改的数据写入数据库.....");
            BOOL bHasWrite = WriteDataToDB();
            if (!bHasWrite)
            {
                CLog::GetInstancePtr()->LogError("所有己修改的数据己写入数据库.");
                break;
            }

            continue;
        }
        else
        {
            WriteDataToDB();
        }

        CommonFunc::Sleep(60000); //休息10秒
    }
    m_DBConnection.Close();
    m_DBConnection.Uninit();

    return ;
}