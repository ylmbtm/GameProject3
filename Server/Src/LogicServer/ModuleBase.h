#ifndef __CMODULE_BASE_H__
#define __CMODULE_BASE_H__
#include "GameDefine.h"
#include "Utility/AVLTree.h"
#include "Utility/Position.h"

class CModuleBase
{
public:
	CModuleBase();

	~CModuleBase();

	virtual BOOL OnCreate(UINT64 u64RoleID)
	{
		return TRUE;
	}

	virtual BOOL OnDestroy(UINT64 u64RoleID)
	{
		return TRUE;
	}

	virtual BOOL OnLogin(UINT64 u64RoleID)
	{
		return TRUE;
	}

	virtual BOOL OnLogout(UINT64 u64RoleID)
	{
		return TRUE;
	}

	virtual BOOL OnNewDay()
	{
		return TRUE;
	}

	virtual BOOL OnLoadData(UINT64 u64RoleID)
	{
		return TRUE;
	}

	virtual BOOL IsDataOK()
	{
		return m_bIsDataOK;
	}

	virtual BOOL SetDataOK(BOOL bOK)
	{
		m_bIsDataOK = bOK;
		return TRUE;
	}

	BOOL SetOwner(CPlayerObject *pOwner)
	{
		m_pOwnPlayer = pOwner;
	}

	CPlayerObject* GetOwner()
	{
		return m_pOwnPlayer;
	}

public:
	CPlayerObject *m_pOwnPlayer;
	BOOL		   m_bIsDataOK;
};


#endif //__CMODULE_BASE_H__
