#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_
#include "Position.h"
#include "../Message/Msg_Move.pb.h"
#include "../ServerData/ServerDefine.h"
#include "SkillObject.h"
class CScene;
class ResultPlayer;
class CBuffObject;
class CSceneObject
{
public:
	CSceneObject(UINT64 uGuid, UINT32 dwActorID, UINT32 ObjType, UINT32 dwCamp, std::string strName);

	~CSceneObject();
public:
	BOOL			SendMsgProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata);
	BOOL			SendMsgRawData(UINT32 dwMsgID, const char* pdata, UINT32 dwLen);
	BOOL			SetConnectID(UINT32 dwProxyID, UINT32 dwClientID);
	BOOL			OnUpdate(UINT32 dwTick);
	BOOL            SaveNewObject(ObjectNewNty& Nty);

	//以下为对象的操作方法
public:
	UINT32          GetHp();
	UINT32          GetMp();
	UINT64          GetObjectGUID();
	UINT32			GetActorID();
	UINT32			GetObjType();
	UINT32          GetCamp();

	VOID            AddHp(UINT32 dwValue);
	VOID            SubHp(UINT32 dwValue);
	VOID            AddMp(UINT32 dwValue);
	VOID            SubMp(UINT32 dwValue);



	BOOL			IsConnected();

	BOOL            IsEnterCopy();
	VOID			SetEnterCopy();
	BOOL            IsDie();

	BOOL			SetPos(FLOAT x, FLOAT y, FLOAT z, FLOAT ft = 0);

	UINT32			GetLastSkillTime(UINT32 dwSkillID);
	BOOL			SetLastSkillTime(UINT32 dwSkillID, UINT32 dwTime);

	BOOL			SaveBattleResult(ResultPlayer* pResult);

public:
	//////////////////////////////////////////////////////////////////////////
	//buff的处理
	BOOL			AddBuff(UINT32 dwBuffID);
	BOOL			RemoveBuff(UINT32 dwBuffID);

	BOOL			UpdateBuff(UINT32 dwTick);
	std::map<UINT32, CBuffObject*> m_mapBuff;
	//////////////////////////////////////////////////////////////////////////


public:
	//对象的基本信息
	//////////////////////////////////////////////////////////
	UINT64          m_uGuid;						//实例ID, 对玩家是角色ID
	UINT32          m_dwActorID;					//对象ID
	UINT32          m_dwObjType;					//对象类型 玩家，宠物， NPC之类的
	std::string     m_strName;						//对象的名字
	INT32			m_dwCamp;						//阵营
	FLOAT			m_x, m_y, m_z, m_ft;			//对象坐标, 朝向
	UINT32          m_dwObjState;					//对象当前的状态
	UINT32          m_dwBuffState;					//对象的Buff状态
	INT32			m_dwLevel;						//等级
	INT32           m_Propertys[PROPERTY_NUM];		//15个属性的数值

	BOOL			m_bDataChange;					//数据发生改变

	UINT64          m_uHostGuid;					//主人的GUID
	UINT64          m_uControlerID;					//AI控制人的GUID
	UINT64          m_uSummonerID;					//招唤者的GUID

	BOOL			m_bIsCampCheck;					//是否影响阵营结算
	BOOL			m_bIsMonsCheck;					//是否影响刷怪(玩家阵营的都不影响, 宠物，招唤物, 配制的特定物)

	//////////////////////////////////////////////////////////////////////////
	//技能
	std::map<UINT32, UINT32> m_mapSkillTime;
	CSkillObject			 m_SkillObject;
	BOOL					 StartSkill(const ActionReqItem& Item);
	BOOL					 StartAction(const ActionReqItem& Item);
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////
	//对象的一些标记
	CScene*			m_pScene;   //所在场景
	BOOL            m_bEnter;   //玩家是否己经进入副本

	//对象的副本结算数据
	////////////////////////////////////////////////////////////////
	UINT32			m_dwResult;		//0:未结算，1:胜利，2: 失败 3: 平局
	UINT32			m_dwKill;		//击杀数
	UINT32          m_dwDeath;		//被杀次数
	UINT32			m_dwHeal;		//治疗值
	UINT32          m_dwDamage;		//总伤害值

	//对象的连接情况，仅玩家有效
	////////////////////////////////////////////////////////////////
	UINT32          m_dwProxyConnID;
	UINT32          m_dwClientConnID;
};


#endif //_SCENE_OBJECT_H_
