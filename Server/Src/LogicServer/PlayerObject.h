#ifndef __WS_PLAYER_OBJECT_H__
#define __WS_PLAYER_OBJECT_H__
#include "GameDefine.h"
#include "Utility/AVLTree.h"
#include "Utility/Position.h"
#include "ModuleBase.h"

enum MouduleType
{
	MT_ROLE,
	MT_END
};

class CPlayerObject
{
public:
	CPlayerObject();

	~CPlayerObject();
	
	//  初始化玩家对象
	BOOL		Init();
	//  反初始化玩家对家
	BOOL		Uninit();

	virtual BOOL OnCreate(UINT64 u64RoleID);

	virtual BOOL OnDestroy(UINT64 u64RoleID);

	virtual BOOL OnLogin(UINT64 u64RoleID);

	virtual BOOL OnLogout(UINT64 u64RoleID);

	virtual BOOL OnNewDay();

	virtual BOOL OnLoadData(UINT64 u64RoleID);

	virtual BOOL DispatchPacket(NetPacket *pNetPack);

	BOOL    SendProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata);


	//模块函数
	BOOL			CreateAllModule();
	BOOL			DestroyAllModule();
	BOOL			OnModuleFnished();
	BOOL			IsAllModuleOK();
	BOOL			OnAllModuleOK();
	template <typename T>
	T*	GetModuleByType(int nType)
	{
		if(nType >= m_MoudleList.size())
		{
			return NULL;
		}

		return (T*)m_MoudleList.at(nType);
	}

public:
	UINT64			GetObjectID();
public:
	UINT64		m_u64ID;
	UINT32      m_dwProxyConnID;
	std::vector<CModuleBase*> m_MoudleList;
};


#endif //__WS_PLAYER_OBJECT_H__
