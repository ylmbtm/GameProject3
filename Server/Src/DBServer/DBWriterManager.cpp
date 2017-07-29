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
	m_pRoleDataWriter = new DataWriter<RoleDataObject>("Role");
	m_pGlobalDataWriter = new DataWriter<GlobalDataObject>("Global");
	m_pBagDataWriter = new DataWriter<BagDataObject>("Bag");
	m_pCopyDataWriter = new DataWriter<CopyDataObject>("Copy");

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
	m_pRoleDataWriter->saveModifyToDB(&m_DBManager);
	m_pGlobalDataWriter->saveModifyToDB(&m_DBManager);
	m_pBagDataWriter->saveModifyToDB(&m_DBManager);
	m_pCopyDataWriter->saveModifyToDB(&m_DBManager);
}

BOOL CDBWriterManager::IsStop()
{
	return m_Stop;
}

Th_RetName _DBWriteThread(void *pParam)
{
	CDBWriterManager *pDBWriterManager = (CDBWriterManager *)pParam;

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
