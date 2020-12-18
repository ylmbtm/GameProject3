#ifndef __LOGIN_CLIENT_MGR_H__
#define __LOGIN_CLIENT_MGR_H__
#include "AVLTree.h"


enum EClientStatue
{
	ECS_NONE            = 0,   //未知
	ECS_VER_CHECKED     = 1,   //己验证版本
	ECS_PSD_CHECKED     = 2,   //账号密码验证通过
};

class  CLoginClient
{
public:
	CLoginClient();

	~CLoginClient();

public:
	VOID	SetConnID(UINT32 dwConnID);

	UINT32  GetConnID();

	EClientStatue GetClientStatue();
public:

	UINT32          m_dwConnID;

	EClientStatue  m_ClientStatue;
};

class CLoginClientMgr : public AVLTree<UINT32, CLoginClient>
{
private:
	CLoginClientMgr(void);
	~CLoginClientMgr(void);

public:
	static CLoginClientMgr* GetInstancePtr();

	CLoginClient*	GetByConnID(UINT32 dwConnID);

	BOOL			RemoveByConnID(UINT32 dwConnID);

	CLoginClient*   CreateLoginClient(UINT32 dwConnID);

	BOOL            CheckClientMessage(UINT32 dwConnID, UINT32 dwMsgID);

	BOOL            OnCloseConnect(UINT32 dwConnID);
};

#endif /*__LOGIN_CLIENT_MGR_H__*/