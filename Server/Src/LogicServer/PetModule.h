#ifndef __PET_MODULE_H__
#define __PET_MODULE_H__
#include "ModuleBase.h"
#include "..\ServerData\PetData.h"

struct PetDataObject;
class CPetModule  : public CModuleBase
{
public:
	CPetModule(CPlayerObject *pOwner);

	~CPetModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL ReadFromLoginAck(DBRoleLoginAck &Ack);

public:
	std::map<UINT32, PetDataObject*>m_mapPetData;


};

#endif //__PET_MODULE_H__
