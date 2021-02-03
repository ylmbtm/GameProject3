#include "stdafx.h"
#include "DBWriterManager.h"

CDBWriterManager::CDBWriterManager()
{
	m_Stop = FALSE;
	m_nCurErrorCount = 0;
	m_dwSaveTime = 30;
}

CDBWriterManager::~CDBWriterManager()
{
	m_Stop = FALSE;
	m_nCurErrorCount = 0;
	m_dwSaveTime = 30;
}

BOOL CDBWriterManager::Init()
{
	m_dwSharePageSize = CConfigFile::GetInstancePtr()->GetIntValue("share_page_size");
	if (m_dwSharePageSize <= 1)
	{
		m_dwSharePageSize = 1024;
	}
	
	m_dwSaveTime = CConfigFile::GetInstancePtr()->GetIntValue("db_save_time");
	if (m_dwSaveTime <= 0)
	{
		m_dwSaveTime = 30;
	}

	UINT32 nAreaID = CConfigFile::GetInstancePtr()->GetIntValue("areaid");
	ERROR_RETURN_FALSE(nAreaID > 0);

	m_vtDataWriters.assign(ESD_END, NULL);
	m_vtDataWriters[ESD_ROLE]           = new DataWriter<RoleDataObject>(ESD_ROLE, 1024);
	m_vtDataWriters[ESD_GLOBAL]         = new DataWriter<GlobalDataObject>(ESD_GLOBAL, 1024);
	m_vtDataWriters[ESD_BAG]            = new DataWriter<BagDataObject>(ESD_BAG, 1024);
	m_vtDataWriters[ESD_COPY]           = new DataWriter<CopyDataObject>(ESD_COPY, 1024);
	m_vtDataWriters[ESD_CHAPTER]        = new DataWriter<ChapterDataObject>(ESD_CHAPTER, 1024);
	m_vtDataWriters[ESD_EQUIP]          = new DataWriter<EquipDataObject>(ESD_EQUIP, 1024);
	m_vtDataWriters[ESD_GEM]            = new DataWriter<GemDataObject>(ESD_GEM, 1024);
	m_vtDataWriters[ESD_PET]            = new DataWriter<PetDataObject>(ESD_PET, 1024);
	m_vtDataWriters[ESD_PARTNER]        = new DataWriter<PartnerDataObject>(ESD_PARTNER, 1024);
	m_vtDataWriters[ESD_GUILD]          = new DataWriter<GuildDataObject>(ESD_GUILD, 1024);
	m_vtDataWriters[ESD_GUILD_MEMBER]   = new DataWriter<MemberDataObject>(ESD_GUILD_MEMBER, 1024);
	m_vtDataWriters[ESD_TASK]           = new DataWriter<TaskDataObject>(ESD_TASK, 1024);
	m_vtDataWriters[ESD_MOUNT]          = new DataWriter<MountDataObject>(ESD_MOUNT, 1024);
	m_vtDataWriters[ESD_MAIL]           = new DataWriter<MailDataObject>(ESD_MAIL, 1024);
	m_vtDataWriters[ESD_OFFDATA]        = new DataWriter<OffDataObject>(ESD_OFFDATA, 1024);
	m_vtDataWriters[ESD_GROUP_MAIL]     = new DataWriter<GroupMailDataObject>(ESD_GROUP_MAIL, 1024);
	m_vtDataWriters[ESD_ACTIVITY]       = new DataWriter<ActivityDataObject>(ESD_ACTIVITY, 1024);
	m_vtDataWriters[ESD_COUNTER]        = new DataWriter<CounterDataObject>(ESD_COUNTER, 1024);
	m_vtDataWriters[ESD_FRIEND]         = new DataWriter<FriendDataObject>(ESD_FRIEND, 1024);
	m_vtDataWriters[ESD_SKILL]          = new DataWriter<SkillDataObject>(ESD_SKILL, 1024);
	m_vtDataWriters[ESD_PAYMENT]        = new DataWriter<PayDataObject>(ESD_PAYMENT, 1024);
	m_vtDataWriters[ESD_SEAL_ROLE]      = new DataWriter<SealDataObject>(ESD_SEAL_ROLE, 1024);

	for (int i = ESD_BEGIN + 1; i < ESD_END; i++)
	{
		if (m_vtDataWriters[i] == NULL)
		{
			CLog::GetInstancePtr()->LogError("CDBWriterManager::Init Error: ModuleID:[%d] Is NULL!", i);
			return FALSE;
		}
	}

	std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_ip");
	UINT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_game_svr_port");
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


	return TRUE;
}

BOOL CDBWriterManager::WriteDataToDB()
{
	BOOL bHasWrite = FALSE;
	UINT32 nErrorCount = 0;
	for (int i = ESD_BEGIN + 1; i < ESD_END; i++)
	{
		ERROR_TO_CONTINUE(m_vtDataWriters[i] != NULL);

		if (m_vtDataWriters[i]->SaveModifyToDB(&m_DBConnection))
		{
			bHasWrite = TRUE;
		}

		nErrorCount += m_vtDataWriters[i]->GetErrorCount();
	}

	m_nCurErrorCount = nErrorCount;

	return bHasWrite;
}

BOOL CDBWriterManager::IsStop()
{
	return m_Stop;
}

BOOL CDBWriterManager::Update()
{
	static UINT32 nLastErrorCount = 0;

	if (nLastErrorCount == m_nCurErrorCount)
	{
		return TRUE;
	}

	nLastErrorCount = m_nCurErrorCount;

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

		BOOL bHasWrite = WriteDataToDB();
		if (!bHasWrite && IsStop())
		{
			break;
		}

		CommonFunc::Sleep(60000); //休息10秒
	}

	m_DBConnection.Uninit();

	return ;
}