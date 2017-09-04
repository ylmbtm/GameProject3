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
	m_pRoleDataWriter		= new DataWriter<RoleDataObject>("Role");
	m_pGlobalDataWriter		= new DataWriter<GlobalDataObject>("Global");
	m_pBagDataWriter		= new DataWriter<BagDataObject>("Bag");
	m_pCopyDataWriter		= new DataWriter<CopyDataObject>("Copy");
	m_pEquipDataWriter		= new DataWriter<EquipDataObject>("Equip");
	m_pPetDataWriter		= new DataWriter<PetDataObject>("Pet");
	m_pPartnerDataWriter	= new DataWriter<PartnerDataObject>("Partner");
	m_pGuildDataWriter		= new DataWriter<GuildDataObject>("Guild");
	m_pMemberDataWriter		= new DataWriter<MemberDataObject>("GuildMember");
	m_pTaskDataWriter		= new DataWriter<TaskDataObject>("Task");
	m_pMountDataWriter		= new DataWriter<MountDataObject>("Mount");
	m_pMailDataWriter		= new DataWriter<MailDataObject>("Mail");
	m_pGroupMailDataWriter	= new DataWriter<GroupMailDataObject>("GroupMail");
	m_pActivityDataWriter   = new DataWriter<ActivityDataObject>("Activity");
	m_pCounterDataWriter    = new DataWriter<CounterDataObject>("Counter");
	m_pFriendDataWriter    = new DataWriter<FriendDataObject>("Friend");
	m_hWorkThread = CommonThreadFunc::CreateThread(_DBWriteThread, this);

	return TRUE;
}

BOOL CDBWriterManager::Uninit()
{
	m_Stop = TRUE;

	CommonThreadFunc::WaitThreadExit(m_hWorkThread);

	return TRUE;
}

void CDBWriterManager::WriteWork()
{
	m_pRoleDataWriter->SaveModifyToDB(&m_DBManager);
	m_pGlobalDataWriter->SaveModifyToDB(&m_DBManager);
	m_pBagDataWriter->SaveModifyToDB(&m_DBManager);
	m_pCopyDataWriter->SaveModifyToDB(&m_DBManager);
	m_pEquipDataWriter->SaveModifyToDB(&m_DBManager);
	m_pPetDataWriter->SaveModifyToDB(&m_DBManager);
	m_pPartnerDataWriter->SaveModifyToDB(&m_DBManager);
	m_pGuildDataWriter->SaveModifyToDB(&m_DBManager);
	m_pMemberDataWriter->SaveModifyToDB(&m_DBManager);
	m_pTaskDataWriter->SaveModifyToDB(&m_DBManager);
	m_pMountDataWriter->SaveModifyToDB(&m_DBManager);
	m_pMailDataWriter->SaveModifyToDB(&m_DBManager);
	m_pGroupMailDataWriter->SaveModifyToDB(&m_DBManager);
	m_pActivityDataWriter->SaveModifyToDB(&m_DBManager);
	m_pCounterDataWriter->SaveModifyToDB(&m_DBManager);
	m_pFriendDataWriter->SaveModifyToDB(&m_DBManager);
}

BOOL CDBWriterManager::IsStop()
{
	return m_Stop;
}

Th_RetName _DBWriteThread(void* pParam)
{
	CDBWriterManager* pDBWriterManager = (CDBWriterManager*)pParam;

	pDBWriterManager->m_DBManager.Init();

	while(!pDBWriterManager->IsStop())
	{
		pDBWriterManager->WriteWork();

		CommonThreadFunc::Sleep(1); //ÐÝÏ¢10Ãë
	}
	pDBWriterManager->Uninit();
	CommonThreadFunc::ExitThread();

	return Th_RetValue;
}
