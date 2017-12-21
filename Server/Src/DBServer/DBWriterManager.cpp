#include "stdafx.h"
#include "DBWriterManager.h"

CDBWriterManager::CDBWriterManager()
{
	m_Stop = FALSE;
}

CDBWriterManager::~CDBWriterManager()
{

}

BOOL CDBWriterManager::Init()
{
	m_pRoleDataWriter		= new DataWriter<RoleDataObject>(ESD_ROLE, 1024);
	m_pGlobalDataWriter		= new DataWriter<GlobalDataObject>(ESD_GLOBAL, 1024);
	m_pBagDataWriter		= new DataWriter<BagDataObject>(ESD_BAG, 1024);
	m_pCopyDataWriter		= new DataWriter<CopyDataObject>(ESD_COPY, 1024);
	m_pEquipDataWriter		= new DataWriter<EquipDataObject>(ESD_EQUIP, 1024);
	m_pPetDataWriter		= new DataWriter<PetDataObject>(ESD_PET, 1024);
	m_pPartnerDataWriter	= new DataWriter<PartnerDataObject>(ESD_PARTNER, 1024);
	m_pGuildDataWriter		= new DataWriter<GuildDataObject>(ESD_GUILD, 1024);
	m_pMemberDataWriter		= new DataWriter<MemberDataObject>(ESD_GUILD_MEMBER, 1024);
	m_pTaskDataWriter		= new DataWriter<TaskDataObject>(ESD_TASK, 1024);
	m_pMountDataWriter		= new DataWriter<MountDataObject>(ESD_MOUNT, 1024);
	m_pMailDataWriter		= new DataWriter<MailDataObject>(ESD_MAIL, 1024);
	m_pGroupMailDataWriter	= new DataWriter<GroupMailDataObject>(ESD_GROUP_MAIL, 1024);
	m_pActivityDataWriter   = new DataWriter<ActivityDataObject>(ESD_ACTIVITY, 1024);
	m_pCounterDataWriter    = new DataWriter<CounterDataObject>(ESD_COUNTER, 1024);
	m_pFriendDataWriter    = new DataWriter<FriendDataObject>(ESD_FRIEND, 1024);

	std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_ip");
	UINT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_game_svr_port");
	std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_user");
	std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_pwd");
	std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_db_name");

	m_DBConnection.SetConnectParam(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort);

	m_hWorkThread = CommonThreadFunc::CreateThread(_DBWriteThread, this);

	return TRUE;
}

BOOL CDBWriterManager::Uninit()
{
	m_Stop = TRUE;

	CommonThreadFunc::WaitThreadExit(m_hWorkThread);

	return TRUE;
}

void CDBWriterManager::SaveDataToDB()
{
	m_pRoleDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pGlobalDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pBagDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pCopyDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pEquipDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pPetDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pPartnerDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pGuildDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pMemberDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pTaskDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pMountDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pMailDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pGroupMailDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pActivityDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pCounterDataWriter->SaveModifyToDB(&m_DBConnection);
	m_pFriendDataWriter->SaveModifyToDB(&m_DBConnection);
}

BOOL CDBWriterManager::IsStop()
{
	return m_Stop;
}

Th_RetName _DBWriteThread(void* pParam)
{
	CDBWriterManager* pDBWriterManager = (CDBWriterManager*)pParam;

	pDBWriterManager->m_DBConnection.Init();

	pDBWriterManager->m_DBConnection.Reconnect();

	while (!pDBWriterManager->IsStop())
	{
		if (!pDBWriterManager->m_DBConnection.Ping())
		{
			pDBWriterManager->m_DBConnection.Reconnect();
		}

		pDBWriterManager->SaveDataToDB();

		CommonFunc::Sleep(1); //休息10秒
	}

	pDBWriterManager->Uninit();
	CommonThreadFunc::ExitThread();

	return Th_RetValue;
}