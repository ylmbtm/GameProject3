#ifndef __ROLE_MODULE_H__
#define __ROLE_MODULE_H__
#include "ModuleBase.h"
class CRoleModule : public CModuleBase
{
public:
	CRoleModule();

	~CRoleModule();

public:
	virtual BOOL OnCreate();

	virtual BOOL OnDestroy();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL OnNewDay();

	virtual BOOL OnLoadData();

public:
	UINT64 m_u64ID;  //角色ID
	std::string m_strName;
	UINT64 m_u64AccountID; //账号ID
	UINT32 m_RoleType; //角色类型
	UINT32 m_dwLevel;
	UINT64 m_Money[10];
	UINT64 m_dwExp;
	UINT32 m_dwLangID; //语言ID
};

#endif //__ROLE_MODULE_H__
