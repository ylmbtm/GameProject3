#ifndef _SCENE_H_
#define _SCENE_H_
#include "GameObject/SceneObject.h"
#include "SceneLogic/SceneLogic_Base.h"
#include "../Message/Msg_Game.pb.h"

class MonsterCreator;

class CScene
{
public:
	CScene();

	~CScene();

	BOOL	        DispatchPacket(NetPacket* pNetPack);

	BOOL			Init(UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCopyType, UINT32 dwPlayerNum);

	BOOL	        Uninit();

	UINT32	        GetCopyGuid();

	UINT32          GetCopyID();

	UINT32			GetCopyType();

	BOOL            OnUpdate( UINT32 dwTick );

	BOOL			CreateSceneLogic(UINT32 dwCopyType);

	BOOL			DestroySceneLogic(UINT32 dwCopyType);

	BOOL            BroadNewObject(CSceneObject* pSceneObject);

	BOOL            BroadRemoveObject(CSceneObject* pSceneObject);

	BOOL	        SendAllNewObjectToPlayer(CSceneObject* pSceneObject);

	BOOL			BroadDieNotify(UINT64 uObjectID);

	BOOL            SyncObjectState();
	INT32	        GetPlayerCount();
	INT32			GetConnectCount();



	CSceneObject*   GetPlayer(UINT64 uID);
	BOOL            AddPlayer(CSceneObject* pSceneObject);
	VOID		    DeletePlayer(UINT64 uID);

	BOOL			AddMonster(CSceneObject* pSceneObject);
	VOID		    DeleteMonster(UINT64 uID);

	CSceneObject*   GetSceneObject(UINT64 uID);

	BOOL			UpdateAiController(UINT64 uFilterID);
	UINT64			SelectController(UINT64 uFilterID);

	//在单人PVE情况下，副本应该只有一个人
	CSceneObject*   GetOwnPlayer();

	//////////////////////////////////////////////////////////////////////////
	//创建角色相关对象
	BOOL            CreatePlayer(const TransRoleData& roleData, UINT64 uHostID, UINT32 dwCamp );
	BOOL            CreatePet(const TransPetData& petData, UINT64 uHostID, UINT32 dwCamp );
	BOOL            CreatePartner(const TransPartnerData& partnerData, UINT64 uHostID, UINT32 dwCamp );

	//创建怪物对象
	BOOL            CreateMonster( UINT32 dwActorID, UINT32 dwCamp, FLOAT x, FLOAT y, FLOAT z, FLOAT ft);


	BOOL            CreateSummon( UINT32 dwActorID, UINT64 uSummonerID, UINT32 dwCamp, FLOAT x, FLOAT y, FLOAT z, FLOAT ft);
	BOOL			IsCampAllDie(UINT32 dwCamp);  //用于结算，判断阵营是否全部死亡.
	BOOL			IsMonsterAllDie();            //用于判断下一波怪是否需要刷出.

	//解析场景配制文件
	BOOL			ReadSceneXml();
public:
	BOOL			IsFinished();
	BOOL            IsAllDataReady();
	BOOL			IsAllLoginReady();
	UINT32			GetStartTime();
	UINT32			GetCreateTime();
	UINT32          GetLastTick();
	BOOL			SetLastTick(UINT32 dwTick);
	UINT64			GenNewGuid();

protected:
	UINT32							m_dwPlayerNum;  //副本需要进入的人数
	UINT32							m_dwLoginNum;   //登录副本的人数
	UINT32							m_dwCopyGuid;	//当前副本实例ID
	UINT32							m_dwCopyID;		//当前副本TYPE
	UINT32							m_dwCopyType;	//逻辑类型
	UINT32                          m_dwCreateTime; //副本创建时间
	UINT32							m_dwStartTime;  //副本开始时间
	UINT32							m_dwLastTick;	//上一个tick时间
	UINT64							m_uMaxGuid;	//场景里的最大GUID

	SceneLogicBase*					m_pSceneLogic;
	MonsterCreator*					m_pMonsterCreator;

	std::map<UINT64, CSceneObject*>	 m_PlayerMap;		//玩家管理器
	std::map<UINT64, CSceneObject*>  m_MonsterMap;      //怪物管理器

	ObjectActionNty                  m_ObjectActionNty;

	//////////////////////////////////////////////////////////////////////////
	//攻击伤害计算

	BOOL ProcessActionItem(const ActionItem& Item);
	BOOL SkillFight(CSceneObject* pAttacker, UINT32 dwSkillID, CSceneObject* pDefender);

	BOOL SendBattleResult();
	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgTransRoleDataReq(NetPacket* pNetPacket);
	BOOL OnMsgEnterSceneReq(NetPacket* pNetPacket);
	BOOL OnMsgLeaveSceneReq(NetPacket* pNetPacket);
	BOOL OnMsgObjectActionReq(NetPacket* pNetPacket);
	BOOL OnMsgRoleDisconnect(NetPacket* pNetPacket);
	BOOL OnMsgHeartBeatReq(NetPacket* pNetPacket);
	BOOL OnMsgUseHpBottleReq(NetPacket* pNetPacket);
	BOOL OnMsgUseMpBottleReq(NetPacket* pNetPacket);
	BOOL OnMsgBattleChatReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif //_MAP_H_


