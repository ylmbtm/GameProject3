#ifndef _SCENE_H_
#define _SCENE_H_
#include "GameObject/SceneObject.h"
#include "SceneLogic/SceneLogic_Base.h"

class MonsterCreator;

class CScene
{
public:
	CScene();

	~CScene();

	BOOL	        DispatchPacket(NetPacket *pNetPack);

	BOOL	        Init(UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwLogicType,UINT32 dwPlayerNum);

	BOOL	        Uninit();
	
	UINT32	        GetCopyID();

	UINT32          GetCopyType();

	BOOL            OnUpdate( UINT32 dwTick );

	BOOL	        CreateSceneLogic(UINT32 dwLogicType);

	BOOL            DestroySceneLogic(UINT32 dwLogicType);

    BOOL            BroadNewObject(CSceneObject *pSceneObject);

    BOOL            BroadRemoveObject(CSceneObject *pSceneObject);

	BOOL	        SendAllNewObjectToPlayer(CSceneObject *pSceneObject);

    BOOL            SyncObjectState();
	BOOL	        GetPlayerCount();

    CSceneObject*   GetPlayer(UINT64 uID);
    BOOL            AddPlayer(CSceneObject *pSceneObject);
	VOID		    DeletePlayer(UINT64 uID);
	CSceneObject*   GetSceneObject(UINT64 uID);

    //在单人PVE情况下，副本应该只有一个人
    CSceneObject*   GetOwnPlayer();

	//////////////////////////////////////////////////////////////////////////
	//创建对象
    BOOL            GenMonster( UINT32 dwActorID, UINT32 dwCamp, FLOAT x, FLOAT y);

	//解析场景配制文件
	BOOL			ReadSceneXml();
public:
	BOOL			IsFinished();
    BOOL            IsAllDataReady();
	BOOL			IsAllLoginReady();
	UINT32			GetStartTime();
	UINT32			GetCreateTime();

protected:
	UINT32							m_dwPlayerNum;  //副本需要进入的人数
	UINT32							m_dwLoginNum;   //登录副本的人数
	UINT32							m_dwCopyID;		//当前副本实例ID
	UINT32							m_dwCopyType;   //当前副本TYPE
	UINT32							m_dwLogicType;  //逻辑类型
    UINT32                          m_dwCreateTime; //副本创建时间
	UINT32							m_dwStartTime;   //副本开始时间
	UINT32							m_dwLastTick;
	UINT32							m_dwMaxGuid;

	SceneLogicBase					*m_pSceneLogic;
	MonsterCreator					*m_pMonsterCreator;

	std::map<UINT64, CSceneObject*>	 m_PlayerMap;		//玩家管理器
    std::map<UINT64, CSceneObject*>  m_MonsterMap;      //怪物管理器
   
    ObjectActionNty                  m_ObjectActionNty;

    //////////////////////////////////////////////////////////////////////////
    //攻击伤害计算
   
    BOOL ProcessActionItem(const ActionItem &Item);
    BOOL SkillFight(CSceneObject *pAttacker, UINT32 dwSkillID, CSceneObject *pDefender);
	
	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgTransRoleDataReq(NetPacket *pNetPacket);
	BOOL OnMsgEnterSceneReq(NetPacket *pNetPacket);
	BOOL OnMsgLeaveSceneReq(NetPacket *pNetPacket);
    BOOL OnMsgObjectActionReq(NetPacket *pNetPacket);
    BOOL OnMsgRoleDisconnect(NetPacket *pNetPacket);
    //*********************消息处理定义结束******************************
};

#endif //_MAP_H_


