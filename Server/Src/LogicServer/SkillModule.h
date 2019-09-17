#ifndef __SKILL_MODULE_H__
#define __SKILL_MODULE_H__
#include "ModuleBase.h"
#include "../ServerData/SkillData.h"
struct SkillDataObject;
class CSkillModule  : public CModuleBase
{
public:
	CSkillModule(CPlayerObject* pOwner);

	~CSkillModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	VOID RegisterMessageHanler();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL NotifyChange();

	SkillDataObject* GetSkillData(UINT32 dwSkillID);

public:
	//*********************消息处理定义开始******************************

	//*********************消息处理定义结束******************************

public:
	std::map<UINT32, SkillDataObject*> m_mapSkillData;

};

#endif //__COUNTER_MODULE_H__
