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

Th_RetName _DBWriteThread(void* pParam);

class CDBWriterManager
{

public:
	CDBWriterManager();
	~CDBWriterManager();

public:
	BOOL Init();

	BOOL Uninit();

	void SaveDataToDB();

	BOOL IsStop();
public:
	DataWriter<RoleDataObject>* m_pRoleDataWriter;
	DataWriter<GlobalDataObject>* m_pGlobalDataWriter;
	DataWriter<BagDataObject>* m_pBagDataWriter;
	DataWriter<CopyDataObject>* m_pCopyDataWriter;
	DataWriter<ChapterDataObject>* m_pChapterDataWriter;
	DataWriter<EquipDataObject>* m_pEquipDataWriter;
	DataWriter<GemDataObject>* m_pGemDataWriter;
	DataWriter<PetDataObject>* m_pPetDataWriter;
	DataWriter<PartnerDataObject>* m_pPartnerDataWriter;
	DataWriter<GuildDataObject>* m_pGuildDataWriter;
	DataWriter<MemberDataObject>* m_pMemberDataWriter;
	DataWriter<TaskDataObject>* m_pTaskDataWriter;
	DataWriter<MountDataObject>* m_pMountDataWriter;
	DataWriter<MailDataObject>* m_pMailDataWriter;
	DataWriter<GroupMailDataObject>* m_pGroupMailDataWriter;
	DataWriter<ActivityDataObject>* m_pActivityDataWriter;
	DataWriter<CounterDataObject>* m_pCounterDataWriter;
	DataWriter<FriendDataObject>* m_pFriendDataWriter;
	DataWriter<SkillDataObject>* m_pSkillDataWriter;
	BOOL			m_Stop;
	THANDLE			m_hWorkThread;
	CDBConnection   m_DBConnection;
};

#endif //_DB_WRITER_MANAGER_H_