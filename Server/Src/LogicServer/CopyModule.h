#ifndef __COPY_MODULE_H__
#define __COPY_MODULE_H__
#include "ModuleBase.h"

struct CopyDataObject;
struct ChapterDataObject;

class BattleResultNty;
class CCopyModule : public CModuleBase
{
public:
	CCopyModule(CPlayerObject* pOwner);

	~CCopyModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL NotifyChange();

	BOOL CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue);

	VOID RegisterMessageHanler();

	BOOL OnMainCopyResult(BattleResultNty* pNty, INT32 nIndex);
public:

	CopyDataObject* GetCopyData(UINT32 dwCopyID);
	ChapterDataObject* GetChapter(UINT32 dwChapter, UINT32 dwCopyType);

public:
	std::map<UINT32, CopyDataObject*>m_mapCopyData;
	std::map<UINT32, ChapterDataObject*>m_mapChapterData;
};

#endif //__ROLE_MODULE_H__
