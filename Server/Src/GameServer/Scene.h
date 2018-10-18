#ifndef _SCENE_H_
#define _SCENE_H_
#include "GameObject/SceneObject.h"
#include "SceneLogic/SceneLogic_Base.h"
#include "../Message/Msg_Game.pb.h"
#include "GameObject/BulletObject.h"
class MonsterCreator;

class CScene
{
public:
	CScene();

	~CScene();

	BOOL	        DispatchPacket(NetPacket* pNetPack);

	BOOL			Init(UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCopyType, UINT32 dwPlayerNum, UINT64 uCreateKey);

	BOOL	        Uninit();

	UINT32	        GetCopyGuid();

	UINT32          GetCopyID();

	UINT32			GetCopyType();

	BOOL            OnUpdate( UINT64 uTick );

	BOOL			CreateSceneLogic(UINT32 dwCopyType);

	BOOL			DestroySceneLogic(UINT32 dwCopyType);

	BOOL            BroadNewObject(CSceneObject* pSceneObject);

	BOOL            BroadNewBullet(CBulletObject* pBulletObject);

	BOOL            BroadMessage(UINT32 dwMsgID, const google::protobuf::Message& pdata);

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
	BOOL			RemoveDeadObject();

	BOOL			UpdateAiController(UINT64 uFilterID);
	UINT64			SelectController(UINT64 uFilterID);

	//在单人PVE情况下，副本应该只有一个人
	CSceneObject*   GetOwnPlayer();

	//////////////////////////////////////////////////////////////////////////
	//创建角色相关对象
	CSceneObject*	CreatePlayer(const TransRoleData& roleData, UINT64 uHostID, UINT32 dwCamp );
	CSceneObject*	CreatePet(const TransPetData& petData, UINT64 uHostID, UINT32 dwCamp );
	CSceneObject*	CreatePartner(const TransPartnerData& partnerData, UINT64 uHostID, UINT32 dwCamp );

	//创建怪物对象
	CSceneObject*	CreateMonster( UINT32 dwActorID, UINT32 dwCamp, FLOAT x, FLOAT y, FLOAT z, FLOAT ft);

	//创建召唤对象
	CSceneObject*	CreateSummon( UINT32 dwActorID, UINT64 uSummonerID, UINT32 dwCamp, FLOAT x, FLOAT y, FLOAT z, FLOAT ft);

	//创建子弹对象
	CBulletObject*	CreateBullet(UINT32 dwBulletID, UINT32 dwType, FLOAT Angle, FLOAT Fix, FLOAT Muti);

	BOOL			IsCampAllDie(UINT32 dwCamp);  //用于结算，判断阵营是否全部死亡.
	BOOL			IsMonsterAllDie();            //用于判断下一波怪是否需要刷出.

	//解析场景配制文件
	BOOL			ReadSceneXml();
public:
	BOOL			IsFinished();
	VOID			SetFinished();
	BOOL            IsAllDataReady();
	BOOL			IsAllLoginReady();
	UINT64			GetStartTime();
	UINT64			GetCreateTime();
	UINT64			GetTotalTime();
	UINT64          GetLastTick();
	BOOL			SetLastTick(UINT64 uTick);
	UINT64			GenNewGuid();

public:
	UINT32			m_dwPlayerNum;  //副本需要进入的人数
	UINT32			m_dwLoginNum;   //登录副本的人数
	UINT32			m_dwCopyGuid;	//当前副本实例ID
	UINT32			m_dwCopyID;		//当前副本TYPE
	UINT32			m_dwCopyType;	//逻辑类型
	UINT64          m_uCreateTime;  //副本创建时间
	UINT64			m_uStartTime;   //副本开始时间
	UINT64			m_uTotalTime;	//副本持续时间
	UINT64			m_dwLastTick;	//上一个tick时间
	UINT64			m_uMaxGuid;	    //场景里的最大GUID
	UINT64			m_uCreateKey;	//创建副本的专用参数
	BOOL			m_bFinished;	//副本是否完成
	SceneLogicBase*	m_pSceneLogic;
	MonsterCreator*	m_pMonsterCreator;

	std::map<UINT64, CSceneObject*>	 m_PlayerMap;		//玩家管理器
	std::map<UINT64, CSceneObject*>  m_MonsterMap;      //怪物管理器
	std::map<UINT64, CBulletObject*> m_BulletMap;		//子弹管理器
	//////////////////////////////////////////////////////////////////////////
	//攻击伤害计算

	BOOL ProcessActionItem(const ActionReqItem& Item);

	BOOL SendBattleResult();
	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgTransRoleDataReq(NetPacket* pNetPacket);
	BOOL OnMsgEnterSceneReq(NetPacket* pNetPacket);
	BOOL OnMsgLeaveSceneReq(NetPacket* pNetPacket);
	BOOL OnMsgRoleDisconnect(NetPacket* pNetPacket);
	BOOL OnMsgHeartBeatReq(NetPacket* pNetPacket);
	BOOL OnMsgUseHpBottleReq(NetPacket* pNetPacket);
	BOOL OnMsgUseMpBottleReq(NetPacket* pNetPacket);
	BOOL OnMsgBattleChatReq(NetPacket* pNetPacket);
	BOOL OnMsgObjectActionReq(NetPacket* pNetPacket);
	BOOL OnMsgSkillCastReq(NetPacket* pNetPacket);
	BOOL OnMsgObjectChangeNtf(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif //_MAP_H_


